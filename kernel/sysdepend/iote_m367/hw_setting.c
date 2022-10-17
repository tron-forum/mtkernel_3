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
 *	hw_setting.c (M367 IoT-Engine)
 *	startup / shoutdown processing for hardware
 *	
 *	Pin function Setting (for IoT-Engine Starter board)
 *		PA0 ~ PA3 : Debug I/F
 *		PA4  : UART RTS5
 *		PA5  : UART RXD5
 *		PA6  : UART TXD5
 *		PA7  : UART CTS5
 *
 *		(USE_SDEV_DRV)
 *		PB0  : GPIO out (LED4)
 *		PF3  : GPIO out (LED3)
 *		PF6  : I2C SCL1
 *		PF7  : I2C SDA1
 *		PL1  : GPIO out (I2C Enable)
 *
 * 		(A/DC)
 * 		PI0  ; AIN A0 (Light sensor)
 *		PI2  : AIN A2 (Ardino A2)
 * 		PI3  : AIN A3 (Ardino A1)
  * 		PI4  : AIN B0 (Ardino A0)
 * 		PI5  : AIN B1 (Temp sensor)
 * 		PI6  : AIN B2 (Analog SW)
 * 		PI7  ; AIN B3 (Analog SW)
 */

#include "kernel.h"
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* 
 * Setup pin functions Tadle
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

LOCAL const T_SETUP_REG setup_regs[] = {

#if !USE_SDEV_DRV	// Do not use device sample driver
	//  Debugger I/F : PA0 ~ PA3
	//  Serial debug I/F : PA4 -> RTS5, PA5 -> RXD5, PA6 -> TXD5, PA7 -> CTS5
	{PORT_FR1(A),	0x0000000F},
	{PORT_FR2(A),	0x000000F0},
	{PORT_PUP(A),	0x0000000A},
	{PORT_CR(A),	0x00000053},
	{PORT_IE(A),	0x000000AE},

#else			// Use the device sample driver
	// Port Output : PB0
	{PORT_DATA(B),	0x00000000},
	{PORT_CR(B),	0x00000001},

	//  Debugger I/F : PA0 ~ PA3
	//  Serial debug I/F : PA4 -> RTS5, PA5 -> RXD5, PA6 -> TXD5, PA7 -> CTS5
	{PORT_FR1(A),	0x0000000F},
	{PORT_FR2(A),	0x000000F0},
	{PORT_PUP(A),	0x0000000A},
	{PORT_CR(A),	0x00000053},
	{PORT_IE(A),	0x000000AE},

	// I2C I/F : PF6 -> SCL1, PF7 -> SDA1
	// Port Output : PF3
	{PORT_FR4(F),	0x000000C0},
	{PORT_OD(F),	0x000000C0},
	{PORT_DATA(F),	0x00000000},
	{PORT_CR(F),	0x000000C8},
	{PORT_IE(F),	0x000000C0},

	// PL1	P-OUT	I2C_ENABLE
	{PORT_DATA(L),	0x00000000},
	{PORT_CR(L),	0x00000002},

#endif /* USE_SDEV_DRV */

	{0, 0}
};

/*
 * Startup Device
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	/* Disable Watch Dog Timer */
	*(_UW*)WDOG_WDMOD &= ~0x00000080;
	*(_UW*)WDOG_WDCR = WDOG_WDCR_DISABLE;

	startup_clock(PLL_MODE_6X);

	/* Setup Pin Function */
	for(p = setup_regs; p->addr != 0; p++) {
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


#endif /* IOTE_M367 */