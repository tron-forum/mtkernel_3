/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07.
 *
 *----------------------------------------------------------------------
 */

/*
 *	syslib.h
 *
 *	micro T-Kernel System Library  (ARMv8-M core)
 */

#ifndef __TK_SYSLIB_DEPEND_CORE_H__
#define __TK_SYSLIB_DEPEND_CORE_H__

#include <tk/errno.h>
#include <sys/sysdef.h>

/*----------------------------------------------------------------------*/
/*
 * CPU interrupt control for ARMv8-M.
 *	As armv8-m architecture does not support disable interrupt in
 *	xpsr register, we have to raise the excution priority to 
 *	that the interrupt group have. Write the BASEPRI to implement 
 *	disint.
 */

IMPORT void set_basepri(UW intsts);	/* Set Base Priority register */
IMPORT UW get_basepri(void);		/* Get Base Priority register */
IMPORT UW disint(void);			/* Disable interrupt */


#define DI(intsts)		( (intsts) = disint() )
#define EI(intsts)		( set_basepri(intsts) )
#define isDI(intsts)		( (intsts) != 0 )

#define INTLEVEL_DI		(0)
#define INTLEVEL_EI		(255)

/*
 * Interrupt priority grouping
 *
 *	PRIGROUP in the AIRCR register determines the split of group
 *	priority from subpriority. PRIGROUP is initialized to 3
 *	(pri:subpri = 4:4)) in the boot sequence.
 */
#define INTPRI_GROUP(pri, subpri)	(((pri) << (8-INTPRI_BITWIDTH)) | (subpri))


/* ------------------------------------------------------------------------ */
/*
 * Convert to interrupt definition number
 *
 * For backward compatibility.
 * 	INTVEC has been obsoleted since micro T-Kernel 2.0.
 */
#define DINTNO(intvec)	(intvec)

/* ------------------------------------------------------------------------ */
/*
 * I/O port access
 *	for memory mapped I/O
 */
Inline void out_d( UW port, UD data )
{
	*(_UD*)port = data;
}
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

Inline UD in_d( UW port )
{
	return *(_UD*)port;
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

#endif /* __TK_SYSLIB_DEPEND_CORE_H__ */
