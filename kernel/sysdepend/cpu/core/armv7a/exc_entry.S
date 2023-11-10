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
 *	exc_entry.S  (ARMv7-A)
 *	Exception entry routine
 */

#define	_in_asm_source_
#include <config.h>
#include <sys/knldef.h>
#include "offset.h"

/* GIC Register definition */
#define GIC_ICC_BASE	0xE8222000
#define GIC_ICC_IAR	0x000C		// Interrupt Acknowledge Register
#define GIC_ICC_EOIR	0x0010		// End of Interrupt Register

	.section    EXC_ENTRY, "ax"
	.arm	

/* ---------------------------------------------------------------------------------
 *	IRQ exception
 */
	.global Csym(irq_entry)
	.extern Csym(knl_intvec_tbl)
	.extern Csym(Default_Handler)
	
Csym(irq_entry):
	sub	lr, lr, #4			// Return address adjustment
	srsdb	sp!, #PSR_SVC			// Save SPSR_irq & R14_irq to SVC stack
	CPS	#PSR_SVC			// Change to SVC mode
	stmfd	sp!, {r0-r3, ip, lr}		// Save R0-R3,R12,LR to SVC stack

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

	ldr	lr, =GIC_ICC_BASE
	ldr	r3, [lr, #GIC_ICC_IAR]		// Read interrupt response register
	mov	r3, r3, lsl #22
	mov	r3, r3, lsr #22			// r3 <- Interrupt ID (& 0x3ff)

	ldr	r0, =N_INTVEC
	cmp	r3, r0				// Is the interrupt number correct?
	bge	l_irq1				//	NO => Jump

	mov	r0, r3
	ldr	lr,  =Csym(knl_intvec_tbl)
	add	lr, r3, lsl #2
	ldr	lr, [lr]			// lr <- Interrupt handler address
	cmp	lr, #0				// Is an interrupt handler registered?
	bxne	lr				//	YES => jump    r0: Interrupt ID
	b	l_irq2				//	NO  => default_entry

l_irq1: // Interrupt ID is an abnormal value
	ldr     r0, =0x3ff
	cmp     r3, r0				// If Interrupt ID is 0x3ff,
	beq     l_irq3				//	it is a false positive and returns.

l_irq2:	// Call the default handler
	ldr	lr, =GIC_ICC_BASE
	str	r3, [lr, #GIC_ICC_EOIR]		// Writing the interrupt end register

	ldr	lr, =Csym(Default_Handler)
	bx	lr

l_irq3:	// Return without doing anything.
	ldmfd	sp!, {r0-r3, ip, lr}		// restore registers
	rfefd	sp!				// restore SPSR_irq, R14_irq (return from exception)


/* ---------------------------------------------------------------------------------
 *	Supervisor call (SVC)
 */
	.global Csym(svc_entry)
	.extern Csym(knl_svcvec_tbl)
	.extern Csym(SVC_default_Handler)
Csym(svc_entry):
	srsdb	sp!, #PSR_SVC			// Save SPSR_svc and R14_svc on SVC stack.
	stmfd	sp!, {r0-r3, ip, lr}		// Save R0-R3,R12,LR on SVC stack.
	mov	r0, #0xffffffff
	str	r0, [sp, #(5 * 4)]		// Change the value of LR on the stack.

	/*
	* Stack image
	*          +---------------+
	* SP_svc ->|R0             |
	*          |R1             |
	*          |R2             |
	*          |R3             |
	*          |R12(ip)        |
	*          |0xffffffff     |
	*          |R14_svc(lr)    | <- return address from exception
	*          |SPSR_svc       |
	*          +---------------+
	*/

	mrs	r0, spsr
	tst	r0, #PSR_T
	ldrneh	r0, [lr, #-2]		// Thumb instruction
	ldreq	r0, [lr, #-4]		// ARM instruction
	bicne	r0, r0, #0xff00
	biceq   r0, r0, #0xff000000
	// r0 = SVC No.

	cmp	r0, #N_SVCHDR
	bge	SVC_default_Handler	// SVC No. >= N_SVC  => SVC default_entry

	ldr	lr, =knl_svcvec_tbl
	add	lr, r0, lsl #2
	ldr	lr, [lr]		// lr: SVC handler address
	cmp	lr, #0			// Registered?
	bxne	lr			//   YES -> jump    r0: SVC No.
	b	SVC_default_Handler		//   NO  -> SVC default_entry


/* ---------------------------------------------------------------------------------
 *	FIQ exception
 */
	.global Csym(fiq_entry)
	.extern Csym(FIQ_Handler)
Csym(fiq_entry):
	sub	lr, lr, #4		// Return address adjustment
	ldr	sp, =__fiq_stack_start	// Switch to exception stack
	stmfd	sp!, {r0-r3, ip, lr}	// Save R0-R3,R12,LR to FIQ stack

	/* Stack image
	*          +---------------+
	* SP_fiq ->|R0             |
	*          |R1             |
	*          |R2             |
	*          |R3             |
	*          |R12(ip)        |
	*          |R14_fiq(lr)    | <- return address from exception
	*          +---------------+
	*/

	ldr	lr, =Csym(FIQ_Handler)
	bx	lr

/* ---------------------------------------------------------------------------------
 *	Undefined instruction exception
 */
	.global Csym(undef_entry)
	.extern Csym(UndefinedInst_Handler)
	.extern Csym(VFPInvalid_Handler)

Csym(undef_entry):
	sub	lr, lr, #4		// Return address adjustment
	ldr	sp, =__und_stack_start	// Switch to exception stack
	stmfd	sp!, {r0-r3, ip, lr}	// Save R0-R3,R12,LR to UDF stack

	/* Stack image
	*          +---------------+
	* SP_udf ->|R0             |
	*          |R1             |
	*          |R2             |
	*          |R3             |
	*          |R12(ip)        |
	*          |R14_und(lr)    | <- return address from exception
	*          +---------------+
	*/

#if USE_FPU

#define TA_FPU	0x00001000		// TA_FPU = TA_COP0 = 0x00001000

	mrc	p15, 0, ip, c1, c0, 2	// CPACR
	tst	ip, #5 << 20		// CP10,11 available ?
	beq	exe_undhdr		//  no -> jump	
	fmrx	ip, fpexc
	tst	ip, #0x40000000		// fpexc.en ?
	bne	exe_undhdr		//  no -> jump	

	/* After that, processing for FPU instructions */
	ldr	ip, =Csym(knl_ctxtsk)
	ldr	r0, [ip]		// r0 = ctxtsk
	cmp	r0, #0			// if ctxtsk == NULL  -> invalid use of VFP
	beq	exe_undhdr

	ldr	ip, [r0, #TCB_tskatr]	
	tst	ip, #TA_FPU		// if no TA_FPU attribute  ->  invalid use of VFP
	beq	exe_undhdr

	ldr	ip, =Csym(knl_taskindp)
	ldr	r2, [ip]		// r2 = knl_taskindp
	cmp	r2, #0			// if knl_taskindp > 0  ->  invalid use of VFP
	bhi	exe_undhdr

	/* enable VFP */
	fmrx	ip, fpexc
	orr	ip, ip, #0x40000000	// FPEXC.EN = 1
	fmxr	fpexc, ip

	ldr	r2, =Csym(knl_fpu_ctx)	// r2 = &knl_fpu_ctx
	ldr	r1, [r2]		// r1 = knl_fpu_ctx
	cmp	r1, r0			// if knl_fpu_ctx == ctxtsk  -> context switching is not required.
	beq	rtn_undef

	cmp	r1, #0			// if knl_fpu_ctx == NULL  -> context saving is not required.
	beq	load_fpuctx

	/* save VFP context */
	fmrx	lr, fpscr		// Floating-Point Status and Control Register
	ldr	ip, [r1, #TCB_isstack]	// r1 = TCB.isstack
	sub	ip, ip, #FPU_context	// IP = FPU context

	stmia	ip!, {r0, lr}		// (r0 is padding)
	fstmiad	ip!, {d0-d15}
	fstmiad	ip!, {d16-d31}

	/* load VFP context */
load_fpuctx:
	ldr	ip, [r0, #TCB_isstack]	// r0 = ctxtsk
	sub	ip, ip, #FPU_context	// IP = FPU context

	ldmia	ip!, {r1, lr}		// (r1 is padding)
	fldmiad	ip!, {d0-d15}
	fldmiad	ip!, {d16-d31}
	fmxr	fpscr, lr		// Floating-Point Status and Control Register

	str	r0, [r2]		// knl_fpu_ctx = ctxtsk

rtn_undef:
	ldmfd	sp!, {r0-r3, ip, lr}	// restore registers
	movs	pc,lr			// return from exception

exe_undhdr:
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
	ldr	sp, =__abt_stack_start	// Switch to exception stack
	stmfd	sp!, {r0-r3, ip, lr}	// Save R0-R3,R12,LR to ABT stack

	/* Stack image
	*          +---------------+
	* SP_abt ->|R0             |
	*          |R1             |
	*          |R2             |
	*          |R3             |
	*          |R12(ip)        |
	*          |R14_abt(lr)    | <- return address from exception
	*          +---------------+
	*/

	ldr	lr, =Csym(PrefetchAbort_Handler)
	bx	lr

/* ---------------------------------------------------------------------------------
 *	Data abort exception
 */
	.global Csym(dabort_entry)
	.extern Csym(DataAbort_Handler)

Csym(dabort_entry):
	sub	lr, lr, #4		// Return address adjustment
	ldr	sp, =__abt_stack_start	// Switch to exception stack
	stmfd	sp!, {r0-r3, ip, lr}	// Save R0-R3,R12,LR to ABT stack

	/* Stack image
	*          +---------------+
	* SP_abt ->|R0             |
	*          |R1             |
	*          |R2             |
	*          |R3             |
	*          |R12(ip)        |
	*          |R14_abt(lr)    | <- return address from exception
	*          +---------------+
	*/

	ldr	lr, =Csym(DataAbort_Handler)
	bx	lr

/* ---------------------------------------------------------------------------------
 *	Exception Stack Area 
 */
	.section	.fiq_stack_section, "aw", %nobits
	.global	__fiq_stack
__fiq_stack:
	.space	FIQ_STACK_SIZE

	.section	.und_stack_section, "aw", %nobits
	.global	__und_stack
__und_stack:
	.space	UND_STACK_SIZE

	.section	.abt_stack_section, "aw", %nobits
	.global	__abt_stack
__abt_stack:
	.space	ABT_STACK_SIZE


#endif	/* CPU_CORE_ARMV7A */