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
 *	i2c_cnf_sysdep.h 
 *	I2C device configuration file
 *		for RX231
 */
#ifndef	__DEV_I2C_CNF_RX231_H__
#define	__DEV_I2C_CNF_RX231_H__

/* Device initialization */
#define DEVCONF_I2C_INIT_MSTP	FALSE		// Initialize module stop

/* Frequency selection (0-7)  ICMR1.CKS */
#define	DEVCNF_I2C_CKS		1

/* Interrupt priority */
#define	DEVCNF_I2C_INTPRI	5

/* Communication timeout time */
#define	DEVCNF_I2C_TMO		1000

#endif		/* __DEV_I2C_CNF_RX231_H__ */
