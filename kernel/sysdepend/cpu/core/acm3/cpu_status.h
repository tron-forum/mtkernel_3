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
 *	cpu_status.h (ARM Cortex-M3)
 *	Cortex-M3 Dependent Definition
 */

#ifndef _SYSDEPEND_CPU_CORE_STATUS_
#define _SYSDEPEND_CPU_CORE_STATUS_

#include <tk/syslib.h>
#include <sys/sysdef.h>

/*
 * Start/End critical section
 */
#define BEGIN_CRITICAL_SECTION	{ UINT _basepri_ = disint();
#define END_CRITICAL_SECTION	if ( !isDI(_basepri_)			\
				  && knl_ctxtsk != knl_schedtsk		\
				  && !knl_dispatch_disabled ) {		\
					knl_dispatch();			\
				}					\
				set_basepri(_basepri_); }

/*
 * Start/End interrupt disable section
 */
#define BEGIN_DISABLE_INTERRUPT	{ UINT _basepri_ = disint();
#define END_DISABLE_INTERRUPT	set_basepri(_basepri_); }

/*
 * Interrupt enable/disable
 */
#define ENABLE_INTERRUPT	{ set_basepri(0); }
#define DISABLE_INTERRUPT	{ disint(); }

/*
 * Enable interrupt nesting
 *	Enable the interrupt that has a higher priority than 'level.'
 */
#define ENABLE_INTERRUPT_UPTO(level)	{ set_basepri(0); }

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

/*
 * When a system call is called during dispatch disable, TRUE
 * Also include the task independent part as during dispatch disable.
 */
#define in_ddsp()	( knl_dispatch_disabled	\
			|| in_indp()		\
			|| isDI(get_basepri()) )

/*
 * When a system call is called during CPU lock (interrupt disable), TRUE
 * Also include the task independent part as during CPU lock.
 */
#define in_loc()	( isDI(get_basepri())		\
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
IMPORT void knl_force_dispatch( void );
IMPORT void knl_dispatch( void );


/* ------------------------------------------------------------------------ */
/*
 *	Control register operation
 */

Inline UW knl_get_ipsr(void)
{
	UW	ipsr;
	Asm("mrs %0, ipsr": "=r"(ipsr));
	return ipsr;
}

Inline UW knl_get_xpsr(void)
{
	UW	xpsr;
	Asm("mrs %0, psr": "=r"(xpsr));
	return xpsr;
}

Inline UW knl_get_primask(void)
{
	UW	primask;
	Asm("mrs %0, primask": "=r"(primask));
	return primask;
}

/* ----------------------------------------------------------------------- */
/*
 * Task context block
 */
typedef struct {
	void	*ssp;		/* System stack pointer */
} CTXB;


#endif /* _SYSDEPEND_CPU_CORE_STATUS_ */
