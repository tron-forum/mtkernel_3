/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (M367 IoT-Engine)
 */
#include <tk/tkernel.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef IOTE_M367
#if TM_COM_SERIAL_DEV

#define	UART_BASE		(0x40049000UL)

#define	UART_DR 		(*(_UW*)(UART_BASE + 0x0000))	/* Data register */
#define	UART_FR 		(*(_UW*)(UART_BASE + 0x0018))	/* Flag register */
#define	UART_IBDR 		(*(_UW*)(UART_BASE + 0x0024))	/* Integer baud rate register */
#define	UART_FBDR 		(*(_UW*)(UART_BASE + 0x0028))	/* Fractional baud rate register */
#define	UART_LCR_H 		(*(_UW*)(UART_BASE + 0x002C))	/* Line control register */
#define	UART_CR 		(*(_UW*)(UART_BASE + 0x0030))	/* Control register */
#define	UART_IMSC 		(*(_UW*)(UART_BASE + 0x0038))	/* Interrupt mask set/clear register */
#define	UART_RIS 		(*(_UW*)(UART_BASE + 0x003C))	/* Raw interrupt status register */
#define	UART_ICR 		(*(_UW*)(UART_BASE + 0x0044))	/* Interrupt clear register */
#define	UART_DMACR 		(*(_UW*)(UART_BASE + 0x0048))	/* DMA control register */

#define	UART_FR_TXFE		(1 << 7)
#define	UART_FR_RXFE		(1 << 4)

#define	UART_LCR_H_WLEN(n)	(((n)-5) << 5)

#define	UART_CR_RTS		(1 << 11)
#define	UART_CR_DTR		(1 << 10)
#define	UART_CR_RXE		(1 << 9)
#define	UART_CR_TXE		(1 << 8)
#define	UART_CR_UARTEN		(1 << 0)


EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	UB	*b;

	for( b = (UB *)buf; size > 0; size--, b++ ){
		while ( (UART_FR & UART_FR_TXFE) == 0 );
		UART_DR = *b;
		while ( (UART_FR & UART_FR_TXFE) == 0 );
	}
}


EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	for( ; size > 0; size--, buf++ ){
		while ( (UART_FR & UART_FR_RXFE) != 0 );
		*buf = UART_DR & 0xff;
	}
}


EXPORT	void	tm_com_init(void)
{
	UW	n;

	UART_CR = 0;
	UART_DMACR = 0;
	UART_IMSC = 0;
	UART_ICR = UART_RIS;

	n = CLOCK_fsys * (64*2 / 16) / 115200;
	n = (n + 1) >> 1;
	UART_IBDR = n >> 6;
	UART_FBDR = n & 0x3f;

	/* data 8bit, stop 1bit, no parity */
	UART_LCR_H = UART_LCR_H_WLEN(8);

	UART_CR =
		UART_CR_RTS | UART_CR_DTR |
		UART_CR_RXE | UART_CR_TXE | UART_CR_UARTEN;
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* IOTE_M367 */
#endif /* USE_TMONITOR */