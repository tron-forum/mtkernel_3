/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	task.c
 *	Task Control
 */

#include "kernel.h"
#include "task.h"
#include "ready_queue.h"
#include "wait.h"
#include "check.h"

#include "../sysdepend/cpu_task.h"

/*
 * Task dispatch disable state
 */
Noinit(EXPORT INT	knl_dispatch_disabled);	/* DDS_XXX see task.h */

/*
 * Task execution control 
 */
Noinit(EXPORT TCB	*knl_ctxtsk);	/* Task in execution */
Noinit(EXPORT TCB	*knl_schedtsk);	/* Task which should be executed */

Noinit(EXPORT RDYQUE	knl_ready_queue);	/* Ready queue */

/*
 * Task control information
 */
Noinit(EXPORT TCB	knl_tcb_table[NUM_TSKID]);	/* Task control block */
Noinit(EXPORT QUEUE	knl_free_tcb);	/* FreeQue */

/*
 * TCB Initialization
 */
EXPORT ER knl_task_initialize( void )
{
	INT	i;
	TCB	*tcb;
	ID	tskid;

	/* Get system information */
	if ( NUM_TSKID < 1 ) {
		return E_SYS;
	}

	/* Initialize task execution control information */
	knl_ctxtsk = knl_schedtsk = NULL;
	knl_ready_queue_initialize(&knl_ready_queue);
	knl_dispatch_disabled = DDS_ENABLE;

	/* Register all TCBs onto FreeQue */
	QueInit(&knl_free_tcb);
	for ( tcb = knl_tcb_table, i = 0; i < NUM_TSKID; tcb++, i++ ) {
		tskid = ID_TSK(i);
		tcb->tskid = tskid;
		tcb->state = TS_NONEXIST;
#if USE_LEGACY_API && USE_RENDEZVOUS
		tcb->wrdvno = tskid;
#endif

		QueInsert(&tcb->tskque, &knl_free_tcb);
	}

	return E_OK;
}

/*
 * Prepare task execution.
 */
EXPORT void knl_make_dormant( TCB *tcb )
{
	/* Initialize variables which should be reset at DORMANT state */
	tcb->state	= TS_DORMANT;
	tcb->priority	= tcb->bpriority = tcb->ipriority;
	tcb->sysmode	= tcb->isysmode;
	tcb->wupcnt	= 0;
	tcb->suscnt	= 0;

	tcb->klockwait	= FALSE;
	tcb->klocked	= FALSE;

#if USE_DBGSPT && defined(USE_FUNC_TD_INF_TSK)
	tcb->stime	= 0;
	tcb->utime	= 0;
#endif

	tcb->wercd = NULL;

#if USE_MUTEX == 1
	tcb->mtxlist	= NULL;
#endif

	/* Set context to start task */
	knl_setup_context(tcb);
}

/* ------------------------------------------------------------------------ */

/*
 * Set task to READY state.
 *	Update the task state and insert in the ready queue. If necessary, 
 *	update 'knl_schedtsk' and request to start task dispatcher. 
 */
EXPORT void knl_make_ready( TCB *tcb )
{
	tcb->state = TS_READY;
	if ( knl_ready_queue_insert(&knl_ready_queue, tcb) ) {
		knl_schedtsk = tcb;
	}
}

/*
 * Set task to non-executable state.
 *	Delete the task from the ready queue.
 *	If the deleted task is 'knl_schedtsk', set 'knl_schedtsk' to the
 *	highest priority task in the ready queue. 
 *	'tcb' task must be READY.
 */
EXPORT void knl_make_non_ready( TCB *tcb )
{
	knl_ready_queue_delete(&knl_ready_queue, tcb);
	if ( knl_schedtsk == tcb ) {
		knl_schedtsk = knl_ready_queue_top(&knl_ready_queue);
	}
}

/*
 * Change task priority.
 */
EXPORT void knl_change_task_priority( TCB *tcb, INT priority )
{
	INT	oldpri;

	if ( tcb->state == TS_READY ) {
		/*
		 * When deleting a task from the ready queue, 
		 * a value in the 'priority' field in TCB is needed. 
		 * Therefore you need to delete the task from the
		 * ready queue before changing 'tcb->priority.'
		 */
		knl_ready_queue_delete(&knl_ready_queue, tcb);
		tcb->priority = (UB)priority;
		knl_ready_queue_insert(&knl_ready_queue, tcb);
		knl_reschedule();
	} else {
		oldpri = tcb->priority;
		tcb->priority = (UB)priority;

		/* If the hook routine at the task priority change is defined,
		   execute it */
		if ( (tcb->state & TS_WAIT) != 0 && tcb->wspec->chg_pri_hook) {
			(*tcb->wspec->chg_pri_hook)(tcb, oldpri);
		}
	}
}

/*
 * Rotate ready queue.
 */
EXPORT void knl_rotate_ready_queue( INT priority )
{
	knl_ready_queue_rotate(&knl_ready_queue, priority);
	knl_reschedule();
}

/*
 * Rotate the ready queue including the highest priority task.
 */
EXPORT void knl_rotate_ready_queue_run( void )
{
	if ( knl_schedtsk != NULL ) {
		knl_ready_queue_rotate(&knl_ready_queue,
				knl_ready_queue_top_priority(&knl_ready_queue));
		knl_reschedule();
	}
}

/* ------------------------------------------------------------------------ */
/*
 *	Debug support function
 */
#if USE_DBGSPT

#ifdef USE_FUNC_TD_RDY_QUE
/*
 * Refer ready queue
 */
SYSCALL INT td_rdy_que( PRI pri, ID list[], INT nent )
{
	QUEUE	*q, *tskque;
	INT	n = 0;

	CHECK_PRI(pri);

	BEGIN_DISABLE_INTERRUPT;
	tskque = &knl_ready_queue.tskque[int_priority(pri)];
	for ( q = tskque->next; q != tskque; q = q->next ) {
		if ( n++ < nent ) {
			*(list++) = ((TCB*)q)->tskid;
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_RDY_QUE */

#endif /* USE_DBGSPT */
