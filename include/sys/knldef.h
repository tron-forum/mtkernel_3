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
 *	knldef.h
 *	micro T-Kernel system definition form Configuration
 */

#ifndef _SYS_KNLDEF_H_
#define _SYS_KNLDEF_H_

#include <sys/sysdef.h>

/*---------------------------------------------------------------------- */
/* Check configuration data
 */
#if ( CNF_TIMER_PERIOD < MIN_TIMER_PERIOD || CNF_TIMER_PERIOD > MAX_TIMER_PERIOD )
# error "CNF_TIMER_PERIOD is out of range."
#endif

#if CNF_MAX_TSKPRI < 16
# error "The maximum task priority must be 16 or higher."
#endif

#if USE_PTMR && !CPU_HAS_PTMR
# error "USE_PTMR cannot be specified."
#endif

#if USE_FPU && !CPU_HAS_FPU
# error "USE_FPU cannot be specified."
#endif

#if USE_DSP && !CPU_HAS_DSP
# error "USE_DSP cannot be specified."
#endif

/*---------------------------------------------------------------------- */
/* System memory area definition
 */
#define	SYSTEMAREA_TOP	CNF_SYSTEMAREA_TOP
#define SYSTEMAREA_END	CNF_SYSTEMAREA_END


/*---------------------------------------------------------------------- */
/* Task priority configuration
 */
#define MIN_TSKPRI	(1)			/* Minimum priority number = highest priority */
#define MAX_TSKPRI	(CNF_MAX_TSKPRI)	/* Maximum priority number = lowest priority */
#define NUM_TSKPRI	(CNF_MAX_TSKPRI)	/* Number of priority levels */
#define CHK_PRI(pri)	((MIN_TSKPRI) <= (pri) && (pri) <= (MAX_TSKPRI))


/*---------------------------------------------------------------------- */
/* System Timer period
 */
#define	TIMER_PERIOD	CNF_TIMER_PERIOD


/*---------------------------------------------------------------------- */
/* Kernel object configuration
 */

/* Task configuration */
#define MIN_TSKID	(1)
#define MAX_TSKID	(CNF_MAX_TSKID)
#define NUM_TSKID	(MAX_TSKID)
#define CHK_TSKID(id)	((MIN_TSKID) <= (id) && (id) <= (MAX_TSKID))
#define INDEX_TSK(id)	((id)-(MIN_TSKID))
#define ID_TSK(index)	((index)+(MIN_TSKID))

/* Semaphore configuration */
#if USE_SEMAPHORE
#define MAX_SEMID	(CNF_MAX_SEMID)
#define MIN_SEMID	(1)
#define NUM_SEMID	(MAX_SEMID)
#define CHK_SEMID(id)	((MIN_SEMID) <= (id) && (id) <= (MAX_SEMID))
#define INDEX_SEM(id)	((id)-(MIN_SEMID))
#define ID_SEM(index)	((index)+(MIN_SEMID))
#else
#define MAX_SEMID	(0)
#endif

/* Event flag configuration */
#if USE_EVENTFLAG
#define MIN_FLGID	(1)
#define MAX_FLGID	(CNF_MAX_FLGID)
#define NUM_FLGID	(MAX_FLGID)
#define CHK_FLGID(id)	((MIN_FLGID) <= (id) && (id) <= (MAX_FLGID))
#define INDEX_FLG(id)	((id)-(MIN_FLGID))
#define ID_FLG(index)	((index)+(MIN_FLGID))
#else
#define MAX_FLGID	(0)
#endif

/* Mailbox configuration */
#if USE_MAILBOX
#define MIN_MBXID	(1)
#define MAX_MBXID	(CNF_MAX_MBXID)
#define NUM_MBXID	(MAX_MBXID)
#define CHK_MBXID(id)	((MIN_MBXID) <= (id) && (id) <= (MAX_MBXID))
#define INDEX_MBX(id)	((id)-(MIN_MBXID))
#define ID_MBX(index)	((index)+(MIN_MBXID))
#else
#define MAX_MBXID	(0)
#endif

/* Mutex configuration */
#if USE_MUTEX
#define MIN_MTXID	(1)
#define MAX_MTXID	(CNF_MAX_MTXID)
#define NUM_MTXID	(MAX_MTXID)
#define CHK_MTXID(id)	((MIN_MTXID) <= (id) && (id) <= (MAX_MTXID))
#define INDEX_MTX(id)	((id)-(MIN_MTXID))
#define ID_MTX(index)	((index)+(MIN_MTXID))
#else
#define NUM_MTXID	(0)
#endif

/* Message buffer configuration */
#if USE_MESSAGEBUFFER
#define MIN_MBFID	(1)
#define MAX_MBFID	(CNF_MAX_MBFID)
#define NUM_MBFID	(MAX_MBFID)
#define CHK_MBFID(id)	((MIN_MBFID) <= (id) && (id) <= (MAX_MBFID))
#define INDEX_MBF(id)	((id)-(MIN_MBFID))
#define ID_MBF(index)	((index)+(MIN_MBFID))
#else
#define MAX_MBFID	(0)
#endif

/* Memory pool configuration */
#if USE_MEMORYPOOL
#define MIN_MPLID	(1)
#define MAX_MPLID	(CNF_MAX_MPLID)
#define NUM_MPLID	(MAX_MPLID)
#define CHK_MPLID(id)	((MIN_MPLID) <= (id) && (id) <= (MAX_MPLID))
#define INDEX_MPL(id)	((id)-(MIN_MPLID))
#define ID_MPL(index)	((index)+(MIN_MPLID))
#else
#define MAX_MPLID	(0)
#endif

/* Fixed size memory pool configuration */
#if USE_FIX_MEMORYPOOL
#define MIN_MPFID	(1)
#define MAX_MPFID	(CNF_MAX_MPFID)
#define NUM_MPFID	(MAX_MPFID)
#define CHK_MPFID(id)	((MIN_MPFID) <= (id) && (id) <= (MAX_MPFID))
#define INDEX_MPF(id)	((id)-(MIN_MPFID))
#define ID_MPF(index)	((index)+(MIN_MPFID))
#else
#define MAX_MPFID	(0)
#endif

/* Cyclic handler configuration */
#if USE_CYCLICHANDLER
#define MIN_CYCID	(1)
#define MAX_CYCID	(CNF_MAX_CYCID)
#define NUM_CYCID	(MAX_CYCID)
#define CHK_CYCID(id)	((MIN_CYCID) <= (id) && (id) <= (MAX_CYCID))
#define INDEX_CYC(id)	((id)-(MIN_CYCID))
#define ID_CYC(index)	((index)+(MIN_CYCID))
#else
#define MAX_CYCID	(0)
#endif


/* Alarm handler configuration */
#if USE_ALARMHANDLER
#define MIN_ALMID	(1)
#define MAX_ALMID	(CNF_MAX_ALMID)
#define NUM_ALMID	(MAX_ALMID)
#define CHK_ALMID(id)	((MIN_ALMID) <= (id) && (id) <= (MAX_ALMID))
#define INDEX_ALM(id)	((id)-(MIN_ALMID))
#define ID_ALM(index)	((index)+(MIN_ALMID))
#else
#define MAX_ALMID	(0)
#endif

/* Rendezvous configuration */
#if USE_LEGACY_API && USE_RENDEZVOUS
#define MIN_PORID	(1)
#define MAX_PORID	(CNF_MAX_PORID)
#define NUM_PORID	(MAX_PORID)
#define CHK_PORID(id)	((MIN_PORID) <= (id) && (id) <= (MAX_PORID))
#define INDEX_POR(id)	((id)-(MIN_PORID))
#define ID_POR(index)	((index)+(MIN_PORID))
#else
#define MAX_PORID	(0)
#endif /* USE_LEGACY_API && USE_RENDEZVOUS */

/*---------------------------------------------------------------------- */
/* Device configuration
 */
#if USE_DEVICE
#define MAX_REGDEV	(CNF_MAX_REGDEV)
#define MAX_OPNDEV	(CNF_MAX_OPNDEV)
#define MAX_REQDEV	(CNF_MAX_REQDEV)
#define DEVT_MBFSZ0	(CNF_DEVT_MBFSZ0)
#define DEVT_MBFSZ1	(CNF_DEVT_MBFSZ1)
#else
#define MAX_REGDEV	(0)
#endif /* USE_DEVICE */


/*---------------------------------------------------------------------- */
/* Stack size definition
 */
#define EXC_STACK_SIZE	CNF_EXC_STACK_SIZE
#define	TMP_STACK_SIZE	CNF_TMP_STACK_SIZE

/*---------------------------------------------------------------------- */
/* Version Number
 */
#define VER_MAKER	CNF_VER_MAKER
#define VER_PRID	CNF_VER_PRID
#define VER_MAJOR	3
#define VER_MINOR	0
#define VER_SPVER	(0x6000 | (VER_MAJOR << 8) | VER_MINOR)
#define VER_PRVER	CNF_VER_PRVER
#define VER_PRNO1	CNF_VER_PRNO1
#define VER_PRNO2	CNF_VER_PRNO2
#define VER_PRNO3	CNF_VER_PRNO3
#define VER_PRNO4	CNF_VER_PRNO4


#endif /* _SYS_KNLDEF_H_ */
