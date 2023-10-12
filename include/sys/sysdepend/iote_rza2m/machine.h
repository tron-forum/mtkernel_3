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
 *	Machine type definition (RZ/A2M IoT-Engine depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- RZ/A2M IoT-Engine (CPU: RZ/A2M) definition ----- */

#define IOTE_RZA2M		1				/* Target system : RZ/A2M IoT-Engine */
#define CPU_RZA2M		1				/* Target CPU : Renesas RZ/A2M */
#define	CPU_CORE_ARMV7A		1				/* Target CPU-Core type : ARMv7-A */
#define CPU_CORE_ACA9		1				/* Target CPU-Core : ARM Cortex-A9 */

#define TARGET_DIR		iote_rza2m			/* Sysdepend-Directory name */
#define KNL_SYSDEP_PATH		iote_rza2m			/* Kernel sysdepend path */

/*
 **** CPU-depeneded profile (RZA2M)
 */
#include "../cpu/rza2m/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
