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
 *	System-dependent definition for RX231
 */

#ifndef	__DEV_SER_RX231_H__
#define	__DEV_SER_RX231_H__

#define	DEV_SER_UNITNM		(7)	/* Number of devive channels */

/*
 * UART registers
 */

/* Base address for each channel */
#define	SCI0_BASE		(0x8A000)
#define	SCI1_BASE		(0x8A020)
#define	SCI5_BASE		(0x8A0A0)
#define	SCI6_BASE		(0x8A0C0)
#define	SCI8_BASE		(0x8A100)
#define	SCI9_BASE		(0x8A120)
#define SCI12_BASE		(0x8B300)

/* Register offset */
#define	SCI_SMR			(0x00000000)	// Serial mode register
#define	SCI_BRR			(0x00000001)	// Bit rate register
#define	SCI_SCR			(0x00000002)	// Serial control register
#define	SCI_SSR			(0x00000004)	// Serial status register
#define	SCI_TDR			(0x00000003)	// Transmit data register
#define	SCI_RDR			(0x00000005)	// Receive data register
#define	SCI_SCMR		(0x00000006)	// Smart card mode register
#define	SCI_SEMR		(0x00000007)	// Serial extended mode register
#define	SCI_SPMR		(0x0000000D)	// SPI mode register

/*  SMR: Serial mode register */
#define	SCI_SMR_PCLK_01		(0x00)		// Clock select bit
#define	SCI_SMR_PCLK_04		(0x01)
#define	SCI_SMR_PCLK_16		(0x02)
#define	SCI_SMR_PCLK_64		(0x03)
#define	SCI_SMR_CHR		(0x40)		// Character length bit

/* SCR: Serial control register */
#define	SCI_SCR_TEIE		(0x04)		// Transmit end interrupt enable
#define	SCI_SCR_RE		(0x10)		// Receive enable
#define	SCI_SCR_TE		(0x20)		// Transmit enable
#define	SCI_SCR_RIE		(0x40)		// Receive interrupt enable
#define	SCI_SCR_TIE		(0x80)		// Transmit end interrupt enable

#define	SCI_SCR_INI		(0x00)		// SCR initial value
#define	SCI_SCR_DEBUG		(0x32)		// SCR initial value when using debug

/* SSR: Serial status register */
#define	SCI_SSR_TEND		(0x04)		// Transmit end
#define	SCI_SSR_PER		(0x08)		// Parity error
#define	SCI_SSR_FER		(0x10)		// Framing error
#define	SCI_SSR_ORER		(0x20)		// Overrun error
#define SCI_SSR_RDRF		(0x40)		// Receive data full
#define SCI_SSR_TDRF		(0x80)		// Transmit data empty

/* SEMR: Serial extended mode register */
#define SCI_SEMR_ABCS		(0x10)		// Clock select */

/* SPMR: SPI mode register */
#define	SCI_SPMR_INI		(0x00)		// SPMR initial value

/*
 *UART interrupt number
 */
#define	INTNO_SCI0_ERI		214		// Receive error interrupt
#define	INTNO_SCI0_RXI		215		// Receive data full interrupt
#define	INTNO_SCI0_TXI		216		// Transmit data empty interrupt
#define	INTNO_SCI0_TEI		217		// Transmit end interrupt

#define	INTNO_SCI1_ERI		218		// Receive error interrupt
#define	INTNO_SCI1_RXI		219		// Receive data full interrupt
#define	INTNO_SCI1_TXI		220		// Transmit data empty interrupt
#define	INTNO_SCI1_TEI		221		// Transmit end interrupt

#define	INTNO_SCI5_ERI		222		// Receive error interrupt
#define	INTNO_SCI5_RXI		223		// Receive data full interrupt
#define	INTNO_SCI5_TXI		224		// Transmit data empty interrupt
#define	INTNO_SCI5_TEI		225		// Transmit end interrupt

#define	INTNO_SCI6_ERI		226		// Receive error interrupt
#define	INTNO_SCI6_RXI		227		// Receive data full interrupt
#define	INTNO_SCI6_TXI		228		// Transmit data empty interrupt
#define	INTNO_SCI6_TEI		229		// Transmit end interrupt

#define	INTNO_SCI8_ERI		230		// Receive error interrupt
#define	INTNO_SCI8_RXI		231		// Receive data full interrupt
#define	INTNO_SCI8_TXI		232		// Transmit data empty interrupt
#define	INTNO_SCI8_TEI		233		// Transmit end interrupt

#define	INTNO_SCI9_ERI		234		// Receive error interrupt
#define	INTNO_SCI9_RXI		235		// Receive data full interrupt
#define	INTNO_SCI9_TXI		236		// Transmit data empty interrupt
#define	INTNO_SCI9_TEI		237		// Transmit end interrupt

#define	INTNO_SCI12_ERI		238		// Receive error interrupt
#define	INTNO_SCI12_RXI		239		// Receive data full interrupt
#define	INTNO_SCI12_TXI		240		// Transmit data empty interrupt
#define	INTNO_SCI12_TEI		241		// Transmit end interrupt


#endif		/* __DEV_SER_RX231_H__ */
