/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07.
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

#if TRUSTZONE_SCALL
#define	INITTASK_STKSZ_SEC	(1*1024)

#if USE_IMALLOC
#define INITTASK_TSKATR		(TA_HLNG | TA_RNG0 | TA_TZCALL)
#else
#define INITTASK_TSKATR		(TA_HLNG | TA_RNG0 | TA_TZCALL |TA_USERBUF)
#endif

#else	// TRUSTZONE_SCALL

#if USE_IMALLOC
#define INITTASK_TSKATR		(TA_HLNG | TA_RNG0)
#else
#define INITTASK_TSKATR		(TA_HLNG | TA_RNG0 | TA_USERBUF)
#endif

#endif	// TRUSTZONE_SCALL

#if USE_IMALLOC
#define INITTASK_STACK		(NULL)
#else
#define INITTASK_STACK		init_task_stack
#endif

#endif /* _INITTASK_DEF_ */
