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
 *	i2c_sysdep.h
 *	I2C device driver
 *	System-dependent definition for TX03_M367
 */

#ifndef	__DEV_I2C_M367_H__
#define	__DEV_I2C_M367_H__

#define	DEV_I2C_UNITNM		(3)	/* Number of device units */

/* I2C device driver operating state */
#define	I2C_STS_START		0x0000
#define	I2C_STS_RESTART		0x0001
#define	I2C_STS_STOP		0x0003
#define	I2C_STS_SEND		0x0004
#define	I2C_STS_RECV		0x0005
#define	I2C_STS_TOP		0x8000

/*----------------------------------------------------------------------
 * I2C registers
 */
#define	I2C0_BASE		(0x400E0000UL)
#define	I2C1_BASE		(0x400E0100UL)
#define	I2C2_BASE		(0x400E0200UL)

#define	I2C_SBIxCR0		0x0000		/* RW Control register 0 */
#define	I2C_SBIxCR1		0x0004		/* RW Control register 1 */
#define	I2C_SBIxDBR		0x0008		/* RW Data buffer register */
#define	I2C_SBIxI2CAR		0x000C		/* RW I2C bus address register */
#define	I2C_SBIxCR2		0x0010		/* W  Control register 2 */
#define	I2C_SBIxSR		0x0010		/* R  Status register */
#define	I2C_SBIxBR0		0x0014		/* RW Baud rate register 0 */

/* Control register 0 - Bit definition */
#define	I2C_SBIxCR0_SBIEN	(1 << 7)	/* RW Serial bus interface enable */

/* Control register 1 - Bit definition */
#define	I2C_SBIxCR1_BC(n)	((n) << 5)	/* RW Selection of transfer bit number */
#define	I2C_SBIxCR1_ACK		(1 << 4)	/* RW ACK */
#define	I2C_SBIxCR1_SCK(n)	((n) << 0)	/* W  Output clock frequency selection */
#define	I2C_SBIxCR1_SWRMON	(1 << 0)	/* R  Software reset status moitor */

/* Control register 2 - Bit definition */
#define	I2C_SBIxCR2_MST		(1 << 7)	/* W Master / Slave mode Selection */
#define	I2C_SBIxCR2_TRX		(1 << 6)	/* W Send / Receive selection */
#define	I2C_SBIxCR2_BB		(1 << 5)	/* W Generation of start / stop condition */
#define	I2C_SBIxCR2_PIN		(1 << 4)	/* W INTSBIx Release interrupt request */
#define	I2C_SBIxCR2_SBIM(n)	((n) << 2)	/* W Select operating mode of serial bus interface */
#define	I2C_SBIxCR2_SBIM_SIO	I2C_SBIxCR2_SBIM(1)
#define	I2C_SBIxCR2_SBIM_I2C	I2C_SBIxCR2_SBIM(2)
#define	I2C_SBIxCR2_SWRST(n)	((n) << 0)	/* W Occurrence of software reset */

/* Status register 0 Bit definition */
#define	I2C_SBIxSR_MST		(1 << 7)	/* R Master / slave selection */
#define	I2C_SBIxSR_TRX		(1 << 6)	/* R Transmitter / receiver selection status */
#define	I2C_SBIxSR_BB		(1 << 5)	/* R I2C Bus state */
#define	I2C_SBIxSR_PIN		(1 << 4)	/* R INTSBIx Interrupt request status */
#define	I2C_SBIxSR_AL		(1 << 3)	/* R Arbitration lost detection */
#define	I2C_SBIxSR_AAS		(1 << 2)	/* R Slave address match detection */
#define	I2C_SBIxSR_ADO		(1 << 1)	/* R General call detection */
#define	I2C_SBIxSR_LRB		(1 << 0)	/* R Last received bit */

/* Baud rate register 0 Bit definition */
#define	I2C_SBIxBR0_I2SBI	(1 << 6)	/* RW IDLE mode */
/* 
 * I2C interrupt number
  */
#define	INTNO_INTSBI(n)		(26 + (n))	/* n=0～2 : I2C/SIO */

/*
 * I2C/SIO setting value
 */
#define	I2C_SBIxCR1_INIT	(	\
		I2C_SBIxCR1_BC(0) |	\
		I2C_SBIxCR1_ACK 	)

#define	I2C_SBIxCR2_START	(	\
		I2C_SBIxCR2_SBIM_I2C |	\
		I2C_SBIxCR2_MST |	\
		I2C_SBIxCR2_TRX |	\
		I2C_SBIxCR2_BB |	\
		I2C_SBIxCR2_PIN		)

#define	I2C_SBIxCR2_RESTART	(	\
		I2C_SBIxCR2_SBIM_I2C |	\
		I2C_SBIxCR2_PIN		)

#define	I2C_SBIxCR2_STOP	(	\
		I2C_SBIxCR2_SBIM_I2C |	\
		I2C_SBIxCR2_MST |	\
		I2C_SBIxCR2_TRX |	\
		I2C_SBIxCR2_PIN		)

#endif		/* __DEV_I2C_M367_H__ */
