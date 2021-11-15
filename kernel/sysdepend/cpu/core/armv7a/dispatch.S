/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
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
 *	dispatch.S (ARMv7-A)
 *	Dispatcher
 */

#define	_in_asm_source_
#include "offset.h"

/* ------------------------------------------------------------------------ */
/*
 * Dispatcher
 *	knl_dispatch_entry:
 *		Normal dispatch processing.
 *	knl_dispatch_to_schedtsk:
 *		force dispatch processing.
 *
 *	Contexts to save
 *	Save registers except for sp to a stack. Save 'sp' to TCB.
 *
 *       Low Address +---------------+
 *             sp -> | R4            |
 *                   | R5            |
 *                   | R6            |
 *                   | R7            |
 *                   | R8            |
 *                   | R9            |
 *                   | R10           |
 *                   | R11           |
 *                   +---------------+
 *                   | R0            |
 *                   | R1            |
 *                   | R2            |
 *                   | R3            |
 *                   | R12(ip)       |
 *                   | R14_svc(lr)   |
 *                   | R14_xxx(lr)   | Return address (pc)
 *                   | SPSR_xxx      |
 *      High Address +---------------+
 */

/* ----------------------------------------
 *     force dispatch
 * ---------------------------------------- */
	.text
	.balign 4
	.arm
	.globl	Csym(knl_dispatch_to_schedtsk)
	.type	Csym(knl_dispatch_to_schedtsk), %function
Csym(knl_dispatch_to_schedtsk):
	/* Interrupt is disabled(CPSR.I=1),during SVC mode */
	ldr	sp, =__tmp_stack_start			// Set temporal stack

	ldr	ip, =Csym(knl_dispatch_disabled)
	ldr	r0, =1
	str	r0, [ip]				// Dispatch disable

	ldr	r4, =Csym(knl_ctxtsk)			// R4: &ctxtsk
	ldr	r0, =0

	ldr	r8, [r4]				// R8: ctxtsk
	str	r0, [r4]				// ctxtsk = NULL
	b	l_dispatch0

/* ----------------------------------------
 *     normal dispatch
 * ---------------------------------------- */
	.text
	.balign 4
	.arm
	.globl	Csym(knl_dispatch_entry)
	.type	Csym(knl_dispatch_entry), %function
Csym(knl_dispatch_entry):
	/* Interrupt is disabled(CPSR.I=1),during SVC mode */
	ldr	r0, =Csym(knl_dispatch_disabled)
	ldr	r1, =1
	str	r1, [r0]				// Dispatch disable

	// Context save
	stmfd	sp!, {r4-r11}				// save registers

	ldr	r4, =Csym(knl_ctxtsk)			// R4: &ctxtsk
	ldr	r0, =0
	ldr	r8, [r4]				// R8: ctxtsk
	cmp	r8, #0					// ctxtsk == NULL ?
	strne	sp, [r8, #TCB_tskctxb + CTXB_ssp]	// Save 'sp' to TCB
	strne	r0, [r4]				// ctxtsk = NULL

l_dispatch0:
	ldr	r5, =Csym(knl_schedtsk)			// R5: &schedtsk
	ldr	r6, =Csym(knl_lowpow_discnt)		// R6: &lowpow_discnt

l_dispatch1:
	cpsid	ia					// disable interrupt

	ldr	r8, [r5]				// R8: schedtsk
	cmp	r8, #0					// 'schedtsk' exist?
	bne	l_dispatch2				// yes -> jump

	/* Because there is no task that should be executed, move to the power-saving mode */
	ldr	ip, [r6]				// Is 'low_pow' disabled?
	cmp	ip, #0
	bleq	Csym(low_pow)			// call low_pow()

	cpsie	ia					// enable interrupt
	nop
	b	l_dispatch1				// try again

l_dispatch2:						// Switch to 'schedtsk'
	str	r8, [r4]				// r4: &ctxtsk, r8: schedtsk

	// restore context
	ldr	sp, [r8, #TCB_tskctxb + CTXB_ssp]	// Restore 'sp' from TCB

#if USE_FPU
	/* switch VFP context */
	ldr	ip, =Csym(knl_fpu_ctx)			// r8: ctxtsk
	ldr	ip, [ip]
	fmrx	r0, fpexc
	cmp	ip, r8
	orreq	r0, r0, #0x40000000			// enable  VFP(use current context)
	bicne	r0, r0, #0x40000000			// disable VFP(need context switch)
	fmxr	fpexc, r0
#endif /* USE_FPU */

	ldmfd	sp!, {r4-r11}				// restore registers

	ldr	r0, =Csym(knl_dispatch_disabled)
	ldr	r1, =0
	str	r1, [r0]				// Dispatch enable

	/* return to task */
	ldmfd	sp!, {r0-r3, ip, lr}	// restore registers
	rfefd	sp!			// restore SPSR_xxx, pc_xxxx(return from exception)

#endif /* CPU_CORE_ARMV7A */