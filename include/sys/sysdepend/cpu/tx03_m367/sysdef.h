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

/*
 *	sysdef.h
 *
 *	System dependencies definition (TX03M367 depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

/*
 **** CPU core-depeneded profile (ARMv7M(ARM Cortex-M3))
 */

#include "../core/armv7m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)  0x20000800 - 0x2001FFFF
 */
#define INTERNAL_RAM_SIZE       0x0001F800
#define INTERNAL_RAM_START      0x20000800
#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END


/* ------------------------------------------------------------------------ */
/*
 * Clock Control
 */

/* Clock Generator Registers */
#define CLKCTRL_BASE		0x400F3000
#define CLKCTRL_CGSYSCR		(CLKCTRL_BASE + 0x0000)
#define CLKCTRL_CGOSCCR		(CLKCTRL_BASE + 0x0004)
#define CLKCTRL_CGSTBYCR	(CLKCTRL_BASE + 0x0008)
#define CLKCTRL_CGPLLSEL	(CLKCTRL_BASE + 0x000C)
#define CLKCTRL_CGCKSTP		(CLKCTRL_BASE + 0x0018)
#define CLKCTRL_CGPROTECT	(CLKCTRL_BASE + 0x003C)

#define CLKCTRL_CGIMCG		(CLKCTRL_BASE + 0x0040)
#define CLKCTRL_CGIMCGA		(CLKCTRL_BASE + 0x0040)
#define CLKCTRL_CGIMCGB		(CLKCTRL_BASE + 0x0044)
#define CLKCTRL_CGIMCGC		(CLKCTRL_BASE + 0x0048)
#define CLKCTRL_CGIMCGD		(CLKCTRL_BASE + 0x004C)
#define CLKCTRL_CGICRCG		(CLKCTRL_BASE + 0x0060)

/* PLL Selection */
#define	CLKCTRL_CGPLLSEL_4X		0x00005A0F
#define	CLKCTRL_CGPLLSEL_6X		0x00006296

#define CLKCTRL_CGPLLSEL_PLLSEL		0x00000001

/* Oscillation control */
#define CLKCTRL_CGOSCCR_ALWAYSWRITE	0x00000030

#define CLKCTRL_CGOSCCR_WUEON		0x00000001
#define CLKCTRL_CGOSCCR_WUEF		0x00000002
#define CLKCTRL_CGOSCCR_PLLON		0x00000004
#define CLKCTRL_CGOSCCR_WUPSEL_XT1	0x00000008

#define CLKCTRL_CGOSCCR_WUPT(usec,freq)	( (usec * freq) >> 4 ) << 20

#define	MHz			(UW)(1000000UL)
#define HISPEED_CLOCK_MHz	12

#define PLL_MODE_4X		4
#define PLL_MODE_6X		6

#define	CLOCK_fPLL		(HISPEED_CLOCK_MHz * PLL_MODE_6X * MHz)
#define	CLOCK_fc		(CLOCK_fPLL)
#define	CLOCK_fgear		(CLOCK_fc / 2)
#define	CLOCK_fsys		(CLOCK_fgear)
#define	CLOCK_fperiph		(CLOCK_fgear)
#define	CLOCK_T0		(CLOCK_fperiph)

#define TMCLK			(CLOCK_fsys / MHz)	/* Timer clock input (MHz) */
#define TMCLK_KHz		(CLOCK_fsys / 1000)	/* Timer clock input (kHz) */

/*
 * Settable interval range (millisecond)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50


/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define N_INTVEC		128	/* Number of Interrupt vectors */
#define	N_SYSVEC		16	/* Number of System Exceptions */

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		3


/* ------------------------------------------------------------------------ */
/*
 * Interrupt Priority Levels
 */
#define INTPRI_MAX_EXTINT_PRI	1	/* Highest Ext. interrupt level */
#define INTPRI_SVC		0	/* SVCall */
#define INTPRI_SYSTICK		1	/* SysTick */
#define INTPRI_PENDSV		7	/* PendSV */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0


/* ------------------------------------------------------------------------ */
/*
 * Watchdog Timer
 */
#define WDOG_BASE		0x400F2000

#define	WDOG_WDMOD		(WDOG_BASE + 0x0000)	/* Mode Reg. */
#define	WDOG_WDCR		(WDOG_BASE + 0x0004)	/* Control Reg. */

#define	WDOG_WDMOD_WDTE		0x00000080	/* Enable */
#define	WDOG_WDCR_DISABLE	0x000000B1	/* Disable Code */


/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define	PORTA_BASE	0x400C0000
#define	PORTB_BASE	0x400C0100
#define	PORTE_BASE	0x400C0400
#define	PORTF_BASE	0x400C0500
#define	PORTG_BASE	0x400C0600
#define	PORTH_BASE	0x400C0700
#define	PORTI_BASE	0x400C0800
#define	PORTK_BASE	0x400C0A00
#define	PORTL_BASE	0x400C0B00

#define	PORT_DATA(n)	(PORT##n##_BASE + 0x00)	// data
#define	PORT_CR(n)	(PORT##n##_BASE + 0x04)	// Output Control
#define	PORT_FR1(n)	(PORT##n##_BASE + 0x08)	// Function 1
#define	PORT_FR2(n)	(PORT##n##_BASE + 0x0C)	// Function 2
#define	PORT_FR3(n)	(PORT##n##_BASE + 0x10)	// Function 3
#define	PORT_FR4(n)	(PORT##n##_BASE + 0x14)	// Function 4
#define	PORT_FR5(n)	(PORT##n##_BASE + 0x18)	// Function 5
#define	PORT_FR6(n)	(PORT##n##_BASE + 0x1C)	// Function 6
#define	PORT_OD(n)	(PORT##n##_BASE + 0x28)	// Open Drain Control
#define	PORT_PUP(n)	(PORT##n##_BASE + 0x2C)	// Pull-up Control
#define	PORT_PDN(n)	(PORT##n##_BASE + 0x30)	// Pull-down Control
#define	PORT_IE(n)	(PORT##n##_BASE + 0x38)	// Input Control

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for TX03M67 TMRB)
 */
#define	CPU_HAS_PTMR	(1)

/* TMRB Register definition */
#define TMRB0_BASE	0x400C4000
#define TMRB1_BASE	0x400C4100
#define TMRB2_BASE	0x400C4200
#define TMRB3_BASE	0x400C4300
#define TMRB4_BASE	0x400C4400
#define TMRB5_BASE	0x400C4500
#define TMRB6_BASE	0x400C4600
#define TMRB7_BASE	0x400C4700

#define TBxEN		0x0000
#define TBxRUN		0x0004
#define	TBxCR		0x0008
#define	TBxMOD		0x000C
#define TBxFFCR		0x0010
#define TBxST		0x0014
#define TBxIM		0x0018
#define TBxUC		0x001C
#define TBxRG0		0x0020
#define TBxRG1		0x0024
#define TBxCP0		0x0028
#define TBxCP1		0x002C

#define TBxEN_TBEN	(1<<7)
#define TBxRUN_TBRUN	(1<<0)
#define TBxRUN_TBPRUN	(1<<2)
#define	TBxFFCR_TDE	(0x000000C3)
#define TBxMOD_TBCLE	(1<<3)
#define TBxIM_TBIMOF	(1<<2)
#define TBxIM_TBIM1	(1<<1)
#define TBxIM_TBIM0	(1<<0)

/* Physical timer clock */
#define	TB0MOD_TBCLK	(0x00000001)    // Source clock = T1
#define	TB1MOD_TBCLK	(0x00000001)    // Source clock = T1
#define	TB2MOD_TBCLK	(0x00000001)    // Source clock = T1
#define	TB3MOD_TBCLK	(0x00000001)    // Source clock = T1
#define	TB4MOD_TBCLK	(0x00000001)    // Source clock = T1
#define	TB5MOD_TBCLK	(0x00000001)    // Source clock = T1
#define	TB6MOD_TBCLK	(0x00000001)    // Source clock = T1
#define	TB7MOD_TBCLK	(0x00000001)    // Source clock = T1

/* Physical timer interrupt number */
#define INTNO_TMRB0	75
#define INTNO_TMRB1	78
#define INTNO_TMRB2	81
#define INTNO_TMRB3	84
#define INTNO_TMRB4	87
#define INTNO_TMRB5	90
#define INTNO_TMRB6	93
#define INTNO_TMRB7	96

/* Physical timer interrupt priority */
#define INTPRI_TMRB0	5
#define INTPRI_TMRB1	5
#define INTPRI_TMRB2	5
#define INTPRI_TMRB3	5
#define INTPRI_TMRB4	5
#define INTPRI_TMRB5	5
#define INTPRI_TMRB6	5
#define INTPRI_TMRB7	5

/* Phycail timer Maximum count */
#define PTMR_MAX_CNT    (0x0000FFFF)


/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU			0
#define CPU_HAS_DSP			0

/*
 *  Number of coprocessors to use. 
 *    There is no coprocessor for this microcomputer.
 */
#define NUM_COPROCESSOR		0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
