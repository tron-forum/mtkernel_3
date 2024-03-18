/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for RX65N
 */

#ifndef	__DEV_SER_RX65N_H__
#define	__DEV_SER_RX65N_H__

#define	DEV_SER_UNITNM		(13)	/* Number of device channels */

/*
 * UART registers
 */

/* Base address for each channel */
#define	SCI0_BASE		(0x0008A000)
#define	SCI1_BASE		(0x0008A020)
#define	SCI2_BASE		(0x0008A040)
#define	SCI3_BASE		(0x0008A060)
#define	SCI4_BASE		(0x0008A080)
#define	SCI5_BASE		(0x0008A0A0)
#define	SCI6_BASE		(0x0008A0C0)
#define	SCI7_BASE		(0x0008A0E0)
#define	SCI8_BASE		(0x0008A100)
#define	SCI9_BASE		(0x0008A120)
#define SCI10_BASE		(0x000D0040)
#define SCI11_BASE		(0x000D0060)
#define SCI12_BASE		(0x0008B300)

/* Register offset */
#define	SCIx_SMR		(0x00000000)	// Serial mode register
#define	SCIx_BRR		(0x00000001)	// Bit rate register
#define	SCIx_SCR		(0x00000002)	// Serial control register
#define	SCIx_SSR		(0x00000004)	// Serial status register
#define	SCIx_TDR		(0x00000003)	// Transmit data register
#define	SCIx_RDR		(0x00000005)	// Receive data register
#define	SCIx_SCMR		(0x00000006)	// Smart card mode register
#define	SCIx_SEMR		(0x00000007)	// Serial extended mode register
#define	SCIx_SPMR		(0x0000000D)	// SPI mode register

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
#define	INTNO_SCI0_RXI		58		// Receive data full interrupt
#define	INTNO_SCI0_TXI		59		// Transmit data empty interrupt

#define	INTNO_SCI1_RXI		60		// Receive data full interrupt
#define	INTNO_SCI1_TXI		61		// Transmit data empty interrupt

#define	INTNO_SCI2_RXI		62		// Receive data full interrupt
#define	INTNO_SCI2_TXI		63		// Transmit data empty interrupt

#define	INTNO_SCI3_RXI		58		// Receive data full interrupt
#define	INTNO_SCI3_TXI		59		// Transmit data empty interrupt

#define	INTNO_SCI4_RXI		82		// Receive data full interrupt
#define	INTNO_SCI4_TXI		83		// Transmit data empty interrupt

#define	INTNO_SCI5_RXI		84		// Receive data full interrupt
#define	INTNO_SCI5_TXI		85		// Transmit data empty interrupt

#define	INTNO_SCI6_RXI		86		// Receive data full interrupt
#define	INTNO_SCI6_TXI		87		// Transmit data empty interrupt

#define	INTNO_SCI7_RXI		98		// Receive data full interrupt
#define	INTNO_SCI7_TXI		99		// Transmit data empty interrupt

#define	INTNO_SCI8_RXI		100		// Receive data full interrupt
#define	INTNO_SCI8_TXI		101		// Transmit data empty interrupt

#define	INTNO_SCI9_RXI		102		// Receive data full interrupt
#define	INTNO_SCI9_TXI		103		// Transmit data empty interrupt

#define	INTNO_SCI10_RXI		104		// Receive data full interrupt
#define	INTNO_SCI10_TXI		105		// Transmit data empty interrupt

#define	INTNO_SCI11_RXI		114		// Receive data full interrupt
#define	INTNO_SCI11_TXI		115		// Transmit data empty interrupt

#define	INTNO_SCI12_RXI		116		// Receive data full interrupt
#define	INTNO_SCI12_TXI		117		// Transmit data empty interrupt

/* Receive error interrupt (Group Interrupt) */
#define INTNO_SCI0_ERI		(INTNO_GROUP_BL0+1)
#define INTNO_SCI1_ERI		(INTNO_GROUP_BL0+3)
#define INTNO_SCI2_ERI		(INTNO_GROUP_BL0+5)
#define INTNO_SCI3_ERI		(INTNO_GROUP_BL0+7)
#define INTNO_SCI4_ERI		(INTNO_GROUP_BL0+9)
#define INTNO_SCI5_ERI		(INTNO_GROUP_BL0+11)
#define INTNO_SCI6_ERI		(INTNO_GROUP_BL0+13)
#define INTNO_SCI7_ERI		(INTNO_GROUP_BL0+15)
#define INTNO_SCI8_ERI		(INTNO_GROUP_BL1+25)
#define INTNO_SCI9_ERI		(INTNO_GROUP_BL1+27)
#define INTNO_SCI10_ERI		(INTNO_GROUP_AL0+9)
#define INTNO_SCI11_ERI		(INTNO_GROUP_AL0+13)
#define INTNO_SCI12_ERI		(INTNO_GROUP_BL0+17)

#endif		/* __DEV_SER_RX65N_H__ */
