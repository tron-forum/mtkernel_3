/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.02
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_RXV2

/*
 *	vector_tbl.h (RXv2)
 *	Exception Vector Table
 */

#include "kernel.h"
#include "../../../sysdepend.h"

/* ------------------------------------------------------------------------ */
/*
 * Reset Vector
 * 
 */

const void (*reset_vect[])()  __attribute__((section (".fvectors"))) = {
	Reset_Handler,				/* Power On Reset */
};

/* ------------------------------------------------------------------------ */
/*
 * Exception Vector Table
 * 
 */

const void (*exvect_tbl[])()  __attribute__((section(".exvectors"))) = {
	(void(*)())(0xffffffff),		/* MDES  Endian Select Register */
	0,
	(void(*)())(0xffffffff),		/* OFS1 Option function */
	(void(*)())(0xffffffff),		/* OFS0 Option function */
	0,
	0,
	0,
	0,
	(void(*)())(0xffffffff),		/* ID Code */
	(void(*)())(0xffffffff),		/* ID Code */
	(void(*)())(0xffffffff),		/* ID Code */
	(void(*)())(0xffffffff),		/* ID Code */
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	SuperVisorInst_Handler,			/* Supervisor Instruction Exception */
	AccessInst_Handler,			/* Access Instruction Exception */
	0,
	UndefinedInst_Handler,			/*  Undefined Instruction Exception */
	0,
	FloatingPoint_Handler,			/* Floating Point Exception */
	0,
	0,
	0,
	0,
	NMI_Handler,				/* Non Maskable Interrupt */
};

#endif /* CPU_CORE_RXV2 */
