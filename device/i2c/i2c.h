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
 *	i2c.h
 *	I2C device driver
 */

#ifndef	__DEV_I2C_H__
#define	__DEV_I2C_H__

#include "../common/drvif/msdrvif.h"
#include "../include/dev_i2c.h"
#include "i2c_cnf.h"

/*----------------------------------------------------------------------
/* Hardware dependent definition
 */
#ifdef CPU_TMPM369FDFG
#include "sysdepend/tx03_m367/i2c_m367.h"
#endif		/* CPU_TMPM369FDFG */

/*----------------------------------------------------------------------
/* Device driver Control block
 */
typedef struct {
	UW	unit;		/* Unit No. */
	UINT	omode;		/* Open mode */

	ID	evtmbfid;			/* MBF ID for event notification */
} T_I2C_DCB;

/*----------------------------------------------------------------------
/* Device driver low-level interface
 * 	These functions are defined in the system dependent part.
 */
IMPORT W dev_i2c_llctl( UW, INT, UW, UW, UW*);		/* Low-Level device control */
IMPORT ER dev_i2c_llinit( T_I2C_DCB *p_dcb);	/* Low-level device initialization */

typedef enum {
	LLD_I2C_OPEN,
	LLD_I2C_CLOSE,
	LLD_I2C_READ,
	LLD_I2C_WRITE,
	LLD_I2C_EXEC,
} T_LLD_I2C_CMD;


#endif		/* __DEV_I2C_H__ */
