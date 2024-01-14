/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17 .
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RX231
/*
 *	ptimer_rx231.c
 *
 *	Physical timer (for RX231)
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#if USE_PTMR

typedef struct {
	UW	baddr;		// Register Base Address
	INT	mode;		// Timer mode
	UW	limit;		// Counter Limit
	FP	ptmrhdr;	// Timer Hnadler
	PRI	intptr;		// Interrupt priority
	UINT	intno;		// Interrupt No.
	UW	clock;		// Source clock
	void	*exinf;		// Extended information
} T_PTMRCB;

T_PTMRCB ptmrcb[TK_MAX_PTIMER] = {
	{ TMR01_BASE, -1, 0, (FP)NULL, INTPRI_TMR01, 0, TMR01_CLOCK, 0 },
	{ TMR23_BASE, -1, 0, (FP)NULL, INTPRI_TMR23, 0, TMR23_CLOCK, 0 },
};

#define	TMR_TCR		(p_cb->baddr + TCR)
#define	TMR_TCSR	(p_cb->baddr + TCSR)
#define	TMR_TCOR	(p_cb->baddr + TCORA)
#define	TMR_TCNT	(p_cb->baddr + TCNT)
#define	TMR_TCCR	(p_cb->baddr + TCCR)
#define	TMR_TCSTR	(p_cb->baddr + TCSTR)

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
		out_h(TMR_TCCR, 0);			// Timer Stop
	}
}

LOCAL void ptmr1_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[0]); }
LOCAL void ptmr2_inthdr( UINT intno ) { ptmr_int_main( intno, &ptmrcb[1]); }

LOCAL void (* const inthdr_tbl[])() = {
	ptmr1_inthdr, ptmr2_inthdr
};

#define INTNO_CMIA(no)	(no==1?INTNO_CMIA0:INTNO_CMIA2)
#define INTNO_OVI(no)	(no==1?INTNO_OVI0:INTNO_OVI2)

/*
 * Start of physical timer operation
 */
EXPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode)
{
	T_DINT		dint;
	T_PTMRCB	*p_cb;
	ER		err;

	/* parameter check */
	if(( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) || ( limit == 0 || limit > PTMR_MAX_CNT )
		|| ( mode > TA_CYC_PTMR ))	return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];
	p_cb->limit	= limit;
	p_cb->mode	= mode;

	/* Timer initialization */
	out_h(TMR_TCCR, 0);			// Timer Stop
	out_h(TMR_TCNT, 0);			// Counter Clear

	if( limit != PTMR_MAX_CNT ) {
		out_h(TMR_TCOR, limit);				// Set Compare match register
		out_b(TMR_TCR, TCR_CMIEA | TCR_CCLR_CMA);	// Enable CMIAn
		p_cb->intno = INTNO_CMIA(ptmrno);
	} else {
		out_b(TMR_TCR, TCR_OVIE);			// Enable OVIn
		p_cb->intno = INTNO_OVI(ptmrno);
	}

	/* Register interrupt handler */
	dint.intatr	= TA_HLNG;
	dint.inthdr	= inthdr_tbl[ptmrno - 1];
	err = tk_def_int( p_cb->intno, &dint);
	if(err != E_OK) {
		return err;
	}
	EnableInt( p_cb->intno, p_cb->intptr);

	/* Start Physical Timer */
	out_h(TMR_TCCR, TCCR_CSS_16BIT | (p_cb->clock)<<8);	// 16bit mode & Clock set

	return E_OK;
}

EXPORT ER StopPhysicalTimer( UINT ptmrno )
{
	T_PTMRCB	*p_cb;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];

	/* Stop Physical Timer */
	DisableInt( p_cb->intno);
	out_h(TMR_TCCR, 0);			// Timer Stop

	return E_OK;
}


IMPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count )
{
	T_PTMRCB	*p_cb;

	/* parameter check */
	if( ptmrno == 0 || ptmrno > TK_MAX_PTIMER ) return E_PAR;

	p_cb = &ptmrcb[ptmrno - 1];
	if(p_cb->mode < 0) return E_PAR;

	/* Get Physical timer counter */
	*p_count = (UW)in_h(TMR_TCNT);	// Read counter.

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
		case 0x08:
			ptmrclk = SYSCLK_PCLKB;
			break;
		case 0x09:
			ptmrclk = SYSCLK_PCLKB/2;
			break;
		case 0x0A:
			ptmrclk = SYSCLK_PCLKB/8;
			break;
		case 0x0B:
			ptmrclk = SYSCLK_PCLKB/32;
			break;
		case 0x0C:
			ptmrclk = SYSCLK_PCLKB/64;
			break;
		case 0x0D:
			ptmrclk = SYSCLK_PCLKB/1024;
			break;
		case 0x0E:
			ptmrclk = SYSCLK_PCLKB/8192;
			break;
		default:
			ptmrclk = 0;		// Unknown
	}
	pk_rptmr->ptmrclk	= ptmrclk;
	pk_rptmr->maxcount	= PTMR_MAX_CNT;
	pk_rptmr->defhdr	= TRUE;

	return E_OK;
}

#endif	/* USE_PTMR */
#endif	/* CPU_RX231 */
