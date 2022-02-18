/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/2.
 *
 *----------------------------------------------------------------------
 */


/*
 *	adc_cnf_sysdep.h 
 *	A/D converter device driver configuration file
 *		for TX03_M367 (TMPM369FDFG)
 */
#ifndef	__DEV_ADC_CNF_M367_H__
#define	__DEV_ADC_CNF_M367_H__

/* Device control data */
/* ADC Unit-A */
#define DEVCNF_ADCA_CLK		0x00000001	// Clock setting (ADxCLK register initial value)
#define	DEVCNF_ADCA_INTPRI	5		// Interrupt priority
#define	DEVCNF_ADCA_TMOSCAN	1000		// A/D conversion timeout time

/* ADC Unit-B */
#define DEVCNF_ADCB_CLK		0x00000001	// Clock setting (ADxCLK register initial value)
#define	DEVCNF_ADCB_INTPRI	5		// Interrupt priority
#define	DEVCNF_ADCB_TMOSCAN	1000		// A/D conversion timeout time

#endif		/* __DEV_ADC_CNF_M367_H__ */
