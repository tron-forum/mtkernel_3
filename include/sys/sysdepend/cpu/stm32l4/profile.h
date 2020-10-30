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
 *	profile.h
 *
 *	Service Profile (STM32L4 depended)
 */

#ifndef __SYS_DEPEND_PROFILE_CPU_H__
#define __SYS_DEPEND_PROFILE_CPU_H__

/*
 **** CPU core-depeneded profile (ARM Cortex-M4)
 */
#include "../core/armv7m/profile.h"

/*
 **** CPU-depeneded profile (STM32L4)
 */

/*
 * Device Support
 */
#define TK_SUPPORT_IOPORT	TRUE		/* Support of I/O port access */

/*
 * Physical timer
 */
#define TK_SUPPORT_PTIMER	FALSE		/* Support of physical timer */
#define TK_MAX_PTIMER		0		/* Maximum number of physical timers. */


#endif /* __SYS_DEPEND_PROFILE_CPU_H__ */
