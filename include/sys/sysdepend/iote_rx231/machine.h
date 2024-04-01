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
 *	Machine type definition (RX231 IoT-Engine depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_H__
#define __SYS_SYSDEPEND_MACHINE_H__

/*
 * [TYPE]_[CPU]		TARGET SYSTEM
 * CPU_xxxx		CPU type
 * CPU_CORE_xxx		CPU core type
 */

/* ----- RX231 IoT-Engine (CPU: RX231) definition ----- */

#define IOTE_RX231		1				/* Target system : RX231 IoT-Engine */
#define CPU_RX231		1				/* Target CPU : Renesas RX231 */
#define CPU_CORE_RXV2		1				/* Target CPU-Core : RX200 series*/

#define TARGET_DIR		iote_rx231			/* Sysdepend-Directory name */
#define KNL_SYSDEP_PATH		iote_rx231			/* Kernel sysdepend path */

/*
 **** CPU-depeneded profile (RX231)
 */
#include "../cpu/rx231/machine.h"


#endif /* __SYS_SYSDEPEND_MACHINE_H__ */
