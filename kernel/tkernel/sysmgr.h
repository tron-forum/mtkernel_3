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
 *	sysmgr.h
 *	micro T-Kernel/SM(System Maneger) Definition
 */

#ifndef _SYSMGR_
#define _SYSMGR_

#include <tk/tkernel.h>
#include <sys/queue.h>

#include "kernel.h"


/* ------------------------------------------------------------------------ */
/*
 *	Device management function
 */

/*
 * Lock for device management exclusive control
 */
IMPORT FastMLock	knl_DevMgrLock;
#define LockDM()	MLock(&knl_DevMgrLock, 0)
#define UnlockDM()	MUnlock(&knl_DevMgrLock, 0)

/*
 * Lock for device registration exclusive control
 */
#define LockREG()	MLock(&knl_DevMgrLock, 1)
#define UnlockREG()	MUnlock(&knl_DevMgrLock, 1)

/*
 * Device registration information
 */
typedef struct DeviceControlBlock {
	QUEUE	q;
	UB	devnm[L_DEVNM+1];	/* Device name */
	T_DDEV	ddev;			/* Registration information */
	QUEUE	openq;			/* Open device management queue */
} DevCB;

IMPORT	DevCB		knl_DevCBtbl[];	/* Device registration information
					   table */
IMPORT	QUEUE		knl_UsedDevCB;	/* In-use queue */

#define DID(devcb)		( ((devcb) - knl_DevCBtbl + 1) << 8 )
#define DEVID(devcb, unitno)	( DID(devcb) + (unitno) )
#define DEVCB(devid)		( knl_DevCBtbl + (((devid) >> 8) - 1) )
#define UNITNO(devid)		( (devid) & 0xff )

/*
 * Open management information
 */
typedef struct OpenControlBlock {
	QUEUE		q;
	QUEUE		resq;		/* For connection from resource
					   management */
	ID		resid;		/* Section resource ID */
	DevCB		*devcb;		/* Target device */
	INT		unitno;		/* Subunit number
					   (0: Physical device) */
	UINT		omode;		/* Open mode */
	QUEUE		requestq;	/* Request management queue */
	UH		waitone;	/* Number of individual request
					   waits */
	T_DEVREQ	*waireqlst;	/* List of multiple request waits */
	INT		nwaireq;	/* Number of multiple request waits */
	ID		abort_tskid;	/* Abort completion wait task */
	INT		abort_cnt;	/* Number of abort completion wait
					   requests */
	ID		abort_semid; /* Semaphore for abort completion wait */
} OpnCB;

#define RESQ_OPNCB(rq)		( (OpnCB*)((B*)(rq) - offsetof(OpnCB, resq)) )

/*
 * Request management information
 */
typedef struct RequestControlBlock {
	QUEUE		q;
	OpnCB		*opncb;		/* Open device */
	ID		tskid;		/* Processing task */
	T_DEVREQ	req;		/* Request packet */
} ReqCB;

/*
 * Resource management information
 */
typedef struct ResourceControlBlock {
	QUEUE		openq;		/* Open device management queue */
	INT		dissus;		/* Suspend disable request count */
} ResCB;

/*
 * Request function types
 */

typedef ER  (*OPNFN)( ID devid, UINT omode, void *exinf );
typedef ER  (*ABTFN)( ID tskid, T_DEVREQ *devreq, INT nreq, void *exinf );
typedef INT (*WAIFN)( T_DEVREQ *devreq, INT nreq, TMO tmout, void *exinf );
typedef INT (*EVTFN)( INT evttyp, void *evtinf, void *exinf );
typedef ER  (*CLSFN)( ID devid, UINT option, void *exinf );
typedef ER  (*EXCFN)( T_DEVREQ *devreq, TMO tmout, void *exinf );

/* ------------------------------------------------------------------------ */

#define IMPORT_DEFINE	1
#if IMPORT_DEFINE
/* device.c */
IMPORT	FastMLock	knl_DevMgrLock;
IMPORT	DevCB		knl_DevCBtbl[];
IMPORT	QUEUE		knl_UsedDevCB;
IMPORT	DevCB*		knl_searchDevCB( CONST UB *devnm );
IMPORT	INT			knl_phydevnm( UB *pdevnm, CONST UB *ldevnm );
IMPORT	ER			knl_initialize_devmgr( void );
IMPORT	ER			knl_finish_devmgr( void );
/* deviceio.c */
IMPORT ER knl_check_devdesc( ID dd, UINT mode, OpnCB **p_opncb );
IMPORT void knl_devmgr_startup( void );
IMPORT void knl_devmgr_cleanup( void );
IMPORT ER knl_initDevIO( void );
IMPORT ER knl_finishDevIO( void );

#endif

#endif /* _SYSMGR_ */
