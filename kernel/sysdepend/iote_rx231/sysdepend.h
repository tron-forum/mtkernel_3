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
 *	sysdepend.h (RX231 IoT-Engine)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_TARGET_SYSDEPEND_
#define _SYSDEPEND_TARGET_SYSDEPEND_

#include "../cpu/rx231/sysdepend.h"

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(void);
IMPORT void shutdown_clock(void);

#endif /* _SYSDEPEND_TARGET_SYSDEPEND_ */