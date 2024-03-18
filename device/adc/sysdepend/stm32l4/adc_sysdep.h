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
 *	adc_sysdep.h
 *	A/D converter device driver
 *	System-dependent definition for STM32L4
 */

#ifndef	__DEV_ADC_STM32L4_H__
#define	__DEV_ADC_STM32L4_H__

#define	DEV_ADC_UNITNM	(3)	/* Number of device units */
#define	DEV_ADC_1	(0)	/* ADC1 */
#define	DEV_ADC_2	(1)	/* ADC2 */
#define	DEV_ADC_3	(2)	/* ADC3 */

/* NUmber of A/DC channels */
#define	ADC_CH_NUM	(19)
#define	ADC_MAX_SQ	(16)

/*
 * A/DC Clock source
 */
#define	RCC_CCIPR_ADCSEL	0x30000000	// ADC Clock source

/*----------------------------------------------------------------------
 * A/DC registers
 */

/* A/DC common register */
#define	ADC_CSR		(0x50040300UL)	// Common status register
#define	ADC_CCR		(0x50040308UL)	// Common control register
#define	ADC_CDR		(0x5004030CUL)	// Common regular register

/* Base address */
#define ADC1_BASE	(0x50040000UL)
#define ADC2_BASE	(0x50040100UL)
#define ADC3_BASE	(0x50040200UL)

/* Register offset */
#define	ADCx_ISR	(0x00)		// Interrupt & status register
#define	ADCx_IER	(0x04)		// Interrupt enable register
#define	ADCx_CR		(0x08)		// Control register
#define	ADCx_CFGR	(0x0C)
#define	ADCx_CFGR2	(0x10)
#define	ADCx_SMPR1	(0x14)		// Sampling time register 1
#define	ADCx_SMPR2	(0x18)		// Sampling time register 2
#define	ADCx_TR1	(0x20)
#define	ADCx_TR2	(0x24)
#define	ADCx_TR3	(0x28)
#define	ADCx_SQR1	(0x30)		// Regular sequence register 1
#define	ADCx_SQR2	(0x34)		// Regular sequence register 2
#define	ADCx_SQR3	(0x38)		// Regular sequence register 3
#define	ADCx_SQR4	(0x3C)		// Regular sequence register 4
#define	ADCx_DR		(0x40)		// Regular data register
#define	ADCx_JSQR	(0x4C)
#define	ADCx_OFR1	(0x60)
#define	ADCx_OFR2	(0x64)
#define	ADCx_OFR3	(0x68)
#define	ADCx_OFR4	(0x6C)
#define	ADCx_JDR1	(0x80)
#define	ADCx_JDR2	(0x84)
#define	ADCx_JDR3	(0x88)
#define	ADCx_JDR4	(0x8C)
#define	ADCx_AWD2CR	(0xA0)
#define	ADCx_AWD3CR	(0xA4)
#define	ADCx_DIFSEL	(0xB0)
#define	ADCx_CALFACT	(0xB4)

#define	ADC_ISR_ADRDY	(1<<0)		// ADC ready
#define	ADC_ISR_EOC	(1<<2)		// End of conversion
#define	ADC_ISR_EOS	(1<<3)		// End of regular sequence

#define	ADC_IER_ADRDYIE	(1<<0)		// ADC ready interrupt enable
#define	ADC_IER_EOCIE	(1<<2)		// End of conversion interrupt enable
#define	ADC_IER_OVRIE	(1<<4)		// Overrun interrupt enable


#define	ADC_CR_ADEN	(1<<0)		// ADC enable
#define	ADC_CR_ADDIS	(1<<1)		// ADC disable
#define	ADC_CR_ADSTART	(1<<2)		// ADC regular convert start
#define	ADC_CR_ADSTP	(1<<4)		// ADC regular convert stop
#define	ADC_CR_ADVREGEN	(1<<28)		// ADC voltage regulator enabled
#define	ADC_CR_DEEPPWD	(1<<29)		// ADC deep powerdown enable
#define	ADC_CR_ADCAL	(1<<31)		// ADC calibration

/*
 * A/D converter interrupt number
*/
#define	INTNO_INTADC1_2		18
#define	INTNO_INTADC3		47

#endif		/* __DEV_ADC_STM32L4_H__ */
