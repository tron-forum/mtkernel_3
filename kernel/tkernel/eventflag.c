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
 *	eventflag.c
 *	Event Flag
 */

#include "kernel.h"
#include "wait.h"
#include "check.h"
#include "eventflag.h"

#if USE_EVENTFLAG == 1

Noinit(EXPORT FLGCB	knl_flgcb_table[NUM_FLGID]);	/* Event flag control block */
Noinit(EXPORT QUEUE	knl_free_flgcb);	/* FreeQue */


/*
 * Initialization of event flag control block 
 */
EXPORT ER knl_eventflag_initialize( void )
{
	FLGCB	*flgcb, *end;

	/* Get system information */
	if ( NUM_FLGID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_flgcb);
	end = knl_flgcb_table + NUM_FLGID;
	for ( flgcb = knl_flgcb_table; flgcb < end; flgcb++ ) {
		flgcb->flgid = 0;
		QueInsert(&flgcb->wait_queue, &knl_free_flgcb);
	}

	return E_OK;
}

/*
 * Create event flag
 */
SYSCALL ID tk_cre_flg( CONST T_CFLG *pk_cflg )
{
#if CHK_RSATR
	const ATR VALID_FLGATR = {
		 TA_TPRI
		|TA_WMUL
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	FLGCB	*flgcb;
	ID	flgid;
	ER	ercd;

	CHECK_RSATR(pk_cflg->flgatr, VALID_FLGATR);

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	flgcb = (FLGCB*)QueRemoveNext(&knl_free_flgcb);
	if ( flgcb == NULL ) {
		ercd = E_LIMIT;
	} else {
		flgid = ID_FLG(flgcb - knl_flgcb_table);

		/* Initialize control block */
		QueInit(&flgcb->wait_queue);
		flgcb->flgid = flgid;
		flgcb->exinf = pk_cflg->exinf;
		flgcb->flgatr = pk_cflg->flgatr;
		flgcb->flgptn = pk_cflg->iflgptn;
#if USE_OBJECT_NAME
		if ( (pk_cflg->flgatr & TA_DSNAME) != 0 ) {
			knl_strncpy((char*)flgcb->name, (char*)pk_cflg->dsname,
				OBJECT_NAME_LENGTH);
		}
#endif
		ercd = flgid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_DEL_FLG
/*
 * Delete event flag
 */
SYSCALL ER tk_del_flg( ID flgid )
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);

	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
	} else {
		/* Release wait state of task (E_DLT) */
		knl_wait_delete(&flgcb->wait_queue);

		/* Return to FreeQue */
		QueInsert(&flgcb->wait_queue, &knl_free_flgcb);
		flgcb->flgid = 0;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_FLG */

/*
 * Event flag set
 */
SYSCALL ER tk_set_flg( ID flgid, UINT setptn )
{
	FLGCB	*flgcb;
	TCB	*tcb;
	QUEUE	*queue;
	UINT	wfmode, waiptn;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);

	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	/* Set event flag */
	flgcb->flgptn |= setptn;

	/* Search task which should be released */
	queue = flgcb->wait_queue.next;
	while ( queue != &flgcb->wait_queue ) {
		tcb = (TCB*)queue;
		queue = queue->next;

		/* Meet condition for release wait? */
		waiptn = tcb->winfo.flg.waiptn;
		wfmode = tcb->winfo.flg.wfmode;
		if ( knl_eventflag_cond(flgcb, waiptn, wfmode) ) {

			/* Release wait */
			*tcb->winfo.flg.p_flgptn = flgcb->flgptn;
			knl_wait_release_ok(tcb);

			/* Clear event flag */
			if ( (wfmode & TWF_BITCLR) != 0 ) {
				if ( (flgcb->flgptn &= ~waiptn) == 0 ) {
					break;
				}
			}
			if ( (wfmode & TWF_CLR) != 0 ) {
				flgcb->flgptn = 0;
				break;
			}
		}
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

/*
 * Clear event flag 
 */
SYSCALL ER tk_clr_flg( ID flgid, UINT clrptn )
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);

	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
	} else {
		flgcb->flgptn &= clrptn;
	}
	END_CRITICAL_SECTION;

	return ercd;
}

/*
 * Processing if the priority of wait task changes
 */
LOCAL void flg_chg_pri( TCB *tcb, INT oldpri )
{
	FLGCB	*flgcb;

	flgcb = get_flgcb(tcb->wid);
	knl_gcb_change_priority((GCB*)flgcb, tcb);
}

/*
 * Definition of event flag wait specification
 */
LOCAL CONST WSPEC knl_wspec_flg_tfifo = { TTW_FLG, NULL, NULL };
LOCAL CONST WSPEC knl_wspec_flg_tpri  = { TTW_FLG, flg_chg_pri, NULL };

/*
 * Event flag wait
 */
SYSCALL ER tk_wai_flg( ID flgid, UINT waiptn, UINT wfmode, UINT *p_flgptn, TMO tmout )
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);
	CHECK_PAR(waiptn != 0);
	CHECK_PAR((wfmode & ~(TWF_ORW|TWF_CLR|TWF_BITCLR)) == 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (flgcb->flgatr & TA_WMUL) == 0 && !isQueEmpty(&flgcb->wait_queue) ) {
		/* Disable multiple tasks wait */
		ercd = E_OBJ;
		goto error_exit;
	}

	/* Meet condition for release wait? */
	if ( knl_eventflag_cond(flgcb, waiptn, wfmode) ) {
		*p_flgptn = flgcb->flgptn;

		/* Clear event flag */
		if ( (wfmode & TWF_BITCLR) != 0 ) {
			flgcb->flgptn &= ~waiptn;
		}
		if ( (wfmode & TWF_CLR) != 0 ) {
			flgcb->flgptn = 0;
		}
	} else {
		/* Ready for wait */
		knl_ctxtsk->wspec = ( (flgcb->flgatr & TA_TPRI) != 0 )?
					&knl_wspec_flg_tpri: &knl_wspec_flg_tfifo;
		knl_ctxtsk->wercd = &ercd;
		knl_ctxtsk->winfo.flg.waiptn = waiptn;
		knl_ctxtsk->winfo.flg.wfmode = wfmode;
		knl_ctxtsk->winfo.flg.p_flgptn = p_flgptn;
		knl_gcb_make_wait((GCB*)flgcb, tmout);
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_REF_FLG
/*
 * Check event flag state
 */
SYSCALL ER tk_ref_flg( ID flgid, T_RFLG *pk_rflg )
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);

	flgcb = get_flgcb(flgid);

	BEGIN_CRITICAL_SECTION;
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rflg->exinf = flgcb->exinf;
		pk_rflg->wtsk = knl_wait_tskid(&flgcb->wait_queue);
		pk_rflg->flgptn = flgcb->flgptn;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_FLG */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_eventflag_getname(ID id, UB **name)
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(id);

	BEGIN_DISABLE_INTERRUPT;
	flgcb = get_flgcb(id);
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (flgcb->flgatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = flgcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_LST_FLG
/*
 * Refer event flag usage state
 */
SYSCALL INT td_lst_flg( ID list[], INT nent )
{
	FLGCB	*flgcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_flgcb_table + NUM_FLGID;
	for ( flgcb = knl_flgcb_table; flgcb < end; flgcb++ ) {
		if ( flgcb->flgid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = flgcb->flgid;
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_FLG */

#ifdef USE_FUNC_TD_REF_FLG
/*
 * Refer event flag state
 */
SYSCALL ER td_ref_flg( ID flgid, TD_RFLG *pk_rflg )
{
	FLGCB	*flgcb;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);

	flgcb = get_flgcb(flgid);

	BEGIN_DISABLE_INTERRUPT;
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rflg->exinf = flgcb->exinf;
		pk_rflg->wtsk = knl_wait_tskid(&flgcb->wait_queue);
		pk_rflg->flgptn = flgcb->flgptn;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_FLG */

#ifdef USE_FUNC_TD_FLG_QUE
/*
 * Refer event flag wait queue
 */
SYSCALL INT td_flg_que( ID flgid, ID list[], INT nent )
{
	FLGCB	*flgcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_FLGID(flgid);

	flgcb = get_flgcb(flgid);

	BEGIN_DISABLE_INTERRUPT;
	if ( flgcb->flgid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = flgcb->wait_queue.next; q != &flgcb->wait_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_FLG_QUE */

#endif /* USE_DBGSPT */
#endif /* USE_EVENTFLAG */
