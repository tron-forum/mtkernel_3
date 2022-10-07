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
 *	sysdepend.h (RX231)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_SYSDEPEND_
#define _SYSDEPEND_CPU_SYSDEPEND_

#include "../core/rxv2/sysdepend.h"

/*
 *    Interrupt Vector Table (ROM) (intvect_tbl.c)
 */
IMPORT void (* const knl_int_vect_rom[])();

/* ------------------------------------------------------------------------ */
/*
 * HLL Interrupt Handler Table (ROM) (hllint_tbl.c)
 * 
 */
IMPORT void (* const knl_hll_inthdr_rom[])();

#endif /* _SYSDEPEND_CPU_SYSDEPEND_ */