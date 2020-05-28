/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_RXV2

/*
 *	reset_hdr.S (RXv2)
 *	Reset handler
 */

#define	_in_asm_source_

#include <sys/sysdef.h>

/*
 * ---------------------------------------------------------------------- 
 *	Entry Point at Reset
 * ----------------------------------------------------------------------
 */
	.section .text
	.global Csym(Reset_Handler)

	.extern Csym(reset_main )

Csym(Reset_Handler):
	mvtc	#INTSTACK_TOP, ISP		/* initialise interrupt stack pointer */

	mvtc	#_exvectors_start, extb		/* setup exception vector */
	mvtc	#_rvectors_start, intb		/* setup interrupt vector */

	mvtc	#FPSW_VAL, fpsw			/* setup FPSW */

	mov    #Csym(reset_main),r7		/* goto reset_handler main routine  */
	jsr    r7				/* no return */

loop_here:
	bra loop_here

	.end

#endif	/* CPU_CORE_RXV2 */
