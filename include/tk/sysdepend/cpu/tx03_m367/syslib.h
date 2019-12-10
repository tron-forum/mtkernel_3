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

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (TX03M367 depended)
 */

#ifndef __TK_SYSLIB_CPU_DEPEND_H__
#define __TK_SYSLIB_CPU_DEPEND_H__

#include "../core/acm3/syslib.h"

/* ------------------------------------------------------------------------ */
/*
 * Interrupt number
 */

#define	M367_INT0		0
#define	M367_INT1		1
#define	M367_INT2		2
#define	M367_INT3		3
#define	M367_INT4		4
#define	M367_INT5		5
#define	M367_INT6		6
#define	M367_INT7		7
#define	M367_INT8		8
#define	M367_INT9		9
#define	M367_INTA		10
#define	M367_INTB		11
#define	M367_INTC		12
#define M367_INTD		13
#define	M367_INTUSBWKUP		34
#define M367_INTRMCRX		74
#define	M367_INTRTC		99

/* ------------------------------------------------------------------------ */
/*
 * I/O port access
 *	for memory mapped I/O
 */
Inline void out_w( UW port, UW data )
{
	*(_UW*)port = data;
}
Inline void out_h( UW port, UH data )
{
	*(_UH*)port = data;
}
Inline void out_b( UW port, UB data )
{
	*(_UB*)port = data;
}

Inline UW in_w( UW port )
{
	return *(_UW*)port;
}
Inline UH in_h( UW port )
{
	return *(_UH*)port;
}
Inline UB in_b( UW port )
{
	return *(_UB*)port;
}

#endif /* __TK_SYSLIB_DEPEND_H__ */
