/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (RA8M1 depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

#include "../../../machine.h"

/* CPU Core-dependent definition */
#include "../core/armv8m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */
/* RA8M1 Internal SRAM   0x22000000 - 0x220DFFFF  (Size 896KB)     */
#if TRUSTZONE_SECURE		// Secure
	#define INTERNAL_RAM_START      0x22000000
#elif TRUSTZONE_NONSECURE	// Non-Secure
	#define INTERNAL_RAM_START      0x32000000
#endif

#define INTERNAL_RAM_SIZE       0x000E0000

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

/* ------------------------------------------------------------------------ */
/*
 * Module Stop (MSTP)
 */
#if TRUSTZONE_SECURE		// Secure
	#define MSTP_BASE		0x40203000
#elif TRUSTZONE_NONSECURE	// Non-Secure
	#define MSTP_BASE		0x50203000
#endif

#define MSTP_CRA		(MSTP_BASE+0x000)
#define MSTP_CRB		(MSTP_BASE+0x004)
#define MSTP_CRC		(MSTP_BASE+0x008)
#define MSTP_CRD		(MSTP_BASE+0x00C)
#define MSTP_CRE		(MSTP_BASE+0x010)

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Control Unit (ICU)
 */
#if TRUSTZONE_SECURE		// Secure
	#define ICU_BASE		0x4000C000
#elif TRUSTZONE_NONSECURE	// Non-Secure
	#define ICU_BASE		0x5000C000
#endif

#define ICU_IELSR(n)		(ICU_BASE+0x300+0x4*(n))
#define ICU_IELSR_IR		(1<<16)

/* ------------------------------------------------------------------------ */
/*
 * System Timer clock
 */

/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define	N_SYSVEC		16	/* Number of System Exceptions */
#define N_INTVEC		96	/* Number of Interrupt vectors */

/*
 * Exception vector table alignment
*/
#define	EXCTBL_ALIGN		512

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		4

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		1	/* SysTick */
#define INTPRI_PENDSV		15	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU		1
#define CPU_HAS_DSP		0

/*
 *  Number of coprocessors to use. Depends on user configuration
 */
#if USE_FPU
#define NUM_COPROCESSOR		1
#else
#define NUM_COPROCESSOR		0
#endif


/* ------------------------------------------------------------------------ */
/*
 * Physical timer
 */
#define	CPU_HAS_PTMR	(1)

/* Timer register definition */
#if TRUSTZONE_SECURE		// Secure
	#define	AGT0_BASE	0x40221000
	#define	AGT1_BASE	0x40221100
#elif TRUSTZONE_NONSECURE	// Non-Secure
	#define	AGT0_BASE	0x50221000
	#define	AGT1_BASE	0x50221100
#endif

#define	AGTxAGT		0x00
#define AGTxCMA		0x02
#define AGTxCMB		0x04
#define AGTxCR		0x08
#define	AGTxMR1		0x09
#define AGTxMR2		0x0A
#define AGTxIOC		0x0C
#define AGTxISR		0x0D
#define AGTxCMSR	0x0E
#define AGTxIOSEL	0x0F

#define	AGTxCR_TUND		(1<<5)
#define	AGTxCR_TSTOP		(1<<2)
#define	AGTxCR_TCSTF		(1<<1)
#define	AGTxCR_TSTART		(1<<0)
#define AGTxMR1_TMOD_TIMER	(0)
#define AGTxMR1_TCK(n)		((n)<<4)


/* Counter source */
#define AGT0_TCK_PCLKB	0
#define AGT1_TCK_PCLKB	0

/* Physical timer interrupt number */
#define INTNO_AGT0	0
#define INTNO_AGT1	1

/* Physical timer interrupt priority */
#define INTPRI_AGT0	5
#define INTPRI_AGT1	5

/* Phycail timer Maximum count */
#define PTMR_MAX_CNT16    (0x0000FFFF)
#define PTMR_MAX_CNT32    (0xFFFFFFFF)

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
