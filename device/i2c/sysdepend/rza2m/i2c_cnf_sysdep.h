/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2022 by Ken Sakamura.
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
 *		for RZ/A2M
 */
#ifndef	__DEV_I2C_CNF_RZA2M_H__
#define	__DEV_I2C_CNF_RZA2M_H__

/* Device initialization */
#define DEVCONF_I2C_INIT_MSTP	FALSE		// Initialize module stop

/* Frequency selection (0-7)  ICMR1.CKS */
#define	DEVCNF_I2C0_CKS		(5)
#define	DEVCNF_I2C1_CKS		(5)
#define	DEVCNF_I2C2_CKS		(5)
#define	DEVCNF_I2C3_CKS		(5)

/* Clock width  bit0-4:BRL  bit8-12 BRH */
#define	DEVCNF_I2C0_BR		(16<<8)|(14)
#define	DEVCNF_I2C1_BR		(16<<8)|(14)
#define	DEVCNF_I2C2_BR		(16<<8)|(14)
#define	DEVCNF_I2C3_BR		(16<<8)|(14)

/* Interrupt priority */
#define	DEVCNF_I2C0_INTPRI	5
#define	DEVCNF_I2C1_INTPRI	5
#define	DEVCNF_I2C2_INTPRI	5
#define	DEVCNF_I2C3_INTPRI	5

/* Communication timeout time */
#define	DEVCNF_I2C_TMO		1000

#endif		/* __DEV_I2C_CNF_RZA2M_H__ */
