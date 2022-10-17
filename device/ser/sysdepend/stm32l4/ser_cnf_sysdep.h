/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
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
 *		for STM32L4
 */
#ifndef	__DEV_SER_CNF_STM32L4_H__
#define	__DEV_SER_CNF_STM32L4_H__

/* Device clock initialization */
#define DEVCNF_SER_INIT_MCLK		TRUE		// Initialize module clock
#define DEVCNF_USARTxSEL_INIT		0x00000000	// All UART clocks are PCLK (bit 0-9)

/* Interrupt priority */
#define	DEVCNF_USART1_INTPRI	5
#define	DEVCNF_USART2_INTPRI	5
#define	DEVCNF_USART3_INTPRI	5
#define	DEVCNF_UART4_INTPRI	5
#define	DEVCNF_UART5_INTPRI	5

/* Debug option
 *	Specify the device used by T-Monitor.
 *	  -1 : T-Monitor does not use serial devices
 */
#if USE_TMONITOR
#define	DEVCNF_SER_DBGUN	1		// Used by T-Monitor
#else 
#define	DEVCNF_SER_DBGUN	-1		// T-Monitor not executed
#endif

#endif		/* __DEV_SER_CNF_STM32L4_H__ */
