/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7A

/*
 *	reset_main.c  (ARMv7-A)
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
IMPORT	const void *_HeapStart;

void L1CacheInit(void);

/* ------------------------------------------------------------------------ */
/*
 * Reset Handler Main routine (Called from reset_hdl.S)
 */

EXPORT void reset_main(void)
{
	UW	*src, *top, *end;

	/* Startup Hardware */
	knl_startup_hw();

	/* Load .data to ram */

	src = (UW*)&_data_org;
	top = (UW*)&_data_start;
	end = (UW*)&_data_end;
	while(top < end) {
		*top++ = *src++;
	}

	/* Initialize .bss */
	top = (UW*)&_bss_start;
	end = (UW*)&_bss_end;
	while( top < end) {
		*top++ = 0;
	}

#if USE_IMALLOC
	/* Set System memory area */
	if(INTERNAL_RAM_START > SYSTEMAREA_TOP) {
		knl_lowmem_top = (UW*)INTERNAL_RAM_START;
	} else {
		knl_lowmem_top = (UW*)SYSTEMAREA_TOP;
	}
	if((UW)knl_lowmem_top < (UW)&_HeapStart) {
		knl_lowmem_top = (UW*)&_HeapStart;
	}

	if((SYSTEMAREA_END != 0) && (INTERNAL_RAM_END > SYSTEMAREA_END)) {
		knl_lowmem_limit = (UW*)(SYSTEMAREA_END);
	} else {
		knl_lowmem_limit = (UW*)(INTERNAL_RAM_END);
	}
#endif	/* USE_IMALLOC */

	L1CacheInit();		/* L1 cache initialize */

	/* Startup Kernel */
	main();		/**** No return ****/
	while(1);	/* guard - infinite loops */
}


#endif	/* CPU_CORE_ARMV7A */

