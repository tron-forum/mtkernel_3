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
 *	task_sync.c
 *	Task with Synchronize Function
 */

#include "kernel.h"
#include "wait.h"
#include "check.h"
#include "limits.h"

#ifdef USE_FUNC_TK_SUS_TSK
/*
 * Suspend task
 */
SYSCALL ER tk_sus_tsk( ID tskid )
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	state = (TSTAT)tcb->state;
	if ( !knl_task_alive(state) ) {
		ercd = ( state == TS_NONEXIST )? E_NOEXS: E_OBJ;
		goto error_exit;
	}
	if ( tcb == knl_ctxtsk && knl_dispatch_disabled >= DDS_DISABLE ) {
		ercd = E_CTX;
		goto error_exit;
	}
	if ( tcb->suscnt == INT_MAX ) {
		ercd = E_QOVR;
		goto error_exit;
	}

	/* Update suspend request count */
	++tcb->suscnt;

	/* Move to forced wait state */
	if ( state == TS_READY ) {
		knl_make_non_ready(tcb);
		tcb->state = TS_SUSPEND;

	} else if ( state == TS_WAIT ) {
		tcb->state = TS_WAITSUS;
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SUS_TSK */

#ifdef USE_FUNC_TK_RSM_TSK
/*
 * Resume task
 */
SYSCALL ER tk_rsm_tsk( ID tskid )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	switch ( tcb->state ) {
	  case TS_NONEXIST:
		ercd = E_NOEXS;
		break;

	  case TS_DORMANT:
	  case TS_READY:
	  case TS_WAIT:
		ercd = E_OBJ;
		break;

	  case TS_SUSPEND:
		if ( --tcb->suscnt == 0 ) {
			knl_make_ready(tcb);
		}
		break;
	  case TS_WAITSUS:
		if ( --tcb->suscnt == 0 ) {
			tcb->state = TS_WAIT;
		}
		break;

	  default:
		ercd = E_SYS;
		break;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_RSM_TSK */

#ifdef USE_FUNC_TK_FRSM_TSK
/*
 * Force resume task
 */
SYSCALL ER tk_frsm_tsk( ID tskid )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	switch ( tcb->state ) {
	  case TS_NONEXIST:
		ercd = E_NOEXS;
		break;

	  case TS_DORMANT:
	  case TS_READY:
	  case TS_WAIT:
		ercd = E_OBJ;
		break;

	  case TS_SUSPEND:
		tcb->suscnt = 0;
		knl_make_ready(tcb);
		break;
	  case TS_WAITSUS:
		tcb->suscnt = 0;
		tcb->state = TS_WAIT;
		break;

	  default:
		ercd = E_SYS;
		break;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_FRSM_TSK */

/* ------------------------------------------------------------------------ */

/*
 * Definition of task wait specification
 */
LOCAL CONST WSPEC knl_wspec_slp = { TTW_SLP, NULL, NULL };

#ifdef USE_FUNC_TK_SLP_TSK
/*
 * Move its own task state to wait state
 */
SYSCALL ER tk_slp_tsk( TMO tmout )
{
	ER	ercd = E_OK;

	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	BEGIN_CRITICAL_SECTION;

	if ( knl_ctxtsk->wupcnt > 0 ) {
		knl_ctxtsk->wupcnt--;
	} else {
		ercd = E_TMOUT;
		if ( tmout != TMO_POL ) {
			knl_ctxtsk->wspec = &knl_wspec_slp;
			knl_ctxtsk->wid = 0;
			knl_ctxtsk->wercd = &ercd;
			knl_make_wait(tmout, TA_NULL);
			QueInit(&knl_ctxtsk->tskque);
		}
	}

	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SLP_TSK */

#ifdef USE_FUNC_TK_WUP_TSK
/*
 * Wakeup task
 */
SYSCALL ER tk_wup_tsk( ID tskid )
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	state = (TSTAT)tcb->state;
	if ( !knl_task_alive(state) ) {
		ercd = ( state == TS_NONEXIST )? E_NOEXS: E_OBJ;

	} else if ( (state & TS_WAIT) != 0 && tcb->wspec == &knl_wspec_slp ) {
		knl_wait_release_ok(tcb);

	} else if ( tcb->wupcnt == INT_MAX ) {
		ercd = E_QOVR;
	} else {
		++tcb->wupcnt;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_WUP_TSK */

#ifdef USE_FUNC_TK_CAN_WUP
/*
 * Cancel wakeup request
 */
SYSCALL INT tk_can_wup( ID tskid )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);

	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	switch ( tcb->state ) {
	  case TS_NONEXIST:
		ercd = E_NOEXS;
		break;
	  case TS_DORMANT:
		ercd = E_OBJ;
		break;

	  default:
		ercd = tcb->wupcnt;
		tcb->wupcnt = 0;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_CAN_WUP */

#ifdef USE_FUNC_TK_DLY_TSK
/*
 * Definition of task delay wait specification
 */
LOCAL CONST WSPEC knl_wspec_dly = { TTW_DLY, NULL, NULL };

/*
 * Task delay
 */
SYSCALL ER tk_dly_tsk( RELTIM dlytim )
{
	ER	ercd = E_OK;

	CHECK_RELTIM(dlytim);

	CHECK_DISPATCH();

	if ( dlytim > 0 ) {
		BEGIN_CRITICAL_SECTION;
		knl_ctxtsk->wspec = &knl_wspec_dly;
		knl_ctxtsk->wid = 0;
		knl_ctxtsk->wercd = &ercd;
		knl_make_wait_reltim(dlytim, TA_NULL);
		QueInit(&knl_ctxtsk->tskque);
		END_CRITICAL_SECTION;
	}

	return ercd;
}
#endif /* USE_FUNC_TK_DLY_TSK */

