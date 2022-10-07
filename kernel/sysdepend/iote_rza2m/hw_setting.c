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
#ifdef IOTE_RZA2M

/*
 *	hw_setting.c (RZ/A2M IoT-Engine)
 *		hardware settings
 *
 *	startup / shoutdown processing for hardware
 *	
 *	Pin function Setting (for IoT-Engine Starter board)
 *		P90  : TxD4
 *		PB1  : RxD4
 *
 *		(USE_SDEV_DRV)
 *		P50  : AN000 (Arduino A0)
 *		P51  : AN001 (Arduino A1)
 *		P52  : AN002 (Arduino A2)
 *		P53  : AN003 (Arduino A3)
 *		P54  : AN004 (Arduino A4)
 *		P55  : AN005 (Arduino A5)
 *		PD0  : RIIC0 SCL (Arduino I2C)
 *		PD1  : RIIC0 SDA (Arduino I2C)
 *		PD6  : RIIC3 SCL (On Board)
 *		PD7  : RIIC3 SDA (On Board)
 *
 */

#include "kernel.h"
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* 
 * Setup register data 
 */
typedef struct {
	UW	addr;
	UB	data;
} T_SETUP_REG;

/* 
 * Setup module stop Tadle
 */
LOCAL const T_SETUP_REG stbcr_tbl[] = {
#if !USE_SDEV_DRV	// Do not use sample device driver
	{CPG_STBCR3 , 0b10001101},	/* Enable OSTM0, OSTM1, OSTM2 */
	{CPG_STBCR4 , 0b11110111},	/* Enable SCIF4 */

#else			// Use the sample device driver
	{CPG_STBCR3 , 0b10001101},	/* Enable OSTM0, OSTM1, OSTM2 */
	{CPG_STBCR4 , 0b11110111},	/* Enable SCIF4 */
	{CPG_STBCR5 , 0b01110011},	/* Enable A/D, RTC0, RTC1 */
	{CPG_STBCR8 , 0b01100111},	/* Enable RIIC0, RIIC3, SPIBSC */

#endif

/* When all devices are enabled */
//	{CPG_STBCR2 , 0x6A},	/* Port level is keep in standby mode, [1], [1], [0], [1], [0], [1], CoreSight */
//	{CPG_STBCR3 , 0x80},	/* [1], OSTM0, OSTM1, OSTM3, MTU3, CAN-FD, [0], GPT */
//	{CPG_STBCR4 , 0x00},	/* SCIF0, SCIF1, SCIF2, SCIF3, SCIF4, SCI0, SCI1,IrDA */
//	{CPG_STBCR5 , 0x31},	/* A/D, CEU, [1], [1], RTC0, RTC1, JCU, [1] */
//	{CPG_STBCR6 , 0x80},	/* [1], VIN, ETHER0, ETHER1, EtherPTR, EtherM, USB0, USB1 */
//	{CPG_STBCR7 , 0x10},	/* IMR-LS2, DAVE-2D, MIPI, [1], SSIF0, SSIF1, SSIF2, SSIF3 */
//	{CPG_STBCR8 , 0x05},	/* IIC0, IIC1, IIC2, IIC3, SPIBSC, [1], VDC6, [1] */
//	{CPG_STBCR9 , 0x11},	/* RSPI0, RSPI1, RSPI2, [1], HYPER, OCTA, SPDIF, DRP */
//	{CPG_STBCR10, 0x00},	/* TSIP, [0], [0], NAND, SDHI00, SDHI01, SDHI10, SDHI11 */
//	{CPG_STBCR11, 0x3F},	/* POE3, POEG, [1], [1], [1], [1], [1] , [1] */

	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pfunc_tbl[] = {
#if !USE_SDEV_DRV	// Do not use sample device driver
	{PORT9n_PFS(0), 0x04},		// P90 -> SCIFA.TxD4
	{PORT9n_PFS(1), 0x04},		// P91 -> SCIFA.RxD4

#else			// Use the sample device driver
	{PORT5n_PFS(0), 0x01},		// P50 = AN000
	{PORT5n_PFS(1), 0x01},		// P51 = AN001
	{PORT5n_PFS(2), 0x01},		// P52 = AN002
	{PORT9n_PFS(0), 0x04},		// P90 -> SCIFA.TxD4
	{PORT9n_PFS(1), 0x04},		// P91 -> SCIFA.RxD4
	{PORTDn_PFS(0), 0x01},		// PD0 -> RIIC0.SCL
	{PORTDn_PFS(1), 0x01},		// PD1 -> RIIC0.SDA
	{PORTDn_PFS(6), 0x01},		// PD6 -> RIIC3.SCL
	{PORTDn_PFS(7), 0x01},		// PD7 -> RIIC3.SDA

#endif
	{0, 0}
};

/* 
 * Setup port mode Tadle
 */
LOCAL const T_SETUP_REG pmode_tbl[] = {
#if !USE_SDEV_DRV	// Do not use sample device driver
	{PORT9_PMR, 0b00000011},	// P90-P91 peripheral function

#else			// Use the sample device driver
	{PORT5_PMR, 0b00000111},	// P50-P52 peripheral function
	{PORT9_PMR, 0b00000011},	// P90-P91 peripheral function
	{PORTD_PMR, 0b11000011},	// PD0-PD1, PD6-PD7 peripheral function

#endif
	{0, 0}
};

/*
 * Startup hardware
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;
	_UB			dummy_b;

	/* Startup System Clock (CPG settings) */
	startup_clock();

	/* Setting the clock supply to each module */
	for(p = stbcr_tbl; p->addr != 0; p++) {
		out_b(p->addr, p->data);
		dummy_b = in_b(p->addr);
	}
	dummy_b;

	/* Setup port mode */
	for(p = pmode_tbl; p->addr != 0; p++) {
		or_b(p->addr, p->data);
	}

	/* Setup Pin Function */
	out_b(PORT_PWPR, 0);
	out_b(PORT_PWPR, PORT_PWPR_PFSWE);		/* Allow writing to PFS */
	for(p = pfunc_tbl; p->addr != 0; p++) {
		out_b(p->addr, p->data);
	}
	out_b(PORT_PWPR, PORT_PWPR_B0WI);		/* Prohibit writing to PFS */

}

#if USE_SHUTDOWN
/*
 * Shutdown hardware
 */
EXPORT void knl_shutdown_hw( void )
{
	disint();
	shutdown_clock();

	while(1);
}
#endif /* USE_SHUTDOWN */

/*
 * Re-start hardware
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

#endif	/* IOTE_RZA2M */