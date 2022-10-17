/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */


/*
 *	dev_adc_sydep.h
 *	A/D converter device driver
 *	System-dependent definition for RX231
 */

#ifndef	__DEV_ADC_RX231_H__
#define	__DEV_ADC_RX231_H__

#define	DEV_ADC_UNITNM	(1)	/* Number of devive units */
#define	DEV_ADC_UNIT0	(0)

#define ADC_CH_NUM	(24)		/* Number of A/DC chanels */

/*
 * ADC registers
 */
#define	ADCSR		(0x00089000)		/* A/D control reg. */
#define	ADCSR_ADST	(1<<15)
#define	ADCSR_ADIE	(1<<12)

#define	ADCER		(0x0008900E)		/* A/D Control extension reg. */
#define	ADCER_INI	(1<<5)			/* register initial value */


#define	ADANSA0		(0x00089004)		/* A/D chanel select reg. A0*/
#define	ADANSA1		(0x00089006)		/* A/D chanel select reg. A1*/
#define	ADANSB0		(0x00089014)		/* A/D chanel select reg. B0*/
#define	ADANSB1		(0x00089016)		/* A/D chanel select reg. B1*/

#define	ADDR(c)		(0x00089020 + 2*c)	/* A/D Data reg. */

/* A/D Sampling state reg. */
#define	ADSSTRL		(0x000890DD)
#define	ADSSTRT		(0x000890DE)
#define	ADSSTRO		(0x000890DF)
#define	ADSSTR(c)	(0x000890E0 + c)


/*
 * A/D converter interrupt number
*/
#define	INTNO_S10ADI0	102


#endif		/* __DEV_ADC_RX231_H__ */
