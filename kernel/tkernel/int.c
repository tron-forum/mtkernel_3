/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	int.c
 *	Interrupt Control
 */

#include "kernel.h"
#include "check.h"

/* ------------------------------------------------------------------------ */
/*
 * Interrupt handler definition
 */
SYSCALL ER tk_def_int( UINT intno, CONST T_DINT *pk_dint )
{
#if USE_STATIC_IVT
	return E_NOSPT;
#else
	ATR	intatr;
	FP	inthdr;
	ER	ercd;

	CHECK_PAR(intno < N_INTVEC);
	if(pk_dint != NULL) {
		CHECK_RSATR(pk_dint->intatr, TA_HLNG|TA_ASM);
		intatr	= pk_dint->intatr;
		inthdr	= pk_dint->inthdr;
	} else {
		intatr	= 0;
		inthdr	= NULL;
	}

	BEGIN_CRITICAL_SECTION;
	ercd = knl_define_inthdr(intno, intatr, inthdr);
	END_CRITICAL_SECTION;

	return ercd;
#endif
}

/* ------------------------------------------------------------------------ */
/*
 * return Interrupt handler
 */
SYSCALL void tk_ret_int( void )
{
	knl_return_inthdr();
	return;
}

