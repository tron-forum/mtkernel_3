/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/04.
 *
 *----------------------------------------------------------------------
 */


/*
 *	dev_adc_sydep.h
 *	A/D converter device driver
 *	System-dependent definition for RX65N
 */

#ifndef	__DEV_ADC_RX65N_H__
#define	__DEV_ADC_RX65N_H__

#define	DEV_ADC_UNITNM	(2)	/* Number of devive units */
#define	DEV_ADC_UNIT0	(0)
#define	DEV_ADC_UNIT1	(1)

#define ADC_CH_NUM0	(8)		/* Number of A/DC chanels */
#define ADC_CH_NUM1	(21)

/*
 * ADC registers
 */

/* Base address */
#define ADC0_BASE	(0x00089000UL)
#define ADC1_BASE	(0x00089100UL)

/* Register offset */
#define	ADCx_ADCSR	(0x00)		/* A/DC control reg. */
#define	ADCSR_ADST	(1<<15)
#define	ADCSR_ADIE	(1<<12)

#define	ADCx_ADANSA0	(0x04)		/* A/DC chanel select reg. A0*/
#define	ADCx_ADANSA1	(0x06)		/* A/DC chanel select reg. A1*/
#define	ADCx_ADCER	(0x0E)		/* A/DC Control extension reg. */
#define	ADCx_ADANSB0	(0x14)		/* A/DC chanel select reg. B0*/
#define	ADCx_ADANSB1	(0x16)		/* A/DC chanel select reg. B1*/

#define	ADCx_ADDR(c)	(0x20+(2*c))	/* A/DC Data reg. */

#define	ADCER_INI	(1<<5)		/* register initial value */

#define	ADCx_ADSSTR(c)	(0xE0+c)	/* A/D Sampling state reg. */
#define	ADC_ADSSTRL	(0x000891DDUL)
#define	ADC_ADSSTRT	(0x000891DEUL)
#define	ADC_ADSSTRO	(0x000891DFUL)

/*
 * A/D converter interrupt factor number
*/
#define	INTFN_S12ADI0	64
#define	INTFN_S12GBADI0	65
#define	INTFN_S12GCADI0	66

#define	INTFN_S12ADI1	68
#define	INTFN_S12GBADI1	69
#define	INTFN_S12GCADI1	70

#endif		/* __DEV_ADC_RX65N_H__ */
