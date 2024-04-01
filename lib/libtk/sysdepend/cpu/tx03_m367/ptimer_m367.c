/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_TMPM367FDFG
/*
 *	ptimer_m367.c
 *
 *	Physical timer (for TX03M67 TMRB)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#if USE_PTMR

typedef struct {
	UW	baddr;		// Register Base Address
	UINT	mode;		// Timer mode
	UW	limit;		// Counter Limit
	FP	ptmrhdr;	// Timer Hnadler
	PRI	intptr;		// Interrupt priority
	UW	clock;		// Source clock
	void	*exinf;		// Extended information
} T_PTMRCB;

T_PTMRCB ptmrcb[TK_MAX_PTIMER] = {
	{ TMRB0_BASE, -1, 0, (FP)NULL, INTPRI_TMRB0, TB0MOD_TBCLK, 0 },
	{ TMRB1_BASE, -1, 0, (FP)NULL, INTPRI_TMRB1, TB1MOD_TBCLK, 0 },
	{ TMRB2_BASE, -1, 0, (FP)NULL, INTPRI_TMRB2, TB2MOD_TBCLK, 0 },
	{ TMRB3_BASE, -1, 0, (FP)NULL, INTPRI_TMRB3, TB3MOD_TBCLK, 0 },
	{ TMRB4_BASE, -1, 0, (FP)NULL, INTPRI_TMRB4, TB4MOD_TBCLK, 0 },
	{ TMRB5_BASE, -1, 0, (FP)NULL, INTPRI_TMRB5, TB5MOD_TBCLK, 0 },
	{ TMRB6_BASE, -1, 0, (FP)NULL, INTPRI_TMRB6, TB6MOD_TBCLK, 0 },
	{ TMRB7_BASE, -1, 0, (FP)NULL, INTPRI_TMRB7, TB7MOD_TBCLK, 0 }
};

#define TMRB_TBxEN	(p_cb->baddr + TBxEN)
#define TMRB_TBxRUN	(p_cb->baddr + TBxRUN)
#define TMRB_TBxFFCR	(p_cb->baddr + TBxFFCR)
#define	TMRB_TBxMOD	(p_cb->baddr + TBxMOD)
#define	TMRB_TBxRG1	(p_cb->baddr + TBxRG1)
#define	TMRB_TBxIM	(p_cb->baddr + TBxIM)
#define	TMRB_TBxUC	(p_cb->baddr + TBxUC)

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
		/* Stop Physical timer */
		DisableInt( intno);
		out_w( TMRB_TBxEN, TBxEN_TBEN );	// Timer module disabled.
	}
}

LOCAL void ptmr1_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[0]); }
LOCAL void ptmr2_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[1]); }
LOCAL void ptmr3_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[2]); }
LOCAL void ptmr4_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[3]); }
LOCAL void ptmr5_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[4]); }
LOCAL void ptmr6_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[5]); }
LOCAL void ptmr7_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[6]); }
LOCAL void ptmr8_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[7]); }

LOCAL void (* const inthdr_tbl[])() = {
	ptmr1_inthdr, ptmr2_inthdr, ptmr3_inthdr, ptmr4_inthdr, ptmr5_inthdr,
	ptmr6_inthdr, ptmr7_inthdr, ptmr8_inthdr
};

LOCAL const UINT intno_tbl[] = {
	INTNO_TMRB0, INTNO_TMRB1, INTNO_TMRB2, INTNO_TMRB3, INTNO_TMRB4,
	INTNO_TMRB5, INTNO_TMRB6, INTNO_TMRB7
};

/*
 * Start of physical timer operation
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode)
{
	T_DINT		dint;
	T_PTMRCB	*p_cb;
	UINT		intno;
	ER		err;

	/* parameter check */
	if(( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) || ( limit == 0 || limit > PTMR_MAX_CNT )
		|| ( mode > TA_CYC_PTMR ))	return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];
	p_cb->limit	= limit;
	p_cb->mode	= mode;

	/* Timer initialization */
	out_w( TMRB_TBxEN, TBxEN_TBEN );	// Timer module enabled.
	out_w( TMRB_TBxRUN , 0);		// Stop Timer & Counter clear.
	out_w( TMRB_TBxFFCR, TBxFFCR_TDE);	// Trigger disable

	if( limit != PTMR_MAX_CNT ) {
		out_w( TMRB_TBxMOD , TBxMOD_TBCLE | p_cb->clock);	// Set Clock & Up-Counter control.
		out_w( TMRB_TBxRG1 ,  limit + 1);			// Set limit time.
		out_w( TMRB_TBxIM, TBxIM_TBIMOF | TBxIM_TBIM0 );	// Set interrupt mask.
	} else {
		out_w( TMRB_TBxMOD , p_cb->clock);			// Set Clock.
		out_w( TMRB_TBxIM, TBxIM_TBIM1 | TBxIM_TBIM0 );		// Set interrupt mask
	}

	/* Register interrupt handler */
	intno		= intno_tbl[ptmrno - 1];
	dint.intatr	= TA_HLNG;
	dint.inthdr	= inthdr_tbl[ptmrno - 1];
	err = tk_def_int( intno, &dint);
	if(err != E_OK) {
		return err;
	}
	EnableInt( intno, p_cb->intptr);

	/* Start Physical Timer */
	out_w( TMRB_TBxRUN , TBxRUN_TBRUN | TBxRUN_TBPRUN );	// Start Timer.

	return E_OK;
}

EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	T_PTMRCB	*p_cb;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];

	/* Stop Physical Timer */
	DisableInt( intno_tbl[ptmrno - 1]);
	out_w( TMRB_TBxEN, TBxEN_TBEN );	// Timer module disabled.

	return E_OK;
}


IMPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	T_PTMRCB	*p_cb;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];

	/* Get Physical timer counter */
	*p_count = in_w( TMRB_TBxUC );	// Read counter.

	return E_OK;
}


EXPORT ER DefinePhysicalTimerHandler( UINT ptmrno, CONST T_DPTMR *pk_dptmr )
{
	T_PTMRCB	*p_cb;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];

	/* Set user Handler */
	if(pk_dptmr != NULL) {
		p_cb->ptmrhdr	= pk_dptmr->ptmrhdr;
		p_cb->exinf	= pk_dptmr->exinf;
	} else {
		p_cb->ptmrhdr	= NULL;
	}

	return E_OK;
}


EXPORT ER GetPhysicalTimerConfig(UINT ptmrno, T_RPTMR *pk_rptmr)
{
	T_PTMRCB	*p_cb;
	UW		ptmrclk;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];

	switch(p_cb->clock) {
	case 0x00000001:
		ptmrclk = (CLOCK_fc / 4);	// fc/2^2
		break;
	case 0x00000010:
		ptmrclk = (CLOCK_fc / 16);	// fc/2^4
		break;
	case 0x00000011:
		ptmrclk = (CLOCK_fc / 64);	// fc/2^6
		break;
	case 0x00000100:
		ptmrclk = (CLOCK_fc / 128);	// fc/2^7
		break;
	case 0x00000101:
		ptmrclk = (CLOCK_fc / 256);	// fc/2^8
		break;
	case 0x00000110:
		ptmrclk = (CLOCK_fc / 512);	// fc/2^9
		break;
	case 0x00000111:
		ptmrclk = (CLOCK_fc / 1024);	// fc/2^10
		break;
	default:
		ptmrclk = 0;			// Unknown
	}
	pk_rptmr->ptmrclk	= ptmrclk;
	pk_rptmr->maxcount	= PTMR_MAX_CNT;
	pk_rptmr->defhdr	= TRUE;

	return E_OK;
}

#endif	/* USE_PTMR */
#endif	/* CPU_TMPM367FDFG */