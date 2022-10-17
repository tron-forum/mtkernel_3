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
 *	ser_cnf_sysdep.h 
 *	Serial Device configuration file
 *		for RX65N
 */
#ifndef	__DEV_SER_CNF_RX65N_H__
#define	__DEV_SER_CNF_RX65N_H__

/* Device initialization */
#define DEVCONF_SER_INIT_MSTP	FALSE		// Initialize module stop

/* Interrupt priority */
#define	DEVCNF_SCI0_INTPRI	5
#define	DEVCNF_SCI1_INTPRI	5
#define	DEVCNF_SCI2_INTPRI	5
#define	DEVCNF_SCI3_INTPRI	5
#define	DEVCNF_SCI4_INTPRI	5
#define	DEVCNF_SCI5_INTPRI	5
#define	DEVCNF_SCI6_INTPRI	5
#define	DEVCNF_SCI7_INTPRI	5
#define	DEVCNF_SCI8_INTPRI	5
#define	DEVCNF_SCI9_INTPRI	5
#define	DEVCNF_SCI10_INTPRI	5
#define	DEVCNF_SCI11_INTPRI	5
#define	DEVCNF_SCI12_INTPRI	5

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  0 - 6: "sera" - "serg"
 *	  other : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	8		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif		/* __DEV_SER_CNF_RX65N_H__ */
