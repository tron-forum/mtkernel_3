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
 *	ser_mode_stm32l4.h
 *	Serial communication device driver
 *	Communication mode definition for STM32L4
 */

#ifndef	__DEV_SER_MODE_STM32L4_H__
#define	__DEV_SER_MODE_STM32L4_H__

#define	DEV_SER_MODE_7BIT	(0x10000000)	/* Word length 7bit */
#define	DEV_SER_MODE_8BIT	(0x00000000)	/* Word length 8bit */

#define	DEV_SER_MODE_1STOP	(0x00000000)	/* Stop-bit 1bit */
#define	DEV_SER_MODE_2STOP	(0x00002000)	/* Stop-bit 2bit */

#define DEV_SER_MODE_PODD	(0x00000400)	/* parity Odd */
#define DEV_SER_MODE_PEVEN	(0x00000600)	/* parity Even */
#define DEV_SER_MODE_PNON	(0x00000000)	/* parity None */

#define	DEV_SER_MODE_CTSEN	(0x00008000)	/* CTS Hard flow control enable */
#define	DEV_SER_MODE_RTSEN	(0x00004000)	/* RTS Hard flow control enable */

/* Communication Error */
#define	DEV_SER_ERR_PE		(1<<0)	/* Parity Error */
#define	DEV_SER_ERR_FE		(1<<1)	/* Framing Error */
#define	DEV_SER_ERR_NF		(1<<2)	/* Start bit noise detection flag */
#define	DEV_SER_ERR_OE		(1<<3)	/* Overrun Error */

#endif /* __DEV_SER_MODE_STM32L4_H__ */
