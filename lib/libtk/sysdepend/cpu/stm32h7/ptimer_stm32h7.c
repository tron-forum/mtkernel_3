/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_STM32H7
/*
 *	ptimer_stm32h7.c
 *
 *	Physical timer (for STM32H7)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#if USE_PTMR

typedef struct {
	UW	baddr;		// Register Base Address
	UINT	mode;		// Timer mode
	UW	limit;		// Counter Limit
	FP	ptmrhdr;	// Timer Hnadler
	PRI	intpri;		// Interrupt priority
	UW	psc;		// Prescaler value
	BOOL	tim32;		// 32-bit timer?
	void	*exinf;		// Extended information
} T_PTMRCB;

T_PTMRCB ptmrcb[TK_MAX_PTIMER] = {
	{ TIM2_BASE, -1, 0, (FP)NULL, INTPRI_TIM2, TIM2PSC_PSC_INIT, TRUE,  0 },// No.1
	{ TIM3_BASE, -1, 0, (FP)NULL, INTPRI_TIM3, TIM3PSC_PSC_INIT, FALSE, 0 },// No.2
	{ TIM4_BASE, -1, 0, (FP)NULL, INTPRI_TIM4, TIM4PSC_PSC_INIT, FALSE, 0 },// No.3
	{ TIM5_BASE, -1, 0, (FP)NULL, INTPRI_TIM5, TIM5PSC_PSC_INIT, TRUE,  0 },// No.4
};

#define TIM_CR1(n)	(ptmrcb[n].baddr + TIMxCR1)
#define TIM_DIER(n)	(ptmrcb[n].baddr + TIMxDIER)
#define TIM_SR(n)	(ptmrcb[n].baddr + TIMxSR)
#define TIM_EGR(n)	(ptmrcb[n].baddr + TIMxEGR)
#define TIM_CNT(n)	(ptmrcb[n].baddr + TIMxCNT)
#define TIM_PSC(n)	(ptmrcb[n].baddr + TIMxPSC)
#define TIM_ARR(n)	(ptmrcb[n].baddr + TIMxARR)

/*
 * Physical timer interrupt handler
 */
LOCAL void ptmr_int_main( UINT intno, T_PTMRCB *p_cb)
{
	out_h(p_cb->baddr + TIMxSR, 0);			// Clear interrupt flag
	ClearInt( intno);

	if( p_cb->ptmrhdr != NULL) {
		(*p_cb->ptmrhdr)( p_cb->exinf);		// Execute user handler.
	}

	if( p_cb->mode == TA_ALM_PTMR)  {
		DisableInt( intno);			// Stop Physical timer
	}
}

LOCAL void ptmr1_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[0]); }
LOCAL void ptmr2_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[1]); }
LOCAL void ptmr3_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[2]); }
LOCAL void ptmr4_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[3]); }

LOCAL void (* const inthdr_tbl[])() = {
	ptmr1_inthdr, ptmr2_inthdr, ptmr3_inthdr, ptmr4_inthdr
};

LOCAL const UINT intno_tbl[] = {
	INTNO_TIM2, INTNO_TIM3, INTNO_TIM4, INTNO_TIM5
};

/*
 * Start of physical timer operation
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode)
{
	T_DINT		dint;
	UINT		intno;
	UW		limit_max;
	ER		err;

	/* parameter check */
	if(( ptmrno == 0 || ptmrno > TK_MAX_PTIMER )
		|| ( limit == 0 ) || ( mode > TA_CYC_PTMR ))	return E_PAR;

	ptmrno--;
	if(ptmrcb[ptmrno].tim32) {
		limit_max = PTMR_MAX_CNT32;
	} else {
		limit_max = PTMR_MAX_CNT16;
	}
	if( limit > limit_max) {
		return E_PAR;
	}

	ptmrcb[ptmrno].limit	= limit;
	ptmrcb[ptmrno].mode	= mode;

	/* Timer initialization */
	out_h( TIM_CR1(ptmrno), 0);			// Stop timer.
	out_h( TIM_PSC(ptmrno), ptmrcb[ptmrno].psc);	// Set prescaler.
	out_w( TIM_ARR(ptmrno), limit);			// Set the counter upper limit.

	/* Register interrupt handler */
	intno		= intno_tbl[ptmrno];
	dint.intatr	= TA_HLNG;
	dint.inthdr	= inthdr_tbl[ptmrno];
	err = tk_def_int( intno, &dint);
	if(err != E_OK) {
		return err;
	}
	out_h( TIM_SR(ptmrno), 0);			// Clear Interrupt flag.
	out_h( TIM_DIER(ptmrno), TIMxDIER_UIE);		// Enable Update Interrupt.
	EnableInt( intno, ptmrcb[ptmrno].intpri);

	/* Start Physical Timer */
	out_h( TIM_CR1(ptmrno) , TIMxCR1_CEN | ((mode==TA_ALM_PTMR)?TIMxCR1_OPM:0));	// Start Timer.

	return E_OK;
}

EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrno--;

	/* Stop Physical Timer */
	DisableInt( intno_tbl[ptmrno]);
	out_h( TIM_CR1(ptmrno), 0);		// Stop timer.

	return E_OK;
}


IMPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrno--;

	/* Get Physical timer counter */
	if(ptmrcb[ptmrno].tim32) {	/* 32bit timer */
		*p_count = in_w( TIM_CNT(ptmrno)) ;			// Read counter.
	} else {			/* 16bit timer */
		*p_count = in_w( TIM_CNT(ptmrno)) & PTMR_MAX_CNT16;	// Read counter.
	}

	return E_OK;
}


EXPORT ER DefinePhysicalTimerHandler( UINT ptmrno, CONST T_DPTMR *pk_dptmr )
{
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrno--;

	/* Set user Handler */
	if(pk_dptmr != NULL) {
		ptmrcb[ptmrno].ptmrhdr	= pk_dptmr->ptmrhdr;
		ptmrcb[ptmrno].exinf	= pk_dptmr->exinf;
	} else {
		ptmrcb[ptmrno].ptmrhdr	= NULL;
	}

	return E_OK;
}


EXPORT ER GetPhysicalTimerConfig(UINT ptmrno, T_RPTMR *pk_rptmr)
{
	UW		ptmrclk;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrclk = (PCLK1 / (ptmrcb[--ptmrno].psc + 1));

	pk_rptmr->ptmrclk	= ptmrclk;
	pk_rptmr->maxcount	= (ptmrcb[--ptmrno].tim32)?PTMR_MAX_CNT32:PTMR_MAX_CNT16;
	pk_rptmr->defhdr	= TRUE;

	return E_OK;
}

#endif	/* USE_PTMR */
#endif	/* CPU_STM32H7 */