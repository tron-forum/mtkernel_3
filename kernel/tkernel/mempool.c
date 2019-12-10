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
 *	mempool.c
 *	Variable Size Memory Pool
 */

#include "kernel.h"
#include "wait.h"
#include "check.h"
#include "memory.h"
#include "mempool.h"

#if USE_MEMORYPOOL


Noinit(EXPORT MPLCB knl_mplcb_table[NUM_MPLID]);	/* Variable size memory pool control block */
Noinit(EXPORT QUEUE knl_free_mplcb);	/* FreeQue */


/*
 * Initialization of variable size memory pool control block
 */
EXPORT ER knl_memorypool_initialize( void )
{
	MPLCB	*mplcb, *end;

	if ( NUM_MPLID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_mplcb);
	end = knl_mplcb_table + NUM_MPLID;
	for ( mplcb = knl_mplcb_table; mplcb < end; mplcb++ ) {
		mplcb->mplid = 0;
		QueInsert(&mplcb->wait_queue, &knl_free_mplcb);
	}

	return E_OK;
}

/* ------------------------------------------------------------------------ */

/*
 * Registration of free area on FreeQue
 *   Specialized version for merging with top/end area
 */
LOCAL void knl_appendFreeAreaBound( MPLCB *mplcb, QUEUE *aq )
{
	IMACB	*imacb = (IMACB*)&(mplcb->mplsz);
	QUEUE	*fq, *top, *end;
	W	size;

	if ( aq == &(mplcb->areaque) ) {
		top = (QUEUE*)mplcb->mempool;
	} else {
		top = aq + 1;
	}

	if ( aq->next == &(mplcb->areaque_end) ) {
		end = (QUEUE*)((VB*)mplcb->mempool + mplcb->mplsz);
	} else {
		end = aq->next;
	}

	size = (W)((VB*)end - (VB*)top);

	/* Registration position search */
	/*  Search the free area whose size is equal to 'blksz',
	 *  or larger than 'blksz' but closest.
	 *  If it does not exist, return '&imacb->freeque'.
	 */
	fq = knl_searchFreeArea(imacb, size);

	/* Register */
	clrAreaFlag(aq, AREA_USE);
	if ( fq != &imacb->freeque && FreeSize(fq) == size ) {
		/* FreeQue Same size */
		(top + 1)->next = (fq + 1)->next;
		(fq  + 1)->next = top + 1;
		(top + 1)->prev = fq + 1;
		if( (top + 1)->next != NULL ) {
			(top + 1)->next->prev = top + 1;
		}
		top->next = NULL;
	} else {
		/* FreeQue Size order */
		QueInsert(top, fq);
		(top + 1)->next = NULL;
		(top + 1)->prev = (QUEUE*)size;
	}
}

/*
 * Get memory block 
 *	'blksz' must be larger than minimum fragment size
 *	and adjusted by ROUNDSZ unit.
 */
LOCAL void *knl_get_blk( MPLCB *mplcb, W blksz )
{
	QUEUE	*q, *aq, *aq2;
	IMACB*	imacb = (IMACB*)&(mplcb->mplsz);

	/* Search FreeQue */
	q = knl_searchFreeArea(imacb, blksz);
	if ( q == &(imacb->freeque) ) {
		return NULL;
	}

	/* remove free area from FreeQue */
	knl_removeFreeQue(q);
	aq = ((void *)q == mplcb->mempool) ? &(mplcb->areaque) :  q - 1;

	/* If there is a fragment smaller than the minimum fragment size,
	   allocate them together */
	if ( FreeSize(q) - (UW)blksz >= MIN_FRAGMENT + sizeof(QUEUE) ) {

		/* Divide the area into 2. */
		aq2 = (QUEUE*)((VB*)q + blksz);
		knl_insertAreaQue(aq, aq2);

		/* Register the remaining area onto FreeQue */
		if ( aq2->next == &(mplcb->areaque_end) ) {
			knl_appendFreeAreaBound(mplcb, aq2);
		} else {
			knl_appendFreeArea(imacb, aq2);
		}
	}
	setAreaFlag(aq, AREA_USE);

	return (void *)q;
}

/*
 * Free memory block 
 */
LOCAL ER knl_rel_blk( MPLCB *mplcb, void *blk )
{
	QUEUE	*aq;
	IMACB*	imacb = (IMACB*)&(mplcb->mplsz);

	aq = (blk == mplcb->mempool) ? &(mplcb->areaque) : (QUEUE*)blk - 1;

#if CHK_PAR
	if ( !chkAreaFlag(aq, AREA_USE) ) {
		return E_PAR;
	}
#endif
	clrAreaFlag(aq, AREA_USE);

	if ( !chkAreaFlag(aq->next, AREA_USE) ) {
		/* Merge to the next area */
		knl_removeFreeQue(aq->next + 1);
		knl_removeAreaQue(aq->next);
	}
	if ( !chkAreaFlag(aq->prev, AREA_USE) ) {
		/* Merge to the previous area */
		QUEUE *fq;
		aq = aq->prev;
		fq = (aq == &(mplcb->areaque)) ? (QUEUE*)(mplcb->mempool) : aq + 1;

		knl_removeFreeQue(fq);
		knl_removeAreaQue(aq->next);
	}

	/* Register free area onto FreeQue */
	if ( aq == &(mplcb->areaque) || aq->next == &(mplcb->areaque_end) ) {
		knl_appendFreeAreaBound(mplcb, aq);
	} else {
		knl_appendFreeArea(imacb, aq);
	}

	return E_OK;
}

/* ------------------------------------------------------------------------ */

/*
 * Allocate memory and release wait task,
 * as long as there are enough free memory.
 */
EXPORT void knl_mpl_wakeup( MPLCB *mplcb )
{
	TCB	*top;
	void	*blk;
	W	blksz;

	while ( !isQueEmpty(&mplcb->wait_queue) ) {
		top = (TCB*)mplcb->wait_queue.next;
		blksz = top->winfo.mpl.blksz;

		/* Check free space */
		if ( blksz > knl_MaxFreeSize(mplcb) ) {
			break;
		}

		/* Get memory block */
		blk = knl_get_blk(mplcb, blksz);
		*top->winfo.mpl.p_blk = blk;

		/* Release wait task */
		knl_wait_release_ok(top);
	}
}


/*
 * Memory pool initial setting
 */
LOCAL void init_mempool( MPLCB *mplcb )
{
	QueInit(&mplcb->areaque);
	QueInit(&mplcb->freeque);

	/* Register onto AreaQue */
	knl_insertAreaQue(&mplcb->areaque, &mplcb->areaque_end);

	/* Set AREA_USE for locations that must not be free area */
	setAreaFlag(&mplcb->areaque_end, AREA_USE);

	/* Register onto FreeQue */
	knl_appendFreeAreaBound(mplcb, &mplcb->areaque);
}

/*
 * Create variable size memory pool 
 */
SYSCALL ID tk_cre_mpl( CONST T_CMPL *pk_cmpl )
{
#if CHK_RSATR
	const ATR VALID_MPLATR = {
		 TA_TPRI
		|TA_RNG3
		|TA_USERBUF
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	MPLCB	*mplcb;
	ID	mplid;
	W	mplsz;
	void	*mempool;
	ER	ercd;

	CHECK_RSATR(pk_cmpl->mplatr, VALID_MPLATR);
	CHECK_PAR(pk_cmpl->mplsz > 0 && pk_cmpl->mplsz <= MAX_ALLOCATE);
#if !USE_IMALLOC
	/* TA_USERBUF must be specified if configured in no Imalloc */
	CHECK_PAR((pk_cmpl->mplatr & TA_USERBUF) != 0);
#endif
	CHECK_DISPATCH();

	mplsz = roundSize(pk_cmpl->mplsz);

#if USE_IMALLOC
	if ( (pk_cmpl->mplatr & TA_USERBUF) != 0 ) {
		/* Size of user buffer must be multiples of sizeof(QUEUE)
			and larger than sizeof(QUEUE)*2 */
		if ( mplsz != pk_cmpl->mplsz ) {
			return E_PAR;
		}
		/* Use user buffer */
		mempool = pk_cmpl->bufptr;
	} else {
		/* Allocate memory for memory pool */
		mempool = knl_Imalloc((UW)mplsz);
		if ( mempool == NULL ) {
			return E_NOMEM;
		}
	}
#else
	/* Size of user buffer must be multiples of sizeof(QUEUE)
		and larger than sizeof(QUEUE)*2 */
	if ( mplsz != pk_cmpl->mplsz ) {
		return E_PAR;
	}
	/* Use user buffer */
	mempool = pk_cmpl->bufptr;
#endif

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	mplcb = (MPLCB*)QueRemoveNext(&knl_free_mplcb);
	if ( mplcb == NULL ) {
		ercd = E_LIMIT;
	} else {
		mplid = ID_MPL(mplcb - knl_mplcb_table);

		/* Initialize control block */
		QueInit(&mplcb->wait_queue);
		mplcb->mplid  = mplid;
		mplcb->exinf  = pk_cmpl->exinf;
		mplcb->mplatr = pk_cmpl->mplatr;
		mplcb->mplsz  = mplsz;
#if USE_OBJECT_NAME
		if ( (pk_cmpl->mplatr & TA_DSNAME) != 0 ) {
			knl_strncpy((char*)mplcb->name, (char*)pk_cmpl->dsname, OBJECT_NAME_LENGTH);
		}
#endif

		mplcb->mempool = mempool;

		/* Initialize memory pool */
		init_mempool(mplcb);

		ercd = mplid;
	}
	END_CRITICAL_SECTION;

#if USE_IMALLOC
	if ( (ercd < E_OK) && ((pk_cmpl->mplatr & TA_USERBUF) == 0) ) {
		knl_Ifree(mempool);
	}
#endif

	return ercd;
}

#ifdef USE_FUNC_TK_DEL_MPL
/*
 * Delete variable size memory pool 
 */
SYSCALL ER tk_del_mpl( ID mplid )
{
	MPLCB	*mplcb;
	void	*mempool = NULL;
	ATR	memattr = 0;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);
	CHECK_DISPATCH();

	mplcb = get_mplcb(mplid);

	BEGIN_CRITICAL_SECTION;
	if ( mplcb->mplid == 0 ) {
		ercd = E_NOEXS;
	} else {
		mempool = mplcb->mempool;
		memattr = mplcb->mplatr;

		/* Free wait state of task (E_DLT) */
		knl_wait_delete(&mplcb->wait_queue);

		/* Return to FreeQue */
		QueInsert(&mplcb->wait_queue, &knl_free_mplcb);
		mplcb->mplid = 0;
	}
	END_CRITICAL_SECTION;

#if USE_IMALLOC
	if ( (ercd == E_OK) && ((memattr & TA_USERBUF) == 0) ) {
		knl_Ifree(mempool);
	}
#endif

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_MPL */

/*
 * Processing if the priority of wait task changes.
 *	You need to execute with interrupt disable.
 */
LOCAL void mpl_chg_pri( TCB *tcb, INT oldpri )
{
	MPLCB	*mplcb;

	mplcb = get_mplcb(tcb->wid);
	if ( oldpri >= 0 ) {
		/* Reorder wait line */
		knl_gcb_change_priority((GCB*)mplcb, tcb);
	}

	/* From the new top task of a wait queue, free the assign
	   wait of memory blocks as much as possible. */
	knl_mpl_wakeup(mplcb);
}

/*
 * Processing if the wait task is freed
 */
LOCAL void mpl_rel_wai( TCB *tcb )
{
	mpl_chg_pri(tcb, -1);
}

/*
 * Definition of variable size memory pool wait specification
 */
LOCAL CONST WSPEC knl_wspec_mpl_tfifo = { TTW_MPL, NULL,        mpl_rel_wai };
LOCAL CONST WSPEC knl_wspec_mpl_tpri  = { TTW_MPL, mpl_chg_pri, mpl_rel_wai };

/*
 * Get variable size memory block 
 */
SYSCALL ER tk_get_mpl( ID mplid, SZ blksz, void **p_blk, TMO tmout )
{
	MPLCB	*mplcb;
	void	*blk = NULL;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);
	CHECK_PAR(blksz > 0 && blksz <= MAX_ALLOCATE);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	mplcb = get_mplcb(mplid);
	blksz = roundSize(blksz);

	BEGIN_CRITICAL_SECTION;
	if ( mplcb->mplid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}

#if CHK_PAR
	if ( blksz > mplcb->mplsz ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	if ( knl_gcb_top_of_wait_queue((GCB*)mplcb, knl_ctxtsk) == knl_ctxtsk
	  && (blk = knl_get_blk(mplcb, blksz)) != NULL ) {
		/* Get memory block */
		*p_blk = blk;
	} else {
		/* Ready for wait */
		knl_ctxtsk->wspec = ( (mplcb->mplatr & TA_TPRI) != 0 )?
					&knl_wspec_mpl_tpri: &knl_wspec_mpl_tfifo;
		knl_ctxtsk->wercd = &ercd;
		knl_ctxtsk->winfo.mpl.blksz = blksz;
		knl_ctxtsk->winfo.mpl.p_blk = p_blk;
		knl_gcb_make_wait((GCB*)mplcb, tmout);
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

/*
 * Return variable size memory block 
 */
SYSCALL ER tk_rel_mpl( ID mplid, void *blk )
{
	MPLCB	*mplcb;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);
	CHECK_DISPATCH();

	mplcb = get_mplcb(mplid);

	BEGIN_CRITICAL_SECTION;
	if ( mplcb->mplid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#if CHK_PAR
	if ( (B*)blk < (B*)mplcb->mempool || (B*)blk > (B*)mplcb->mempool + mplcb->mplsz ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	/* Free memory block */
	ercd = knl_rel_blk(mplcb, blk);
	if ( ercd < E_OK ) {
		goto error_exit;
	}

	/* Assign memory block to waiting task */
	knl_mpl_wakeup(mplcb);

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_REF_MPL
/*
 * Refer variable size memory pool state
 */
SYSCALL ER tk_ref_mpl( ID mplid, T_RMPL *pk_rmpl )
{
	MPLCB	*mplcb;
	QUEUE	*fq, *q;
	W	frsz, blksz;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);
	CHECK_DISPATCH();

	mplcb = get_mplcb(mplid);

	BEGIN_CRITICAL_SECTION;
	if ( mplcb->mplid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmpl->exinf = mplcb->exinf;
		pk_rmpl->wtsk  = knl_wait_tskid(&mplcb->wait_queue);
		frsz = 0;
		for ( fq = mplcb->freeque.next; fq != &mplcb->freeque; fq = fq->next ) {
			blksz = FreeSize(fq);
			frsz += blksz;
			for ( q = (fq+1)->next; q != NULL; q = q->next ) {
				frsz += blksz;
			}
		}
		pk_rmpl->frsz  = frsz;
		pk_rmpl->maxsz = knl_MaxFreeSize(mplcb);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_MPL */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_memorypool_getname(ID id, UB **name)
{
	MPLCB	*mplcb;
	ER	ercd = E_OK;

	CHECK_MPLID(id);

	BEGIN_DISABLE_INTERRUPT;
	mplcb = get_mplcb(id);
	if ( mplcb->mplid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (mplcb->mplatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = mplcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_LST_MPL
/*
 * Refer variable size memory pool usage state
 */
SYSCALL INT td_lst_mpl( ID list[], INT nent )
{
	MPLCB	*mplcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_mplcb_table + NUM_MPLID;
	for ( mplcb = knl_mplcb_table; mplcb < end; mplcb++ ) {
		if ( mplcb->mplid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = ID_MPL(mplcb - knl_mplcb_table);
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_MPL */

#ifdef USE_FUNC_TD_REF_MPL
/*
 * Refer variable size memory pool state
 */
SYSCALL ER td_ref_mpl( ID mplid, TD_RMPL *pk_rmpl )
{
	MPLCB	*mplcb;
	QUEUE	*fq, *q;
	W	frsz, blksz;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);

	mplcb = get_mplcb(mplid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mplcb->mplid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmpl->exinf = mplcb->exinf;
		pk_rmpl->wtsk  = knl_wait_tskid(&mplcb->wait_queue);
		frsz = 0;
		for ( fq = mplcb->freeque.next; fq != &mplcb->freeque; fq = fq->next ) {
			blksz = FreeSize(fq);
			frsz += blksz;
			for ( q = (fq+1)->next; q != NULL; q = q->next ) {
				frsz += blksz;
			}
		}
		pk_rmpl->frsz  = frsz;
		pk_rmpl->maxsz = knl_MaxFreeSize(mplcb);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_MPL */

#ifdef USE_FUNC_TD_MPL_QUE
/*
 * Refer variable size memory pool wait queue 
 */
SYSCALL INT td_mpl_que( ID mplid, ID list[], INT nent )
{
	MPLCB	*mplcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_MPLID(mplid);

	mplcb = get_mplcb(mplid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mplcb->mplid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = mplcb->wait_queue.next; q != &mplcb->wait_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_MPL_QUE */

#endif /* USE_DBGSPT */
#endif /* USE_MEMORYPOOL */
