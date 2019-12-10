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
 *	task.h
 *	Task Definition
 */

#ifndef _TASK_
#define _TASK_

/*
 * Internal expression of task state
 *	Can check with 'state & TS_WAIT' whether the task is in the wait state.
 *	Can check with 'state & TS_SUSPEND' whether the task is in the forced 
 *	wait state.
 */
typedef enum {
	TS_NONEXIST	= 0,	/* Unregistered state */
	TS_READY	= 1,	/* RUN or READY state */
	TS_WAIT		= 2,	/* WAIT state */
	TS_SUSPEND	= 4,	/* SUSPEND state */
	TS_WAITSUS	= 6,	/* Both WAIT and SUSPEND state */
	TS_DORMANT	= 8	/* DORMANT state */
} TSTAT;

/*
 * If the task is alive ( except NON-EXISTENT,DORMANT ), return TRUE.
 */
Inline BOOL knl_task_alive( TSTAT state )
{
	return ( (state & (TS_READY|TS_WAIT|TS_SUSPEND)) != 0 );
}


/*
 * Task priority internal/external expression conversion macro
 */
#define int_priority(x)		( (INT)((x) - MIN_TSKPRI) )
#define ext_tskpri(x)		( (PRI)((x) + MIN_TSKPRI) )


/*
 * Task control information
 */
IMPORT TCB	knl_tcb_table[];	/* Task control block */
IMPORT QUEUE	knl_free_tcb;	/* FreeQue */

/*
 * Get TCB from task ID.
 */
#define get_tcb(id)		( &knl_tcb_table[INDEX_TSK(id)] )
#define get_tcb_self(id)	( ( (id) == TSK_SELF )? knl_ctxtsk: get_tcb(id) )

/*
 * Prepare task execution.
 */
IMPORT void knl_make_dormant( TCB *tcb );

/*
 * Make task executable.
 *	If the 'tcb' task priority is higher than the executed task,
 *	make it executable. If the priority is lower, connect the task to the 
 *	ready queue.
 */
IMPORT void knl_make_ready( TCB *tcb );

/*
 * Make task non-executable.
 *	Change the 'tcb' task state to be a non-executable state (wait state, 
 *	forced wait, or dormant state). When calling this function, the
 *	task must be executable. Change 'tcb->state' on the caller side
 *	after returning from this function.
 */
IMPORT void knl_make_non_ready( TCB *tcb );

/*
 * Change task priority.
 *	Change 'tcb' task priority to 'priority'.
 *	Then make the required task state transition occur.
 */
IMPORT void knl_change_task_priority( TCB *tcb, INT priority );

/*
 * Rotate ready queue.
 *	'rotate_ready_queue' rotates the priority ready queue at 'priority'.
 *	'rotate_ready_queue_run' rotates the ready queue including the highest 
 *	priority task in the ready queue.
 */
IMPORT void knl_rotate_ready_queue( INT priority );
IMPORT void knl_rotate_ready_queue_run( void );


#include "ready_queue.h"

/*
 * Reselect task to execute
 *	Set 'schedtsk' to the head task at the ready queue.
 */
Inline void knl_reschedule( void )
{
	TCB	*toptsk;

	toptsk = knl_ready_queue_top(&knl_ready_queue);
	if ( knl_schedtsk != toptsk ) {
		knl_schedtsk = toptsk;
	}
}

#endif /* _TASK_ */
