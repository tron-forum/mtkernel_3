/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
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
 *	reset_hdl.S  (ARMv7-A)
 *	Reset handler
 */

#define	_in_asm_source_
#include <sys/sysdef.h>

	.section    RESET_HANDLER, "ax"
	.arm	

/* ---------------------------------------------------------------------------------
 *	Reset Handler
 */
	.weak start
start:
	.global  Csym(Reset_Handler)
Csym(Reset_Handler):
	/* Disable IRQ */
	mrs	r0, cpsr
	orr	r0, r0, #PSR_DI
	msr	cpsr_c, r0	

	/* Disable cache & MMU */
	mrc	p15, 0, r0, c1, c0, 0	// Read CP15 System Control register (SCTLR)
	bic	r0, r0, #CP15_SCTLR_I	// Clear I bit to disable I Cache
	bic	r0, r0, #CP15_SCTLR_C	// Clear C bit to disable D Cache
	bic	r0, r0, #CP15_SCTLR_M	// Clear M bit to disable MMU
	bic	r0, r0, #CP15_SCTLR_V	// Clear V bit to VBAR vector
	mcr	p15, 0, r0, c1, c0, 0	// Write value back to CP15 System Control register (SCTLR)
	isb				// Instruction Synchronization barrier	

	/* Set VBAR */
	ldr	r0, =Csym(vector_table)
	mcr	p15, 0, r0, c12, c0, 0	

	/* Set stack pointer */
	cps	#PSR_SVC		// SVC Mode  **** uT-kernel use only SVC mode ****
	ldr	sp, =__tmp_stack_start

	/* CPG settings */
	bl	=Csym(startup_clock)	// (CPU depended part)/cpu_clock.c

	/* Invalidate TLB */
	mov	r0,#0
	mcr	p15, 0, r0, c8, c7, 0

	/* Invalidate I-cache, flushes BTAC */
	mov  r0, #0
	mcr  p15, 0, r0, c7, c5, 0

	/* Invalidate D-Caches */ 
	mov	r2, #4
dc_loop1:				// loop of way
	mov	r1, #256		// # of set
	sub	r0, r2, #1		// current way
	mov	r0, r0, lsl #30
dc_loop2:				// loop of set
	mcr	p15, 0, r0, c7, c6, 2	// invalidate by set/way
	add	r0, r0, #0x20
	subs	r1, r1, #1
	bne	dc_loop2
	subs	r2, r2, #1
	bne	dc_loop1

	/* TTB initialize */
	bl	=Csym(knl_init_ttb)	// (System depended part)/ttb_init.S

	// domain access control register
	mrc	p15, 0, r0, c3, c0, 0	// Read CP15 Domain Access Control Register (DACR)
	ldr	r0, =0x55555555		// Initialize every domain entry to b01 (client)
	mcr	p15, 0, r0, c3, c0, 0	// Write CP15 Domain Access Control Register (DACR)

	// enable MMU (cache not yet)	
	mrc	p15, 0, r0, c1, c0, 0	// Read CP15 System Control register (SCTLR)
	bic	r0, r0, #CP15_SCTLR_I	// Clear I bit to disable I Cache
	bic	r0, r0, #CP15_SCTLR_C	// Clear C bit to disable D Cache
	bic	r0, r0, #CP15_SCTLR_A	// Clear A bit to disable strict alignment fault checking
	orr	r0, r0, #CP15_SCTLR_M	// Set   M bit to enable MMU before scatter loading
	mcr	p15, 0, r0, c1, c0, 0	// Write CP15 System Control register (SCTLR)
	isb				// Instruction  Synchronization  barrier

#if USE_FPU
	// setting NEON/VFP
fpu_init:
	mrc	p15, 0, r0, c1, c0, 2	// Read CP15 Coprocessor Access Control Register(CPACR)
	orr	r0, r0, #(0xf << 20)	// CP10/CP11 access enable
	bic	r0, r0, #(3 << 30)	// clear ASEDIS/D32DIS bit    (SIMD extention enable/d16-d31 access enable)
	mcr	p15, 0, r0, c1, c0, 2	// write CP15 Coprocessor Access Control Register(CPACR)
	isb				// Instruction  Synchronization  barrier
	// NEON/VFP Disable
	mov	r0, #0x00000000		// clear EN bit
	vmsr	FPEXC, r0		// Write Floating-Point Exception Control register(FPEXC)
#endif // USE_FPU

	bl	Csym(reset_main)	// Jump to Reset handler Main routine (reset_main.c)

terminate:				// infinite loop
	b	terminate


/* ---------------------------------------------------------------------------------
 *	L1 D/I cache enable
 */
	.global  Csym(knl_init_l1cache)
Csym(knl_init_l1cache): 
	// enable D/I-cache, enable branch prediction
	mrc	p15, 0, r0, c1, c0, 0	// Read CP15 System Control register (SCTLR)
	orr	r0, r0, #CP15_SCTLR_I	// Set   I bit to dnable I Cache
	orr	r0, r0, #CP15_SCTLR_C	// Set   C bit to enable D Cache
	orr	r0, r0, #CP15_SCTLR_Z	// Set   Z bit to enable Program Flow Prediction
	mcr	p15, 0, r0, c1, c0, 0	// Write CP15 System Control register (SCTLR)
	isb				// Instruction  Synchronization  barrier

	// enable Dside prefetch
	mrc	p15, 0, r0, c1, c0, 1	// Read CP15 Auxiliary Control Register(ACTLR)
	orr	r0, r0, #CP15_ACTLR_DPF	// Enable Dside prefetch
	mcr	p15, 0, r0, c1, c0, 1	// Weite CP15 Auxiliary Control Register(ACTLR)
	isb				// Instruction  Synchronization  barrier

	bx	lr

/* ==================================================================
    STACK AREA 
  ================================================================== */
    .section    .tmp_stack_section, "aw", %nobits
    .global   __tmp_stack
__tmp_stack:
    .space    TMP_STACK_SIZE

    .section    .irq_stack_section, "aw", %nobits
    .global   __irq_stack
__irq_stack:
    .space    EXC_STACK_SIZE

#endif	/* CPU_CORE_ARMV7A */