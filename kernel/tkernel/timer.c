/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06A
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	timer.c
 *	Timer Control
 */

#include "kernel.h"
#include "timer.h"
#include "../sysdepend/sys_timer.h"

/*
 * Current time (Software clock)
 *	'current_time' shows the total operation time since
 *	operating system Starts. 'real_time_ofs' shows difference
 *	between the current time and the operating system clock
 *	(current_time). Do not change 'current_time' when setting
 *	time by 'set_tim()'. Set 'real_time_ofs' with the time  	 
 *   	difference between 'current_time' and setup time.
 *	Therefore 'current_time' does not affect with time change
 *	and it increases simply.
 */
Noinit(EXPORT LSYSTIM	knl_current_time);	/* System operation time */
Noinit(EXPORT LSYSTIM	knl_real_time_ofs);	/* Actual time - System operation time */

/* 
 * Timer event queue
 */
Noinit(EXPORT QUEUE	knl_timer_queue);

/*
 * Start system timer
 */
EXPORT ER knl_timer_startup( void )
{
	knl_current_time = knl_real_time_ofs = uitoll(0);
	QueInit(&knl_timer_queue);

	/* Start timer interrupt */
	knl_start_hw_timer();

	return E_OK;
}

#if USE_SHUTDOWN
/*
 * Stop system timer
 */
EXPORT void knl_timer_shutdown( void )
{
	knl_terminate_hw_timer();
}
#endif /* USE_SHUTDOWN */


/*
 * Insert timer event to timer event queue
 */
LOCAL void knl_enqueue_tmeb( TMEB *event )
{
	QUEUE	*q;
	ABSTIM	ofs = lltoul(knl_current_time) - ABSTIM_DIFF_MIN;

	for ( q = knl_timer_queue.next; q != &knl_timer_queue; q = q->next ) {
		if ( (ABSTIM)(event->time - ofs) < (ABSTIM)((((TMEB*)q)->time) - ofs) ) {
			break;
		}
	}
	QueInsert(&event->queue, q);
}

/*
 * Set timeout event
 *	Register the timer event 'event' onto the timer queue to
 *	start after the timeout 'tmout'. At timeout, start with the
 *	argument 'arg' on the callback function 'callback'.
 *	When 'tmout' is TMO_FEVR, do not register onto the timer
 *	queue, but initialize queue area in case 'timer_delete' 
 *	is called later.
 *
 *	"include/tk/typedef.h"
 *	typedef	W		TMO;
 *	typedef UW		RELTIM;
 *	#define TMO_FEVR	(-1)
 */
EXPORT void knl_timer_insert( TMEB *event, TMO tmout, CBACK callback, void *arg )
{
	event->callback = callback;
	event->arg = arg;

	if ( tmout == TMO_FEVR ) {
		QueInit(&event->queue);
	} else {
		/* To guarantee longer wait time specified by 'tmout',
		   add TIMER_PERIOD on wait time */
		event->time = lltoul(knl_current_time) + tmout + TIMER_PERIOD;
		knl_enqueue_tmeb(event);
	}
}

EXPORT void knl_timer_insert_reltim( TMEB *event, RELTIM tmout, CBACK callback, void *arg )
{
	event->callback = callback;
	event->arg = arg;

	/* To guarantee longer wait time specified by 'tmout',
	   add TIMER_PERIOD on wait time */
	event->time = lltoul(knl_current_time) + tmout + TIMER_PERIOD;
	knl_enqueue_tmeb(event);
}

/*
 * Set time specified event
 *	Register the timer event 'evt' onto the timer queue to start at the 
 *	(absolute) time 'time'.
 *	'time' is not an actual time. It is system operation time.
 */
EXPORT void knl_timer_insert_abs( TMEB *evt, ABSTIM time, CBACK cback, void *arg )
{
	evt->callback = cback;
	evt->arg = arg;
	evt->time = time;
	knl_enqueue_tmeb(evt);
}

/* ------------------------------------------------------------------------ */

/*
 * System timer interrupt handler
 *	This interrupt handler starts every TIMER_PERIOD millisecond 
 *	interval by hardware timer. Update the software clock and start the 
 *	timer event upon arriving at start time.
 */

EXPORT void knl_timer_handler( void )
{
	TMEB	*event;
	ABSTIM	cur;

	knl_clear_hw_timer_interrupt();		/* Clear timer interrupt */

	BEGIN_CRITICAL_SECTION;
	knl_current_time = ll_add(knl_current_time, uitoll(TIMER_PERIOD));
	cur = lltoul(knl_current_time);

#if USE_DBGSPT && defined(USE_FUNC_TD_INF_TSK)
	if ( knl_ctxtsk != NULL ) {
		/* Task at execution */
		if ( knl_ctxtsk->sysmode > 0 ) {
			knl_ctxtsk->stime += TIMER_PERIOD;
		} else {
			knl_ctxtsk->utime += TIMER_PERIOD;
		}
	}
#endif

	/* Execute event that passed occurring time. */
	while ( !isQueEmpty(&knl_timer_queue) ) {
		event = (TMEB*)knl_timer_queue.next;

		if ( !knl_abstim_reached(cur, event->time) ) {
			break;
		}

		QueRemove(&event->queue);
		if ( event->callback != NULL ) {
			(*event->callback)(event->arg);
		}
	}

	END_CRITICAL_SECTION;

	knl_end_of_hw_timer_interrupt();		/* Clear timer interrupt */
}
