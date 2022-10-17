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
 *	i2c_cnf_sysdep.h 
 *	I2C device configuration file
 *		for RX65N
 */
#ifndef	__DEV_I2C_CNF_RX65N_H__
#define	__DEV_I2C_CNF_RX65N_H__

/* Device initialization */
#define DEVCONF_I2C_INIT_MSTP	FALSE		// Initialize module stop

/* Communication bit rate setting */
#define	DEVCNF_I2C_CKS		5		// Frequency selection (0-7)  ICMR1.CKS
#define	DEVCNF_ICBRH_INI	(0xEE)
#define	DEVCNF_ICBRL_INI	(0xF1)

/* Interrupt priority */
#define	DEVCNF_I2C0_INTPRI	5
#define	DEVCNF_I2C1_INTPRI	5
#define	DEVCNF_I2C2_INTPRI	5

/* Communication timeout time */
#define	DEVCNF_I2C0_TMO		1000
#define	DEVCNF_I2C1_TMO		1000
#define	DEVCNF_I2C2_TMO		1000

#endif		/* __DEV_I2C_CNF_RX65N_H__ */
