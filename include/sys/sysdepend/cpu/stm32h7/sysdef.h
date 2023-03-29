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
 *	System dependencies definition (STM32H7 depended)
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_CPU_H__
#define __TK_SYSDEF_DEPEND_CPU_H__

#include "../../../machine.h"

/* CPU Core-dependent definition */
#include "../core/armv7m/sysdef.h"

/* ------------------------------------------------------------------------ */
/*
 * Internal Memorie (Main RAM)
 */

/* STM32H723 Internal SRAM-D1   0x24000000 - 0x2404FFFF  (Size 320KB) */
#if CPU_STM32H723
#define INTERNAL_RAM_SIZE       0x00050000
#define INTERNAL_RAM_START      0x24000000
#endif

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END


/* ------------------------------------------------------------------------ */
/*
 * System configuration controler (SYSCFG)
 */

#define SYSCFG_BASE	0x58000400

#define SYSCFG_PMCR	(SYSCFG_BASE+0x0004)
#define SYSCFG_EXTICR1	(SYSCFG_BASE+0x0008)
#define SYSCFG_EXTICR2	(SYSCFG_BASE+0x000C)
#define SYSCFG_EXTICR3	(SYSCFG_BASE+0x0010)
#define SYSCFG_EXTICR4	(SYSCFG_BASE+0x0014)
#define SYSCFG_CFGR	(SYSCFG_BASE+0x0018)
#define SYSCFG_CCCSR	(SYSCFG_BASE+0x0020)
#define SYSCFG_CCVR	(SYSCFG_BASE+0x0024)
#define SYSCFG_CCCR	(SYSCFG_BASE+0x0028)
#define SYSCFG_ADC2ALT	(SYSCFG_BASE+0x0030)
#define SYSCFG_PKGR	(SYSCFG_BASE+0x0124)

#define SYSCFG_UR0	(SYSCFG_BASE+0x0300)
#define SYSCFG_UR2	(SYSCFG_BASE+0x0308)
#define SYSCFG_UR3	(SYSCFG_BASE+0x030C)
#define SYSCFG_UR4	(SYSCFG_BASE+0x0310)
#define SYSCFG_UR5	(SYSCFG_BASE+0x0314)
#define SYSCFG_UR6	(SYSCFG_BASE+0x0318)
#define SYSCFG_UR7	(SYSCFG_BASE+0x031C)
#define SYSCFG_UR11	(SYSCFG_BASE+0x032C)
#define SYSCFG_UR12	(SYSCFG_BASE+0x0330)
#define SYSCFG_UR13	(SYSCFG_BASE+0x0334)
#define SYSCFG_UR14	(SYSCFG_BASE+0x0338)
#define SYSCFG_UR15	(SYSCFG_BASE+0x033C)
#define SYSCFG_UR16	(SYSCFG_BASE+0x0340)
#define SYSCFG_UR17	(SYSCFG_BASE+0x0344)

/* ------------------------------------------------------------------------ */
/*
 * Internal flash memory controls
 */

#define FLASH_BASE	0x52002000

#define FLASH_ACR		(FLASH_BASE+0x0000)	/* Flash access control register */
#define FLASH_KEYR		(FLASH_BASE+0x0004)	/* Flash key register */
#define FLASH_OPTKEYR		(FLASH_BASE+0x0008)	/* Flash option key register */
#define FLASH_CR		(FLASH_BASE+0x000C)	/* Flash control register */
#define FLASH_SR		(FLASH_BASE+0x0010)	/* Flash status register */
#define FLASH_CCR		(FLASH_BASE+0x0014)	/* Flash clear control register */
#define FLASH_OPTCR		(FLASH_BASE+0x0018)	/* Flash option control register */
#define FLASH_OPTSR_CUR		(FLASH_BASE+0x001C)	/* Flash option status register */
#define FLASH_OPTSR_PRG		(FLASH_BASE+0x0020)
#define FLASH_OPTCCR		(FLASH_BASE+0x0024)	/* Flash option clear control register */
#define	FLASH_PRAR_CUR		(FLASH_BASE+0x0028)	/* FLASH protection address */
#define	FLASH_PRAR_PRG		(FLASH_BASE+0x002C)
#define	FLASH_SCAR_CUR		(FLASH_BASE+0x0030)	/* FLASH secure address */
#define	FLASH_SCAR_PRG		(FLASH_BASE+0x0034)
#define	FLASH_WPSN_CUR		(FLASH_BASE+0x0038)	/* FLASH write sector protection */
#define	FLASH_WPSN_PRG		(FLASH_BASE+0x003C)
#define	FLASH_BOOT_CUR		(FLASH_BASE+0x0040)	/* FLASH register boot address for Arm® Cortex®-M7 core */
#define	FLASH_BOOT_PRG		(FLASH_BASE+0x0044)
#define	FLASH_CRCCR		(FLASH_BASE+0x0050)	/* FLASH CRC control register */
#define	FLASH_CRCSADDR		(FLASH_BASE+0x0054)	/* FLASH CRC start address register */
#define	FLASH_CRCEADDR		(FLASH_BASE+0x0058)	/* FLASH CRC end address register */
#define	FLASH_CRCDATAR		(FLASH_BASE+0x005C)	/* FLASH CRC data register */
#define	FLASH_ECC_FAR		(FLASH_BASE+0x0060)	/* FLASH ECC fail address */
#define	FLASH_OPTSR2_CUR	(FLASH_BASE+0x0070)	/* FLASH option status register 2 */
#define	FLASH_OPTSR2_PRG	(FLASH_BASE+0x0074)

/* FLASH_ACR bit definition */
#define FLASH_ACR_WRHIGHFREQ_MASK	0x00000030	/* Flash signal delay */
#define FLASH_ACR_LATENCY_MASK		0x0000000F	/* Read latency */
#define FLASH_ACR_LATENCY(n)		(((_UW)(n)<<0) & FLASH_ACR_LATENCY_MASK)

/* ------------------------------------------------------------------------ */
/*
 * Power & Clock Control
 */
/*
 * PWR (Power Control) registers
*/
#define PWR_BASE	0x58024800

#define PWR_CR1		(PWR_BASE+0x0000)	/* PWR control register 1 */
#define PWR_CSR1	(PWR_BASE+0x0004)	/* PWR control status register 1 */
#define PWR_CR2		(PWR_BASE+0x0008)	/* PWR control register 2 */
#define PWR_CR3		(PWR_BASE+0x000C)	/* PWR control register 3 */
#define	PWR_CPUCR	(PWR_BASE+0x0010)	/* PWR CPU control register */
#define	PWR_D3CR	(PWR_BASE+0x0018)	/* PWR D3 domain control register */
#define	PWR_WKUPCR	(PWR_BASE+0x0020)	/* PWR wakeup clear register */
#define	PWR_WKUPFR	(PWR_BASE+0x0024)	/* PWR wakeup flag register */
#define	PWR_WKUPEPR	(PWR_BASE+0x0028)	/* PWR wakeup enable and polarity register */

/* PWR_CSR1 bit definition */
#define	PWR_CSR1_ACTVOSRDY	(1<<13)		/* Voltage levels ready bit */

/* PWR_CR3 bit definition */
#define	PWR_CR3_LDOEN		(1<<1)		/* Low drop-out regulator enable bit */

/* PWR_D3CR bit definition */
#define	PWR_D3CR_VOSRDY		(1<<13)		/* VOS Ready bit */

/*
 * RCC (Reset & Clock control) registers
 */
#define RCC_BASE	0x58024400

#define RCC_CR		(RCC_BASE + 0x0000)	/* RCC Clock control register */
#define RCC_HSICFGR	(RCC_BASE + 0x0004)	/* RCC HSI configuration register */
#define RCC_CRRCR	(RCC_BASE + 0x0008)	/* RCC Clock recovery RC register */
#define RCC_CSICFGR	(RCC_BASE + 0x000C)	/* RCC CSI configuration register */
#define RCC_CFGR	(RCC_BASE + 0x0010)	/* RCC Clock configuration register */
#define RCC_D1CFGR	(RCC_BASE + 0x0018)	/* RCC domain 1 clock configuration register */
#define RCC_D2CFGR	(RCC_BASE + 0x001C)	/* RCC domain 2 clock configuration register */
#define RCC_D3CFGR	(RCC_BASE + 0x0020)	/* RCC domain 3 clock configuration register */
#define RCC_PLLCKSELR	(RCC_BASE + 0x0028)	/* RCC PLLs clock source selection register */
#define RCC_PLLCFGR	(RCC_BASE + 0x002C)	/* RCC PLL configuration register */
#define RCC_PLL1DIVR	(RCC_BASE + 0x0030)	/* RCC PLL1 dividers configuration register */
#define RCC_PLL1FRACR	(RCC_BASE + 0x0034)	/* RCC PLL1 fractional divider register */
#define RCC_PLL2DIVR	(RCC_BASE + 0x0038)	/* RCC PLL2 dividers configuration register */
#define RCC_PLL2FRACR	(RCC_BASE + 0x003C)	/* RCC PLL2 fractional divider register */
#define RCC_PLL3DIVR	(RCC_BASE + 0x0040)	/* RCC PLL3 dividers configuration register */
#define RCC_PLL3FRACR	(RCC_BASE + 0x0044)	/* RCC PLL3 fractional divider register */
#define RCC_D1CCIPR	(RCC_BASE + 0x004C)	/* RCC domain 1 kernel clock configuration register */
#define RCC_D2CCIP1R	(RCC_BASE + 0x0050)	/* RCC domain 2 kernel clock configuration register */
#define RCC_D2CCIP2R	(RCC_BASE + 0x0054)	/* RCC domain 2 kernel clock configuration register */
#define RCC_D3CCIPR	(RCC_BASE + 0x0058)	/* RCC domain 3 kernel clock configuration register */
#define RCC_CIER	(RCC_BASE + 0x0060)	/* RCC Clock interrupt enable register */
#define RCC_CIFR	(RCC_BASE + 0x0064)	/* RCC Clock interrupt flag register */
#define RCC_CICR	(RCC_BASE + 0x0068)	/* RCC Clock interrupt clear register */
#define RCC_BDCR	(RCC_BASE + 0x0070)	/* RCC　Backup domain control register */
#define RCC_CSR		(RCC_BASE + 0x0074)	/* RCC Control/status register */
#define RCC_AHB3RSTR	(RCC_BASE + 0x007C)	/* RCC AHB3 peripheral reset register */
#define RCC_AHB1RSTR	(RCC_BASE + 0x0080)	/* RCC AHB1 peripheral reset register */
#define RCC_AHB2RSTR	(RCC_BASE + 0x0084)	/* RCC AHB2 peripheral reset register */
#define RCC_AHB4RSTR	(RCC_BASE + 0x0088)	/* RCC AHB4 peripheral reset register */
#define RCC_APB3RSTR	(RCC_BASE + 0x008C)	/* RCC APB3 peripheral reset register */
#define RCC_APB1LRSTR	(RCC_BASE + 0x0090)	/* RCC APB1 peripheral reset register */
#define RCC_APB1HRSTR	(RCC_BASE + 0x0094)	/* RCC APB1 peripheral reset register */
#define RCC_APB2RSTR	(RCC_BASE + 0x0098)	/* RCC APB2 peripheral reset register */
#define RCC_APB4RSTR	(RCC_BASE + 0x009C)	/* RCC APB4 peripheral reset register */
#define RCC_GCR		(RCC_BASE + 0x00A0)	/* RCC global control register */
#define RCC_D3AMR	(RCC_BASE + 0x00A8)	/* RCC D3 Autonomous mode register */
#define RCC_RSR		(RCC_BASE + 0x00A8)	/* RCC reset status register */

#define RCC_AHB3ENR	(RCC_BASE + 0x00D4)	/* RCC AHB3 clock register */
#define RCC_AHB1ENR	(RCC_BASE + 0x00D8)	/* RCC AHB1 clock register */
#define RCC_AHB2ENR	(RCC_BASE + 0x00DC)	/* RCC AHB2 clock register */
#define RCC_AHB4ENR	(RCC_BASE + 0x00E0)	/* RCC AHB4 clock register */
#define RCC_APB3ENR	(RCC_BASE + 0x00E4)	/* RCC APB3 clock register */
#define RCC_APB1LENR	(RCC_BASE + 0x00E8)	/* RCC APB1 clock register */
#define RCC_APB1HENR	(RCC_BASE + 0x00EC)	/* RCC APB1 clock register */
#define RCC_APB2ENR	(RCC_BASE + 0x00F0)	/* RCC APB2 clock register */
#define RCC_APB4ENR	(RCC_BASE + 0x00F4)	/* RCC APB4 clock register */
#define	RCC_AHB3LPENR	(RCC_BASE + 0x00FC)	/* RCC AHB3 Sleep clock register */
#define	RCC_AHB1LPENR	(RCC_BASE + 0x0100)	/* RCC AHB1 Sleep clock register */
#define	RCC_AHB2LPENR	(RCC_BASE + 0x0104)	/* RCC AHB2 Sleep clock register */
#define	RCC_AHB4LPENR	(RCC_BASE + 0x0108)	/* RCC AHB4 Sleep clock register */
#define	RCC_APB3LPENR	(RCC_BASE + 0x010C)	/* RCC APB3 Sleep clock register */
#define	RCC_APB1LLPENR	(RCC_BASE + 0x0110)	/* RCC APB1 Low Sleep clock register */
#define	RCC_APB1HLPENR	(RCC_BASE + 0x0114)	/* RCC APB1 High Sleep clock register */
#define	RCC_APB2LPENR	(RCC_BASE + 0x0118)	/* RRC APB2 Sleep clock register */
#define	RCC_APB4LPENR	(RCC_BASE + 0x011C)	/* RRC APB2 Sleep clock register */

/* RCC_CR bit definition */
#define RCC_CR_PLL3RDY		0x20000000	// PLL3 clock ready flag	
#define RCC_CR_PLL3ON		0x10000000	// PLL3 enable
#define RCC_CR_PLL2RDY		0x08000000	// PLL2 clock ready flag	
#define RCC_CR_PLL2ON		0x04000000	// PLL2 enable
#define RCC_CR_PLL1RDY		0x02000000	// PLL1 clock ready flag	
#define RCC_CR_PLL1ON		0x01000000	// PLL1 enable
#define RCC_CR_HSECSSON		0x00080000	// HSE Clock Security System enable
#define RCC_CR_HSEBYP		0x00040000	// HSE clock bypass
#define RCC_CR_HSERDY		0x00020000	// HSE clock ready flag
#define RCC_CR_HSEON		0x00010000	// HSE clock enable
#define RCC_CR_D2CKRDY		0x00008000	// D2 domain clocks ready flag
#define RCC_CR_D1CKRDY		0x00004000	// D1 domain clocks ready flag
#define RCC_CR_HSI48RDY		0x00002000	// HSI48 clock ready flag
#define RCC_CR_HSI48ON		0x00001000	// HSI48 clock enable
#define RCC_CR_CSIKERON		0x00000200	// CSI clock enable in Stop mode
#define RCC_CR_CSIRDY		0x00000100	// CSI clock ready flag
#define RCC_CR_CSION		0x00000080	// CSI clock enable
#define RCC_CR_HSIDIVF		0x00000020	// HSI divider flag 
#define	RCC_CR_HSIDIV_MASK	0x00000018	// HSI clock divider

#define RCC_CR_HSIDIV_1		0x00000000	// HSI clock divider (/1)
#define RCC_CR_HSIDIV_2		0x00000008	// HSI clock divider (/2)
#define RCC_CR_HSIDIV_4		0x00000010	// HSI clock divider (/4)
#define RCC_CR_HSIDIV_8		0x00000018	// HSI clock divider (/8)

#define RCC_CR_HSIRDY		0x00000004	// HSI clock ready flag
#define RCC_CR_HSIKERON		0x00000002	// HSI clock enable in Stop mode
#define RCC_CR_HSION		0x00000001	// HSl clock enable

/* RCC_PLLCKSELR bit definition */
#define	RCC_PLLCKSELR_DIVM3	0x03F00000	// Prescaler for PLL3
#define	RCC_PLLCKSELR_DIVM2	0x0003F000	// Prescaler for PLL2
#define	RCC_PLLCKSELR_DIVM1	0x000003F0	// Prescaler for PLL1
#define	RCC_PLLCKSELR_PLLSRC	0x00000003	// DIVMx and PLLs clock source

/* RCC_CFGR bit definition */
#define	RCC_CFGR_SWS		0x00000038	// System clock switch status
#define	RCC_CFGR_SW		0x00000007	// System clock switch

/* RCC_PLLCFGR bit definition */
#define	RCC_PLLCFGR_DIVR3EN	0x01000000	// PLL3 DIVR divider output enable
#define	RCC_PLLCFGR_DIVQ3EN	0x00800000	// PLL3 DIVQ divider output enable
#define	RCC_PLLCFGR_DIVP3EN	0x00400000	// PLL3 DIVP divider output enable
#define	RCC_PLLCFGR_DIVR2EN	0x00200000	// PLL2 DIVR divider output enable
#define	RCC_PLLCFGR_DIVQ2EN	0x00100000	// PLL2 DIVQ divider output enable
#define	RCC_PLLCFGR_DIVP2EN	0x00080000	// PLL2 DIVP divider output enable
#define	RCC_PLLCFGR_DIVR1EN	0x00040000	// PLL1 DIVR divider output enable
#define	RCC_PLLCFGR_DIVQ1EN	0x00020000	// PLL1 DIVQ divider output enable
#define	RCC_PLLCFGR_DIVP1EN	0x00010000	// PLL1 DIVP divider output enable
#define RCC_PLLCFGR_PLL3RGE	0x00000C00	// PLL3 input frequency range
#define RCC_PLLCFGR_PLL3VCOSEL	0x00000200	// PLL3 VCO selection
#define RCC_PLLCFGR_PLL3FRACEN	0x00000100	// PLL3 fractional latch enable
#define RCC_PLLCFGR_PLL2RGE	0x000000C0	// PLL2 input frequency range
#define RCC_PLLCFGR_PLL2VCOSEL	0x00000020	// PLL2 VCO selection
#define RCC_PLLCFGR_PLL2FRACEN	0x00000010	// PLL2 fractional latch enable
#define RCC_PLLCFGR_PLL1RGE	0x0000000C	// PLL1 input frequency range
#define RCC_PLLCFGR_PLL1VCOSEL	0x00000002	// PLL1 VCO selection
#define RCC_PLLCFGR_PLL1FRACEN	0x00000001	// PLL1 fractional latch enable

/* RCC_PLL2FRACR bit definition */
#define RCC_PLL2FRACR_FRACN2	0x0000FFF8	// Fractional part of the multiplication factor for PLL2 VCO

/* RCC_D2CCIP2R bit definition */
#define	RCC_D2CCIP2R_USARTxSEL	0x0000003F	// USARTx/UARTx clock select
#define RCC_D2CCIP2R_I2C1235SEL	0x00003000	// I2C1/2/3/5 clock select

/* RCC_D3CCIPR bit definition */
#define RCC_D3CCIPR_ADCSEL	0x00030000	// ADC clock select
#define RCC_D3CCIPR_I2C4SEL	0x00000300	// I2C4 clock select

/* RCC_AHB1ENR bit definition */
#define	RCC_AHB1ENR_ADC12EN	0x00000020	// ADC1/2 clock enable

/* RCC_AHB4ENR bit definition */
#define	RCC_AHB4ENR_ADC3EN	0x01000000	// ADC3 clock enable

/* RCC_APB1LENR bit definition */
#define RCC_APB1LENR_UART8EN	0x80000000	// UART8 clock enable
#define RCC_APB1LENR_UART7EN	0x40000000	// UART7 clock enable
#define	RCC_APB1LENR_I2C5EN	0x02000000	// I2C5 clock enable
#define	RCC_APB1LENR_I2C3EN	0x00800000	// I2C3 clock enable
#define	RCC_APB1LENR_I2C2EN	0x00400000	// I2C2 clock enable
#define	RCC_APB1LENR_I2C1EN	0x00200000	// I2C1 clock enable
#define RCC_APB1LENR_UART5EN	0x00100000	// UART5 clock enable
#define RCC_APB1LENR_UART4EN	0x00080000	// UART4 clock enable
#define RCC_APB1LENR_USART3EN	0x00040000	// USART3 clock enable
#define RCC_APB1LENR_USART2EN	0x00020000	// USART2 clock enable

/* RCC_APB2ENR bit definition */
#define RCC_APB2ENR_USART10EN	0x00000080	// USART10 clock enable
#define RCC_APB2ENR_UART9EN	0x00000040	// UART9 clock enable
#define RCC_APB2ENR_USART6EN	0x00000020	// USART6 clock enable
#define RCC_APB2ENR_USART1EN	0x00000010	// USART1 clock enable

/* RCC_APB4ENR bit definition */
#define	RCC_APB4ENR_I2C4EN	0x00000080	// I2C4 clock enable

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
#define N_INTVEC		190	/* Number of Interrupt vectors */
#define	N_SYSVEC		16	/* Number of System Exceptions */

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
#define	N_EXTIEVT			92	/*  Number of EXTI event input */

#define EXTI_BASE	0x58000000

#define	EXTI_RTSR1	(EXTI_BASE + 0x00)
#define	EXTI_FTSR1	(EXTI_BASE + 0x04)
#define	EXTI_SWIER1	(EXTI_BASE + 0x08)
#define	EXTI_D3PMR1	(EXTI_BASE + 0x0C)
#define	EXTI_D3PCR1L	(EXTI_BASE + 0x10)
#define	EXTI_D3PCR1H	(EXTI_BASE + 0x14)

#define	EXTI_RTSR2	(EXTI_BASE + 0x20)
#define	EXTI_FTSR2	(EXTI_BASE + 0x24)
#define	EXTI_SWIER2	(EXTI_BASE + 0x28)
#define	EXTI_D3PMR2	(EXTI_BASE + 0x2C)
#define	EXTI_D3PCR2L	(EXTI_BASE + 0x30)
#define	EXTI_D3PCR2H	(EXTI_BASE + 0x34)

#define	EXTI_RTSR3	(EXTI_BASE + 0x40)
#define	EXTI_FTSR3	(EXTI_BASE + 0x44)
#define	EXTI_SWIER3	(EXTI_BASE + 0x48)
#define	EXTI_D3PMR3	(EXTI_BASE + 0x4C)
#define	EXTI_D3PCR3L	(EXTI_BASE + 0x50)
#define	EXTI_D3PCR3H	(EXTI_BASE + 0x54)

#define	EXTI_CPUIMR1	(EXTI_BASE + 0x80)
#define	EXTI_CPUEMR1	(EXTI_BASE + 0x84)
#define	EXTI_CPUPR1	(EXTI_BASE + 0x88)

#define	EXTI_CPUIMR2	(EXTI_BASE + 0x90)
#define	EXTI_CPUEMR2	(EXTI_BASE + 0x94)
#define	EXTI_CPUPR2	(EXTI_BASE + 0x98)

#define	EXTI_CPUIMR3	(EXTI_BASE + 0xA0)
#define	EXTI_CPUEMR3	(EXTI_BASE + 0xA4)
#define	EXTI_CPUPR3	(EXTI_BASE + 0xA8)

/* ------------------------------------------------------------------------ */
/*
 * Watchdog Timer
 */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define	GPIOA_BASE	0x58020000
#define	GPIOB_BASE	0x58020400
#define	GPIOC_BASE	0x58020800
#define	GPIOD_BASE	0x58020C00
#define	GPIOE_BASE	0x58021000
#define	GPIOF_BASE	0x58021400
#define	GPIOG_BASE	0x58021800
#define	GPIOH_BASE	0x58021C00
#define	GPIOJ_BASE	0x58022400
#define	GPIOK_BASE	0x58022800

#define GPIO_MODER(n)	(GPIO##n##_BASE + 0x00)		/* GPIO port mode register */
#define GPIO_OTYPER(n)	(GPIO##n##_BASE + 0x04)		/* GPIO port output type register */
#define GPIO_OSPEEDR(n)	(GPIO##n##_BASE + 0x08)		/* GPIO port output speed register */
#define GPIO_PUPDR(n)	(GPIO##n##_BASE + 0x0C)		/* GPIO port pull-up/pull-down register */
#define GPIO_IDR(n)	(GPIO##n##_BASE + 0x10)		/* GPIO port input data register */
#define GPIO_ODR(n)	(GPIO##n##_BASE + 0x14)		/* GPIO port output data register */
#define GPIO_BSRR(n)	(GPIO##n##_BASE + 0x18)		/* GPIO port bit set/reset register */
#define GPIO_LCKR(n)	(GPIO##n##_BASE + 0x1C)		/* GPIO port configuration lock register */
#define GPIO_AFRL(n)	(GPIO##n##_BASE + 0x20)		/* GPIO alternate function low register */
#define GPIO_AFRH(n)	(GPIO##n##_BASE + 0x24)		/* GPIO alternate function high register */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for STM32H7)
 */
#define	CPU_HAS_PTMR	(1)

/* Timer register definition */
#define	TIM2_BASE	0x40000000
#define	TIM3_BASE	0x40000400
#define	TIM4_BASE	0x40000800
#define	TIM5_BASE	0x40000C00

#define	TIMxCR1		0x00
#define TIMxCR2		0x04
#define TIMxSMCR	0x08
#define TIMxDIER	0x0C
#define TIMxSR		0x10
#define TIMxEGR		0x14
#define TIMxCCMR1	0x18
#define TIMxCCMR2	0x1C
#define TIMxCCER	0x20
#define TIMxCNT		0x24
#define TIMxPSC		0x28
#define TIMxARR		0x2C
#define TIMxCCR1	0x34
#define TIMxCCR2	0x38
#define TIMxCCR3	0x3C
#define TIMxCCR4	0x40
#define TIMxDCR		0x48
#define TIMxDMAR	0x4C
#define TIMxAF1		0x60
#define TIMxTISEL	0x68

#define	TIMxCR1_CEN	(1<<0)
#define	TIMxCR1_OPM	(1<<3)
#define	TIMxCR1_DIR	(1<<4)
#define	TIMxDIER_UIE	(1<<0)
#define TIMxSR_UIF	(1<<0)
#define TIMxEGR_UG	(1<<0)

/* Prescaler value */
#define TIM2PSC_PSC_INIT	0
#define TIM3PSC_PSC_INIT	0
#define TIM4PSC_PSC_INIT	0
#define TIM5PSC_PSC_INIT	0

/* Physical timer interrupt number */
#define INTNO_TIM2	28
#define INTNO_TIM3	29
#define INTNO_TIM4	30
#define INTNO_TIM5	50

/* Physical timer interrupt priority */
#define INTPRI_TIM2	5
#define INTPRI_TIM3	5
#define INTPRI_TIM4	5
#define INTPRI_TIM5	5

/* Phycail timer Maximum count */
#define PTMR_MAX_CNT16    (0x0000FFFF)
#define PTMR_MAX_CNT32    (0xFFFFFFFF)

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

#endif /* __TK_SYSDEF_DEPEND_CPU_H__ */
