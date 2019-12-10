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
 *	time_calls.h
 *	Time Management Function
 */

#ifndef _TIME_CALLS_H
#define _TIME_CALLS_H

#define	DIFF_TRON_UTC		(473385600000LL)		/* Differences between UTC and TRON-Time */

/*
 * Cyclic handler control block
 */
typedef struct cyclic_handler_control_block {
	void	*exinf;		/* Extended information */
	ATR	cycatr;		/* Cyclic handler attribute */
	FP	cychdr;		/* Cyclic handler address */
	UINT	cycstat;	/* Cyclic handler state */
	RELTIM	cyctim;		/* Cyclic time */
	TMEB	cyctmeb;	/* Timer event block */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} CYCCB;

IMPORT CYCCB	knl_cyccb_table[];	/* Cyclic handler control block */
IMPORT QUEUE	knl_free_cyccb;	/* FreeQue */

#define get_cyccb(id)	( &knl_cyccb_table[INDEX_CYC(id)] )


/*
 * Next startup time
 */
Inline ABSTIM knl_cyc_next_time( CYCCB *cyccb )
{
	ABSTIM		tm, cur;

	cur = lltoul(knl_current_time);
	tm = cyccb->cyctmeb.time + cyccb->cyctim;

	if ( knl_abstim_reached(cur, tm) ) {
		/* Adjust time to be later than current time */
		tm = ((cur - cyccb->cyctmeb.time) / cyccb->cyctim + 1) * cyccb->cyctim + cyccb->cyctmeb.time;
	}

	return tm;
}

IMPORT void knl_call_cychdr( CYCCB* cyccb );

/*
 * Register timer event queue
 */
Inline void knl_cyc_timer_insert( CYCCB *cyccb, ABSTIM tm )
{
	knl_timer_insert_abs(&cyccb->cyctmeb, tm, (CBACK)knl_call_cychdr, cyccb);
}


/*
 * Alarm handler control block
 */
typedef struct alarm_handler_control_block {
	void	*exinf;		/* Extended information */
	ATR	almatr;		/* Alarm handler attribute */
	FP	almhdr;		/* Alarm handler address */
	UINT	almstat;	/* Alarm handler state */
	TMEB	almtmeb;	/* Timer event block */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} ALMCB;
/** [END Common Definitions] */

IMPORT ALMCB	knl_almcb_table[];	/* Alarm handler control block */
IMPORT QUEUE	knl_free_almcb;	/* FreeQue */

#define get_almcb(id)	( &knl_almcb_table[INDEX_ALM(id)] )

IMPORT void knl_call_almhdr( ALMCB *almcb );

/*
 * Register onto timer event queue
 */
Inline void knl_alm_timer_insert( ALMCB *almcb, RELTIM reltim )
{
	ABSTIM	tm;

	/* To guarantee to start the handler after the specified time, 
	   add TIMER_PERIOD */
	tm = lltoul(knl_current_time) + reltim + TIMER_PERIOD;

	knl_timer_insert_abs(&almcb->almtmeb, tm, (CBACK)knl_call_almhdr, almcb);
}


#endif /* _TIME_CALLS_H */
