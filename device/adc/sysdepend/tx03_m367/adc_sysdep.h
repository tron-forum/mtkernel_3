/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21.
 *
 *----------------------------------------------------------------------
 */


/*
 *	adc_sysdep.h
 *	A/D converter device driver
 *	System-dependent definition for TX03_M367
 */

#ifndef	__DEV_ADC_M367_H__
#define	__DEV_ADC_M367_H__

#define	DEV_ADC_UNITNM	(2)	/* Number of device units */
#define	DEV_ADC_UNIT0	(0)
#define	DEV_ADC_UNIT1	(1)

#define ADC_CH_NUM	(4)		/* NUmber of A/DC channels */

/*
 * ADC registers
 */
#define ADA_BASE	(0x40050000UL)
#define ADB_BASE	(0x40051000UL)

#define ADxCLK		(0x00)		/* Clock setting register */

/* Mode register */
#define ADxMOD0		(0x04)
#define ADxMOD1		(0x08)
#define ADxMOD2		(0x0C)
#define ADxMOD3		(0x10)
#define ADxMOD4		(0x14)
#define ADxMOD5		(0x18)
#define ADxMOD6		(0x1C)

/* Monitor interrupt setting register */
#define ADxCMPCR0	(0x24)
#define ADxCMPCR1	(0x28)

/* Conversion result comparison register */
#define ADxCMP0		(0x2C)
#define ADxCMP1		(0x30)

/* Conversion result storage register */
#define ADxREG00	(0x34)
#define ADxREG01	(0x38)
#define ADxREG02	(0x3C)
#define ADxREG03	(0x40)
#define ADxREG04	(0x44)
#define ADxREG05	(0x48)
#define ADxREG06	(0x4C)
#define ADxREG07	(0x50)

/* Register Bit definition */
#define ADxMOD0_ADS	(1<<0)
#define ADxMOD1_DACON	(1<<7)

/* Conversion mode (ADxMOD3) */
#define	ADMD_CHFIX_SINGLE	0x00	// MOD3.REPEAT = 0, MODE3.SCAN = 0
#define	ADMD_CHSCAN_SINGLE	0x01	// MOD3.REPEAT = 0, MODE3.SCAN = 1

/*
 * A/D converter interrupt number
*/
#define	INTNO_INTADA	43
#define	INTNO_INTADB	47

#endif		/* __DEV_ADC_M367_H__ */
