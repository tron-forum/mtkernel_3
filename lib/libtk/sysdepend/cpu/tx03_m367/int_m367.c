/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_TMPM369FDFG

/*
 *	int.c
 *
 *	Interrupt controller (TX03M67 )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>
/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	UW	val;

	ClearPendingInt(intno);		/* Un-pends the associated interrupt */

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

	cgimcg = (_UW*)(CLKCTRL_CGIMCG + (n >> 2));
	k = (intno & 3) <<3;

	DI(s);
	*cgimcg = (*cgimcg & ~(7 << (k + 4))) | (e << (k + 4));
	*cgimcg |= (1 << k);
	EI(s);
}

#endif /* CPU_TMPM369FDFG */