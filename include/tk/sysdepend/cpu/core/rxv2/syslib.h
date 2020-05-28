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
 *	micro T-Kernel System Library  (RXv2 core depended)
 */

#ifndef __TK_SYSLIB_DEPEND_CORE_H__
#define __TK_SYSLIB_DEPEND_CORE_H__

#include <tk/errno.h>
#include <sys/sysdef.h>

/*----------------------------------------------------------------------*/
/*
 * CPU interrupt control for RXv2
 *	'intsts' is the value of PSW register in CPU
 *	disint()  Set PSW.IPL value to 15. And return the original PSW.IPL value.
 *	setint()  Set PSW.IPL value to ipl.
 */
IMPORT UW disint( void );
IMPORT void setint( UW intsts );

#define INTLEVEL_DI		(15)
#define INTLEVEL_EI		(0)

#define DI(intsts)		( (intsts) = disint() )
#define EI(intsts)		( setint(intsts) )
#define isDI(intsts)		( (intsts) == INTLEVEL_DI )

/*
 * Convert to interrupt definition number
 *
 * For backward compatibility.
 * 	INTVEC has been obsoleted since micro T-Kernel 2.0.
 */
#define DINTNO(intvec)	(intvec)

#endif /* __TK_SYSLIB_DEPEND_CORE_H__ */
