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
 *	sysdepend.h (TX03-M367)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_SYSDEPEND_
#define _SYSDEPEND_CPU_SYSDEPEND_

#include "../core/armv7m/sysdepend.h"

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(UB pll_mode);
IMPORT void shutdown_clock(void);

#endif /* _SYSDEPEND_CPU_SYSDEPEND_ */