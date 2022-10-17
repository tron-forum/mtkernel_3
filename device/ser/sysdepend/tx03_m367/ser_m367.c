/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/08.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_TMPM367FDFG

#include <tk/tkernel.h>
#include "../../ser.h"
#include "../../../include/dev_def.h"
#if DEV_SER_ENABLE
/*
 *	ser_m367.c
 *	Serial communication device driver
 *	System dependent processing for TX03_M367
 */

/*----------------------------------------------------------------------
 * Device register base address
 */
const LOCAL UW ba[DEV_SER_UNITNM] = { UART4_BASE, UART5_BASE};

/*----------------------------------------------------------------------
 * Device data
*/
const LOCAL struct {
	UINT	intno;		// Interrupt number
	PRI	intpri;		// Interrupt priority
} ll_devdat[DEV_SER_UNITNM] = {
	{	/* UART4 */
		.intno		= INTNO_UART0,
		.intpri		= DEVCNF_UART4_INTPRI,
	},
	{	/* UART5 */
		.intno		= INTNO_UART1,
		.intpri		= DEVCNF_UART5_INTPRI,
	},
};

/*----------------------------------------------------------------------
 * Device low-level control data
*/
typedef struct {
	UW	mode;		// Serial mode
	UW	speed;		// Spped (bit rate)
} T_DEV_SER_LLDEVCB;

LOCAL T_DEV_SER_LLDEVCB		ll_devcb[DEV_SER_UNITNM];

/*----------------------------------------------------------------------
 * Interrupt handler
 */
void uart_inthdr( UINT intno)
{
	UW	data, err;
	W	unit;

	if(intno>=INTNO_UART0 && intno<=INTNO_UART1) {
		unit = intno - INTNO_UART0;
	} else {
		ClearInt(intno);
		return;
	}
	
	/* Clear Interrupt */
	out_w( ba[unit] + UARTxICR, UARTxINT_ALL);
	ClearInt(intno);

	/* Reception processing */
	while( (in_w( ba[unit] + UARTxFR) & UARTxFR_RXFE) == 0) {
		data	= in_w(ba[unit] + UARTxDR);
		err	= data >> 8;
		
		if (err) {
			dev_ser_notify_err(unit, err);	/* Notify the main process of the communication error. */
			out_w( ba[unit] + UARTxECR, 0);	/* Clear error */
			data &= 0x000000FF;
		}
		dev_ser_notify_rcv(unit, data);	/* Notify the main process of data reception. */
	}

	/* Transmission process */
	while( (in_w( ba[unit] + UARTxFR) & UARTxFR_TXFF) == 0 ) {
		if( !dev_ser_get_snddat(unit, &data))  break;
		out_w( ba[unit] + UARTxDR, data);
	}
}

/*----------------------------------------------------------------------
 * Set mode & Start communication
 */
LOCAL void start_com(UW unit, UW mode, UW speed)
{
	/* Set communication Speed */
	out_w( ba[unit] + UARTxIBDR, speed >> 6);
	out_w( ba[unit] + UARTxFBDR, speed & 0x3f);		

	/* Set communication mode */
	out_w( ba[unit] + UARTxLCR_H, (mode & (UARTxLCR_H_SPS|UARTxLCR_H_WLEN(8)| UARTxLCR_H_STP2| UARTxLCR_H_EPS|UARTxLCR_H_PEN)) |UARTxLCR_H_FEN);
	*(UW*)(ba[unit] + UARTxCR) |= mode & (UARTxCR_CTSEN | UARTxCR_RTSEN);

	/* Start communication */
	*(UW*)(ba[unit] + UARTxCR) |= (UARTxCR_TXE | UARTxCR_RXE | UARTxCR_UARTEN);
}

/*----------------------------------------------------------------------
 * Stop communication
 */
LOCAL void stop_com(UW unit)
{
	if(unit != DEVCNF_SER_DBGUN) {
		out_w( ba[unit] + UARTxCR, 0);
	} else {	/* Used by T-Monitor */
		out_w( ba[unit] + UARTxCR, UARTxCR_UARTEN | UARTxCR_TXE | UARTxCR_RXE);
	}
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT ER dev_ser_llctl( UW unit, INT cmd, UW parm)
{
	ER	err	= E_OK;

	switch(cmd) {
	case LLD_SER_MODE:	/* Set Communication mode */
		ll_devcb[unit].mode = parm;
		break;
	
	case LLD_SER_SPEED:	/* Set Communication Speed */
		ll_devcb[unit].speed = ((CLOCK_fsys * (64*2 / 16) / parm) + 1) >> 1;
		break;
	
	case LLD_SER_START:	/* Start communication */
		out_w(ba[unit] + UARTxICR, UARTxINT_ALL);	// Clear interrupt
		out_w(ba[unit] + UARTxIMSC, UARTxINT_COM);	// Unmask all interrupts
		EnableInt(ll_devdat[unit].intno, ll_devdat[unit].intpri);	// Enable Interrupt
		start_com( unit, ll_devcb[unit].mode, ll_devcb[unit].speed);
		break;
	
	case LLD_SER_STOP:
		DisableInt(ll_devdat[unit].intno);
		stop_com(unit);
		break;

	case LLD_SER_SEND:
		if((in_w( ba[unit] + UARTxFR) & UARTxFR_TXFF) == 0) {
			out_w(ba[unit] + UARTxDR, parm);
			err = E_OK;
		} else {
			err = E_BUSY;
		}
		break;

	case LLD_SER_BREAK:	/* Send Break */
		if(parm) {
			*(UW*)(ba[unit] + UARTxLCR_H) |= UARTxLCR_H_BRK;
		} else {
			*(UW*)(ba[unit] + UARTxLCR_H) &= ~UARTxLCR_H_BRK;
		}
		break;
	}

	return err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_ser_llinit( T_SER_DCB *p_dcb)
{
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= uart_inthdr,
	};
	UW	unit;
	ER	err;

	unit = p_dcb->unit;

	out_w( ba[unit] + UARTxCR, 0);

	/* UART device initialize */
	out_w(ba[unit] + UARTxIMSC, 0);			// Mask all interrupt
	out_w(ba[unit] + UARTxECR, 0);			// Clear error
	out_w(ba[unit] + UARTxIFLS, 			// Set FIFO level
			UARTxIFLS_RXIFLSEL(UARTxIFLS_RXINI) |
			UARTxIFLS_TXIFLSEL(UARTxIFLS_TXINI));
	out_w(ba[unit] + UARTxICR, 0x000007FF);		// Clear interrupt
	out_w(ba[unit] + UARTxDMACR, 0);		// Stop DMA

	/* Device Control block Initizlize */
	p_dcb->intno_rcv = p_dcb->intno_snd = ll_devdat[unit].intno;

	/* Interrupt handler definition */
	err = tk_def_int(ll_devdat[unit].intno, &dint);

	stop_com(unit);

	return err;
}

#endif		/* DEV_SER_ENABLE */
#endif		/* CPU_TMPM367FDFG */