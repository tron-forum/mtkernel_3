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
 *	vector_tbl.S  (ARMv7-A)
 *	Exception Vector Table
 */

	.section    VECTOR_TABLE, "ax"
	.arm	

/* ---------------------------------------------------------------------------------
 *	Exception Vector Table
 */
	.global _PowerON_Reset
_PowerON_Reset:				// Power-on reset entry point

	.global Csym(vector_table)
	.extern Csym(Reset_Handler)
	.extern Csym(undef_entry)
	.extern Csym(svc_entry)
	.extern Csym(iabort_entry)
	.extern Csym(dabort_entry)
	.extern Csym(irq_entry)
	.extern Csym(fiq_entry)

Csym(vector_table):
	ldr pc, =Csym(Reset_Handler)	// 0x0000 : Reset exception
	ldr pc, =Csym(undef_entry)	// 0x0004 : Undefined instructions exception
	ldr pc, =Csym(svc_entry)	// 0x0008 : Software interrupts exceptions
	ldr pc, =Csym(iabort_entry)	// 0x000c : Prefetch abort exception
	ldr pc, =Csym(dabort_entry)	// 0x0010 : Data abort exception
	nop				// 0x0014 : Reserved
	ldr pc, =Csym(irq_entry)	// 0x0018 : IRQ exception
	ldr pc, =Csym(fiq_entry)	// 0x001c : FIQ exception

#endif	/* CPU_CORE_ARMV7A */