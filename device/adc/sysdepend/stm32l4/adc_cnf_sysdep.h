/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */


/*
 *	adc_cnf_sysdep.h 
 *	A/D converter device driver configuration file
 *		for STM32L4
 */
#ifndef	__DEV_ADC_CNF_STM32L4_H__
#define	__DEV_ADC_CNF_STM32L4_H__

/* Device clock initialization */
#define DEVCONF_ADC_INIT_MCLK	TRUE	// Initialize module clock
#define	DEVCNF_ADCSEL		1	// ADC clock source   1:PLLADC1CLK  2:PLLADC2CLK  3:SYSCLK

/* Device control data */
/* Common */
#define	DEVCNF_ADC_CKMODE	0	// ADC clock mode  0:Asynchronous clock mode
#define	DEVCNF_ADC_PRESC	10	// ADC prescaler  0 ~ 11

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
