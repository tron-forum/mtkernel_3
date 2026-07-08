/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (NUCLEO-STM32N657 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- NUCLEO-STM32N657 (CPU: STM32N657X0) definition ----- */
#define NUCLEO_STM32N657	1	/* Target Board    : NUCLEO-STM32N657 */

#define CPU_STM32N6		1	/* Target CPU type : STM32N6 series */
#define CPU_STM32N657		1	/* Target CPU      : STM32N657 */

#define CPU_CORE_ARMV8M		1	/* Target CPU-Core type : ARMv8-M */
#define CPU_CORE_ACM55		1	/* Target CPU-Core      : ARM Cortex-M55 */

#define TARGET_DIR		nucleo_stm32n6		/* Sysdepend-Directory name */
#define	TARGET_CPU_DIR		stm32n6			/* Sysdepend-CPU-Directory name */

#define KNL_SYSDEP_PATH		sysdepend/kernel/nucleo_stm32n6	/* Kernel sysdepend path */

/* ----- Extension definition ----*/
#define EXT_SEC		1	/* Secure Extension for Arm (TrustZone) */

/*
 **** CPU Core depended profile (ARMv8M)
 */
#include "../cpu/core/armv8m/machine.h"

#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
