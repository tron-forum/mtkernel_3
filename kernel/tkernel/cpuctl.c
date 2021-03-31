/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpuctl.c
 *	CPU Control
 */

#include "kernel.h"
#include "task.h"
#include "check.h"


#ifdef USE_FUNC_TK_DIS_DSP
/* ------------------------------------------------------------------------ */
/*
 * Dispatch disable
 */
SYSCALL ER tk_dis_dsp( void )
{
	CHECK_CTX(!in_loc());

	knl_dispatch_disabled = DDS_DISABLE;

	return E_OK;
}
#endif /* USE_FUNC_TK_DIS_DSP */


/* ------------------------------------------------------------------------ */
/*
 * Dispatch enable
 */
#ifdef USE_FUNC_TK_ENA_DSP
SYSCALL ER tk_ena_dsp( void )
{
	CHECK_CTX(!in_loc());

	knl_dispatch_disabled = DDS_ENABLE;
	if ( knl_ctxtsk != knl_schedtsk ) {
		knl_dispatch();
	}

	return E_OK;
}
#endif /* USE_FUNC_TK_ENA_DSP */

#if TK_SUPPORT_REGOPS
#ifdef USE_FUNC_TK_SET_REG
/* ------------------------------------------------------------------------ */
/*
 * Set task register contents
 */
SYSCALL ER tk_set_reg( ID tskid,
		CONST T_REGS *pk_regs, CONST T_EIT *pk_eit, CONST T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_set_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_REG */

#ifdef USE_FUNC_TK_GET_REG
/* ------------------------------------------------------------------------ */
/*
 * Get task register contents
 */
SYSCALL ER tk_get_reg( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_get_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}

#endif /* USE_FUNC_TK_GET_REG */
#endif /* TK_SUPPORT_REGOPS */

#if NUM_COPROCESSOR > 0
#ifdef USE_FUNC_TK_SET_CPR
/* ------------------------------------------------------------------------ */
/*
 * Set coprocessor registers contents
 */
SYSCALL ER tk_set_cpr( ID tskid, INT copno, CONST T_COPREGS *pk_copregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);
	if((copno < 0) || (copno >= NUM_COPROCESSOR)
		|| !(tcb->tskatr & (TA_COP0 << copno))) {
		return E_PAR;
	}

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		ercd = knl_set_cpr(tcb, copno, pk_copregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_CPR */

#ifdef USE_FUNC_TK_GET_CPR
/* ------------------------------------------------------------------------ */
/*
 * Get coprocessor registers contents
 */
SYSCALL ER tk_get_cpr( ID tskid, INT copno, T_COPREGS *pk_copregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);
	if((copno < 0) || (copno >= NUM_COPROCESSOR)
		|| !(tcb->tskatr & (TA_COP0 << copno))) {
		return E_PAR;
	}

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		ercd = knl_get_cpr(tcb, copno, pk_copregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_CPR */
#endif /* NUM_COPROCESSOR > 0 */

#if USE_DBGSPT
/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */

#if TK_SUPPORT_REGOPS
#ifdef USE_FUNC_TD_SET_REG
/* ------------------------------------------------------------------------ */
/*
 * Set task register
 */
SYSCALL ER td_set_reg( ID tskid, CONST T_REGS *regs, CONST T_EIT *eit, CONST T_CREGS *cregs )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == knl_ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_set_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_SET_REG */

#ifdef USE_FUNC_TD_GET_REG
/* ------------------------------------------------------------------------ */
/*
 * Get task register
 */
SYSCALL ER td_get_reg( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == knl_ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_get_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_GET_REG */
#endif /* TK_SUPPORT_REGOPS */

#endif /* USE_DBGSPT */
