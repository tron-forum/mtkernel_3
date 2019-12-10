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
 *	sys_timer.h
 *	Hardware-Dependent System Timer Processing
 */

#ifndef _SYSDEPEND_SYSTIMER_
#define _SYSDEPEND_SYSTIMER_

/* System dependencies */
#define SYSTIMER_PATH_(a)	#a
#define SYSTIMER_PATH(a)	SYSTIMER_PATH_(a)
#define SYSTIMER_SYSDEP()	SYSTIMER_PATH(TARGET_DIR/sys_timer.h)
#include SYSTIMER_SYSDEP()

#endif /* _SYSDEPEND_SYSTIMER_ */