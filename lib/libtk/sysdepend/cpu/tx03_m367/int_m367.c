/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_TMPM367FDFG

/*
 *	int.c
 *
 *	Interrupt controller (TX03M67 )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#include "../core/armv7m/int_armv7m.h"

/*----------------------------------------------------------------------*/
/*
 * Interrupt control API
 * 
 */

/*
 * Enable interrupt 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	EnableInt_nvic( intno, level);
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	DisableInt_nvic( intno);
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	UW	val;

	ClearInt_nvic(intno);		/* Un-pends the associated interrupt */

	/* Clear Clock Generetor Interrupt request */
	switch (intno) 	{
	case M367_INTUSBWKUP:
		val = 12;
		break;
	case M367_INTD:
		val = 13;
		break;
	case M367_INTRTC:
		val = 14;
		break;
	case M367_INTRMCRX:
		val = 15;
		break;
	default:
		if(intno >= M367_INT0 && intno <= M367_INTC) {
			val = intno;
		} else {
			return;
		}
		break;
	}
	*(_UW*)CLKCTRL_CGICRCG = val;
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
 */
EXPORT BOOL CheckInt( UINT intno )
{
	return CheckInt_nvic( intno);
}

/*
 * Set interrupt mode
 */
EXPORT void SetIntMode(UINT intno, UINT mode)
{
	_UW	*cgimcg;		/* CG Interrupt mode control register */
	UINT	n, e, k, s;

	switch (intno) 	{
	case M367_INTUSBWKUP:
		n = 12;
		break;
	case M367_INTD:
		n = 13;
		break;
	case M367_INTRTC:
		n = 14;
		break;
	case M367_INTRMCRX:
		n = 15;
		break;
	default:
		if(intno >= M367_INT0 && intno <= M367_INTC) {
			n = intno;
		} else {
			return;
		}
		break;
	}

	if(mode & IM_LEVEL) {		
		e = 0;	/* Level sence */
	} else {
		e = 2;	/* Edge sense */
	}
	if(~(mode & IM_LOW)) {
		e++;
	}

	cgimcg = (_UW*)(CLKCTRL_CGIMCG) + (n >> 2);
	k = (intno & 3) <<3;

	DI(s);
	*cgimcg = (*cgimcg & ~(7 << (k + 4))) | (e << (k + 4));
	*cgimcg |= (1 << k);
	EI(s);
}

#endif /* CPU_TMPM367FDFG */