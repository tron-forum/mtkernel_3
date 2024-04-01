/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RX65N

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"
#if DEV_SER_ENABLE
/*
 *	ser_rx65n.c
 *	Serial communication device driver
 *	System dependent processing for RX65N
 */

/*----------------------------------------------------------------------
 * Device register base address
 */
const LOCAL UW ba[DEV_SER_UNITNM] = {
	SCI0_BASE, SCI1_BASE, SCI2_BASE, SCI3_BASE, SCI4_BASE, SCI5_BASE, SCI6_BASE, 
	SCI7_BASE, SCI8_BASE, SCI9_BASE, SCI10_BASE, SCI11_BASE, SCI12_BASE
};

#define	SCI_SMR(x)	(ba[x] + SCIx_SMR)	// Serial mode register
#define	SCI_BRR(x)	(ba[x] + SCIx_BRR)	// Bit rate register
#define	SCI_SCR(x)	(ba[x] + SCIx_SCR)	// Serial control register
#define	SCI_SSR(x)	(ba[x] + SCIx_SSR)	// Serial status register
#define	SCI_TDR(x)	(ba[x] + SCIx_TDR)	// Transmit data register
#define	SCI_RDR(x)	(ba[x] + SCIx_RDR)	// Receive data register
#define	SCI_SCMR(x)	(ba[x] + SCIx_SCMR)	// Smart card mode register
#define	SCI_SEMR(x)	(ba[x] + SCIx_SEMR)	// Serial extended mode register
#define	SCI_SPMR(x)	(ba[x] + SCIx_SPMR)	// SPI mode register

/*----------------------------------------------------------------------
 * Interrupt number Table
 */
const LOCAL UINT inotbl[DEV_SER_UNITNM] = {
	INTNO_SCI0_RXI, INTNO_SCI1_RXI, INTNO_SCI2_RXI, INTNO_SCI3_RXI, INTNO_SCI4_RXI,
	INTNO_SCI5_RXI, INTNO_SCI6_RXI, INTNO_SCI7_RXI, INTNO_SCI8_RXI, INTNO_SCI9_RXI,
	INTNO_SCI10_RXI, INTNO_SCI11_RXI, INTNO_SCI12_RXI
};
#define INTNO_RXI(n)	(inotbl[n])
#define INTNO_TXI(n)	(inotbl[n]+1)

const LOCAL UINT eritbl[DEV_SER_UNITNM] = {
	INTNO_SCI0_ERI, INTNO_SCI1_ERI, INTNO_SCI2_ERI, INTNO_SCI3_ERI, INTNO_SCI4_ERI,
	INTNO_SCI5_ERI, INTNO_SCI6_ERI, INTNO_SCI7_ERI, INTNO_SCI8_ERI, INTNO_SCI9_ERI,
	INTNO_SCI10_ERI, INTNO_SCI11_ERI, INTNO_SCI12_ERI
};
#define INTNO_ERI(n)	(eritbl[n])

/*----------------------------------------------------------------------
 * Interrupt priority Table
 */
const LOCAL INT pritbl[DEV_SER_UNITNM] = {
	DEVCNF_SCI0_INTPRI, DEVCNF_SCI1_INTPRI, DEVCNF_SCI2_INTPRI, DEVCNF_SCI3_INTPRI, DEVCNF_SCI4_INTPRI, 
	DEVCNF_SCI5_INTPRI, DEVCNF_SCI6_INTPRI, DEVCNF_SCI7_INTPRI, DEVCNF_SCI8_INTPRI, DEVCNF_SCI9_INTPRI,
	DEVCNF_SCI10_INTPRI, DEVCNF_SCI11_INTPRI, DEVCNF_SCI12_INTPRI
};
#define INTPRI(n)	(pritbl[n])

/*----------------------------------------------------------------------
 * Device control data
*/
typedef struct {
	UW	mode;		// Serial mode
	UW	speed;		// Speed (bit rate)
} T_DEV_SER_LLDEVCB;

LOCAL T_DEV_SER_LLDEVCB		ll_devcb[DEV_SER_UNITNM];

/*----------------------------------------------------------------------
 * Interrupt handler
 */

/*
 * TXI: Transmit data empty interrupt
 */
void sci_txi_inthdr( UINT intno)
{
	UW	unit, data;

	ClearInt(intno);
	for( unit = 0; unit < DEV_SER_UNITNM; unit++) {
		if(INTNO_TXI(unit) == intno) break;
	}
	if(unit >= DEV_SER_UNITNM ) return;

	if( dev_ser_get_snddat(unit, &data)) {
		out_b(SCI_TDR(unit), (UB)data);
	}
}

/*
 * RXI: Receive data full interrupt
 */
void sci_rxi_inthdr( UINT intno)
{
	UW	unit, data;

	for( unit = 0; unit < DEV_SER_UNITNM; unit++) {
		if(INTNO_RXI(unit) == intno) break;
	}
	if(unit >= DEV_SER_UNITNM ) return;

	data = in_b(SCI_RDR(unit));
	dev_ser_notify_rcv(unit, data);	/* Notify the main process of data reception. */
}

/*
 * ERI: Receive error interrupt
 */
void sci_eri_inthdr( UINT intno)
{
	UW	unit, ssr;

	for( unit = 0; unit < DEV_SER_UNITNM; unit++) {
		if(INTNO_ERI(unit) == intno) break;
	}
	if(unit >= DEV_SER_UNITNM ) return;

	ssr = in_b(SCI_SSR(unit));
	out_b(SCI_SSR(unit), 0);		/* Clear error */

	ssr &= (SCI_SSR_ORER | SCI_SSR_PER | SCI_SSR_FER);
	dev_ser_notify_err(unit, ssr);	/* Notify error to main process */
}

/*----------------------------------------------------------------------
 * Set mode & Start communication
 */
LOCAL void start_com(UW unit, UW mode, UW speed)
{
	UB	data;

	/* Set communication mode & Speed */
	out_b( SCI_SMR(unit), (UB)((mode & 0x00000078)|(speed>>8 & 0x00000003)));
	out_b( SCI_SCMR(unit), 0xF2);
	data = in_b( SCI_SEMR(unit));
	out_b( SCI_SEMR(unit), (data & ~SCI_SEMR_ABCS) | (speed & SCI_SEMR_ABCS));
	out_b( SCI_BRR(unit), speed >>16);

	/* SCR.TE,RE,TIE,RIE <- 1 */
	out_b( SCI_SCR(unit), (SCI_SCR_RE | SCI_SCR_TE | SCI_SCR_RIE | SCI_SCR_TIE | SCI_SCR_INI));
}

/*----------------------------------------------------------------------
 * Stop communication
 */
LOCAL void stop_com(UW unit)
{
	if(unit != DEVCNF_SER_DBGUN) {
		out_b(SCI_SCR(unit), SCI_SCR_INI);		/* SCR.TIE,RIE,TE,RE,TEIE <- 0 & Set SCR.CKE */
	} else {	/* Used by T-Monitor */
		out_b(SCI_SCR(unit), SCI_SCR_DEBUG);
	}
}

/*----------------------------------------------------------------------
 * Calculate baud rate
 */
LOCAL UW calc_brr(UW baud)
{
	UW	abcs, brr, cks;
	W	work;

	work = SYSCLK_PCLKB / (baud * 256);
	if(work < 16) {
		abcs	= SCI_SEMR_ABCS;
		cks	= 0;
		brr = SYSCLK_PCLKB / (baud * 16) - 1;
	} else if(work < 32) {
		abcs	= 0;
		cks	= 0;
		brr = SYSCLK_PCLKB / (baud * 32) - 1;
	} else if(work < 64) {
		abcs	= SCI_SEMR_ABCS;
		cks	= 1;
		brr = SYSCLK_PCLKB / (baud * 64) - 1;
	} else if(work < 128) {
		abcs	= 0;
		cks	= 1;
		brr = SYSCLK_PCLKB / (baud * 128) - 1;
	} else if(work < 256) {
		abcs	= SCI_SEMR_ABCS;
		cks	= 2;
		brr = SYSCLK_PCLKB / (baud * 256) - 1;
	} else if(work < 512) {
		abcs	= 0;
		cks	= 2;
		brr = SYSCLK_PCLKB / (baud * 512) - 1;
	} else if(work < 1024) {
		abcs	= SCI_SEMR_ABCS;
		cks	= 3;
		brr = SYSCLK_PCLKB / (baud * 1024) - 1;
	} else if(work < 2048) {
		abcs	= 0;
		cks	= 3;
		brr = SYSCLK_PCLKB / (baud * 2048) - 1;
	} else {
		return 0;
	}

	return (brr<<16 | cks << 8 | abcs);
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT ER dev_ser_llctl( UW unit, INT cmd, UW parm)
{
	UW	data;
	ER	err	= E_OK;

	switch(cmd) {
	case LLD_SER_MODE:	/* Set Communication mode */
		ll_devcb[unit].mode = parm;
		break;
	
	case LLD_SER_SPEED:	/* Set Communication Speed */
		data = calc_brr(parm);
		if(data != 0) {
			ll_devcb[unit].speed = data;
		} else {
			err = E_PAR;
		}
		break;
	
	case LLD_SER_START:	/* Start communication */
		/* Clear Interrupt */
		in_b(SCI_SSR(unit));
		out_b(SCI_SSR(unit), 0xC0);
		/* Enable Interrupt */
		EnableInt( INTNO_RXI(unit), INTPRI(unit));
		EnableInt( INTNO_TXI(unit), INTPRI(unit));
		EnableInt( INTNO_ERI(unit), 0);
		/* Set mode & Start communication */
		start_com( unit, ll_devcb[unit].mode, ll_devcb[unit].speed);
		break;
	
	case LLD_SER_STOP:
		/* Disable Interrupt */
		DisableInt( INTNO_RXI(unit));
		DisableInt( INTNO_TXI(unit));
		DisableInt( INTNO_ERI(unit));
		stop_com(unit);			// Stop communication
		break;

	case LLD_SER_SEND:
		if((in_b( SCI_SSR(unit)) & SCI_SSR_TDRF) != 0) {
			out_b(SCI_TDR(unit), (UB)parm);
			err = E_OK;
		} else {
			err = E_BUSY;
		}
		break;

	case LLD_SER_BREAK:	/* Send Break */
		err = E_NOSPT;
		break;
	}

	return err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_ser_llinit( T_SER_DCB *p_dcb)
{
	T_DINT		dint;
	UW		unit;
	ER		err;

	unit = p_dcb->unit;

#if DEVCONF_SER_INIT_MSTP		// Initialize module stop
	const UINT	mstp_bit[] = { 31, 30, 29, 28, 27, 26, 25, 24, 27, 26, 25, 24, 4};
	UINT		sts;
	UW		*mstp;

	if(unit >= 8 && unit <= 11) {	// SCI8-11
		mstp = (UB*)MSTPCRC;
	} else { 			// SCI0-7, 12
		mstp = (UB*)MSTPCRB;
	}

	if(in_w(mstp) & (1<<mstp_bit[unit])) {
		DI(sts);
		out_h(SYSTEM_PRCR, 0xA502);	/* Disable Register Protect */
		*mstp &= ~(1<<mstp_bit[unit]);	/* Release module stop */
		out_h(SYSTEM_PRCR, 0xA500);	/* Enable Register protect */
		EI(sts);
	}
#endif		/* DEVCONF_SER_INIT_MSTP */

	stop_com(unit);					/* Stop communication */
	out_b(SCI_SPMR(unit), SCI_SPMR_INI);	/* Set Hard flow control */

	/* Device Control block Initizlize */
	p_dcb->intno_rcv = INTNO_SCI0_RXI + (unit<<2);
	p_dcb->intno_snd = INTNO_SCI0_TXI + (unit<<2);

	/* Interrupt handler definition */
	dint.intatr	= TA_HLNG;
	dint.inthdr	= sci_eri_inthdr;
	err = tk_def_int(INTNO_ERI(unit), &dint);
	dint.inthdr	= sci_rxi_inthdr;
	err = tk_def_int(INTNO_RXI(unit), &dint);	
	dint.inthdr	= sci_txi_inthdr;
	err = tk_def_int(INTNO_TXI(unit), &dint);

	return err;
}

#endif		/* DEV_SER_ENABLE */
#endif		/* CPU_RX65N */