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
 *	i2c_cnf.h 
 *	I2C device driver
 *		Device configuration file
 */
#ifndef	__DEV_I2C_CNF_H__
#define	__DEV_I2C_CNF_H__


#define	DEVCNF_I2C_DEVNAME	"iic"		// Device name ("iic")

#define	DEVCNF_I2C_MAX_SDATSZ	100		// Maximum transmission data size
#define	DEVCNF_I2C_MAX_RDATSZ	100		// Maximum received data size


/*----------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#define DEVDEF_I2C_CNF_PATH_(a)	#a
#define DEVDEF_I2C_CNF_PATH(a)	DEVDEF_I2C_CNF_PATH_(a)
#define DEVDEF_I2C_CNF_SYSDEP()	DEVDEF_I2C_CNF_PATH(sysdepend/TARGET_CPU_DIR/i2c_cnf_sysdep.h)
#include DEVDEF_I2C_CNF_SYSDEP()

#endif		/* __DEVCNF_I2C_H__ */
