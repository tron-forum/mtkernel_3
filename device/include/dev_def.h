/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2012/05/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *	dev_def.c
 *	Device driver common definition
*/

#ifndef	__DEVINC_DEVDEF_H__
#define	__DEVINC_DEVDEF_H__

#include <config_device.h>

#if USE_SDEV_DRV		/* Use Sample device driver */

#if DEVCNF_USE_SER		/* Use Serial communication device */
#define DEV_SER_ENABLE		1
#endif	/* DEVCNF_USE_SER */

#if DEVCNF_USE_ADC		/* Use A/D conversion device */
#define DEV_ADC_ENABLE		1
#endif	/* DEVCNF_USE_ADC */

#if DEVCNF_USE_IIC		/* Use I2C communication device */
#define DEV_IIC_ENABLE		1
#endif	/* DEVCNF_USE_IIC */

#else				/* Do not use sample device driver */

#define DEV_SER_ENABLE		0		// Serial communication device 
#define DEV_ADC_ENABLE		0		// A/D conversion device
#define DEV_IIC_ENABLE		0		// I2C communication device

#endif	/* USE_SDEV_DRV */
#endif	/* __DEVINC_DEVICE_H__ */
