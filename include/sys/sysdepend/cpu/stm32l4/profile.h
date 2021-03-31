/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
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
#if USE_PTMR
#define TK_SUPPORT_PTIMER	TRUE		/* Support of physical timer */
#define TK_MAX_PTIMER		4		/* Maximum number of physical timers. */
#else
#define TK_SUPPORT_PTIMER	FALSE		/* Support of physical timer */
#define TK_MAX_PTIMER		0		/* Maximum number of physical timers. */
#endif


#endif /* __SYS_DEPEND_PROFILE_CPU_H__ */
