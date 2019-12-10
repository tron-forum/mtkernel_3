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
 *	sysdepend.h (ARM Cortex-M3)
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_CPU_CORE_SYSDEPEND_
#define _SYSDEPEND_CPU_CORE_SYSDEPEND_

/*
 *    System data (sys_dat.c)
 */
IMPORT	W	knl_taskindp;		/* Task independent status */
IMPORT	UB	exchdr_tbl[];		/* Exception handler table (RAM) */

/*
 *    Reset Handler (vector_tbl.c/reset_hdl.c)
 */
IMPORT void Reset_Handler(void);		/* Reset Handler */

/*
 *    Exception Handler (vector_tbl.c/exc_hdr.c)
 */
IMPORT void Reset_Handler(void);		/* Reset Handler */
IMPORT void NMI_Handler(void);			/* NMI Handler */
IMPORT void HardFault_Handler(void);		/* Hard Fault Handler */
IMPORT void MemManage_Handler(void);		/* MPU Fault Handler */
IMPORT void BusFault_Handler(void);		/* Bus Fault Handler */
IMPORT void UsageFault_Handler(void);		/* Usage Fault Handler */
IMPORT void Svcall_Handler(void);		/* Svcall */
IMPORT void DebugMon_Handler(void);		/* Debug Monitor Handler */

IMPORT void Default_Handler(void);		/* Default Handler (Undefine Interrupt) */

/*
 * Dispatcher (dispatch.S)
 */
IMPORT void knl_dispatch_entry(void);		/* dispatch entry */
IMPORT void knl_dispatch_to_schedtsk(void);	/* force dispatch */

/*
 * Interrupt Control (interrupt.c)
 */
IMPORT void knl_systim_inthdr(void);		/* System-timer Interrupt handler */


#endif /* _SYSDEPEND_CPU_CORE_SYSDEPEND_ */
