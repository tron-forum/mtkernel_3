/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/.
 *
 *----------------------------------------------------------------------
 */


/*
 *	i2c_cnf_stm32l4.h 
 *	I2C device configuration file
 *		for STM32L4
 */
#ifndef	__DEV_I2C_CNF_STM32L4_H__
#define	__DEV_I2C_CNF_STM32L4_H__

/* Device initialization */
#define DEVCONF_I2C_INIT_MCLK	FALSE		// Initialize module clock

/* ADC Clock (I2C) */
#define	DEVCNF_I2CSEL		0		// I2C clock source

/* Register initial value */
#define	I2C_TIMINGR_INIT		0x10909CEC	// Standard mode, 100KHz
//#define	I2C_TIMINGR_INIT	0x00702991	// Fast mode, 400KHz

// Interrupt priority
#define	DEVCNF_I2C1_INTPRI	5
#define	DEVCNF_I2C2_INTPRI	5
#define	DEVCNF_I2C3_INTPRI	5

/* Communication timeout time */
#define	DEVCNF_I2C1_TMO		1000
#define	DEVCNF_I2C2_TMO		1000
#define	DEVCNF_I2C3_TMO		1000


#endif		/* __DEV_I2C_CNF_STM32L4_H__ */
