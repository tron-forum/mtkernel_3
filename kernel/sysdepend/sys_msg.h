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
 *	sys_msg.h
 *	Hardware-Dependent System Message
 */

#ifndef _SYSDEPEND_SYSMSG_
#define _SYSDEPEND_SYSMSG_

#include <tm/tmonitor.h>

#if (USE_SYSTEM_MESSAGE && USE_TMONITOR)
#define SYSTEM_MESSAGE(s)	tm_putstring((UB*)s)
#else
#define SYSTEM_MESSAGE(s)
#endif /* USE_SYSTEM_MESSAGE && USE_TMONITOR */


#endif /* _SYSDEPEND_SYSMSG_ */