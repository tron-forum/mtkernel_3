/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */


/*
 *	ser_cnf_rza2m.h 
 *	Serial Device configuration file
 *		for RZ/A2M
 */
#ifndef	__DEV_SER_CNF_RZA2M_H__
#define	__DEV_SER_CNF_RZA2M_H__

/* Device initialization */
#define DEVCONF_SER_INIT_MSTP	FALSE		// Initialize module stop

/* Device control data */
#define	DEVCNF_SER_INTPRI	5		// Interrupt priority

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  0 - 4: "sera" - "sere"
 *	  other : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	4		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif		/* __DEV_SER_CNF_RZA2M_H__ */
