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

#include <sys/machine.h>
#ifdef CPU_STM32H7

#include <tk/tkernel.h>
#include "../../adc.h"
#include "../../../include/dev_def.h"
#if DEV_ADC_ENABLE
/*
 *	dev_adc_stm32h7.c
 *	A/D converter device driver
 *	System dependent processing for STM32H7
 */

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_ADC_UNITNM] = { ADC1_BASE, ADC2_BASE, ADC3_BASE };
const LOCAL UW cba[DEV_ADC_UNITNM] = { ADC12_CBASE, ADC12_CBASE, ADC3_CBASE };

#define	ADC_ISR(x)	(ba[x] + ADCx_ISR)	// Interrupt & status register
#define	ADC_IER(x)	(ba[x] + ADCx_IER)	// Interrupt enable register
#define	ADC_CR(x)	(ba[x] + ADCx_CR)	// Control register
#define	ADC_CFGR(x)	(ba[x] + ADCx_CFGR)	// configuration register
#define	ADC_CFGR2(x)	(ba[x] + ADCx_CFGR2)	// configuration register 2
#define	ADC_SMPR1(x)	(ba[x] + ADCx_SMPR1)	// Sampling time register 1
#define	ADC_SMPR2(x)	(ba[x] + ADCx_SMPR2)	// Sampling time register 2
#define	ADC_PCSEL(x)	(ba[x] + ADCx_PCSEL)	// Channel preselection register
#define	ADC_SQR1(x)	(ba[x] + ADCx_SQR1)	// Regular sequence register
#define	ADC_DR(x)	(ba[x] + ADCx_DR)	// Regular data register

#define	ADC_CSR(x)	(cba[x] + ADCx_CSR)	// Common status register
#define	ADC_CCR(x)	(cba[x] + ADCx_CCR)	// Common control register

/*----------------------------------------------------------------------
 * Device control data
*/
LOCAL struct {
	ID	wait_tskid;
	UW	cfgr, cfgr2;
	UW	smpr1, smpr2;
	UW	*buf;
} ll_devcb[DEV_ADC_UNITNM] = {
	{0, DEVCNF_ADC1_CFGR, DEVCNF_ADC1_CFGR2, DEVCNF_ADC1_SMPR1, DEVCNF_ADC1_SMPR2, 0},
	{0, DEVCNF_ADC2_CFGR, DEVCNF_ADC2_CFGR2, DEVCNF_ADC2_SMPR1, DEVCNF_ADC2_SMPR2, 0},
	{0, DEVCNF_ADC3_CFGR, DEVCNF_ADC3_CFGR2, DEVCNF_ADC3_SMPR1, DEVCNF_ADC3_SMPR2, 0}
};


/*----------------------------------------------------------------------
 * Interrupt handler
 */
void adc_inthdr( UINT intno)
{
	UW	isr;
	UW	unit;

	if(intno == INTNO_INTADC3) {
		unit = DEV_ADC_3;
	} else if( in_w(ADC_ISR(DEV_ADC_1))) {
		unit = DEV_ADC_1;
	} else if( in_w(ADC_ISR(DEV_ADC_2))) {
		unit = DEV_ADC_2;
	} else {
		ClearInt(intno);
		return;
	}

	isr = in_w(ADC_ISR(unit));
	if(isr & (ADC_ISR_ADRDY | ADC_ISR_EOS)) {
		if(ll_devcb[unit].wait_tskid) {
			tk_wup_tsk(ll_devcb[unit].wait_tskid);
		}
	}
	if(isr & ADC_ISR_EOC) {
		*(ll_devcb[unit].buf++) = in_w(ADC_DR(unit));
		isr &= ~ADC_ISR_EOC;
	}

	out_w(ADC_ISR(unit), isr);	// Clear all interrupt flag.
	ClearInt(intno);
}

/*----------------------------------------------------------------------
 * A/D convert
 */
LOCAL UW adc_convert( UINT unit, INT ch, INT size, UW *buf )
{
	_UW	*sqr;
	UINT	sqsz, sqch, sqpos;
	UW	pcsel;
	ER	err;

	if((ch >= ADC_CH_NUM) || (size > ADC_MAX_SQ) || ((ch+size) > ADC_CH_NUM)) return (UW)E_PAR;

	out_w(ADC_SQR1(unit), size-1);		// Set channel sequence length.
	
	/* Set channel sequence */
	sqr = (UW*)ADC_SQR1(unit);
	sqsz = size; sqch = ch; sqpos = 6; pcsel = 0;
	while(sqsz--) {
		pcsel |= 1<<sqch;
		*sqr |= (sqch++)<<sqpos;
		if((sqpos += 6) >= 32) {
			sqpos = 0;
			*(++sqr) = 0; 
		}
	}
	if(unit != DEV_ADC_3) {
		out_w(ADC_PCSEL(unit), pcsel);		// Set channel preselection
	}

	ll_devcb[unit].wait_tskid = tk_get_tid();
	ll_devcb[unit].buf = buf;

	tk_can_wup(TSK_SELF);
	out_w(ADC_CR(unit), ADC_CR_ADSTART | ADC_CR_ADVREGEN);	// Start Convert
	err = tk_slp_tsk(DEVCNF_ADC_TMOSCAN);
	ll_devcb[unit].wait_tskid = 0;

	return (err < E_OK)? err:size;
}


/*----------------------------------------------------------------------
 * A/DC open
 */
LOCAL ER adc_open(UW unit)
{
	ER	err = E_OK;

	/* Set sampling time */
	out_w(ADC_SMPR1(unit), ll_devcb[unit].smpr1);
	out_w(ADC_SMPR2(unit), ll_devcb[unit].smpr2);

	/* Initialize interrupt */
	out_w(ADC_ISR(unit), 0x000007FF);			// Clear all interrupt flag.
	out_w(ADC_IER(unit), ADC_IER_ADRDYIE | ADC_IER_EOCIE);	// Set Interrupt mask.

	if(unit != DEV_ADC_3) {		// ADC1 or ADC2
		EnableInt(INTNO_INTADC1_2, DEVCNF_ADC1_2_INTPRI);
	} else {			// ADC3
		EnableInt(INTNO_INTADC3, DEVCNF_ADC3_INTPRI);
	}

	/* Enable ADC */
	ll_devcb[unit].wait_tskid = tk_get_tid();
	out_w(ADC_CR(unit), ADC_CR_ADEN | ADC_CR_ADVREGEN);	// Set ADEN

	err = tk_slp_tsk(DEVCNF_ADC_TMOSCAN);
	if(err < E_OK) err = E_TMOUT;
	ll_devcb[unit].wait_tskid = 0;

	return err;
}

/*----------------------------------------------------------------------
 * A/DC close
 */
LOCAL void adc_close(UW unit)
{
	DisableInt((unit != DEV_ADC_3 )? INTNO_INTADC1_2: INTNO_INTADC3);

	/* Disable ADC */
	out_w(ADC_CR(unit), ADC_CR_ADDIS);		// Set ADDIS
	while(in_w(ADC_ISR(unit)) & ADC_ISR_ADRDY);	// Wait until ADEN is clear
}

/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_adc_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	W	rtn	= (W)E_OK;

	switch(cmd) {
	case LLD_ADC_OPEN:	/* Open　A/DC */
		rtn = (W)adc_open(unit);
		break;

	case LLD_ADC_CLOSE:	/* Close　A/DC */
		adc_close(unit);
		break;
	
	case LLD_ADC_READ:	/* Read A/DC data */
		rtn = adc_convert(unit, p1, p2, pp);
		break;
	
	case LLD_ADC_RSIZE:	/* Get read data size */
		rtn = ADC_CH_NUM - p1;
		if(rtn < 0 ) rtn = 0;
		break;
	}
	
	return rtn;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_adc_llinit( T_ADC_DCB *p_dcb)
{
	static BOOL init_12	= FALSE;

	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= adc_inthdr
	};
	UW	unit;
	ER	err;

	unit = p_dcb->unit;

#if DEVCNF_ADC_INIT_MCLK
	/* Select clock source */
	out_w(RCC_D3CCIPR, (in_w(RCC_D3CCIPR) & ~RCC_D3CCIPR_ADCSEL) | (DEVCNF_ADCSEL));

	/* Enable module clock */
	if(unit < DEV_ADC_3) {		// ADC1_2
		*(_UW*)RCC_AHB1ENR |= RCC_AHB1ENR_ADC12EN;
	} else  {			// ADC3
		*(_UW*)RCC_AHB4ENR |= RCC_AHB4ENR_ADC3EN;
	}

#endif

	/* ADC Power-On */
	out_w(ADC_CR(unit), 0);				// DEEPPWD = 0 
	out_w(ADC_CR(unit), ADC_CR_ADVREGEN);		// ADVREGEN = 1

	_UW	wait_cnt = (TMCLK_KHz / 200UL) + 1UL;
	while(wait_cnt-- != 0);

	/* Common ADC settings */
	if(unit < DEV_ADC_3) {		// ADC1 or ADC2
		if(!init_12) {
			out_w(ADC_CCR(unit), 
				((DEVCNF_ADC12_CKMODE & 0x03)<< 16)	// ADC clock mode
				|((DEVCNF_ADC12_PRESC & 0x0F)<< 18)	// ADC prescaler
			);
		}
	} else {			// ADC3
		out_w(ADC_CCR(unit), 
			((DEVCNF_ADC3_CKMODE & 0x03)<< 16)	// ADC clock mode
			|((DEVCNF_ADC3_PRESC & 0x0F)<< 18)	// ADC prescaler
		);
	}

	/* ADC configuration */
	out_w(ADC_CFGR(unit), ll_devcb[unit].cfgr);
	out_w(ADC_CFGR2(unit), ll_devcb[unit].cfgr2);

	/* Configure boost mode */
	out_w(ADC_CR(unit), (in_w(ADC_CR(unit))&ADC_CR_BOOST) | (ADC_CR_BOOST_VAL<<8));

	/* Interrupt handler definition */
	err = tk_def_int((unit != DEV_ADC_3 )? INTNO_INTADC1_2: INTNO_INTADC3, &dint);

	init_12 = TRUE;
	return err;
}

#endif		/* DEV_ADC_ENABLE */
#endif		/* CPU_STM32H7 */