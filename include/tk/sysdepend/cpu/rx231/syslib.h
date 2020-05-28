/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (RX231 depended)
 */

#ifndef __TK_SYSLIB_CPU_DEPEND_H__
#define __TK_SYSLIB_CPU_DEPEND_H__

#include "../core/rxv2/syslib.h"

/*----------------------------------------------------------------------*/
/*
 * Interrupt controller Control
 *
 */
#define IM_LEVEL	0x0004		/* Level trigger */
#define IM_EDGE		0x0000		/* Edge trigger */

#define IM_HI		0x0000		/* H level/Interrupt at rising edge */
#define IM_LOW		0x0001		/* L level/Interrupt at falling edge */
#define IM_BOTH		0x0003		/* L level/Interrupt at both edge */

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
