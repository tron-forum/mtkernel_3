/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08.B1
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/03.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV8M

/*
 *	cache.c (ARMv8-M)
 *	Cache control
 *	 *** These functions must be executed with interrupts disabled.
 */
#include <tk/tkernel.h>
#include <kernel.h>
#include "sysdepend.h"
#include "cpu_status.h"

#if USE_CACHE && CPU_HAS_CACHE

#define MEMORY_BARRIER	knl_dsb();knl_isb();

/* ------------------------------------------------------------------------ */
/*
 * Enable I-Cache
 */
EXPORT void knl_enable_icache(void)
{
	UW	ccr = in_w(SCB_CCR);

	if(ccr & CCR_IC) return;	// Return if I-Cache is already enabled

	MEMORY_BARRIER
	out_w(SCB_ICIALLU, 0);		// Invalidate I-Cache
	MEMORY_BARRIER
	out_w(SCB_CCR, ccr | CCR_IC);	// Enable I-Cache
	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Disable I-Cache
 */
EXPORT void knl_disable_icache(void)
{
	UW	ccr = in_w(SCB_CCR);

	MEMORY_BARRIER
	out_w(SCB_CCR, ccr & ~ CCR_IC);	// Disable I-Cache
	out_w(SCB_ICIALLU, 0);		// Invalidate I-Cache
	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Invalidates I-Cache
 */
EXPORT void knl_invalidate_icache(void)
{
	MEMORY_BARRIER
	out_w(SCB_ICIALLU, 0);		// Invalidate I-Cache
	MEMORY_BARRIER
}


/* ------------------------------------------------------------------------ */
/*
 * Enable D-Cache
 */
EXPORT void knl_enable_dcache(void)
{
	UW	ccsidr, sets, ways;
	UW	ccr = in_w(SCB_CCR);

	if(ccr & CCR_DC) return;	// Return if D-Cache is already enabled

	out_w(SCB_CSSELR, 0);		// Select Level 1 data cache
	knl_dsb();

	/* Invalidate D-Cache */
	ccsidr = in_w(SCB_CCSIDR);
	sets = CCSIDR_VAL_SETS(ccsidr);
	do {
		ways = CCSIDR_VAL_WAYS(ccsidr);
		do {
			out_w(SCB_DCISW, SCB_VAL_SET(sets) | SCB_VAL_WAY(ways)); 
		} while(ways-- != 0);
	} while(sets-- != 0);

	knl_dsb();
	out_w(SCB_CCR, ccr | CCR_DC);
	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Disable D-Cache
 */
EXPORT void knl_disable_dcache(void)
{
	struct {
		UW ccsidr;
		UW sets;
		UW ways;
	} locals __attribute__ ((aligned(SCB_DCACHE_LINE_SIZE)));

	out_w(SCB_CSSELR, 0);		// Select Level 1 data cache
	knl_dsb();

	out_w(SCB_CCR, in_w(SCB_CCR) & ~CCR_DC);  // disable D-Cache
	knl_dsb();

	out_w(SCB_DCCIMVAC, (UW)&locals);
	MEMORY_BARRIER

	/* Clean & invalidate D-Cache */
	locals.ccsidr = in_w(SCB_CCSIDR);
	locals.sets = CCSIDR_VAL_SETS(locals.ccsidr);
	do {
		locals.ways = CCSIDR_VAL_WAYS(locals.ccsidr);
		do {
			out_w(SCB_DCCISW, SCB_VAL_SET(locals.sets) | SCB_VAL_WAY(locals.ways));
		} while(locals.ways-- != 0);
	} while(locals.sets-- != 0);

	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Invalidate D-Cache
 */
EXPORT void knl_invalidate_dcache(void)
{
	UW	ccsidr, sets, ways;

	out_w(SCB_CSSELR, 0);		// Select Level 1 data cache
	knl_dsb();

	/* Invalidate D-Cache */
	ccsidr = in_w(SCB_CCSIDR);
	sets = CCSIDR_VAL_SETS(ccsidr);
	do {
		ways = CCSIDR_VAL_WAYS(ccsidr);
		do {
			out_w(SCB_DCISW, SCB_VAL_SET(sets) | SCB_VAL_WAY(ways)); 
		} while(ways-- != 0);
	} while(sets-- != 0);

	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Clean D-Cache
 */
EXPORT void knl_clean_dcache(void)
{
	UW	ccsidr, sets, ways;

	out_w(SCB_CSSELR, 0);		// Select Level 1 data cache
	knl_dsb();

	/* Clean & Invalidate D-Cache */
	ccsidr = in_w(SCB_CCSIDR);
	sets = CCSIDR_VAL_SETS(ccsidr);
	do {
		ways = CCSIDR_VAL_WAYS(ccsidr);
		do {
			out_w(SCB_DCCSW, SCB_VAL_SET(sets) | SCB_VAL_WAY(ways)); 
		} while(ways-- != 0);
	} while(sets-- != 0);

	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Clean & Invalidate D-Cache
 */
EXPORT void knl_clean_inval_dcache(void)
{
	UW	ccsidr, sets, ways;

	out_w(SCB_CSSELR, 0);		// Select Level 1 data cache
	knl_dsb();

	/* Clean & Invalidate D-Cache */
	ccsidr = in_w(SCB_CCSIDR);
	sets = CCSIDR_VAL_SETS(ccsidr);
	do {
		ways = CCSIDR_VAL_WAYS(ccsidr);
		do {
			out_w(SCB_DCCISW, SCB_VAL_SET(sets) | SCB_VAL_WAY(ways)); 
		} while(ways-- != 0);
	} while(sets-- != 0);

	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Invalidate D-Cache by Address
 */
EXPORT void knl_invalidate_dcache_adr(volatile void *daddr, W dsize)
{
	W	sz = dsize + ((UW)daddr & (SCB_DCACHE_LINE_SIZE -1));
	UW	adr = (UW)daddr;

	knl_dsb();

	while(sz > 0) {
		out_w(SCB_DCIMVAC, adr);
		adr += SCB_DCACHE_LINE_SIZE;
		sz  -= SCB_DCACHE_LINE_SIZE;
	}

	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Clean D-Cache by Address
 */
EXPORT void knl_clean_dcache_adr(volatile void *daddr, W dsize)
{
	W	sz = dsize + ((UW)daddr & (SCB_DCACHE_LINE_SIZE -1));
	UW	adr = (UW)daddr;

	knl_dsb();

	while(sz > 0) {
		out_w(SCB_DCCMVAC, adr);
		adr += SCB_DCACHE_LINE_SIZE;
		sz  -= SCB_DCACHE_LINE_SIZE;
	}

	MEMORY_BARRIER
}

/* ------------------------------------------------------------------------ */
/*
 * Clean and Invalidate D-Cache by Address
 */
EXPORT void knl_clean_inval_dcache_adr(volatile void *daddr, W dsize)
{
	W	sz = dsize + ((UW)daddr & (SCB_DCACHE_LINE_SIZE -1));
	UW	adr = (UW)daddr;

	knl_dsb();

	while(sz > 0) {
		out_w(SCB_DCCIMVAC, adr);
		adr += SCB_DCACHE_LINE_SIZE;
		sz  -= SCB_DCACHE_LINE_SIZE;
	}

	MEMORY_BARRIER
}

#endif	/* USE_CACHE && CPU_HAS_CACHE */
#endif /* CPU_CORE_ARMV8M */