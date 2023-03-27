/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06A
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/03.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_RXV2
/*
 *	cpu_cntl.c (RXv2)
 *	CPU-Dependent Control
 */
#include "kernel.h"
#include "../../../sysdepend.h"

#include "cpu_task.h"


/* ------------------------------------------------------------------------ */
/*
 * Task independent status
 */
EXPORT	W	knl_taskindp = 0;

/* ------------------------------------------------------------------------ */
/* Temporal stack 
 *	used when 'dispatch_to_schedtsk' is called.
 */

Noinit(EXPORT UB knl_tmp_stack[TMP_STACK_SIZE]);

#if defined(USE_FUNC_TK_SET_REG) || defined(USE_FUNC_TD_SET_REG)
/* ------------------------------------------------------------------------ */
/*
 * Set task register contents (Used in tk_set_reg())
 */
EXPORT void knl_set_reg( TCB *tcb, CONST T_REGS *regs, CONST T_EIT *eit, CONST T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT		i;

	ssp = (SStackFrame*)(( cregs != NULL )? cregs->ssp: tcb->tskctxb.ssp);

	if ( regs != NULL ) {
		for ( i = 0; i < 15; ++i ) {
			ssp->r[i] = regs->r[i];
		}
	}

	if ( eit != NULL ) {
		ssp->spc  = eit->pc;
		ssp->spsw = eit->psw;
	}

	if ( cregs != NULL ) {
		tcb->tskctxb.ssp = cregs->ssp;
	}
}
#endif /* USE_FUNC_TK_SET_REG || USE_FUNC_TD_SET_REG */

#if defined(USE_FUNC_TK_GET_REG) || defined(USE_FUNC_TD_GET_REG)
/* ------------------------------------------------------------------------ */
/*
 * Get task register contents (Used in tk_get_reg())
 */
EXPORT void knl_get_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT		i;

	ssp = (SStackFrame*)tcb->tskctxb.ssp;

	if ( regs != NULL ) {
		for ( i = 0; i < 15; ++i ) {
			regs->r[i] = ssp->r[i];
		}
	}

	if ( eit != NULL ) {
		eit->pc = ssp->spc;
		eit->psw = ssp->spsw;
	}

	if ( cregs != NULL ) {
		cregs->ssp = tcb->tskctxb.ssp; 
	}
}
#endif /* USE_FUNC_TK_GET_REG || USE_FUNC_TD_GET_REG */

#if NUM_COPROCESSOR > 0
#ifdef USE_FUNC_TK_SET_CPR
/* ------------------------------------------------------------------------ */
/*
 * Set Coprocessor registers contents (Used in tk_set_cpr())
 */
EXPORT ER knl_set_cpr( TCB *tcb, INT copno, CONST T_COPREGS *copregs )
{
	SStackFrame	*ssp;
	ER		ercd;

	ssp = (SStackFrame*)tcb->tskctxb.ssp;
	ercd = E_OK;

	switch(copno) {
	case COPNO_0:		/* copno 0: FPU */
#if	USE_FPU
		ssp->fpsw = copregs->cop0.fpsw;
#else
		ercd = E_PAR;
#endif
		break;

	case COPNO_1:			/* copno 1: DSP */
#if	USE_DSP
		ssp->acc0lo = copregs->cop1.acc0lo;
		ssp->acc0hi = copregs->cop1.acc0hi;
		ssp->acc0gu = copregs->cop1.acc0gu;
		ssp->acc1lo = copregs->cop1.acc1lo;
		ssp->acc1hi = copregs->cop1.acc1hi;
		ssp->acc1gu = copregs->cop1.acc1gu;
#else
		ercd = E_PAR;
#endif
		break;
	default:
		ercd = E_PAR;
	}

	return ercd;
}
#endif /* USE_FUNC_TK_SET_CPR */

#ifdef USE_FUNC_TK_GET_CPR
/* ------------------------------------------------------------------------ */
/*
 * Get Coprocessor registers contents (Used in tk_get_cpr())
 */
EXPORT ER knl_get_cpr( TCB *tcb, INT copno, T_COPREGS *copregs)
{
	SStackFrame	*ssp;
	ER		ercd;

	ssp = (SStackFrame*)tcb->tskctxb.ssp;
	ercd = E_OK;

	switch (copno) {
	case COPNO_0:			/* copno 0: FPU */
#if	USE_FPU
		copregs->cop0.fpsw = ssp->fpsw;
#else
		ercd = E_PAR;
#endif
		break;

	case COPNO_1:			/* copno 1: DSP */
#if	USE_DSP
		copregs->cop1.acc0lo = ssp->acc0lo;
		copregs->cop1.acc0hi = ssp->acc0hi;
		copregs->cop1.acc0gu = ssp->acc0gu;
		copregs->cop1.acc1lo = ssp->acc1lo;
		copregs->cop1.acc1hi = ssp->acc1hi;
		copregs->cop1.acc1gu = ssp->acc1gu;
#else
		ercd = E_PAR;
#endif
		break;

	default:
		ercd = E_PAR;
		break;
	}

	return ercd;
}
#endif /* USE_FUNC_TK_GET_CPR */
#endif /* NUM_COPROCESSOR > 0 */

/* ----------------------------------------------------------------------- */
/*
 *	Task dispatcher startup
 */
EXPORT void knl_force_dispatch( void )
{
	FP	fp = knl_dispatch_to_schedtsk;

	Asm("jsr %0" :: "r"(fp));	/* No return */
}

EXPORT void knl_dispatch( void )
{
	knl_dispatch_entry();
}

#endif /* CPU_CORE_RXV2 */
