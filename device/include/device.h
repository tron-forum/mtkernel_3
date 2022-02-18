/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *	device.c
 *	Device driver definition for User program
*/

#ifndef	__DEVINC_DEVICE_H__
#define	__DEVINC_DEVICE_H__

#include <config_device.h>

#if USE_SDEV_DRV		/* Use Sample device driver */

#if DEVCNF_USE_SER		/* Use Serial communication device */
#include "dev_ser.h"
#endif	/* DEVCNF_USE_SER */

#if DEVCNF_USE_ADC		/* Use A/D conversion device */
#include "dev_adc.h"
#endif	/* DEVCNF_USE_ADC */

#if DEVCNF_USE_IIC		/* Use I2C communication device */
#include "dev_i2c.h"
#endif	/* DEVCNF_USE_IIC */

#endif	/* USE_SDEV_DRV */
#endif	/* __DEVINC_DEVICE_H__ */
