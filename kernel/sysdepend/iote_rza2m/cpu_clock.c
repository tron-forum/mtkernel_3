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

#include <sys/machine.h>
#ifdef CPU_RZA2M

/*
 *	cpu_clock.c (RZ/A2M)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 *  Startup System Clock (CPG settings)
 */
EXPORT void startup_clock(void)
{
	_UW	dummy_32b;
	_UB	dummy_8b;

	/* standby_mode_en bit of Power Control Register setting */
	*(_UW*)PL310_POWER_CTL |= 0x0001;
	dummy_32b = in_w(PL310_POWER_CTL);
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