/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel
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
 *	System-dependent definition for RZ/A2M
 */

#ifndef	__DEV_I2C_RZA2M_H__
#define	__DEV_I2C_RZA2M_H__

#define	DEV_I2C_UNITNM		(4)	/* Number of device units */
#define	DEV_I2C_0		(0)	/* RIIC0 */
#define	DEV_I2C_1		(1)	/* RIIC1 */
#define	DEV_I2C_2		(2)	/* RIIC2 */
#define	DEV_I2C_3		(3)	/* RIIC3 */

/* I2C device driver operating state */
#define	I2C_STS_START		0x0000
#define	I2C_STS_RESTART		0x0001
#define	I2C_STS_STOP		0x0003
#define	I2C_STS_SEND		0x0004
#define	I2C_STS_RECV		0x0005
#define	I2C_STS_TOP		0x8000
#define	I2C_STS_LAST		0x0800

/*----------------------------------------------------------------------
 * RIIC registers
 */
#define	RIIC0_BASE		0xE803A000
#define	RIIC1_BASE		0xE803A400
#define	RIIC2_BASE		0xE803A800
#define	RIIC3_BASE		0xE803AC00

#define	RIICx_CR1		(0x00)		/* I2C bus cintrol reg. 1 */
#define	RIICx_CR2		(0x04)		/* I2C bus cintrol reg. 2 */
#define RIICx_MR1		(0x08)		/* I2C bus mode reg. 1 */
#define RIICx_MR2		(0x0C)		/* I2C bus mode reg. 2 */
#define RIICx_MR3		(0x10)		/* I2C bus mode reg. 3 */
#define	RIICx_FER		(0x14)		/* I2C bus function enable reg */
#define	RIICx_SER		(0x18)		/* I2C bus Status enable reg. */
#define	RIICx_IER		(0x1C)		/* I2C bus Interrupt enable reg. */
#define	RIICx_SR1		(0x20)		/* I2C bus Status reg. 1 */
#define	RIICx_SR2		(0x24)		/* I2C bus Status reg. 2 */
#define	RIICx_SAR0		(0x28)		/* Slave address reg. 0 */
#define	RIICx_SAR1		(0x2C)		/* Slave address reg. 1 */
#define	RIICx_SAR2		(0x30)		/* Slave address reg. 2 */
#define	RIICx_BRL		(0x34)		/* I2C bus bit rate Low reg. */
#define	RIICx_BRH		(0x38)		/* I2C bus bit rate High reg. */
#define	RIICx_DRT		(0x3C)		/* I2C bus transmit data reg. */
#define	RIICx_DRR		(0x40)		/* I2C bus receive data reg. */

#define	RIIC_CR1_ICE		(1<<7)	
#define	RIIC_CR1_RST		(1<<6)
#define	RIIC_CR1_SOWP		(1<<4)

#define	RIIC_CR2_BBSY		(1<<7)
#define	RIIC_CR2_ST		(1<<1)
#define	RIIC_CR2_RS		(1<<2)
#define	RIIC_CR2_SP		(1<<3)

#define RIIC_MR3_WAIT		(1<<6)
#define RIIC_MR3_ACKWP		(1<<4)
#define RIIC_MR3_ACKBT		(1<<3)

#define	RIIC_IER_TIE		(1<<7)
#define	RIIC_IER_TEIE		(1<<6)
#define	RIIC_IER_RIE		(1<<5)
#define	RIIC_IER_NAKIE		(1<<4)
#define	RIIC_IER_SPIE		(1<<3)
#define	RIIC_IER_STIE		(1<<2)
#define	RIIC_IER_ALIE		(1<<1)
#define	RIIC_IER_TMOIE		(1<<0)

#define RIIC_SR2_TDRE		(1<<7)
#define RIIC_SR2_TEND		(1<<6)
#define RIIC_SR2_RDRF		(1<<5)
#define RIIC_SR2_NACKF		(1<<4)
#define RIIC_SR2_STOP		(1<<3)
#define RIIC_SR2_START		(1<<2)
#define RIIC_SR2_AL		(1<<1)
#define RIIC_SR2_TMOF		(1<<0)

#define RIIC_FER_TMOE		(1<<0)

/* Register initial value */
#define	RIIC_MR1_INI		(1<<4)

/* 
 * I2C interrupt number
	*/
#define	INTNO_RIIC_TEI0		264		// Transmit end interrupt
#define	INTNO_RIIC_RI0		265		// Receive data full interrupt
#define	INTNO_RIIC_TI0		266		// Transmit data empty interrupt
#define	INTNO_RIIC_SPI0		267		// STOP condition detection interrupt
#define	INTNO_RIIC_STI0		268		// START condition detection interrupt
#define	INTNO_RIIC_NAKI0	269		// NACK reception interrupt
#define	INTNO_RIIC_ALI0		270		// Arbitration lost interrupt
#define	INTNO_RIIC_TMOI0	271		// Time out interrupt

#define	INTNO_RIIC_TEI1		272		// Transmit end interrupt
#define	INTNO_RIIC_RI1		273		// Receive data full interrupt
#define	INTNO_RIIC_TI1		274		// Transmit data empty interrupt
#define	INTNO_RIIC_SPI1		275		// STOP condition detection interrupt
#define	INTNO_RIIC_STI1		276		// START condition detection interrupt
#define	INTNO_RIIC_NAKI1	277		// NACK reception interrupt
#define	INTNO_RIIC_ALI1		278		// Arbitration lost interrupt
#define	INTNO_RIIC_TMOI1	279		// Time out interrupt

#define	INTNO_RIIC_TEI2		280		// Transmit end interrupt
#define	INTNO_RIIC_RI2		281		// Receive data full interrupt
#define	INTNO_RIIC_TI2		282		// Transmit data empty interrupt
#define	INTNO_RIIC_SPI2		283		// STOP condition detection interrupt
#define	INTNO_RIIC_STI2		284		// START condition detection interrupt
#define	INTNO_RIIC_NAKI2	285		// NACK reception interrupt
#define	INTNO_RIIC_ALI2		286		// Arbitration lost interrupt
#define	INTNO_RIIC_TMOI2	287		// Time out interrupt

#define	INTNO_RIIC_TEI3		288		// Transmit end interrupt
#define	INTNO_RIIC_RI3		289		// Receive data full interrupt
#define	INTNO_RIIC_TI3		290		// Transmit data empty interrupt
#define	INTNO_RIIC_SPI3		291		// STOP condition detection interrupt
#define	INTNO_RIIC_STI3		292		// START condition detection interrupt
#define	INTNO_RIIC_NAKI3	293		// NACK reception interrupt
#define	INTNO_RIIC_ALI3		294		// Arbitration lost interrupt
#define	INTNO_RIIC_TMOI3	295		// Time out interrupt

#endif	/* __DEV_I2C_RZA2M_H__ */
