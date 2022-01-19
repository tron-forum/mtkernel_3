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
 *	Service Profile (RZ/A2M IoT-Engine depended)
 */

#ifndef __SYS_DEPEND_PROFILE_H__
#define __SYS_DEPEND_PROFILE_H__

/*
 **** CPU-depeneded profile (RZ/A2M)
 */
#include "../cpu/rza2m/profile.h"

/*
 **** Target-depeneded profile (RZ/A2M IoT-Engine)
 */

/*
 * Power management
 */
#define TK_SUPPORT_LOWPOWER	FALSE		/* Support of power management */



#endif /* __SYS_DEPEND_PROFILE_H__ */
