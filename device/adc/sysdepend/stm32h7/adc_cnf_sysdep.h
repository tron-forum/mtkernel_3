/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2022 by Ken Sakamura.
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
 *		for STM32H7
 */
#ifndef	__DEV_ADC_CNF_STM32H7_H__
#define	__DEV_ADC_CNF_STM32H7_H__

/* Device initialization */
#define DEVCNF_ADC_INIT_MCLK	TRUE	// Initialize module clock
#define	DEVCNF_ADCSEL		0	// ADC clock source   0:PLL2P  1:PLL3R  2: PER_CK


/* ADC peripherals clock configuration */
//#define ADC_PLL2_DIVM2		0	// RCC_PLLCKSELR_DIVM2

//#define ADC_PLL2DIVR_INIT	((1<<24)|(1<<16)|(1<<9)|(23))
				// PLL2 N = 24	
				// PLL2 P = 2	
				// PLL2 Q= 2	
				// PLL2 R = 2	

//#define ADC_PLL2_RGE		3	// 8 ~ 16 MHz
//#define ADC_PLL2_VCOSEL		0	// Wide VCO range
//#define ADC_PLL2_FRACN		0

#define	ADC_PCLK	96000000UL	// 96MHz
#if (ADC_PCLK/2) <= 6250000UL
	#define ADC_CR_BOOST_VAL	0
#elif (ADC_PCLK/2) <= 12500000UL
	#define ADC_CR_BOOST_VAL	1
#elif (ADC_PCLK/2) <= 25000000UL
	#define ADC_CR_BOOST_VAL	2
#else /* > 25000000UL */
	#define ADC_CR_BOOST_VAL	3
#endif

/* Device control data */
/* Common */
#define	DEVCNF_ADC12_CKMODE	1	// CKMODE = 1
#define	DEVCNF_ADC12_PRESC	0	// div 1
#define	DEVCNF_ADC3_CKMODE	1	// CKMODE = 1
#define	DEVCNF_ADC3_PRESC	0	// div 1

/* ADC configuration */
#define DEVCNF_ADC1_CFGR	0x80000000
#define DEVCNF_ADC2_CFGR	0x80000000
#define DEVCNF_ADC3_CFGR	0x80000000
	// CONT   = 0	Single conversion mode
	// OVRMOD = 0	Overrun Mode 0
	// RES    = 0	Data resolution 16bit
	// DISCEN = 0	Discontinuous mode for regular channels disabled
	// EXTEN  = 0	Hardware trigger detection disabled
	// AUTDLY = 0	Auto-delayed conversion mode off
	// DMNGT  = 0	Regular conversion data stored in DR only
	// AUTDLY = 0	Auto-delayed conversion mode off
	// DMNGT  = 0	Regular conversion data stored in DR only

#define	DEVCNF_ADC1_CFGR2	0x00000000
#define	DEVCNF_ADC2_CFGR2	0x00000000
#define	DEVCNF_ADC3_CFGR2	0x00000000
	// ROVSE = 0	Regular Oversampling disabled
	// LSHIFT = 0	No left shift

/* ADC1 */
#define	DEVCNF_ADC1_SMPR1	0x00000000	// Sampling time (ADC_SMPR1 initial value)
#define	DEVCNF_ADC1_SMPR2	0x00000000	// Sampling time (ADC_SMPR2 initial value)

/* ADC2 */
#define	DEVCNF_ADC2_SMPR1	0x00000000	// Sampling time (ADC_SMPR1 initial value)
#define	DEVCNF_ADC2_SMPR2	0x00000000	// Sampling time (ADC_SMPR2 initial value)

/* ADC3 */
#define	DEVCNF_ADC3_SMPR1	0x00000000	// Sampling time (ADC_SMPR1 initial value)
#define	DEVCNF_ADC3_SMPR2	0x00000000	// Sampling time (ADC_SMPR2 initial value)

/* Interrupt t priority */
#define	DEVCNF_ADC1_2_INTPRI	5
#define	DEVCNF_ADC3_INTPRI	5


/* A/D conversion timeout time */
#define	DEVCNF_ADC_TMOSCAN	1000

#endif		/* __DEV_ADC_CNF_STM32H7_H__ */
