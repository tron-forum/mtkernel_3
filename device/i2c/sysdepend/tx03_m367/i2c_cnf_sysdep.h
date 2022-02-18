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
 *		for TX03_M367 (TMPM369FDFG)
 */
#ifndef	__DEV_I2C_CNF_M367_H__
#define	__DEV_I2C_CNF_M367_H__

/* Frequency selection (0-6)  SBIxCR1.SCK */
#define	DEVCNF_I2C0_SCK		0
#define	DEVCNF_I2C1_SCK		0
#define	DEVCNF_I2C2_SCK		0

// Interrupt priority
#define	DEVCNF_I2C0_INTPRI	5
#define	DEVCNF_I2C1_INTPRI	5
#define	DEVCNF_I2C2_INTPRI	5

/* Communication timeout time */
#define	DEVCNF_I2C0_TMO		1000
#define	DEVCNF_I2C1_TMO		1000
#define	DEVCNF_I2C2_TMO		1000


#endif		/* __DEV_I2C_CNF_M367_H__ */
