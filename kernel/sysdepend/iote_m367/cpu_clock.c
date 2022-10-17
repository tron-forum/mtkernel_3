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
#ifdef IOTE_M367

/*
 *	cpu_clock.c (M367 IoT-Engine)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 *  Startup System Clock
 *    Input clock = 12MHz  Output clock = 72MHz(PLL_MODE_6X) or 48MHz(PLL_MODE_4X)
 *    **** This implementation assumes 6 multiplication.
 */
EXPORT void startup_clock(UB pll_mode)
{
	_UW	*osccr	= (_UW*)CLKCTRL_CGOSCCR;

	/* Unprotect CG . */
	*(_UW*)CLKCTRL_CGPROTECT = 0xC1;
	

	*osccr = (*osccr | ((1 << 19) | (1 << 18) | (1 << 8))) & 0xFFFFFFF7;
	
	/* Warming up = 100usec */
	*osccr = (*osccr & 0x000FFFFF) | (0x64 << 20) | (1);
	while( (*osccr & CLKCTRL_CGOSCCR_WUEF) != 0 ) {
		;
	}
	
	/* External oscillator select  */
	do{
		*osccr |= (1 << 17);
	} while((*osccr & (1 << 17)) == 0);
	
	/* Internal oscillator off */
	*osccr &= ~(1 << 16);
	
	
	/* PLLON = off */
	*osccr &= 0xFFFFFFFB;

	/* Specify the number of PLL multiplier factors */
	if(pll_mode == PLL_MODE_4X) {
		*(_UW*)CLKCTRL_CGPLLSEL = CLKCTRL_CGPLLSEL_4X << 1;
	}
	else if(pll_mode == PLL_MODE_6X) {
		*(_UW*)CLKCTRL_CGPLLSEL = CLKCTRL_CGPLLSEL_6X << 1;
	}
	else {		/* invarid pamameter */
		return;
	}
	
	/* Waiting for PLL stablization (100usec) */
	*osccr = (*osccr & 0x000FFFFF) | CLKCTRL_CGOSCCR_WUPT(100, HISPEED_CLOCK_MHz) | (1);
	while( (*osccr & CLKCTRL_CGOSCCR_WUEF) != 0 ) {
		;
	}
	
	/* Enable PLL operation and Wait for PLL stablization (200usec) */
	*osccr = (*osccr & 0x000FFFFF) | (CLKCTRL_CGOSCCR_PLLON |
			    CLKCTRL_CGOSCCR_WUPT(200, HISPEED_CLOCK_MHz) | (1));
	while( (*osccr & CLKCTRL_CGOSCCR_WUEF) != 0 ) {
		;
	}
	
	/* PLL operation select */
	*(_UW*)CLKCTRL_CGPLLSEL |= CLKCTRL_CGPLLSEL_PLLSEL;
	
	/* fgear = fc/2, fperiph = fgear, T0 = fperiph, SCOUT = fsys/2 */
	*(_UW*)CLKCTRL_CGSYSCR = (1 << 16) | (4);
	
	/* Protect CG */
	*(_UW*)CLKCTRL_CGPROTECT = 0xFF;
	
	return;
	
}


EXPORT void shutdown_clock(void)
{

}

#endif /* IOTE_M367 */