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
 *	mailbox.c
 *	Mailbox
 */

#include "kernel.h"
#include "wait.h"
#include "check.h"
#include "mailbox.h"

#if USE_MAILBOX == 1

Noinit(EXPORT MBXCB	knl_mbxcb_table[NUM_MBXID]);	/* Mailbox control block */
Noinit(EXPORT QUEUE	knl_free_mbxcb);	/* FreeQue */


/*
 * Initialization of mailbox control block 
 */
EXPORT ER knl_mailbox_initialize( void )
{
	MBXCB	*mbxcb, *end;

	/* Get system information */
	if ( NUM_MBXID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_mbxcb);
	end = knl_mbxcb_table + NUM_MBXID;
	for ( mbxcb = knl_mbxcb_table; mbxcb < end; mbxcb++ ) {
		mbxcb->mbxid = 0;
		QueInsert(&mbxcb->wait_queue, &knl_free_mbxcb);
	}

	return E_OK;
}


/*
 * Create mailbox
 */
SYSCALL ID tk_cre_mbx( CONST T_CMBX *pk_cmbx )
{
#if CHK_RSATR
	const ATR VALID_MBXATR = {
		 TA_MPRI
		|TA_TPRI
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	MBXCB	*mbxcb;
	ID	mbxid;
	ER	ercd;

	CHECK_RSATR(pk_cmbx->mbxatr, VALID_MBXATR);

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	mbxcb = (MBXCB*)QueRemoveNext(&knl_free_mbxcb);
	if ( mbxcb == NULL ) {
		ercd = E_LIMIT;
	} else {
		mbxid = ID_MBX(mbxcb - knl_mbxcb_table);

		/* Initialize control block */
		QueInit(&mbxcb->wait_queue);
		mbxcb->mbxid  = mbxid;
		mbxcb->exinf  = pk_cmbx->exinf;
		mbxcb->mbxatr = pk_cmbx->mbxatr;
		mbxcb->mq_head.msgque[0] = NULL;
#if USE_OBJECT_NAME
		if ( (pk_cmbx->mbxatr & TA_DSNAME) != 0 ) {
			knl_strncpy((char*)mbxcb->name, (char*)pk_cmbx->dsname,
				OBJECT_NAME_LENGTH);
		}
#endif
		ercd = mbxid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_DEL_MBX
/*
 * Delete mailbox
 */
SYSCALL ER tk_del_mbx( ID mbxid )
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);

	mbxcb = get_mbxcb(mbxid);

	BEGIN_CRITICAL_SECTION;
	if ( mbxcb->mbxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		/* Release wait state of task (E_DLT) */
		knl_wait_delete(&mbxcb->wait_queue);

		/* Return to FreeQue */
		QueInsert(&mbxcb->wait_queue, &knl_free_mbxcb);
		mbxcb->mbxid = 0;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_MBX */

/*
 * Send to mailbox
 */
SYSCALL ER tk_snd_mbx( ID mbxid, T_MSG *pk_msg )
{
	MBXCB	*mbxcb;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);

	mbxcb = get_mbxcb(mbxid);

	BEGIN_CRITICAL_SECTION;
	if (mbxcb->mbxid == 0) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	if ( (mbxcb->mbxatr & TA_MPRI) != 0 ) {
		if ( ((T_MSG_PRI*)pk_msg)->msgpri <= 0 ) {
			ercd = E_PAR;
			goto error_exit;
		}
	}

	if ( !isQueEmpty(&mbxcb->wait_queue) ) {
		/* Directly send to receive wait task */
		tcb = (TCB*)(mbxcb->wait_queue.next);
		*tcb->winfo.mbx.ppk_msg = pk_msg;
		knl_wait_release_ok(tcb);

	} else {
		/* Connect message to queue */
		if ( (mbxcb->mbxatr & TA_MPRI) != 0 ) {
			/* Connect message to queue following priority */
			knl_queue_insert_mpri((T_MSG_PRI*)pk_msg, &mbxcb->mq_head);
		} else {
			/* Connect to end of queue */
			nextmsg(pk_msg) = NULL;
			if ( headmsg(mbxcb) == NULL ) {
				headmsg(mbxcb) = pk_msg;
			} else {
				nextmsg(mbxcb->mq_tail) = pk_msg;
			}
			mbxcb->mq_tail = pk_msg;
		}
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

/*
 * Processing if the priority of wait task changes
 */
LOCAL void mbx_chg_pri( TCB *tcb, INT oldpri )
{
	MBXCB	*mbxcb;

	mbxcb = get_mbxcb(tcb->wid);
	knl_gcb_change_priority((GCB*)mbxcb, tcb);
}

/*
 * Definition of mailbox wait specification
 */
LOCAL CONST WSPEC knl_wspec_mbx_tfifo = { TTW_MBX, NULL, NULL };
LOCAL CONST WSPEC knl_wspec_mbx_tpri  = { TTW_MBX, mbx_chg_pri, NULL };

/*
 * Receive from mailbox
 */
SYSCALL ER tk_rcv_mbx( ID mbxid, T_MSG **ppk_msg, TMO tmout )
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	mbxcb = get_mbxcb(mbxid);

	BEGIN_CRITICAL_SECTION;
	if ( mbxcb->mbxid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	if ( headmsg(mbxcb) != NULL ) {
		/* Get message from head of queue */
		*ppk_msg = headmsg(mbxcb);
		headmsg(mbxcb) = nextmsg(*ppk_msg);
	} else {
		/* Ready for receive wait */
		knl_ctxtsk->wspec = ( (mbxcb->mbxatr & TA_TPRI) != 0 )?
					&knl_wspec_mbx_tpri: &knl_wspec_mbx_tfifo;
		knl_ctxtsk->wercd = &ercd;
		knl_ctxtsk->winfo.mbx.ppk_msg = ppk_msg;
		knl_gcb_make_wait((GCB*)mbxcb, tmout);
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_REF_MBX
/*
 * Refer mailbox state 
 */
SYSCALL ER tk_ref_mbx( ID mbxid, T_RMBX *pk_rmbx )
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);

	mbxcb = get_mbxcb(mbxid);

	BEGIN_CRITICAL_SECTION;
	if ( mbxcb->mbxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmbx->exinf = mbxcb->exinf;
		pk_rmbx->wtsk = knl_wait_tskid(&mbxcb->wait_queue);
		pk_rmbx->pk_msg = headmsg(mbxcb);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_MBX */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_mailbox_getname(ID id, UB **name)
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(id);

	BEGIN_DISABLE_INTERRUPT;
	mbxcb = get_mbxcb(id);
	if ( mbxcb->mbxid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (mbxcb->mbxatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = mbxcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_LST_MBX
/*
 * Refer mailbox usage state
 */
SYSCALL INT td_lst_mbx( ID list[], INT nent )
{
	MBXCB	*mbxcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_mbxcb_table + NUM_MBXID;
	for ( mbxcb = knl_mbxcb_table; mbxcb < end; mbxcb++ ) {
		if ( mbxcb->mbxid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = mbxcb->mbxid;
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_MBX */

#ifdef USE_FUNC_TD_REF_MBX
/*
 * Refer mailbox state
 */
SYSCALL ER td_ref_mbx( ID mbxid, TD_RMBX *pk_rmbx )
{
	MBXCB	*mbxcb;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);

	mbxcb = get_mbxcb(mbxid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mbxcb->mbxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmbx->exinf = mbxcb->exinf;
		pk_rmbx->wtsk = knl_wait_tskid(&mbxcb->wait_queue);
		pk_rmbx->pk_msg = headmsg(mbxcb);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_MBX */

#ifdef USE_FUNC_TD_MBX_QUE
/*
 * Refer mailbox wait queue
 */
SYSCALL INT td_mbx_que( ID mbxid, ID list[], INT nent )
{
	MBXCB	*mbxcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_MBXID(mbxid);

	mbxcb = get_mbxcb(mbxid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mbxcb->mbxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = mbxcb->wait_queue.next; q != &mbxcb->wait_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_MBX_QUE */

#endif /* USE_DBGSPT */
#endif /* USE_MAILBOX */
