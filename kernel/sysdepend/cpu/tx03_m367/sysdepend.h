/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h (TX03-M367)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_SYSDEPEND_
#define _SYSDEPEND_CPU_SYSDEPEND_

#include "../core/acm3/sysdepend.h"

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void enable_pll(UB pll_mode);
IMPORT void disable_pll();

#endif /* _SYSDEPEND_CPU_SYSDEPEND_ */