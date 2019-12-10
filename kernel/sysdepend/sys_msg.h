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
 *	sys_msg.h
 *	Hardware-Dependent System Message
 */

#ifndef _SYSDEPEND_SYSMSG_
#define _SYSDEPEND_SYSMSG_

/* System dependencies */
#define SYSMSG_PATH_(a)	#a
#define SYSMSG_PATH(a)	SYSMSG_PATH_(a)
#define SYSMSG_SYSDEP()	SYSMSG_PATH(TARGET_DIR/sys_msg.h)
#include SYSMSG_SYSDEP()

#endif /* _SYSDEPEND_SYSMSG_ */