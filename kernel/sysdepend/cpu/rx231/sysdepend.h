/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03/13.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h (RX231)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_SYSDEPEND_
#define _SYSDEPEND_CPU_SYSDEPEND_

#include "../core/rxv2/sysdepend.h"

/*
 *    Interrupt Vector Table (ROM) (intvect_tbl.c)
 */
IMPORT const void (*knl_int_vect_rom[])();

/* ------------------------------------------------------------------------ */
/*
 * HLL Interrupt Handler Table (ROM) (hllint_tbl.c)
 * 
 */
IMPORT const void (*knl_hll_inthdr_rom[])();

/*
 *    Clock Setting (cpu_clock.c)
 */
IMPORT void startup_clock(void);
IMPORT void shutdown_clock(void);

#endif /* _SYSDEPEND_CPU_SYSDEPEND_ */