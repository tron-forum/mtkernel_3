/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03/13.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#ifdef CPU_CORE_ACM3

/*
 *	reset_hdr.c (ARM Cortex-M3)
 *	Reset handler
 */

#include "kernel.h"
#include "../../../sysdepend.h"

#include <tm/tmonitor.h>


/* Low level memory manager information */
EXPORT	void	*knl_lowmem_top;		// Head of area (Low address)
EXPORT	void	*knl_lowmem_limit;		// End of area (High address)

IMPORT	const void *__data_org;
IMPORT	const void *__data_start;
IMPORT	const void *__data_end;
IMPORT	const void *__bss_start;
IMPORT	const void *__bss_end;
#if USE_NOINIT
IMPORT	const void *__noinit_end;
#endif

IMPORT const void (*vector_tbl[])();

EXPORT void Reset_Handler(void)
{
	UW	*src, *top, *end;
	UW	reg;
	INT	i;

	/* Startup Hardware */
	knl_startup_hw();

#if !USE_STATIC_IVT
	/* Load Vector Table from ROM to RAM */
	src = (UW*)vector_tbl;;
	top = (UW*)exchdr_tbl;

	for(i=0; i < ( (SYS_VECTOR_SIZE+INT_VECTOR_SIZE)/sizeof(UW) ); i++) {
		*top++ = *src++;
	}
	
	/* Set Vector Table offset to SRAM */
	*(_UW*)SCB_VTOR = (UW)exchdr_tbl;
#endif

	/* Load .data to ram */
	src = (UW*)&__data_org;;
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
	for(i = (INT)&__bss_end - (INT)&__bss_start; i > 0 ; i--) {
		*top++ = 0;
	}

#if USE_IMALLOC
	/* Set System memory area */
	if(INTERNAL_RAM_START > SYSTEMAREA_TOP) {
		knl_lowmem_top = (UW*)INTERNAL_RAM_START;
	} else {
		knl_lowmem_top = (UW*)SYSTEMAREA_TOP;
	}
	if((UW)knl_lowmem_top < (UW)&__bss_end) {
		knl_lowmem_top = (UW*)&__bss_end;
	}

	if((SYSTEMAREA_END != 0) && (INTERNAL_RAM_END > CNF_SYSTEMAREA_END)) {
		knl_lowmem_limit = (UW*)(SYSTEMAREA_END - EXC_STACK_SIZE);
	} else {
		knl_lowmem_limit = (UW*)(INTERNAL_RAM_END - EXC_STACK_SIZE);
	}
#endif

	/* Configure exception priorities */
	reg = *(_UW*)SCB_AIRCR;
	reg = (reg & (~AIRCR_PRIGROUP7)) | AIRCR_PRIGROUP3;	// PRIGRP:SUBPRI = 4 : 4
	*(_UW*)SCB_AIRCR = (reg & 0x0000FFFF) | AIRCR_VECTKEY;
	
	*(_UW*)SCB_SHPR2 = SCB_SHPR2_VAL;		// SVC pri = 0
	*(_UW*)SCB_SHPR3 = SCB_SHPR3_VAL;		// SysTick = 1 , PendSV = 7

	/* Allow user to access SCB_STIR */
	*(_UW*)SCB_CCR |= CCR_USERSETMPEND;

	/* Startup Kernel */
	main();		/**** No return ****/
	while(1);	/* guard - infinite loops */
}

#endif	/* CPU_CORE_ACM3 */