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
 *	micro T-Kernel System Library  (ARM Cortex-M3 core depended)
 */

#ifndef __TK_SYSLIB_DEPEND_CORE_H__
#define __TK_SYSLIB_DEPEND_CORE_H__

#include <tk/errno.h>
#include <sys/sysdef.h>

/*----------------------------------------------------------------------*/
/*
 * CPU interrupt control for ARM Cortex-M3.
 *	As armv7-m architecture does not support disable interrupt in
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
 *	PRIGROUP in the AIRCR register determins the split of group
 *	priority from subpriority. PRIGROUP is initialized to 3
 *	(pri:subpri = 4:4)) in the boot sequence.
 */
#define INTPRI_GROUP(pri, subpri)	(((pri) << (8-INTPRI_BITWIDTH)) | (subpri))


/* ------------------------------------------------------------------------ */
/*
 * Interrupt Controller functions for ARM Cortex-M3.
 */

/*
 * Set-Pending
 *  Pends the associated interrupt under software control.
 */
IMPORT void SetPendingInt( UINT intno );

/*
 * Clear-Pending
 *	Un-pends the associated interrupt under software control.
 */
IMPORT void ClearPendingInt( UINT intno );


/*
 * Convert to interrupt definition number
 *
 * For backward compatibility.
 * 	INTVEC has been obsoleted since micro T-Kernel 2.0.
 */
#define DINTNO(intvec)	(intvec)

#endif /* __TK_SYSLIB_DEPEND_CORE_H__ */
