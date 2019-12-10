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
 *	mempfix.c
 *	Fixed Size Memory Pool
 */

#include "kernel.h"
#include "klock.h"
#include "wait.h"
#include "check.h"
#include "mempfix.h"

#if USE_FIX_MEMORYPOOL

Noinit(EXPORT MPFCB	knl_mpfcb_table[NUM_MPFID]);	/* Fixed size memory pool control block */
Noinit(EXPORT QUEUE	knl_free_mpfcb);	/* FreeQue */


/*
 * Initialization of fixed size memory pool control block
 */
EXPORT ER knl_fix_memorypool_initialize( void )
{
	MPFCB	*mpfcb, *end;

	/* Get system information */
	if ( NUM_MPFID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_mpfcb);
	end = knl_mpfcb_table + NUM_MPFID;
	for ( mpfcb = knl_mpfcb_table; mpfcb < end; mpfcb++ ) {
		mpfcb->mpfid = 0;
		knl_InitOBJLOCK(&mpfcb->lock);
		QueInsert(&mpfcb->wait_queue, &knl_free_mpfcb);
	}

	return E_OK;
}


/*
 * Create fixed size memory pool
 */
SYSCALL ID tk_cre_mpf( CONST T_CMPF *pk_cmpf )
{
#if CHK_RSATR
	const ATR VALID_MPFATR = {
		 TA_TPRI
		|TA_RNG3
		|TA_USERBUF
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	MPFCB	*mpfcb;
	ID	mpfid;
	W	blfsz, mpfsz;
	void	*mempool;

	CHECK_RSATR(pk_cmpf->mpfatr, VALID_MPFATR);
	CHECK_PAR(pk_cmpf->mpfcnt > 0);
	CHECK_PAR(pk_cmpf->blfsz > 0);
#if !USE_IMALLOC
	/* TA_USERBUF must be specified if configured in no Imalloc */
	CHECK_PAR((pk_cmpf->mpfatr & TA_USERBUF) != 0);
#endif
	CHECK_DISPATCH();

	blfsz = (W)MINSZ(pk_cmpf->blfsz);
	mpfsz = blfsz * pk_cmpf->mpfcnt;

#if USE_IMALLOC
	if ( (pk_cmpf->mpfatr & TA_USERBUF) != 0 ) {
		/* Size of user buffer must be multiples of sizeof(FREEL) */
		if ( blfsz != pk_cmpf->blfsz ) {
			return E_PAR;
		}
		/* Use user buffer */
		mempool = pk_cmpf->bufptr;
	} else {
		/* Allocate memory for memory pool */
		mempool = knl_Imalloc((UW)mpfsz);
		if ( mempool == NULL ) {
			return E_NOMEM;
		}
	}
#else
	/* Size of user buffer must be larger than sizeof(FREEL) */
	if ( blfsz != pk_cmpf->blfsz ) {
		return E_PAR;
	}
	/* Use user buffer */
	mempool = pk_cmpf->bufptr;
#endif

	/* Get control block from FreeQue */
	DISABLE_INTERRUPT;
	mpfcb = (MPFCB*)QueRemoveNext(&knl_free_mpfcb);
	ENABLE_INTERRUPT;

	if ( mpfcb == NULL ) {
#if USE_IMALLOC
		if ( (pk_cmpf->mpfatr & TA_USERBUF) == 0 ) {
			knl_Ifree(mempool);
		}
#endif
		return E_LIMIT;
	}

	knl_LockOBJ(&mpfcb->lock);
	mpfid = ID_MPF(mpfcb - knl_mpfcb_table);

	/* Initialize control block */
	QueInit(&mpfcb->wait_queue);
	mpfcb->exinf    = pk_cmpf->exinf;
	mpfcb->mpfatr   = pk_cmpf->mpfatr;
	mpfcb->mpfcnt   = mpfcb->frbcnt = pk_cmpf->mpfcnt;
	mpfcb->blfsz    = blfsz;
	mpfcb->mpfsz    = mpfsz;
	mpfcb->unused   = mpfcb->mempool = mempool;
	mpfcb->freelist = NULL;
#if USE_OBJECT_NAME
	if ( (pk_cmpf->mpfatr & TA_DSNAME) != 0 ) {
		knl_strncpy((char*)mpfcb->name, (char*)pk_cmpf->dsname, OBJECT_NAME_LENGTH);
	}
#endif

	mpfcb->mpfid    = mpfid;  /* Set ID after completion */
	knl_UnlockOBJ(&mpfcb->lock);

	return mpfid;
}

#ifdef USE_FUNC_TK_DEL_MPF
/*
 * Delete fixed size memory pool 
 */
SYSCALL ER tk_del_mpf( ID mpfid )
{
	MPFCB	*mpfcb;
	void	*mempool = NULL;
	ATR	memattr = 0;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);
	CHECK_DISPATCH();

	mpfcb = get_mpfcb(mpfid);

	knl_LockOBJ(&mpfcb->lock);
	if ( mpfcb->mpfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		DISABLE_INTERRUPT;
		mempool = mpfcb->mempool;
		memattr = mpfcb->mpfatr;

		/* Release wait state of task (E_DLT) */
		knl_wait_delete(&mpfcb->wait_queue);

		/* Return to FreeQue */
		QueInsert(&mpfcb->wait_queue, &knl_free_mpfcb);
		mpfcb->mpfid = 0;
		ENABLE_INTERRUPT;
	}
	knl_UnlockOBJ(&mpfcb->lock);

#if USE_IMALLOC
	if ( (mempool != NULL) && ((memattr & TA_USERBUF) == 0) ) {
		knl_Ifree(mempool);
	}
#endif

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_MPF */

/*
 * Processing if the priority of wait task changes
 */
LOCAL void knl_mpf_chg_pri( TCB *tcb, INT oldpri )
{
	MPFCB	*mpfcb;

	mpfcb = get_mpfcb(tcb->wid);
	knl_gcb_change_priority((GCB*)mpfcb, tcb);
}

/*
 * Definition of fixed size memory pool wait specification
 */
LOCAL CONST WSPEC knl_wspec_mpf_tfifo = { TTW_MPF, NULL, NULL };
LOCAL CONST WSPEC knl_wspec_mpf_tpri  = { TTW_MPF, knl_mpf_chg_pri, NULL };

/*
 * Get fixed size memory block 
 */
SYSCALL ER tk_get_mpf( ID mpfid, void **p_blf, TMO tmout )
{
	MPFCB	*mpfcb;
	FREEL	*free;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	mpfcb = get_mpfcb(mpfid);

	knl_LockOBJ(&mpfcb->lock);
	if ( mpfcb->mpfid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	/* If there is no space, ready for wait */
	if ( mpfcb->frbcnt <= 0 ) {
		goto wait_mpf;
	} else {
		/* Get memory block */
		if ( mpfcb->freelist != NULL ) {
			free = mpfcb->freelist;
			mpfcb->freelist = free->next;
			*p_blf = free;
		} else {
			*p_blf = mpfcb->unused;
			mpfcb->unused = (VB*)mpfcb->unused + mpfcb->blfsz;
		}
		mpfcb->frbcnt--;
	}

    error_exit:
	knl_UnlockOBJ(&mpfcb->lock);

	return ercd;

wait_mpf:
	/* Ready for wait */
	BEGIN_CRITICAL_SECTION;
	knl_ctxtsk->wspec = ( (mpfcb->mpfatr & TA_TPRI) != 0 )?
				&knl_wspec_mpf_tpri: &knl_wspec_mpf_tfifo;
	knl_ctxtsk->wercd = &ercd;
	knl_ctxtsk->winfo.mpf.p_blf = p_blf;
	knl_gcb_make_wait((GCB*)mpfcb, tmout);

	knl_UnlockOBJ(&mpfcb->lock);
	END_CRITICAL_SECTION;

	return ercd;
}

/*
 * Return fixed size memory block 
 */
SYSCALL ER tk_rel_mpf( ID mpfid, void *blf )
{
	MPFCB	*mpfcb;
	TCB	*tcb;
	FREEL	*free;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);
	CHECK_DISPATCH();

	mpfcb = get_mpfcb(mpfid);

	knl_LockOBJ(&mpfcb->lock);
	if ( mpfcb->mpfid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#if CHK_PAR
	if ( blf < mpfcb->mempool || blf >= knl_mempool_end(mpfcb) || (((VB*)blf - (VB*)mpfcb->mempool) % mpfcb->blfsz) != 0 ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	DISABLE_INTERRUPT;
	if ( !isQueEmpty(&mpfcb->wait_queue) ) {
		/* Send memory block to waiting task,
		   and then release the task */
		tcb = (TCB*)mpfcb->wait_queue.next;
		*tcb->winfo.mpf.p_blf = blf;
		knl_wait_release_ok(tcb);
		ENABLE_INTERRUPT;
	} else {
		ENABLE_INTERRUPT;
		/* Free memory block */
		free = (FREEL*)blf;
		free->next = mpfcb->freelist;
		mpfcb->freelist = free;
		mpfcb->frbcnt++;
	}

error_exit:
	knl_UnlockOBJ(&mpfcb->lock);

	return ercd;
}

#ifdef USE_FUNC_TK_REF_MPF
/*
 * Check fixed size pool state
 */
SYSCALL ER tk_ref_mpf( ID mpfid, T_RMPF *pk_rmpf )
{
	MPFCB	*mpfcb;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);
	CHECK_DISPATCH();

	mpfcb = get_mpfcb(mpfid);

	knl_LockOBJ(&mpfcb->lock);
	if ( mpfcb->mpfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		DISABLE_INTERRUPT;
		pk_rmpf->wtsk = knl_wait_tskid(&mpfcb->wait_queue);
		ENABLE_INTERRUPT;
		pk_rmpf->exinf = mpfcb->exinf;
		pk_rmpf->frbcnt = mpfcb->frbcnt;
	}
	knl_UnlockOBJ(&mpfcb->lock);

	return ercd;
}
#endif /* USE_FUNC_TK_REF_MPF */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_fix_memorypool_getname(ID id, UB **name)
{
	MPFCB	*mpfcb;
	ER	ercd = E_OK;

	CHECK_MPFID(id);

	BEGIN_DISABLE_INTERRUPT;
	mpfcb = get_mpfcb(id);
	if ( mpfcb->mpfid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (mpfcb->mpfatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = mpfcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_LST_MPF
/*
 * Refer fixed size memory pool usage state
 */
SYSCALL INT td_lst_mpf( ID list[], INT nent )
{
	MPFCB	*mpfcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_mpfcb_table + NUM_MPFID;
	for ( mpfcb = knl_mpfcb_table; mpfcb < end; mpfcb++ ) {
		if ( mpfcb->mpfid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = ID_MPF(mpfcb - knl_mpfcb_table);
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_MPF */

#ifdef USE_FUNC_TD_REF_MPF
/*
 * Refer fixed size memory pool state 
 */
SYSCALL ER td_ref_mpf( ID mpfid, TD_RMPF *pk_rmpf )
{
	MPFCB	*mpfcb;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);

	mpfcb = get_mpfcb(mpfid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mpfcb->mpfid == 0 ) {
		ercd = E_NOEXS;
	} else if ( knl_isLockedOBJ(&mpfcb->lock) ) {
		ercd = E_CTX;
	} else {
		pk_rmpf->wtsk = knl_wait_tskid(&mpfcb->wait_queue);
		pk_rmpf->exinf = mpfcb->exinf;
		pk_rmpf->frbcnt = mpfcb->frbcnt;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_MPF */

#ifdef USE_FUNC_TD_MPF_QUE
/*
 * Refer fixed size memory wait queue 
 */
SYSCALL INT td_mpf_que( ID mpfid, ID list[], INT nent )
{
	MPFCB	*mpfcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_MPFID(mpfid);

	mpfcb = get_mpfcb(mpfid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mpfcb->mpfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = mpfcb->wait_queue.next; q != &mpfcb->wait_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_MPF_QUE */

#endif /* USE_DBGSPT */
#endif /* USE_FIX_MEMORYPOOL */
