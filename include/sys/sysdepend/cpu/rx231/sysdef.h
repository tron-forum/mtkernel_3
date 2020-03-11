/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03.
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
#define CMCR_DIV	(8)		/* PCLK/CMCR_DIV */
#define CMCR_CKS_PLCK8	(0x0000)	/* CMCR.CKS PCLK/8 */

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
#define N_INTVEC		256	/* Number of Interrupt vectors */

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

#define MPC_PWPR		0x008C11FUL
#define	MPC_PWMR_PFSWE		(0x40)
#define	MPC_PWMR_B0WI		(0x80)

#define	MPC_PB0PFS		0x0008C198UL
#define	MPC_PB1PFS		0x0008C199UL

/* ------------------------------------------------------------------------ */
/*
 * Port mode
 */
#define	PORT0_PMR		0x0008C060UL
#define	PORT1_PMR		0x0008C061UL
#define	PORT2_PMR		0x0008C062UL
#define	PORT3_PMR		0x0008C063UL
#define	PORT4_PMR		0x0008C064UL
#define	PORT5_PMR		0x0008C065UL
#define	PORTA_PMR		0x0008C06AUL
#define	PORTB_PMR		0x0008C06BUL
#define	PORTC_PMR		0x0008C06CUL
#define	PORTD_PMR		0x0008C06DUL
#define	PORTE_PMR		0x0008C06EUL
#define	PORTH_PMR		0x0008C071UL
#define	PORTJ_PMR		0x0008C072UL


/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU			1
#define CPU_HAS_DPS			1

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
