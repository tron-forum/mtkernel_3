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
 *	cpu_status.h
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPUSTATUS_
#define _SYSDEPEND_CPUSTATUS_

/* System dependencies */
#define CPUSTATUS_PATH_(a)	#a
#define CPUSTATUS_PATH(a)	CPUSTATUS_PATH_(a)
#define CPUSTATUS_SYSDEP()	CPUSTATUS_PATH(TARGET_DIR/cpu_status.h)
#include CPUSTATUS_SYSDEP()

#endif /* _SYSDEPEND_CPUSTATUS_ */