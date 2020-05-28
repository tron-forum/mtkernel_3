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
 *	int_asm.S (RXv2)
 *	Interrupt control assembler routine
 */
#define	_in_asm_source_

#include <sys/sysdef.h>

	.section .text
	.globl Csym(knl_hll_inthdr)
	.globl Csym(knl_return_inthdr)

/* ------------------------------------------------------------------------ */
/*
 * HLL(High level programming language) Interrupt Handler
 *
 */
	.extern Csym(knl_hll_inthdr_ram)
	.extern Csym(knl_taskindp)
	.extern Csym(knl_int_nest)
	.extern Csym(ret_int_dispatch)

Csym(knl_hll_inthdr):
	/* During interrupt disable PSW.I=0 */
	pushm	r14-r15				/* R1-R2 are already saved */
	pushm	r3-r7

#if	USE_FPU
	mvfc	fpsw, r3
	push.l	r3
#endif

	mov.l	#Csym(knl_taskindp), r6		/* enter task independent mode */
	mov.l	[r6], r7
	add	#1, r7
	mov.l	r7, [r6]

	mov.l	#Csym(knl_int_nest), r4		/* interrupt nest count */
	mov.l	[r4], r3
	mov.l	r3, r6
	add	#1, r3
	mov.l	r3, [r4]

	cmp	#0, r6
	bne	l_no_change_sp			/* multiple interrupt */
	mov.l	r0, r3
	mov.l	#INTSTACK_TOP, r0		/* change to ISP */
	mov.l	r3, [-r0]			/* SSP save */
l_no_change_sp:

	setpsw	I				/* Interrupt enable */

#if USE_STATIC_IVT
	mov.l	#Csym(knl_hll_inthdr_rom), r2	/* call knl_hll_inthdr_rom[n](dintno) */
#else
	mov.l	#Csym(knl_hll_inthdr_ram), r2	/* call knl_hll_inthdr_rom[n](dintno) */
#endif
	mov.l	[r1, r2], r4			/* r1 is dintno. */
	jsr	r4

	clrpsw	I				/* Interrupt disable */

	; During interrupt disable PSW.I=0
	cmp	#0, r6
	bne	l_no_change_sp2			/* multiple interrupt */

	mov.l	[r0+], r3			/* r3 = SSP */
	mov.l	r3, r0
l_no_change_sp2:
	sub	#1, r7
	mov.l	#Csym(knl_taskindp), r6
	mov.l	r7, [r6]

#if	USE_FPU
	pop	r3
	mvtc	r3, fpsw
#endif
	popm	r3-r7
	popm	r14-r15

	/* bra  Cym(knl_return_inthdr) */

/* ------------------------------------------------------------------------ */
/*
 * knl_return_inthdr()  Called from tk_ret_int call.
 *    This call can be called only from a handler with the assembler attribute.
 */
Csym(knl_return_inthdr):
	/* During interrupt disable PSW.I=0 */
	mov.l	#Csym(knl_int_nest), r1		/* Is it a nesting interrupt? */
	mov.l	[r1], r2
	sub	#1, r2
	mov.l	r2, [r1]
	bne	l_nodispatch

	mov.l	#Csym(knl_dispatch_disabled), r1	/* Is it during dispatch disable? */
	mov.l	[r1], r1
	cmp	#0, r1
	bne	l_nodispatch

	mov.l	#Csym(knl_ctxtsk), r1		/* Is dispatch required? */
	mov.l	[r1], r1
	mov.l	#Csym(knl_schedtsk), r2
	mov.l	[r2], r2
	cmp	r1, r2
	beq	l_nodispatch

	popm	r1-r2				/* R1, R2 restore */
	bra	Csym(ret_int_dispatch)		/* To dispatch processing */

l_nodispatch:					/* Dispatch not required */
	popm	r1-r2				/* R1, R2 restore */
	rte

	.end

#endif 	/* CPU_CORE_RXV2 */
