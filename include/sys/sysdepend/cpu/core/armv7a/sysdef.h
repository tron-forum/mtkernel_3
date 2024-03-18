/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef_depend.h
 *
 *	System dependencies definition (ARMv7-A core depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_CORE_H__
#define __SYS_SYSDEF_DEPEND_CORE_H__

/* ------------------------------------------------------------------------ */
/*
 * Program Status Register (PSR)
 */
#define	PSR_N		0x80000000	/* conditional flag  negative */
#define	PSR_Z		0x40000000	/*		     zero */
#define	PSR_C		0x20000000	/*		     carry */
#define	PSR_V		0x10000000	/*		     overflow */
#define	PSR_Q		0x08000000	/* Sticky overflow */
#define	PSR_GE		0x000f0000	/* SIMD >= flag  */

#define	PSR_J		0x01000000	/* Jazelle mode */
#define	PSR_IT		0x0600fc00	/* Thumb If-Then state */
#define	PSR_E		0x00000200	/* data endian */
#define	PSR_A		0x00000100	/* disable asynchronous abort */
#define	PSR_I		0x00000080	/* disable interrupt(IRQ) */
#define	PSR_F		0x00000040	/* disable fast interrupt(FIQ) */
#define	PSR_T		0x00000020	/* Thumb mode */

#define	PSR_M(n)	( n )		/* processor mode 0-31 */
#define	PSR_USR		PSR_M(16)	/* user mode */
#define	PSR_FIQ		PSR_M(17)	/* fast interrupt(FIQ) mode */
#define	PSR_IRQ		PSR_M(18)	/* interrupt (IRQ) mode */
#define	PSR_SVC		PSR_M(19)	/* supervisor mode */
#define	PSR_ABT		PSR_M(23)	/* abort mode */
#define	PSR_UND		PSR_M(27)	/* undefined instruction mode */
#define	PSR_SYS		PSR_M(31)	/* system mode */

#define PSR_DI		( PSR_I|PSR_A )	/* disable (ordinary) interrupt */

/*
 * CP15 System Control Register (SCTLR)
 */
#define CP15_SCTLR_I	0x00001000
#define CP15_SCTLR_Z	0x00000800
#define CP15_SCTLR_C	0x00000004
#define CP15_SCTLR_A	0x00000002
#define CP15_SCTLR_M	0x00000001
#define CP15_SCTLR_V	0x00002000

/*
 * CP15 Auxiliary Control Register(ACTLR)
 */
#define CP15_ACTLR_DPF		0x00000004


/* ------------------------------------------------------------------------ */
/*
 * Parameter of SVC instruction 
 */
#define	SVC_SYSCALL		6	/* micro T-Kernel system call */
#define	SVC_FORCE_DISPATCH	7	/* force dispatch */
#define	SVC_DISPATCH		8	/* task dispatcher */
#define	SVC_DEBUG_SUPPORT	9	/* debug support function */
#define	SVC_EXTENDED_SVC	10	/* Extended SVC */

/* ------------------------------------------------------------------------ */
/*
 * Number of SVC handlers
 */
#define N_SVCHDR		(11)	/* Number of SVC handlers ( used by micro T-Kernel) */


/* ------------------------------------------------------------------------ */
/*
 * GIC(Generic Interrupt Controller) register
 * 	GIC-400 
 */

/* Distributor register address */
#define GICD_CTLR		(GICD_BASE + 0x0000)		/* Distributor Control Register */
#define GICD_TYPER		(GICD_BASE + 0x0004)		/* Interrupt Controller Type Register */
#define GICD_IIDR		(GICD_BASE + 0x0008)		/* Distributor Implementer Identification Register */

#define GICD_IGROUPR(n)		(GICD_BASE + 0x0080 + (0x04*(n)))	/* Interrupt Group Registerse */
#define GICD_ISENABLER(n)	(GICD_BASE + 0x0100 + (0x04*(n)))	/* Interrupt Set-Enable Registers */
#define GICD_ICENABLER(n)	(GICD_BASE + 0x0180 + (0x04*(n)))	/* Interrupt Clear-Enable Registers */
#define GICD_ISPENDR(n)		(GICD_BASE + 0x0200 + (0x04*(n)))	/* Interrupt Set-Pending Registers */
#define GICD_ICPENDR(n)		(GICD_BASE + 0x0280 + (0x04*(n)))	/* Interrupt Clear-Pending Registers */
#define GICD_ISACTIVER(n)	(GICD_BASE + 0x0300 + (0x04*(n)))	/* Interrupt Set-Active Registers */
#define GICD_ICACTIVER(n)	(GICD_BASE + 0x0380 + (0x04*(n)))	/* Interrupt Clear-Active Registers */
#define GICD_IPRIORITYR(n)	(GICD_BASE + 0x0400 + (0x04*(n)))	/* Interrupt Priority Registers */
#define GICD_ITARGETR(n)	(GICD_BASE + 0x0800 + (0x04*(n)))	/* Interrupt Processor Targets Registersi */
#define GICD_ICFGR(n)		(GICD_BASE + 0x0C00 + (0x04*(n)))	/* Interrupt Configuration Registers */
#define GICD_PPISR		(GICD_BASE + 0x0D00)		/* Private Peripheral Interrupt Status Register */
#define GICD_SPISR(n)		(GICD_BASE + 0x0D04 + (0x04*(n)))	/* Shared Peripheral Interrupt Status Registers */
#define GICD_SGIR		(GICD_BASE + 0x0F00)		/* Software Generated Interrupt Register */
#define GICD_CPENDSGIR(n)	(GICD_BASE + 0x0F10 + (0x04*(n))) /* SGI Clear-Pending Registers */
#define GICD_SPENDSGIR(n)	(GICD_BASE + 0x0F20 + (0x04*(n))) /* SGI Set-Pending Registers */

#define GICD_PIDR4		(GICD_BASE + 0x0FD0)		/* Peripheral ID 4 Register */
#define GICD_PIDR5		(GICD_BASE + 0x0FD4)		/* Peripheral ID 5 Register */
#define GICD_PIDR6		(GICD_BASE + 0x0FD8)		/* Peripheral ID 6 Register */
#define GICD_PIDR7		(GICD_BASE + 0x0FDC)		/* Peripheral ID 7 Register */
#define GICD_PIDR0		(GICD_BASE + 0x0FE0)		/* Peripheral ID 0 Register */
#define GICD_PIDR1		(GICD_BASE + 0x0FE4)		/* Peripheral ID 1 Register */
#define GICD_PIDR2		(GICD_BASE + 0x0FE8)		/* Peripheral ID 2 Register */
#define GICD_PIDR3		(GICD_BASE + 0x0FEC)		/* Peripheral ID 3 Register */

#define GICD_CIDR0		(GICD_BASE + 0x0FF0)		/* Component ID 0 Register */
#define GICD_CIDR1		(GICD_BASE + 0x0FF4)		/* Component ID 1 Register */
#define GICD_CIDR2		(GICD_BASE + 0x0FF8)		/* Component ID 2 Register */
#define GICD_CIDR3		(GICD_BASE + 0x0FFC)		/* Component ID 3 Register */

/* Number of registers*/
#define GICD_IGROUPR_N		16
#define GICD_ICFGR_N		32
#define GICD_IPRIORITYR_N	128
#define GICD_ITARGETR_N		128
#define GICD_ISENABLER_N	16
#define GICD_ICENABLER_N	16

/* CPU interface register address */
#define GICC_CTLR		(GICC_BASE + 0x0000)		/* CPU Interface Control Register */
#define GICC_PMR		(GICC_BASE + 0x0004)		/* Interrupt Priority Mask Register */
#define GICC_BPR		(GICC_BASE + 0x0008)		/* Binary Point Register */
#define GICC_IAR		(GICC_BASE + 0x000C)		/* Interrupt Acknowledge Register */
#define GICC_EOIR		(GICC_BASE + 0x0010)		/* End of Interrupt Register */
#define GICC_RPR		(GICC_BASE + 0x0014)		/* Running Priority Register */
#define GICC_HPPIR		(GICC_BASE + 0x0018)		/* Highest Priority Pending Interrupt Register */
#define GICC_ABPR		(GICC_BASE + 0x001C)		/* Aliased Binary Point Register */
#define GICC_AIAR		(GICC_BASE + 0x0020)		/* Aliased Interrupt Acknowledge Register */
#define GICC_AEOIR		(GICC_BASE + 0x0024)		/* Aliased End of Interrupt Register */
#define GICC_AHPPIR		(GICC_BASE + 0x0028)		/* Aliased Highest Priority Pending Interrupt Register */
#define GICC_APR0		(GICC_BASE + 0x00D0)		/* Active Priority Register */
#define GICC_NSAPR0		(GICC_BASE + 0x00E0)		/* Non-Secure Active Priority Register */
#define GICC_IIDR		(GICC_BASE + 0x00FC)		/* CPU Interface Identification Register */
#define GICC_DIR		(GICC_BASE + 0x1000)		/* Deactivate Interrupt Register */


/*
 * VFP Status/Control Register (FPSCR)
 */
#define	FPSCR_N		0x80000000	/* Cond. flag	minus */
#define	FPSCR_Z		0x40000000	/*		zero */
#define	FPSCR_C		0x20000000	/*		carry */
#define	FPSCR_V		0x10000000	/*		overflow */
#define	FPSCR_AHP	0x04000000	/* Alternative Half-Precision */
#define	FPSCR_DN	0x02000000	/* Default NaN */
#define	FPSCR_FZ	0x01000000	/* Flush to zeto */
#define	FPSCR_RMODE(n)	( (n) << 22 )	/* Round mode */
#define	FPSCR_STRIDE(n)	( (n) << 20 )	/* Short Vector */
#define	FPSCR_LEN(n)	( (n) << 16 )	/* Short Vector */
#define	FPSCR_IDE	0x00008000	/* Enable Trap	input denormal */
#define	FPSCR_IXE	0x00001000	/*		inexact */
#define	FPSCR_UFE	0x00000800	/*		undefflow */
#define	FPSCR_OFE	0x00000400	/*		overflow */
#define	FPSCR_DZE	0x00000200	/*		division by zero */
#define	FPSCR_IOE	0x00000100	/*		invalid operation */
#define	FPSCR_IDC	0x00000080	/* Except.Flag	input denormal */
#define	FPSCR_IXC	0x00000010	/*		inexact */
#define	FPSCR_UFC	0x00000008	/*		underflow */
#define	FPSCR_OFC	0x00000004	/*		overflow */
#define	FPSCR_DZC	0x00000002	/*		division by zero */
#define	FPSCR_IOC	0x00000001	/*		invalid operation */

#define FPSCR_INIT	0x00000000	/* Init value (Full-compliance mode) */


/* ------------------------------------------------------------------------ */
/*
 * memory barrier instruction
 *	ISB()	Instruction Synchronization Barrier
 *	DSB()	Data Synchronization Barrier
 *	DMB()	Data Memory Barrier
 */
#define ISB()		Asm("isb")
#define DSB()		Asm("dsb")
#define DMB()		Asm("dmb")

/* ------------------------------------------------------------------------ */
/*
 * Definition of minimum system stack size
 *	Minimum system stack size when setting the system stack size
 *	per task by 'tk_cre_tsk().'
 *  this size must be larger than the size of SStackFrame
 */
#if USE_FPU
#define MIN_SYS_STACK_SIZE	128+256
#else
#define MIN_SYS_STACK_SIZE	128
#endif /* USE_FPU */
/*
 * Default task system stack 
 */

#define DEFAULT_SYS_STKSZ	(MIN_SYS_STACK_SIZE)

/* ------------------------------------------------------------------------ */
/* 
 *	Exception Stack Size
 */
#define	FIQ_STACK_SIZE	(256)	/* FIQ exception stack size */
#define	UND_STACK_SIZE	(256)	/* Undefined instruction exception stack size */
#define	ABT_STACK_SIZE	(256)	/* Abort exception stack size */

/* ------------------------------------------------------------------------ */
/* 
 *	FPU control during interrupt handler execution
 *		0: do nothing    1: Disable FPU
 */
#define	INTHDR_DIS_FPU	1

#endif /* __SYS_SYSDEF_DEPEND_CORE_H__ */
