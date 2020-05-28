/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	cpu_task.h (RXv2)
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPU_CORE_CPUTASK_
#define _SYSDEPEND_CPU_CORE_CPUTASK_

/*
 * System stack configuration at task startup
 */
typedef struct {
#if	USE_DSP
	UW	acc0lo;
	UW	acc0hi;
	VW	acc0gu;	/* Saved ACC0 */
	UW	acc1lo;
	UW	acc1hi;
	VW	acc1gu;	/* Saved ACC1 */
#endif
#if	USE_FPU
	VW	fpsw;	/* Saved FPSW */
#endif
	VW	r[15];	/* R1-R15 */
	void	*spc;	/* Saved return PC */
	VW	spsw;	/* Saved PSW */
} SStackFrame;

/*
 * Size of system stack area destroyed by 'make_dormant()'
 * In other words, the size of area required to write by 'setup_context().'
 */
#if	USE_FPU && USE_DSP
#define DORMANT_STACK_SIZE	( sizeof(VW) * 24 )	/* To 'spsw', 'spc', 'acc0', 'acc1', 'fpsw' position */
#elif USE_DSP
#define DORMANT_STACK_SIZE	( sizeof(VW) * 23 )	/* To 'spsw', 'spc', 'acc0', 'acc1' position */
#elif USE_FPU
#define DORMANT_STACK_SIZE	( sizeof(VW) * 18 )	/* To 'spsw', 'spc', 'fpsw' position */
#else
#define DORMANT_STACK_SIZE	( sizeof(VW) * 2 )	/* To 'spsw' and 'spc' position */
#endif

/*
 * Create stack frame for task startup
 *	Call from 'make_dormant()'
 */
Inline void knl_setup_context( TCB *tcb )
{
	SStackFrame	*ssp;
	void		*pc;
	VW		psw;

	ssp = (SStackFrame*)((UB*)(tcb->isstack) - (UB*)(sizeof(SStackFrame)));

	psw = 0x00010000UL;
	pc = (void*)tcb->task;

	/* CPU context initialization */
	ssp->spsw	= psw;	/* Initial PSW */ 
	ssp->spc	= pc;	/* Task startup address */
#if	USE_FPU
	ssp->fpsw	= FPSW_VAL;
#endif
	tcb->tskctxb.ssp = ssp;			/* System stack */
}

/*
 * Set task startup code
 *	Called by 'tk_sta_tsk()' processing.
 */
Inline void knl_setup_stacd( TCB *tcb, INT stacd )
{
	SStackFrame	*ssp = tcb->tskctxb.ssp;

	ssp->r[0] = stacd;		/* R1 */
	ssp->r[1] = (VW)tcb->exinf;	/* R2 */
}

/*
 * Delete task contexts
 */
Inline void knl_cleanup_context( TCB *tcb )
{
}

#endif /* _SYSDEPEND_CPU_CORE_CPUTASK_ */
