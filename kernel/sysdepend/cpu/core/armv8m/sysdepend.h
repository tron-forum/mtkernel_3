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

/*
 *	sysdepend.h (ARMv8-M)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_CORE_SYSDEPEND_
#define _SYSDEPEND_CPU_CORE_SYSDEPEND_

/*
 * Dispatcher (dispatch.S)
 */
IMPORT void knl_dispatch_entry(void);		/* dispatch entry */
IMPORT void knl_dispatch_to_schedtsk(void);	/* force dispatch */

/*
 * Interrupt Control (interrupt.c)
 */
IMPORT UW knl_exctbl[];		/* Exception handler table (RAM) */
IMPORT UW *knl_exctbl_o;		// Exception handler table (Origin)
IMPORT void knl_systim_inthdr(void);	/* System-timer Interrupt handler */


/*
 *    Reset Handler (reset_hdl.c)
 */
IMPORT void Reset_Handler(void);		/* Reset Handler */

/*
 * Exception handler (exc_hdr.c)
 */
IMPORT void knl_nmi_handler(void);		/* 2: NMI Handler */
IMPORT void knl_hardfault_handler(void);	/* 3: Hard Fault Handler */
IMPORT void knl_memmanage_handler(void);	/* 4: MPU Fault Handler */
IMPORT void knl_busfault_handler(void);		/* 5: Bus Fault Handler */
IMPORT void knl_usagefault_handler(void);	/* 6: Usage Fault Handler */
IMPORT void knl_securefault_handler(void);	/* 7: Secure Fault Handler */
IMPORT void knl_svcall_handler(void);		/* 11: Svcall */
IMPORT void knl_debugmon_handler(void);		/* 12: Debug Monitor Handler */

IMPORT void knl_default_handler(void);		/* Default Handler */

/*
 *	Kernel start routine (sys_start.c)
 */
IMPORT void knl_start_mtkernel(void);

/*
 * Task context block
 */
typedef struct {
	void	*ssp;		/* System stack pointer */

#if USE_SPMON
	void	*spsa;		/* Stack stat address */
	void	*spea;		/* Stack end address */
#endif
} CTXB;

/*
 *	Control register operation
 */

Inline UW knl_get_ipsr(void)
{
	UW	ipsr;
	Asm("mrs %0, ipsr": "=r"(ipsr));
	return ipsr;
}

Inline UW knl_get_xpsr(void)
{
	UW	xpsr;
	Asm("mrs %0, psr": "=r"(xpsr));
	return xpsr;
}

Inline UW knl_get_primask(void)
{
	UW	primask;
	Asm("mrs %0, primask": "=r"(primask));
	return primask;
}

/*
 *	Memory Barrier
 */
Inline void knl_isb(void)
{
	Asm("isb 0xF":::"memory");
}

Inline void knl_dsb(void)
{
	Asm("dsb 0xF":::"memory");
}

Inline void knl_dmb(void)
{
	Asm("dmb 0xF":::"memory");
}

#if USE_CACHE && CPU_HAS_CACHE
/*
 * Cache control (cache.c)
 */
IMPORT void knl_enable_icache(void);
IMPORT void knl_disable_icache(void);
IMPORT void knl_invalidate_icache(void);

IMPORT void knl_enable_dcache(void);
IMPORT void knl_disable_dcache(void);
IMPORT void knl_invalidate_dcache(void);
IMPORT void knl_clean_dcache(void);
IMPORT void knl_clean_inval_dcache(void);

IMPORT void knl_invalidate_dcache_adr(volatile void *daddr, W dsize);
IMPORT void knl_clean_dcache_adr(volatile void *daddr, W dsize);
IMPORT void knl_clean_inval_dcache_adr(volatile void *daddr, W dsize);

Inline BOOL knl_check_icache(void)
{
	return (in_w(SCB_CCR)&CCR_IC)?TRUE:FALSE;
}

Inline BOOL knl_check_dcache(void)
{
	return (in_w(SCB_CCR)&CCR_DC)?TRUE:FALSE;
}
#endif	/* USE_CACHE && CPU_HAS_CACHE */

/*
 * Task System Dependent definition
 */
IMPORT ER knl_tcb_sysdep_cre(TCB *tcb, CONST T_CTSK *pk_ctsk);	// TCB system dependent initialization
IMPORT ER knl_tcb_sysdep_del(TCB *tcb);				// TCB system dependent finalization

#endif /* _SYSDEPEND_CPU_CORE_SYSDEPEND_ */
