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


/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for RZ/A2M
 */

#ifndef	__DEV_SER_RZA2M_H__
#define	__DEV_SER_RZA2M_H__

#define	DEV_SER_UNITNM		(5)	/* Number of devive channels */

/*
 * UART registers
 */

/* Base address for each channel */
#define	SCI0_BASE		(0xE8007000)
#define	SCI1_BASE		(0xE8007800)
#define	SCI2_BASE		(0xE8008000)
#define	SCI3_BASE		(0xE8008800)
#define	SCI4_BASE		(0xE8009000)

/* Register offset */
#define	SCI_SMR			(0x00000000)	// Serial mode register
#define	SCI_BRR			(0x00000002)	// Bit rate register
#define	SCI_MDDR		(0x00000002)	// Modulation duty register
#define	SCI_SCR			(0x00000004)	// Serial control register
#define	SCI_FTDR		(0x00000006)	// Transmit FIFO data register
#define	SCI_FSR			(0x00000008)	// Serial status register
#define	SCI_FRDR		(0x0000000A)	// Receive FIFO data register
#define	SCI_FCR			(0x0000000C)	// FIFO control register
#define	SCI_FDR			(0x0000000E)	// FIFO data number register
#define	SCI_SPTR		(0x00000010)	// Serial port register
#define	SCI_LSR			(0x00000012)	// Line status register
#define	SCI_SEMR		(0x00000014)	// Serial extended mode register
#define	SCI_FTCR		(0x00000016)	// FIFO trigger control

/*  SMR: Serial mode register */
#define	SCI_SMR_CKS		(0x0003)	// Clock select bit
#define	SCI_SMR_CHR		(0x0040)	// Character length bit

/* SCR: Serial control register */
#define	SCI_SCR_TEIE		(0x0004)	// Transmit end interrupt enable
#define	SCI_SCR_REIE		(0x0008)	// Receive error interrupt enable
#define	SCI_SCR_RE		(0x0010)	// Receive enable
#define	SCI_SCR_TE		(0x0020)	// Transmit enable
#define	SCI_SCR_RIE		(0x0040)	// Receive interrupt enable
#define	SCI_SCR_TIE		(0x0080)	// Transmit interrupt enable

#define	SCI_SCR_INI		(0x0000)	// SCR initial value
#define	SCI_SCR_DEBUG		(0x0030)	// SCR initial value when using debug

/* FSR: Serial status register */
#define SCI_FSR_DR		(0x0001)	// Receive data ready
#define SCI_FSR_RDF		(0x0002)	// Receive FIFO data full
#define	SCI_FSR_PER		(0x0004)	// Parity error
#define	SCI_FSR_FER		(0x0008)	// Framing error
#define	SCI_FSR_BRK		(0x0010)	// Break detection
#define SCI_FSR_TDFE		(0x0020)	// Transmit FIFO data empty
#define	SCI_FSR_TEND		(0x0040)	// Transmit end
#define	SCI_FSR_ER		(0x0080)	// Receive error

/* SPTR: Serial port register */
#define	SCI_SPTR_INI		(0x0003)	// SPTR  initial value

/* LSR: Line status register */
#define	SCI_LSR_ORER		(0x0001)	// Overrun error

/* SEMR: Serial extended mode register */
#define SCI_SEMR_ABCS		(0x01)		// Clock select */

/* FCR: FIFO control register */
#define	SCI_FCR_RFRST		(0x0002)	// Receive FIFO Reset
#define	SCI_FCR_TFRST		(0x0004)	// Transmit FIFO Reset

#define	SCI_FCR_INI		(0x0600)	// FCR initial value

/* FTCR: FIFO trigger control*/
#define	SCI_FTCR_INI		(0x8186)	// SPTR initial value (RTRGS=1, RFTC=0, TTRGS=1, TFTC=6)

/*
 *UART interrupt number
 */
#define	INTNO_SCI0_ERI		297		// Receive error interrupt
#define	INTNO_SCI0_RXI		298		// Receive data full interrupt
#define	INTNO_SCI0_TXI		299		// Transmit data empty interrupt
#define	INTNO_SCI0_TEI		300		// Transmit end interrupt

#define	INTNO_SCI1_ERI		303		// Receive error interrupt
#define	INTNO_SCI1_RXI		304		// Receive data full interrupt
#define	INTNO_SCI1_TXI		305		// Transmit data empty interrupt
#define	INTNO_SCI1_TEI		306		// Transmit end interrupt

#define	INTNO_SCI2_ERI		309		// Receive error interrupt
#define	INTNO_SCI2_RXI		310		// Receive data full interrupt
#define	INTNO_SCI2_TXI		311		// Transmit data empty interrupt
#define	INTNO_SCI2_TEI		312		// Transmit end interrupt

#define	INTNO_SCI3_ERI		315		// Receive error interrupt
#define	INTNO_SCI3_RXI		316		// Receive data full interrupt
#define	INTNO_SCI3_TXI		317		// Transmit data empty interrupt
#define	INTNO_SCI3_TEI		318		// Transmit end interrupt

#define	INTNO_SCI4_ERI		321		// Receive error interrupt
#define	INTNO_SCI4_RXI		322		// Receive data full interrupt
#define	INTNO_SCI4_TXI		323		// Transmit data empty interrupt
#define	INTNO_SCI4_TEI		324		// Transmit end interrupt


#endif		/* __DEV_SER_RZA2M_H__ */
