/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7M

/*
 *	int_armv7m.c
 *
 *	Interrupt controller (ARMv7-M)
 */

#include <tk/tkernel.h>

#include "int_armv7m.h"

/*----------------------------------------------------------------------*/
/*
 * CPU Interrupt Control for ARM Cortex-M4.
 *
 */

/*
 * Set Base Priority register
 */
EXPORT void set_basepri(UW intsts)
{	
	Asm("msr basepri, %0":: "r"(intsts));
}

/*
 * Get Base Priority register
 */
EXPORT UW get_basepri(void)
{
	UW	basepri;

	Asm("mrs %0, basepri": "=r"(basepri));
	return basepri;
}

/* 
 * Disable interrupt 
 */
EXPORT UW disint(void)
{
	UW	intsts, maxint;

	maxint = INTPRI_VAL(INTPRI_MAX_EXTINT_PRI);
	Asm("mrs %0, basepri": "=r"(intsts));
	Asm("msr basepri, %0":: "r"(maxint));

	return intsts;
}

/*
 * Set Interrupt Mask Level in CPU
 */
EXPORT void SetCpuIntLevel( INT level )
{
	set_basepri((level+1) << (8-INTPRI_BITWIDTH));
}

/*
 * Get Interrupt Mask Level in CPU
 */
EXPORT INT GetCpuIntLevel( void )
{
	INT	lv;

	lv = (INT)(get_basepri() >>(8-INTPRI_BITWIDTH)) -1;
	return lv<0?INTLEVEL_EI:lv;
}


/*----------------------------------------------------------------------*/
/*
 * Interrupt controller (NVIC) Control
 *
 */

/*
 * Enable interrupt for NVIC
 */
EXPORT void EnableInt_nvic( UINT intno, INT level )
{
	UINT	imask;
	
	DI(imask);
	/* Set interrupt priority level. */
	*(_UB*)(NVIC_IPR(intno)) = (UB)INTPRI_GROUP(level, 0);

	/* Enables the specified interrupt. */
	*(_UW*)(NVIC_ISER(intno)) = (0x01U << (intno % 32));

	EI(imask);
}



#endif /* CPU_CORE_ARMV7M */
