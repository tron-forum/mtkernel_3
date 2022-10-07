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
 *	sysdepend.h (STM32L4 IoT-Engine)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_TARGET_SYSDEPEND_
#define _SYSDEPEND_TARGET_SYSDEPEND_


#include "../cpu/stm32l4/sysdepend.h"

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(ATR clkatr);
/* ATR clkatr: Clock attribute */
#define	CLKATR_MSI		0x00000001
#define	CLKATR_HSI		0x00000002
#define	CLKATR_HSE		0x00000004

#define CLKATR_USE_PLL		0x00000010
#define CLKATR_USE_PLLSAI1	0x00000020
#define CLKATR_USE_PLLSAI2	0x00000040

#define CLKATR_LATENCY_0	0x00000000
#define CLKATR_LATENCY_1	0x00000100
#define CLKATR_LATENCY_2	0x00000200
#define CLKATR_LATENCY_3	0x00000300
#define CLKATR_LATENCY_4	0x00000400
#define	CLKATR_LATENCY_MASK	0x00000F00

IMPORT void shutdown_clock(void);

#endif /* _SYSDEPEND_TARGET_SYSDEPEND_ */