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
 *	machine.h
 *
 *	Machine type definition (STM32L4 IoT-Engine depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- STM32L4 IoT-Engine (CPU: STM32L486VGT6) definition ----- */

#define IOTE_STM32L4		1		/* Target system   : STM32L4 IoT-Engine */
#define CPU_STM32L4		1		/* Target CPU type : STM32L4 series */
#define CPU_STM32L486		1		/* Target CPU      : STM32L486 */
#define CPU_CORE_ARMV7M		1		/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM4F		1		/* Target CPU-Core      : ARM Cortex-M4F */

#define TARGET_DIR		iote_stm32l4	/* Sysdepend-Directory name */
#define KNL_SYSDEP_PATH		iote_stm32l4	/* Kernel sysdepend path */

/*
 **** CPU-depeneded profile (STM32L4)
 */
#include "../cpu/stm32l4/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
