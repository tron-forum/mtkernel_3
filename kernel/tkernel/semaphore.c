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
 *	semaphore.c
 *	Semaphore
 */

#include "kernel.h"
#include "wait.h"
#include "check.h"
#include "semaphore.h"

#if USE_SEMAPHORE == 1

Noinit(EXPORT SEMCB knl_semcb_table[NUM_SEMID]);	/* Semaphore control block */
Noinit(EXPORT QUEUE knl_free_semcb);	/* FreeQue */


/* 
 * Initialization of semaphore control block 
 */
EXPORT ER knl_semaphore_initialize( void )
{
	SEMCB	*semcb, *end;

	/* Get system information */
	if ( NUM_SEMID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_semcb);
	end = knl_semcb_table + NUM_SEMID;
	for ( semcb = knl_semcb_table; semcb < end; semcb++ ) {
		semcb->semid = 0;
		QueInsert(&semcb->wait_queue, &knl_free_semcb);
	}

	return E_OK;
}


/*
 * Create semaphore
 */
SYSCALL ID tk_cre_sem( CONST T_CSEM *pk_csem )
{
#if CHK_RSATR
	const ATR VALID_SEMATR = {
		 TA_TPRI
		|TA_CNT
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	SEMCB	*semcb;
	ID	semid;
	ER	ercd;

	CHECK_RSATR(pk_csem->sematr, VALID_SEMATR);
	CHECK_PAR(pk_csem->isemcnt >= 0);
	CHECK_PAR(pk_csem->maxsem > 0);
	CHECK_PAR(pk_csem->maxsem >= pk_csem->isemcnt);

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	semcb = (SEMCB*)QueRemoveNext(&knl_free_semcb);
	if ( semcb == NULL ) {
		ercd = E_LIMIT;
	} else {
		semid = ID_SEM(semcb - knl_semcb_table);

		/* Initialize control block */
		QueInit(&semcb->wait_queue);
		semcb->semid = semid;
		semcb->exinf = pk_csem->exinf;
		semcb->sematr = pk_csem->sematr;
		semcb->semcnt = pk_csem->isemcnt;
		semcb->maxsem = pk_csem->maxsem;
#if USE_OBJECT_NAME
		if ( (pk_csem->sematr & TA_DSNAME) != 0 ) {
			knl_strncpy((char*)semcb->name, (char*)pk_csem->dsname,
				OBJECT_NAME_LENGTH);
		}
#endif
		ercd = semid;
	}
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_DEL_SEM
/*
 * Delete semaphore
 */
SYSCALL ER tk_del_sem( ID semid )
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(semid);

	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if ( semcb->semid == 0 ) {
		ercd = E_NOEXS;
	} else {
		/* Release wait state of task (E_DLT) */
		knl_wait_delete(&semcb->wait_queue);

		/* Return to FreeQue */
		QueInsert(&semcb->wait_queue, &knl_free_semcb);
		semcb->semid = 0;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_SEM */

/*
 * Signal semaphore
 */
SYSCALL ER tk_sig_sem( ID semid, INT cnt )
{
	SEMCB	*semcb;
	TCB	*tcb;
	QUEUE	*queue;
	ER	ercd = E_OK;
    
	CHECK_SEMID(semid);
	CHECK_PAR(cnt > 0);

	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if ( semcb->semid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( cnt > (semcb->maxsem - semcb->semcnt) ) {
		ercd = E_QOVR;
		goto error_exit;
	}

	/* Return semaphore counts */
	semcb->semcnt += cnt;

	/* Search task that frees wait */
	queue = semcb->wait_queue.next;
	while ( queue != &semcb->wait_queue ) {
		tcb = (TCB*)queue;
		queue = queue->next;

		/* Meet condition for Releasing wait? */
		if ( semcb->semcnt < tcb->winfo.sem.cnt ) {
			if ( (semcb->sematr & TA_CNT) == 0 ) {
				break;
			}
			continue;
		}

		/* Release wait */
		knl_wait_release_ok(tcb);

		semcb->semcnt -= tcb->winfo.sem.cnt;
		if ( semcb->semcnt <= 0 ) {
			break;
		}
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

/*
 * Processing if the priority of wait task changes
 */
LOCAL void sem_chg_pri( TCB *tcb, INT oldpri )
{
	SEMCB	*semcb;
	QUEUE	*queue;
	TCB	*top;

	semcb = get_semcb(tcb->wid);
	if ( oldpri >= 0 ) {
		/* Reorder wait line */
		knl_gcb_change_priority((GCB*)semcb, tcb);
	}

	if ( (semcb->sematr & TA_CNT) != 0 ) {
		return;
	}

	/* From the head task in a wait queue, allocate semaphore counts
	   and release wait state as much as possible */
	queue = semcb->wait_queue.next;
	while ( queue != &semcb->wait_queue ) {
		top = (TCB*)queue;
		queue = queue->next;

		/* Meet condition for releasing wait? */
		if ( semcb->semcnt < top->winfo.sem.cnt ) {
			break;
		}

		/* Release wait */
		knl_wait_release_ok(top);

		semcb->semcnt -= top->winfo.sem.cnt;
	}
}

/*
 * Processing if the wait task is freed
 */
LOCAL void sem_rel_wai( TCB *tcb )
{
	sem_chg_pri(tcb, -1);
}

/*
 * Definition of semaphore wait specification
 */
LOCAL CONST WSPEC knl_wspec_sem_tfifo = { TTW_SEM, NULL,        sem_rel_wai };
LOCAL CONST WSPEC knl_wspec_sem_tpri  = { TTW_SEM, sem_chg_pri, sem_rel_wai };

/*
 * Wait on semaphore
 */
SYSCALL ER tk_wai_sem( ID semid, INT cnt, TMO tmout )
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(semid);
	CHECK_PAR(cnt > 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if ( semcb->semid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#if CHK_PAR
	if ( cnt > semcb->maxsem ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	if ( ((semcb->sematr & TA_CNT) != 0
	      || knl_gcb_top_of_wait_queue((GCB*)semcb, knl_ctxtsk) == knl_ctxtsk)
	  && semcb->semcnt >= cnt ) {
		/* Get semaphore count */
		semcb->semcnt -= cnt;

	} else {
		/* Ready for wait */
		knl_ctxtsk->wspec = ( (semcb->sematr & TA_TPRI) != 0 )?
					&knl_wspec_sem_tpri: &knl_wspec_sem_tfifo;
		knl_ctxtsk->wercd = &ercd;
		knl_ctxtsk->winfo.sem.cnt = cnt;
		knl_gcb_make_wait((GCB*)semcb, tmout);
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}

#ifdef USE_FUNC_TK_REF_SEM
/*
 * Refer semaphore state
 */
SYSCALL ER tk_ref_sem( ID semid, T_RSEM *pk_rsem )
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(semid);

	semcb = get_semcb(semid);

	BEGIN_CRITICAL_SECTION;
	if ( semcb->semid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rsem->exinf  = semcb->exinf;
		pk_rsem->wtsk   = knl_wait_tskid(&semcb->wait_queue);
		pk_rsem->semcnt = semcb->semcnt;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_SEM */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_semaphore_getname(ID id, UB **name)
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(id);

	BEGIN_DISABLE_INTERRUPT;
	semcb = get_semcb(id);
	if ( semcb->semid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (semcb->sematr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = semcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_LST_SEM
/*
 * Refer object usage state
 */
SYSCALL INT td_lst_sem( ID list[], INT nent )
{
	SEMCB	*semcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_semcb_table + NUM_SEMID;
	for ( semcb = knl_semcb_table; semcb < end; semcb++ ) {
		if ( semcb->semid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = semcb->semid;
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_SEM */

#ifdef USE_FUNC_TD_REF_SEM
/*
 * Refer object state
 */
SYSCALL ER td_ref_sem( ID semid, TD_RSEM *pk_rsem )
{
	SEMCB	*semcb;
	ER	ercd = E_OK;

	CHECK_SEMID(semid);

	semcb = get_semcb(semid);

	BEGIN_DISABLE_INTERRUPT;
	if ( semcb->semid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rsem->exinf  = semcb->exinf;
		pk_rsem->wtsk   = knl_wait_tskid(&semcb->wait_queue);
		pk_rsem->semcnt = semcb->semcnt;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_SEM */

#ifdef USE_FUNC_TD_SEM_QUE
/*
 * Refer wait queue
 */
SYSCALL INT td_sem_que( ID semid, ID list[], INT nent )
{
	SEMCB	*semcb;
	QUEUE	*q;
	ER	ercd;

	CHECK_SEMID(semid);

	semcb = get_semcb(semid);

	BEGIN_DISABLE_INTERRUPT;
	if ( semcb->semid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT	n = 0;
		for ( q = semcb->wait_queue.next; q != &semcb->wait_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_SEM_QUE */

#endif /* USE_DBGSPT */
#endif /* USE_SEMAPHORE */
