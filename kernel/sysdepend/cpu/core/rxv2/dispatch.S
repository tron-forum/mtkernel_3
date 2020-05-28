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
 *	dispatch.S (RXv2 core)
 *	Dispatcher
 */

#define	_in_asm_source_

#include <sys/machine.h>
#include <tk/errno.h>
#include <sys/sysdef.h>
#include <sys/knldef.h>

#include "offset.h"

	.extern	Csym(knl_tmp_stack)
	.extern	Csym(knl_dispatch_disabled)
	.extern	Csym(knl_ctxtsk)
	.extern	Csym(knl_schedtsk)
	.extern	Csym(knl_lowpow_discnt)
	.extern	Csym(low_pow)

/* ------------------------------------------------------------------------*/
/*
 * Dispatcher
 *	dispatch_to_schedtsk:
 *		Destroys the current context and forces dispatch to "schedtsk".
 *			Called directly by a jump (jmp), but does not return.
 *			Called with an undefined stack state (undefined 'ssp').
 *			Called when interrupts are disabled.
 *	dispatch_entry:
 *		Normal dispatch processing.
 *	ret_int_dispatch:
 *		Called when dispatch is required with "tk_ret_int ()".
 *
 *	Contexts to save
 *		Saves registers other than SP to the stack. Save the SP to TCB.
 *
 *		   High Address	+---------------+
 *				| SPSW(32bit)	| Saved by interrupt entry
 *				| SPC(32bit)	| routines until here.
 *				+---------------+
 *				| R15		|
 *				| R14		|
 *				| R13		|
 *				|  :		|
 *				| R3		|
 *				| R2		|
 *	Saved to TCB  SP =>	| R1		|-> If USE_FPU,	  | R1		  |-> If USE_DSP,  | R1		   |
 *		    Low Address	+---------------+	 SP =>	  | FPSW	  |		   | ACC1	   |
 *						      Low Address +---------------+	SP =>	   | ACC0	   |
 *										       Low Address +---------------+
 */
	.globl Csym(knl_dispatch_to_schedtsk)
	.globl Csym(knl_dispatch_entry)
	.globl Csym(ret_int_dispatch)

	.section .text
Csym(knl_dispatch_to_schedtsk):				// Forces dispatch entry
	; During interrupt disable (PSW.I=0)
	mov.l	#(Csym(knl_tmp_stack) + TMP_STACK_SIZE), r0	// Set temporal stack

	mov.l	#Csym(knl_dispatch_disabled), r1
	mov.l	#1, [r1]				// Dispatch disable

	mov.l	#Csym(knl_ctxtsk), r7			
#if USE_DBGSPT != 0
	mov.l	[r7], r2				// r2 := ctxtsk
#endif
	mov.l	#0, [r7]				// ctxtsk = NULL
	mvtipl	#0					// IPL = 0 (Lowest level)
	bra	l_dispatch1

Csym(knl_dispatch_entry):				// Normal dispatch entry
	// During interrupt disable (PSW.I=0).
	mvfc	psw, r1
	bset	#16, r1					// PSW compensation (I=1)
	xchg	[r0], r1
	push.l	r1

Csym(ret_int_dispatch):					// tk_ret_int() dispatch entry
	// During interrupt disable (PSW.I=0).
	pushm	r14-r15
	mov.l	#Csym(knl_dispatch_disabled), r15
	mov.l	#1, [r15]				// Dispatch disable
	pushm	r1-r13					// Context save

#if USE_FPU						// Save FPU registers
	mvfc	fpsw, r1
	push.l	r1
#endif

#if USE_DSP						// Save DSP registers
	mvfaclo #0,a1,r2
	mvfachi #0,a1,r3
	mvfacgu	#0,a1,r4
	pushm	r2-r4
	mvfaclo #0,a0,r2
	mvfachi #0,a0,r3
	mvfacgu	#0,a0,r4
	pushm	r2-r4
#endif
	mov.l	#Csym(knl_ctxtsk), r7
	mov.l	[r7], r2
	mov.l	r0, (TCB_tskctxb + CTXB_ssp)[r2]	// Save SSP to ctxtsk's TCB

	mvtipl	#0				// IPL = 0 (Lowest level)
l_dispatch1:
	clrpsw	I				// Interrupt disable

	mov.l	#Csym(knl_schedtsk), r2
	mov.l	[r2], r6
	cmp	#0, r6				// ? schedtsk == NULL
	bne	l_dispatch2			//	No. goto l_dispatch2
						//	Yes.
	// Move to power saving mode because there are no tasks to perform.
	mov.l	#Csym(knl_lowpow_discnt), r1 
	mov.l	[r1], r1			// Is 'low_pow' disabled?
	cmp	#0, r1
	bne	l_dispatch11

	mov.l	#Csym(low_pow), r1
	jsr	r1				// Move to power saving mode. call low_pow().
l_dispatch11:

	setpsw	I				// Interrupt enable
	bra	l_dispatch1

l_dispatch2:					// Switch to 'schedtsk'
	// During interrupt disable (PSW.I=0).
	mov.l	r6, [r7]				// ctxtsk := schedtsk
	mov.l	(TCB_tskctxb + CTXB_ssp)[r6], r0	// Restore SSP from TCB

	mov.l	#Csym(knl_dispatch_disabled), r2
	mov.l	#0, [r2]				// Dispatch enable
	
#if USE_DSP					// Restore DSP register.
	popm	r2-r4
	mvtacgu	r4,a0
	mvtachi r3,a0
	mvtaclo r2,a0
	popm	r2-r4
	mvtacgu	r4,a1
	mvtachi r3,a1
	mvtaclo r2,a1
#endif

#if USE_FPU					// Restore FPU register.
	pop	r1
	mvtc	r1, fpsw
#endif

	popm	r1-r15				// Restore context.
	rte

	.end

#endif	/* CPU_RX231 */
