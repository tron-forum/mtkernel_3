/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03/13.
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

/* ----- TX03-M367 IoT-Engine (CPU: TMPM369FDFG) definition ----- */

#define IOTE_M367		1				/* Target system : M367 IoT-Engine */
#define CPU_TMPM369FDFG		1				/* Target CPU : Toshiba TMPM369FDFG */
#define CPU_CORE_ACM3		1				/* Target CPU-Core : ARM Cortex-M3 */

#define TARGET_DIR		iote_m367			/* Sysdepend-Directory name */

/*
 **** CPU-depeneded profile (TX03M367)
 */
#include "../cpu/tx03_m367/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
