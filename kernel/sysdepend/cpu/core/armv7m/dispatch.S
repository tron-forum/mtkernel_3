/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7M
/*
 *	dispatch.S (ARMv7-M)
 *	Dispatcher
 */

#define	_in_asm_source_

#include <sys/machine.h>
#include <config.h>
#include <tk/errno.h>
#include <sys/sysdef.h>
#include <sys/knldef.h>

#include "offset.h"

	.code 16
	.syntax unified
	.thumb

/* ------------------------------------------------------------------------ */
/*
 * Dispatcher
 *
 *	Contexts to save
 *	Save registers except for ssp to a stack. Save 'ssp' to TCB.
 *
 *	   High Address +---------------+
 *			| (fpscr)	|
 *			| (S0 - S15)	|
 *			+---------------+
 *			| xpsr		|
 *			| pc		| Return address
 *			| lr		|
 *			| R12		|
 *			| R0-R3		|
 *			+---------------+ Save by Exception entry process.
 *			| R4 - R11	|
 *		ssp ->	| lr		|
 *			+---------------+
 *			| (S16 - S31)	|
 *		ssp ->	| (ufpu)	|
 *	    Low Address +---------------+
 *
 *		( ) Stacked only while using FPU
 */

#if USE_FPU
#define TA_FPU		0x00001000	/* Task attribute - Use FPU */
#define	EXPRN_NO_FPU	0x00000010	/* FPU usage flag  0:use 1:no use */
#endif

	.text
	.align 2
	.thumb
	.thumb_func
	.globl Csym(knl_dispatch_entry)

Csym(knl_dispatch_entry):	
/*----------------- Start dispatch processing. -----------------*/
	ldr	r0, =Csym(knl_dispatch_disabled)
	ldr	r1, =1
	str	r1, [r0]			// Dispatch disable

	ldr	r0, =Csym(knl_ctxtsk)
	ldr	r1, [r0]			// R1 = ctxtsk
	cmp	r1, #0
	bne	l_dispatch_000

	ldr	sp, =(Csym(knl_tmp_stack) + TMP_STACK_SIZE)	// Set temporal stack
	b	l_dispatch_100

/*----------------- Save "ctxtsk" context. -----------------*/
l_dispatch_000: 
	push	{r4-r11}
	push	{lr}

#if USE_FPU			// Save FPU register
	ldr	r2, [r1, #TCB_tskatr]
	ands	r2, r2, #TA_FPU
	beq	l_dispatch_010			// ctxtsk is not a TA_FPU attribute.

	ands	r3,lr, #EXPRN_NO_FPU
	bne	l_dispatch_010			// ctxtsk does not execute FPU instructions.

	vpush	{s16-s31}			// Push FPU register (S16-S31)
	push	{r3}				//FPU usage flag

l_dispatch_010:			// End of FPU register save process
#endif /* USE_FPU */

	str	sp, [r1, #TCB_tskctxb + CTXB_ssp]	// Save 'ssp' to TCB

	ldr	r2, =0
	str	r2, [r0]			// ctxtsk = NULL


/*----------------- Dispatch from "ctxtsk" to "schedtsk" -----------------*/
l_dispatch_100:
	ldr	r4, =Csym(knl_ctxtsk)
	ldr	r5, =Csym(knl_schedtsk)		// R5 = &schedtsk
	ldr	r6, =Csym(knl_lowpow_discnt)	// R6 = &lowpow_discnt

l_dispatch_110:
	ldr	r2, =INTPRI_VAL(INTPRI_MAX_EXTINT_PRI)	// Disable interruput
	msr	basepri, r2

	ldr	r8, [r5]			// R8 = schedtsk
	cmp	r8, #0				// Is there 'schedtsk'?
	bne	l_dispatch_120

	/* Moves to power saving mode because there are no tasks that can be run. */
	ldr	ip, [r6]			// Is 'low_pow' disabled?
	cmp	ip, #0
	it	eq
	bleq	Csym(low_pow)			// call low_pow()

	ldr	r2, =0
	msr	basepri, r2			// Enable interruput

	b	l_dispatch_110

l_dispatch_120:			// Switch to 'schedtsk'
	str	r8, [r4]			// ctxtsk = schedtsk
	ldr	sp, [r8, #TCB_tskctxb + CTXB_ssp]	// Restore 'ssp' from TCB


/*----------------- Restore "schedtsk" context. -----------------*/

#if USE_FPU			// Restore FPU context
	ldr	r0, [r8, #TCB_tskatr]
	ands	r0, r0, #TA_FPU
	beq	l_dispatch_200			// schedtsk is not a TA_FPU attribute.

	ldr	r3,[sp]				// load FPU usage flag
	ands	r3, r3, #EXPRN_NO_FPU
	bne	l_dispatch_200			// schedtsk does not execute FPU instructions.

	pop	{r3}
	vpop	{s16-s31}			// Pop FPU register (S16-S31)

l_dispatch_200:			//  End of FPU register restore process
#endif	/* USE_FPU */

	pop	{lr}
	pop	{r4-r11}

	ldr	r0, =Csym(knl_dispatch_disabled)
	ldr	r1, =0
	str	r1, [r0]			// Dispatch enable

	msr	basepri, r1			// Enable inperrupt

	bx	lr

#endif /* CPU_CORE_ARMV7M */
