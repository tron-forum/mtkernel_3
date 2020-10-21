/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.02
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21 .
 *
 *----------------------------------------------------------------------
 */

/*
 *	profile.h
 *
 *	Service Profile (RX231 depended)
 */

#ifndef __SYS_DEPEND_PROFILE_CPU_H__
#define __SYS_DEPEND_PROFILE_CPU_H__

/*
 **** CPU core-depeneded profile (RXv2)
 */
#include "../core/rxv2/profile.h"

/*
 **** CPU-depeneded profile (RX231)
 */

/*
 * Device Support
 */
#define TK_SUPPORT_IOPORT	TRUE		/* Support of I/O port access */

/*
 * physical timer
 */
#if USE_PTMR
#define TK_SUPPORT_PTIMER	TRUE		/* Support of physical timer */
#define TK_MAX_PTIMER		2		/* Maximum number of physical timers. */
#else
#define TK_SUPPORT_PTIMER	FALSE		/* Support of physical timer */
#define TK_MAX_PTIMER		0		/* Maximum number of physical timers. */
#endif



#endif /* __SYS_DEPEND_PROFILE_CPU_H__ */
