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
 *	i2c_sysdep.h
 *	I2C device driver
 *	System-dependent definition for RX65N
 */

#ifndef	__DEV_I2C_RX65N_H__
#define	__DEV_I2C_RX65N_H__

#define	DEV_I2C_UNITNM	(3)	// Number of devive units
#define	DEV_I2C_UNIT0	(0)
#define	DEV_I2C_UNIT1	(1)
#define	DEV_I2C_UNIT2	(2)

/* I2C device driver operating state */
#define	I2C_STS_START	0x0000
#define	I2C_STS_RESTART	0x0001
#define	I2C_STS_STOP	0x0003
#define	I2C_STS_SEND	0x0004
#define	I2C_STS_RECV	0x0005
#define	I2C_STS_TOP	0x8000
#define	I2C_STS_LAST	0x0800

/*----------------------------------------------------------------------
 * RIIC registers
 */
#define	RIIC0_BASE		(0x00088300UL)
#define	RIIC1_BASE		(0x00088320UL)
#define	RIIC2_BASE		(0x00088340UL)

#define	RIICx_ICCR1		(0x00)	/* I2C bus cintrol reg. 1 */
#define	RIICx_ICCR2		(0x01)	/* I2C bus cintrol reg. 2 */
#define RIICx_ICMR1		(0x02)	/* I2C bus mode reg. 1 */
#define RIICx_ICMR2		(0x03)	/* I2C bus mode reg. 2 */
#define RIICx_ICMR3		(0x04)	/* I2C bus mode reg. 3 */
#define	RIICx_ICFER		(0x05)	/* I2C bus function enable reg */
#define	RIICx_ICSER		(0x06)	/* I2C bus Status enable reg. */
#define	RIICx_ICIER		(0x07)	/* I2C bus Interrupt enable reg. */
#define	RIICx_ICSR1		(0x08)	/* I2C bus Status reg. 1 */
#define	RIICx_ICSR2		(0x09)	/* I2C bus Status reg. 2 */

#define	RIICx_SARL(y)		(0x0A+(y<<1))	/* Slave address reg. L0~2 */
#define	RIICx_SARU(y)		(0x0B+(y<<1))	/* Slave address reg. U0~2 */

#define	RIICx_ICBRL		(0x10)	/* I2C bus bit rate Low reg. */
#define	RIICx_ICBRH		(0x11)	/* I2C bus bit rate High reg. */
#define	RIICx_ICDRT		(0x12)	/* I2C bus transmit data reg. */
#define	RIICx_ICDRR		(0x13)	/* I2C bus receive data reg. */

#define	RIIC_ICCR1_ICE		(1<<7)	
#define	RIIC_ICCR1_IICRST	(1<<6)
#define	RIIC_ICCR1_SOWP		(1<<4)

#define	RIIC_IICR2_BBSY		(1<<7)
#define	RIIC_IICR2_ST		(1<<1)
#define	RIIC_IICR2_RS		(1<<2)
#define	RIIC_IICR2_SP		(1<<3)

#define	RIIC_ICMR1_MTWP		(1<<7)

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

/* 
 * I2C interrupt number
*/
#define	INTNO_RIIC0_RXI		52
#define	INTNO_RIIC0_TXI		53
#define	INTNO_RIIC0_TEI		(INTNO_GROUP_BL1+13)	//  Group interrupts (GROUPBL1 - 13)
#define	INTNO_RIIC0_EEI		(INTNO_GROUP_BL1+14)	//  Group interrupts (GROUPBL1 - 14)

#define	INTNO_RIIC1_RXI		50
#define	INTNO_RIIC1_TXI		51
#define	INTNO_RIIC1_TEI		(INTNO_GROUP_BL1+28)	//  Group interrupts (GROUPBL1 - 28)
#define	INTNO_RIIC1_EEI		(INTNO_GROUP_BL1+29)	//  Group interrupts (GROUPBL1 - 29)

#define	INTNO_RIIC2_RXI		54
#define	INTNO_RIIC2_TXI		55
#define	INTNO_RIIC2_TEI		(INTNO_GROUP_BL1+15)	//  Group interrupts (GROUPBL1 - 15)
#define	INTNO_RIIC2_EEI		(INTNO_GROUP_BL1+16)	//  Group interrupts (GROUPBL1 - 16)

#endif		/* __DEV_I2C_RX65N_H__ */
