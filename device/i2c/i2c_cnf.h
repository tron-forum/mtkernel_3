/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020//.
 *
 *----------------------------------------------------------------------
 */

/*
 *	i2c_cnf.h 
 *	I2C device driver
 *		Device configuration file
 */
#ifndef	__DEV_I2C_CNF_H__
#define	__DEV_I2C_CNF_H__


#define	DEVCNF_I2C_DEVNAME	"iic"		// Device name ("iic") */

#define	DEVCNF_I2C_MAX_SDATSZ	100		// Maximum transmission data size
#define	DEVCNF_I2C_MAX_RDATSZ	100		// Maximum received data size


/* Hardware dependent definition */
#ifdef CPU_TMPM369FDFG
#include "sysdepend/tx03_m367/i2c_cnf_m367.h"
#endif		/* CPU_TMPM369FDFG */


#endif		/* __DEVCNF_I2C_H__ */
