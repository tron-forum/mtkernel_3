/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.00.03
 *
 *    Copyright (C) 2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
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

/* ADC Clock (RCC) */
#define	DEVCNF_ADCSEL		1		// ADC clock source

/* Device control data */
/* Common */
#define	DEVCNF_ADC_CKMODE	0		// ADC clock mode
#define	DEVCNF_ADC_PRESC	0		// ADC prescaler

/* ADC1 */
#define	DEVCONF_ADC1_SMPR1	0x00000000	// Sampling time (ADC_SMPR1 initial value)
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

/* A/D conversion timeout time */
#define	DEVCNF_ADC_TMOSCAN	1000

#endif		/* __DEV_ADC_CNF_STM32L4_H__ */
