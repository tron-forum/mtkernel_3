/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06.B0
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_STM32H7

/*
 *	cpu_clock.c (STM32H7)
 *	Clock Setting
 */

#include <tk/tkernel.h>

#include "sysdepend.h"

/*
 *  Startup System Clock
 */
EXPORT void startup_clock(ATR clkatr)
{
	/* Supply configuration update enable */
	out_w(PWR_CR3, (in_w(PWR_CR3)&~(0x00000003)) | PWR_CR3_LDOEN);
	while((in_w(PWR_CSR1) & PWR_CSR1_ACTVOSRDY) == 0);

	/* Configure the main internal regulator output voltage */
	out_w(PWR_D3CR, PWR_D3CR_INIT);
	in_w(PWR_D3CR);
	while((in_w(PWR_D3CR) & PWR_D3CR_VOSRDY) == 0);

	/* Enable clock source */
	/* Use HSI clock */
	if( clkatr & CLKATR_HSI ) {
		*(_UW*)RCC_CR |= RCC_CR_HSION;				// HSI enable
		while((*(_UW*)RCC_CR & RCC_CR_HSIRDY) == 0 );		// Wait HSI ready
	}

	/* Use CSI clock */
	if( clkatr & CLKATR_CSI ) {
		*(_UW*)RCC_CR |= RCC_CR_CSION;				// CSI enable
		while((*(_UW*)RCC_CR & RCC_CR_CSIRDY) == 0 );		// Wait CSI ready
	}

	/* Use MSE clock */
	if( clkatr & CLKATR_HSE ) {
		if( clkatr & CLKATR_HSEBYP) *(_UW*)RCC_CR |= RCC_CR_HSEBYP;	// HSE clock bypass
		*(_UW*)RCC_CR |= RCC_CR_HSEON;				// HSE enable
		while( (*(_UW*)RCC_CR & RCC_CR_HSERDY) == 0 );		// Wait HSE ready
	}

	/* Use HSI48 clock */
	if( clkatr & CLKATR_HSI48 ) {
		*(_UW*)RCC_CR |= RCC_CR_HSI48ON;			// HSI48 enable
		while( (*(_UW*)RCC_CR & RCC_CR_HSI48RDY) == 0 );	// Wait HSI48 ready
	}

	/* PLL1 Configuration */
	if(clkatr & CLKATR_PLL1) {
		*(_UW*)RCC_CR &= ~RCC_CR_PLL1ON;		// PLL1 disable
		while( (*((_UW*)RCC_CR) & RCC_CR_PLL1RDY) != 0 );	// Wait PLL1 ready

		out_w(RCC_PLLCKSELR, RCC_PLLCKSELR_INIT);	// PLLs clock source selection
		out_w(RCC_PLL1DIVR, RCC_PLL1DIVR_INIT);		// PLL1 dividers configuration

		*(_UW*)RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL1FRACEN;	// PLL1 fractional latch disable
	
		out_w(RCC_PLL1FRACR, RCC_PLL1FRACR_INIT);	// PLL1 fractional divider
		out_w(RCC_PLLCFGR, RCC_PLLCFGR_INIT);		// PLLs Configuration
	
		*(_UW*)RCC_PLLCFGR |= RCC_PLLCFGR_PLL1FRACEN;	// PLL1 fractional latch enable
	
		*(_UW*)RCC_CR |= RCC_CR_PLL1ON;			// PLL1 eneble
		while( (*((_UW*)RCC_CR) & RCC_CR_PLL1RDY) == 0 );	// Wait PLL1 ready
	}

	/* Prescaler configuration */
	out_w(RCC_D1CFGR, RCC_D1CFGR_INIT);	// D1 domain AHB & Core prescaler(HCLK & SYSCLK)
	out_w(RCC_D2CFGR, RCC_D2CFGR_INIT);	// D2 domain APB1&2 prescaler (PCLK1&2)
	out_w(RCC_D3CFGR, RCC_D3CFGR_INIT);	// D3 domain APB4 prescaler (D3PCLK1)

	/* Set Flash Memory Access latency  */
	out_w(FLASH_ACR, FLASH_ACR_INIT);	// FLASH Read latency
	while(in_w(FLASH_ACR) != FLASH_ACR_INIT);

	/* Set system clock switch */
	out_w(RCC_CFGR, (in_w(RCC_CFGR) & ~RCC_CFGR_SW) | RCC_CFGR_SW_INIT);
	while(((in_w(RCC_CFGR) & RCC_CFGR_SWS) >>3) != RCC_CFGR_SW_INIT) ;

	/* PLL2 Configuration */
	if(clkatr & CLKATR_PLL2) {
		*(_UW*)RCC_CR &= ~RCC_CR_PLL2ON;			// PLL2 disable
		while( (*((_UW*)RCC_CR) & RCC_CR_PLL2RDY) != 0 );	// Wait PLL2 ready

		out_w(RCC_PLLCKSELR, (in_w(RCC_PLLCKSELR) & ~RCC_PLLCKSELR_DIVM2)|(RCC_PLLCKSELR_DIVM2_INIT<<12));
		out_w(RCC_PLL2DIVR, RCC_PLL2DIVR_INIT);

		*(_UW*)RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL2FRACEN;	// PLL2 fractional latch disable

		out_w(RCC_PLL2FRACR, RCC_PLL2FRACR_INIT);	// PLL2 fractional divider
		out_w(RCC_PLLCFGR, (in_w(RCC_PLLCFGR) & ~(RCC_PLLCFGR_PLL2RGE | RCC_PLLCFGR_PLL2VCOSEL))
					|((RCC_PLLCFGR_PLL2RGE_INIT<<6)|(RCC_PLLCFGR_PLL2VCOSEL_INIT<<5)));

		*(_UW*)RCC_PLLCFGR |= RCC_PLLCFGR_PLL2FRACEN;	// PLL2 fractional latch enable
		
		*(_UW*)RCC_CR |= RCC_CR_PLL2ON;				// PLL2 Enable
		while( (*((_UW*)RCC_CR) & RCC_CR_PLL2RDY) != 0 );	// Wait PLL2 ready
	}

	/* PLL3 Configuration */
	if(clkatr & CLKATR_PLL3) {
		*(_UW*)RCC_CR &= ~RCC_CR_PLL3ON;			// PLL3 disable
		while( (*((_UW*)RCC_CR) & RCC_CR_PLL3RDY) != 0 );	// Wait PLL3 ready

		out_w(RCC_PLLCKSELR, (in_w(RCC_PLLCKSELR) & ~RCC_PLLCKSELR_DIVM3)|(RCC_PLLCKSELR_DIVM3_INIT<<20));
		out_w(RCC_PLL3DIVR, RCC_PLL3DIVR_INIT);

		*(_UW*)RCC_PLLCFGR &= ~RCC_PLLCFGR_PLL3FRACEN;	// PLL3 fractional latch disable

		out_w(RCC_PLL3FRACR, RCC_PLL3FRACR_INIT);	// PLL3 fractional divider
		out_w(RCC_PLLCFGR, (in_w(RCC_PLLCFGR) & ~(RCC_PLLCFGR_PLL3RGE | RCC_PLLCFGR_PLL3VCOSEL))
					|((RCC_PLLCFGR_PLL3RGE_INIT<<10)|(RCC_PLLCFGR_PLL2VCOSEL_INIT<<9)));

		*(_UW*)RCC_PLLCFGR |= RCC_PLLCFGR_PLL3FRACEN;	// PLL3 fractional latch enable
		
		*(_UW*)RCC_CR |= RCC_CR_PLL3ON;				// PLL3 Enable
		while( (*((_UW*)RCC_CR) & RCC_CR_PLL3RDY) != 0 );	// Wait PLL3 ready
	}

	/* Disable all interrupts */
	out_w(RCC_CIER, 0);
}

EXPORT void shutdown_clock(void)
{

}

#endif /* CPU_STM32H7 */
