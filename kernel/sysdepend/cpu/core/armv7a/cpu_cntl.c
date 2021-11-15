/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7A
/*
 *	cpu_cntl.c (ARMv7-A)
 *	CPU-Dependent Control
 */
#include "kernel.h"
#include "../../../sysdepend.h"

#include "cpu_task.h"


/* Temporal stack used when 'dispatch_to_schedtsk' is called */
Noinit(EXPORT UB knl_tmp_stack[TMP_STACK_SIZE]);

/* Task independent status */
EXPORT	W	knl_taskindp = 0;

#if USE_FPU
EXPORT TCB	*knl_fpu_ctx;	/* Task in FPU context */
#endif /* USE_FPU */

/* ------------------------------------------------------------------------ */
/*
 * Set task register contents (Used in tk_set_reg())
 */
EXPORT void knl_set_reg( TCB *tcb, CONST T_REGS *regs, CONST T_EIT *eit, CONST T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT	i;

	ssp = tcb->tskctxb.ssp;

	if ( cregs != NULL ) {
		ssp = cregs->ssp;
		tcb->tskctxb.ssp  = ssp;
	}

	if ( regs != NULL ) {
		for ( i = 0; i < 4; ++i ) {
			ssp->r[i] = regs->r[i];
		}
		for ( i = 4; i < 12; ++i){
			ssp->r_[i - 4] = regs->r[i];
		}
		ssp->ip = regs->r[12];
		ssp->lr = regs->lr;
	}

	if ( eit != NULL ) {
		ssp->pc       = eit->pc;
		ssp->cpsr = eit->cpsr;
	}
}

/* ------------------------------------------------------------------------ */
/*
 * Get task register contents (Used in tk_get_reg())
 */
EXPORT void knl_get_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
	INT		i;

	ssp = tcb->tskctxb.ssp;

	if ( regs != NULL ) {
		for ( i = 0; i < 4; ++i ) {
			regs->r[i] = ssp->r[i];
		}
		for ( i = 4; i < 12; ++i ){
			regs->r[i] = ssp->r_[i - 4];
		}
		regs->r[12] = ssp->ip;
		regs->lr = ssp->lr;
	}

	if ( eit != NULL ) {
		eit->pc       = ssp->pc;
		eit->cpsr     = ssp->cpsr;
		eit->taskmode = 0;
	}

	if ( cregs != NULL ) {
		cregs->ssp   = tcb->tskctxb.ssp;
	}
}

#if USE_FPU

LOCAL void save_fpuctx(FPUContext *fpu)
{
	UW	bk_fpexe, bk_fpscr;

	Asm("fmrx %0, fpexc":"=r"(bk_fpexe));		// bk_fpexe = FPEXC
	Asm("orr ip, %0, #0x40000000"::"r"(bk_fpexe));	// FPEXC.EN = 1
	Asm("fmxr fpexc, ip");				// VFP enable

	Asm("mov ip, %0"::"r"(fpu));

	// save VFP context
	Asm("fmrx %0, fpscr":"=r"(bk_fpscr));		// Floating-Point Status and Control Register
	Asm("stmia ip!, {r0, %0}"::"r"(bk_fpscr));	// (r0 is padding)
	Asm("fstmiad ip!, {d0-d15}");
	Asm("fstmiad ip!, {d16-d31}");

	Asm("fmxr fpexc, %0"::"r"(bk_fpexe));		// restore FPEXC
}

#ifdef USE_FUNC_TK_SET_CPR
/* ------------------------------------------------------------------------ */
/*
 * Set task register contents (Used in tk_set_reg())
 */
EXPORT ER knl_set_cpr( TCB *tcb, INT copno, CONST T_COPREGS *copregs)
{
	FPUContext	*fpu;
	INT		i;
	
	fpu = tcb->isstack;
	fpu--;

	if (tcb == knl_fpu_ctx) {
		save_fpuctx(fpu);	/* save FPU context */
		knl_fpu_ctx = NULL;
	}

	if( copregs != NULL ){
		for ( i = 0; i < 32; i++ ) {
			fpu->d[i] = copregs->d[i];
		}
		fpu->fpscr = copregs->fpscr;
	}

	return E_OK;
}

#endif /* USE_FUNC_TK_SET_CPR */

#ifdef USE_FUNC_TK_GET_CPR
/* ------------------------------------------------------------------------ */
/*
 * Get task FPU register contents (Used in tk_get_cpr())
 */
EXPORT ER knl_get_cpr( TCB *tcb, INT copno, T_COPREGS *copregs)
{
	FPUContext	*fpu;
	INT		i;

	fpu = tcb->isstack;
	fpu--;

	if (tcb == knl_fpu_ctx) {
		save_fpuctx(fpu);	/* save FPU context */
	}

	if (copregs != NULL) {
		for (i = 0; i < 32; i++) {
			copregs->d[i] = fpu->d[i];
		}
		copregs->fpscr = fpu->fpscr;
	}

	return E_OK;
}
#endif /* USE_FUNC_TK_GET_CPR */
#endif /* USE_FPU */


#endif /* CPU_CORE_ARMV7A */
