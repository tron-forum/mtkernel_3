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

/* CPG initial value */
/*	PLL(x88), I:G:B:P1:P0 = 22:11:5.5:2.75:1.375 
 *	CKIO:Output at time usually,
 *	Output when bus right is opened,
 *	output at standby"L"
 */
#define	CPG_FRQCR_INIVAL	0x1012
#define CLOCK_in	24	/* Clockin  = 24MH */
#define CLOCK_I		528	/* I  Clock = 528MHz */
#define CLOCK_G		264	/* G  Clock = 264MHz */
#define CLOCK_B		132	/* B  Clock = 132MHz */
#define	CLOCK_P1	66	/* P1 Clock = 66MHz */
#define	CLOCK_P0	33	/* P0 Clock = 33MHz */

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
#define CPG_STBCR11		0xFCFE0440


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


/* P1 clock 66.666MHz */
#define TMCLK			66		/* Timer clock input (MHz) */
#define TMCLK_KHz		66666		/* Timer clock input (KHz) */
#define	COUNT_PER_SEC		66666666LL	/* count/sec */
#define	NSEC_PER_COUNT		15		/* nsec/count */


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
 * General purpose I / O port
 */
#define PORT_BASE	0xFCFFE000

/* Port direction register */
#define PORT0_PDR	(PORT_BASE + 0x0000)
#define PORT1_PDR	(PORT_BASE + 0x0002)
#define PORT2_PDR	(PORT_BASE + 0x0004)
#define PORT3_PDR	(PORT_BASE + 0x0006)
#define PORT4_PDR	(PORT_BASE + 0x0008)
#define PORT5_PDR	(PORT_BASE + 0x000A)
#define PORT6_PDR	(PORT_BASE + 0x000C)
#define PORT7_PDR	(PORT_BASE + 0x000E)
#define PORT8_PDR	(PORT_BASE + 0x0010)
#define PORT9_PDR	(PORT_BASE + 0x0012)
#define PORTA_PDR	(PORT_BASE + 0x0014)
#define PORTB_PDR	(PORT_BASE + 0x0016)
#define PORTC_PDR	(PORT_BASE + 0x0018)
#define PORTD_PDR	(PORT_BASE + 0x001A)
#define PORTE_PDR	(PORT_BASE + 0x001C)
#define PORTF_PDR	(PORT_BASE + 0x001E)
#define PORTG_PDR	(PORT_BASE + 0x0020)
#define PORTH_PDR	(PORT_BASE + 0x0022)
#define PORTJ_PDR	(PORT_BASE + 0x0024)
#define PORTK_PDR	(PORT_BASE + 0x0026)
#define PORTL_PDR	(PORT_BASE + 0x0028)
#define PORTM_PDR	(PORT_BASE + 0x002A)

/* Port output data register */
#define PORT0_PODR	(PORT_BASE + 0x0040)
#define PORT1_PODR	(PORT_BASE + 0x0041)
#define PORT2_PODR	(PORT_BASE + 0x0042)
#define PORT3_PODR	(PORT_BASE + 0x0043)
#define PORT4_PODR	(PORT_BASE + 0x0044)
#define PORT5_PODR	(PORT_BASE + 0x0045)
#define PORT6_PODR	(PORT_BASE + 0x0046)
#define PORT7_PODR	(PORT_BASE + 0x0047)
#define PORT8_PODR	(PORT_BASE + 0x0048)
#define PORT9_PODR	(PORT_BASE + 0x0049)
#define PORTA_PODR	(PORT_BASE + 0x004A)
#define PORTB_PODR	(PORT_BASE + 0x004B)
#define PORTC_PODR	(PORT_BASE + 0x004C)
#define PORTD_PODR	(PORT_BASE + 0x004D)
#define PORTE_PODR	(PORT_BASE + 0x004E)
#define PORTF_PODR	(PORT_BASE + 0x004F)
#define PORTG_PODR	(PORT_BASE + 0x0050)
#define PORTH_PODR	(PORT_BASE + 0x0051)
#define PORTJ_PODR	(PORT_BASE + 0x0052)
#define PORTK_PODR	(PORT_BASE + 0x0053)
#define PORTL_PODR	(PORT_BASE + 0x0054)
#define PORTM_PODR	(PORT_BASE + 0x0055)

/* Port input data register */
#define PORT0_PIDR	(PORT_BASE + 0x0060)
#define PORT1_PIDR	(PORT_BASE + 0x0061)
#define PORT2_PIDR	(PORT_BASE + 0x0062)
#define PORT3_PIDR	(PORT_BASE + 0x0063)
#define PORT4_PIDR	(PORT_BASE + 0x0064)
#define PORT5_PIDR	(PORT_BASE + 0x0065)
#define PORT6_PIDR	(PORT_BASE + 0x0066)
#define PORT7_PIDR	(PORT_BASE + 0x0067)
#define PORT8_PIDR	(PORT_BASE + 0x0068)
#define PORT9_PIDR	(PORT_BASE + 0x0069)
#define PORTA_PIDR	(PORT_BASE + 0x006A)
#define PORTB_PIDR	(PORT_BASE + 0x006B)
#define PORTC_PIDR	(PORT_BASE + 0x006C)
#define PORTD_PIDR	(PORT_BASE + 0x006D)
#define PORTE_PIDR	(PORT_BASE + 0x006E)
#define PORTF_PIDR	(PORT_BASE + 0x006F)
#define PORTG_PIDR	(PORT_BASE + 0x0070)
#define PORTH_PIDR	(PORT_BASE + 0x0071)
#define PORTJ_PIDR	(PORT_BASE + 0x0072)
#define PORTK_PIDR	(PORT_BASE + 0x0073)
#define PORTL_PIDR	(PORT_BASE + 0x0074)
#define PORTM_PIDR	(PORT_BASE + 0x0075)

/* Port mode register */
#define PORT0_PMR	(PORT_BASE + 0x0080)
#define PORT1_PMR	(PORT_BASE + 0x0081)
#define PORT2_PMR	(PORT_BASE + 0x0082)
#define PORT3_PMR	(PORT_BASE + 0x0083)
#define PORT4_PMR	(PORT_BASE + 0x0084)
#define PORT5_PMR	(PORT_BASE + 0x0085)
#define PORT6_PMR	(PORT_BASE + 0x0086)
#define PORT7_PMR	(PORT_BASE + 0x0087)
#define PORT8_PMR	(PORT_BASE + 0x0088)
#define PORT9_PMR	(PORT_BASE + 0x0089)
#define PORTA_PMR	(PORT_BASE + 0x008A)
#define PORTB_PMR	(PORT_BASE + 0x008B)
#define PORTC_PMR	(PORT_BASE + 0x008C)
#define PORTD_PMR	(PORT_BASE + 0x008D)
#define PORTE_PMR	(PORT_BASE + 0x008E)
#define PORTF_PMR	(PORT_BASE + 0x008F)
#define PORTG_PMR	(PORT_BASE + 0x0090)
#define PORTH_PMR	(PORT_BASE + 0x0091)
#define PORTJ_PMR	(PORT_BASE + 0x0092)
#define PORTK_PMR	(PORT_BASE + 0x0093)
#define PORTL_PMR	(PORT_BASE + 0x0094)
#define PORTM_PMR	(PORT_BASE + 0x0095)

/* Drive capacity control register */
#define PORT0_DSCR	(PORT_BASE + 0x0140)
#define PORT1_DSCR	(PORT_BASE + 0x0142)
#define PORT2_DSCR	(PORT_BASE + 0x0144)
#define PORT3_DSCR	(PORT_BASE + 0x0146)
#define PORT4_DSCR	(PORT_BASE + 0x0148)
#define PORT5_DSCR	(PORT_BASE + 0x014A)
#define PORT6_DSCR	(PORT_BASE + 0x014C)
#define PORT7_DSCR	(PORT_BASE + 0x014E)
#define PORT8_DSCR	(PORT_BASE + 0x0150)
#define PORT9_DSCR	(PORT_BASE + 0x0152)
#define PORTA_DSCR	(PORT_BASE + 0x0154)
#define PORTB_DSCR	(PORT_BASE + 0x0156)
#define PORTC_DSCR	(PORT_BASE + 0x0158)
#define PORTD_DSCR	(PORT_BASE + 0x015A)
#define PORTE_DSCR	(PORT_BASE + 0x015C)
#define PORTF_DSCR	(PORT_BASE + 0x015E)
#define PORTG_DSCR	(PORT_BASE + 0x0160)
#define PORTH_DSCR	(PORT_BASE + 0x0162)
#define PORTJ_DSCR	(PORT_BASE + 0x0164)
#define PORTK_DSCR	(PORT_BASE + 0x0166)
#define PORTL_DSCR	(PORT_BASE + 0x0168)
#define PORTM_DSCR	(PORT_BASE + 0x016A)

/* Pin function selection register */
#define PORT0n_PFS(n)	(PORT_BASE + 0x0200 + (n))
#define PORT1n_PFS(n)	(PORT_BASE + 0x0208 + (n))
#define PORT2n_PFS(n)	(PORT_BASE + 0x0210 + (n))
#define PORT3n_PFS(n)	(PORT_BASE + 0x0218 + (n))
#define PORT4n_PFS(n)	(PORT_BASE + 0x0220 + (n))
#define PORT5n_PFS(n)	(PORT_BASE + 0x0228 + (n))
#define PORT6n_PFS(n)	(PORT_BASE + 0x0230 + (n))
#define PORT7n_PFS(n)	(PORT_BASE + 0x0238 + (n))
#define PORT8n_PFS(n)	(PORT_BASE + 0x0240 + (n))
#define PORT9n_PFS(n)	(PORT_BASE + 0x0248 + (n))
#define PORTAn_PFS(n)	(PORT_BASE + 0x0250 + (n))
#define PORTBn_PFS(n)	(PORT_BASE + 0x0258 + (n))
#define PORTCn_PFS(n)	(PORT_BASE + 0x0260 + (n))
#define PORTDn_PFS(n)	(PORT_BASE + 0x0268 + (n))
#define PORTEn_PFS(n)	(PORT_BASE + 0x0270 + (n))
#define PORTFn_PFS(n)	(PORT_BASE + 0x0278 + (n))
#define PORTGn_PFS(n)	(PORT_BASE + 0x0280 + (n))
#define PORTHn_PFS(n)	(PORT_BASE + 0x0288 + (n))
#define PORTJn_PFS(n)	(PORT_BASE + 0x0290 + (n))
#define PORTKn_PFS(n)	(PORT_BASE + 0x0298 + (n))
#define PORTLn_PFS(n)	(PORT_BASE + 0x02A0 + (n))
#define PORTMn_PFS(n)	(PORT_BASE + 0x02A8 + (n))

#define PORT_PFS_ISEL		(0x40)
#define PORT_PFS_PSEL(n)	(0x07 & n)

#define	PORT_PWPR	(PORT_BASE + 0x02FF)		/* Write protect register */
#define PORT_PWPR_B0WI	0b10000000			/* PFSWE write permission bit */
#define PORT_PWPR_PFSWE	0b01000000			/* PFS write permission bit */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for RZ/A2M)
 */
#define	CPU_HAS_PTMR	(1)

/* Timer register definition */
#define OSTM1_BASE	0xE803C000
#define OSTM2_BASE	0xE803D000

#define OSTMnCMP	0x00
#define OSTMnCNT	0x04
#define OSTMnTE		0x10
#define OSTMnTS		0x14
#define OSTMnTT		0x18
#define OSTMnCTL	0x20

#define	OSTMnCTL_MD1	0x02
#define	OSTMnCTL_MD0	0x01

/* Physical timer interrupt number */
#define INTNO_OSTM1	89
#define INTNO_OSTM2	90

/* Physical timer interrupt priority */
#define INTPRI_OSTM1	5
#define INTPRI_OSTM2	5

/* OS Timer clock 33.333MHz */
#define	PTMCLK		33

/* Phycail timer Maximum count */
#define PTMR_MAX_CNT	(0xFFFFFFFF)


/* ------------------------------------------------------------------------ */
/*
 * Coprocessor
 */
#define CPU_HAS_FPU			1
#define CPU_HAS_DSP			0

/*
 *  Number of coprocessors to use. 
 *    There is no coprocessor for this microcomputer.
 */
#if USE_FPU
#define NUM_COPROCESSOR		1
#else
#define NUM_COPROCESSOR		0
#endif

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
