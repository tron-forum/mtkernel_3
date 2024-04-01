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
 *	System dependencies definition (RX231 CPU depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__


/* CPU Core-dependent definition */
#include "../core/rxv2/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)  0x00000000 - 0x0000FFFF (or 0x00007FFF)
 *	RX231 has two types of memory size, 32KB and 64KB.
 */
#define INTERNAL_RAM_SIZE	0x0000FFFF
//#define INTERNAL_RAM_SIZE	0x00007FFF

#define INTERNAL_RAM_START	0x00000000
#define INTERNAL_RAM_END	(INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INTSTACK_TOP		INTERNAL_RAM_END+1


/* ------------------------------------------------------------------------ */
/*
 * Module Stop Control 
 */

/* Module Stop Control register */
#define	MSTPCRA		(0x00080010)
#define	MSTPCRB		(0x00080014)
#define	MSTPCRC		(0x00080018)
#define	MSTPCRD		(0x0008001C)

/* ------------------------------------------------------------------------ */
/*
 * System Clock Control 
 *     Use CMT0: Compare match timer ch.0
 */

/* CMT0 Registers */
#define CMT_BASE	0x00088000
#define CMT_STR0	(CMT_BASE + 0x0000)
#define CMT0_CR		(CMT_BASE + 0x0002)
#define CMT0_CNT	(CMT_BASE + 0x0004)
#define CMT0_COR	(CMT_BASE + 0x0006)

#define CMT_STR0_STR0	(0x0001)
#define CMT0_CR_CKS	(0x0003)
#define CMT0_CR_CMIE	(0x0040)

#define	MHz		(UW)(1000000UL)
#define EXTAL_CLOCK	(UW)(54*MHz)

#define PCLK_DIV	(2)		/* PCLK Devistion = PCLKB of SCKCR */
#define CMCR_DIV	(8)		/* CMT Clock = PCLK/CMCR_DIV */
#define CMCR_CKS_PLCK	(0x0000)	/* CMCR.CKS = PCLK/8 */

#define	SYSCLK_ICLK	(UW)(54*MHz)
#define	SYSCLK_PCLKA	(UW)(54*MHz)
#define	SYSCLK_PCLKB	(UW)(27*MHz)
#define	SYSCLK_PCLKD	(UW)(54*MHz)

/*
 * Settable interval range (milli second)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50


/*----------------------------------------------------------*/
/*
 * Interrupt Control
 */

/* 
 * RX231 does not have software configurable interrupt and group interrupt functions.
 * Both values ​​must be FALSE.
 */
#define USE_SFTCNF_INT		(FALSE)		/* Software Configurable Interrupt */
#define USE_GROUP_INT		(FALSE)		/* Group Interrupt */

/*
 * ICU (Interrupt Controller) register
 */
#define ICU_IR_BASE	0x00087000UL		/* Interrupt request reg. */
#define ICU_IR(x)	(ICU_IR_BASE + (x))
#define	ICU_IR_IR	(0x01)

#define ICU_IER_BASE	0x00087200UL		/* Interrupt enable reg. */
#define ICU_IER(x)	(ICU_IER_BASE + ((x) >> 3))
#define ICU_IER_IEN(x)	(0x01 << ((x) & 7))

#define ICU_IPR_BASE	0x00087300UL		/* Interrupt priority reg. */
#define ICU_IPR(x)	(ICU_IPR_BASE + (x))

#define ICU_IRQCR_BASE	0x00087500UL		/* Interrupt request control reg. */
#define ICU_IRQCR(x)	(ICU_IRQCR_BASE + (x))

/*
 * Number of Interrupt vectors
 */

#define	N_INTVEC0	256		/* INTVEC0 is the number of interrupts other than group interrupts */
#define N_INTVEC	(N_INTVEC0)	/* RX231 has no group interrupt, INTVEC and INTVEC0 are always equal. */

/*
 * Interrupt Priority Levels
 */
#define INTPRI_MIN_INT_PRI	1	/* Lowest interrupt level */
#define INTPRI_MAX_INT_PRI	15	/* Highest Interrupt level */

/*
 * Interrupt used by micro T-Kernel
 */
#define INTNO_SYS_DISPATCH	1	/* Dispatch (reserved)*/	
#define INTNO_SYS_SVC		2	/* System call (reserved) */
#define INTNO_SYS_RET_INT	3	/* System call : tk_ret_int (reserved) */
#define INTNO_SYS_DGSPT		4	/* Debugger support (reserved) */
#define INTNO_USER_MIN		5	/* Minimum number of user-defined interrupt */

#define INTNO_SYS_TICK		28	/* Systten timer tick */

/*
 * System timer interrupt level
 */
#define INTLEVEL_SYS_TICK	15

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0


/* ------------------------------------------------------------------------ */
/*
 * Pin function control
 */

#define MPC_PWPR		(0x0008C11F)
#define	MPC_PWMR_PFSWE		(1<<6)
#define	MPC_PWMR_B0WI		(1<<7)

#define MPC_P0nPFS(n)		(0x0008C140+n)
#define MPC_P1nPFS(n)		(0x0008C148+n)
#define MPC_P2nPFS(n)		(0x0008C150+n)
#define MPC_P3nPFS(n)		(0x0008C158+n)
#define MPC_P4nPFS(n)		(0x0008C160+n)
#define MPC_P5nPFS(n)		(0x0008C168+n)
#define MPC_PAnPFS(n)		(0x0008C190+n)
#define MPC_PBnPFS(n)		(0x0008C198+n)
#define MPC_PCnPFS(n)		(0x0008C1A0+n)
#define MPC_PDnPFS(n)		(0x0008C1A8+n)
#define MPC_PEnPFS(n)		(0x0008C1B0+n)
#define MPC_PHnPFS(n)		(0x0008C1C8+n)

#define	MPC_PFS_ASEL		(1<<7)
#define	MPC_PFS_PSEL		(0x1F)

/* ------------------------------------------------------------------------ */
/*
 * I/O Port
 */
/* Port direction register */
#define	PORT0_PDR		(0x0008C000)
#define	PORT1_PDR		(0x0008C001)
#define	PORT2_PDR		(0x0008C002)
#define	PORT3_PDR		(0x0008C003)
#define	PORT4_PDR		(0x0008C004)
#define	PORT5_PDR		(0x0008C005)
#define	PORTA_PDR		(0x0008C00A)
#define	PORTB_PDR		(0x0008C00B)
#define	PORTC_PDR		(0x0008C00C)
#define	PORTD_PDR		(0x0008C00D)
#define	PORTE_PDR		(0x0008C00E)
#define	PORTH_PDR		(0x0008C011)
#define	PORTJ_PDR		(0x0008C012)

/* Port output data register */
#define	PORT0_PODR		(0x0008C020)
#define	PORT1_PODR		(0x0008C021)
#define	PORT2_PODR		(0x0008C022)
#define	PORT3_PODR		(0x0008C023)
#define	PORT4_PODR		(0x0008C024)
#define	PORT5_PODR		(0x0008C025)
#define	PORTA_PODR		(0x0008C02A)
#define	PORTB_PODR		(0x0008C02B)
#define	PORTC_PODR		(0x0008C02C)
#define	PORTD_PODR		(0x0008C02D)
#define	PORTE_PODR		(0x0008C02E)
#define	PORTH_PODR		(0x0008C031)
#define	PORTJ_PODR		(0x0008C032)

/* Port input data register */
#define	PORT0_PIDR		(0x0008C040)
#define	PORT1_PIDR		(0x0008C041)
#define	PORT2_PIDR		(0x0008C042)
#define	PORT3_PIDR		(0x0008C043)
#define	PORT4_PIDR		(0x0008C044)
#define	PORT5_PIDR		(0x0008C045)
#define	PORTA_PIDR		(0x0008C04A)
#define	PORTB_PIDR		(0x0008C04B)
#define	PORTC_PIDR		(0x0008C04C)
#define	PORTD_PIDR		(0x0008C04D)
#define	PORTE_PIDR		(0x0008C04E)
#define	PORTH_PIDR		(0x0008C051)
#define	PORTJ_PIDR		(0x0008C052)

/* Port mode register */
#define	PORT0_PMR		(0x0008C060)
#define	PORT1_PMR		(0x0008C061)
#define	PORT2_PMR		(0x0008C062)
#define	PORT3_PMR		(0x0008C063)
#define	PORT4_PMR		(0x0008C064)
#define	PORT5_PMR		(0x0008C065)
#define	PORTA_PMR		(0x0008C06A)
#define	PORTB_PMR		(0x0008C06B)
#define	PORTC_PMR		(0x0008C06C)
#define	PORTD_PMR		(0x0008C06D)
#define	PORTE_PMR		(0x0008C06E)
#define	PORTH_PMR		(0x0008C071)
#define	PORTJ_PMR		(0x0008C072)

/* Open drain control register 0 */
#define	PORT1_ODR0		(0x0008C082)
#define	PORT2_ODR0		(0x0008C084)
#define	PORT3_ODR0		(0x0008C086)
#define	PORT5_ODR0		(0x0008C08A)
#define	PORTA_ODR0		(0x0008C094)
#define	PORTB_ODR0		(0x0008C096)
#define	PORTC_ODR0		(0x0008C098)
#define	PORTE_ODR0		(0x0008C09C)
#define	PORTJ_ODR0		(0x0008C0A4)

/* Open drain control register 1 */
#define	PORT1_ODR1		(0x0008C083)
#define	PORT2_ODR1		(0x0008C085)
#define	PORT3_ODR1		(0x0008C087)
#define	PORT5_ODR1		(0x0008C08B)
#define	PORTA_ODR1		(0x0008C095)
#define	PORTB_ODR1		(0x0008C097)
#define	PORTC_ODR1		(0x0008C099)
#define	PORTE_ODR1		(0x0008C09D)

/* Pull-up control register */
#define	PORT0_PCR		(0x0008C0C0)
#define	PORT1_PCR		(0x0008C0C1)
#define	PORT2_PCR		(0x0008C0C2)
#define	PORT3_PCR		(0x0008C0C3)
#define	PORT4_PCR		(0x0008C0C4)
#define	PORT5_PCR		(0x0008C0C5)
#define	PORTA_PCR		(0x0008C0CA)
#define	PORTB_PCR		(0x0008C0CB)
#define	PORTC_PCR		(0x0008C0CC)
#define	PORTD_PCR		(0x0008C0CD)
#define	PORTE_PCR		(0x0008C0CE)
#define	PORTH_PCR		(0x0008C0D1)
#define	PORTJ_PCR		(0x0008C0D2)

/* Port switching register */
#define	PORT_PSRA		(0x0008C121)
#define	PORT_PSRB		(0x0008C120)

/* Drive capacity control register */
#define	PORT1_DSCR		(0x0008C0E1)
#define	PORT2_DSCR		(0x0008C0E2)
#define	PORT3_DSCR		(0x0008C0E3)
#define	PORT5_DSCR		(0x0008C0E5)
#define	PORTA_DSCR		(0x0008C0EA)
#define	PORTB_DSCR		(0x0008C0EB)
#define	PORTC_DSCR		(0x0008C0EC)
#define	PORTD_DSCR		(0x0008C0ED)
#define	PORTE_DSCR		(0x0008C0EE)
#define	PORTH_DSCR		(0x0008C0F1)
#define	PORTJ_DSCR		(0x0008C0F2)


/* ------------------------------------------------------------------------ */
/*
 * Physical timer
 */
#define	CPU_HAS_PTMR	(1)

/* TMR Register definition */
#define	TMR01_BASE		0x00088200
#define	TMR23_BASE		0x00088210

#define TCR			0x0000
#define TCSR			0x0002
#define	TCORA			0x0004
#define	TCORB			0x0006
#define	TCNT			0x0008
#define	TCCR			0x000A
#define TCSTR			0x000C

#define	TCR_CCLR_CMA		(1<<3)		// Cleared by compare match A
#define	TCR_OVIE		(1<<5)		// Enable OVIn
#define	TCR_CMIEA		(1<<6)		// Enable CMIAn
#define	TCR_CMIEB		(1<<7)		// Enable CMIBn

#define TCCR_CSS_16BIT		(0x18)

/* Physical timer clock */
#define TMR01_CLOCK		(0x08)		// Count PCLK
#define TMR23_CLOCK		(0x08)		// Count PCLK

/* Physical timer interrupt number */
#define INTNO_CMIA0		174
#define INTNO_CMIB0		175
#define INTNO_OVI0		176
#define INTNO_CMIA1		177
#define INTNO_CMIB1		178
#define INTNO_OVI1		179
#define INTNO_CMIA2		180
#define INTNO_CMIB2		181
#define INTNO_OVI2		182
#define INTNO_CMIA3		183
#define INTNO_CMIB3		184
#define INTNO_OVI3		185

/* Physical timer interrupt priority */
#define INTPRI_TMR01		5
#define INTPRI_TMR23		5

/* Phycail timer Maximum count */
#define PTMR_MAX_CNT    (0x0000FFFF)


/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU			1
#define CPU_HAS_DSP			1

/*
 *  Number of coprocessors to use. Depends on user configuration
 */
#if USE_FPU && USE_DSP
#define NUM_COPROCESSOR		2
#elif USE_FPU
#define NUM_COPROCESSOR		1
#elif USE_DSP
#define NUM_COPROCESSOR		1
#else
#define NUM_COPROCESSOR		0
#endif


#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
