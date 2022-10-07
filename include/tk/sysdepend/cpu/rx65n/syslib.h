/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 */
/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (RX65N depended)
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

/* 
 * Software Configurable Interrupt (PERIA & PERIB) Control
 */
#if USE_SFTCNF_INT

IMPORT ER SetPERI(UINT intno, UINT fctno);	/* Set Software Configurable Interrupt */

#endif /* USE_SFTCNF_INT */

/* 
 * Group Interrupt Control (Internal API. Do not call from the user program!)
 */
#if USE_GROUP_INT

IMPORT void knl_enable_gint( UINT intno);
IMPORT void knl_disable_gint( UINT intno);
IMPORT void knl_clear_gint( UINT intno);
IMPORT BOOL knl_check_gint( UINT intno);

#endif /* USE_GROUP_INT */

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
