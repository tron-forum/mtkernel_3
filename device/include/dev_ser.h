/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/.
 *
 *----------------------------------------------------------------------
 */

/*
 *	dev_ser.h
 *	Serial communication device driver
 */

#ifndef	__DEV_SER_API_H__
#define	__DEV_SER_API_H__

/*----------------------------------------------------------------------*/
/* Device driver initialization
 */
IMPORT ER dev_init_ser( UW unit );

/*----------------------------------------------------------------------*/
/* Attribute data number
 */
typedef enum {
	TDN_SER_MODE	= -100,		/* RW: Communication mode */
	TDN_SER_SPEED	= -101,		/* RW: Communication Speed */
	TDN_SER_SNDTMO	= -102,		/* RW: Send timeout */
	TDN_SER_RCVTMO	= -103,		/* RW: Receive timeout */
	TDN_SER_COMERR	= -104,		/* R-: Communication Error */
	TDN_SER_BREAK	= -105,		/* -W: Send Break */
} T_DN_SER_ATR;

/* Communication Error */
#define	DEV_SER_ERR_ROVR	(1<<7)	/* Recive buffe over flow */
#define	DEV_SER_ERR_SOVR	(1<<6)	/* Send buffer over flow */
#define	DEV_SER_ERR_OE		(1<<3)	/* Overrun Error */
#define	DEV_SER_ERR_BE		(1<<2)	/* Break Error */
#define	DEV_SER_ERR_PE		(1<<1)	/* Parity Error */
#define	DEV_SER_ERR_FE		(1<<0)	/* Framing Error */

/*----------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#ifdef CPU_TMPM369FDFG
#include "../ser/sysdepend/tx03_m367/ser_mode_m367.h"
#endif		/* CPU_TMPM369FDFG */
#ifdef CPU_RX231
#include "../ser/sysdepend/rx231/ser_mode_rx231.h"
#endif	/* CPU_RX231 */

#endif		/* __DEV_SER_API_H__ */