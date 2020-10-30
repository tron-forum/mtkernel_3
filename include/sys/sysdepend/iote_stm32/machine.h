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
 *	machine.h
 *
 *	Machine type definition (STM32 IoT-Engine depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- TX03-M367 IoT-Engine (CPU: STM32L486) definition ----- */

#define IOTE_STM32		1		/* Target system   : STM32 IoT-Engine */
#define CPU_STM32L4		1		/* Target CPU type : STM32L4 series */
#define CPU_STM32L486		1		/* Target CPU      : STM32L486 */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM4F		1		/* Target CPU-Core      : ARM Cortex-M4F */

#define TARGET_DIR		iote_stm32	/* Sysdepend-Directory name */

/*
 **** CPU-depeneded profile (STM32L4)
 */
#include "../cpu/stm32l4/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
