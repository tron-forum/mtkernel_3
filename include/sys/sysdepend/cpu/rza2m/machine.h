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
 *	Machine type definition (RZ/A2M depended)
 */

#ifndef __SYS_SYSDEPEND_MACHINE_CPU_H__
#define __SYS_SYSDEPEND_MACHINE_CPU_H__

/*
 **** CPU core-depeneded profile (ARM Cortex-A9)
 */

#include "../core/armv7a/machine.h"

/*
 **** CPU-depeneded profile (RZ/A2M)
 */

#define	TARGET_CPU_DIR		rza2m		/* Sysdepend-CPU-Directory name */


#endif /* __SYS_SYSDEPEND_MACHINE_CPU_H__ */
