/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */


/*
 *	profile.h
 *
 *	Service Profile (RZ/A2M depended)
 */

#ifndef __SYS_DEPEND_PROFILE_CPU_H__
#define __SYS_DEPEND_PROFILE_CPU_H__

/*
 **** CPU core-depeneded profile (ARM Cortex-A9)
 */
#include "../core/armv7a/profile.h"

/*
 **** CPU-depeneded profile (RZ/A9M)
 */

/*
 * Device Support
 */
#define TK_SUPPORT_IOPORT	TRUE		/* Support of I/O port access */

/*
 * physical timer
 */
#define TK_SUPPORT_PTIMER	TRUE		/* Support of physical timer */
#define TK_MAX_PTIMER		2		/* Maximum number of physical timers. */



#endif /* __SYS_DEPEND_PROFILE_CPU_H__ */
