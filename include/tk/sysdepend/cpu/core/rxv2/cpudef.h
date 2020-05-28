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
 *	cpudef.h
 *
 *	CPU dependent definition  (RXv2 core depended)
 */

#ifndef __TK_CPUDEF_CORE_H__
#define __TK_CPUDEF_CORE_H__

#include <config.h>

/*
 * Using Coprocessor
 *   TA_COP0		FPU ( = TA_FPU)
 *   TA_COP1		DSP
 *   TA_COP2 & 3	unused
 */

#if USE_FPU && USE_DSP
#define	TA_COPS		(TA_COP0|TA_COP1)
#elif USE_FPU
#define	TA_COPS		TA_COP0
#elif USE_DSP
#define	TA_COPS		TA_COP1
#else
#define	TA_COPS		0
#endif

#define TA_FPU		TA_COP0

/*
 * Coprocessor number (depend on CPU)
 */
#define COPNO_0		0		/* FPU */
#define COPNO_1		1		/* DSP */


/*---------------------------------------------------------------------- */
/*
 * General purpose register		tk_get_reg tk_set_reg
 */
typedef struct t_regs {
	VW	r[15];	/* General purpose register R1-R15 */
} T_REGS;

/*
 * Exception-related register		tk_get_reg tk_set_reg
 */
typedef struct t_eit {
	void	*pc;	/* Program counter */
	VW	psw;	/* Status register */
} T_EIT;

/*
 * Control register			tk_get_reg tk_set_reg
 */
typedef struct t_cregs {
	void	*ssp;	/* System stack pointer R0 */
} T_CREGS;

/*
 * Coprocessor registers
 */

typedef struct t_cop0reg {
	VW	fpsw;		/* Floating Point Status register */
} T_COP0REG;

typedef struct t_cop1reg {
	UW	acc0lo;
	UW	acc0hi;
	VW	acc0gu;		/* Accumulator 0 */
	UW	acc1lo;
	UW	acc1hi;
	VW	acc1gu;		/* Accumulator 1 */
} T_COP1REG;

typedef struct t_copregs {
	T_COP0REG	cop0;	/* COP0: FPU */
	T_COP1REG	cop1;	/* COP1: DSP */
} T_COPREGS;


#endif /* __TK_CPUDEF_CORE_H__ */
