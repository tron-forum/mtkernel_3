/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06A
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h (ARMv7-A)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_CORE_SYSDEPEND_
#define _SYSDEPEND_CPU_CORE_SYSDEPEND_

/*
 *    Reset Handler (reset_hdl.S)
 */
IMPORT void Reset_Handler(void);		/* Reset Handler */

/*
 *    Exception Handler (exc_hdr.c)
 */
IMPORT void UndefinedInst_Handler(void);	/* Undefined instruction exception handler */
IMPORT void PrefetchAbort_Handler(void);	/* Prefetch abort exception handler */
IMPORT void DataAbort_Handler(void);		/* Data abort exception handler */
IMPORT void VFPInvalid_Handler(void);		/* VFP invalid handler */

IMPORT void Default_Handler(void);		/* Default Handler (Undefine Interrupt) */
IMPORT void SVC_default_Handler(void);		/* SVC default Handler (Undefine SVC) */

/*
 * Dispatcher (dispatch.S)
 */
IMPORT void knl_dispatch_entry(void);		/* dispatch entry */
IMPORT void knl_dispatch_to_schedtsk(void);	/* force dispatch */

/*
 * Interrupt Control (interrupt.c)
 */
IMPORT FP knl_intvec_tbl[N_INTVEC];		/* Interrupt vector table */
IMPORT FP knl_hll_inthdr_tbl[N_INTVEC];		/* High level programming language interrupt handler table */
IMPORT const FP knl_svcvec_tbl[N_SVCHDR];	/* SVC handler table */

/*
 * Interrupt Control (int_asm.S)
 */
IMPORT void knl_hll_inthdr(void);		/* High level programming language routine for interrupt handler */


/*
 * Task context block
 */
typedef struct {
	void	*ssp;		/* System stack pointer */
} CTXB;


#endif /* _SYSDEPEND_CPU_CORE_SYSDEPEND_ */
