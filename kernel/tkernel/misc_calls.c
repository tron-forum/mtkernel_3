/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	misc_calls.c
 *	Other System Calls
 */

#include "kernel.h"
#include "check.h"


#ifdef USE_FUNC_TK_REF_SYS
/*
 * Refer system state
 */
SYSCALL ER tk_ref_sys( T_RSYS *pk_rsys )
{
	BOOL	b_qtsk;

	if ( in_indp() ) {
		pk_rsys->sysstat = TSS_INDP;
	} else {
		BEGIN_DISABLE_INTERRUPT;
		b_qtsk = in_qtsk();	
		END_DISABLE_INTERRUPT;
		
		if ( b_qtsk ) {
			pk_rsys->sysstat = TSS_QTSK;
		} else {
			pk_rsys->sysstat = TSS_TSK;
		}
		if ( in_loc() ) {
			pk_rsys->sysstat |= TSS_DINT;
		}
		if ( in_ddsp() ) {
			pk_rsys->sysstat |= TSS_DDSP;
		}
	}
	pk_rsys->runtskid = ( knl_ctxtsk != NULL )? knl_ctxtsk->tskid: 0;
	pk_rsys->schedtskid = ( knl_schedtsk != NULL )? knl_schedtsk->tskid: 0;

	return E_OK;
}
#endif /* USE_FUNC_TK_REF_SYS */

#ifdef USE_FUNC_TK_REF_VER
/*
 * Refer version information
 *	If there is no kernel version information,
 *	set 0 in each information. (Do NOT cause errors.)
 */
SYSCALL ER tk_ref_ver( T_RVER *pk_rver )
{
	pk_rver->maker = (UH)VER_MAKER;	/* OS manufacturer */
	pk_rver->prid  = (UH)VER_PRID;	/* OS identification number */
	pk_rver->spver = (UH)VER_SPVER;	/* Specification version */
	pk_rver->prver = (UH)VER_PRVER;	/* OS product version */
	pk_rver->prno[0] = (UH)VER_PRNO1;	/* Product number */
	pk_rver->prno[1] = (UH)VER_PRNO2;	/* Product number */
	pk_rver->prno[2] = (UH)VER_PRNO3;	/* Product number */
	pk_rver->prno[3] = (UH)VER_PRNO4;	/* Product number */

	return E_OK;
}
#endif /* USE_FUNC_TK_REF_VER */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#ifdef USE_FUNC_TD_REF_SYS
/*
 * Refer system state
 */
SYSCALL ER td_ref_sys( TD_RSYS *pk_rsys )
{
	BOOL	b_qtsk;

	if ( in_indp() ) {
		pk_rsys->sysstat = TSS_INDP;
	} else {
		BEGIN_DISABLE_INTERRUPT;
		b_qtsk = in_qtsk();	
		END_DISABLE_INTERRUPT;
		
		if ( b_qtsk ) {
			pk_rsys->sysstat = TSS_QTSK;
		} else {
			pk_rsys->sysstat = TSS_TSK;
		}
		if ( in_loc() ) {
			pk_rsys->sysstat |= TSS_DINT;
		}
		if ( in_ddsp() ) {
			pk_rsys->sysstat |= TSS_DDSP;
		}
	}
	pk_rsys->runtskid = ( knl_ctxtsk != NULL )? knl_ctxtsk->tskid: 0;
	pk_rsys->schedtskid = ( knl_schedtsk != NULL )? knl_schedtsk->tskid: 0;

	return E_OK;
}
#endif /* USE_FUNC_TD_REF_SYS */

#endif /* USE_DBGSPT */
