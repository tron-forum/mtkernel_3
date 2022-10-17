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
 *	reset_main.c (RXv2)
 *	Reset handler Main routine (Called from reset_hdl.S)
 */

#include "kernel.h"
#include "../../../sysdepend.h"

#include <tm/tmonitor.h>


/* Low level memory manager information */
EXPORT	void	*knl_lowmem_top;		// Head of area (Low address)
EXPORT	void	*knl_lowmem_limit;		// End of area (High address)

IMPORT	const void *_data_org;
IMPORT	const void *_data_start;
IMPORT	const void *_data_end;
IMPORT	const void *_bss_start;
IMPORT	const void *_bss_end;
#if USE_NOINIT
IMPORT	const void *_noinit_end;
#endif

/* ------------------------------------------------------------------------ */
/*
 * Reset Handler Main routine (Called from reset_hdl.S)
 */

EXPORT void reset_main(void)
{
	UW	*src, *top, *end;
	INT	i;

	/* Startup Hardware */
	knl_startup_hw();

#if !USE_STATIC_IVT
	/* Load Interrupt Vector Table from ROM to RAM */
	src = (UW*)knl_int_vect_rom;
	top = (UW*)knl_int_vect_ram;
	for(i=0; i < (N_INTVEC0); i++) {
		*top++ = *src++;
	}
	/* Set Vector Table offset to SRAM */
	knl_set_intb((UW)knl_int_vect_ram);

	/* Load HLL-Interrupt Handler Table from ROM to RAM */
	src = (UW*)knl_hll_inthdr_rom;
	top = (UW*)knl_hll_inthdr_ram;
	for(i=0; i < (N_INTVEC0); i++) {
		*top++ = *src++;
	}
#endif

	/* Load .data to ram */
	src = (UW*)&_data_org;
	top = (UW*)&_data_start;
	end = (UW*)&_data_end;
	while(top != end) {
		*top++ = *src++;
	}

	/* Initialize .bss */
#if USE_NOINIT
	top = (UW*)&_noinit_end;
#else 
	top = (UW*)&_bss_start;
#endif
	for(i = ((INT)&_bss_end - (INT)&_bss_start)/sizeof(UW); i > 0 ; i--) {
		*top++ = 0;
	}

#if USE_IMALLOC
	/* Set System memory area */
	if(INTERNAL_RAM_START > SYSTEMAREA_TOP) {
		knl_lowmem_top = (UW*)INTERNAL_RAM_START;
	} else {
		knl_lowmem_top = (UW*)SYSTEMAREA_TOP;
	}
	if((UW)knl_lowmem_top < (UW)&_bss_end) {
		knl_lowmem_top = (UW*)&_bss_end;
	}

	if((SYSTEMAREA_END != 0) && (INTERNAL_RAM_END > CNF_SYSTEMAREA_END)) {
		knl_lowmem_limit = (UW*)(SYSTEMAREA_END - EXC_STACK_SIZE);
	} else {
		knl_lowmem_limit = (UW*)(INTERNAL_RAM_END - EXC_STACK_SIZE);
	}
#endif

	/* Startup Kernel */
	main();		/**** No return ****/
	while(1);	/* guard - infinite loops */
}

#endif	/* CPU_CORE_RXV2 */