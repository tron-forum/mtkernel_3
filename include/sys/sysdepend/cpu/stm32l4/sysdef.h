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
 *	System dependencies definition (STM32L4 depended)
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

/* STM32L478, STM32L486 Internal SRAM1   0x20000000 - 0x20017FFF  (Size 96KB) */
#if CPU_STM32L476 | CPU_STM32L486
#define INTERNAL_RAM_SIZE       0x00018000
#define INTERNAL_RAM_START      0x20000000
#endif

#define INTERNAL_RAM_END        (INTERNAL_RAM_START+INTERNAL_RAM_SIZE)

/* ------------------------------------------------------------------------ */
/*
 * Initial Stack pointer (Used in initialization process)
 */
#define	INITIAL_SP		INTERNAL_RAM_END


/* ------------------------------------------------------------------------ */
/*
 * System configuration controller (SYSCFG)
 */

#define SYSCFG_BASE	0x40010000

#define SYSCFG_MEMRMP	(SYSCFG_BASE+0x0000)
#define SYSCFG_CFGR1	(SYSCFG_BASE+0x0004)
#define SYSCFG_EXTICR1	(SYSCFG_BASE+0x0008)
#define SYSCFG_EXTICR2	(SYSCFG_BASE+0x000C)
#define SYSCFG_EXTICR3	(SYSCFG_BASE+0x0010)
#define SYSCFG_EXTICR4	(SYSCFG_BASE+0x0014)
#define SYSCFG_SCSR	(SYSCFG_BASE+0x0018)
#define SYSCFG_CFGR2	(SYSCFG_BASE+0x001C)
#define SYSCFG_SWPR	(SYSCFG_BASE+0x0020)
#define SYSCFG_SKR	(SYSCFG_BASE+0x0024)
#define SYSCFG_SWPR2	(SYSCFG_BASE+0x0028)


/* ------------------------------------------------------------------------ */
/*
 * Internal flash memory controls
 */

#define FLASH_BASE	0x40022000

#define FLASH_ACR	(FLASH_BASE+0x0000)	/* Flash access control register */
#define FLASH_PDKEYR	(FLASH_BASE+0x0004)	/* Flash Power-down key register */
#define FLASH_KEYR	(FLASH_BASE+0x0008)	/* Flash key register */
#define FLASH_OPTKEYR	(FLASH_BASE+0x000C)	/* Flash option key register */
#define FLASH_SR	(FLASH_BASE+0x0010)	/* Flash status register */
#define FLASH_CR	(FLASH_BASE+0x0014)	/* Flash control register */
#define FLASH_ECCR	(FLASH_BASE+0x0018)	/* Flash ECC register */
#define FLASH_OPTR	(FLASH_BASE+0x0020)	/* Flash option register */
#define FLASH_PCROP1SR	(FLASH_BASE+0x0024)	/* Flash PCROP1 Start address register */
#define FLASH_PCROP1ER	(FLASH_BASE+0x0028)	/* Flash PCROP1 End address register */
#define FLASH_WRP1AR	(FLASH_BASE+0x002C)	/* Flash WRP1 area A address register */
#define FLASH_WRP1BR	(FLASH_BASE+0x0030)	/* Flash WRP1 area B address register */
#define FLASH_PCROP2SR	(FLASH_BASE+0x0044)	/* Flash PCROP2 Start address register */
#define FLASH_PCROP2ER	(FLASH_BASE+0x0048)	/* Flash PCROP2 End address register */
#define FLASH_WRP2AR	(FLASH_BASE+0x004C)	/* Flash WRP2 area A address register */
#define FLASH_WRP2BR	(FLASH_BASE+0x0050)	/* Flash WRP2 area B address register */
#define FLASH_CFGR	(FLASH_BASE+0x0130)	/* Flash configuration register */


/* FLASH_ACR bit definition */
#define FLASH_ACR_SLEEP_PD	0x00004000	// Flash Power-down mode during Sleep or Low-power sleep mode
#define FLASH_ACR_RUN_PD	0x00002000	// Flash Power-down mode during Run or Low-power run mode
#define FLASH_ACR_DCRST		0x00001000	// Data cache reset
#define FLASH_ACR_ICRST		0x00000800	// Instruction cache reset
#define FLASH_ACR_DCEN		0x00000400	// Data cache enable
#define FLASH_ACR_ICEN		0x00000200	// Instruction cache enable
#define FLASH_ACR_PRFTEN	0x00000100	// Prefetch enable
#define FLASH_ACR_LATENCY_MASK	0x00000007	// Latency
#define FLASH_ACR_LATENCY(n)	(((_UW)(n)<<0) & FLASH_ACR_LATENCY_MASK)

/* ------------------------------------------------------------------------ */
/*
 * Power & Clock Control
 */
/*
 * PWR (Power Control) registers
*/
#define PWR_BASE	0x40007000
#define PWR_CR1		(PWR_BASE+0x0000)	/* Power control register 1 */
#define PWR_CR2		(PWR_BASE+0x0004)	/* Power control register 2 */
#define PWR_CR3		(PWR_BASE+0x0008)	/* Power control register 3 */
#define PWR_CR4		(PWR_BASE+0x000C)	/* Power control register 4 */
#define PWR_SR1		(PWR_BASE+0x0010)	/* Power status register 1 */
#define PWR_SR2		(PWR_BASE+0x0014)	/* Power status register 2 */
#define PWR_SCR		(PWR_BASE+0x0018)	/* Power status clear register */
#define PWR_PUCRA	(PWR_BASE+0x0020)	/* Power Port A pull-up   control register */
#define PWR_PDCRA	(PWR_BASE+0x0024)	/* Power Port A pull-down control register */
#define PWR_PUCRB	(PWR_BASE+0x0028)	/* Power Port B pull-up   control register */
#define PWR_PDCRB	(PWR_BASE+0x002C)	/* Power Port B pull-down control register */
#define PWR_PUCRC	(PWR_BASE+0x0030)	/* Power Port C pull-up   control register */
#define PWR_PDCRC	(PWR_BASE+0x0034)	/* Power Port C pull-down control register */
#define PWR_PUCRD	(PWR_BASE+0x0038)	/* Power Port D pull-up   control register */
#define PWR_PDCRD	(PWR_BASE+0x003C)	/* Power Port D pull-down control register */
#define PWR_PUCRE	(PWR_BASE+0x0040)	/* Power Port E pull-up   control register */
#define PWR_PDCRE	(PWR_BASE+0x0044)	/* Power Port E pull-down control register */
#define PWR_PUCRF	(PWR_BASE+0x0048)	/* Power Port F pull-up   control register */
#define PWR_PDCRF	(PWR_BASE+0x004C)	/* Power Port F pull-down control register */
#define PWR_PUCRG	(PWR_BASE+0x0050)	/* Power Port G pull-up   control register */
#define PWR_PDCRG	(PWR_BASE+0x0054)	/* Power Port G pull-down control register */
#define PWR_PUCRH	(PWR_BASE+0x0058)	/* Power Port H pull-up   control register */
#define PWR_PDCRH	(PWR_BASE+0x005C)	/* Power Port H pull-down control register */
#define PWR_PUCRI	(PWR_BASE+0x0060)	/* Power Port I pull-up   control register */
#define PWR_PDCRI	(PWR_BASE+0x0064)	/* Power Port I pull-down control register */

/* PWR_CR1 bit definition */
#define PWR_CR1_LPR		0x00004000	/* Low-power mode */

#define PWR_CR1_VOS		0x00000600	/* Voltage scaling range selection */
#define PWR_CR1_VOS_RANGE1	0x00000200	/*	 Range 1 */
#define PWR_CR1_VOS_RANGE2	0x00000400	/*	 Range 2 */

#define PWR_CR1_DBP		0x00000100	/* Disable backup domain write protection */
#define PWR_CR1_LPMS		0x00000007	/* Low-power mode selection */
#define PWR_CR1_LPMS_STOP0	0x00000000	/*	000 : Stop 0 mode */
#define PWR_CR1_LPMS_STOP1	0x00000001	/*	001 : Stop 1 mode */
#define PWR_CR1_LPMS_STOP2	0x00000002	/*	010 : Stop 2 mode */
#define PWR_CR1_LPMS_STANDBY	0x00000003	/*	011 : Sandby mode */
#define PWR_CR1_LPMS_SHUTDOWN	0x00000004	/*	1xx : Shutdown mo */

/* PWR register initial value */
/* PWR_CR1 */
#define	PWR_CR1_INIT		(PWR_CR1_VOS_RANGE1) /* set High Performance Range (Range1) */

/*
 * RCC (Reset & Clock control) registers
 */
#define RCC_BASE	0x40021000
#define RCC_CR		(RCC_BASE + 0x0000)	/* Clock control register */
#define RCC_ICSCR	(RCC_BASE + 0x0004)	/* Internal clock sources calibration register */
#define RCC_CFGR	(RCC_BASE + 0x0008)	/* Clock configuration register */
#define RCC_PLLCFGR	(RCC_BASE + 0x000C)	/* PLL configuration register */
#define RCC_PLLSAI1CFGR	(RCC_BASE + 0x0010)	/* PLLSAI1 configuration register */
#define RCC_PLLSAI2CFGR	(RCC_BASE + 0x0014)	/* PLLSAI2 configuration register */
#define RCC_CIER	(RCC_BASE + 0x0018)	/* Clock interrupt enable register */
#define RCC_CIFR	(RCC_BASE + 0x001C)	/* Clock interrupt flag register */
#define RCC_CICR	(RCC_BASE + 0x0020)	/* Clock interrupt clear register */
#define RCC_AHB1RSTR	(RCC_BASE + 0x0028)	/* AHB1 peripheral reset register */
#define RCC_AHB2RSTR	(RCC_BASE + 0x002C)	/* AHB2 peripheral reset register */
#define RCC_AHB3RSTR	(RCC_BASE + 0x0030)	/* AHB3 peripheral reset register */
#define RCC_APB1RSTR1	(RCC_BASE + 0x0038)	/* APB1 peripheral reset register 1 */
#define RCC_APB1RSTR2	(RCC_BASE + 0x003C)	/* APB1 peripheral reset register 2 */
#define RCC_APB2RSTR	(RCC_BASE + 0x0040)	/* APB2 peripheral reset register */
#define RCC_AHB1ENR	(RCC_BASE + 0x0048)	/* AHB1 peripheral clock enable register */
#define RCC_AHB2ENR	(RCC_BASE + 0x004C)	/* AHB2 peripheral clock enable register */
#define RCC_AHB3ENR	(RCC_BASE + 0x0050)	/* AHB3 peripheral clock enable register */
#define RCC_APB1ENR1	(RCC_BASE + 0x0058)	/* APB1 peripheral clock enable register 1 */
#define RCC_APB1ENR2	(RCC_BASE + 0x005C)	/* APB1 peripheral clock enable register 2 */
#define RCC_APB2ENR	(RCC_BASE + 0x0060)	/* APB2 peripheral clock enable register */
#define RCC_AHB1SMENR	(RCC_BASE + 0x0068)	/* AHB1 peripheral clocks enable in Sleep and Stop modes register */
#define RCC_AHB2SMENR	(RCC_BASE + 0x006C)	/* AHB2 peripheral clocks enable in Sleep and Stop modes register */
#define RCC_AHB3SMENR	(RCC_BASE + 0x0070)	/* AHB3 peripheral clocks enable in Sleep and Stop modes register */
#define RCC_APB1SMENR1	(RCC_BASE + 0x0078)	/* APB1 peripheral clocks enable in Sleep and Stop modes register 1 */
#define RCC_APB1SMENR2	(RCC_BASE + 0x007C)	/* APB1 peripheral clocks enable in Sleep and Stop modes register 2 */
#define RCC_APB2SMENR	(RCC_BASE + 0x0080)	/* APB2 peripheral clocks enable in Sleep and Stop modes register */
#define RCC_CCIPR	(RCC_BASE + 0x0088)	/* Peripherals independent clock configuration register */
#define RCC_BDCR	(RCC_BASE + 0x0090)	/* Backup domain control register */
#define RCC_CSR		(RCC_BASE + 0x0094)	/* Control/status register */
#define RCC_CRRCR	(RCC_BASE + 0x0098)	/* Clock recovery RC register */
#define RCC_CCIPR2	(RCC_BASE + 0x009C)	/* Peripherals independent clock configuration register */

/* RCC_CR bit definition */
#define RCC_CR_PLLSAI2RDY	0x20000000	// SAI2 PLL clock ready flag
#define RCC_CR_PLLSAI2ON	0x10000000	// SAI2 PLL enable
#define RCC_CR_PLLSAI1RDY	0x08000000	// SAI1 PLL clock ready flag
#define RCC_CR_PLLSAI1ON	0x04000000	// SAI1 PLL enable
#define RCC_CR_PLLRDY		0x02000000	// Main PLL clock ready flag
#define RCC_CR_PLLON		0x01000000	// Main PLL enable
#define RCC_CR_CSSON		0x00080000	// Clock security system enable
#define RCC_CR_HSEBYP		0x00040000	// HSE crystal oscillator bypass
#define RCC_CR_HSERDY		0x00020000	// HSE clock ready flag
#define RCC_CR_HSEON		0x00010000	// HSE clock enable
#define RCC_CR_HSIASFS		0x00000800	// HSI16 automatic start from Stop
#define RCC_CR_HSIRDY		0x00000400	// HSI16 clock ready flag
#define RCC_CR_HSIKERON		0x00000200	// HSI16 always enable for peripheral kernels.
#define RCC_CR_HSION		0x00000100	// HSI16 clock enable
#define RCC_CR_MSIRANGE		0x000000F0	// MSI clock ranges
#define RCC_CR_MSIRGSEL		0x00000008	// MSI clock range selection
#define RCC_CR_MSIPLLEN		0x00000004	// MSI clock PLL enable
#define RCC_CR_MSIRDY		0x00000002	// MSI clock ready flag
#define RCC_CR_MSION		0x00000001	// MSI clock enable

/* MSI clock range value (RCC_CR_MSIRANGE) */
#define RCC_CR_MSIRANGE_100K	0x00000000	//	around 100 KHz
#define RCC_CR_MSIRANGE_200K	0x00000010	//	around 200 KHz
#define RCC_CR_MSIRANGE_400K	0x00000020	//	around 400 KHz
#define RCC_CR_MSIRANGE_800K	0x00000030	//	around 800 KHz
#define RCC_CR_MSIRANGE_1M	0x00000040	//	around   1 MHz
#define RCC_CR_MSIRANGE_2M	0x00000050	//	around   2 MHz
#define RCC_CR_MSIRANGE_4M	0x00000060	//	around   4 MHz
#define RCC_CR_MSIRANGE_8M	0x00000070	//	around   8 MHz
#define RCC_CR_MSIRANGE_16M	0x00000080	//	around  16 MHz
#define RCC_CR_MSIRANGE_24M	0x00000090	//	around  24 MHz
#define RCC_CR_MSIRANGE_32M	0x000000A0	//	around  32 MHz
#define RCC_CR_MSIRANGE_48M	0x000000B0	//	around  48 MHz

/* RCC_CFGR bit definition */
#define RCC_CFGR_MCOPRE		0x70000000	// Microcontroller clock output prescaler
#define RCC_CFGR_MCOSEL		0x0F000000	// Microcontroller clock output
#define RCC_CFGR_STOPWUCK	0x00008000	// Wakeup from Stop and CSS backup clock selection
#define RCC_CFGR_PPRE2		0x00003800	// APB high-speed prescaler (APB2)
#define RCC_CFGR_PPRE1		0x00000700	// APB low-speed prescaler (APB1)
#define RCC_CFGR_HPRE		0x000000F0	// AHB prescaler
#define RCC_CFGR_SWS		0x0000000C	// System clock switch status
#define RCC_CFGR_SW		0x00000003	// System clock switch

#define RCC_CFGR_SWS_MSI	0x00000000	// MSI used for system clock
#define RCC_CFGR_SWS_HSI16	0x00000004	// HSI16 used for system clock 
#define RCC_CFGR_SWS_HSE	0x00000008	// HSE used for system clock
#define RCC_CFGR_SWS_PLL	0x0000000C	// PLL used for system clock

#define RCC_CFGR_SW_MSI		0x00000000	// Use MSI for system clock
#define RCC_CFGR_SW_HSI16	0x00000001	// Use HSI16 for system clock
#define RCC_CFGR_SW_HSE		0x00000002	// Use HSE for system clock
#define RCC_CFGR_SW_PLL		0x00000003	// Use PLL for system clock

/* RCC_PLLCFGR bit definition */
#define RCC_PLLCFGR_PLLR	0x06000000	// Main PLL division factor for PLLCLK (system clock)
#define RCC_PLLCFGR_PLLREN	0x01000000	// Main PLL PLLCLK output enable
#define RCC_PLLCFGR_PLLQ	0x00600000	// Main PLL division factor for PLL48M1CLK (48 MHz clock).
#define RCC_PLLCFGR_PLLQEN	0x00100000	// Main PLL PLL48M1CLK output enable
#define RCC_PLLCFGR_PLLP	0x00020000	// Main PLL division factor for PLLSAI3CLK (SAI1 and SAI2 clock).
#define RCC_PLLCFGR_PLLPEN	0x00010000	// Main PLL PLLSAI3CLK output enable
#define RCC_PLLCFGR_PLLN	0x00007F00	// Main PLL multiplication factor for VCO
#define RCC_PLLCFGR_PLLM	0x000000F0	// Division factor for the main PLLinput clock
#define RCC_PLLCFGR_PLLSRC	0x00000003	// Main PLL entry clock source

#define RCC_PLLCFGR_PLLSRC_NON	0x00000000	// No clock
#define RCC_PLLCFGR_PLLSRC_MSI	0x00000001	// Use MSI for system clock
#define RCC_PLLCFGR_PLLSRC_HSI	0x00000002	// Use HSI16 for system clock
#define RCC_PLLCFGR_PLLSRC_HSE	0x00000003	// Use HSE for system clock

/* RCC_APB1ENR1 bit definition */
#define RCC_APB1ENR1_LPTIM1EN	0x80000000	// Low power timer 1 clock enable
#define RCC_APB1ENR1_OPAMPEN	0x40000000	// OPAMP interface clock enable
#define RCC_APB1ENR1_DAC1EN	0x20000000	// DAC1 interface clock enable
#define RCC_APB1ENR1_PWREN	0x10000000	// Power interface clock enable
#define RCC_APB1ENR1_CAN1EN	0x02000000	// CAN1 clock enable
#define RCC_APB1ENR1_CRSEN	0x01000000	// Clock Recovery System clock enable
#define RCC_APB1ENR1_I2C3EN	0x00800000	// I2C3 clock enable
#define RCC_APB1ENR1_I2C2EN	0x00400000	// I2C2 clock enable
#define RCC_APB1ENR1_I2C1EN	0x00200000	// I2C1 clock enable
#define RCC_APB1ENR1_UART5EN	0x00100000	// UART5 clock enable
#define RCC_APB1ENR1_UART4EN	0x00080000	// UART4 clock enable
#define RCC_APB1ENR1_USART3EN	0x00040000	// USART3 clock enable
#define RCC_APB1ENR1_USART2EN	0x00020000	// USART2 clock enable
#define RCC_APB1ENR1_SPI3EN	0x00008000	// SPI3 clock enable
#define RCC_APB1ENR1_SPI2EN	0x00004000	// SPI2 clock enable
#define RCC_APB1ENR1_WWDGEN	0x00000800	// Window watchdog clock enable
#define RCC_APB1ENR1_RTCAPBEN	0x00000400	// RTC APB clock enable
#define RCC_APB1ENR1_TIM7EN	0x00000020	// TIM7 timer clock enable
#define RCC_APB1ENR1_TIM6EN	0x00000010	// TIM6 timer clock enable
#define RCC_APB1ENR1_TIM5EN	0x00000008	// TIM5 timer clock enable
#define RCC_APB1ENR1_TIM4EN	0x00000004	// TIM4 timer clock enable
#define RCC_APB1ENR1_TIM3EN	0x00000002	// TIM3 timer clock enable
#define RCC_APB1ENR1_TIM2EN	0x00000001	// TIM2 timer clock enable

/* RCC_APB1ENR2 bit definition */
#define RCC_APB1ENR2_LPTIM2EN	0x00000020	// Low power timer 2 clock enable
#define	RCC_APB1ENR2_SWPMI1EN	0x00000004	// Single wire protocol clock enable
#define RCC_APB1ENR2_I2C4EN	0x00000002	// I2C4 clock enable
#define RCC_APB1ENR2_LPUART1EN	0x00000001	// Low power UART 1 clock enable

/* RCC_APB2ENR bit definition */
#define RCC_APB2ENR_DFSDM1EN	0x01000000	// DFSDM 1 Timer clock enable
#define RCC_APB2ENR_SAI2EN	0x00400000	// SAI 2 clock enable
#define RCC_APB2ENR_SAI1EN	0x00200000	// SAI1 clock enable
#define RCC_APB2ENR_TIM17EN	0x00040000	// TIM 17 Enable timer clock
#define RCC_APB2ENR_TIM16EN	0x00020000	// TIM16 Enable timer clock
#define RCC_APB2ENR_TIM15EN	0x00010000	// TIM 15 Enable timer clock
#define RCC_APB2ENR_USART1EN	0x00004000	// USART1 clock enable
#define RCC_APB2ENR_TIM8EN	0x00002000	// TIM 8 Timer clock enable
#define RCC_APB2ENR_SPI1EN	0x00001000	// SPI 1 clock enable
#define RCC_APB2ENR_TIM1EN	0x00000800	// TIM1 Enable timer clock
#define RCC_APB2ENR_SDMMC1EN	0x00000400	// Enable SDMMC clock
#define RCC_APB2ENR_FWEN	0x00000080	// Enable Firewall Clock
#define RCC_APB2ENR_SYSCFGEN	0x00000001	// SYSCFG + COMP + VREFBUF clock enable

/* RCC_AHB2ENR bit definition */
#define	RCC_AHB2ENR_GPIOAEN	0x00000001	// GPIOA clock enable
#define	RCC_AHB2ENR_GPIOBEN	0x00000002	// GPIOB clock enable
#define	RCC_AHB2ENR_GPIOCEN	0x00000004	// GPIOC clock enable
#define	RCC_AHB2ENR_GPIODEN	0x00000008	// GPIOD clock enable
#define	RCC_AHB2ENR_GPIOEEN	0x00000010	// GPIOE clock enable
#define	RCC_AHB2ENR_GPIOFEN	0x00000020	// GPIOF clock enable
#define	RCC_AHB2ENR_GPIOGEN	0x00000040	// GPIOG clock enable
#define	RCC_AHB2ENR_GPIOHEN	0x00000080	// GPIOH clock enable
#define	RCC_AHB2ENR_GPIOIEN	0x00000100	// GPIOI clock enable
#define	RCC_AHB2ENR_ADCEN	0x00002000	// ADC clock enable

/* RCC_CCIPR bit definition */
#define	RCC_CCIPR_USARTxSEL	0x000003FF	// USART clock select
#define	RCC_CCIPR_LPUART1SEL	0x00000C00	// LPUART1 clock select
#define	RCC_CCIPR_I2CxSEL	0x0003F000	// I2Cx clock select
#define	RCC_CCIPR_LPTIMxSEL	0x003C0000	// LPTIMx clock select
#define	RCC_CCIPR_SAIxSEL	0x03C00000	// SAIx clock select
#define	RCC_CCIPR_CLK48SEL	0x0C000000	// CLK48 clock select
#define	RCC_CCIPR_ADCSEL	0x30000000	// ADC clock select
#define	RCC_CCIPR_SWPMI1SEL	0x40000000	// SWPMI1 clock select
#define	RCC_CCIPR_DFSDM1SEL	0x80000000	// DFSDM1 clock select


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
#define N_INTVEC		82	/* Number of Interrupt vectors */
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
#define EXTI_BASE	0x40010400

#define	EXTI_IMR1	(EXTI_BASE + 0x00)
#define	EXTI_EMR1	(EXTI_BASE + 0x04)
#define	EXTI_RTSR1	(EXTI_BASE + 0x08)
#define	EXTI_FTSR1	(EXTI_BASE + 0x0C)
#define	EXTI_SWIER1	(EXTI_BASE + 0x10)
#define	EXTI_PR1	(EXTI_BASE + 0x14)
#define	EXTI_IMR2	(EXTI_BASE + 0x20)
#define	EXTI_EMR2	(EXTI_BASE + 0x24)
#define	EXTI_RTSR2	(EXTI_BASE + 0x28)
#define	EXTI_FTSR2	(EXTI_BASE + 0x2C)
#define	EXTI_SWIER2	(EXTI_BASE + 0x30)
#define	EXTI_PR2	(EXTI_BASE + 0x34)

/* ------------------------------------------------------------------------ */
/*
 * Watchdog Timer
 */

/* ------------------------------------------------------------------------ */
/*
 * GPIO
 */
#define	GPIOA_BASE	0x48000000
#define	GPIOB_BASE	0x48000400
#define	GPIOC_BASE	0x48000800
#define	GPIOD_BASE	0x48000C00
#define	GPIOE_BASE	0x48001000
#define	GPIOF_BASE	0x48000400
#define	GPIOG_BASE	0x48000800
#define	GPIOH_BASE	0x48000C00
#define	GPIOI_BASE	0x48002000

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
#define GPIO_BRR(n)	(GPIO##n##_BASE + 0x28)		/* GPIO port bit reset register */
#define	GPIO_ASCR(n)	(GPIO##n##_BASE + 0x2C)		/* GPIO port analog switch control register */

/* ------------------------------------------------------------------------ */
/*
 * Physical timer (for STM32L4)
 */
#define	CPU_HAS_PTMR	(1)

/* Timer register definition */
#define	TIM2_BASE	0x40000000
#define	TIM3_BASE	0x40000400
#define	TIM4_BASE	0x40000800
#define	TIM5_BASE	0x40000C00
#define	TIM6_BASE	0x40001000
#define	TIM7_BASE	0x40001400

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
#define TIMxOR1		0x50
#define TIMxOR2		0x60

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
