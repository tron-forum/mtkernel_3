/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	ready_queue.h
 *	Ready Queue Operation Routine
 */

#ifndef _READY_QUEUE_
#define _READY_QUEUE_

#include "tstdlib.h"

/*
 * Definition of ready queue structure 
 *	In the ready queue, the task queue 'tskque' is provided per priority.
 *	The task TCB is registered onto queue with the applicable priority.
 *	For effective ready queue search, the bitmap area 'bitmap' is provided
 *	to indicate whether there are tasks in task queue per priority.
 *	
 *	Also, to search a task at the highest priority in the ready queue  
 *    	effectively, put the highest task priority in the 'top_priority' field.
 *	If the ready queue is empty, set the value in this field to NUM_TSKPRI. 
 *	In this case, to return '0' with referring 'tskque[top_priority]',
 *      there is 'null' field which is always '0'.
 *
 *	Multiple READY tasks with kernel lock do not exist at the same time.
 */

#define BITMAPSZ	( sizeof(UINT) * 8 )
#define NUM_BITMAP	( (NUM_TSKPRI + BITMAPSZ - 1) / BITMAPSZ )

typedef	struct ready_queue {
	INT	top_priority;		/* Highest priority in ready queue */
	QUEUE	tskque[NUM_TSKPRI];	/* Task queue per priority */
	TCB	*null;			/* When the ready queue is empty, */
	UINT	bitmap[NUM_BITMAP];	/* Bitmap area per priority */
	TCB	*klocktsk;	/* READY task with kernel lock */
} RDYQUE;

IMPORT RDYQUE	knl_ready_queue;

#if NUM_TSKPRI <= INT_BITWIDTH
Inline INT knl_ready_queue_calc_top_priority( UINT bitmap, INT pos )
{
	for ( ; pos < NUM_TSKPRI; pos++ ) {
		if ( bitmap & (1U << pos) ) {
			return pos;
		}
	}
	return NUM_TSKPRI;
}
#endif

/*
 * Ready queue initialization
 */
Inline void knl_ready_queue_initialize( RDYQUE *rq )
{
	INT	i;

	rq->top_priority = NUM_TSKPRI;
	for ( i = 0; i < NUM_TSKPRI; i++ ) {
		QueInit(&rq->tskque[i]);
	}
	rq->null = NULL;
	rq->klocktsk = NULL;
	knl_memset(rq->bitmap, 0, sizeof(rq->bitmap));
}

/*
 * Return the highest priority task in ready queue
 */
Inline TCB* knl_ready_queue_top( RDYQUE *rq )
{
	/* If there is a task at kernel lock, that is the highest priority task */
	if ( rq->klocktsk != NULL ) {
		return rq->klocktsk;
	}

	return (TCB*)rq->tskque[rq->top_priority].next;
}

/*
 * Return the priority of the highest priority task in the ready queue
 */
Inline INT knl_ready_queue_top_priority( const RDYQUE *rq )
{
	return rq->top_priority;
}

/*
 * Insert task in ready queue
 *	Insert it at the end of the same priority tasks with task priority 
 *	indicated with 'tcb'. Set the applicable bit in the bitmap area and 
 *	update 'top_priority' if necessary. When updating 'top_priority,' 
 *	return TRUE, otherwise FALSE.
 */
Inline BOOL knl_ready_queue_insert( RDYQUE *rq, TCB *tcb )
{
	INT	priority = tcb->priority;

	QueInsert(&tcb->tskque, &rq->tskque[priority]);
#if NUM_TSKPRI <= INT_BITWIDTH
	rq->bitmap[0] |= (1U << priority);
#else
	knl_bitset(rq->bitmap, priority);
#endif

	if ( tcb->klocked ) {
		rq->klocktsk = tcb;
	}

	if ( priority < rq->top_priority ) {
		rq->top_priority = priority;
		return TRUE;
	}
	return FALSE;
}

/*
 * Insert task at head in ready queue 
 */
Inline void knl_ready_queue_insert_top( RDYQUE *rq, TCB *tcb )
{
	INT	priority = tcb->priority;

	QueInsert(&tcb->tskque, rq->tskque[priority].next);
#if NUM_TSKPRI <= INT_BITWIDTH
	rq->bitmap[0] |= (1U << priority);
#else
	knl_bitset(rq->bitmap, priority);
#endif

	if ( tcb->klocked ) {
		rq->klocktsk = tcb;
	}

	if ( priority < rq->top_priority ) {
		rq->top_priority = priority;
	}
}

/*
 * Delete task from ready queue
 *	Take out TCB from the applicable priority task queue, and if the task 
 *	queue becomes empty, clear the applicable bit from the bitmap area.
 *	In addition, update 'top_priority' if the deleted task had the highest 
 *	priority. In such case, use the bitmap area to search the second
 *	highest priority task.
 */
Inline void knl_ready_queue_delete( RDYQUE *rq, TCB *tcb )
{
	INT	priority = tcb->priority;
#if NUM_TSKPRI > INT_BITWIDTH
	INT	i;
#endif

	if ( rq->klocktsk == tcb ) {
		rq->klocktsk = NULL;
	}

	QueRemove(&tcb->tskque);
	if ( tcb->klockwait ) {
		/* Delete from kernel lock wait queue */
		tcb->klockwait = FALSE;
		return;
	}
	if ( !isQueEmpty(&rq->tskque[priority]) ) {
		return;
	}

#if NUM_TSKPRI <= INT_BITWIDTH
	rq->bitmap[0] &= ~(1U << priority);
#else
	knl_bitclr(rq->bitmap, priority);
#endif
	if ( priority != rq->top_priority ) {
		return;
	}

#if NUM_TSKPRI <= INT_BITWIDTH
	rq->top_priority = knl_ready_queue_calc_top_priority(rq->bitmap[0], priority);
#else
	i = knl_bitsearch1(rq->bitmap, priority, NUM_TSKPRI - priority);
	if ( i >= 0 ) {
		rq->top_priority = priority + i;
	} else {
		rq->top_priority = NUM_TSKPRI;
	}
#endif
}

/*
 * Move the task, whose ready queue priority is 'priority', at head of
 * queue to the end of queue. Do nothing, if the queue is empty.
 */
Inline void knl_ready_queue_rotate( RDYQUE *rq, INT priority )
{
	QUEUE	*tskque = &rq->tskque[priority];
	TCB	*tcb;

	tcb = (TCB*)QueRemoveNext(tskque);
	if ( tcb != NULL ) {
		QueInsert((QUEUE*)tcb, tskque);
	}
}

/*
 * Put 'tcb' to the end of ready queue. 
 */
Inline TCB* knl_ready_queue_move_last( RDYQUE *rq, TCB *tcb )
{
	QUEUE	*tskque = &rq->tskque[tcb->priority];

	QueRemove(&tcb->tskque);
	QueInsert(&tcb->tskque, tskque);

	return (TCB*)tskque->next;	/* New task at head of queue */
}

#endif /* _READY_QUEUE_ */
