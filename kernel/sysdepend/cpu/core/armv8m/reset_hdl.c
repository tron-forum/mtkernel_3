/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV8M

/*
 *	reset_hdr.c (ARMv8-M)
 *	Reset handler
 */
#include <tk/tkernel.h>
#include <kernel.h>
#include "sysdepend.h"

#if !DONT_USE_RESETHDR
/*
 * Memory section address (Defined in the Linker definition file)
*/
IMPORT	const void *__data_org;
IMPORT	const void *__data_start;
IMPORT	const void *__data_end;
IMPORT	const void *__bss_start;
IMPORT	const void *__bss_end;
#if USE_NOINIT
IMPORT	const void *__noinit_end;
#endif

EXPORT void Reset_Handler(void)
{
	UW	*src, *top, *end;
	UW	reg;
	INT	i;

	/* Startup Hardware */
	knl_startup_hw();

	/* Load .data to ram */
	src = (UW*)&__data_org;
	top = (UW*)&__data_start;
	end = (UW*)&__data_end;
	while(top != end) {
		*top++ = *src++;
	}

	/* Initialize .bss */
#if USE_NOINIT
	top = (UW*)&__noinit_end;
#else 
	top = (UW*)&__bss_start;
#endif
	for(i = ((INT)&__bss_end - (INT)top)/sizeof(UW); i > 0 ; i--) {
		*top++ = 0;
	}

#if USE_FPU
	/* Enable FPU */
	*(_UW*)FPU_CPACR |= FPU_CPACR_FPUENABLE;
	*(_UW*)FPU_FPCCR |= (FPU_FPCCR_ASPEN | FPU_FPCCR_LSPEN);
#endif /* USE_FPU */

	knl_start_mtkernel();	/**** No return ****/
	while(1);	/* guard - infinite loops */
}

#endif 	/* !DONT_USE_RESETHDR */
#endif	/* CPU_CORE_ARMV8M */
