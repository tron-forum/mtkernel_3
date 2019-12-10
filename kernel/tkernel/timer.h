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
 *	timer.h
 *	System Timer Module Definition
 */

#ifndef _TIMER_
#define _TIMER_

#include "longlong.h"

/*
 * SYSTIM internal expression and conversion
 */
typedef	D	LSYSTIM;	/* SYSTIM int. expression */

Inline LSYSTIM knl_toLSYSTIM( CONST SYSTIM *time )
{
	LSYSTIM		ltime;

	hilo_ll(ltime, time->hi, time->lo);

	return ltime;
}

Inline SYSTIM knl_toSYSTIM( LSYSTIM ltime )
{
	SYSTIM		time;

	ll_hilo(time.hi, time.lo, ltime);

	return time;
}

/*
 * Absolute time (can be considered the lower 32bit of SYSTIM)
 */
typedef	UW	ABSTIM;

#define ABSTIM_DIFF_MIN  (0x7FFFFFFF)

Inline BOOL knl_abstim_reached( ABSTIM curtim, ABSTIM evttim )
{
	return (ABSTIM)(curtim - evttim) <= (ABSTIM)ABSTIM_DIFF_MIN;
}

/*
 * Definition of timer event block 
 */
typedef void	(*CBACK)(void *);	/* Type of callback function */

typedef struct timer_event_block {
	QUEUE	queue;		/* Timer event queue */
	ABSTIM	time;		/* Event time */
	CBACK	callback;	/* Callback function */
	void	*arg;		/* Argument to be sent to callback function */
} TMEB;

/*
 * Current time (Software clock)
 */
IMPORT LSYSTIM	knl_current_time;	/* System operation time */
IMPORT LSYSTIM	knl_real_time_ofs;	/* Difference from actual time */

/*
 * Time-event queue
 */
IMPORT QUEUE	knl_timer_queue;

/*
 * Register time-event onto timer queue
 */
IMPORT void knl_timer_insert( TMEB *evt, TMO tmout, CBACK cback, void *arg );
IMPORT void knl_timer_insert_reltim( TMEB *event, RELTIM tmout, CBACK callback, void *arg );
IMPORT void knl_timer_insert_abs( TMEB *evt, ABSTIM time, CBACK cback, void *arg );

/*
 * Delete from time-event queue
 */
Inline void knl_timer_delete( TMEB *event )
{
	QueRemove(&event->queue);
}

#endif /* _TIMER_ */
