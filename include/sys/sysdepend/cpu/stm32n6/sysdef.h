/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
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
 *	System dependencies definition (STM32N6 depended)
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
#if TRUSTZONE_SECURE		// Secure
	/* STM32N657 Internal AXI SRAM0   0x34000400 - 0x341FFFFF  (Size 2047KB) */
	#define INTERNAL_RAM_START      0x34000400
	#define INTERNAL_RAM_SIZE       0x001FFC00

#elif TRUSTZONE_NONSECURE	// Non-Secure
	/* STM32N657 Internal AXI SRAM1   0x24100400 - 0x241FFFFF  (Size 1023KB) */
	#define INTERNAL_RAM_START      0x24100400
	#define INTERNAL_RAM_SIZE       0x000FFC00

#else				// TrustZone disable
	/* STM32N657 Internal AXI SRAM0   0x34000400 - 0x341FFFFF  (Size 2047KB) */
	#define INTERNAL_RAM_START      0x34000400
	#define INTERNAL_RAM_SIZE       0x001FFC00

#endif

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END

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
#define N_INTVEC		196	/* Number of Interrupt vectors */

/*
 * Exception vector table alignment
*/
#define	EXCTBL_ALIGN		1024

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
 * EXTI (Extended interrupt controller)
 */
#define	N_EXTIEVT		78	/*  Number of EXTI event input */

#if TRUSTZONE_SECURE		// Secure
	#define MTK_EXTI_BASE	0x56025000
#elif TRUSTZONE_NONSECURE	// Non-Secure
	#define	MTK_EXTI_BASE	0x46025000
#else				// TrustZone disebla
	#define MTK_EXTI_BASE	0x56025000
#endif

#define	EXTI_RTSR1	(MTK_EXTI_BASE + 0x00)
#define	EXTI_FTSR1	(MTK_EXTI_BASE + 0x04)
#define	EXTI_SWIER1	(MTK_EXTI_BASE + 0x08)
#define	EXTI_RPR1	(MTK_EXTI_BASE + 0x0C)
#define	EXTI_FPR1	(MTK_EXTI_BASE + 0x10)
#define	EXTI_SECCFGR1	(MTK_EXTI_BASE + 0x14)
#define EXTI_PRIVCFGR1	(MTK_EXTI_BASE + 0x18)

#define	EXTI_RTSR2	(MTK_EXTI_BASE + 0x20)
#define	EXTI_FTSR2	(MTK_EXTI_BASE + 0x24)
#define	EXTI_SWIER2	(MTK_EXTI_BASE + 0x28)
#define	EXTI_RPR2	(MTK_EXTI_BASE + 0x2C)
#define	EXTI_FPR2	(MTK_EXTI_BASE + 0x30)
#define	EXTI_SECCFGR2	(MTK_EXTI_BASE + 0x34)
#define EXTI_PRIVCFGR2	(MTK_EXTI_BASE + 0x38)

#define	EXTI_RTSR3	(MTK_EXTI_BASE + 0x40)
#define	EXTI_FTSR3	(MTK_EXTI_BASE + 0x44)
#define	EXTI_SWIER3	(MTK_EXTI_BASE + 0x48)
#define	EXTI_RPR3	(MTK_EXTI_BASE + 0x4C)
#define	EXTI_FPR3	(MTK_EXTI_BASE + 0x50)
#define	EXTI_SECCFGR3	(MTK_EXTI_BASE + 0x54)
#define EXTI_PRIVCFGR3	(MTK_EXTI_BASE + 0x58)

#define	EXTI_EXTICR1	(MTK_EXTI_BASE + 0x60)
#define	EXTI_EXTICR2	(MTK_EXTI_BASE + 0x64)
#define	EXTI_EXTICR3	(MTK_EXTI_BASE + 0x68)
#define	EXTI_EXTICR4	(MTK_EXTI_BASE + 0x6C)

#define	EXTI_LOCKR	(MTK_EXTI_BASE + 0x70)

#define	EXTI_IMR1	(MTK_EXTI_BASE + 0x80)
#define	EXTI_EMR1	(MTK_EXTI_BASE + 0x84)

#define	EXTI_IMR2	(MTK_EXTI_BASE + 0x90)
#define	EXTI_EMR2	(MTK_EXTI_BASE + 0x94)

#define	EXTI_IMR3	(MTK_EXTI_BASE + 0xA0)
#define	EXTI_EMR3	(MTK_EXTI_BASE + 0xA4)

#define	EXTI_RTSR(n)	(MTK_EXTI_BASE + 0x00 +(0x20)*(n))
#define	EXTI_FTSR(n)	(MTK_EXTI_BASE + 0x04 +(0x20)*(n))
#define EXTI_RPR(n)	(MTK_EXTI_BASE + 0x0C +(0x20*(n)))	
#define EXTI_FPR(n)	(MTK_EXTI_BASE + 0x10 +(0x20*(n)))	
#define EXTI_IMR(n)	(MTK_EXTI_BASE + 0x80 +(0x10*(n)))

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

/*
 * RCC (Reset & Clock control) registers (Used from physical timer)
 */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#if TRUSTZONE_SECURE		// Secure
#define	MTK_GPIOA_BASE	0x56020000
#define	MTK_GPIOB_BASE	0x56020400
#define	MTK_GPIOC_BASE	0x56020800
#define	MTK_GPIOD_BASE	0x56020C00
#define	MTK_GPIOE_BASE	0x56021000
#define	MTK_GPIOF_BASE	0x56021400
#define	MTK_GPIOG_BASE	0x56021800
#define	MTK_GPIOH_BASE	0x56021C00
#define	MTK_GPION_BASE	0x56023400
#define	MTK_GPIOO_BASE	0x56023800
#define	MTK_GPIOP_BASE	0x56023C00
#define	MTK_GPIOQ_BASE	0x56024000

#elif TRUSTZONE_NONSECURE	// Non-Secure
#define	MTK_GPIOA_BASE	0x46020000
#define	MTK_GPIOB_BASE	0x46020400
#define	MTK_GPIOC_BASE	0x46020800
#define	MTK_GPIOD_BASE	0x46020C00
#define	MTK_GPIOE_BASE	0x46021000
#define	MTK_GPIOF_BASE	0x46021400
#define	MTK_GPIOG_BASE	0x46021800
#define	MTK_GPIOH_BASE	0x46021C00
#define	MTK_GPION_BASE	0x46023400
#define	MTK_GPIOO_BASE	0x46023800
#define	MTK_GPIOP_BASE	0x46023C00
#define	MTK_GPIOQ_BASE	0x46024000

#else	// TrustZone disable
#define	MTK_GPIOA_BASE	0x56020000
#define	MTK_GPIOB_BASE	0x56020400
#define	MTK_GPIOC_BASE	0x56020800
#define	MTK_GPIOD_BASE	0x56020C00
#define	MTK_GPIOE_BASE	0x56021000
#define	MTK_GPIOF_BASE	0x56021400
#define	MTK_GPIOG_BASE	0x56021800
#define	MTK_GPIOH_BASE	0x56021C00
#define	MTK_GPION_BASE	0x56023400
#define	MTK_GPIOO_BASE	0x56023800
#define	MTK_GPIOP_BASE	0x56023C00
#define	MTK_GPIOQ_BASE	0x56024000

#endif

#define GPIO_MODER(n)	(MTK_GPIO##n##_BASE + 0x00)		/* GPIO port mode register */
#define GPIO_OTYPER(n)	(MTK_GPIO##n##_BASE + 0x04)		/* GPIO port output type register */
#define GPIO_OSPEEDR(n)	(MTK_GPIO##n##_BASE + 0x08)		/* GPIO port output speed register */
#define GPIO_PUPDR(n)	(MTK_GPIO##n##_BASE + 0x0C)		/* GPIO port pull-up/pull-down register */
#define GPIO_IDR(n)	(MTK_GPIO##n##_BASE + 0x10)		/* GPIO port input data register */
#define GPIO_ODR(n)	(MTK_GPIO##n##_BASE + 0x14)		/* GPIO port output data register */
#define GPIO_BSRR(n)	(MTK_GPIO##n##_BASE + 0x18)		/* GPIO port bit set/reset register */
#define GPIO_LCKR(n)	(MTK_GPIO##n##_BASE + 0x1C)		/* GPIO port configuration lock register */
#define GPIO_AFRL(n)	(MTK_GPIO##n##_BASE + 0x20)		/* GPIO alternate function low register */
#define GPIO_AFRH(n)	(MTK_GPIO##n##_BASE + 0x24)		/* GPIO alternate function high register */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for STM32N6)
 */
#define	CPU_HAS_PTMR	0

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
