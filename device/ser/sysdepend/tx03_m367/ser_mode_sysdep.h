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
 *	ser_mode_sysdep.h
 *	Serial communication device driver
 *	Communication mode definition for TX03_M367
 */

#ifndef	__DEV_SER_MODE_M367_H__
#define	__DEV_SER_MODE_M367_H__

#define	DEV_SER_MODE_7BIT	(0x00000040)	/* Word length 7bit */
#define	DEV_SER_MODE_8BIT	(0x00000060)	/* Word length 8bit */

#define	DEV_SER_MODE_1STOP	(0x00000000)	/* Stop-bit 1bit */
#define	DEV_SER_MODE_2STOP	(0x00000008)	/* Stop-bit 2bit */

#define DEV_SER_MODE_PODD	(0x00000002)	/* parity Odd */
#define DEV_SER_MODE_PEVEN	(0x00000006)	/* parity Even */
#define DEV_SER_MODE_PNON	(0x00000000)	/* parity None */

#define	DEV_SER_MODE_CTSEN	(0x00008000)	/* CTS Hard flow control enable */
#define	DEV_SER_MODE_RTSEN	(0x00004000)	/* RTS Hard flow control enable */

/* Communication Error */
#define	DEV_SER_ERR_OE		(1<<3)	/* Overrun Error */
#define	DEV_SER_ERR_BE		(1<<2)	/* Break Error */
#define	DEV_SER_ERR_PE		(1<<1)	/* Parity Error */
#define	DEV_SER_ERR_FE		(1<<0)	/* Framing Error */


#endif /* __DEV_SER_MODE_M367_H__ */
