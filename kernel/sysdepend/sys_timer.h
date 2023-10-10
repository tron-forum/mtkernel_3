/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sys_timer.h
 *	Hardware-Dependent System Timer Processing
 */

#ifndef _SYSDEPEND_SYSTIMER_
#define _SYSDEPEND_SYSTIMER_

#include <sys/sysdef.h>

/* System dependencies */
#define SYSTIMER_PATH_(a)	#a
#define SYSTIMER_PATH(a)	SYSTIMER_PATH_(a)
#define SYSTIMER_SYSDEP()	SYSTIMER_PATH(KNL_SYSDEP_PATH/sys_timer.h)
#include SYSTIMER_SYSDEP()

#endif /* _SYSDEPEND_SYSTIMER_ */