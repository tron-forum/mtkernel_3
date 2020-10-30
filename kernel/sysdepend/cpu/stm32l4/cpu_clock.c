/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_STM32L4

/*
 *	cpu_clock.c (STM32L4)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 *  Startup System Clock
 */
EXPORT void startup_clock(ATR clkatr)
{	
	UW	f_ratency;

	/* Select clock */
	/* Use HSI clock */
	if( clkatr & CLKATR_HSI ) {
		*(_UW*)RCC_CR |= RCC_CR_HSION;			// HSI enable
		while((*(_UW*)RCC_CR & RCC_CR_HSIRDY) == 0 );	// Wait HSI ready

	/* Use HSE clock */
	} else if( clkatr & CLKATR_HSE ) {
		*(_UW*)RCC_CR |= RCC_CR_HSEON;			// HSE enable
		while( (*(_UW*)RCC_CR & RCC_CR_HSERDY) == 0 );	/* Wait HSE ready */

	/* Use MSI clock */
	} else {
		*(_UW*)RCC_CR |= RCC_CR_MSION;			// MSI enable
		while((*(_UW*)RCC_CR & RCC_CR_MSIRDY) == 0 );	// Wait MSI ready
	}

	/* PLL  Configuration */
	if(clkatr & CLKATR_USE_PLL) {
		/* PLL Configuration */
		*(_UW*)RCC_CR &= ~RCC_CR_PLLON;			// Disable PLL
		while((*(_UW*)RCC_CR & RCC_CR_PLLRDY) != 0 );	// Wait PLL disable

		out_w(RCC_PLLCFGR, RCC_PLLCFGR_INIT);		// Set PLL

		*(_UW*)RCC_CR |= RCC_CR_PLLON;			// Enable PLL
		*(_UW*)RCC_PLLCFGR |= RCC_PLLCFGR_PLLREN;	// Enable PLL System Clock output
		while((*(_UW*)RCC_CR & RCC_CR_PLLRDY) == 0);	// Wait PLL ready
	}

	/* Set Flash Memory Access latency  */
	f_ratency = (clkatr & CLKATR_LATENCY_MASK)>>8;
	*(_UW*)FLASH_ACR = (*(_UW*)FLASH_ACR & ~FLASH_ACR_LATENCY_MASK)| FLASH_ACR_LATENCY(f_ratency);
	while( (*(_UW*)FLASH_ACR & FLASH_ACR_LATENCY_MASK) != FLASH_ACR_LATENCY(f_ratency) );

	/* Set CFGR register */
	out_w(RCC_CFGR, RCC_CFGR_INIT);
	while((*(_UW*)RCC_CFGR & RCC_CFGR_SW) != (RCC_CFGR_INIT & RCC_CFGR_SW));

	/* Disable all interrupts */
	out_w(RCC_CIER, 0);
}

EXPORT void shutdown_clock()
{

}

#endif /* CPU_STM32L467 */