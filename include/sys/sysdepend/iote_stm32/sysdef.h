/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (STM32 IoT-Engine depended)
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
#define	RCC_CFGR_INIT		(0x00000003)	// SYSCLK not divided. Source = PLL
#define	RCC_PLLCFGR_INIT	(0x00002813)	// PLLM = 2, PLLN = 40, PLLP = 7, PLLQ = 2, PLLR =2

/* ------------------------------------------------------------------------ */
/*
 * Maximum value of Power-saving mode switching prohibition request.
 * Use in tk_set_pow API.
 */
#define LOWPOW_LIMIT	0x7fff		/* Maximum number for disabling */

#endif /* __TK_SYSDEF_DEPEND_H__ */
