/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpu_task.h (ARMv7-M)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPU_CORE_CPUTASK_
#define _SYSDEPEND_CPU_CORE_CPUTASK_

/*
 * System stack configuration at task startup
 */
typedef struct {
	UW	exp_ret;	/* Exception return */
	UW	r_[8];		/* R4-R11 */
	// Exception entry stack
	UW	r[4];		/* R0-R3 */
	UW	ip;		/* R12 */
	void	*lr;		/* lr */
	void	*pc;		/* pc */
	UW	xpsr;		/* xpsr */
} SStackFrame;

/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'knl_setup_context().'
 */
#define DORMANT_STACK_SIZE	( sizeof(VW) * 7 ) /* To 'R4' position */

#if USE_FPU
typedef struct {
	UW	ufpu;		/* FPU usage flag */
	UW	s_[16];		/* S16-S31 */

	UW	exp_ret;	/* Exception return */
	UW	r_[8];		/* R4-R11 */

	// Exception entry stack
	UW	r[4];		/* R0-R3 */
	UW	ip;		/* R12 */
	void	*lr;		/* lr */
	void	*pc;		/* pc */
	UW	xpsr;		/* xpsr */

	UW	s[16];		/* S0-S15 */
	UW	fpscr;		/* fpscr */
} SStackFrame_wFPU;

#define	EXPRN_NO_FPU		0x00000010	/* FPU usage flag  0:use 1:no use */

#endif /* USE_FPU */

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
Inline void knl_setup_context( TCB *tcb )
{
	SStackFrame	*ssp;

	ssp = tcb->isstack;
	ssp--;

	/* CPU context initialization */
	ssp->exp_ret	= 0xFFFFFFF9;
	ssp->lr		= 0;
	ssp->xpsr	= 0x01000000;		/* Initial SR */
	ssp->pc = (void*)((UW)tcb->task & ~0x00000001UL);	/* Task startup address */

	tcb->tskctxb.ssp = ssp;		/* System stack pointer */
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
Inline void knl_setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp;
	
	ssp = tcb->tskctxb.ssp;

	ssp->r[0] = stacd;
	ssp->r[1] = (VW)tcb->exinf;
}

/*
 * Delete task contexts
 */
Inline void knl_cleanup_context( TCB *tcb )
{
#if USE_FPU		/* Clear CONTROL.FPCA */
	UW	control;

	if(tcb == knl_ctxtsk) {
		/* Clear CONTROL.FPCA */
		Asm("mrs %0, control":"=r"(control));
		control &= ~(1<<2);
		Asm("msr control, %0"::"r"(control));
	}
#endif
}

#endif /* _SYSDEPEND_CPU_CORE_CPUTASK_ */
