/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdepend.h (RXv2)
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
IMPORT void SuperVisorInst_Handler(void) __attribute__ ((interrupt));	/* Supervisor Instruction Exception */
IMPORT void AccessInst_Handler(void) __attribute__ ((interrupt));	/* Access Instruction Exception */
IMPORT void UndefinedInst_Handler(void) __attribute__ ((interrupt));	/* Undefined Instruction Exception */
IMPORT void FloatingPoint_Handler(void) __attribute__ ((interrupt));	/* Floating Point Exception */
IMPORT void NMI_Handler(void) __attribute__ ((interrupt));		/* NMI Handler */

IMPORT void Default_Handler(UW intno);	/* Default HLL Interrupt Handler (Undefine Interrupt) */

/*
 * Dispatcher (dispatch.S)
 */
IMPORT void knl_dispatch_entry(void);		/* dispatch entry */
IMPORT void knl_dispatch_to_schedtsk(void);	/* force dispatch */

/*
 * Interrupt Control (interrupt.c)
 */
IMPORT FP knl_int_vect_ram[N_INTVEC];		/* Interrupt Vector table (RAM) */
IMPORT FP knl_hll_inthdr_ram[N_INTVEC];		/* HLL Interrupt Handler Table (RAM) */

IMPORT	W knl_int_nest;			/* Interrupt nest counter */

IMPORT void knl_systim_inthdr(void);		/* System-timer Interrupt handler */


#endif /* _SYSDEPEND_CPU_CORE_SYSDEPEND_ */
