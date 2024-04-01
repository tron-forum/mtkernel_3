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
 *	cpudef.h
 *
 *	CPU dependent definition  (ARMv7-A core depended)
 */

#ifndef __TK_CPUDEF_CORE_H__
#define __TK_CPUDEF_CORE_H__

/*
 * Using FPU (depend on CPU)
 *   TA_COP0		FPU ( = TA_FPU)
 */

#if USE_FPU
#define	TA_COPS		TA_COP0
#else
#define	TA_COPS		0
#endif

#define TA_FPU		TA_COP0		/* dummy. An error occurs when checking API calls. */

/*
 * General purpose register		tk_get_reg tk_set_reg
 */
typedef struct t_regs {
	VW	r[13];		/* General purpose register R0-R12 */
	void	*lr;		/* Link register R14 */
} T_REGS;

/*
 * Exception-related register		tk_get_reg tk_set_reg
 */
typedef struct t_eit {
	void	*pc;		/* Program counter R15 */
	UW	cpsr;		/* Program status register */
	UW	taskmode;	/* Task mode flag */
} T_EIT;

/*
 * Control register			tk_get_reg tk_set_reg
 */
typedef struct t_cregs {
	void	*ssp;		/* System stack pointer R13_svc */
} T_CREGS;

#if NUM_COPROCESSOR > 0
/*
 * Coprocessor registers
 */
typedef struct t_copregs {
	UD	d[32];		/* FPU General purpose register d0-d31 */
	UW	fpscr;		/* Floating-point Status and Control Register */
} T_COPREGS;
#endif  /* NUM_COPROCESSOR  */

#endif /* __TK_CPUDEF_CORE_H__ */
