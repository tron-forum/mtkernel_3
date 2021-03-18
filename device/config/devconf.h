/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03.
 *
 *----------------------------------------------------------------------
 */


/*
 *	devconf.h
 *	Device configuration definition
 */

#ifndef	__DEV_CONF_H__
#define	__DEV_CONF_H__

#if USE_SDEV_DRV
/* ------------------------------------------------------------------------ */
/* Device usage settings
 *	1: Use   0: Do not use
 */

#ifdef CPU_TMPM367FDFG
#define DEVCNF_DEV_SER		1		// Serial communication device 
#define DEVCNF_DEV_ADC		1		// A/D conversion device
#define DEVCNF_DEV_IIC		1		// I2C communication device
#endif

#ifdef CPU_RX231
#define DEVCNF_DEV_SER		1		// Serial communication device 
#define DEVCNF_DEV_ADC		1		// A/D conversion device
#define DEVCNF_DEV_IIC		1		// I2C communication device
#endif

#ifdef CPU_STM32L4
#define DEVCNF_DEV_SER		1		// Serial communication device 
#define DEVCNF_DEV_ADC		1		// A/D conversion device
#define DEVCNF_DEV_IIC		1		// I2C communication device
#endif

#endif /* USE_SDEV_DRV */
#endif	/* __DEV_CONF_H__ */