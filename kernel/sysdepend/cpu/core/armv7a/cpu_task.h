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

/*
 *	cpu_task.h (ARMv7-A)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPU_CORE_CPUTASK_
#define _SYSDEPEND_CPU_CORE_CPUTASK_

/*
 * System stack configuration at task startup
 */
typedef struct {
	UW	r_[8];		/* R4-R11 */
	// Exception entry stack
	UW	r[4];		/* R0-R3 */
	UW	ip;		/* R12 */
	void	*lr;		/* lr */
	void	*pc;		/* pc */
	UW	cpsr;		/* cpsr */
} SStackFrame;

/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'knl_setup_context().'
 */
#define DORMANT_STACK_SIZE	( sizeof(SStackFrame) + 0x10 )

#if USE_FPU
typedef struct {
	UW	rsv;
	UW	fpscr;
	UD	d[32];
} FPUContext;

IMPORT TCB	*knl_fpu_ctx;	/* Task in FPU context */

#endif /* USE_FPU */

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
Inline void knl_setup_context( TCB *tcb )
{
	SStackFrame	*ssp;

	ssp = tcb->isstack;

#if USE_FPU
	if ( (tcb->tskatr & TA_FPU) != 0 ) {
		/* Initialize FPU context */
		FPUContext *fpu	= tcb->isstack;
		(--fpu)->fpscr	= FPSCR_INIT;
		ssp = (SStackFrame*)fpu;
	}
#endif /* USE_FPU */

	ssp--;

	/* CPU context initialization */
	ssp->lr = 0;
	ssp->cpsr = PSR_SVC;		/* Initial CPSR */
	ssp->pc = (void *)tcb->task;	/* Task startup address */

	tcb->tskctxb.ssp = ssp;		/* System stack */
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
Inline void knl_setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp = tcb->tskctxb.ssp;

	ssp->r[0] = stacd;
	ssp->r[1] = (VW)tcb->exinf;
}

/*
 * Delete task contexts
 */
Inline void knl_cleanup_context( TCB *tcb )
{
#if	USE_FPU
	if(tcb->tskatr & TA_FPU){
		if (knl_fpu_ctx == tcb) {
			knl_fpu_ctx = NULL;
		}
	}
#endif	// USE_FPU
}

#endif /* _SYSDEPEND_CPU_CORE_CPUTASK_ */
