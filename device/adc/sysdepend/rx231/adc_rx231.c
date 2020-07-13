/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/07/13.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#ifdef CPU_RX231
#include "../../../config/devconf.h"
#if DEVCNF_DEV_ADC
/*
 *	dev_adc_rx231.c
 *	A/D converter device driver
 *	System dependent processing for RX231
 */
#include <tk/tkernel.h>
#include "../../adc.h"

/*----------------------------------------------------------------------
 * Device control data
*/
LOCAL struct {
	ID	wait_tskid;
} ll_devcb;


/*----------------------------------------------------------------------
/* Interrupt handler
 */
void adc_inthdr( UINT intno)
{
	INT		unit;

	ClearInt(intno);
	tk_wup_tsk(ll_devcb.wait_tskid);
}

/*----------------------------------------------------------------------
 * A/D convert
 */
LOCAL UW adc_convert( INT start, INT size, UW *buf )
{
	INT	ch, end;
	UH	data;
	ER	err;

	ll_devcb.wait_tskid = tk_get_tid();
	tk_can_wup(TSK_SELF);

	if((start > 7 && start <16) || start > 31) return E_PAR;
	if( start >= 16) start -= 8;
	end = start + size -1;
	if( end > 31) return (UW)E_PAR;

	/* Set target channel */
	for(ch = start; ch <= end; ch++) {
		if(ch <= 7) {
			*(UH*)(ADANSA0) |= (1<<ch);
		} else {
			*(UH*)(ADANSA1) |= (1<<(ch-8));
		}
	}

	out_h(ADCSR, ADCSR_ADST | ADCSR_ADIE);	// Start Covert

	err = tk_slp_tsk(DEVCNF_ADC_TMOSCAN);
	if(err != E_OK) return (UW)err;

	for(ch = start; ch <= end; ch++) {	/* Read data */
		if(ch <= 7) {
			*buf++ = (UW)in_h(ADDR(ch));
		} else {
			*buf++ = (UW)in_h(ADDR(ch+8));
		}
	}
	return (UW)size;
}


/*----------------------------------------------------------------------
/* Low level device control
 */
EXPORT W dev_adc_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	UW	rtn = (UW)E_OK;

	switch(cmd) {
	case LLD_ADC_OPEN:	/* Open　A/DC */
		out_h(ADANSA0,0);
		out_h(ADANSA1,0);
		EnableInt(INTNO_S10ADI0, DEVCNF_ADC_INTPRI);
		break;

	case LLD_ADC_CLOSE:	/* Close　A/DC */
		DisableInt(INTNO_S10ADI0);
		break;
	
	case LLD_ADC_READ:	/* Read A/DC data */
		rtn = adc_convert(p1, p2, pp);
		break;
	
	case LLD_ADC_RSIZE:	/* Get read data size */
		if((p1 > 7 && p1 <16) || p1 > 31) return (UW)E_PAR;
		rtn = ADC_CH_NUM - (p1<=7?p1:p1-8);
		if(rtn < 0 ) rtn = 0;
		break;

	}
	
	return rtn;
}

/*----------------------------------------------------------------------
/* Device initialization
 */
EXPORT ER dev_adc_llinit( T_ADC_DCB *p_dcb)
{
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= adc_inthdr,
	};
	ER	err;

#if DEVCONF_ADC_INIT_MSTP		// Initialize module stop
	UINT	sts;

	if(in_w(MSTPCRA) & (1<<17)) {
		DI(sts);
		out_h(SYSTEM_PRCR, 0xA502);	/* Disable Register Protect */
		*(UW*)(MSTPCRA) &= ~(1<<17);	/* Release module stop */
		out_h(SYSTEM_PRCR, 0xA500);	/* Enable Register protect */
		EI(sts);
	}
#endif	/* DEVCONF_ADC_INIT_MSTP */

#if DEVCOBF_ADC_INIT_PIN		// Initialize I/O pin
	INT	i;
	UB	cnf;

	out_b(MPC_PWPR, 0);				// PWPR.B0WI = 0
	out_b(MPC_PWPR, MPC_PWMR_PFSWE);		// PWPR.PFSWE = 1

	/* P40-P47 = AN00-AN07 */
	cnf = DEVCONF_ENA_AN00_07;
	*(UB*)PORTn_PDR(4) &= ~cnf;		// Set input port
	*(UB*)PORTn_PMR(4) &= ~cnf;		// Set General-purpose i/o port
	for( i = 0; i < 8; i++, cnf<<=1) {
		if(cnf & 1) {
			out_b(MPC_P4nPFS(i), MPC_PFS_ASEL);
		}
	}

	/* PE0-PE7 = AN16-AN23 */
	cnf = DEVCONF_ENA_AN16_23;
	*(UB*)PORTE_PDR &= ~cnf;		// Set input port
	*(UB*)PORTE_PMR &= ~cnf;		// Set General-purpose i/o port
	for( i = 0; i < 8; i++, cnf<<=1) {
		if(cnf & 1) {
			out_b(MPC_PEnPFS(i), MPC_PFS_ASEL);
		}
	}

	/* PD0-PD7 = AN24-AN31 */
	cnf = DEVCONF_ENA_AN24_31;
	*(UB*)PORTD_PDR &= ~cnf;		// Set input port
	*(UB*)PORTD_PMR &= ~cnf;		// Set General-purpose i/o port
	for( i = 0; i < 8; i++, cnf<<=1) {
		if(cnf & 1) {
			out_b(MPC_PDnPFS(i), MPC_PFS_ASEL);
		}
	}

	out_b(MPC_PWPR, MPC_PWMR_B0WI);			// PWPR.PFSWE = 0, PWPR.B0WI = 1
#endif		/* DEVCOBF_ADC_INIT_PIN */

	/* ADC device initialize */
	out_h(ADCER, ADCER_INI);

	out_h(ADSSTR(0), DEVCNF_ADSSTR0_INI);	// Set sampling rate
	out_h(ADSSTR(1), DEVCNF_ADSSTR1_INI);
	out_h(ADSSTR(2), DEVCNF_ADSSTR2_INI);
	out_h(ADSSTR(3), DEVCNF_ADSSTR3_INI);
	out_h(ADSSTR(4), DEVCNF_ADSSTR4_INI);
	out_h(ADSSTR(5), DEVCNF_ADSSTR5_INI);
	out_h(ADSSTR(6), DEVCNF_ADSSTR6_INI);
	out_h(ADSSTR(7), DEVCNF_ADSSTR7_INI);
	out_h(ADSSTRL, DEVCNF_ADSSTRL_INI);

	/* Interrupt handler definition */
	err = tk_def_int(INTNO_S10ADI0, &dint);

	return err;
}

#endif		/* DEVCNF_DEV_ADC */
#endif		/* CPU_RX231 */