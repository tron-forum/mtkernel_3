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
#ifdef CPU_RZA2M

/*
 *	sf_bott.S  (RZ/A2M)
 *	Boot Loader for Serial Flash
 */

#define	_in_asm_source_
#include <sys/sysdef.h>

/* ------------------------------------------------------------------------*/
/*
 *	Vector Table for Serial Flash
 */
	.section	SF_VECTOR_TABLE, "ax"
	.arm

	.global Csym(sf_vector_table)
Csym(sf_vector_table):
	ldr pc, =sf_reset_handler	// 0x0000 : Reset exception
	ldr pc, =sf_default_entry	// 0x0004 : Undefined instructions exception
	ldr pc, =sf_default_entry	// 0x0008 : Software interrupts exceptions
	ldr pc, =sf_default_entry	// 0x000c : Prefetch abort exception
	ldr pc, =sf_default_entry	// 0x0010 : Data abort exception
	nop				// 0x0014 : Reserved
	ldr pc, =sf_default_entry	// 0x0018 : IRQ exception
	ldr pc, =sf_default_entry	// 0x001c : FIQ exception

Literals_vector:
	.LTORG

sf_default_entry:
	b	.

/* ------------------------------------------------------------------------*/
/*
 *	Reset handler for Serial Flash
 */
	.section	SF_RESET_HANDLER, "ax"
	.arm
	.global Csym(start)
Csym(start):
sf_reset_handler:
	/* Disable IRQ, FIQ */
	mrs	r0, cpsr
	orr	r0, r0, #PSR_DI
	msr	cpsr_c, r0

	/* disable I Cache & MMU */
	mrc	p15, 0, r0, c1, c0, 0	// Read CP15 register (SCTLR)
	bic	r0, r0, #CP15_SCTLR_I	// Clear I bit to disable I Cache
	bic	r0, r0, #CP15_SCTLR_C	// Clear C bit to disable D Cache
	bic	r0, r0, #CP15_SCTLR_M	// Clear M bit to disable MMU
	bic	r0, r0, #CP15_SCTLR_V	// Clear V bit to VBAR vector
	mcr	p15, 0, r0, c1, c0, 0	// Write value back to CP15 register (SCTLR)
	ISB				// Instruction Synchronization barrier

	/* Set VBAR vector */
	ldr	r0, =sf_vector_table
	mcr	p15, 0, r0, c12, c0, 0

	/* Set stack (use temporarys stack) */
	cps	#PSR_SVC		// SVC Mode
	ldr	sp, =__tmp_stack_start

	/* copy text section */
	ldr	r0, =_text_start	// r0 <- text start address 
	ldr	r1, =_text_size	// r1 <- text_size 
	ldr	r2, =_text_load	// r2 <- text load address

	add	r1, r1, #3
	and	r1, r1, #0xfffffffc	// Rounded up

	cmp	r1, #0
	beq	copy_done		// if text_size == 0
	cmp	r0, r2
	beq	copy_done		// if _text_start == _text_load(text section is not imaged)

copy_loop:
	subs	r1, r1, #16		// text_size -= 16
	ldmhsia	r2!, {r8, r9, r10, r11}	// if text_size >= 0 
	stmhsia	r0!, {r8, r9, r10, r11}	// if text_size >= 0
	bhi	copy_loop		// if text_size > 0 
	beq	copy_done		// if text_size == 0 

	add	r1, r1, #16

copy_loop2:
	subs	r1, r1, #4		// text_size -= 4
	ldmhsia	r2!, {r8}		// if text_size >= 0
	stmhsia	r0!, {r8}		// if text_size >= 0
	bhi	copy_loop2		// if text_size > 0 

copy_done:
	/* clear registers */
	mov	r0, #0
	mov	r1, #0
	mov	r2, #0
	mov	r3, #0
	mov	r5, #0
	mov	r6, #0
	mov	r7, #0
	mov	r8, #0
	mov	r9, #0
	mov	r10, #0
	mov	r11, #0
	mov	r12, #0	
	/* jump to text section on RAM */
	ldr  lr, =_text_start		// r0 <- text start address 
	bx   lr

#endif /* CPU_RZA2M */