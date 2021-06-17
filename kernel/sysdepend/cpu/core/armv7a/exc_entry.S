/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7A

/*
 *	exc_entry.S  (ARMv7-MA)
 *	Exception entry routine
 */

#define	_in_asm_source_
#include <sys/knldef.h>

/* GIC Register definition */
#define GIC_ICC_BASE	0xE8202000
#define GIC_ICC_IAR	0x000C		// Interrupt Acknowledge Register
#define GIC_ICC_EOIR	0x0010		// End of Interrupt Register

	.section    EXC_ENTRY, "ax"
	.arm	

/* Stack image by exception handling
*	          +---------------+
*	 SP_svc ->|R0             |
*	          |R1             |
*	          |R2             |
*	          |R3             |
*	          |R12(ip)        |
*	          |R14_svc(lr)    |
*	          |R14_irq(lr)    | <- return address from interrupt
*	          |SPSR_irq       |
*	          +---------------+
*/

/* ---------------------------------------------------------------------------------
 *	IRQ exception
 */
	.global Csym(irq_entry)
	.extern Csym(knl_intvec_tbl)
	.extern Csym(Default_Handler)
	
Csym(irq_entry):
	sub	lr, lr, #4			// Return address adjustment
	srsdb	sp!, #PSR_SVC			// Save SPSR_und & R14_und to SVC stack
	CPS	#PSR_SVC			// Change to SVC mode
	stmfd	sp!, {r0-r3, ip, lr}		// Save R0-R3,R12,LR to SVC stack

	ldr	lr, =GIC_ICC_BASE
	ldr	r3, [lr, #GIC_ICC_IAR]		// Read interrupt response register
	mov	r3, r3, lsl #22
	mov	r3, r3, lsr #22			// r3 <- Interrupt ID (& 0x3ff)

	ldr	r0, =N_INTVEC
	cmp	r3, r0				// Is the interrupt number correct?
	bge	1f				//	NO => Jump

	ldr	lr,  =Csym(knl_intvec_tbl)
	add	lr, r0, lsl #2
	ldr	lr, [lr]			// lr <- Interrupt handler address
	cmp	lr, #0				// Is an interrupt handler registered?
	bxne	lr				//	YES => jump    r0: Interrupt ID
	b	2f				//	NO  => default_entry

1:  // Interrupt ID is an abnormal value
	ldr     r0, =0x3ff
	cmp     r3, r0		// False positive if ID is 0x3ff
	beq     3f		//	 => return
2:
	ldr	lr, =GIC_ICC_BASE
	str	r3, [lr, #GIC_ICC_EOIR]		// Writing the interrupt end register

	ldr	lr, =Csym(Default_Handler)
	bx	lr
3:
	ldmfd	sp!, {r0-r3, ip, lr}		// restore registers
	rfefd	sp!				// restore SPSR_xxx, pc_xxxx(return from exception)


/* ---------------------------------------------------------------------------------
 *	FIQ exception
 */
	.global Csym(fiq_entry)
Csym(fiq_entry):

/* ---------------------------------------------------------------------------------
 *	Supervisor call (SVC)
 */
	.global Csym(svc_entry)
Csym(svc_entry):

/* ---------------------------------------------------------------------------------
 *	Undefined instruction exception
 */
	.global Csym(undef_entry)
	.extern Csym(UndefinedInst_Handler)
	.extern Csym(VFPInvalid_Handler)

Csym(undef_entry):
	sub	lr, lr, #4		// Return address adjustment
	srsdb	sp!, #PSR_SVC		// Save SPSR_und & R14_und to SVC stack
	CPS	#PSR_SVC		// Change to SVC mode
	stmfd	sp!, {r0-r3, ip, lr}	// Save R0-R3,R12,LR to SVC stack

#if USE_FPU
	mrc	p15, 0, ip, c1, c0, 2	// CPACR
	tst	ip, #5 << 20		// CP10,11 available ?
	beq	not_navfp		//  no -> jump	
	fmrx	ip, fpexc
	tst	ip, #0x40000000		// fpexc.en ?
	bne	not_navfp		//  yes -> jump	
	ldr	lr, =Csym(VFPInvalid_Handler)
	bx	lr

not_navfp:
#endif /* USE_FPU */
	ldr	lr, =Csym(UndefinedInst_Handler)
	bx	lr

/* ---------------------------------------------------------------------------------
 *	Prefetch abort exception
 */
	.global Csym(iabort_entry)
	.extern Csym(PrefetchAbort_Handler)

Csym(iabort_entry):
	sub	lr, lr, #4		// Return address adjustment
	srsdb	sp!, #PSR_SVC		// Save SPSR_abt & R14_abt to SVC stack
	CPS	#PSR_SVC		// Change to SVC mode
	stmfd	sp!, {r0-r3, ip, lr}	// Save R0-R3,R12,LR to SVC stack

	ldr	lr, =Csym(PrefetchAbort_Handler)
	bx	lr

/* ---------------------------------------------------------------------------------
 *	Data abort exception
 */
	.global Csym(dabort_entry)
	.extern Csym(DataAbort_Handler)

Csym(dabort_entry):
	sub	lr, lr, #4		// Return address adjustment
	srsdb	sp!, #PSR_SVC		// Save SPSR_abt & R14_abt to SVC stack
	CPS	#PSR_SVC		// Change to SVC mode
	stmfd	sp!, {r0-r3, ip, lr}	// Save R0-R3,R12,LR to SVC stack

	ldr	lr, =Csym(DataAbort_Handler)
	bx	lr

#endif	/* CPU_CORE_ARMV7A */