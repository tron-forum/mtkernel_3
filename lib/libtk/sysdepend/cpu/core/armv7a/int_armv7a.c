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

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7A

/*
 *	int_armv7a.c
 *
 *	Interrupt controller (ARMv7-A)
 */

#include <tk/tkernel.h>

/*----------------------------------------------------------------------*/
/*
 * CPU Interrupt Control for ARMv7-A.
 *
 */

/* 
 * Disable interrupt (IRQ)
 *	Set CPSR.I value to 1. And return the original CPSR.I value.
 */
EXPORT UW disint(void)
{
	UW	intsts;

	Asm("mrs %0, cpsr": "=r"(intsts));	/* Get CPSR reg. */
	Asm("cpsid I");				/* Disable IRQ */

	return intsts & PSR_I;
}

/* 
 * Enable interrupt (IRQ)
 *	Sets the CPSR.I value to the specified value..
 */
EXPORT void enaint( UW intsts )
{
	if(intsts & PSR_I) {
		Asm("cpsid I");		/* Disable IRQ */
	} else {
		Asm("cpsie I");		/* Enable IRQ */
	}
	return;
}

/*----------------------------------------------------------------------*/
/*
 * Arm Generic Interrupt Controller (GIC))
 *
 */

/*
 * Set Interrupt Mask Level in Interrupt Controller
 */
EXPORT void SetCtrlIntLevel( INT level )
{
	_UW	dummy;

	if (level < INTPRI_HIGHEST || level >= INTPRI_LOWEST) return;	/* Error */

	level = ((++level)<<INTPRI_SHIFT) & 0x00FF;	/* bit7-3 is valid */

	out_w(GICC_PMR, (UW)level);
	dummy = in_w(GICC_PMR);
	(void)dummy;				/* Warning avoidance */

	return;
}

/*
 * Get Interrupt Mask Level in Interrupt Controller
 */
EXPORT INT GetCtrlIntLevel( void )
{
	UW level;

	level = in_w(GICC_PMR);
	level = (level & 0x00FF) >>INTPRI_SHIFT;	/* bit7-3 is valid */

	return (INT)(--level);
}

/*
 * Enable interrupt 
 *	Enables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	UW	pri, icdipr;
	UINT	shift;
	UW	addr;

	if (( intno >= N_INTVEC) 
		|| (level < INTPRI_HIGHEST) || (level > INTPRI_LOWEST)) return;	/* Error */

	/* Set priority (GICD.IPRIORITYR reg) */
	pri = ((UW)level & 0x00FF) << INTPRI_SHIFT;	/* bit7-3 is valid */

	addr = (UW)GICD_IPRIORITYR(intno>>2);
	shift = (intno & 0x03) << 3;	/* (intno % 4) * 8 */

	icdipr = in_w(addr);
	icdipr &= ~(0x000000ff << shift);
	icdipr |= pri << shift;
	out_w(addr , icdipr);

	/* Enable Interrupt */
	out_w(GICD_ISENABLER(intno>>5), 1<<(intno&0x1F));

	return;
}

/*
 * Disable interrupt 
 *	Disables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void DisableInt( UINT intno )
{
	if (intno >= N_INTVEC) return;		/* Error */

	out_w(GICD_ICENABLER(intno>>5), 1<<(intno&0x1F));

	return;
}

/*
 * Set interrupt mode
 *	Set the interrupt mode specified by `intvec' to the mode given
 *  	by `mode'. If an illegal mode is given, subsequent correct behavior
 *      of the system is not guaranteed.
 */
EXPORT void SetIntMode(  UINT intno, UINT mode )
{
	UW	bit;
	_UW	*addr;

	if (intno >= N_INTVEC || mode > IM_EDGE) return;	/* Error */

	addr = (_UW*)GICD_ICFGR(intno>>4);
	bit = 1 << (((intno&0x0F)<<1) + 1);

	if (IM_LEVEL == mode) {
		*addr &= ~bit;		/* high level detection */
	} else {
		*addr |= bit;		/* Rising edge detection */
	}

	return;
}

/*
 * Clear Interrupt
 *	Un-pends the associated interrupt under software control.
 *	External Interrupt can be specified. 
 */
EXPORT void ClearInt( UINT intno )
{
	if (intno >= N_INTVEC) return;		/* Error */

	out_w(GICD_ICPENDR(intno>>5), 1<<(intno&0x1F));

	return;
}

/*
 * Check active state
 *	Current active state for the associated interrupt
 *	External Interrupt can be specified. 
 */
EXPORT BOOL CheckInt( UINT intno )
{
	UW	data;

	if (intno >= N_INTVEC) return FALSE;	/* Error */

	data = in_w(GICD_ICPENDR(intno>>5));
	return (data & (1 << (intno&0x1F))) ? TRUE : FALSE;
}

EXPORT void EndOfInt( UINT intno )
{
	if (intno >= N_INTVEC) return;	/* Error */

	out_w(GICC_EOIR, intno);
	
	return;
}


#endif /* CPU_CORE_ARMV7A */
