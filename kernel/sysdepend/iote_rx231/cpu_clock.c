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

#include <sys/machine.h>
#ifdef IOTE_RX231

/*
 *	cpu_clock.c (RX231 IoT-Engine)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 *  Startup System Clock
 *    Used Main Clock(8MHz), Uesd PLL/UPLL, System Clock 54MHz
 *    ICLK:54MHz, PCLKA:54MHz, PCLKB:27MHz, PCLKD:54MHz, FCLK:1.6875MHz, UCLK:48MHz
 */
EXPORT void startup_clock(void)
{
	out_h(SYSTEM_PRCR, 0xA503);			/* Register Protect Disable */

	out_b(SYSTEM_MOSCWTCR, 0x05);			/* LOCO(4MHz)*16384cyc=4.096ms(Over 3ms) */
	out_b(SYSTEM_MOSCCR, 0x00);			/* Enable Main Clock */

	while(!(in_b(SYSTEM_OSCOVFSR) & 0x01));		/* Wait Main Clock Stabilization */

	out_h(SYSTEM_PLLCR, 0x1A01);			/* PLL 8MHz/2*13.5=54MHz */
	out_b(SYSTEM_PLLCR2, 0x00);			/* Enable PLL */
	while(!(in_b(SYSTEM_OSCOVFSR) & 0x04));		/* Wait PLL Stabilization */

	out_h(SYSTEM_UPLLCR, 0x0B11);			/* UPLL 8MHz/1*6=48MHz */
	out_b(SYSTEM_UPLLCR2, 0x00);			/* Enable UPLL */
	while( !(in_b(SYSTEM_OSCOVFSR) & 0x20));	/* Wait UPLL Stabilization */

	out_b(SYSTEM_OPCCR, 0x00);			/* High Speed Mode */
	while( in_b(SYSTEM_OPCCR) & 0x10);		/* Wait Transition Completed */
	
	out_b(SYSTEM_MEMWAIT, 0x01);			/* Memory Wait States */

	out_w(SYSTEM_SCKCR, 0x50860100);		/* ICLK=PCLKA=PCLKD:54MHz,PCLKB:27MHz,UCLK:48MHz,FCLK:1.6875MHz */
	out_h(SYSTEM_SCKCR3, 0x0400);			/* Select PLL */

	out_b(SYSTEM_LOCOCR, 0x01);			/* Disable LOCO */

	out_h(SYSTEM_PRCR, 0xA500);			/* Register protect Enable */

	return;
}


EXPORT void shutdown_clock(void)
{
}

#endif /* IOTE_RX231 */