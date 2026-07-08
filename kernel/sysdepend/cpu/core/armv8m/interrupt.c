/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV8M
/*
 *	interrupt.c (ARMv8-M)
 *	Interrupt control
 */

#include <tk/tkernel.h>
#include <kernel.h>
#include "sysdepend.h"
#include "cpu_status.h"

/* HLL Interrupt Handler Table */
LOCAL UW hllint_tbl[N_INTVEC];

/* ------------------------------------------------------------------------ */
/*
 * HLL(High level programming language) Interrupt Handler
 */
EXPORT void knl_hll_inthdr(void)
{
	FP	inthdr;
	UW	intno;

	ENTER_TASK_INDEPENDENT;

	intno	= knl_get_ipsr() - 16;
	inthdr	= (FP)hllint_tbl[intno];

	(*(void(*)(UW))inthdr)(intno);

	LEAVE_TASK_INDEPENDENT;
}

/* ------------------------------------------------------------------------ */
/*
 * System-timer Interrupt handler
 */
EXPORT void knl_systim_inthdr(void)
{
	ENTER_TASK_INDEPENDENT;

	knl_timer_handler();

	LEAVE_TASK_INDEPENDENT;
}

/* ----------------------------------------------------------------------- */
/*
 * Set interrupt handler (Used in tk_def_int())
 */
EXPORT ER knl_define_inthdr( INT intno, ATR intatr, FP inthdr )
{
	volatile FP	*intvet;

	if(inthdr != NULL) {
		if ( (intatr & TA_HLNG) != 0 ) {
			hllint_tbl[intno] = (UW)inthdr;
			inthdr = knl_hll_inthdr;
		}		
	} else 	{	/* Clear interrupt handler */
		inthdr = (FP)knl_exctbl_o[N_SYSVEC + intno];
	}
	intvet = (FP*)(knl_exctbl + N_SYSVEC);
	intvet[intno] = inthdr;

#if USE_CACHE
	if(knl_check_dcache()) {	// Clear D-cache if it is valid
		knl_clean_dcache_adr(&intvet[intno], sizeof(inthdr));
	}
	if(knl_check_icache()) {	// Clear I-cache if it is valid
		knl_dsb();
		knl_invalidate_icache();
		knl_isb();
	}
#endif	/* USE_CACHE */

	return E_OK;
}

/* ----------------------------------------------------------------------- */
/*
 * Return interrupt handler (Used in tk_ret_int())
 */
EXPORT void knl_return_inthdr(void)
{
	/* No processing in ARM. */
	return;
}

/* ------------------------------------------------------------------------ */
/*
 * Interrupt initialize
 */
EXPORT ER knl_init_interrupt( void )
{
	/* Set Exception handler */
	knl_exctbl[2]	= (UW)knl_nmi_handler;		/* 2: NMI Handler */
	knl_exctbl[3]	= (UW)knl_hardfault_handler;	/* 3: Hard Fault Handler */
	knl_exctbl[4]	= (UW)knl_memmanage_handler;	/* 4: MPU Fault Handler */
	knl_exctbl[5]	= (UW)knl_busfault_handler;	/* 5: Bus Fault Handler */
	knl_exctbl[6]	= (UW)knl_usagefault_handler;	/* 6: Usage Fault Handler */

	knl_exctbl[11]	= (UW)knl_svcall_handler;	/* 11: Svcall */
	knl_exctbl[12]	= (UW)knl_debugmon_handler;	/* 12: Debug Monitor Handler */

	knl_exctbl[14]	= (UW)knl_dispatch_entry;	/* 14: Pend SV */
	knl_exctbl[15]	= (UW)knl_systim_inthdr;	/* 15: Systick */

#if USE_CACHE
	if(knl_check_dcache()) {	// Clear D-cache if it is valid
		knl_clean_dcache_adr(&knl_exctbl[2], sizeof(UW)*9);
	}
	if(knl_check_icache()) {	// Clear I-cache if it is valid
		knl_dsb();
		knl_invalidate_icache();
		knl_isb();
	}
#endif	/* USE_CACHE */

	return E_OK;
}

#endif	/* CPU_CORE_ARMV8M */