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

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (RZ/A2M depended)
 */

#ifndef __TK_SYSLIB_CPU_DEPEND_H__
#define __TK_SYSLIB_CPU_DEPEND_H__

#include "../core/armv7a/syslib.h"

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Control
 */

/*
 * Interrupt number
 */

/*
 * Interrupt mode ( Use SetIntMode )
 */
#define	IM_LEVEL	0x00	/* high level detection */
#define	IM_EDGE		0x01	/* Rising edge detection */

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

Inline void and_w( UW port, UW data)
{
	*(_UW*)port &= data;
}
Inline void and_h( UW port, UH data)
{
	*(_UH*)port &= data;
}
Inline void and_b( UW port, UB data)
{
	*(_UB*)port &= data;
}

Inline void or_w( UW port, UW data)
{
	*(_UW*)port |= data;
}
Inline void or_h( UW port, UH data)
{
	*(_UH*)port |= data;
}
Inline void or_b( UW port, UB data)
{
	*(_UB*)port |= data;
}

#endif /* __TK_SYSLIB_DEPEND_H__ */
