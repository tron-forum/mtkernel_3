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
 *	profile.h
 *
 *	Service Profile
 */

#ifndef __SYS_PROFILE_H__
#define __SYS_PROFILE_H__

#include <sys/machine.h>
#include <sys/knldef.h>

/*
 **** System-depeneded profile
 */

/* System dependencies */
#define PROF_PATH_(a)		#a
#define PROF_PATH(a)		PROF_PATH_(a)
#define PROF_SYSDEP()		PROF_PATH(sysdepend/TARGET_DIR/profile.h)
#include PROF_SYSDEP()


/* 
 **** Common profile
 */

/*
 * OS type and version
 */
#define TK_SPECVER_MAGIC		6					/* micro T-Kernel magic number */
#define TK_SPECVER_MAJOR		3					/* major version number */
#define TK_SPECVER_MINOR		0					/* minor version number */
#define TK_SPECVER			((TK_SPECVER_MAJOR << 8) | TK_SPECVER_MINOR)

/*
 * Maximum task priority (>= 16)
 */
#define TK_MAX_TSKPRI			(MAX_TSKPRI)		/* Maximum task priority */
#define TK_WAKEUP_MAXCNT		(+2147483647L)		/* Maximum queuing number of the task wakeup requests. */
#define TK_SEMAPHORE_MAXCNT		(+2147483647L)		/* Maximum semaphore count */
/*
 * Maximum queuing count of the forced wait of tasks.
 */
#define TK_SUSPEND_MAXCNT		(+2147483647L)

/*
 * Device Driver
 */
#define TK_SUPPORT_TASKEVENT		FALSE				/* Support of task event */
#define TK_SUPPORT_DISWAI		FALSE				/* Support of disabling wait (API: tk_dis_wai) */

/*
 * Memory management
 */
#define TK_SUPPORT_USERBUF		TRUE				/* Support of user-specified buffer (TA_USERBUF) */
#define TK_SUPPORT_AUTOBUF		TRUE				/* Support of automatic buffer allocation (No TA_USERBUF specification) */
#define TK_SUPPORT_MEMLIB		(USE_IMALLOC)			/* Support of memory allocation library */

/*
 * Task exception
 */
#define TK_SUPPORT_TASKEXCEPTION	FALSE				/* Support of task exception */

/*
 * Subsystem
 */
#define TK_SUPPORT_SUBSYSTEM		FALSE				/* Support of subsystem */
#define TK_SUPPORT_SSYEVENT		FALSE				/* Support of subsystem event processing */

/*
 * System configuration information
 */
#define TK_SUPPORT_SYSCONF		FALSE				/* Support of getting system configuration information */

/*
 * Data types and size
 */
#define TK_HAS_DOUBLEWORD		TRUE				/* Support of 64-bit data types (D, UD, VD) */
#define TK_SUPPORT_USEC			FALSE				/* Support of microsecond */
#define TK_SUPPORT_LARGEDEV		FALSE				/* Support of Large Mass-storage Device (64-bit) */
#define TK_SUPPORT_SERCD		FALSE				/* Support of sub error code */

/*
 * other function
 */
#define TK_TRAP_SVC			FALSE				/* Use CPU Trap instruction for system call entry */
#define TK_HAS_SYSSTACK			FALSE				/* Task has a separate system stack. */
#define	TK_SUPPORT_UTC			TRUE				/* Support of UTC(Unix style) */
#define TK_SUPPORT_TRONTIME		TRUE				/* Support of TRON time */

/*
 * Debug support
 */
#define TK_SUPPORT_DSNAME		(USE_OBJECT_NAME)	/* Support of DS object names */
#define TK_SUPPORT_DBGSPT		(USE_DBGSPT)		/* Support of T-Kernel/DS */


#endif /* __SYS_PROFILE_H__ */
