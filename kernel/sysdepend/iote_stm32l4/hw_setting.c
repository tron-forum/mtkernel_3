/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef IOTE_STM32L4

/*
 *	hw_setting.c (STM32L4 IoT-Engine)
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

#if !USE_SDEV_DRV	// Do not use sample device driver
	{RCC_AHB2ENR,		0x00000008},	// GPIO-D enable
	{RCC_APB1ENR1,		0x0002000F},	// USART2, TIM2-TIM5 enable
	{RCC_APB2ENR,		0x00000001},	// SYSCFG enable

#else			// Use the sample device driver
	{RCC_AHB2ENR,		0x0000000B},	// GPIO-A, B, D enable
	{RCC_APB1ENR1,		0x0002000F},	// USART2, TIM2-TIM5 enable
	{RCC_APB2ENR,		0x00000001},	// SYSCFG enable
#endif /* !USE_SDEV_DRV */

	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {

#if !USE_SDEV_DRV	// Do not use device sample driver
	// Serial debug I/F : PD5 -> USART2_TX, PD6 -> USART2_RX
	{GPIO_MODER(D),		0x00002800},
	{GPIO_OTYPER(D),	0x00000000},
	{GPIO_OSPEEDR(D),	0x00001400},
	{GPIO_PUPDR(D),		0x00001400},
	{GPIO_AFRL(D),		0x00000000},
	{GPIO_AFRL(D),		0x07700000},

#else			// Use the device sample driver
	// A/DC in: PA1,PA2,PA4
	{GPIO_MODER(A),		0xABFFFFFF},
	{GPIO_OTYPER(A),	0x00000000},
	{GPIO_OSPEEDR(A),	0x0C0F0000},
	{GPIO_PUPDR(A),		0x64050000},
	{GPIO_AFRL(A),		0x00000000},
	{GPIO_AFRH(A),		0x00000000},
	{GPIO_ASCR(A),		0x00000016},	

	// I2C I/F : PB8 -> I2C1_SCL, PB9 -> I2C1_SDA
	// A/DC in : PB1
	{GPIO_MODER(B),		0xFFFAFFBF},
	{GPIO_OTYPER(B),	0x00000300},
	{GPIO_OSPEEDR(B),	0x000F0000},
	{GPIO_PUPDR(B),		0x00050100},
	{GPIO_AFRL(B),		0x00000000},
	{GPIO_AFRH(B),		0x00000044},
	{GPIO_ASCR(B),		0x00000002},

	// Serial debug I/F : PD5 -> USART2_TX, PD6 -> USART2_RX
	// Port Output : PD9, PD11, PD15
	{GPIO_MODER(D),		0x40442800},
	{GPIO_OTYPER(D),	0x00000000},
	{GPIO_OSPEEDR(D),	0x00001400},
	{GPIO_PUPDR(D),		0x00001400},
	{GPIO_AFRL(D),		0x00000000},
	{GPIO_AFRL(D),		0x07700000},

#endif /* USE_SDEV_DRV */

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


#endif /* IOTE_STM32L4 */