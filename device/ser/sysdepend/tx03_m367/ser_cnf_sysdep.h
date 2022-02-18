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
 *	ser_cnf_sysdep.h 
 *	Serial Device configuration file
 *		for TX03_M367 (TMPM369FDFG)
 */
#ifndef	__DEV_SER_CNF_M367_H__
#define	__DEV_SER_CNF_M367_H__

/* Interrupt priority */
#define	DEVCNF_UART4_INTPRI	5
#define	DEVCNF_UART5_INTPRI	5

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
