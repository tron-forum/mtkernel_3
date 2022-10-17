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
#ifdef IOTE_RX231

/*
 *	hw_setting.c (RX231 IoT-Engine)
 *	startup / shoutdown processing for hardware
 *	
 *	Pin function Setting (for IoT-Engine Starter board)
 *		PB0  : SCI6.RXD6
 *		PB1  : SCI6.TXD6
 *
 *		(USE_SDEV_DRV)
 *		P16  : SCL0 (Arduino I2C)
		P17  : SDA0 (Arduino I2C)
		P40  : AN000 (Analog-SW/Arduino A0)
		P41  : AN001 (Analog-SW/Arduino A1)
		P42  : AN002 (Temp sensor/Arduino A2)
 *
 */

#include "kernel.h"
#include <tm/tmonitor.h>

#include "sysdepend.h"

/* ------------------------------------------------------------------------ */
/* 
 * Setup register data 
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

/* 
 * Setup module stop Tadle
 */
LOCAL const T_SETUP_REG mstop_tbl[] = {
#if !USE_SDEV_DRV	// Do not use sample device driver
	{ MSTPCRA, 0xEFFF7FCF },	/* Enable DMAC/DTC, CMT0-1, TMR0-3 */
	{ MSTPCRB, 0xFDFFFFFF },	/* Enable SCI6 */
	{ MSTPCRC, 0x7FFF0000 },	/* Disable Deep-Sleep mode, Enable RAM */
	{ MSTPCRD, 0xFFFFFF00 },

#else			// Use the sample device driver
	{ MSTPCRA, 0xEFFD7FCF },	/* Enable DMAC/DTC, ADC, CMT0-1, TMR0-3 */
	{ MSTPCRB, 0xFDDFFFFF },	/* Enable SCI6, RIIC0 */
	{ MSTPCRC, 0x7FFF0000 },	/* Disable Deep-Sleep mode, Enable RAM */
	{ MSTPCRD, 0xFFFFFF00 },

#endif /* !USE_SDEV_DRV */
	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {
	{MPC_PBnPFS(0), 0x0B},		/* PB0 = SCI6.RXD6 */
	{MPC_PBnPFS(1), 0x0B},		/* PB1 = SCI6.TXD6 */

#if USE_SDEV_DRV	// Use the sample device driver
	{MPC_P1nPFS(6), 0x0F},		/* P16 = SCL0 */
	{MPC_P1nPFS(7), 0x0F},		/* P17 = SDA0 */
	{MPC_P4nPFS(0), 0x80},		/* P40 = AN000 */
	{MPC_P4nPFS(1), 0x80},		/* P41 = AN001 */
	{MPC_P4nPFS(2), 0x80},		/* P42 = AN002 */

#endif /* USE_SDEV_DRV */
	{0, 0}
};

/* 
 * Setup port mode Tadle
 */
LOCAL const T_SETUP_REG portmode_tbl[] = {
	{PORTB_PMR, 0x03},		/* Set PB0&PB1 as a peripheral function. */

#if USE_SDEV_DRV	// Use the sample device driver
	{PORT1_PMR, 0xC0},		// P16-P17 peripheral function
	{PORT1_ODR1, 0x50},		// P16-O17 open drain
	{PORT4_PMR, 0x00},		/* P40-P42 General-purpose i/o port */
	{PORT4_PDR, 0x00},		/* P40-P42 input port */
#endif /* USE_SDEV_DRV */

	{0, 0}
};

/*
 * Startup hardware
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	/* Setup module stop */
	out_h(SYSTEM_PRCR, 0xA502);		/* Disable Register Protect */
	for(p = mstop_tbl; p->addr != 0; p++) {
		out_w(p->addr, p->data);
	}
	out_h(SYSTEM_PRCR, 0xA500);		/* Enable Register protect */

	/* Setup Pin Function */
	out_b(MPC_PWPR, 0);
	out_b(MPC_PWPR, MPC_PWMR_PFSWE);	/* Disable Register Protect */
	for(p = pinfnc_tbl; p->addr != 0; p++) {
		out_b(p->addr, (UB)p->data);
	}
	out_b(MPC_PWPR, MPC_PWMR_B0WI);		/* Enable Register protect */

	/* Setup port mode */
	for(p = portmode_tbl; p->addr != 0; p++) {
		out_b(p->addr, (UB)p->data);
	}	

	startup_clock();			/* Startup CPU Clock */
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


#endif /* IOTE_RX231 */