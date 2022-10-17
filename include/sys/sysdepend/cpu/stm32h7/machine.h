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
 *	machine_depend.h
 *
 *	Machine type definition (STM32H7 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_CPU_H__
#define __SYS_SYSDEPEND_MACHINE_CPU_H__

/*
 **** CPU core-depeneded profile (ARMv7M(ARM Cortex-M7))
 */

#include "../core/armv7m/machine.h"

/*
 **** CPU-depeneded profile (STM32H7)
 */

#define	TARGET_CPU_DIR		stm32h7		/* Sysdepend-CPU-Directory name */


#endif /* __SYS_SYSDEPEND_MACHINE_CPU_H__ */
