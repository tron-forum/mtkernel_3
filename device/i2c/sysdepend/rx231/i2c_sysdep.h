/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02
 *
 *----------------------------------------------------------------------
 */

/*
 *	i2c_sysdep.h
 *	I2C device driver
 *	System-dependent definition for RX231
 */

#ifndef	__DEV_I2C_RX231_H__
#define	__DEV_I2C_RX231_H__

#define	DEV_I2C_UNITNM		(1)	/* Number of device units */

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
#define	RIIC_BASE		0x00088300

#define	RIIC_ICCR1		(RIIC_BASE+0x00)	/* I2C bus cintrol reg. 1 */
#define	RIIC_ICCR2		(RIIC_BASE+0x01)	/* I2C bus cintrol reg. 2 */
#define RIIC_ICMR1		(RIIC_BASE+0x02)	/* I2C bus mode reg. 1 */
#define RIIC_ICMR2		(RIIC_BASE+0x03)	/* I2C bus mode reg. 2 */
#define RIIC_ICMR3		(RIIC_BASE+0x04)	/* I2C bus mode reg. 3 */
#define	RIIC_ICFER		(RIIC_BASE+0x05)	/* I2C bus function enable reg */
#define	RIIC_ICSER		(RIIC_BASE+0x06)	/* I2C bus Status enable reg. */
#define	RIIC_ICIER		(RIIC_BASE+0x07)	/* I2C bus Interrupt enable reg. */
#define	RIIC_ICSR1		(RIIC_BASE+0x08)	/* I2C bus Status reg. 1 */
#define	RIIC_ICSR2		(RIIC_BASE+0x09)	/* I2C bus Status reg. 2 */

#define	RIIC_SARL(y)		(RIIC_BASE+0x0A+(y<<1))	/* Slave address reg. L0~2 */
#define	RIIC_SARU(y)		(RIIC_BASE+0x0B+(y<<1))	/* Slave address reg. U0~2 */

#define	RIIC_ICBRL		(RIIC_BASE+ 0x10)	/* I2C bus bit rate Low reg. */
#define	RIIC_ICBRH		(RIIC_BASE+ 0x11)	/* I2C bus bit rate High reg. */
#define	RIIC_ICDRT		(RIIC_BASE+ 0x12)	/* I2C bus transmit data reg. */
#define	RIIC_ICDRR		(RIIC_BASE+ 0x13)	/* I2C bus receive data reg. */

#define	RIIC_ICCR1_ICE		(1<<7)	
#define	RIIC_ICCR1_IICRST	(1<<6)
#define	RIIC_ICCR1_SOWP		(1<<4)

#define	RIIC_IICR2_BBSY		(1<<7)
#define	RIIC_IICR2_ST		(1<<1)
#define	RIIC_IICR2_RS		(1<<2)
#define	RIIC_IICR2_SP		(1<<3)

#define RIIC_ICMR3_WAIT		(1<<6)
#define RIIC_ICMR3_ACKWP	(1<<4)
#define RIIC_ICMR3_ACKBT	(1<<3)

#define	RIIC_ICIER_TIE		(1<<7)
#define	RIIC_ICIER_TEIE		(1<<6)
#define	RIIC_ICIER_RIE		(1<<5)
#define	RIIC_ICIER_NAKIE	(1<<4)
#define	RIIC_ICIER_SPIE		(1<<3)
#define	RIIC_ICIER_STIE		(1<<2)
#define	RIIC_ICIER_ALIE		(1<<1)
#define	RIIC_ICIER_TMOIE	(1<<0)

#define RIIC_ICSR2_TDRE		(1<<7)
#define RIIC_ICSR2_TEND		(1<<6)
#define RIIC_ICSR2_RDRF		(1<<5)
#define RIIC_ICSR2_NACKF	(1<<4)
#define RIIC_ICSR2_STOP		(1<<3)
#define RIIC_ICSR2_START	(1<<2)
#define RIIC_ICSR2_AL		(1<<1)
#define RIIC_ICSR2_TMOF		(1<<0)


/* Register initial value */
#define	RIIC_ICMR1_INI		(1<<4)
#define	RIIC_ICBRH_INI		(14-1)
#define	RIIC_ICBRL_INI		(21-1)

/* 
 * I2C interrupt number
  */
#define	INTNO_RIIC_EEI		246
#define	INTNO_RIIC_RXI		247
#define	INTNO_RIIC_TXI		248
#define	INTNO_RIIC_TEI		249

#endif		/* __DEV_I2C_RX231_H__ */
