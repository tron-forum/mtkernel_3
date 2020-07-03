/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020//.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#ifdef CPU_TMPM369FDFG
#include "../../../config/devconf.h"
#if DEVCNF_DEV_ADC
/*
 *	dev_adc_m367.c
 *	A/D converter device driver
 *	System dependent processing for TX03_M367
 */
#include <tk/tkernel.h>
#include "../../adc.h"

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_ADC_UNITNM] = { ADA_BASE, ADB_BASE };

/*----------------------------------------------------------------------
 * Device control data
*/
LOCAL struct {
	ID	wait_tskid;
} ll_devcb[DEV_ADC_UNITNM];


/*----------------------------------------------------------------------
/* Interrupt handler
 */
void adc_inthdr( UINT intno)
{
	INT		unit;

	unit = (intno == INTNO_INTADA? DEV_ADC_UNIT0: DEV_ADC_UNIT1);
	tk_wup_tsk(ll_devcb[unit].wait_tskid);
}

/*----------------------------------------------------------------------
 * A/D convert
 */
LOCAL UW adc_convert( UINT unit, INT ch, INT size, UW *buf )
{
	UW	addr, rtn;
	ER	err;

	if(ch < 0 || ch >= ADC_CH_NUM) return E_PAR;

	ll_devcb[unit].wait_tskid = tk_get_tid();

	if( size == 1) {
		/* channel fixed single convert */
		out_w( ba[unit] + ADxMOD3, ADMD_CHFIX_SINGLE);	// Ch.Fixed Single mode
		out_w( ba[unit] + ADxMOD2, ch);			// MOD2.ADCH = Channel
		
	} else  if(( ch + size ) <= ADC_CH_NUM ) {
		/* channel scan single convert */
		out_w( ba[unit] + ADxMOD3, ADMD_CHSCAN_SINGLE);	// Ch.Fixed scan mode
		out_w( ba[unit] + ADxMOD4, ch | ((size-1)<<4));	// MOD4.SCANSTA = ch, .SCANAREA = size-1

	} else {
		return (UW)E_PAR;
	}

	tk_can_wup(TSK_SELF);
	out_w( ba[unit] + ADxMOD0, ADxMOD0_ADS);	// MOD0.ADS = 1  Start Covert

	rtn = (UW)tk_slp_tsk(unit?DEVCNF_ADCB_INTPRI:DEVCNF_ADCA_TMOSCAN);
	if(rtn == E_OK) {
		addr = ba[unit] + ADxREG00 + (ch<<2);
		err = size;
		while( size-- > 0) {
			*buf++ = in_w( addr) & 0x0000FFF;
			addr += 4;
		}
		in_w( ba[unit] + ADxMOD5);	// clear MOD5.EOCF
	}

	return rtn;
}


/*----------------------------------------------------------------------
/* Low level device control
 */
EXPORT W dev_adc_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	W	rtn	= (W)E_OK;

	switch(cmd) {
	case LLD_ADC_OPEN:	/* Open　A/DC */
		if(unit == DEV_ADC_UNIT0) {
			out_w(ba[unit] + ADxCLK, DEVCNF_ADCA_CLK);	// Clock setting
			EnableInt(INTNO_INTADA, DEVCNF_ADCA_INTPRI);
		} else {
			out_w(ba[unit] + ADxCLK, DEVCNF_ADCB_CLK);	// Clock setting
			EnableInt(INTNO_INTADB, DEVCNF_ADCB_INTPRI);
		}
		out_w(ba[unit] + ADxMOD1, ADxMOD1_DACON);		// MOD1.DACON = 1
		break;

	case LLD_ADC_CLOSE:	/* Close　A/DC */
		DisableInt((unit?INTNO_INTADB:INTNO_INTADA));
		out_w(ba[unit] + ADxMOD1, 0);		// MOD1.DACON = 0
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
/* Device initialization
 */
EXPORT ER dev_adc_llinit( T_ADC_DCB *p_dcb)
{
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= adc_inthdr,
	};
	ER	err;

	/* Interrupt handler definition */
	err = tk_def_int((p_dcb->unit?INTNO_INTADB:INTNO_INTADA), &dint);

	return err;
}

#endif		/* DEVCNF_DEV_ADC */
#endif		/* CPU_TMPM369FDFG */