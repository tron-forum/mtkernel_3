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
 *	cpu_task.h
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPUTASK_
#define _SYSDEPEND_CPUTASK_

/* System dependencies */
#define CPUTASK_PATH_(a)	#a
#define CPUTASK_PATH(a)		CPUTASK_PATH_(a)
#define CPUTASK_SYSDEP()	CPUTASK_PATH(TARGET_DIR/cpu_task.h)
#include CPUTASK_SYSDEP()

#endif /* _SYSDEPEND_CPUTASK_ */