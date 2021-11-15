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
 *	int_asm.S  (ARMv7-A)
 *	Interrupt control assembler routine
 */

#define	_in_asm_source_
#include <sys/sysdef.h>
#include <sys/knldef.h>

/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for interrupt handler
 *	Called by interrupt entry routine.
 *
 *	input :   r0 : Vector number
 *      stack image
 *       Low Address +---------------+
 *             sp -> | R0            |
 *                   | R1            |
 *                   | R2            |
 *                   | R3            |
 *                   | R12(ip)       |
 *                   | R14_svc(lr)   |
 *                   | R14_xxx(lr)   | Return address (pc)
 *                   | SPSR_xxx      |
 *      High Address +---------------+
 */
	.text
	.balign 4
	.arm
	.globl	Csym(knl_hll_inthdr)
	.type	Csym(knl_hll_inthdr), %function

Csym(knl_hll_inthdr):
	// enter Task independent part
	ldr	ip, =Csym(knl_taskindp)
	ldr	r2, [ip]
	add	r2, r2, #1
	str	r2, [ip]			// knl_taskindp++

	cmp	r2, #1				// 1st interrupt ?
	bne	not_1st_hllint			//	no => jump

	mov	r2, sp				// r2 <- task SP
	ldr	sp, =__irq_stack_start		// switch to handler stack 

#if USE_FPU & INTHDR_DIS_FPU
	fmrx	r1, fpexc			// r1: fpexc
	bic	ip, r1, #0x40000000		// disable VFP
	fmxr	fpexc, ip
#endif /* USE_FPU */

	push	{r1, r2}			// save fpexc & task SP

not_1st_hllint:					// call knl_hll_inthdr_tbl[n](intno)
	ldr	ip, =Csym(knl_hll_inthdr_tbl)
	ldr	ip, [ip, r3, lsl #2]
	blx	ip

	b	Csym(knl_return_inthdr)



/* ------------------------------------------------------------------------
 * Return Interrupt Handler
 */
	.global	Csym(knl_return_inthdr)
	.type	Csym(knl_return_inthdr), %function

Csym(knl_return_inthdr):
	cpsid	ia				// disable interrupt

	ldr	ip, =Csym(knl_taskindp)
	ldr	r2, [ip]
	sub	r2, r2, #1
	str	r2, [ip]			// knl_taskindp--

	cmp	r2, #0				// 1st interrupt ?
	bne	return_inthdr_end		//	no => jump

	pop	{r1, r2}
	mov	sp, r2				// restore task SP

#if USE_FPU & INTHDR_DIS_FPU
	fmxr	fpexc, r1			// restore fpexc
#endif /* USE_FPU */

	// Dispatch disabled?
	ldr	ip, =Csym(knl_dispatch_disabled)
	ldr	r2, [ip]
	cmp	r2, #0
	bne	return_inthdr_end

	// dispatch necessary?
	ldr	ip, =Csym(knl_ctxtsk)
	ldr	lr, =Csym(knl_schedtsk)
	ldr	ip, [ip]
	ldr	lr, [lr]
	cmp	ip, lr
	bne	knl_dispatch_entry		// goto dispatch processing

return_inthdr_end:
	ldmfd	sp!, {r0-r3, ip, lr}		// restore registers
	rfefd	sp!				// restore SPSR_xxx, pc_xxxx(return from exception)

/* ---------------------------------------------------------------------------------
 *	IRQ exception Stack Area 
 */
	.section	.irq_stack_section, "aw", %nobits
	.global	__irq_stack
__irq_stack:
	.space	EXC_STACK_SIZE

#endif	/* CPU_CORE_ARMV7A */