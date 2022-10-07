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
#ifdef CPU_CORE_RXV2
/*
 *	interrupt.c (RXv2)
 *	Interrupt control
 */

#include "kernel.h"
#include "../../../sysdepend.h"

/* ------------------------------------------------------------------------ */
/*
 * Interrupt nest counter
 */
EXPORT	W	knl_int_nest = 0;	/* Interrupt nest counter */


#if !USE_STATIC_IVT
/* ------------------------------------------------------------------------ */
/*
 * Interrupt Vector table (RAM)
 */

EXPORT FP knl_int_vect_ram[N_INTVEC0] __attribute__ ((section (".data_ivector")));

/* ------------------------------------------------------------------------ */
/*
 * HLL(High level programming language) Interrupt Handler Table (RAM)
 */
EXPORT FP knl_hll_inthdr_ram[N_INTVEC0] __attribute__ ((section (".data_hvector")));

#endif	/* !USE_STATIC_IVT */

/* ----------------------------------------------------------------------- */
/*
 * Set interrupt handler (Used in tk_def_int())
 */
#if !USE_STATIC_IVT
EXPORT ER knl_define_inthdr( INT intno, ATR intatr, FP inthdr )
{
#if USE_GROUP_INT	// Group Interrupt
	if(intno >= INTNO_GROUP_TOP) {
		if( (intatr & TA_HLNG) != 0 ) {
			return knl_define_gint( intno, inthdr);
		} else {
			return E_PAR;
		}
	}
#endif /* USE_GROUP_INT */

	/* check system interrupt number */
	if((intno < INTNO_USER_MIN) || (intno == INTNO_SYS_TICK)) {
		return E_PAR;
	}

	if(inthdr != NULL) {	/* define intrrupt handler */
		if ( (intatr & TA_HLNG) != 0 ) {
			knl_hll_inthdr_ram[intno] = inthdr;
			inthdr = knl_int_vect_rom[intno];	/* set HLL-Int handler entry */
		}
	} else 	{		/* Clear interrupt handler */
		knl_hll_inthdr_ram[intno] = knl_hll_inthdr_rom[intno - INTNO_USER_MIN];
		inthdr = knl_int_vect_rom[intno];	/* set HLL-Int handler entry */
	}

	knl_int_vect_ram[intno] = inthdr;

	return E_OK;
}
#endif

/* ------------------------------------------------------------------------ */
/*
 * Interrupt initialize
 */
EXPORT ER knl_init_interrupt( void )
{
	/* Register exception handler used on OS			*/
	/* There is no exception handler used in this implementation.	*/

	return E_OK;
}



#endif	/* CPU_CORE_RXV2 */