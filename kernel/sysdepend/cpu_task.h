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
 *	cpu_task.h
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPUTASK_
#define _SYSDEPEND_CPUTASK_

/* System dependencies */
#define CPUTASK_PATH_(a)	#a
#define CPUTASK_PATH(a)		CPUTASK_PATH_(a)
#define CPUTASK_SYSDEP()	CPUTASK_PATH(KNL_SYSDEP_PATH/cpu_task.h)
#include CPUTASK_SYSDEP()

#endif /* _SYSDEPEND_CPUTASK_ */