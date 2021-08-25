/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/08.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef IOTE_RZA2M

/*
 *	hw_setting.c (RZ/A2M IoT-Engine)
 *		hardware settings
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

/* Setting the clock supply to each module */
LOCAL const T_SETUP_REG stbcr_tbl[] = {
	{CPG_STBCR3 , 0b10001101},	/* [1], OSTM0, OSTM1, OSTM2, MTU3, CAN-FD, [0], GPT */
	{CPG_STBCR4 , 0b11110111},	/* SCIF0, SCIF1, SCIF2, SCIF3, SCIF4, SCI0, SCI1,IrDA */
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

/* Pin mode Tadle */
LOCAL const T_SETUP_REG pmode_tbl[] = {
	// Serial debug I/F : P90 -> TxD4, P91 -> RxD4
	{PORT9_PMR, 0b00000011},

	{0, 0}
};

/* Pin function Tadle */
LOCAL const T_SETUP_REG pfunc_tbl[] = {
	// Serial debug I/F : P90 -> TxD4, P91 -> RxD4
	{PORT9n_PFS(0), 0x04},
	{PORT9n_PFS(1), 0x04},

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

	/* Pin mode selection */
	for(p = pmode_tbl; p->addr != 0; p++) {
		or_b(p->addr, p->data);
	}

	/* Pin function selection */
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