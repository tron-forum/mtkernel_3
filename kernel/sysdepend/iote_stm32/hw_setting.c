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
#ifdef IOTE_STM32

/*
 *	hw_setting.c (STM32 IoT-Engine)
 *	startup / shoutdown processing for hardware
 */

#include "kernel.h"
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* 
 * Setup register data 
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

/*
 * Setup module clock
 */
LOCAL const T_SETUP_REG modclk_tbl[] = {
	{RCC_AHB2ENR,		0x00000008},	// GPIOD enable
	{RCC_APB1ENR1,		0x00020000},	// USART2 enable
	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {
	// Serial debug I/F : GPIOD5 -> USART2_TX, GPIOD6 -> USART2_RX
	// LED Output : GPIOD11, GPIOD15
	{GPIO_AFRL(D),		0x07700000},	
	{GPIO_MODER(D),		0x40402800},
	{GPIO_OSPEEDR(D),	0x00001400},
	{GPIO_OTYPER(D),	0x00000000},
	{GPIO_PUPDR(D),		0x00001400},
	{0, 0}
};

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	startup_clock(CLKATR_HSE | CLKATR_USE_PLL | CLKATR_LATENCY_4);

	/* Startup module clock */
	for(p = modclk_tbl; p->addr != 0; p++) {
		*(_UW*)(p->addr) = p->data;
		while(*(_UW*)(p->addr) != p->data);
	}

	/* Setup Pin Function */
	for(p = pinfnc_tbl; p->addr != 0; p++) {
		*(_UW*)(p->addr) = p->data;
	}
}

#if USE_SHUTDOWN
/*
 * Shutdown device
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start device
 *	mode = -1		reset and re-start	(Reset -> Boot -> Start)
 *	mode = -2		fast re-start		(Start)
 *	mode = -3		Normal re-start		(Boot -> Start)
 */
EXPORT ER knl_restart_hw( W mode )
{
	switch(mode) {
	case -1: /* Reset and re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESET & RESTART >>\n");
		return E_NOSPT;
	case -2: /* fast re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM FAST RESTART >>\n");
		return E_NOSPT;
	case -3: /* Normal re-start */
		SYSTEM_MESSAGE("\n<< SYSTEM RESTART >>\n");
		return E_NOSPT;
	default:
		return E_PAR;
	}
}


#endif /* IOTE_STM32 */