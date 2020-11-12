/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/.
 *
 *----------------------------------------------------------------------
 */


/*
 *	adc_cnf_stm32l4.h 
 *	A/D converter device driver configuration file
 *		for TX03_M367 (STM32L4)
 */
#ifndef	__DEV_ADC_CNF_STM32L4_H__
#define	__DEV_ADC_CNF_STM32L4_H__

/* Device initialization */
#define DEVCONF_ADC_INIT_MCLK	TRUE		// Initialize module clock

/* Device control data */
/* Common */
#define	ADC_CCR_INIT	ADC_CCR_CKMODE_HCLKx4	// ADC common control (ADC_CCR initial value)

/* ADC1 */
#define	DEVCONF_ADC1_SMPR1	0x00000030	// Sampling time (ADC_SMPR1 initial value)
#define	DEVCONF_ADC1_SMPR2	0x00000000	// Sampling time (ADC_SMPR2 initial value)

/* ADC2 */
#define	DEVCONF_ADC2_SMPR1	0x00000000	// Sampling time (ADC_SMPR1 initial value)
#define	DEVCONF_ADC2_SMPR2	0x00000000	// Sampling time (ADC_SMPR2 initial value)

/* ADC3 */
#define	DEVCONF_ADC3_SMPR1	0x00000000	// Sampling time (ADC_SMPR1 initial value)
#define	DEVCONF_ADC3_SMPR2	0x00000000	// Sampling time (ADC_SMPR2 initial value)

/* Interrupt t priority */
#define	DEVCNF_ADC12_INTPRI	5
#define	DEVCNF_ADC3_INTPRI	5

#endif		/* __DEV_ADC_CNF_STM32L4_H__ */
