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
#ifdef IOTE_STM32L4

/*
 *	cpu_clock.c (STM32L4 IoT-Engine)
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

	/* Enable clock source */
	/* Use HSI clock */
	if( clkatr & CLKATR_HSI ) {
		*(_UW*)RCC_CR |= RCC_CR_HSION;			// HSI enable
		while((*(_UW*)RCC_CR & RCC_CR_HSIRDY) == 0 );	// Wait HSI ready
	}
	/* Use HSE clock */
	if( clkatr & CLKATR_HSE ) {
		*(_UW*)RCC_CR |= RCC_CR_HSEON;			// HSE enable
		while( (*(_UW*)RCC_CR & RCC_CR_HSERDY) == 0 );	/* Wait HSE ready */
	}
	/* Use MSI clock */
	if(clkatr & CLKATR_MSI) {
		*(_UW*)RCC_CR |= RCC_CR_MSION;			// MSI enable
		while((*(_UW*)RCC_CR & RCC_CR_MSIRDY) == 0 );	// Wait MSI ready
	}

	if(clkatr & CLKATR_USE_PLL) {		/* PLL Configuration */
		*(_UW*)RCC_CR &= ~RCC_CR_PLLON;			// Disable PLL
		while((*(_UW*)RCC_CR & RCC_CR_PLLRDY) != 0 );	// Wait PLL disable

		out_w(RCC_PLLCFGR, (RCC_PLLCFGR_INIT & ~RCC_PLLCFGR_PLLSRC) | RCC_PLLCFGR_PLLSRC_INIT);	// Set PLL

		*(_UW*)RCC_CR |= RCC_CR_PLLON;			// Enable PLL
		*(_UW*)RCC_PLLCFGR |= RCC_PLLCFGR_PLLREN;	// Enable PLL System Clock output
		while((*(_UW*)RCC_CR & RCC_CR_PLLRDY) == 0);	// Wait PLL ready
	}

	if(clkatr & CLKATR_USE_PLLSAI1) {	/* PLLSAI1 Configuration */
		*(_UW*)RCC_CR &= ~RCC_CR_PLLSAI1ON;		// Disable PLLSAI1
		while((*(_UW*)RCC_CR & RCC_CR_PLLSAI1RDY) != 0 );	// Wait PLLSAI1 disable

		out_w(RCC_PLLSAI1CFGR, RCC_PLLSAI1CFGR_INIT);	// Set PLLSAI1

		*(_UW*)RCC_CR |= RCC_CR_PLLSAI1ON;		// Enable PLLSAI1
		while((*(_UW*)RCC_CR & RCC_CR_PLLSAI1RDY) == 0);	// Wait PLLSAI1 ready
	}

	if(clkatr & CLKATR_USE_PLLSAI2) {	/* PLLSAI2 Configuration */
		*(_UW*)RCC_CR &= ~RCC_CR_PLLSAI2ON;		// Disable PLLSAI2
		while((*(_UW*)RCC_CR & RCC_CR_PLLSAI2RDY) != 0 );	// Wait PLLSAI2 disable

		out_w(RCC_PLLSAI1CFGR, RCC_PLLSAI2CFGR_INIT);	// Set PLLSAI2

		*(_UW*)RCC_CR |= RCC_CR_PLLSAI2ON;		// Enable PLLSAI2
		while((*(_UW*)RCC_CR & RCC_CR_PLLSAI2RDY) == 0);	// Wait PLLSAI2 ready
	}

	/* Set Flash Memory Access latency  */
	f_ratency = (clkatr & CLKATR_LATENCY_MASK)>>8;
	*(_UW*)FLASH_ACR = (*(_UW*)FLASH_ACR & ~FLASH_ACR_LATENCY_MASK)| FLASH_ACR_LATENCY(f_ratency);
	while( (*(_UW*)FLASH_ACR & FLASH_ACR_LATENCY_MASK) != FLASH_ACR_LATENCY(f_ratency) );

	/* Clock setting */
	out_w(RCC_CFGR, (RCC_CFGR_INIT & ~RCC_CFGR_SW) | RCC_CFGR_SW_INIT);
	while((*(_UW*)RCC_CFGR & RCC_CFGR_SW) != RCC_CFGR_SW_INIT);

	/* Disable all interrupts */
	out_w(RCC_CIER, 0);
}

EXPORT void shutdown_clock(void)
{

}

#endif /* IOTE_STM32L4 */