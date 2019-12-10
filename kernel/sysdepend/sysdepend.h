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
 *	Sysdepend.h
 *	System-Dependent local defined
 */

#ifndef _SYSDEPEND_SYSDEPEND_
#define _SYSDEPEND_SYSDEPEND_

/* System dependencies */
#define SYSDEPEND_PATH_(a)	#a
#define SYSDEPEND_PATH(a)	SYSDEPEND_PATH_(a)
#define SYSDEPEND_SYSDEP()	SYSDEPEND_PATH(TARGET_DIR/sysdepend.h)
#include SYSDEPEND_SYSDEP()

#endif /* _SYSDEPEND_SYSDEPEND_ */