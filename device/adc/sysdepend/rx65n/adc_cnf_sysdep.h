/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2021-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */

/*
 *	adc_cnf_sysdep.h 
 *	A/D converter device driver configuration file
 *		for RX65N
 */
#ifndef	__DEV_ADC_CNF_RX65N_H__
#define	__DEV_ADC_CNF_RX65N_H__

/* Device initialization */
#define DEVCONF_ADC_INIT_MSTP	FALSE		// Initialize module stop
#define	DEVCONF_ADC_SET_PERI	TRUE		// Set Software Configurable Interrupt

/* Device control data */
/* Initial sampling rate (ADSSTR) */
#define	DEVCNF_ADC0_ADSSTR0_INI		11
#define	DEVCNF_ADC0_ADSSTR1_INI		11
#define	DEVCNF_ADC0_ADSSTR2_INI		11
#define	DEVCNF_ADC0_ADSSTR3_INI		11
#define	DEVCNF_ADC0_ADSSTR4_INI		11
#define	DEVCNF_ADC0_ADSSTR5_INI		11
#define	DEVCNF_ADC0_ADSSTR6_INI		11
#define	DEVCNF_ADC0_ADSSTR7_INI		11

#define	DEVCNF_ADC1_ADSSTR0_INI		11
#define	DEVCNF_ADC1_ADSSTR1_INI		11
#define	DEVCNF_ADC1_ADSSTR2_INI		11
#define	DEVCNF_ADC1_ADSSTR3_INI		11
#define	DEVCNF_ADC1_ADSSTR4_INI		11
#define	DEVCNF_ADC1_ADSSTR5_INI		11
#define	DEVCNF_ADC1_ADSSTR6_INI		11
#define	DEVCNF_ADC1_ADSSTR7_INI		11
#define	DEVCNF_ADC1_ADSSTR8_INI		11
#define	DEVCNF_ADC1_ADSSTR9_INI		11
#define	DEVCNF_ADC1_ADSSTR10_INI	11
#define	DEVCNF_ADC1_ADSSTR11_INI	11
#define	DEVCNF_ADC1_ADSSTR12_INI	11
#define	DEVCNF_ADC1_ADSSTR13_INI	11
#define	DEVCNF_ADC1_ADSSTR14_INI	11
#define	DEVCNF_ADC1_ADSSTR15_INI	11

#define	DEVCNF_ADC1_ADSSTRL_INI		11
#define	DEVCNF_ADC1_ADSSTRT_INI		11
#define	DEVCNF_ADC1_ADSSTRO_INI		11

/*
 * A/D converter interrupt number
 *	Can be set between INTB128 and INTB207
*/
#define	INTNO_S12ADI0		130
#define	INTNO_S12ADI1		131

#define	DEVCNF_ADC0_INTPRI	5	// Interrupt priority
#define	DEVCNF_ADC1_INTPRI	5	// Interrupt priority

#define	DEVCNF_ADC_TMOSCAN	1000	// A/D conversion timeout time

#endif		/* __DEV_ADC_CNF_RX65N_H__ */
