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
 *		for RX231
 */
#ifndef	__DEV_ADC_CNF_RX231_H__
#define	__DEV_ADC_CNF_RX231_H__

/* Device initialization */
#define DEVCONF_ADC_INIT_MSTP	FALSE		// Initialize module stop

/* Device control data */
/* Initial sampling rate (ADSSTR) */
#define	DEVCNF_ADSSTR0_INI	6
#define	DEVCNF_ADSSTR1_INI	6
#define	DEVCNF_ADSSTR2_INI	6
#define	DEVCNF_ADSSTR3_INI	6
#define	DEVCNF_ADSSTR4_INI	6
#define	DEVCNF_ADSSTR5_INI	6
#define	DEVCNF_ADSSTR6_INI	6
#define	DEVCNF_ADSSTR7_INI	6
#define	DEVCNF_ADSSTRL_INI	6

#define	DEVCNF_ADC_INTPRI	5	// Interrupt priority
#define	DEVCNF_ADC_TMOSCAN	1000	// A/D conversion timeout time

#endif		/* __DEV_ADC_CNF_RX231_H__ */
