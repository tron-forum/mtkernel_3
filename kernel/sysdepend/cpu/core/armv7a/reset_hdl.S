/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06A
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/03.
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
#include <config.h>
#include <sys/sysdef.h>
#include <sys/knldef.h>

	.section    RESET_HANDLER, "ax"
	.arm	

/* ---------------------------------------------------------------------------------
 *
 *	Reset Handler
 */
	.weak start
start:
	.global  Csym(Reset_Handler)

	.extern  Csym(startup_clock)
	.extern  Csym(vector_table)
	.extern  Csym(knl_init_ttb)
	.extern  Csym(reset_main)

Csym(Reset_Handler):
	/* Disable IRQ */
	mrs	r0, cpsr
	orr	r0, r0, #PSR_DI
	msr	cpsr_c, r0	

	/* Disable cache & MMU */
	MRC	p15, 0, r0, c1, c0, 0	// Read CP15 System Control register (SCTLR)
	BIC	r0, r0, #CP15_SCTLR_I	// Clear I bit to disable I Cache
	BIC	r0, r0, #CP15_SCTLR_C	// Clear C bit to disable D Cache
	BIC	r0, r0, #CP15_SCTLR_M	// Clear M bit to disable MMU
	BIC	r0, r0, #CP15_SCTLR_V	// Clear V bit to VBAR vector
	MCR	p15, 0, r0, c1, c0, 0	// Write value back to CP15 System Control register (SCTLR)
	ISB				// Instruction  Synchronization  barrier

	/* Set VBAR */
	ldr	r0, =Csym(vector_table)
	mcr	p15, 0, r0, c12, c0, 0	

	/* Set stack pointer */
	cps	#PSR_SVC		// SVC Mode  **** uT-Kernel typically use SVC mode ****
	ldr	sp, =__tmp_stack_start

	/* CPG settings */
//	bl	Csym(startup_clock)	// (CPU depended part)/cpu_clock.c

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
	bl	init_ttb

	/* domain access control register */
	mrc	p15, 0, r0, c3, c0, 0	// Read CP15 Domain Access Control Register (DACR)
	ldr	r0, =0x55555555		// Initialize every domain entry to b01 (client)
	mcr	p15, 0, r0, c3, c0, 0	// Write CP15 Domain Access Control Register (DACR)

	/* enable MMU (cache not yet) */
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
	bic	r0, r0, #(3 << 30)	// clear ASEDIS/D32DIS bit    (SIMD extension enable/d16-d31 access enable)
	mcr	p15, 0, r0, c1, c0, 2	// write CP15 Coprocessor Access Control Register(CPACR)
	isb				// Instruction  Synchronization  barrier
	mov	r0, #0x40000000		// enable  VFP
	vmsr	fpexc, r0		// Write Floating-Point Exception Control register(FPEXC)
#endif // USE_FPU

	bl	Csym(reset_main)	// Jump to Reset handler Main routine (reset_main.c)

terminate:				// infinite loop
	b	terminate



/* ---------------------------------------------------------------------------------
 *	TTB(translation table base) initialize
 */
#define	PAGESIZE_SECTION	0x00100000	/* 1MB size */
#define	ADDRMASK_SECTION	0xfff00000	/* Bit Mask for 1MB page address. */

	.extern	Csym(knl_ttb_ini)	// (CPU depended part)/ttb_ini.c
init_ttb:
	mov	r0, #0x0
	mcr	p15, 0, r0, c2, c0, 2	// Write CP15 TTBCR

	// Set the base address of the page table in TTB register 0
	ldr	r0, =__ttb_area_top
	mov	r1, #0x08		// RGN=b01  (outer cacheable write-back cached, write allocate)
					// S=0      (translation table walk to non-shared memory)
	orr	r1, r1, #0x40		// IRGN=b01 (inner cacheability for the translation table walk is Write-back Write-allocate)
	orr	r0, r0, r1
	mcr	p15, 0, r0, c2, c0, 0	// Write CP15 TTBR0 (Translation Table Base Register 0)

	// Clear descriptor area
	mov	r2, #0
	mov	r3, #0
	mov	r4, #0
	mov	r5, #0
	mov	r6, #0
	mov	r7, #0
	mov	r8, #0
	mov	r9, #0

	ldr	r0, =__ttb_area_top
	ldr	r1, =__ttb_area_bottom
desc_clr_loop:
	stmia	r0!, {r2-r9}	// ZERO clear
	cmp	r0, r1
	blo	desc_clr_loop

	// Set descriptor area
	ldr	r0, =__ttb_area_top
	ldr	r9, =Csym(knl_ttb_ini)
desc_set_loop:
	ldr	r1, [r9, #0x00]	// address
	ldr	r2, [r9, #0x04]	// size
	ldr	r3, [r9, #0x08]	// attribute
	cmp	r2, #0		// if size == 0
	bxeq	lr
	add	r9, #0x10	// update pointer

	ldr	r5, =ADDRMASK_SECTION	// Mask address
	and	r1, r1, r5
	add	r4, r0, r1, lsr #(20-2)	// r4: Descriptor address(base+Logical address/1M*4)

loop_secdesc:
	orr	r6, r1, r3		// Descriptor Set value
	str	r6, [r4], #4		// Write setting value (with pointer update)

	add	r1, r1, #PAGESIZE_SECTION	// Address update
	subs	r2, r2, #PAGESIZE_SECTION	// Remaining size update
	bhi	loop_secdesc			//　Loop if size is greater than 0

	b	desc_set_loop

/* ---------------------------------------------------------------------------------
 *	L1 D/I cache enable
 */
	.global	Csym(L1CacheInit)
Csym(L1CacheInit): 
	// enable MMU, D/I-cache, branch prediction
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

/* ---------------------------------------------------------------------------------
 *	Temporary Stack Area 
 */
	.section	.tmp_stack_section, "aw", %nobits
	.global	__tmp_stack
__tmp_stack:
	.space	TMP_STACK_SIZE

#endif	/* CPU_CORE_ARMV7A */