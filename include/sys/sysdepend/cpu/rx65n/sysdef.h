/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06A
 *
 *    Copyright (C) 2023 by Ken Sakamura.
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
 *	System dependencies definition (RX65N CPU depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__


/* CPU Core-dependent definition */
#include "../core/rxv2/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)  0x00000000 - 0x003FFFF (256KB)
 *
 */
#define INTERNAL_RAM_SIZE	0x0003FFFF

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
 * RTC control 
 *
 */

#define	RTC_RCR1	(0x0008C422)
#define	RTC_RCR2	(0x0008C424)
#define	RTC_RCR3	(0x0008C426)
#define	RTC_RCR4	(0x0008C428)

/* ------------------------------------------------------------------------ */
/*
 * System Timer clock
 *	Use CMT0: Compare match timer ch.0
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
 * Choosing to enable software configurable interrupt processing
 *	If "FALSE" is selected, the OS does not process software configurable interrupts.
 */
#define USE_SFTCNF_INT		(TRUE)

/*
 * Choosing to enable group interrupt processing
 *	If "FALSE" is selected, the OS does not process group interrupts.
 */
#define USE_GROUP_INT		(TRUE)

/*
 * ICU (Interrupt Controller) register
 */
#define ICU_IR_BASE		0x00087000UL		/* Interrupt request reg. */
#define ICU_IR(x)		(ICU_IR_BASE + (x))
#define	ICU_IR_IR		(0x01)

#define ICU_IER_BASE		0x00087200UL		/* Interrupt enable reg. */
#define ICU_IER(x)		(ICU_IER_BASE + ((x) >> 3))
#define ICU_IER_IEN(x)		(0x01 << ((x) & 7))

#define ICU_IPR_BASE		0x00087300UL		/* Interrupt priority reg. */
#define ICU_IPR(x)		(ICU_IPR_BASE + (x))

#define ICU_IRQCR_BASE		0x00087500UL		/* Interrupt request control reg. */
#define ICU_IRQCR(x)		(ICU_IRQCR_BASE + (x))

/* Software Configurable Interrupt factor reg. */
#define	ICU_SLIBXR_BASE		(0x00087780UL)		/* n = 128 ~ 143 */
#define	ICU_SLIBR_BASE		(0x00087790UL)		/* n = 144 ~ 207 */
#define	ICU_SLIAR_BASE		(0x000879D0UL)		/* n = 208 ~ 255 */

#define ICU_SLIBXR(n)		(ICU_SLIBXR_BASE + (n-128))
#define ICU_SLIBR(n)		(ICU_SLIBR_BASE + (n-144))
#define ICU_SLIAR(n)		(ICU_SLIAR_BASE + (n-208))

#define	ICU_SLIPRCR		(0x00087A00UL)

/* Group interrupts reg. */
#define	ICU_GRPBE0		(0x00087600UL)
#define	ICU_GRPBL0		(0x00087630UL)
#define	ICU_GRPBL1		(0x00087634UL)
#define	ICU_GRPBL2		(0x00087638UL)
#define	ICU_GRPAL0		(0x00087830UL)
#define	ICU_GRPAL1		(0x00087834UL)

#define ICU_GENBE0		(0x00087640UL)
#define ICU_GENBL0		(0x00087670UL)
#define ICU_GENBL1		(0x00087674UL)
#define ICU_GENBL2		(0x00087678UL)
#define ICU_GENAL0		(0x00087870UL)
#define ICU_GENAL1		(0x00087874UL)

#define	ICU_GCRBE0		(0x00087680UL)

#define INTNO_GROUPBE0		106
#define INTNO_GROUPBL0		110
#define INTNO_GROUPBL1		111
#define INTNO_GROUPBL2		117
#define INTNO_GROUPAL0		112
#define INTNO_GROUPAL1		113

#define INTPRI_GROUPBE0		5
#define INTPRI_GROUPBL0		5
#define INTPRI_GROUPBL1		5
#define INTPRI_GROUPBL2		5
#define INTPRI_GROUPAL0		5
#define INTPRI_GROUPAL1		5

/*
 * Number of Interrupt vectors
 */

#define	N_INTVEC0	256	/* INTVEC0 is the number of interrupts other than group interrupts */

#if USE_GROUP_INT

	#define N_GROUP_INT		(32*6)	/* Number of factors for group interrupts */
	#define N_INTVEC		(N_INTVEC0 + N_GROUP_INT)
	
	#define INTNO_GROUP_TOP		(N_INTVEC0)
	#define	INTNO_GROUP_BE0		(INTNO_GROUP_TOP)
	#define	INTNO_GROUP_BL0		(INTNO_GROUP_TOP + 32)
	#define	INTNO_GROUP_BL1		(INTNO_GROUP_TOP + 64)
	#define	INTNO_GROUP_BL2		(INTNO_GROUP_TOP + 96)
	#define	INTNO_GROUP_AL0		(INTNO_GROUP_TOP + 128)
	#define	INTNO_GROUP_AL1		(INTNO_GROUP_TOP + 160)
#else
	#define	N_INTVEC		(N_INTVEC0)

#endif /* USE_GROUP_INT */

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
#define MPC_P6nPFS(n)		(0x0008C170+n)
#define MPC_P7nPFS(n)		(0x0008C178+n)
#define MPC_P8nPFS(n)		(0x0008C180+n)
#define MPC_P9nPFS(n)		(0x0008C188+n)
#define MPC_PAnPFS(n)		(0x0008C190+n)
#define MPC_PBnPFS(n)		(0x0008C198+n)
#define MPC_PCnPFS(n)		(0x0008C1A0+n)
#define MPC_PDnPFS(n)		(0x0008C1A8+n)
#define MPC_PEnPFS(n)		(0x0008C1B0+n)
#define MPC_PFnPFS(n)		(0x0008C1B8+n)
#define MPC_PJnPFS(n)		(0x0008C1D0+n)

#define	MPC_PFS_ASEL		(1<<7)
#define	MPC_PFS_ISEL		(1<<6)
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
#define	PORT6_PDR		(0x0008C006)
#define	PORT7_PDR		(0x0008C007)
#define	PORT8_PDR		(0x0008C008)
#define	PORT9_PDR		(0x0008C009)
#define	PORTA_PDR		(0x0008C00A)
#define	PORTB_PDR		(0x0008C00B)
#define	PORTC_PDR		(0x0008C00C)
#define	PORTD_PDR		(0x0008C00D)
#define	PORTE_PDR		(0x0008C00E)
#define	PORTF_PDR		(0x0008C00F)
#define	PORTG_PDR		(0x0008C010)
#define	PORTJ_PDR		(0x0008C012)

/* Port output data register */
#define	PORT0_PODR		(0x0008C020)
#define	PORT1_PODR		(0x0008C021)
#define	PORT2_PODR		(0x0008C022)
#define	PORT3_PODR		(0x0008C023)
#define	PORT4_PODR		(0x0008C024)
#define	PORT5_PODR		(0x0008C025)
#define	PORT6_PODR		(0x0008C026)
#define	PORT7_PODR		(0x0008C027)
#define	PORT8_PODR		(0x0008C028)
#define	PORT9_PODR		(0x0008C029)
#define	PORTA_PODR		(0x0008C02A)
#define	PORTB_PODR		(0x0008C02B)
#define	PORTC_PODR		(0x0008C02C)
#define	PORTD_PODR		(0x0008C02D)
#define	PORTE_PODR		(0x0008C02E)
#define	PORTF_PODR		(0x0008C02F)
#define	PORTG_PODR		(0x0008C030)
#define	PORTJ_PODR		(0x0008C032)

/* Port input data register */
#define	PORT0_PIDR		(0x0008C040)
#define	PORT1_PIDR		(0x0008C041)
#define	PORT2_PIDR		(0x0008C042)
#define	PORT3_PIDR		(0x0008C043)
#define	PORT4_PIDR		(0x0008C044)
#define	PORT5_PIDR		(0x0008C045)
#define	PORT6_PIDR		(0x0008C046)
#define	PORT7_PIDR		(0x0008C047)
#define	PORT8_PIDR		(0x0008C048)
#define	PORT9_PIDR		(0x0008C049)
#define	PORTA_PIDR		(0x0008C04A)
#define	PORTB_PIDR		(0x0008C04B)
#define	PORTC_PIDR		(0x0008C04C)
#define	PORTD_PIDR		(0x0008C04D)
#define	PORTE_PIDR		(0x0008C04E)
#define	PORTF_PIDR		(0x0008C04F)
#define	PORTG_PIDR		(0x0008C050)
#define	PORTJ_PIDR		(0x0008C052)

/* Port mode register */
#define	PORT0_PMR		(0x0008C060)
#define	PORT1_PMR		(0x0008C061)
#define	PORT2_PMR		(0x0008C062)
#define	PORT3_PMR		(0x0008C063)
#define	PORT4_PMR		(0x0008C064)
#define	PORT5_PMR		(0x0008C065)
#define	PORT6_PMR		(0x0008C066)
#define	PORT7_PMR		(0x0008C067)
#define	PORT8_PMR		(0x0008C068)
#define	PORT9_PMR		(0x0008C069)
#define	PORTA_PMR		(0x0008C06A)
#define	PORTB_PMR		(0x0008C06B)
#define	PORTC_PMR		(0x0008C06C)
#define	PORTD_PMR		(0x0008C06D)
#define	PORTE_PMR		(0x0008C06E)
#define	PORTF_PMR		(0x0008C06F)
#define	PORTG_PMR		(0x0008C070)
#define	PORTJ_PMR		(0x0008C072)

/* Open drain control register 0 */
#define	PORT0_ODR0		(0x0008C080)
#define	PORT1_ODR0		(0x0008C082)
#define	PORT2_ODR0		(0x0008C084)
#define	PORT3_ODR0		(0x0008C086)
#define	PORT4_ODR0		(0x0008C088)
#define	PORT5_ODR0		(0x0008C08A)
#define	PORT6_ODR0		(0x0008C08C)
#define	PORT7_ODR0		(0x0008C08E)
#define	PORT8_ODR0		(0x0008C090)
#define	PORT9_ODR0		(0x0008C092)
#define	PORTA_ODR0		(0x0008C094)
#define	PORTB_ODR0		(0x0008C096)
#define	PORTC_ODR0		(0x0008C098)
#define	PORTD_ODR0		(0x0008C09A)
#define	PORTE_ODR0		(0x0008C09C)
#define	PORTF_ODR0		(0x0008C09E)
#define	PORTG_ODR0		(0x0008C0A0)
#define	PORTJ_ODR0		(0x0008C0A4)

/* Open drain control register 1 */
#define	PORT0_ODR1		(0x0008C081)
#define	PORT1_ODR1		(0x0008C083)
#define	PORT2_ODR1		(0x0008C085)
#define	PORT3_ODR1		(0x0008C087)
#define	PORT4_ODR1		(0x0008C089)
#define	PORT5_ODR1		(0x0008C08B)
#define	PORT6_ODR1		(0x0008C08D)
#define	PORT7_ODR1		(0x0008C08F)
#define	PORT8_ODR1		(0x0008C091)
#define	PORT9_ODR1		(0x0008C093)
#define	PORTA_ODR1		(0x0008C095)
#define	PORTB_ODR1		(0x0008C097)
#define	PORTC_ODR1		(0x0008C099)
#define	PORTD_ODR1		(0x0008C09B)
#define	PORTE_ODR1		(0x0008C09D)
#define	PORTF_ODR1		(0x0008C09F)
#define	PORTG_ODR1		(0x0008C0A1)
#define	PORTJ_ODR1		(0x0008C0A5)

/* Pull-up control register */
#define	PORT0_PCR		(0x0008C0C0)
#define	PORT1_PCR		(0x0008C0C1)
#define	PORT2_PCR		(0x0008C0C2)
#define	PORT3_PCR		(0x0008C0C3)
#define	PORT4_PCR		(0x0008C0C4)
#define	PORT5_PCR		(0x0008C0C5)
#define	PORT6_PCR		(0x0008C0C6)
#define	PORT7_PCR		(0x0008C0C7)
#define	PORT8_PCR		(0x0008C0C8)
#define	PORT9_PCR		(0x0008C0C9)
#define	PORTA_PCR		(0x0008C0CA)
#define	PORTB_PCR		(0x0008C0CB)
#define	PORTC_PCR		(0x0008C0CC)
#define	PORTD_PCR		(0x0008C0CD)
#define	PORTE_PCR		(0x0008C0CE)
#define	PORTF_PCR		(0x0008C0CF)
#define	PORTG_PCR		(0x0008C0D0)
#define	PORTJ_PCR		(0x0008C0D2)

/* Port switching register */
#define	PORT_PSRA		(0x0008C121)
#define	PORT_PSRB		(0x0008C120)

/* Drive capacity control register */
#define	PORT1_DSCR		(0x0008C0E0)
#define	PORT0_DSCR		(0x0008C0E1)
#define	PORT2_DSCR		(0x0008C0E2)
#define	PORT5_DSCR		(0x0008C0E5)
#define	PORT7_DSCR		(0x0008C0E7)
#define	PORT8_DSCR		(0x0008C0E8)
#define	PORT9_DSCR		(0x0008C0E9)
#define	PORTA_DSCR		(0x0008C0EA)
#define	PORTB_DSCR		(0x0008C0EB)
#define	PORTC_DSCR		(0x0008C0EC)
#define	PORTD_DSCR		(0x0008C0ED)
#define	PORTE_DSCR		(0x0008C0EE)
#define	PORTG_DSCR		(0x0008C0F0)
#define	PORTJ_DSCR		(0x0008C0F2)

/* Drive capacity control register 2 */
#define	PORT0_DSCR2		(0x0008C128)
#define	PORT1_DSCR2		(0x0008C129)
#define	PORT2_DSCR2		(0x0008C12A)
#define	PORT3_DSCR2		(0x0008C12B)
#define	PORT5_DSCR2		(0x0008C12D)
#define	PORT7_DSCR2		(0x0008C12F)
#define	PORT8_DSCR2		(0x0008C130)
#define	PORT9_DSCR2		(0x0008C131)
#define	PORTA_DSCR2		(0x0008C132)
#define	PORTB_DSCR2		(0x0008C133)
#define	PORTC_DSCR2		(0x0008C134)
#define	PORTD_DSCR2		(0x0008C135)
#define	PORTE_DSCR2		(0x0008C136)
#define	PORTG_DSCR2		(0x0008C138)
#define	PORTJ_DSCR2		(0x0008C13A)

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

/* TMR interrupt factor number */
#define INTFN_CMIA0		3
#define INTFN_CMIB0		4
#define INTFN_OVI0		5
#define INTFN_CMIA1		6
#define INTFN_CMIB1		7
#define INTFN_OVI1		8
#define INTFN_CMIA2		9
#define INTFN_CMIB2		10
#define INTFN_OVI2		11
#define INTFN_CMIA3		12
#define INTFN_CMIB3		13
#define INTFN_OVI3		14

/* Physical timer clock */
#define TMR01_CLOCK		(0x08)		// Count PCLK
#define TMR23_CLOCK		(0x08)		// Count PCLK

/* Physical timer interrupt factor number
 *	Can be set between INTB128 and INTB207
 */
#define INTNO_PTMR1		128		// INTB128
#define INTNO_PTMR2		129		// INTB129

/* Physical timer interrupt priority */
#define INTPRI_PTMR1		5
#define INTPRI_PTMR2		5

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
