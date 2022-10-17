/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02
 *
 *----------------------------------------------------------------------
 */

/*
 *	i2c_cnf_sysdep.h 
 *	I2C device configuration file
 *		for STM32H7
 */
#ifndef	__DEV_I2C_CNF_STM32H7_H__
#define	__DEV_I2C_CNF_STM32H7_H__

/* Device initialization */
#define DEVCONF_I2C_INIT_MCLK	TRUE	// Initialize module clock

/* ADC Clock (I2C) */
#define	DEVCNF_I2CSEL		0	// I2C clock source  0 = PCLK1

/* Register initial value */
#define	I2C_TIMINGR_INIT	0x60404E72	// Standard mode, 100KHz


// Interrupt priority
#define	DEVCNF_I2C1_INTPRI	5
#define	DEVCNF_I2C2_INTPRI	5
#define	DEVCNF_I2C3_INTPRI	5
#define	DEVCNF_I2C4_INTPRI	5
#define	DEVCNF_I2C5_INTPRI	5

/* Communication timeout time */
#define	DEVCNF_I2C1_TMO		1000
#define	DEVCNF_I2C2_TMO		1000
#define	DEVCNF_I2C3_TMO		1000
#define	DEVCNF_I2C4_TMO		1000
#define	DEVCNF_I2C5_TMO		1000


#endif		/* __DEV_I2C_CNF_STM32H7_H__ */
