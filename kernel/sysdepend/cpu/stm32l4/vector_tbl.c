/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_STM32L4

/*
 *	vector_tbl.h (STM32L)
 *	Exception/Interrupt Vector Table
 */

#include "kernel.h"
#include "../../sysdepend.h"

/* ------------------------------------------------------------------------ */
/*
 * Exception/Interrupt Vector Table
 * 
 */

void (* const vector_tbl[])()  __attribute__((section(".vector"))) = {
	(void(*)()) (INITIAL_SP),		/* 0: Top of Stack */
	Reset_Handler,				/* 1: Reset Handler */
	NMI_Handler,				/* 2: NMI Handler */
	HardFault_Handler,			/* 3: Hard Fault Handler */
	MemManage_Handler,			/* 4: MPU Fault Handler */
	BusFault_Handler,			/* 5: Bus Fault Handler */
	UsageFault_Handler,			/* 6: Usage Fault Handler */
	0,					/* 7: Reserved */
	0,					/* 8: Reserved */
	0,					/* 9: Reserved */
	0,					/* 10: Reserved */
	Svcall_Handler,				/* 11: Svcall */
	DebugMon_Handler,			/* 12: Debug Monitor Handler */
	0,					/* 13: Reserved */
	knl_dispatch_entry,			/* 14: Pend SV */
	knl_systim_inthdr,			/* 15: Systick */

	/* External Interrupts */
	Default_Handler,			/* IRQ 0 */
	Default_Handler,			/* IRQ 1 */
	Default_Handler,			/* IRQ 2 */
	Default_Handler,			/* IRQ 3 */
	Default_Handler,			/* IRQ 4 */
	Default_Handler,			/* IRQ 5 */
	Default_Handler,			/* IRQ 6 */
	Default_Handler,			/* IRQ 7 */
	Default_Handler,			/* IRQ 8 */
	Default_Handler,			/* IRQ 9 */
	Default_Handler,			/* IRQ 10 */
	Default_Handler,			/* IRQ 11 */
	Default_Handler,			/* IRQ 12 */
	Default_Handler,			/* IRQ 13 */
	Default_Handler,			/* IRQ 14 */
	Default_Handler,			/* IRQ 15 */
	Default_Handler,			/* IRQ 16 */
	Default_Handler,			/* IRQ 17 */
	Default_Handler,			/* IRQ 18 */
	Default_Handler,			/* IRQ 19 */
	Default_Handler,			/* IRQ 20 */
	Default_Handler,			/* IRQ 21 */
	Default_Handler,			/* IRQ 22 */
	Default_Handler,			/* IRQ 23 */
	Default_Handler,			/* IRQ 24 */
	Default_Handler,			/* IRQ 25 */
	Default_Handler,			/* IRQ 26 */
	Default_Handler,			/* IRQ 27 */
	Default_Handler,			/* IRQ 28 */
	Default_Handler,			/* IRQ 29 */
	Default_Handler,			/* IRQ 30 */
	Default_Handler,			/* IRQ 31 */
	Default_Handler,			/* IRQ 32 */
	Default_Handler,			/* IRQ 33 */
	Default_Handler,			/* IRQ 34 */
	Default_Handler,			/* IRQ 35 */
	Default_Handler,			/* IRQ 36 */
	Default_Handler,			/* IRQ 37 */
	Default_Handler,			/* IRQ 38 */
	Default_Handler,			/* IRQ 39 */
	Default_Handler,			/* IRQ 40 */
	Default_Handler,			/* IRQ 41 */
	Default_Handler,			/* IRQ 42 */
	Default_Handler,			/* IRQ 43 */
	Default_Handler,			/* IRQ 44 */
	Default_Handler,			/* IRQ 45 */
	Default_Handler,			/* IRQ 46 */
	Default_Handler,			/* IRQ 47 */
	Default_Handler,			/* IRQ 48 */
	Default_Handler,			/* IRQ 49 */
	Default_Handler,			/* IRQ 50 */
	Default_Handler,			/* IRQ 51 */
	Default_Handler,			/* IRQ 52 */
	Default_Handler,			/* IRQ 53 */
	Default_Handler,			/* IRQ 54 */
	Default_Handler,			/* IRQ 55 */
	Default_Handler,			/* IRQ 56 */
	Default_Handler,			/* IRQ 57 */
	Default_Handler,			/* IRQ 58 */
	Default_Handler,			/* IRQ 59 */
	Default_Handler,			/* IRQ 60 */
	Default_Handler,			/* IRQ 61 */
	Default_Handler,			/* IRQ 62 */
	Default_Handler,			/* IRQ 63 */
	Default_Handler,			/* IRQ 64 */
	Default_Handler,			/* IRQ 65 */
	Default_Handler,			/* IRQ 66 */
	Default_Handler,			/* IRQ 67 */
	Default_Handler,			/* IRQ 68 */
	Default_Handler,			/* IRQ 69 */
	Default_Handler,			/* IRQ 70 */
	Default_Handler,			/* IRQ 71 */
	Default_Handler,			/* IRQ 72 */
	Default_Handler,			/* IRQ 73 */
	Default_Handler,			/* IRQ 74 */
	Default_Handler,			/* IRQ 75 */
	Default_Handler,			/* IRQ 76 */
	Default_Handler,			/* IRQ 77 */
	Default_Handler,			/* IRQ 78 */
	Default_Handler,			/* IRQ 79 */
	Default_Handler,			/* IRQ 80 */
	Default_Handler,			/* IRQ 81 */
};

#endif /* CPU_STM32L4 */
