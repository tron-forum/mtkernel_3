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
 *	dev_adc_sysdep.h
 *	A/D converter device driver
 *	System-dependent definition for RZ/A2M
 */

#ifndef	__DEV_ADC_RZA2M_H__
#define	__DEV_ADC_RZA2M_H__

#define	DEV_ADC_UNITNM	(1)		/* Number of device units */
#define	DEV_ADC_UNIT0	(0)

#define ADC_CH_NUM	(8)		/* Number of A/DC channels */

/*
 * ADC registers
 */
#define	ADCSR		(0xE8005800)	// A/D control register
#define	ADCSR_ADIE	(1<<12)		// Scan end interrupt enabled bit
#define	ADCSR_ADST	(1<<15)		// A/DC start bit
#define	ADCSR_INI	(0)		// ADCSR init value (ADC stop, Single scan mode)

#define	ADANSA0		(0xE8005804)
#define	ADADS0		(0xE8005808)
#define	ADADC		(0xE800580C)
#define	ADCER		(0xE800580E)
#define	ADSTRGR		(0xE8005810)
#define	ADANSB0		(0xE8005814)
#define	ADDBLDR		(0xE8005818)
#define	ADRD		(0xE800581E)

/* A/D dara register ch.0-7 */
#define	ADDR(c)		(0xE8005820 + (c<<1))

#define	ADDISCR		(0xE800587A)

#define	ADGSPCR		(0xE8005880)
#define	ADDBLDRA	(0xE8005884)
#define	ADDBLDRB	(0xE8005886)
#define	ADWINMON	(0xE800588C)

#define	ADCMPCR		(0xE8005890)
#define	ADCMPANSR0	(0xE8005894)
#define	ADCMPLR0	(0xE8005898)
#define	ADCMPDR0	(0xE800589C)
#define	ADCMPDR1	(0xE800589E)

#define	ADCMPSR0	(0xE80058A0)
#define	ADCMPBNSR	(0xE80058A6)
#define	ADCWINLLB	(0xE80058A8)
#define	ADCWINLUB	(0xE80058AA)
#define	ADCMPBSR	(0xE80058AC)

#define	ADANSC0		(0xE80058D4)
#define	ADGCTRGR	(0xE80058D9)

/* A/D Sampling state register ch.0-7 */
#define	ADSSTR(c)	(0xE80058E0 + c)

/*
 * A/D converter interrupt number
*/
#define	INTNO_S12ADI0		247
#define	INTNO_S12GBADI0		248
#define	INTNO_S12GCADI0		249
#define	INTNO_S12ADCMPAI0	250
#define	INTNO_S12ADCMPBI0	251

#endif		/* __DEV_ADC_RZA2M_H__ */
