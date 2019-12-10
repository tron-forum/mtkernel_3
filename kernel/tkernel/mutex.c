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
 *	mutex.c
 *	Mutex
 */

#include "kernel.h"
#include "wait.h"
#include "check.h"
#include "mutex.h"

#if USE_MUTEX == 1

Noinit(EXPORT MTXCB	knl_mtxcb_table[NUM_MTXID]);	/* Mutex control block */
Noinit(EXPORT QUEUE	knl_free_mtxcb);	/* FreeQue */


/*
 * Initialization of mutex control block 
 */
EXPORT ER knl_mutex_initialize(void)
{
	MTXCB	*mtxcb, *end;

	/* Get system information */
	if ( NUM_MTXID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_mtxcb);
	end = knl_mtxcb_table + NUM_MTXID;
	for( mtxcb = knl_mtxcb_table; mtxcb < end; mtxcb++ ) {
		mtxcb->mtxid = 0;
		QueInsert(&mtxcb->wait_queue, &knl_free_mtxcb);
	}

	return E_OK;
}


/*
 * Release the lock and delete it from list, and then adjust the
 * priority of task.
 * Set the highest priority between listed below:
 *	(A) The highest priority in all mutexes in which 'tcb' task locks. 
 *	(B) The base priority of 'tcb' task.
 */
EXPORT void knl_release_mutex( TCB *tcb, MTXCB *relmtxcb )
{
	MTXCB	*mtxcb, **prev;
	INT	newpri, pri;

	/* (B) The base priority of task */
	newpri = tcb->bpriority;

	/* (A) The highest priority in mutex which is locked */
	pri = newpri;
	prev = &tcb->mtxlist;
	while ( (mtxcb = *prev) != NULL ) {
		if ( mtxcb == relmtxcb ) {
			/* Delete from list */
			*prev = mtxcb->mtxlist;
			continue;
		}

		switch ( mtxcb->mtxatr & TA_CEILING ) {
		  case TA_CEILING:
			pri = mtxcb->ceilpri;
			break;
		  case TA_INHERIT:
			if ( mtx_waited(mtxcb) ) {
				pri = mtx_head_pri(mtxcb);
			}
			break;
		  default: /* TA_TFIFO, TA_TPRI */
			/* nothing to do */
			break;
		}
		if ( newpri > pri ) {
			newpri = pri;
		}

		prev = &mtxcb->mtxlist;
	}

	if ( newpri != tcb->priority ) {
		/* Change priority of lock get task */
		knl_change_task_priority(tcb, newpri);
	}
}

/*
 * Free mutex when task is terminated
 *	Free all mutexes which the task holds.
 *	Do not need to handle mutex list and priority of terminated task.
 *	
 */
EXPORT void knl_signal_all_mutex( TCB *tcb )
{
	MTXCB	*mtxcb, *next_mtxcb;
	TCB	*next_tcb;

	next_mtxcb = tcb->mtxlist;
	while ( (mtxcb = next_mtxcb) != NULL ) {
		next_mtxcb = mtxcb->mtxlist;

		if ( mtx_waited(mtxcb) ) {
			next_tcb = (TCB*)mtxcb->wait_queue.next;

			/* Wake wait task */
			knl_wait_release_ok(next_tcb);

			/* Change mutex get task */
			mtxcb->mtxtsk = next_tcb;
			mtxcb->mtxlist = next_tcb->mtxlist;
			next_tcb->mtxlist = mtxcb;

			if ( (mtxcb->mtxatr & TA_CEILING) == TA_CEILING ) {
				if ( next_tcb->priority > mtxcb->ceilpri ) {
					/* Raise the priority for the task
					   that got lock to the highest
					   priority limit */
					knl_change_task_priority(next_tcb,
							mtxcb->ceilpri);
				}
			}
		} else {
			/* No wait task */
			mtxcb->mtxtsk = NULL;
		}
	}
}

/*
 * Limit the priority change by mutex at task priority change
 *    1.If the 'tcb' task locks mutex, cannot set lower priority than the 
 *	highest priority in all mutexes which hold lock. In such case, 
 *	return the highest priority of locked mutex. 
 *    2.If mutex with TA_CEILING attribute is locked or waiting to be locked, 
 *	cannot set higher priority than the lowest within the highest 
 *	priority limit of mutex with TA_CEILING attribute.
 *	In this case, return E_ILUSE.
 *    3.Other than above, return the 'priority'.
 */
EXPORT INT knl_chg_pri_mutex( TCB *tcb, INT priority )
{
	MTXCB	*mtxcb;
	INT	hi_pri, low_pri, pri;

	hi_pri  = priority;
	low_pri = int_priority(MIN_TSKPRI);

	/* Mutex lock wait */
	if ( (tcb->state & TS_WAIT) != 0 && (tcb->wspec->tskwait & TTW_MTX) != 0 ) {
		mtxcb = get_mtxcb(tcb->wid);
		if ( (mtxcb->mtxatr & TA_CEILING) == TA_CEILING ) {
			pri = mtxcb->ceilpri;
			if ( pri > low_pri ) {
				low_pri = pri;
			}
		}
	}

	/* Locked Mutex */
	pri = hi_pri;
	for ( mtxcb = tcb->mtxlist; mtxcb != NULL; mtxcb = mtxcb->mtxlist ) {
		switch ( mtxcb->mtxatr & TA_CEILING ) {
		  case TA_CEILING:
			pri = mtxcb->ceilpri;
			if ( pri > low_pri ) {
				low_pri = pri;
			}
			break;
		  case TA_INHERIT:
			if ( mtx_waited(mtxcb) ) {
				pri = mtx_head_pri(mtxcb);
			}
			break;
		  default: /* TA_TFIFO, TA_TPRI */
			/* nothing to do */
			break;
		}
		if ( pri < hi_pri ) {
			hi_pri = pri;
		}
	}

	if ( priority < low_pri ) {
		return E_ILUSE;
	}
	return hi_pri;
}


/*
 * Create mutex
 */
SYSCALL ID tk_cre_mtx( CONST T_CMTX *pk_cmtx )
{
#if CHK_RSATR
	const ATR VALID_MTXATR = {
		 TA_CEILING
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	MTXCB	*mtxcb;
	ID	mtxid;
	INT	ceilpri;
	ER	ercd;

	CHECK_RSATR(pk_cmtx->mtxatr, VALID_MTXATR);

	if ( (pk_cmtx->mtxatr & TA_CEILING) == TA_CEILING ) {
		CHECK_PRI(pk_cmtx->ceilpri);
		ceilpri = int_priority(pk_cmtx->ceilpri);
	} else {
		ceilpri = 0;
	}

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	mtxcb = (MTXCB*)QueRemoveNext(&knl_free_mtxcb);
	if ( mtxcb == NULL ) {
		ercd = E_LIMIT;
	} else {
		mtxid = ID_MTX(mtxcb - knl_mtxcb_table);

		/* Initialize control block */
		QueInit(&mtxcb->wait_queue);
		mtxcb->mtxid   = mtxid;
		mtxcb->exinf   = pk_cmtx->exinf;
		mtxcb->mtxatr  = pk_cmtx->mtxatr;
		mtxcb->ceilpri = ceilpri;
		mtxcb->mtxtsk  = NULL;
		mtxcb->mtxlist = NULL;
#if USE_OBJECT_NAME
		if ( (pk_cmtx->mtxatr & TA_DSNAME) != 0 ) {
			knl_strncpy((char*)mtxcb->name, (char*)pk_cmtx->dsname,
				(UINT)OBJECT_NAME_LENGTH);
		}
#endif
		ercd = mtxid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_DEL_MTX
/*
 * Delete mutex
 */
SYSCALL ER tk_del_mtx( ID mtxid )
{
	MTXCB	*mtxcb;
	ER	ercd = E_OK;

	CHECK_MTXID(mtxid);

	mtxcb = get_mtxcb(mtxid);

	BEGIN_CRITICAL_SECTION;
	if ( mtxcb->mtxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		/* If there is a task that holds mutex to delete,
		 * delete the mutex from the list
		 * and adjust the task priority if necessary.
		 */
		if ( mtxcb->mtxtsk != NULL ) {
			knl_release_mutex(mtxcb->mtxtsk, mtxcb);
		}

		/* Free wait state of task (E_DLT) */
		knl_wait_delete(&mtxcb->wait_queue);

		/* Return to FreeQue */
		QueInsert(&mtxcb->wait_queue, &knl_free_mtxcb);
		mtxcb->mtxid = 0;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_MTX */


/*
 * Processing if the priority of wait task changes
 */
LOCAL void mtx_chg_pri( TCB *tcb, INT oldpri )
{
	MTXCB	*mtxcb;
	TCB	*mtxtsk;

	mtxcb = get_mtxcb(tcb->wid);
	knl_gcb_change_priority((GCB*)mtxcb, tcb);

	if ( (mtxcb->mtxatr & TA_CEILING) == TA_INHERIT ) {
		mtxtsk = mtxcb->mtxtsk;
		if ( mtxtsk->priority > tcb->priority ) {
			/* Since the highest priority of the lock wait task
			   became higher, raise the lock get task priority
			   higher */
			knl_change_task_priority(mtxtsk, tcb->priority);

		} else if ( mtxtsk->priority == oldpri ) {
			/* Since the highest priority of the lock wait task
			   might become lower, adjust this priority */
			reset_priority(mtxtsk);
		}
	}
}

/*
 * Processing if the wait task is released (For TA_INHERIT only)
 */
LOCAL void mtx_rel_wai( TCB *tcb )
{
	MTXCB	*mtxcb;
	TCB	*mtxtsk;

	mtxcb = get_mtxcb(tcb->wid);
	mtxtsk = mtxcb->mtxtsk;

	if ( mtxtsk->priority == tcb->priority ) {
		/* Since the highest priority of the lock wait task might 
		   become lower, adjust this priority */
		reset_priority(mtxtsk);
	}
}

/*
 * Definition of mutex wait specification
 */
LOCAL CONST WSPEC knl_wspec_mtx_tfifo   = { TTW_MTX, NULL, NULL };
LOCAL CONST WSPEC knl_wspec_mtx_tpri    = { TTW_MTX, mtx_chg_pri, NULL };
LOCAL CONST WSPEC knl_wspec_mtx_inherit = { TTW_MTX, mtx_chg_pri, mtx_rel_wai };

/*
 * Lock mutex
 */
SYSCALL ER tk_loc_mtx( ID mtxid, TMO tmout )
{
	MTXCB	*mtxcb;
	TCB	*mtxtsk;
	ATR	mtxatr;
	ER	ercd = E_OK;

	CHECK_MTXID(mtxid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	mtxcb = get_mtxcb(mtxid);

	BEGIN_CRITICAL_SECTION;
	if ( mtxcb->mtxid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( mtxcb->mtxtsk == knl_ctxtsk ) {
		ercd = E_ILUSE;  /* Multiplexed lock */
		goto error_exit;
	}

	mtxatr = mtxcb->mtxatr & TA_CEILING;
	if ( mtxatr == TA_CEILING ) {
		if ( knl_ctxtsk->bpriority < mtxcb->ceilpri ) {
			/* Violation of highest priority limit */
			ercd = E_ILUSE;
			goto error_exit;
		}
	}

	mtxtsk = mtxcb->mtxtsk;
	if ( mtxtsk == NULL ) {
		/* Get lock */
		mtxcb->mtxtsk = knl_ctxtsk;
		mtxcb->mtxlist = knl_ctxtsk->mtxlist;
		knl_ctxtsk->mtxlist = mtxcb;

		if ( mtxatr == TA_CEILING ) {
			if ( knl_ctxtsk->priority > mtxcb->ceilpri ) {
				/* Raise its own task to the highest
				   priority limit */
				knl_change_task_priority(knl_ctxtsk, mtxcb->ceilpri);
			}
		}
	} else {
		ercd = E_TMOUT;
		if ( tmout == TMO_POL ) {
			goto error_exit;
		}

		if ( mtxatr == TA_INHERIT ) {
			if ( mtxtsk->priority > knl_ctxtsk->priority ) {
				/* Raise the priority of task during
				   locking to the same priority as its
				   own task */
				knl_change_task_priority(mtxtsk, knl_ctxtsk->priority);
			}
		}

		/* Ready for wait */
		knl_ctxtsk->wspec = ( mtxatr == TA_TFIFO   )? &knl_wspec_mtx_tfifo:
				( mtxatr == TA_INHERIT )? &knl_wspec_mtx_inherit:
							  &knl_wspec_mtx_tpri;
		knl_ctxtsk->wercd = &ercd;
		knl_ctxtsk->wid = mtxcb->mtxid;
		knl_make_wait(tmout, mtxcb->mtxatr);
		if ( mtxatr == TA_TFIFO ) {
			QueInsert(&knl_ctxtsk->tskque, &mtxcb->wait_queue);
		} else {
			knl_queue_insert_tpri(knl_ctxtsk, &mtxcb->wait_queue);
		}
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

/*
 * Unlock mutex
 */
SYSCALL ER tk_unl_mtx( ID mtxid )
{
	MTXCB	*mtxcb;	
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_MTXID(mtxid);
	CHECK_INTSK();

	mtxcb = get_mtxcb(mtxid);

	BEGIN_CRITICAL_SECTION;
	if ( mtxcb->mtxid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( mtxcb->mtxtsk != knl_ctxtsk ) {
		ercd = E_ILUSE;  /* This is not locked by its own task */
		goto error_exit;
	}

	/* Delete the mutex from the list,
	   and adjust its own task priority if necessary. */
	knl_release_mutex(knl_ctxtsk, mtxcb);

	if ( mtx_waited(mtxcb) ) {
		tcb = (TCB*)mtxcb->wait_queue.next;

		/* Release wait */
		knl_wait_release_ok(tcb);

		/* Change mutex get task */
		mtxcb->mtxtsk = tcb;
		mtxcb->mtxlist = tcb->mtxlist;
		tcb->mtxlist = mtxcb;

		if ( (mtxcb->mtxatr & TA_CEILING) == TA_CEILING ) {
			if ( tcb->priority > mtxcb->ceilpri ) {
				/* Raise the priority of the task that
				   got lock to the highest priority limit */
				knl_change_task_priority(tcb, mtxcb->ceilpri);
			}
		}
	} else {
		/* No wait task */
		mtxcb->mtxtsk = NULL;
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}


#ifdef USE_FUNC_TK_REF_MTX
/*
 * Refer mutex state
 */
SYSCALL ER tk_ref_mtx( ID mtxid, T_RMTX *pk_rmtx )
{
	MTXCB	*mtxcb;
	ER	ercd = E_OK;

	CHECK_MTXID(mtxid);

	mtxcb = get_mtxcb(mtxid);

	BEGIN_CRITICAL_SECTION;
	if ( mtxcb->mtxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmtx->exinf = mtxcb->exinf;
		pk_rmtx->htsk = ( mtxcb->mtxtsk != NULL )?
					mtxcb->mtxtsk->tskid: 0;
		pk_rmtx->wtsk = knl_wait_tskid(&mtxcb->wait_queue);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_MTX */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_mutex_getname(ID id, UB **name)
{
	MTXCB	*mtxcb;
	ER	ercd = E_OK;

	CHECK_MTXID(id);

	BEGIN_DISABLE_INTERRUPT;
	mtxcb = get_mtxcb(id);
	if ( mtxcb->mtxid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (mtxcb->mtxatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = mtxcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_LST_MTX
/*
 * Refer mutex usage state
 */
SYSCALL INT td_lst_mtx( ID list[], INT nent )
{
	MTXCB	*mtxcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_mtxcb_table + NUM_MTXID;
	for ( mtxcb = knl_mtxcb_table; mtxcb < end; mtxcb++ ) {
		if ( mtxcb->mtxid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = mtxcb->mtxid;
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_MTX */

#ifdef USE_FUNC_TD_REF_MTX
/*
 * Refer mutex state
 */
SYSCALL ER td_ref_mtx( ID mtxid, TD_RMTX *pk_rmtx )
{
	MTXCB	*mtxcb;
	ER	ercd = E_OK;

	CHECK_MTXID(mtxid);

	mtxcb = get_mtxcb(mtxid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mtxcb->mtxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmtx->exinf = mtxcb->exinf;
		pk_rmtx->htsk = ( mtxcb->mtxtsk != NULL )?
					mtxcb->mtxtsk->tskid: 0;
		pk_rmtx->wtsk = knl_wait_tskid(&mtxcb->wait_queue);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_MTX */

#ifdef USE_FUNC_TD_MTX_QUE
/*
 * Refer mutex wait queue
 */
SYSCALL INT td_mtx_que( ID mtxid, ID list[], INT nent )
{
	MTXCB	*mtxcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_MTXID(mtxid);

	mtxcb = get_mtxcb(mtxid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mtxcb->mtxid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = mtxcb->wait_queue.next; q != &mtxcb->wait_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_MTX_QUE */

#endif /* USE_DBGSPT */
#endif /* USE_MUTEX */
