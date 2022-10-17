/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RZA2M

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"
#if DEV_SER_ENABLE

/*
 *	ser_rza2m.c
 *	Serial communication device driver
 *	System dependent processing for RZ/A2M
 */

/*----------------------------------------------------------------------
 * Device register base address
 */
const LOCAL UW ba[DEV_SER_UNITNM] = 
	{ SCI0_BASE, SCI1_BASE, SCI2_BASE, SCI3_BASE, SCI4_BASE};

/*----------------------------------------------------------------------
 * Device data
*/
const LOCAL struct {
	UINT	intno;		// Interrupt number
	PRI	intpri;		// Interrupt priority
} ll_devdat[DEV_SER_UNITNM] = {
	{.intno = INTNO_SCI0_ERI, DEVCNF_SER0_INTPRI},
	{.intno = INTNO_SCI1_ERI, DEVCNF_SER1_INTPRI},
	{.intno = INTNO_SCI2_ERI, DEVCNF_SER2_INTPRI},
	{.intno = INTNO_SCI3_ERI, DEVCNF_SER3_INTPRI},
	{.intno = INTNO_SCI4_ERI, DEVCNF_SER4_INTPRI},
};

#define	INTNO_BRI(n)	(ll_devdat[n].intno)
#define	INTNO_ERI(n)	(ll_devdat[n].intno)
#define INTNO_RXI(n)	(ll_devdat[n].intno+1)
#define INTNO_TXI(n)	(ll_devdat[n].intno+2)
#define INTNO_TEI(n)	(ll_devdat[n].intno+3)
#define INTNO_DRI(n)	(ll_devdat[n].intno+3)

#define	INT_PRI(n)	(ll_devdat[n].intpri)

/*----------------------------------------------------------------------
 * Device control data
*/
typedef struct {
	UW	mode;		// Serial mode
	UW	speed;		// Spped (bit rate)
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
	UH	reg;
	UINT	inttxi = INTNO_SCI0_TXI;

	ClearInt(intno);

	for(unit = 0; unit < DEV_SER_UNITNM; unit++, inttxi+=6) {
		if(intno == inttxi) break;
	}

	if(unit < DEV_SER_UNITNM ) {
		if( dev_ser_get_snddat(unit, &data)) {
			out_b(ba[unit] + SCI_FTDR, (UB)data);
		} else {
			reg = in_h(ba[unit] + SCI_SCR);
			out_h(ba[unit] + SCI_SCR, reg & ~SCI_SCR_TIE);
		}
	}

	EndOfInt(intno);
}

/*
 * RXI: Receive data full interrupt
 */
void sci_rxi_inthdr( UINT intno)
{
	UW	unit, data;
	UH	reg;
	UINT	intrxi = INTNO_SCI0_RXI;

	ClearInt(intno);

	for(unit = 0; unit < DEV_SER_UNITNM; unit++, intrxi+=6) {
		if(intno == intrxi) break;
	}

	if(unit < DEV_SER_UNITNM ) {
		data = in_b(ba[unit] + SCI_FRDR);

		/* Clear DR & RDF bit */
		reg = in_h(ba[unit] + SCI_FSR);
		out_h(ba[unit] + SCI_FSR, reg & ~(SCI_FSR_DR|SCI_FSR_RDF));

		dev_ser_notify_rcv(unit, data);	/* Notify the main process of data reception. */	
	}

	EndOfInt(intno);
}

/*
 * ERI: Receive error interrupt
 */
void sci_eri_inthdr( UINT intno)
{
	UW	unit, err;
	UH	reg;
	UINT	interi = INTNO_SCI0_ERI;

	ClearInt(intno);

	for(unit = 0; unit < DEV_SER_UNITNM; unit++, interi+=6) {
		if(intno == interi) break;
	}

	if(unit < DEV_SER_UNITNM ) {
		reg = in_h(ba[unit] + SCI_FSR);
		out_h(ba[unit] + SCI_FSR, reg & ~SCI_FSR_ER);	/* Clear receive error */
		err = (UW)(reg & (DEV_SER_ERR_PE | DEV_SER_ERR_FE));

		reg = in_h(ba[unit] + SCI_LSR);
		out_h(ba[unit] + SCI_FSR, reg & ~SCI_LSR_ORER);	/* Clear receive error */
		err |= (UW)(reg & DEV_SER_ERR_OE);

		dev_ser_notify_err(unit, err);	/* Notify error to main process */
	}

	EndOfInt(intno);
}


/*----------------------------------------------------------------------
 * Set mode & Start communication
 */
LOCAL void start_com(UW unit, UW mode, UW speed)
{
	UH	reg;

	/* Set communication mode & Speed */
	out_h( ba[unit] + SCI_SMR, (UH)((mode & 0x00000078)|(speed>>8 & 0x00000003)));

	reg = in_h( ba[unit] + SCI_SEMR);
	out_h( ba[unit] + SCI_SEMR, (UH)((reg & ~SCI_SEMR_ABCS)|(speed & SCI_SEMR_ABCS)));

	out_b( ba[unit] + SCI_BRR, (UB)(speed >>16));

	/* Clear DR & RDF bit */
	reg = in_h(ba[unit] + SCI_FSR);
	out_h(ba[unit] + SCI_FSR, reg & ~(SCI_FSR_DR|SCI_FSR_RDF));

	/* SCR.TE,RE,RIE,REIE<- 1 */
	out_h( ba[unit] + SCI_SCR, 
		(SCI_SCR_RE | SCI_SCR_TE | SCI_SCR_RIE | SCI_SCR_REIE));
}

/*----------------------------------------------------------------------
 * Stop communication
 */
LOCAL void stop_com(UW unit)
{
	if(unit != DEVCNF_SER_DBGUN) {
		out_h(ba[unit] + SCI_SCR, SCI_SCR_INI);
	} else {	/* Used by T-Monitor */
		out_h(ba[unit] + SCI_SCR, SCI_SCR_DEBUG);
	}
}

/*----------------------------------------------------------------------
 * Calculate baud rate
 */
typedef	struct {
	UW	baud;	// baud tate
	UW	val;	// set value
} T_BPS_VAL;

LOCAL const T_BPS_VAL	bps_tbl[] = {
	/* Val : brr<<16 | cks << 8 | abcs */
	/* P1 clock=66.67MHz */
	{ 500000,	(  3<<16) + (0<<8) + 0 },
	{ 230400,	(  8<<16) + (0<<8) + 0 },
	{ 115200,	( 17<<16) + (0<<8) + 0 },
	{  38400,	( 53<<16) + (0<<8) + 0 },
	{  31250,	( 65<<16) + (0<<8) + 0 },
	{  28800,	( 71<<16) + (0<<8) + 0 },
	{  19200,	(106<<16) + (0<<8) + 0 },
	{  14400,	(142<<16) + (0<<8) + 0 },
	{   9600,	(214<<16) + (0<<8) + 0 },
	{   4800,	(106<<16) + (1<<8) + 0 },
	{   2400,	(214<<16) + (1<<8) + 0 },
	{   1200,	(106<<16) + (2<<8) + 0 },
	{    600,	(214<<16) + (2<<8) + 0 },
	{    300,	(106<<16) + (3<<8) + 0 },
	{    150,	(214<<16) + (3<<8) + 0 },
	{      0,	0 }
};

LOCAL UW calc_brr(UW baud)
{
	const T_BPS_VAL		*p_tbl;

	for(p_tbl = bps_tbl; p_tbl->baud != 0; p_tbl++) {
		if(p_tbl->baud == baud) break;
	}

	return (p_tbl->baud != 0)? p_tbl->val: 0;
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT ER dev_ser_llctl( UW unit, INT cmd, UW parm)
{
	UW	data;
	UH	reg;
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
		/* FIFO initialize */
		out_h(ba[unit] + SCI_FCR, SCI_FCR_RFRST | SCI_FCR_TFRST);	/* FIFO Reset */
		out_h(ba[unit] + SCI_FCR, SCI_FCR_INI);				/* Set RSTRG */
		out_h(ba[unit] + SCI_FTCR, SCI_FTCR_INI);			/* Set FIFO trigger */

		/* Initialize status & Clear Error bits */
		reg = in_h(ba[unit] + SCI_FSR);
		out_h(ba[unit] + SCI_FSR, 0);			/* Initialize FSR */
		reg = in_h(ba[unit] + SCI_LSR);
		out_h(ba[unit] + SCI_LSR, 0);			/* Initialize LSR */

		/* Clear Interrupt */
		ClearInt( INTNO_ERI(unit));
		ClearInt( INTNO_RXI(unit));
		ClearInt( INTNO_TXI(unit));

		/* Enable Interrupt */
		EnableInt( INTNO_ERI(unit), INT_PRI(unit));
		EnableInt( INTNO_RXI(unit), INT_PRI(unit));
		EnableInt( INTNO_TXI(unit), INT_PRI(unit));
	
		/* Set mode & Start communication */
		start_com( unit, ll_devcb[unit].mode, ll_devcb[unit].speed);
		break;
	
	case LLD_SER_STOP:
		/* Disable Interrupt */
		DisableInt( INTNO_ERI(unit));
		DisableInt( INTNO_RXI(unit));
		DisableInt( INTNO_TXI(unit));
		stop_com(unit);			// Stop communication
		break;

	case LLD_SER_SEND:
		reg = in_h( ba[unit] + SCI_FDR);
		if((reg>>8) < 0x0010) {
			out_b(ba[unit] + SCI_FTDR, (UB)parm);
			reg = in_h(ba[unit] + SCI_SCR);
			out_h(ba[unit] + SCI_SCR, reg | SCI_SCR_TIE);
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
	UB	stbc;

	stbc = in_b(CPG_STBCR1);
	out_b(CPG_STBCR1, stbc & ~(1<<(7-unit)));
	stbc = in_b(CPG_STBCR1);	// dummy read
	
#endif		/* DEVCONF_SER_INIT_MSTP */

	stop_com(unit);					/* Stop communication */
	out_h(ba[unit] + SCI_SPTR, SCI_SPTR_INI);	/* Serial port setting */
	out_h( ba[unit] + SCI_SEMR, 0);


	/* Device Control block Initizlize */
	p_dcb->intno_rcv = INTNO_RXI(unit);
	p_dcb->intno_snd = INTNO_TXI(unit);
	p_dcb->int_pri = INT_PRI(unit);

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
#endif		/* CPU_RZA2M */