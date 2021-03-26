/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03.
 *
 *----------------------------------------------------------------------
 */


/*
 *	ser_cnf_stm32l4.h 
 *	Serial Device configuration file
 *		for STM32L4
 */
#ifndef	__DEV_SER_CNF_STM32L4_H__
#define	__DEV_SER_CNF_STM32L4_H__

/* Device initialization */
#define DEVCONF_SER_INIT_MCLK	FALSE		// Initialize module clock

/* Device control data */
#define	DEVCNF_SER_INTPRI	5		// Interrupt priority

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  0: "sera" - USART1
 *	  1: "serb" - USART2
 *	  2: "serc" - USART3
 *	  other : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	1		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif		/* __DEV_SER_CNF_STM32L4_H__ */
