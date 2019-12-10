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
 *	rendezvous.c
 *	Rendezvous
 */


#include "kernel.h"
#include "wait.h"
#include "check.h"
#include "rendezvous.h"

#if USE_LEGACY_API && USE_RENDEZVOUS


Noinit(EXPORT PORCB knl_porcb_table[NUM_PORID]);	/* Rendezvous port control block */
Noinit(EXPORT QUEUE knl_free_porcb);	/* FreeQue */


/* 
 * Initialization of port control block 
 */
EXPORT ER knl_rendezvous_initialize( void )
{
	PORCB	*porcb, *end;

	/* Get system information */
	if ( NUM_PORID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_porcb);
	end = knl_porcb_table + NUM_PORID;
	for ( porcb = knl_porcb_table; porcb < end; porcb++ ) {
		porcb->porid = 0;
		QueInsert(&porcb->call_queue, &knl_free_porcb);
	}

	return E_OK;
}


/*
 * Processing if the priority of send wait task changes
 */
LOCAL void cal_chg_pri( TCB *tcb, INT oldpri )
{
	PORCB	*porcb;

	porcb = get_porcb(tcb->wid);
	knl_gcb_change_priority((GCB*)porcb, tcb);
}

/*
 * Definition of rendezvous wait specification
 */
EXPORT CONST WSPEC knl_wspec_cal_tfifo = { TTW_CAL, NULL, NULL };
EXPORT CONST WSPEC knl_wspec_cal_tpri  = { TTW_CAL, cal_chg_pri, NULL };

EXPORT CONST WSPEC knl_wspec_rdv       = { TTW_RDV, NULL, NULL };


/*
 * Create rendezvous port
 */
SYSCALL ID tk_cre_por( CONST T_CPOR *pk_cpor )
{
#if CHK_RSATR
	const ATR VALID_PORATR = {
		 TA_TPRI
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	PORCB	*porcb;
	ID	porid;
	ER	ercd;

	CHECK_RSATR(pk_cpor->poratr, VALID_PORATR);
	CHECK_PAR(pk_cpor->maxcmsz >= 0);
	CHECK_PAR(pk_cpor->maxrmsz >= 0);
	CHECK_INTSK();

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	porcb = (PORCB*)QueRemoveNext(&knl_free_porcb);
	if ( porcb == NULL ) {
		ercd = E_LIMIT;
	} else {
		porid = ID_POR(porcb - knl_porcb_table);

		/* Initialize control block */
		QueInit(&porcb->call_queue);
		porcb->porid = porid;
		porcb->exinf = pk_cpor->exinf;
		porcb->poratr = pk_cpor->poratr;
		QueInit(&porcb->accept_queue);
		porcb->maxcmsz = pk_cpor->maxcmsz;
		porcb->maxrmsz = pk_cpor->maxrmsz;
#if USE_OBJECT_NAME
		if ( (pk_cpor->poratr & TA_DSNAME) != 0 ) {
			knl_strncpy((char*)porcb->name, (char*)pk_cpor->dsname,
				OBJECT_NAME_LENGTH);
		}
#endif
		ercd = porid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_DEL_POR
/*
 * Delete rendezvous port
 */
SYSCALL ER tk_del_por( ID porid )
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_INTSK();

	porcb = get_porcb(porid);

	BEGIN_CRITICAL_SECTION;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
	} else {
		/* Release wait state of task (E_DLT) */
		knl_wait_delete(&porcb->call_queue);
		knl_wait_delete(&porcb->accept_queue);

		/* Return to FreeQue */
		QueInsert(&porcb->call_queue, &knl_free_porcb);
		porcb->porid = 0;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_POR */

/*
 * Call rendezvous
 */
SYSCALL INT tk_cal_por( ID porid, UINT calptn, void *msg, INT cmsgsz, TMO tmout )
{
	PORCB	*porcb;
	TCB	*tcb;
	QUEUE	*queue;
	RNO	rdvno;
	INT	rmsgsz;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PAR(calptn != 0);
	CHECK_PAR(cmsgsz >= 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	porcb = get_porcb(porid);

	BEGIN_CRITICAL_SECTION;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#if CHK_PAR
	if ( cmsgsz > porcb->maxcmsz ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	/* Search accept wait task */
	queue = porcb->accept_queue.next;
	while ( queue != &porcb->accept_queue ) {
		tcb = (TCB*)queue;
		queue = queue->next;
		if ( (calptn & tcb->winfo.acp.acpptn) == 0 ) {
			continue;
		}

		/* Send message */
		rdvno = knl_gen_rdvno(knl_ctxtsk);
		if ( cmsgsz > 0 ) {
			knl_memcpy(tcb->winfo.acp.msg, msg, (UINT)cmsgsz);
		}
		*tcb->winfo.acp.p_rdvno = rdvno;
		*tcb->winfo.acp.p_cmsgsz = cmsgsz;
		knl_wait_release_ok(tcb);

		/* Ready for rendezvous end wait */
		ercd = E_TMOUT;
		knl_ctxtsk->wspec = &knl_wspec_rdv;
		knl_ctxtsk->wid = 0;
		knl_ctxtsk->wercd = &ercd;
		knl_ctxtsk->winfo.rdv.rdvno = rdvno;
		knl_ctxtsk->winfo.rdv.msg = msg;
		knl_ctxtsk->winfo.rdv.maxrmsz = porcb->maxrmsz;
		knl_ctxtsk->winfo.rdv.p_rmsgsz = &rmsgsz;
		knl_make_wait(TMO_FEVR, porcb->poratr);
		QueInit(&knl_ctxtsk->tskque);

		goto error_exit;
	}

	/* Ready for rendezvous call wait */
	knl_ctxtsk->wspec = ( (porcb->poratr & TA_TPRI) != 0 )?
					&knl_wspec_cal_tpri: &knl_wspec_cal_tfifo;
	knl_ctxtsk->wercd = &ercd;
	knl_ctxtsk->winfo.cal.calptn = calptn;
	knl_ctxtsk->winfo.cal.msg = msg;
	knl_ctxtsk->winfo.cal.cmsgsz = cmsgsz;
	knl_ctxtsk->winfo.cal.p_rmsgsz = &rmsgsz;
	knl_gcb_make_wait((GCB*)porcb, tmout);

    error_exit:
	END_CRITICAL_SECTION;

	return ( ercd < E_OK )? ercd: rmsgsz;
}


LOCAL CONST WSPEC knl_wspec_acp       = { TTW_ACP, NULL, NULL };

/*
 * Accept rendezvous
 */
SYSCALL INT tk_acp_por( ID porid, UINT acpptn, RNO *p_rdvno, void *msg, TMO tmout )
{
	PORCB	*porcb;
	TCB	*tcb;
	QUEUE	*queue;
	RNO	rdvno;
	INT	cmsgsz;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PAR(acpptn != 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	porcb = get_porcb(porid);
    
	BEGIN_CRITICAL_SECTION;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	/* Search call wait task */
	queue = porcb->call_queue.next;
	while ( queue != &porcb->call_queue ) {
		tcb = (TCB*)queue;
		queue = queue->next;
		if ( (acpptn & tcb->winfo.cal.calptn) == 0 ) {
			continue;
		}

		/* Receive message */
		*p_rdvno = rdvno = knl_gen_rdvno(tcb);
		cmsgsz = tcb->winfo.cal.cmsgsz;
		if ( cmsgsz > 0 ) {
			knl_memcpy(msg, tcb->winfo.cal.msg, (UINT)cmsgsz);
		}

		knl_wait_cancel(tcb);

		/* Make the other task at rendezvous end wait state */
		tcb->wspec = &knl_wspec_rdv;
		tcb->wid = 0;
		tcb->winfo.rdv.rdvno = rdvno;
		tcb->winfo.rdv.msg = tcb->winfo.cal.msg;
		tcb->winfo.rdv.maxrmsz = porcb->maxrmsz;
		tcb->winfo.rdv.p_rmsgsz = tcb->winfo.cal.p_rmsgsz;
		knl_timer_insert(&tcb->wtmeb, TMO_FEVR,
					(CBACK)knl_wait_release_tmout, tcb);
		QueInit(&tcb->tskque);

		goto error_exit;
	}

	ercd = E_TMOUT;
	if ( tmout != TMO_POL ) {
		/* Ready for rendezvous accept wait */
		knl_ctxtsk->wspec = &knl_wspec_acp;
		knl_ctxtsk->wid = porid;
		knl_ctxtsk->wercd = &ercd;
		knl_ctxtsk->winfo.acp.acpptn = acpptn;
		knl_ctxtsk->winfo.acp.msg = msg;
		knl_ctxtsk->winfo.acp.p_rdvno = p_rdvno;
		knl_ctxtsk->winfo.acp.p_cmsgsz = &cmsgsz;
		knl_make_wait(tmout, porcb->poratr);
		QueInsert(&knl_ctxtsk->tskque, &porcb->accept_queue);
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ( ercd < E_OK )? ercd: cmsgsz;
}

#ifdef USE_FUNC_TK_FWD_POR
/*
 * Forward Rendezvous to Other Port
 */
SYSCALL ER tk_fwd_por( ID porid, UINT calptn, RNO rdvno, CONST void *msg, INT cmsgsz )
{
	PORCB	*porcb;
	TCB	*caltcb, *tcb;
	QUEUE	*queue;
	RNO	new_rdvno;
	ER	ercd = E_OK;

	CHECK_PORID(porid);
	CHECK_PAR(calptn != 0);
	CHECK_RDVNO(rdvno);
	CHECK_PAR(cmsgsz >= 0);
	CHECK_INTSK();

	porcb = get_porcb(porid);
	caltcb = get_tcb(knl_get_tskid_rdvno(rdvno));

	BEGIN_CRITICAL_SECTION;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#if CHK_PAR
	if ( cmsgsz > porcb->maxcmsz ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif
	if ( (caltcb->state & TS_WAIT) == 0
	  || caltcb->wspec != &knl_wspec_rdv
	  || rdvno != caltcb->winfo.rdv.rdvno ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	if ( porcb->maxrmsz > caltcb->winfo.rdv.maxrmsz ) {
		ercd = E_OBJ;
		goto error_exit;
	}
#if CHK_PAR
	if ( cmsgsz > caltcb->winfo.rdv.maxrmsz ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	/* Search accept wait task */
	queue = porcb->accept_queue.next;
	while ( queue != &porcb->accept_queue ) {
		tcb = (TCB*)queue;
		queue = queue->next;
		if ( (calptn & tcb->winfo.acp.acpptn) == 0 ) {
			continue;
		}

		/* Send message */
		new_rdvno = knl_gen_rdvno(caltcb);
		if ( cmsgsz > 0 ) {
			knl_memcpy(tcb->winfo.acp.msg, msg, (UINT)cmsgsz);
		}
		*tcb->winfo.acp.p_rdvno = new_rdvno;
		*tcb->winfo.acp.p_cmsgsz = cmsgsz;
		knl_wait_release_ok(tcb);

		/* Change rendezvous end wait of the other task */
		caltcb->winfo.rdv.rdvno = new_rdvno;
		caltcb->winfo.rdv.msg = caltcb->winfo.cal.msg;
		caltcb->winfo.rdv.maxrmsz = porcb->maxrmsz;
		caltcb->winfo.rdv.p_rmsgsz = caltcb->winfo.cal.p_rmsgsz;

		goto error_exit;
	}

	/* Change the other task to rendezvous call wait */
	caltcb->wspec = ( (porcb->poratr & TA_TPRI) != 0 )?
				&knl_wspec_cal_tpri: &knl_wspec_cal_tfifo;
	caltcb->wid = porid;
	caltcb->winfo.cal.calptn = calptn;
	caltcb->winfo.cal.msg = caltcb->winfo.rdv.msg;
	caltcb->winfo.cal.cmsgsz = cmsgsz;
	caltcb->winfo.cal.p_rmsgsz = caltcb->winfo.rdv.p_rmsgsz;
	knl_timer_insert(&caltcb->wtmeb, TMO_FEVR,
			(CBACK)knl_wait_release_tmout, caltcb);
	if ( (porcb->poratr & TA_TPRI) != 0 ) {
		knl_queue_insert_tpri(caltcb, &porcb->call_queue);
	} else {
		QueInsert(&caltcb->tskque, &porcb->call_queue);
	}

	if ( cmsgsz > 0 ) {
		knl_memcpy(caltcb->winfo.cal.msg, msg, (UINT)cmsgsz);
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_FWD_POR */

/*
 * Reply rendezvous
 */
SYSCALL ER tk_rpl_rdv( RNO rdvno, CONST void *msg, INT rmsgsz )
{
	TCB	*caltcb;
	ER	ercd = E_OK;

	CHECK_RDVNO(rdvno);
	CHECK_PAR(rmsgsz >= 0);
	CHECK_INTSK();

	caltcb = get_tcb(knl_get_tskid_rdvno(rdvno));

	BEGIN_CRITICAL_SECTION;
	if ( (caltcb->state & TS_WAIT) == 0
	  || caltcb->wspec != &knl_wspec_rdv
	  || rdvno != caltcb->winfo.rdv.rdvno ) {
		ercd = E_OBJ;
		goto error_exit;
	}
#if CHK_PAR
	if ( rmsgsz > caltcb->winfo.rdv.maxrmsz ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	/* Send message */
	if ( rmsgsz > 0 ) {
		knl_memcpy(caltcb->winfo.rdv.msg, msg, (UINT)rmsgsz);
	}
	*caltcb->winfo.rdv.p_rmsgsz = rmsgsz;
	knl_wait_release_ok(caltcb);

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_REF_POR
/*
 * Refer rendezvous port
 */
SYSCALL ER tk_ref_por( ID porid, T_RPOR *pk_rpor )
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_PORID(porid);

	porcb = get_porcb(porid);

	BEGIN_CRITICAL_SECTION;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rpor->exinf = porcb->exinf;
		pk_rpor->wtsk = knl_wait_tskid(&porcb->call_queue);
		pk_rpor->atsk = knl_wait_tskid(&porcb->accept_queue);
		pk_rpor->maxcmsz = porcb->maxcmsz;
		pk_rpor->maxrmsz = porcb->maxrmsz;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_POR */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_rendezvous_getname(ID id, UB **name)
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_PORID(id);

	BEGIN_DISABLE_INTERRUPT;
	porcb = get_porcb(id);
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (porcb->poratr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = porcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_LST_POR
/*
 * Refer rendezvous port usage state
 */
SYSCALL INT td_lst_por( ID list[], INT nent )
{
	PORCB	*porcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_porcb_table + NUM_PORID;
	for ( porcb = knl_porcb_table; porcb < end; porcb++ ) {
		if ( porcb->porid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = porcb->porid;
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_POR */

#ifdef USE_FUNC_TD_REF_POR
/*
 * Refer rendezvous port
 */
SYSCALL ER td_ref_por( ID porid, TD_RPOR *pk_rpor )
{
	PORCB	*porcb;
	ER	ercd = E_OK;

	CHECK_PORID(porid);

	porcb = get_porcb(porid);

	BEGIN_DISABLE_INTERRUPT;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rpor->exinf = porcb->exinf;
		pk_rpor->wtsk = knl_wait_tskid(&porcb->call_queue);
		pk_rpor->atsk = knl_wait_tskid(&porcb->accept_queue);
		pk_rpor->maxcmsz = porcb->maxcmsz;
		pk_rpor->maxrmsz = porcb->maxrmsz;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_POR */

#ifdef USE_FUNC_TD_CAL_QUE
/*
 * Refer rendezvous call wait queue
 */
SYSCALL INT td_cal_que( ID porid, ID list[], INT nent )
{
	PORCB	*porcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_PORID(porid);

	porcb = get_porcb(porid);

	BEGIN_DISABLE_INTERRUPT;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = porcb->call_queue.next; q != &porcb->call_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_CAL_QUE */

#ifdef USE_FUNC_TD_ACP_QUE
/*
 * Refer rendezvous accept wait queue
 */
SYSCALL INT td_acp_que( ID porid, ID list[], INT nent )
{
	PORCB	*porcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_PORID(porid);

	porcb = get_porcb(porid);

	BEGIN_DISABLE_INTERRUPT;
	if ( porcb->porid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = porcb->accept_queue.next; q != &porcb->accept_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_ACP_QUE */

#endif /* USE_DBGSPT */
#endif /* USE_LEGACY_API && USE_RENDEZVOUS */
