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
 *	cpu_status.h
 *	CPU-Dependent Task Start Processing
 */

#ifndef _SYSDEPEND_CPUSTATUS_
#define _SYSDEPEND_CPUSTATUS_

/* System dependencies */
#define CPUSTATUS_PATH_(a)	#a
#define CPUSTATUS_PATH(a)	CPUSTATUS_PATH_(a)
#define CPUSTATUS_SYSDEP()	CPUSTATUS_PATH(KNL_SYSDEP_PATH/cpu_status.h)
#include CPUSTATUS_SYSDEP()

#endif /* _SYSDEPEND_CPUSTATUS_ */