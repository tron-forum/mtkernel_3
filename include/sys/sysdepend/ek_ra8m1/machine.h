/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2026 by Ken Sakamura.
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
 *	Machine type definition (EK-RA8M1 depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- EK-RA8M1 (CPU: R7FA8M1AHECBD) definition ----- */
#define EK_RA8M1		1	/* Target Board    : EK-RA8M1 */

#define CPU_RA8			1	/* Target CPU type : RA8 series */
#define CPU_RA8M1		1	/* Target CPU      : RA8M1 */

#define CPU_CORE_ARMV8M		1	/* Target CPU-Core type : ARMv8-M */
#define CPU_CORE_ACM85		1	/* Target CPU-Core      : ARM Cortex-M85 */

#define TARGET_DIR		ek_ra8m1	/* Sysdepend-Directory name */

#define KNL_SYSDEP_PATH		sysdepend/kernel/ek_ra8m1	/* Kernel sysdepend path */

/* ----- Extension definition ----*/
#define EXT_SEC		1	/* Secure Extension for Arm (TrustZone) */

/*
 **** CPU-depeneded profile (RA8M1)
 */
#include "../cpu/ra8m1/machine.h"

#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
