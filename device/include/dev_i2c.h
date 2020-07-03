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
 *	dev_i2c.h
 *	I2C device driver
 */

#ifndef	__DEV_I2C_API_H__
#define	__DEV_I2C_API_H__

/*----------------------------------------------------------------------
/* Device driver initialization
 */
IMPORT ER dev_init_i2c( UW unit );	/* Device driver initialization */


/*----------------------------------------------------------------------
/* Attribute data number
 */
typedef enum {
	TDN_I2C_EXEC	= -100
} T_DN_I2C_ATR;

/* Send / receive data type (Used by I2C_ATR_EXEC) */
typedef struct {
	UW		sadr;
	SZ		snd_size;
	UB		*snd_data;
	SZ		rcv_size;
	UB		*rcv_data;
} T_I2C_EXEC;


#endif	/* __DEV_I2C_API_H__ */