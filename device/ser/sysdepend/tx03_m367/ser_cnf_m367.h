/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.00.03
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */


/*
 *	ser_cnf_m367.h 
 *	Serial Device configuration file
 *		for TX03_M367 (TMPM369FDFG)
 */
#ifndef	__DEV_SER_CNF_M367_H__
#define	__DEV_SER_CNF_M367_H__

#define	DEVCNF_SER_INTPRI	5		// Interrupt priority

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  0: "sera" - UART4
 *	  1: "serb" - UART5
 *	  other : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	1		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif		/* __DEV_SER_CNF_M367_H__ */
