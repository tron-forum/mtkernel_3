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


/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for TX03_M367
 */

#ifndef	__DEV_SER_M367_H__
#define	__DEV_SER_M367_H__

#define	DEV_SER_UNITNM		(2)	/* Number of devive channels */
#define DEV_SER_UNIT0		(0)	/* UART Ch.4 */
#define DEV_SER_UNIT1		(1)	/* UART Ch.5 */

/*
 * UART registers
 */

/* Base address for each channel */
#define	UART4_BASE		(0x40048000UL)	/* UART Ch.4 */
#define	UART5_BASE		(0x40049000UL)	/* UART Ch.5 */

/* UART modem */
#define	UARTxDR 		(0x0000)	/* Data register */
#define	UARTxRSR 		(0x0004)	/* Receive status register */
#define	UARTxECR 		(0x0004)	/* Error clear register */
#define	UARTxFR 		(0x0018)	/* Flag register */
#define	UARTxILPR 		(0x0020)	/* IrDA low-power counter */
#define	UARTxIBDR 		(0x0024)	/* Integer baud rate register */
#define	UARTxFBDR 		(0x0028)	/* Fractional baud rate register */
#define	UARTxLCR_H 		(0x002C)	/* Line control register */
#define	UARTxCR 		(0x0030)	/* Control register */
#define	UARTxIFLS 		(0x0034)	/* Interrupt FIFO level select register */
#define	UARTxIMSC 		(0x0038)	/* Interrupt mask set/clear register */
#define	UARTxRIS 		(0x003C)	/* Raw interrupt status register */
#define	UARTxMIS 		(0x0040)	/* Masked interrupt status register */
#define	UARTxICR 		(0x0044)	/* Interrupt clear register */
#define	UARTxDMACR 		(0x0048)	/* DMA control register */

#define	UARTxDR_OE		(1 << 11)	/* R Overrun error */
#define	UARTxDR_BE		(1 << 10)	/* R Break error */
#define	UARTxDR_PE		(1 << 9)	/* R Parity error */
#define	UARTxDR_FE		(1 << 8)	/* R Framing error */
#define	UARTxDR_DATA		(0xff)		/* RW Send / receive data */

#define	UARTxRSR_OE		(1 << 3)	/* R Overrun error */
#define	UARTxRSR_BE		(1 << 2)	/* R Break error */
#define	UARTxRSR_PE		(1 << 1)	/* R Parity error */
#define	UARTxRSR_FE		(1 << 0)	/* R Framing error */

#define	UARTxFR_RI		(1 << 8)	/* R Link indicator */
#define	UARTxFR_TXFE		(1 << 7)	/* R Send empty */
#define	UARTxFR_RXFF		(1 << 6)	/* R Receive full */
#define	UARTxFR_TXFF		(1 << 5)	/* R Send full */
#define	UARTxFR_RXFE		(1 << 4)	/* R Receive empty */
#define	UARTxFR_BUSY		(1 << 3)	/* R UART busy */
#define	UARTxFR_DCD		(1 << 2)	/* R Data carrier detect */
#define	UARTxFR_DSR		(1 << 1)	/* R Data set ready */
#define	UARTxFR_CTS		(1 << 0)	/* R Clear to send */

#define	UARTxLCR_H_SPS		(1 << 7)	/* RW Stick parity selection */
#define	UARTxLCR_H_WLEN(n)	(((n)-5) << 5)	/* RW Word length (5～8bit) */
#define	UARTxLCR_H_FEN		(1 << 4)	/* RW Enable FIFO */
#define	UARTxLCR_H_STP2		(1 << 3)	/* RW Stop bit selection */
#define	UARTxLCR_H_EPS		(1 << 2)	/* RW Even parity selection */
#define	UARTxLCR_H_PEN		(1 << 1)	/* RW Parity enable */
#define	UARTxLCR_H_BRK		(1 << 0)	/* RW Send break */

#define	UARTxCR_CTSEN		(1 << 15)	/* RW CTS Hardware flow control enable */
#define	UARTxCR_RTSEN		(1 << 14)	/* RW RTS Hardware flow control enable */
#define	UARTxCR_RTS		(1 << 11)	/* RW Request to send(RTS) */
#define	UARTxCR_DTR		(1 << 10)	/* RW Ready to send data (DTS) */
#define	UARTxCR_RXE		(1 << 9)	/* RW Receive enable */
#define	UARTxCR_TXE		(1 << 8)	/* RW Send enable */
#define	UARTxCR_SIRLP		(1 << 2)	/* RW IrDA  SIR Low power mode */
#define	UARTxCR_SIREN		(1 << 1)	/* RW SIR Enable */
#define	UARTxCR_UARTEN		(1 << 0)	/* RW UART Enable */

#define	UARTxIFLS_RXIFLSEL(n)	((n) << 3)	/* RW Receive interrupt FIFO Level */
#define	UARTxIFLS_TXIFLSEL(n)	((n) << 0)	/* RW Transmission interrupt FIFO Level */
#define	UARTxIFLS_1_8		0		/* FIFO 1/8 */
#define	UARTxIFLS_1_4		1		/* FIFO 1/4 */
#define	UARTxIFLS_1_2		2		/* FIFO 1/2 */
#define	UARTxIFLS_3_4		3		/* FIFO 3/4 */
#define	UARTxIFLS_7_8		4		/* FIFO 7/8 */

#define UARTxIFLS_RXINI		UARTxIFLS_1_2	// Receive FIFO 1/2
#define UARTxIFLS_TXINI		UARTxIFLS_1_4	// Send FIFO 1/4

#define	UARTxINT_OE		(1 << 10)	/* RW Overrun error interrupt */
#define	UARTxINT_BE		(1 << 9)	/* RW Break error interrupt */
#define	UARTxINT_PE		(1 << 8)	/* RW Parity error interrupt */
#define	UARTxINT_FE		(1 << 7)	/* RW Framing error interrupt */
#define	UARTxINT_RT		(1 << 6)	/* RW Receive timeout interrupt */
#define	UARTxINT_TX		(1 << 5)	/* RW Transmission interrupt */
#define	UARTxINT_RX		(1 << 4)	/* RW Receive interrupt */
#define	UARTxINT_DSR		(1 << 3)	/* RW DSR Modem interrupt */
#define	UARTxINT_DCD		(1 << 2)	/* RW DCD Modem interrupt */
#define	UARTxINT_CTS		(1 << 1)	/* RW CTS Modem interrupt */
#define	UARTxINT_RI		(1 << 0)	/* RW RIN Modem interrupt */

#define	UARTxINT_ALL		(0x000007FF)
#define	UARTxINT_COM		(UARTxINT_OE|UARTxINT_BE|UARTxINT_PE|UARTxINT_FE| \
					UARTxINT_RT|UARTxINT_TX|UARTxINT_RX)

#define	UARTxDMACR_DMAONERR	(1 << 2)	/* RW DMA On error */
#define	UARTxDMACR_TXDMAE	(1 << 1)	/* RW Transmission DMA Enable */
#define	UARTxDMACR_RXDMAE	(1 << 0)	/* RW Receive DMA Enable */

/* UART interrupt number */
#define	INTNO_UART0		24		/* UART Ch.4 */
#define	INTNO_UART1		25		/* UART Ch.5 */

#endif		/* __DEV_SER_M367_H__ */
