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
 *	sysdef_depend.h
 *
 *	System dependencies definition (ARMv7-A core depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_CORE_H__
#define __SYS_SYSDEF_DEPEND_CORE_H__

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
#define CP15_SCTLR_I		0x00001000	/* */
#define CP15_SCTLR_Z		0x00000800	/* */
#define CP15_SCTLR_C		0x00000004
#define CP15_SCTLR_A		0x00000002
#define CP15_SCTLR_M		0x00000001
#define CP15_SCTLR_V		0x00002000

/*
 * CP15 Auxiliary Control Register(ACTLR)
 */
#define CP15_ACTLR_DPF		0x00000004


/* ------------------------------------------------------------------------ */

/*
 * Parameter of SVC instruction 
 */
#define	SVC_SYSCALL		    6	/* micro T-Kernel system call */
#define	SVC_FORCE_DISPATCH	7	/* force dispatch */
#define	SVC_DISPATCH		8	/* task dispatcher */
#define	SVC_DEBUG_SUPPORT	9	/* debug support function */
#define	SVC_EXTENDED_SVC	10	/* Extended SVC */

/* ------------------------------------------------------------------------ */

/*
 * INTC(GIC) register
 */
#define GIC_ICC_BASE		0xE8222000
#define GIC_ICC_ICR_OFF		0x0000	/* Control Register　*/
    #define GIC_ICC_ENABLE	0x00000001
#define GIC_ICC_PMR_OFF		0x0004	/* Priority Mask Register */
#define GIC_ICC_BPR_OFF		0x0008	/* Binary Point Register */
#define GIC_ICC_IAR_OFF		0x000C	/* Interrupt Acknowledge Register */
#define GIC_ICC_EOIR_OFF	0x0010	/* End of Interrupt Register */
#define GIC_ICC_RPR_OFF		0x0014	/* Running Priority Register */
#define GIC_ICC_HPIR_OFF	0x0018	/* Highest Pending Interrupt Register */

#define GIC_ICD_ISACTIVER0  0xE8221300	/* Active set register 0 */

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
#define	FPSCR_UFC	0x00000008	/*		undeflow */
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
#define MIN_SYS_STACK_SIZE	128

/*
 * Default task system stack 
 */

#define DEFAULT_SYS_STKSZ	MIN_SYS_STACK_SIZE

/* ------------------------------------------------------------------------ */

#endif /* __SYS_SYSDEF_DEPEND_CORE_H__ */
