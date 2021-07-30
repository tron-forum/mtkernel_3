/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/08.
 *
 *----------------------------------------------------------------------
 */


/*
 *	adc_cnf_rza2m.h 
 *	A/D converter device driver configuration file
 *		for RZ/A2M
 */
#ifndef	__DEV_ADC_CNF_RZA2M_H__
#define	__DEV_ADC_CNF_RZA2M_H__

/* Device initialization */
#define DEVCONF_ADC_INIT_MSTP	TRUE		// Initialize module stop
#define DEVCONF_ADC_INIT_PIN	TRUE		// Initialize I/O pin
#define DEVCONF_ENA_AN00_07	(0b00000111)	// AN00-AN07  1: Used  0: unused


/* Device control data */
/* Initial sampling rate (ADSSTR) */
#define	DEVCNF_ADSSTR0_INI	11
#define	DEVCNF_ADSSTR1_INI	11
#define	DEVCNF_ADSSTR2_INI	11
#define	DEVCNF_ADSSTR3_INI	11
#define	DEVCNF_ADSSTR4_INI	11
#define	DEVCNF_ADSSTR5_INI	11
#define	DEVCNF_ADSSTR6_INI	11
#define	DEVCNF_ADSSTR7_INI	11

#define	DEVCNF_ADC_INTPRI	5	// Interrupt priority
#define	DEVCNF_ADC_TMOSCAN	1000	// A/D conversion timeout time

#endif		/* __DEV_ADC_CNF_RZA2M_H__ */
