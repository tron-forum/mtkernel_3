/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RZA2M

/*
 *	cpu_clock.c (RZ/A2M)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"
#include "iodefine.h"

/*
 *  Startup System Clock (CPG settings)
 */
EXPORT void startup_clock(void)
{
	_UW	dummy_32b;
	_UB	dummy_8b;

	// standby_mode_en bit of Power Control Register setting 
	pl310.REG15_POWER_CTRL.BIT.standby_mode_en = 1;
	dummy_32b = pl310.REG15_POWER_CTRL.LONG;
	(void)dummy_32b;		/* Warning avoidance */

	/* CPG Setting */
	out_h(CPG_FRQCR, CPG_FRQCR_INIVAL);

	/* Writing to On-Chip Data-Retention RAM is enabled. */
	out_b(CPG_SYSCR3,0x0F);
	dummy_8b = in_b(CPG_SYSCR3);
	(void)dummy_8b;		/* Warning avoidance */
}

EXPORT void shutdown_clock(void)
{

}

#endif /* CPU_RZA2M */