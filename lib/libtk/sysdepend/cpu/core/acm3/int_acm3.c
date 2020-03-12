/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03/13.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ACM3

/*
 *	int.c
 *
 *	Interrupt controller (ARM Cortex-M3)
 */

#include <tk/tkernel.h>

/*----------------------------------------------------------------------*/
/*
 * CPU Interrupt Control for ARM Cortex-M3.
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
	return (INT)((get_basepri() >> (8-INTPRI_BITWIDTH))-1);
}


/*----------------------------------------------------------------------*/
/*
 * Interrupt controller (NVIC) Control
 *
 */

/*
 * Enable interrupt 
 *	Enables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	UINT	imask;
	
	DI(imask);
	/* Set interrupt priority level. */
	*(_UB*)(NVIC_IPR(intno)) = (UB)INTPRI_GROUP(level, 0);

	/* Enables the specified interrupt. */
	*(_UW*)(NVIC_ISER(intno)) = (0x01U << (intno % 32));

	EI(imask);
}

/*
 * Disable interrupt 
 *	Disables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void DisableInt( UINT intno )
{
	*(_UW*)(NVIC_ICER(intno)) = (0x01U << (intno % 32));
}

/*
 * Issue EOI to interrupt controller
 */
EXPORT void EndOfInt(UINT intno)
{
	/* No opetarion. */
}

/*
 * Check active state
 *	Current active state for the associated interrupt
 *	External Interrupt can be specified. 
 */
EXPORT BOOL CheckInt( UINT intno )
{
	return (*(_UW*)(NVIC_ICPR(intno)) & (0x01U << (intno % 32)));
}

/*
 * Set-Pending
 *  Pends the associated interrupt under software control.
 *	External Interrupt can be specified. 
 */
EXPORT void SetPendingInt( UINT intno )
{
	*(_UW*)(NVIC_ISPR(intno)) = (0x01U << (intno % 32));
}

/*
 * Clear-Pending
 *	Un-pends the associated interrupt under software control.
 *	External Interrupt can be specified. 
 */
EXPORT void ClearPendingInt( UINT intno )
{
	*(_UW*)(NVIC_ICPR(intno)) = (0x01U << (intno % 32));
}


#endif /* CPU_CORE_ACM3 */
