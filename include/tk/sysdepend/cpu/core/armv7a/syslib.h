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
 *	micro T-Kernel System Library  (ARMv7-A core depended)
 */

#ifndef __TK_SYSLIB_DEPEND_CORE_H__
#define __TK_SYSLIB_DEPEND_CORE_H__

#include <tk/errno.h>
#include <sys/sysdef.h>

/*----------------------------------------------------------------------*/
/*
 * CPU interrupt control for ARMv7-A.
 *	'intsts' is the value of CPSR.I.
 *	disint()  Disable interrupt (IRQ).
 *	enaint()  Enable interrupt (IRQ).
 */

IMPORT UW disint( void );
IMPORT void enaint( UW intsts );

#define DI(intsts)	( (intsts) = (UINT)disint() )
#define EI(intsts)	( enaint((UW)intsts) )
#define isDI(intsts)	( (intsts) != 0 )

#endif /* __TK_SYSLIB_DEPEND_CORE_H__ */
