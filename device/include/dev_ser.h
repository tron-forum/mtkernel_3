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
 *	dev_ser.h
 *	Serial communication device driver
 */

#ifndef	__DEVINC_SER_H__
#define	__DEVINC_SER_H__

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
#define	DEV_SER_ERR_ROVR	(1<<7)	/* Receive buffer over flow */

/*----------------------------------------------------------------------*/
/* Hardware dependent definition
 */

#define DEVDEF_SER_MODE_PATH_(a)	#a
#define DEVDEF_SER_MODE_PATH(a)		DEVDEF_SER_MODE_PATH_(a)
#define DEVDEF_SER_MODE_SYSDEP()	DEVDEF_SER_MODE_PATH(../ser/sysdepend/TARGET_CPU_DIR/ser_mode_sysdep.h)
#include DEVDEF_SER_MODE_SYSDEP()

#endif	/* __DEVINC_SER_H__ */