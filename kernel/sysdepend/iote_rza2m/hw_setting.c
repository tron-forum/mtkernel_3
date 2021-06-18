/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/ /.
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
 * Setting the clock supply to each module
 */
LOCAL const UB stbcr_tbl[] = {
	0b11111101,	/* STBCR3   [1], OSTM0, OSTM1, OSTM3, MTU3, CAN-FD, [0], GPT */
	0b11111111,	/* STBCR4   SCIF0, SCIF1, SCIF2, SCIF3, SCIF4, SCI0, SCI1,IrDA */
	0b11111111,	/* STBCR5   A/D, CEU, [1], [1], RTC0, RTC1, JCU, [1] */
	0b11111111,	/* STBCR6   [1], VIN, ETHER0, ETHER1, EtherPTR, EtherM, USB0, USB1 */
	0b11111111,	/* STBCR7   IMR-LS2, DAVE-2D, MIPI, [1], SSIF0, SSIF1, SSIF2, SSIF3 */
	0b11111111,	/* STBCR8   IIC0, IIC1, IIC2, IIC3, SPIBSC, [1], VDC6, [1] */
	0b11111111,	/* STBCR9   RSPI0, RSPI1, RSPI2, [1], HYPER, OCTA, SPDIF, DRP */
	0b10011111,	/* STBCR10  TSIP, [0], [0], NAND, SDHI00, SDHI01, SDHI10, SDHI11 */
};

/* 
 * Setup register data 
 */
typedef struct {
	UW	addr;
	UW	data;
} T_SETUP_REG;

/* 
 * Setup pin functions Tadle
 */
LOCAL const T_SETUP_REG pinfnc_tbl[] = {
	{0, 0}
};


/*
 * Startup hardware
 */
EXPORT void knl_startup_hw(void)
{
	const T_SETUP_REG	*p;

	_UW	*p_stbcr;
	_UB	dummy_b;
	UINT	i;

	/* Setting the clock supply to each module */
	for(p_stbcr = (_UW*)CPG_STBCR3, i = 0; p_stbcr <= (_UW*)CPG_STBCR10; p_stbcr++, i++ ) {
		*(_UB*)p_stbcr = stbcr_tbl[i];
		dummy_b = *(_UB*)p_stbcr;
	}

	/* Setup Pin Function */
	for(p = pinfnc_tbl; p->addr != 0; p++) {
		*(_UW*)(p->addr) = p->data;
	}

	/* Setup port mode */

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