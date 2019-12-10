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
 *	deviceio.c
 *	Device Management Function: Input/Output
 */

#include "kernel.h"
#include "sysmgr.h"
#include "device.h"

#if USE_DEVICE

Noinit(EXPORT OpnCB	knl_OpnCBtbl[MAX_OPNDEV]);	/* Open management information table */
Noinit(EXPORT QUEUE	knl_FreeOpnCB);	/* Unused queue */

Noinit(EXPORT ReqCB	knl_ReqCBtbl[MAX_REQDEV]);	/* Request management information table */
Noinit(EXPORT QUEUE	knl_FreeReqCB);	/* Unused queue */

Noinit(EXPORT ResCB knl_resource_control_block);


/*
 * Get resource management information
 */
EXPORT ResCB* knl_GetResCB( void )
{
	LockDM();

	/* If the startup function is not called, initialize at this point */
	if ( knl_resource_control_block.openq.next == NULL ) {
		/* Initialization of open device management queue */
		QueInit(&(knl_resource_control_block.openq));
	}

	UnlockDM();

	return &knl_resource_control_block;
}

/*
 * Verify validity of device descriptor
 */
EXPORT ER knl_check_devdesc( ID dd, UINT mode, OpnCB **p_opncb )
{
	OpnCB	*opncb;

	if ( dd < 1 || dd > MAX_OPNDEV ) {
		return E_ID;
	}
	opncb = OPNCB(dd);
	if ( opncb->resid == 0 ) {
		return E_ID;
	}

	if ( mode != 0 ) {
		if ( (opncb->omode & mode) == 0 ) {
			return E_OACV;
		}
	}

	*p_opncb = opncb;
	return E_OK;
}

/*
 * Free open management block
 */
EXPORT void knl_delOpnCB( OpnCB *opncb, BOOL free )
{
	QueRemove(&opncb->q);
	QueRemove(&opncb->resq);

	if ( free ) {
		QueInsert(&opncb->q, &knl_FreeOpnCB);
	}
	opncb->resid = 0;
}

/*
 * Free request management block
 */
EXPORT void knl_delReqCB( ReqCB *reqcb )
{
	QueRemove(&reqcb->q);

	QueInsert(&reqcb->q, &knl_FreeReqCB);
	reqcb->opncb = NULL;
}

/* ------------------------------------------------------------------------ */

/*
 * TRUE if specified device is open.
 */
EXPORT BOOL knl_chkopen( DevCB *devcb, INT unitno )
{
	QUEUE	*q;

	for ( q = devcb->openq.next; q != &devcb->openq; q = q->next ) {
		if ( ((OpnCB*)q)->unitno == unitno ) {
			return TRUE;
		}
	}
	return FALSE;
}


LOCAL CONST T_CSEM knl_pk_csem_DM = {
	NULL,
	TA_TFIFO | TA_FIRST,
	0,
	1,
};

/*
 * Get open management block
 */
LOCAL OpnCB* newOpnCB( DevCB *devcb, INT unitno, UINT omode, ResCB *rescb )
{
	OpnCB	*opncb;

	/* Get space in open management block */
	opncb = (OpnCB*)QueRemoveNext(&knl_FreeOpnCB);
	if ( opncb == NULL ) {
		return NULL; /* No space */
	}

	/* Register as open device */
	QueInsert(&opncb->q, &devcb->openq);
	QueInsert(&opncb->resq, &rescb->openq);

	opncb->devcb  = devcb;
	opncb->unitno = unitno;
	opncb->omode  = omode;
	QueInit(&opncb->requestq);
	opncb->waitone = 0;
	opncb->nwaireq = 0;
	opncb->abort_tskid = 0;

	opncb->resid  = 0; /* Indicate that open processing is not completed */

	return opncb;
}

/*
 * Check open mode
 */
LOCAL ER chkopenmode( DevCB *devcb, INT unitno, UINT omode )
{
	QUEUE	*q;
	OpnCB	*opncb;
	INT	read, write, rexcl, wexcl;

	if ( (omode & TD_UPDATE) == 0 ) {
		return E_PAR;
	}

	/* Check current open state */
	read = write = rexcl = wexcl = 0;
	for ( q = devcb->openq.next; q != &devcb->openq; q = q->next ) {
		opncb = (OpnCB*)q;

		if ( unitno == 0 || opncb->unitno == 0 || opncb->unitno == unitno ) {
			if ( (opncb->omode & TD_READ)  != 0 ) {
				read++;
			}
			if ( (opncb->omode & TD_WRITE) != 0 ) {
				write++;
			}
			if ( (opncb->omode & (TD_EXCL|TD_REXCL)) != 0) {
				rexcl++;
			}
			if ( (opncb->omode & (TD_EXCL|TD_WEXCL)) != 0) {
				wexcl++;
			}
		}
	}

	/* Is it able to open? */
	if ( (omode & (TD_EXCL|TD_REXCL)) != 0 && read  > 0 ) {
		return E_BUSY;
	}
	if ( (omode & (TD_EXCL|TD_WEXCL)) != 0 && write > 0 ) {
		return E_BUSY;
	}
	if ( (omode & TD_READ)  != 0 && rexcl > 0 ) {
		return E_BUSY;
	}
	if ( (omode & TD_WRITE) != 0 && wexcl > 0 ) {
		return E_BUSY;
	}

	return E_OK;
}

/*
 * Device open
 */
SYSCALL ID tk_opn_dev( CONST UB *devnm, UINT omode )
{
	OPNFN	openfn;
	void	*exinf;
	UB	pdevnm[L_DEVNM + 1];
	INT	unitno;
	ResCB	*rescb;
	DevCB	*devcb;
	OpnCB	*opncb;
	ER	ercd;
	ID	semid;

	unitno = knl_phydevnm(pdevnm, devnm);

	/* Get resource management information */
	rescb = knl_GetResCB();
	if ( rescb == NULL ) {
		ercd = E_CTX;
		goto err_ret1;
	}

	LockDM();

	/* Search device to open */
	devcb = knl_searchDevCB(pdevnm);
	if ( devcb == NULL || unitno > devcb->ddev.nsub ) {
		ercd = E_NOEXS;
		goto err_ret2;
	}

	/* Check open mode */
	ercd = chkopenmode(devcb, unitno, omode);
	if ( ercd < E_OK ) {
		goto err_ret2;
	}

	openfn = (OPNFN)devcb->ddev.openfn;
	exinf = devcb->ddev.exinf;

	/* Is device driver call required? */
	if ( knl_chkopen(devcb, unitno) && (devcb->ddev.drvatr & TDA_OPENREQ) == 0 ) {
		openfn = NULL;
	}

	/* Get open management block */
	opncb = newOpnCB(devcb, unitno, omode, rescb);
	if ( opncb == NULL ) {
		ercd = E_LIMIT;
		goto err_ret2;
	}

	semid = tk_cre_sem(&knl_pk_csem_DM);
	if ( semid < E_OK ) {
		ercd = E_SYS;
		goto err_ret2_5;
	}
	opncb->abort_semid = semid;

	UnlockDM();

	if ( openfn != NULL ) {
		/* Device driver call */
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode++;
		ENABLE_INTERRUPT;
		ercd = (*openfn)(DEVID(devcb, unitno), omode, exinf);
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode--;
		ENABLE_INTERRUPT;

		if ( ercd < E_OK ) {
			goto err_ret3;
		}
	}

	LockDM();
	opncb->resid = 1; /* Indicate that open processing is completed */
	UnlockDM();

	return DD(opncb);

err_ret3:
	LockDM();
	tk_del_sem(opncb->abort_semid);
err_ret2_5:
	knl_delOpnCB(opncb, TRUE);
err_ret2:
	UnlockDM();
err_ret1:
	return ercd;
}

/*
 * Abort all requests
 */
LOCAL void abort_allrequest( OpnCB *opncb )
{
	ABTFN	abortfn;
	WAIFN	waitfn;
	void	*exinf;
	DevCB	*devcb;
	ReqCB	*reqcb;
	QUEUE	*q;

	/* If 'execfn' and 'waitfn' are called, execute abort request. */
	LockDM();

	devcb = opncb->devcb;
	abortfn = (ABTFN)devcb->ddev.abortfn;
	waitfn  = (WAIFN)devcb->ddev.waitfn;
	exinf   = devcb->ddev.exinf;

	opncb->abort_tskid = tk_get_tid();
	opncb->abort_cnt = 0;

	if ( opncb->nwaireq > 0 ) {
		/* Multiple requests wait */
		reqcb = DEVREQ_REQCB(opncb->waireqlst);

		/* Device driver call */
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode++;
		ENABLE_INTERRUPT;
		(*abortfn)(reqcb->tskid, opncb->waireqlst, opncb->nwaireq, exinf);
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode--;
		ENABLE_INTERRUPT;

		opncb->abort_cnt++;
	} else {
		/* Start request or single request wait */
		for ( q = opncb->requestq.next; q != &opncb->requestq; q = q->next ) {
			reqcb = (ReqCB*)q;
			if ( reqcb->tskid == 0 ) {
				continue;
			}

			reqcb->req.abort = TRUE;

			/* Device driver call */
			DISABLE_INTERRUPT;
			knl_ctxtsk->sysmode++;
			ENABLE_INTERRUPT;
			(*abortfn)(reqcb->tskid, &reqcb->req, 1, exinf);
			DISABLE_INTERRUPT;
			knl_ctxtsk->sysmode--;
			ENABLE_INTERRUPT;

			opncb->abort_cnt++;
		}
	}

	UnlockDM();

	if ( opncb->abort_cnt > 0 ) {
		/* Wait for completion of abort request processing */
		tk_wai_sem(opncb->abort_semid, 1, TMO_FEVR);
	}
	opncb->abort_tskid = 0;

	/* Abort remaining requests and wait for completion */
	LockDM();
	while ( !isQueEmpty(&opncb->requestq) ) {
		reqcb = (ReqCB*)opncb->requestq.next;
		reqcb->req.abort = TRUE;

		UnlockDM();

		/* Device driver call */
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode++;
		ENABLE_INTERRUPT;
		(*waitfn)(&reqcb->req, 1, TMO_FEVR, exinf);
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode--;
		ENABLE_INTERRUPT;

		LockDM();

		/* Unregister completed request */
		knl_delReqCB(reqcb);
	}
	UnlockDM();
}

/*
 * Device close processing
 */
EXPORT ER knl_close_device( OpnCB *opncb, UINT option )
{
	CLSFN	closefn;
	void	*exinf;
	ID	devid;
	DevCB	*devcb;
	INT	unitno;
	ER	ercd = E_OK;

	/* Abort all requests during processing */
	abort_allrequest(opncb);

	LockDM();

	devcb  = opncb->devcb;
	unitno = opncb->unitno;
	closefn = (CLSFN)devcb->ddev.closefn;
	exinf = devcb->ddev.exinf;
	devid = DEVID(devcb, unitno);

	/* Delete semaphore for completion check of abortion */
	tk_del_sem(opncb->abort_semid);

	/* Free open management block */
	knl_delOpnCB(opncb, FALSE);

	/* Is device driver call required? */
	if ( knl_chkopen(devcb, unitno) ) {
		option &= ~TD_EJECT;
		if ( (devcb->ddev.drvatr & TDA_OPENREQ) == 0 ) {
			closefn = NULL;
		}
	}

	UnlockDM();

	if ( closefn != NULL ) {
		/* Device driver call */
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode++;
		ENABLE_INTERRUPT;
		ercd = (*closefn)(devid, option, exinf);
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode--;
		ENABLE_INTERRUPT;
	}

	LockDM();
	/* Return open management block to FreeQue */
	QueInsert(&opncb->q, &knl_FreeOpnCB);
	UnlockDM();

	return ercd;
}

/*
 * Device close
 */
SYSCALL ER tk_cls_dev( ID dd, UINT option )
{
	OpnCB	*opncb;
	ER	ercd;

	LockDM();

	ercd = knl_check_devdesc(dd, 0, &opncb);
	if ( ercd < E_OK ) {
		UnlockDM();
		goto err_ret;
	}

	opncb->resid = 0; /* Indicate that it is during close processing */

	UnlockDM();

	/* Device close processing */
	ercd = knl_close_device(opncb, option);

err_ret:
	return ercd;
}

/* ------------------------------------------------------------------------ */

/*
 * Get request management block
 */
LOCAL ReqCB* newReqCB( OpnCB *opncb )
{
	ReqCB	*reqcb;

	/* Get space in request management block */
	reqcb = (ReqCB*)QueRemoveNext(&knl_FreeReqCB);
	if ( reqcb == NULL ) {
		return NULL; /* No space */
	}

	/* Register as requested open device */
	QueInsert(&reqcb->q, &opncb->requestq);

	reqcb->opncb = opncb;

	return reqcb;
}

/*
 * Request for starting input/output to device
 */
EXPORT ID knl_request( ID dd, W start, void *buf, W size, TMO tmout, INT cmd )
{
	EXCFN	execfn;
	void	*exinf;
	OpnCB	*opncb;
	DevCB	*devcb;
	ReqCB	*reqcb;
	UINT	m;
	ER	ercd;

	LockDM();

	if ( start <= -0x00010000 && start >= -0x7fffffff ) {
		m = 0; /* Ignore open mode */
	} else {
		m = ( cmd == TDC_READ )? TD_READ: TD_WRITE;
	}
	ercd = knl_check_devdesc(dd, m, &opncb);
	if ( ercd < E_OK ) {
		goto err_ret1;
	}

	devcb = opncb->devcb;
	execfn = (EXCFN)devcb->ddev.execfn;
	exinf = devcb->ddev.exinf;

	/* Get request management block */
	reqcb = newReqCB(opncb);
	if ( reqcb == NULL ) {
		ercd = E_LIMIT;
		goto err_ret1;
	}

	/* Set request packet */
	reqcb->req.next   = NULL;
	reqcb->req.exinf  = NULL;
	reqcb->req.devid  = DEVID(devcb, opncb->unitno);
	reqcb->req.cmd    = cmd;
	reqcb->req.abort  = FALSE;
	reqcb->req.start  = start;
	reqcb->req.size   = size;
	reqcb->req.buf    = buf;
	reqcb->req.asize  = 0;
	reqcb->req.error  = 0;

	/* Indicate that it is during processing */
	reqcb->tskid = tk_get_tid();

	UnlockDM();

	/* Device driver call */
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode++;
	ENABLE_INTERRUPT;
	ercd = (*execfn)(&reqcb->req, tmout, exinf);
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode--;
	ENABLE_INTERRUPT;

	LockDM();

	/* Indicate that it is not during processing */
	reqcb->tskid = 0;

	/* If there is an abort completion wait task,
	   notify abort completion */
	if ( opncb->abort_tskid > 0 && --opncb->abort_cnt == 0 ) {
		tk_sig_sem(opncb->abort_semid, 1);
	}

	if ( ercd < E_OK ) {
		goto err_ret2;
	}

	UnlockDM();

	return REQID(reqcb);

err_ret2:
	knl_delReqCB(reqcb);
err_ret1:
	UnlockDM();
	return ercd;
}

/*
 * Start reading from device
 */
SYSCALL ID tk_rea_dev( ID dd, W start, void *buf, SZ size, TMO tmout )
{
	ER	ercd;

	ercd = knl_request(dd, start, buf, size, tmout, TDC_READ);

	return ercd;
}

/*
 * Synchronous reading from device
 */
SYSCALL ER tk_srea_dev( ID dd, W start, void *buf, SZ size, SZ *asize )
{
	ER	ercd, ioercd;

	ercd = tk_rea_dev(dd, start, buf, size, TMO_FEVR);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	ercd = tk_wai_dev(dd, ercd, asize, &ioercd, TMO_FEVR);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	return ioercd;

err_ret:
	return ercd;
}

/*
 * Start writing to device
 */
SYSCALL ID tk_wri_dev( ID dd, W start, CONST void *buf, SZ size, TMO tmout )
{
	ER	ercd;

	ercd = knl_request(dd, start, (void *)buf, size, tmout, TDC_WRITE);

	return ercd;
}

/*
 * Synchronous writing to device
 */
SYSCALL ER tk_swri_dev( ID dd, W start, CONST void *buf, SZ size, SZ *asize )
{
	ER	ercd, ioercd;

	ercd = tk_wri_dev(dd, start, buf, size, TMO_FEVR);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	ercd = tk_wai_dev(dd, ercd, asize, &ioercd, TMO_FEVR);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	return ioercd;

err_ret:
	return ercd;
}

/*
 * Verify validity of request ID
 */
LOCAL ReqCB* knl_check_reqid( ID reqid, OpnCB *opncb )
{
	ReqCB	*reqcb;

	if ( reqid < 1 || reqid > MAX_REQDEV ) {
		return NULL;
	}
	reqcb = REQCB(reqid);
	if ( reqcb->opncb != opncb ) {
		return NULL;
	}

	return reqcb;
}

/*
 * Request completion wait
 */
SYSCALL ID tk_wai_dev( ID dd, ID reqid, SZ *asize, ER *ioer, TMO tmout )
{
	WAIFN	waitfn;
	void	*exinf;
	OpnCB	*opncb;
	DevCB	*devcb;
	ReqCB	*reqcb;
	T_DEVREQ *devreq;
	INT	reqno, nreq;
	ID	tskid;
	ER	ercd;

	tskid = tk_get_tid();

	LockDM();

	ercd = knl_check_devdesc(dd, 0, &opncb);
	if ( ercd < E_OK ) {
		goto err_ret2;
	}

	devcb = opncb->devcb;
	waitfn = (WAIFN)devcb->ddev.waitfn;
	exinf = devcb->ddev.exinf;

	if ( reqid == 0 ) {
		/* When waiting for completion of any of requests for 'dd' */
		if ( opncb->nwaireq > 0 || opncb->waitone > 0 ) {
			ercd = E_OBJ;
			goto err_ret2;
		}
		if ( isQueEmpty(&opncb->requestq) ) {
			ercd = E_NOEXS;
			goto err_ret2;
		}

		/* Create wait request list */
		reqcb = (ReqCB*)opncb->requestq.next;
		for ( nreq = 1;; nreq++ ) {
			reqcb->tskid = tskid;
			devreq = &reqcb->req;
			reqcb = (ReqCB*)reqcb->q.next;
			if ( reqcb == (ReqCB*)&opncb->requestq ) {
				break;
			}
			devreq->next = &reqcb->req;
		}
		devreq->next = NULL;
		devreq = &((ReqCB*)opncb->requestq.next)->req;

		opncb->waireqlst = devreq;
		opncb->nwaireq = nreq;
	} else {
		/* Wait for completion of abort request processing */
		reqcb = knl_check_reqid(reqid, opncb);
		if ( reqcb == NULL ) {
			ercd = E_ID;
			goto err_ret2;
		}
		if ( opncb->nwaireq > 0 || reqcb->tskid > 0 ) {
			ercd = E_OBJ;
			goto err_ret2;
		}

		/* Create waiting request list */
		reqcb->tskid = tskid;
		devreq = &reqcb->req;
		devreq->next = NULL;
		nreq = 1;

		opncb->waitone++;
	}

	UnlockDM();

	/* Device driver call */
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode++;
	ENABLE_INTERRUPT;
	reqno = (*waitfn)(devreq, nreq, tmout, exinf);
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode--;
	ENABLE_INTERRUPT;

	if ( reqno <  E_OK ) {
		ercd = reqno;
	}
	if ( reqno >= nreq ) {
		ercd = E_SYS;
	}

	LockDM();

	/* Free wait processing */
	if ( reqid == 0 ) {
		opncb->nwaireq = 0;
	} else {
		opncb->waitone--;
	}

	/* If there is an abort completion wait task,
	   notify abort completion */
	if ( opncb->abort_tskid > 0 && --opncb->abort_cnt == 0 ) {
		tk_sig_sem(opncb->abort_semid, 1);
	}

	/* Get processing result */
	while ( devreq != NULL ) {
		reqcb = DEVREQ_REQCB(devreq);
		if ( reqno-- == 0 ) {
			reqid = REQID(reqcb);
			*asize = devreq->asize;
			*ioer  = devreq->error;
		}
		reqcb->tskid = 0;
		devreq = devreq->next;
	}

	if ( ercd < E_OK ) {
		goto err_ret2;
	}

	/* Unregister completed request */
	knl_delReqCB(REQCB(reqid));

	UnlockDM();

	return reqid;

err_ret2:
	UnlockDM();
	return ercd;
}

/* ------------------------------------------------------------------------ */

/* Suspend disable request count */
EXPORT INT	knl_DisSusCnt = 0;

/*
 * Send driver request event to each device
 */
LOCAL ER sendevt_alldevice( INT evttyp, BOOL disk )
{
	EVTFN	eventfn;
	QUEUE	*q;
	DevCB	*devcb;
	BOOL	d;
	ER	ercd = E_OK;

	for ( q = knl_UsedDevCB.next; q != &knl_UsedDevCB; q = q->next ) {
		devcb = (DevCB*)q;

		d = ( (devcb->ddev.devatr & TD_DEVTYPE) == TDK_DISK )?
							TRUE: FALSE;
		if ( disk != d ) {
			continue;
		}

		/* Device driver call */
		eventfn = (EVTFN)devcb->ddev.eventfn;
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode++;
		ENABLE_INTERRUPT;
		ercd = (*eventfn)(evttyp, NULL, devcb->ddev.exinf);
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode--;
		ENABLE_INTERRUPT;
	}

	return ercd;
}

/*
 * Suspend
 */
LOCAL ER do_suspend( void )
{
	ER	ercd;

	/* Stop accepting device registration/unregistration */
	LockREG();

	/* Suspend processing of device except for disks */
	ercd = sendevt_alldevice(TDV_SUSPEND, FALSE);

	/* Suspend processing of disk device */
	ercd = sendevt_alldevice(TDV_SUSPEND, TRUE);

	/* Stop accepting new requests */
	LockDM();

	/*
	 * Insert code to transit to suspend state here
	 */

	/*
	 * Insert code executed on returning from suspend state
	 */


	/* Resume accepting requests */
	UnlockDM();

	/* Resume processing of disk device */
	ercd = sendevt_alldevice(TDV_RESUME, TRUE);

	/* Resume processing of device except for disks */
	ercd = sendevt_alldevice(TDV_RESUME, FALSE);

	/* Resume accepting device registration/unregistration */
	UnlockREG();

	return ercd;
}

/*
 * Suspend processing
 */
SYSCALL INT tk_sus_dev( UINT mode )
{
	ResCB	*rescb;
	BOOL	suspend = FALSE;
	ER	ercd;

	/* Get resource management information */
	rescb = knl_GetResCB();
	if ( rescb == NULL ) {
		ercd = E_CTX;
		goto err_ret1;
	}

	LockDM();

	switch ( mode & 0xf ) {
	  case TD_SUSPEND:	/* Suspend */
		if ( knl_DisSusCnt > 0 && (mode & TD_FORCE) == 0 ) {
			ercd = E_BUSY;
			goto err_ret2;
		}
		suspend = TRUE;
		break;

	  case TD_DISSUS:	/* Disable suspend */
		if ( knl_DisSusCnt >= MAX_DISSUS ) {
			ercd = E_QOVR;
			goto err_ret2;
		}
		knl_DisSusCnt++;
		rescb->dissus++;
		break;
	  case TD_ENASUS:	/* Enable suspend */
		if ( rescb->dissus > 0 ) {
			rescb->dissus--;
			knl_DisSusCnt--;
		}
		break;

	  case TD_CHECK:	/* Get suspend disable request count */
		break;

	  default:
		ercd = E_PAR;
		goto err_ret2;
	}

	UnlockDM();

	if ( suspend ) {
		/* Suspend */
		ercd = do_suspend();
		if ( ercd < E_OK ) {
			goto err_ret1;
		}
	}

	return knl_DisSusCnt;

err_ret2:
	UnlockDM();
err_ret1:
	return ercd;
}

/* ------------------------------------------------------------------------ */

/*
 * Device management startup function
 */
EXPORT void knl_devmgr_startup( void )
{
	LockDM();

	/* Initialization of open device management queue */
	QueInit(&(knl_resource_control_block.openq));
	knl_resource_control_block.dissus = 0;
	
	UnlockDM();

	return;
}

/*
 * Device management cleanup function
 */
EXPORT void knl_devmgr_cleanup( void )
{
	OpnCB	*opncb;

	/* Do nothing if it is not used even once */
	if ( knl_resource_control_block.openq.next == NULL ) {
		return;
	}

	LockDM();

	/* Free suspend disable request */
	knl_DisSusCnt -= knl_resource_control_block.dissus;
	knl_resource_control_block.dissus = 0;

	/* Close all open devices */
	while ( !isQueEmpty(&(knl_resource_control_block.openq)) ) {
		opncb = RESQ_OPNCB(knl_resource_control_block.openq.next);

		/* Indicate that it is during close processing */
		opncb->resid = 0;

		UnlockDM();

		/* Device close processing */
		knl_close_device(opncb, 0);

		LockDM();
	}
	UnlockDM();

	return;
}

/*
 * Initialization sequence of device input/output-related
 */
EXPORT ER knl_initDevIO( void )
{
	INT	i;

	QueInit(&knl_FreeOpnCB);
	for ( i = 0; i < MAX_OPNDEV; ++i ) {
		knl_OpnCBtbl[i].resid = 0;
		QueInsert(&knl_OpnCBtbl[i].q, &knl_FreeOpnCB);
	}

	QueInit(&knl_FreeReqCB);
	for ( i = 0; i < MAX_REQDEV; ++i ) {
		knl_ReqCBtbl[i].opncb = NULL;
		QueInsert(&knl_ReqCBtbl[i].q, &knl_FreeReqCB);
	}

	return E_OK;
}

/*
 * Finalization sequence of device input/output-related
 */
EXPORT ER knl_finishDevIO( void )
{
	return E_OK;
}

#endif /* USE_DEVICE */
