/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (STM32L4 IoT-Engine depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__


/* CPU-dependent definition */
#include "../cpu/stm32l4/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Clock control definition
 */

/* RCC register initial value */
#define	RCC_CFGR_INIT		(0x00000000)	// SYSCLK = PLL ,SYSCLK,HCLK not divided. 
#define	RCC_PLLCFGR_INIT	(0x00002810)	// M = 2, N = 40, P = 7, Q = 2, R =2
#define	RCC_PLLSAI1CFGR_INIT	(0x00002000)	// N = 32, P = 7, Q = 2, R =2
#define	RCC_PLLSAI2CFGR_INIT	(0x00001000)	// N = 16, P = 7, Q = 2, R =2

#define RCC_CFGR_SW_INIT	(RCC_CFGR_SW_PLL)
#define	RCC_PLLCFGR_PLLSRC_INIT	(RCC_PLLCFGR_PLLSRC_HSE)

/* Clock frequency 　*/
#define	SYSCLK			(80)		/* System clock */
#define	HCLK			(SYSCLK)	/* Peripheral clock (AHB) */
#define	PCLK1			(HCLK)		/* Peripheral clock (APB1) */
#define	PCLK2			(HCLK)		/* Peripheral clock (APB2) */
#define TMCLK			(HCLK)		/* System timer clock input (MHz) */
#define TMCLK_KHz		(TMCLK * 1000)	/* System timer clock input (kHz) */

/* ------------------------------------------------------------------------ */
/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Used in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */


#endif /* __TK_SYSDEF_DEPEND_H__ */
