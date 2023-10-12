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
 *	Machine type definition (M367 IoT-Engine depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- TX03-M367 IoT-Engine (CPU: TMPM367FDFG) definition ----- */

#define IOTE_M367		1				/* Target system : M367 IoT-Engine */
#define CPU_TMPM367FDFG		1				/* Target CPU : Toshiba TMPM367FDFG */
#define CPU_CORE_ARMV7M		1				/* Target CPU-Core type : ARMv7-M */
#define CPU_CORE_ACM3		1				/* Target CPU-Core : ARM Cortex-M3 */

#define TARGET_DIR		iote_m367			/* Sysdepend-Directory name */
#define KNL_SYSDEP_PATH		iote_m367			/* Kernel sysdepend path */

/*
 **** CPU-depeneded profile (TX03M367)
 */
#include "../cpu/tx03_m367/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
