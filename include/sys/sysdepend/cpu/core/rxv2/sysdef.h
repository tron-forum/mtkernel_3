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
 *	System dependencies definition (RXv2 core depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_CORE_H__
#define __SYS_SYSDEF_DEPEND_CORE_H__

/* ------------------------------------------------------------------------ */
/*
 * PSW : Processor Status Word
 */
#define PSW_I		(0x000010000)	/* bit-16: Interrupt enable bit */
#define PSW_IPL		(0x00F000000)	/* bit 24~27: IPL(Interrupt priority level) */

/* ------------------------------------------------------------------------ */
/*
 * FPSW : Floating point Status Word
 */
#define FPSW_VAL	(0x00000100)	/* FPSW initial value */

/* ------------------------------------------------------------------------ */
/*
 * SYSTEM module registers
 */

/* Register write protection register */
#define SYSTEM_PRCR	(0x000803FE)

/* Clock generation registers */
#define SYSTEM_SCKCR	(0x00080020)
#define SYSTEM_SCKCR2	(0x00080024)
#define SYSTEM_SCKCR3	(0x00080026)
#define SYSTEM_PLLCR	(0x00080028)
#define SYSTEM_PLLCR2	(0x0008002A)
#define SYSTEM_UPLLCR	(0x0008002C)
#define SYSTEM_UPLLCR2	(0x0008002E)
#define SYSTEM_BCKCR	(0x00080030)
#define SYSTEM_MEMWAIT	(0x00080031)
#define SYSTEM_MOSCCR	(0x00080032)
#define SYSTEM_SOSCCR	(0x00080033)
#define SYSTEM_LOCOCR	(0x00080034)
#define SYSTEM_ILOCOCR	(0x00080035)
#define SYSTEM_HOCOCR	(0x00080036)
#define SYSTEM_HOCOCR2	(0x00080037)
#define SYSTEM_OSCOVFSR	(0x0008003C)
#define SYSTEM_OSTDCR	(0x00080040)
#define SYSTEM_OSTDSR	(0x00080041)
#define SYSTEM_OPCCR	(0x000800A0)
#define	SYSTEM_RSTCKCR	(0x000800A1)
#define SYSTEM_MOSCWTCR	(0x000800A2)
#define SYSTEM_SOSCWTCR	(0x000800A3)
#define SYSTEM_ROMWT	(0x0008101C)
#define SYSTEM_MOFCR	(0x0008C293)
#define SYSTEM_HOCOPCR	(0x0008C294)

/* Reset control registers */
#define SYSTEM_RSTSR0	(0x0008C290)
#define SYSTEM_RSTSR1	(0x0008C291)
#define SYSTEM_RSTSR2	(0x000800C0)
#define SYSTEM_SWRR	(0x000800C2)

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


#endif /* __SYS_SYSDEF_DEPEND_CORE_H__ */
