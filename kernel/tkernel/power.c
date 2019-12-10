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
 *	power.c
 *	power-saving function
 */

#include "kernel.h"
#include "check.h"

/*
 * Number of times for disabling power-saving mode switch
 *	If it is 0, the mode switch is enabled.
 */
EXPORT UINT	knl_lowpow_discnt = 0;

#if TK_SUPPORT_LOWPOWER
/*
 * Set Power-saving mode
 */
SYSCALL ER tk_set_pow( UINT pwmode )
{
	ER	ercd = E_OK;

	CHECK_INTSK();

	BEGIN_CRITICAL_SECTION;

	switch ( pwmode ) {
	  case TPW_DOSUSPEND:
		off_pow();
		break;
	  case TPW_DISLOWPOW:
		if ( knl_lowpow_discnt >= LOWPOW_LIMIT ) {
			ercd = E_QOVR;
		} else {
			knl_lowpow_discnt++;
		}
		break;
	  case TPW_ENALOWPOW:
		if ( knl_lowpow_discnt <= 0 ) {
			ercd = E_OBJ;
		} else {
			knl_lowpow_discnt--;
		}
		break;

	  default:
		ercd = E_PAR;
	}
	END_CRITICAL_SECTION;

	return ercd;
}

#endif	/* TK_SUPPORT_LOWPOWER */



