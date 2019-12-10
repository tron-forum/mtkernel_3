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
 *	inittask.h
 *	Initial task definition
 */

#ifndef _INITTASK_DEF_
#define _INITTASK_DEF_

/*
 * Initial task parameter
 */
#define INITTASK_EXINF		(0x0)
#define INITTASK_ITSKPRI	(1)
#define INITTASK_STKSZ		(1*1024)
#define INITTASK_DSNAME		"inittsk"

#if USE_IMALLOC

#define INITTASK_TSKATR		(TA_HLNG | TA_RNG0)
#define INITTASK_STACK		(NULL)

#else

#define INITTASK_TSKATR		(TA_HLNG | TA_RNG0 | TA_USERBUF)
#define INITTASK_STACK		init_task_stack

#endif

#endif /* _INITTASK_DEF_ */
