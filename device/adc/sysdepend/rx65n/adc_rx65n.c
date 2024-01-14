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

#include <sys/machine.h>
#ifdef CPU_RX65N

#include <tk/tkernel.h>
#include "../../adc.h"
#include "../../../include/dev_def.h"
#if DEV_ADC_ENABLE
/*
 *	dev_adc_rx65n.c
 *	A/D converter device driver
 *	System dependent processing for RX65N
 */

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_ADC_UNITNM] = { ADC0_BASE, ADC1_BASE };

#define	ADC_ADCSR(x)	(ba[x] + ADCx_ADCSR)		// A/DC control reg
#define	ADC_ADANSA0(x)	(ba[x] + ADCx_ADANSA0)		// A/DC channel select reg. A0
#define	ADC_ADANSA1(x)	(ba[x] + ADCx_ADANSA1)		// A/DC channel select reg. A1
#define	ADC_ADCER(x)	(ba[x] + ADCx_ADCER)		// A/DC Control extension reg
#define	ADC_ADANSB0(x)	(ba[x] + ADCx_ADANSB0)		// A/DC channel select reg. B0
#define	ADC_ADANSB1(x)	(ba[x] + ADCx_ADANSB1)		// A/DC channel select reg. B1

#define	ADC_ADDR(x,c)	(ba[x] + ADCx_ADDR(c))		//A/DC Data reg
#define	ADC_ADSSTR(x,c)	(ba[x] + ADCx_ADSSTR(c))	//A/DC Sampling state reg

/*----------------------------------------------------------------------
 * Device control data
*/
LOCAL struct {
	ID	wait_tskid;
	UINT	intno;
} ll_devcb[DEV_ADC_UNITNM];


/*----------------------------------------------------------------------
 * Interrupt handler
 */
void adc_inthdr( UINT intno)
{
	UW	unit;

	ClearInt(intno);
	if(intno == INTNO_S12ADI0) unit = 0;
	else if(intno == INTNO_S12ADI1) unit = 1;
	else return ;

	if(ll_devcb[unit].wait_tskid) {
		tk_wup_tsk(ll_devcb[unit].wait_tskid);
	}
}

/*----------------------------------------------------------------------
 * A/D convert
 */
LOCAL UW adc_convert( UW unit, INT start, INT size, UW *buf )
{
	INT	ch, end;
	ER	err;

	ll_devcb[unit].wait_tskid = tk_get_tid();
	tk_can_wup(TSK_SELF);

	if(unit == 0) {		// S12AD
		if(start >= ADC_CH_NUM0) return (UW)E_PAR;
		end = start + size -1;
		if(end >= ADC_CH_NUM0) return (UW)E_PAR;

		for(ch = start; ch <= end; ch++) {
			*(UH*)(ADC_ADANSA0(0)) |= (1<<ch);
		}
	} else {		// S12AD1
		if(start >= ADC_CH_NUM1) return (UW)E_PAR;
		end = start + size -1;
		if(end >= ADC_CH_NUM1) return (UW)E_PAR;

		for(ch = start; ch <= end; ch++) {
			if(ch <= 7) {
				*(UH*)(ADC_ADANSA0(1)) |= (1<<ch);
			} else {
				*(UH*)(ADC_ADANSA1(1)) |= (1<<(ch-8));
			}
		}
	}

	out_h(ADC_ADCSR(unit), ADCSR_ADST | ADCSR_ADIE);	// Start Convert

	err = tk_slp_tsk(DEVCNF_ADC_TMOSCAN);
	if(err != E_OK) return (UW)err;

	for(ch = start; ch <= end; ch++) {	/* Read data */
		*buf++ = (UW)in_h(ADC_ADDR(unit, ch));
	}
	return (UW)size;
}


/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_adc_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	UW	rtn = (UW)E_OK;

	switch(cmd) {
	case LLD_ADC_OPEN:	/* Open A/DC */
		out_h(ADC_ADANSA0(unit), 0);
		if(unit == 1) {
			out_h(ADC_ADANSA1(unit), 0);
		}
		EnableInt(ll_devcb[unit].intno, unit?DEVCNF_ADC1_INTPRI:DEVCNF_ADC0_INTPRI);
		break;

	case LLD_ADC_CLOSE:	/* Close　A/DC */
		DisableInt(ll_devcb[unit].intno);
		break;
	
	case LLD_ADC_READ:	/* Read A/DC data */
		rtn = adc_convert(unit, p1, p2, pp);
		break;
	
	case LLD_ADC_RSIZE:	/* Get read data size */
		if((p1 > 7 && p1 <16) || p1 > 31) return (UW)E_PAR;
		rtn = (unit?ADC_CH_NUM1:ADC_CH_NUM0) - (p1<=7?p1:p1-8);
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
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= adc_inthdr,
	};
	UW	unit;
	UINT	intno;
	ER	err;

	unit = p_dcb->unit;

#if DEVCONF_ADC_INIT_MSTP		// Initialize module stop
	UINT	sts;
	UW	mstpa;

	mstpa = (1<<19) | (1<<((unit)?16:17));
	DI(sts);
	out_h(SYSTEM_PRCR, 0xA502);	/* Disable Register Protect */
	*(UW*)(MSTPCRA) &= ~mstpa;	/* Release module stop */
	out_h(SYSTEM_PRCR, 0xA500);	/* Enable Register protect */
	EI(sts);
#endif	/* DEVCONF_ADC_INIT_MSTP */

	/* ADC device initialize */
	out_h(ADC_ADCER(unit), ADCER_INI);


	/* Set sampling rate */
	if(unit == 0) {
		out_h(ADC_ADSSTR(0,0), DEVCNF_ADC0_ADSSTR0_INI);
		out_h(ADC_ADSSTR(0,1), DEVCNF_ADC0_ADSSTR1_INI);
		out_h(ADC_ADSSTR(0,2), DEVCNF_ADC0_ADSSTR2_INI);
		out_h(ADC_ADSSTR(0,3), DEVCNF_ADC0_ADSSTR3_INI);
		out_h(ADC_ADSSTR(0,4), DEVCNF_ADC0_ADSSTR4_INI);
		out_h(ADC_ADSSTR(0,5), DEVCNF_ADC0_ADSSTR5_INI);
		out_h(ADC_ADSSTR(0,6), DEVCNF_ADC0_ADSSTR6_INI);
		out_h(ADC_ADSSTR(0,7), DEVCNF_ADC0_ADSSTR7_INI);
	} else {
		out_h(ADC_ADSSTR(1,0), DEVCNF_ADC1_ADSSTR0_INI);
		out_h(ADC_ADSSTR(1,1), DEVCNF_ADC1_ADSSTR1_INI);
		out_h(ADC_ADSSTR(1,2), DEVCNF_ADC1_ADSSTR2_INI);
		out_h(ADC_ADSSTR(1,3), DEVCNF_ADC1_ADSSTR3_INI);
		out_h(ADC_ADSSTR(1,4), DEVCNF_ADC1_ADSSTR4_INI);
		out_h(ADC_ADSSTR(1,5), DEVCNF_ADC1_ADSSTR5_INI);
		out_h(ADC_ADSSTR(1,6), DEVCNF_ADC1_ADSSTR6_INI);
		out_h(ADC_ADSSTR(1,7), DEVCNF_ADC1_ADSSTR7_INI);
		out_h(ADC_ADSSTR(1,8), DEVCNF_ADC1_ADSSTR0_INI);
		out_h(ADC_ADSSTR(1,9), DEVCNF_ADC1_ADSSTR1_INI);
		out_h(ADC_ADSSTR(1,10), DEVCNF_ADC1_ADSSTR2_INI);
		out_h(ADC_ADSSTR(1,11), DEVCNF_ADC1_ADSSTR3_INI);
		out_h(ADC_ADSSTR(1,12), DEVCNF_ADC1_ADSSTR4_INI);
		out_h(ADC_ADSSTR(1,13), DEVCNF_ADC1_ADSSTR5_INI);
		out_h(ADC_ADSSTR(1,14), DEVCNF_ADC1_ADSSTR6_INI);
		out_h(ADC_ADSSTR(1,15), DEVCNF_ADC1_ADSSTR7_INI);
		out_h(ADC_ADSSTRL, DEVCNF_ADC1_ADSSTRL_INI);
		out_h(ADC_ADSSTRT, DEVCNF_ADC1_ADSSTRT_INI);
		out_h(ADC_ADSSTRO, DEVCNF_ADC1_ADSSTRO_INI);
	}

	intno = unit?INTNO_S12ADI1:INTNO_S12ADI0;
	ll_devcb[unit].intno = intno;

#if DEVCONF_ADC_SET_PERI	// Set Software Configurable Interrupt
	err = SetPERI(intno, unit?INTFN_S12ADI1:INTFN_S12ADI0);
	if(err < E_OK) return E_SYS;
#endif /* DEVCONF_ADC_SET_PERI */


	/* Interrupt handler definition */
	err = tk_def_int(intno, &dint);

	return err;
}

#endif		/* DEV_ADC_ENABLE */
#endif		/* CPU_RX65N */