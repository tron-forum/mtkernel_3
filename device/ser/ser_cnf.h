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
 *	ser_cnf.h 
 *	Serial communication device driver
 *		Device configuration file
 */
#ifndef	__DEV_SER_CNF_H__
#define	__DEV_SER_CNF_H__

#define	DEVCNF_SER_DEVNAME	"ser"		// Device name ("ser")

#define	DEVCONF_SER_BUFFSIZE	50		//  Communication data buffer size

/* Default value for attribute data */
#define	DEVCNF_SER_SPEED	115200		// Communication speed (baud rate)
#define DEVCNF_SER_MODE		(DEV_SER_MODE_CTSEN | DEV_SER_MODE_RTSEN | \
				DEV_SER_MODE_8BIT | DEV_SER_MODE_1STOP | DEV_SER_MODE_PNON)
						// Mode: Hard flow control enable,
						// 	data 8bit, stop 1bit, no parity
#define	DEVCNF_SER_SND_TMO	TMO_FEVR	// Send timeout 
#define DEVCNF_SER_RCV_TMO	TMO_FEVR	// Receive timeout 

/*----------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#define DEVDEF_SER_CNF_PATH_(a)	#a
#define DEVDEF_SER_CNF_PATH(a)	DEVDEF_SER_CNF_PATH_(a)
#define DEVDEF_SER_CNF_SYSDEP()	DEVDEF_SER_CNF_PATH(sysdepend/TARGET_CPU_DIR/ser_cnf_sysdep.h)
#include DEVDEF_SER_CNF_SYSDEP()

#endif		/* __DEV_SER_CNF_H__ */
