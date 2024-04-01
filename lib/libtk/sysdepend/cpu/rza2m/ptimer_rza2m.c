/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RZA2M
/*
 *	ptimer_rza2m.c
 *
 *	Physical timer (for RZ/A2M)
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

	void	*exinf;		// Extended information
} T_PTMRCB;

T_PTMRCB ptmrcb[TK_MAX_PTIMER] = {
	{ OSTM1_BASE, -1, 0, (FP)NULL, INTPRI_OSTM1, 0 },	// ptimer No.1
	{ OSTM2_BASE, -1, 0, (FP)NULL, INTPRI_OSTM2, 0 },	// ptimer No.2
};

#define OSTMn_CMP(n)	(ptmrcb[n].baddr + OSTMnCMP)
#define OSTMn_CNT(n)	(ptmrcb[n].baddr + OSTMnCNT)
#define OSTMn_TE(n)	(ptmrcb[n].baddr + OSTMnTE)
#define OSTMn_TS(n)	(ptmrcb[n].baddr + OSTMnTS)
#define OSTMn_TT(n)	(ptmrcb[n].baddr + OSTMnTT)
#define OSTMn_CTL(n)	(ptmrcb[n].baddr + OSTMnCTL)

/*
 * Physical timer interrupt handler
 */
LOCAL void ptmr_int_main( UINT intno, T_PTMRCB *p_cb)
{
	ClearInt( intno);

	if( p_cb->ptmrhdr != NULL) {
		(*p_cb->ptmrhdr)( p_cb->exinf);		// Execute user handler.
	}
	
	if( p_cb->mode == TA_ALM_PTMR)  {
		DisableInt( intno);
		out_b( OSTMn_TT((intno==INTNO_OSTM1)?0:1), 1);	// Stop Physical timer
	}

	EndOfInt(intno);
}

LOCAL void ptmr1_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[0]); }
LOCAL void ptmr2_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[1]); }

LOCAL void (* const inthdr_tbl[])() = {
	ptmr1_inthdr, ptmr2_inthdr
};

LOCAL const UINT intno_tbl[] = {
	INTNO_OSTM1, INTNO_OSTM2
};

/*
 * Start of physical timer operation
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode)
{
	T_DINT		dint;
	UINT		intno;
	ER		err;

	/* parameter check */
	if(( ptmrno == 0 || ptmrno > TK_MAX_PTIMER )
		|| ( limit == 0 ) || ( mode > TA_CYC_PTMR ))	return E_PAR;

	ptmrno--;
	ptmrcb[ptmrno].limit	= limit;
	ptmrcb[ptmrno].mode	= mode;

	/* Timer initialization */
	out_b( OSTMn_TT(ptmrno), 1);		// Stop timer.
	out_w( OSTMn_CMP(ptmrno), limit);	// Set the counter upper limit.

	/* Register interrupt handler */
	intno		= intno_tbl[ptmrno];
	dint.intatr	= TA_HLNG;
	dint.inthdr	= inthdr_tbl[ptmrno];
	err = tk_def_int( intno, &dint);
	if(err != E_OK) {
		return err;
	}
	EnableInt( intno, ptmrcb[ptmrno].intpri);

	/* Set interval timer mode */
	out_b(OSTMn_CTL(ptmrno), 0);
	
	/* Start Physical Timer */
	out_b(OSTMn_TS(ptmrno), 1);

	return E_OK;
}

EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrno--;

	/* Stop Physical Timer */
	DisableInt( intno_tbl[ptmrno]);
	out_b( OSTMn_TT(ptmrno), 1);		// Stop timer.

	return E_OK;
}


IMPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	ptmrno--;

	/* Get Physical timer counter */
	*p_count = ptmrcb[ptmrno].limit - in_w( OSTMn_CNT(ptmrno)) ;	// down counter.

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
	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	pk_rptmr->ptmrclk	= PTMCLK;
	pk_rptmr->maxcount	= PTMR_MAX_CNT;
	pk_rptmr->defhdr	= TRUE;

	return E_OK;
}

#endif	/* USE_PTMR */
#endif	/* CPU_RZA2M */