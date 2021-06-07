/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (RX231 IoT-Engine)
 */

#include <tk/tkernel.h>
#include <sys/sysdef.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef IOTE_RX231
#if TM_COM_SERIAL_DEV

#define	INTNO_TXI6	226
#define INTNO_RXI6	227

#define	PCLK		(27000000UL)		/* P Clock(24MHz) */
#define	ICLK		(54000000UL)		/* I Clock(48MHz) */
#define RS_BAUD_RATE	(115200)		/* 115.2kbps */

#define	SCI6_SMR	(0x008A0C0UL)		/* Serial mode register */
#define SCI6_BRR	(0x008A0C1UL)		/* Bit rate register */
#define	SCI6_SCR	(0x008A0C2UL)		/* Serial control register */
#define	SCI6_TDR	(0x008A0C3UL)		/* Transmit data register */
#define	SCI6_SSR	(0x008A0C4UL)		/* Serial status register */
#define	SCI6_RDR	(0x008A0C5UL)		/* Receive data register */
#define	SCI6_SCMR	(0x008A0C6UL)		/* Smartcard mode register */

#define	SCR_TIE		(0x80)			/* Transmit interrupt enable */
#define	SCR_RIE		(0x40)			/* Receive interrupt ebable */
#define	SCR_TE		(0x20)			/* Transmit enable */
#define	SCR_RE		(0x10)			/* Receive enable */

#define	SSR_TDRE	(0x80)			/* Transmit data empty */
#define SSR_RDRF	(0x40)			/* Receive data full */
#define	SSR_ERR		(0x38)			/* Error */

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while( (in_b(SCI6_SSR) & SSR_TDRE) == 0);
		out_b( SCI6_TDR, *b );
	}
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	UB	ssr;

	for( ; size > 0; size--, buf++ ){
		while(1) {
			ssr = in_b(SCI6_SSR);
			if( (ssr & SSR_RDRF) != 0) {
				*buf = in_b( SCI6_RDR );
				break;
			} else if( ssr & SSR_ERR ) {	/* check Communication error */
				out_b( SCI6_SSR, ssr & ~SSR_ERR);
			}
		}
	}
}

EXPORT	void	tm_com_init(void)
{
	_UW	dly;

	/* Initialize SCI6 */
	out_b(SCI6_SCR, 0x00);				// TE = 0 RE = 0
	out_b(SCI6_SCMR, 0xF2);				// LSB first
	out_b(SCI6_SMR, 0x00);				// Data8 Stop1
	out_b(SCI6_BRR, (PCLK / (64 / 2 * RS_BAUD_RATE)) - 1);	// 115.2kbps

	dly = ICLK/5/RS_BAUD_RATE;
	while ((dly--) != 0) ;

	out_b(SCI6_SCR, 0x30);				// TE = 1 RE = 1
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* IOTE_RX231 */
#endif /* USE_TMONITOR */