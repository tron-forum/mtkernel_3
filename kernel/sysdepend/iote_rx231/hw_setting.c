/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/ .
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#ifdef IOTE_RX231

/*
 *	hw_setting.c (RX231 IoT-Engine)
 *		hardware settings
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
	{ MSTPCRA, MSTPCRA_INI },
	{ MSTPCRB, MSTPCRB_INI },
	{ MSTPCRC, MSTPCRC_INI },
	{ MSTPCRD, MSTPCRD_INI },
	{0, 0}
};

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {
	{MPC_PBnPFS(0), 0x0B},		/* PB0 = SCI6.RXD6 */
	{MPC_PBnPFS(1), 0x0B},		/* PB1 = SCI6.TXD6 */
	{0, 0}
};

/* 
 * Setup port mode Tadle
 */
LOCAL const T_SETUP_REG portmode_tbl[] = {
	{PORTB_PMR, 0x03},		/* Set PB0&PB1 as a peripheral function. */
	{PORTB_PCR, 0x03},
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