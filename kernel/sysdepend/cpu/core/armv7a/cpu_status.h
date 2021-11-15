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
 *	cpu_status.h (ARMv7-A)
 *	CPU-Dependent Status Definition
 */

#ifndef _SYSDEPEND_CPU_CORE_STATUS_
#define _SYSDEPEND_CPU_CORE_STATUS_

#include <tk/syslib.h>
#include <sys/sysdef.h>

#include "sysdepend.h"
/*
 * Start/End critical section
 */
#define BEGIN_CRITICAL_SECTION	{ UINT _intsts_ = disint();
#define END_CRITICAL_SECTION	if ( !isDI(_intsts_)			\
				  && knl_ctxtsk != knl_schedtsk		\
				  && !knl_isTaskIndependent()		\
				  && !knl_dispatch_disabled ) {		\
					knl_dispatch();			\
				}					\
				enaint(_intsts_); }

/*
 * Start/End interrupt disable section
 */
#define BEGIN_DISABLE_INTERRUPT	{ UINT _intsts_ = disint();
#define END_DISABLE_INTERRUPT	enaint(_intsts_); }

/*
 * Interrupt enable/disable
 */
#define ENABLE_INTERRUPT	{ enaint(0); }
#define DISABLE_INTERRUPT	{ disint(); }

/*
 * Enable interrupt nesting
 *	Enable the interrupt that has a higher priority than 'level.'
 */
#define ENABLE_INTERRUPT_UPTO(level)	{ enaint(0); }

/*
 *  Task-independent control
 */
IMPORT	W	knl_taskindp;		/* Task independent status */

/*
 * If it is the task-independent part, TRUE
 */
Inline BOOL knl_isTaskIndependent( void )
{
	return ( knl_taskindp > 0 )? TRUE: FALSE;
}
/*
 * Move to/Restore task independent part
 */
Inline void knl_EnterTaskIndependent( void )
{
	knl_taskindp++;
}
Inline void knl_LeaveTaskIndependent( void )
{
	knl_taskindp--;
}

/*
 * Move to/Restore task independent part
 */
#define ENTER_TASK_INDEPENDENT	{ knl_EnterTaskIndependent(); }
#define LEAVE_TASK_INDEPENDENT	{ knl_LeaveTaskIndependent(); }

/* ----------------------------------------------------------------------- */
/*
 *	Check system state
 */

/*
 * When a system call is called from the task independent part, TRUE
 */
#define in_indp()	( knl_isTaskIndependent() || knl_ctxtsk == NULL )


Inline UINT knl_getCPSR(void)
{
	UINT rtn;
	asm("mrs     %0, cpsr":"=r"(rtn));	/* save CPSR to ret */
	return rtn;
}

/*
 * When a system call is called during dispatch disable, TRUE
 * Also include the task independent part as during dispatch disable.
 */
#define in_ddsp()	( knl_dispatch_disabled		\
			|| in_indp() 			\
			|| (knl_getCPSR() & PSR_I) )

/*
 * When a system call is called during CPU lock (interrupt disable), TRUE
 * Also include the task independent part as during CPU lock.
 */
#define in_loc()	( (knl_getCPSR() & PSR_I)	\
			|| in_indp() )

/*
 * When a system call is called during executing the quasi task part, TRUE
 * Valid only when in_indp() == FALSE because it is not discriminated from 
 * the task independent part. 
 */
#define in_qtsk()	( knl_ctxtsk->sysmode > knl_ctxtsk->isysmode )

/* ----------------------------------------------------------------------- */
/*
 *	Task dispatcher startup
 */
Inline void knl_force_dispatch( void )
{
	Asm("svc %0"::"i"(SVC_FORCE_DISPATCH):"lr");
}
Inline void knl_dispatch( void )
{
	Asm("svc %0"::"i"(SVC_DISPATCH):"lr");
}

#endif /* _SYSDEPEND_CPU_CORE_STATUS_ */
