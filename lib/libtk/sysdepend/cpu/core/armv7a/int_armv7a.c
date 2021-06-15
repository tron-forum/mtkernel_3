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

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7A

/*
 *	int_armv7a.c
 *
 *	Interrupt controller (ARMv7-A)
 */

#include <tk/tkernel.h>

#include "int_armv7a.h"

/*----------------------------------------------------------------------*/
/*
 * CPU Interrupt Control for ARMv7-A.
 *
 */

/*
 * Interrupt priority mask setting
 */
LOCAL void set_mask_level(UB lv)
{
	_UB	dummy;

	if (lv > INTPRI_LOWEST) return;

	out_w(GICC_PMR, lv << INTPRI_SHIFT);	/* bit7-3 is valid */
	dummy = in_w(GICC_PMR);
	(void)dummy;			/* Warning avoidance */
}

/*
 * Set Interrupt Mask Level in CPU
 */
EXPORT void SetCpuIntLevel( INT level )
{
	UB	lv;
	_UB	dummy;

	if (level < INTPRI_HIGHEST || level > INTPRI_LOWEST) return;	/* Error */

	lv = (UB)level << INTPRI_SHIFT;		/* bit7-3 is valid */

	out_w(GICC_PMR, (UW)lv);
	dummy = in_w(GICC_PMR);
	(void)dummy;				/* Warning avoidance */
	return;
}

/*
 * Get Interrupt Mask Level in CPU
 */
EXPORT INT GetCpuIntLevel( void )
{
	UB lv;

	lv =in_w(GICC_PMR) >> INTPRI_SHIFT;	/* bit7-3 is valid */
	return (INT)lv;
}

/*
 * Enable interrupt 
 *	Enables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	UB	pri;
	UW	icdipr;
	UW	shift;
	_UW	*addr;

	if ((intno < IRQVEC_TOP || intno >= IRQVEC_END) 
		|| (level < INTPRI_HIGHEST) || (level > INTPRI_LOWEST)) {
		return;
	}

	/* Set priority (GICD.IPRIORITYR reg) */
	pri = (UB)level << INTPRI_SHIFT;	/* bit7-3 is valid */

	addr = GICD_IPRIORITYR( intno>>2);
	shift = (intno & 0x03) << 3;	/* (intno % 4) * 8 */

	icdipr = in_w(addr);
	icdipr &= ~(0x000000ff << shift);
	icdipr |= (UW)pri << shift;
	out_w(addr , icdipr);

	/* Enable Interrupt */
	out_w(GICD_ISENABLER(intno>>5), 1<<(intno&0x1F));
}

/*
 * Disable interrupt 
 *	Disables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void DisableInt( UINT intno )
{
	_UW	*addr;

	if (intno < IRQVEC_TOP || intno >= IRQVEC_END) return;

	out_w(GICD_ICENABLER(intno>>5), 1<<(intno&0x1F));
}

/*
 * Set interrupt mode
 *	Set the interrupt mode specified by `intvec' to the mode given
 *  	by `mode'. If an illegal mode is given, subsequent correct behavior
 *      of the system is not guaranteed.
 */
EXPORT void SetIntMode(  UINT intno, UINT mode )
{
	UW	mask;
	_UW	*addr;

	if (intno < EIT_TINT0 || intno >= IRQVEC_END || mode > IM_EDGE) return;

	addr = GICD_ICFGR(intno>>4);
	mask = 1 << (((intno&0x0F)<<1) + 1);

	if (IM_LEVEL == mode) {
		*addr &= ~mask;		/* Level detection */
	} else {
		*addr |= mask;		/* Edge detection */
	}
}

/*
 * Set-Pending
 *  Pends the associated interrupt under software control.
 *	External Interrupt can be specified. 
 */
EXPORT void SetPendingInt( UINT intno )
{
	if (intno < IRQVEC_TOP || intno >= IRQVEC_END) return;

	if (intno >= EIT_SW0 && intno <= EIT_SW15) {	/* GIC software interrupt */
		out_w(GICD_SGIR, (0b10 << 24) | (UW)intno);
	} else {
		out_w(GICD_ISPENDR(intno>>5), 1<<(intno&0x1F));
	}
}

/*
 * Clear-Pending
 *	Un-pends the associated interrupt under software control.
 *	External Interrupt can be specified. 
 */
EXPORT void ClearPendingInt( UINT intno )
{
	if (intno < IRQVEC_TOP || intno >= IRQVEC_END) return;

	out_w(GICD_ICPENDR(intno>>5), 1<<(intno&0x1F));
}

/*
 * Check active state
 *	Current active state for the associated interrupt
 *	External Interrupt can be specified. 
 */
EXPORT BOOL CheckInt( UINT intno )
{
	UW	data;

	if (intno < IRQVEC_TOP || intno >= IRQVEC_END) return FALSE;

	data = in_w(GICD_ICPENDR(intno>>5));
	return (data & (1 << (intno&0x1F))) ? TRUE : FALSE;
}

EXPORT void EndOfInt( UINT intno )
{
	if (intno < IRQVEC_TOP || intno >= IRQVEC_END) 	return;

	out_w(GICC_EOIR, intno);
}


#endif /* CPU_CORE_ARMV7A */
