/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21.
 *
 *----------------------------------------------------------------------
 */


/*
 *	ser_cnf_rx231.h 
 *	Serial Device configuration file
 *		for RX231
 */
#ifndef	__DEV_SER_CNF_RX231_H__
#define	__DEV_SER_CNF_RX231_H__

/* Device initialization */
#define DEVCONF_SER_INIT_MSTP	FALSE		// Initialize module stop

/* Device control data */
#define	DEVCNF_SER_INTPRI	5		// Interrupt priority

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  0 - 6: "sera" - "serg"
 *	  other : T-Monitor does not use serial devices
 */
#define	DEVCNF_SER_DBGUN	3		// Used by T-Monitor

#endif		/* __DEV_SER_CNF_RX231_H__ */
