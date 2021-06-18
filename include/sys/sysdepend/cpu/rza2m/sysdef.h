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

/*
 *	sysdef.h
 *
 *	System dependencies definition (RZ/A2M depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__


/* CPU Core-dependent definition */
#include "../core/armv7a/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * RAM Map (Internal Memorie 0x80020000 - 0x803FFFFF)
 *       Backup RAM     0x80000000 - 0x8001FFFF
 *       Cached RAM     0x80020000 - 0x802FFFFF
 *       Uncached RAM   0x80300000 - 0x803FBFFF
 *       TTB RAM        0x803FC000 - 0c803FFFFF
 */

#define	IRAM_START_ADDR			0x80000000
#define	IRAM_SIZE			0x00400000
#define	IRAM_UNCACHED_SIZE		0x00200000

#define	IRAM_CACHED_START_ADDR		IRAM_START_ADDR
#define	IRAM_CACHED_SIZE		(IRAM_SIZE - IRAM_UNCACHED_SIZE)
#define	IRAM_CACHED_END_ADDR		(IRAM_CACHED_START_ADDR + IRAM_CACHED_SIZE)
#define	IRAM_UNCACHED_START_ADDR	IRAM_CACHED_END_ADDR

/*
 * mT-Kernel RAM area (OS uses cached RAM area.)
*/
#define INTERNAL_RAM_SIZE	0x002E0000
#define INTERNAL_RAM_START	0x80020000
#define INTERNAL_RAM_END	(INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

/* ------------------------------------------------------------------------ */
/*
 * TTB(translation table base) Attribute
 */
#define TTB_ATR_STRGLY				0x00DF2	/* 0000_0000_0000_0000_0000_1101_1111_0010    Secure Strongly-ordered memory */
#define TTB_ATR_STRGLY_NS			0x80DF2	/* 0000_0000_0000_1000_0000_1101_1111_0010    Non-secure Strongly-ordered memory */
#define TTB_ATR_DEVICE				0x00DF6	/* 0000_0000_0000_0000_0000_1101_1111_0110    Secure Device memory */
#define TTB_ATR_DEVICE_NS			0x80DF6	/* 0000_0000_0000_1000_0000_1101_1111_0110    Non-secure Device memory */
#define TTB_ATR_NORMAL_NOT_CACHE		0x04DE2	/* 0000_0000_0000_0000_0100_1101_1110_0010    Outer and inner Non-cacheable, and Secure Normal memory */
#define TTB_ATR_NORMAL_NOT_CACHE_NS		0x84DE2	/* 0000_0000_0000_1000_0100_1101_1110_0010    Outer and inner Non-cacheable, and Non-secure Normal memory */
#define TTB_ATR_NORMAL_CACHE			0x01DEE	/* 0000_0000_0000_0000_0001_1101_1110_1110    Outer and inner Write-Back Write-Allocate Cacheable, and Secure Normal memory */
#define TTB_ATR_NORMAL_CACHE_NS			0x81DEE	/* 0000_0000_0000_1000_0001_1101_1110_1110    Outer and inner Write-Back Write-Allocate Cacheable, and Non-secure Normal memory */
#define TTB_ATR_NORMAL_L1_CACHE_L2_NOT_CACHE	0x04DE6	/* 0000_0000_0000_0000_0100_1101_1110_0110    Outer Non-cacheable, Inner Write-Back Write-Allocate Cacheable, and Secure Normal memory */
#define TTB_ATR_NORMAL_L1_CACHE_L2_NOT_CACHE_NS	0x84DE6	/* 0000_0000_0000_1000_0100_1101_1110_0110    Outer Non-cacheable, Inner Write-Back Write-Allocate Cacheable, and Non-secure Normal memory */


/* ------------------------------------------------------------------------ */
/*
 * Power & Clock Control
 */

/*
 * CPG registers
*/
#define CPG_FRQCR		0xFCFE0010
#define	CPG_FRQCR_INIVAL	0x1012	/* CPG initial value */
					/*	PLL(x88), I:G:B:P1:P0 = 22:11:5.5:2.75:1.375 
					 *	CKIO:Output at time usually,
					 *	Output when bus right is opened,
					 *	output at standby"L"
					 *	Clockin  = 24MHz,
					 *	I  Clock = 528MHz,
					 *	G  Clock = 264MHz
					 *	B  Clock = 132MHz,
					 *	P1 Clock = 66MHz,
					 *	P0 Clock = 33MHz
					*/

#define	CPG_CKIOSEL		0xFCFE0100
#define CPG_SCLKSEL		0xFCFE0104

/*
 * Low power consumption mode registers
*/
#define	CPG_SYSCR1		0xFCFE0400
#define	CPG_SYSCR2		0xFCFE0404
#define	CPG_SYSCR3		0xFCFE0408

/*
 * Stanby control registers
 */
#define CPG_STBCR1		0xFCFE0020
#define CPG_STBCR2		0xFCFE0024
#define CPG_STBCR3		0xFCFE0420
#define CPG_STBCR4		0xFCFE0424
#define CPG_STBCR5		0xFCFE0428
#define CPG_STBCR6		0xFCFE042C
#define CPG_STBCR7		0xFCFE0430
#define CPG_STBCR8		0xFCFE0434
#define CPG_STBCR9		0xFCFE0438
#define CPG_STBCR10		0xFCFE043C


/* ------------------------------------------------------------------------ */
/*
 * Level2 cache controller (PL310) registers
 */
#define PL310_BASE		0x1F003000
#define PL310_POWER_CTL		(PL310_BASE + 0x0F80)		/* Power Control Register */

/* ------------------------------------------------------------------------ */
/*
 * System Timer Clock
 */

/* OSTM0 : OS timer ch.0 */
#define OSTM0_BASE	0xE803B000
#define OSTM0_CMP	(OSTM0_BASE + 0x0000)
#define OSTM0_CNT	(OSTM0_BASE + 0x0004)
#define OSTM0_TE	(OSTM0_BASE + 0x0010)
#define OSTM0_TS	(OSTM0_BASE + 0x0014)
#define OSTM0_TT	(OSTM0_BASE + 0x0018)
#define OSTM0_CTL	(OSTM0_BASE + 0x0020)


/* P0 clock 33.333MHz */
#define TMCLK			33		/* Timer clock input (MHz) */
#define TMCLK_KHz		33333		/* Timer clock input (MHz) */
#define	COUNT_PER_SEC		33333333LL	/* count/sec */
#define	NSEC_PER_COUNT		30		/* nsec/count */


/* Settable interval range (millisecond) */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

/* ------------------------------------------------------------------------ */
/*
 * GIC(Generic Interrupt Controller) -400
 */
#define GICD_BASE	0xE8221000UL		/* Distributor register base address */
#define GICC_BASE	0xE8222000UL		/* CPU interface register base address */


/* ------------------------------------------------------------------------ */
/*
 * Number of Interrupt vectors
 */
#define N_INTVEC		(512)		/* Number of Interrupt vectors */

/*
 * The number of the implemented bit width for priority value fields.
 */
#define INTPRI_BITWIDTH		5	/* bitwidth of interrupt priority */

/*
 * Interrupt Priority Levels
 */
#define INTPRI_SHIFT		(8 - INTPRI_BITWIDTH)
#define INTPRI_HIGHEST		0	/* Highest interrupt level */
#define INTPRI_LOWEST		31	/* Lowest  interrupt level  */
#define INTPRI_SYSTICK		1	/* interrupt priority for SysTick */

/*
 * Interrupt used by micro T-Kernel
 */
#define INTNO_SW0		0	/* GIC software interrupt 0 (min.) */
#define INTNO_SW15		15	/* GIC software interrupt 15 (max.) */

#define INTNO_SYSTICK		88	/* System Timer interrupt (OSTIM0) */

/*
 * Time-event handler interrupt level
 */
#define TIMER_INTLEVEL		0

/* ------------------------------------------------------------------------ */
/*
 * Number of SVC handlers
 */
#define N_SVCHDR		(11)	/* Number of SVC handlers ( used by micro T-Kernel) */

/*
 * SVC used by micro T-Kernel
 */
#define	SVC_SYSCALL		6	/* micro T-Kernel system call */
#define	SVC_FORCE_DISPATCH	7	/* force dispatch */
#define	SVC_DISPATCH		8	/* task dispatcher */
#define	SVC_DEBUG_SUPPORT	9	/* debug support function */
#define	SVC_EXTENDED_SVC	10	/* Extended SVC */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for RZ/A2M)
 */
#define	CPU_HAS_PTMR	(0)

/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU			0
#define CPU_HAS_DPS			0

/*
 *  Number of coprocessors to use. 
 *    There is no coprocessor for this microcomputer.
 */
#define NUM_COPROCESSOR		0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
