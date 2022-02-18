/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21.
 *
 *----------------------------------------------------------------------
 */


/*
 *	msdrvif.c
 *
 *	mSDI: Simple Device driver I/F layer for micro T-Kernel
 */

#include <tk/tkernel.h>
#include "msdrvif.h"

/* ------------------------------------------------------------------------ */
/*
 * FastLock fot driver I/F access
 */

#define	LockMSDI(msdi)		Lock(&msdi->lock)
#define	UnlockMSDI(msdi)	Unlock(&msdi->lock)

/* ------------------------------------------------------------------------ */
/*
 *	Processing function
 */

/*
 * Open function
 */
LOCAL ER msdi_openfn( ID devid, UINT omode, T_MSDI *msdi )
{
	ER	err;

	if ( msdi->dmsdi.openfn == NULL ) return E_OK;

	Lock(&msdi->lock);
	err = (*msdi->dmsdi.openfn)(devid, omode, msdi);
	Unlock(&msdi->lock);

	return err;
}

/*
 * Close cunction
 */
LOCAL ER msdi_closefn( ID devid, UINT option, T_MSDI *msdi )
{
	ER	err;

	if ( msdi->dmsdi.closefn == NULL ) return E_OK;

	Lock(&msdi->lock);
	err = (*msdi->dmsdi.closefn)(devid, option, msdi);
	Unlock(&msdi->lock);

	return err;
}

/*
 * Processing start function
 */
LOCAL ER msdi_execfn( T_DEVREQ *req, TMO tmout, T_MSDI *msdi )
{
	INT	(*fp)( T_DEVREQ*, T_MSDI* );
	ER	err;

	fp = ( req->cmd == TDC_READ )? msdi->dmsdi.readfn: msdi->dmsdi.writefn;
	if ( fp == NULL ) return E_NOSPT;

	if ( req->start >= 0 && msdi->dmsdi.blksz <= 0) return E_NOSPT;

	/* I/O processing */
	Lock(&msdi->lock);
	err = (*fp)(req, msdi);
	Unlock(&msdi->lock);

	req->error = err;
	return E_OK;
}

/*
 * Wait-for-completion function
 */
LOCAL INT msdi_waitfn( T_DEVREQ *req, INT nreq, TMO tmout, T_MSDI *msdi )
{
	return 0;
}

/*
 * Abort processing function
 */
LOCAL ER msdi_abortfn( ID tskid, T_DEVREQ *req, INT nreq, T_MSDI *msdi )
{
	return E_OK;
}

/*
 * Event function
 */
LOCAL INT msdi_eventfn( INT evttyp, void *evtinf, T_MSDI *msdi )
{
	INT	ret;

	if ( msdi->dmsdi.eventfn == NULL ) return E_OK;

	Lock(&msdi->lock);
	ret = (*msdi->dmsdi.eventfn)(evttyp, evtinf, msdi);
	Unlock(&msdi->lock);

	return ret;
}

/* ------------------------------------------------------------------------ */
/*
 *	Device registration
 */

/*
 * SDI Device registration
 */
EXPORT ER msdi_def_dev( T_DMSDI *dmsdi, T_IDEV *idev, T_MSDI **p_msdi )
{
	T_MSDI	*msdi;
	T_DDEV	ddev;
	ER	err;

	/* Create "SDI"*/
	msdi = Kmalloc(sizeof(T_MSDI));
	if ( msdi == NULL ) {
		err = E_NOMEM;
		goto err_ret1;
	}

	msdi->dmsdi = *dmsdi;		/* Structure Copy */

	/* Create the lock for exclusive access control */
	err = CreateLock(&msdi->lock, dmsdi->devnm);
	if ( err < E_OK ) goto err_ret2;

	/* Device registration */
	ddev.exinf   = msdi;
	ddev.drvatr  = dmsdi->drvatr;
	ddev.devatr  = dmsdi->devatr;
	ddev.nsub    = dmsdi->nsub;
	ddev.blksz   = dmsdi->blksz;
	ddev.openfn  = (FP)msdi_openfn;
	ddev.closefn = (FP)msdi_closefn;
	ddev.execfn  = (FP)msdi_execfn;
	ddev.waitfn  = (FP)msdi_waitfn;
	ddev.abortfn = (FP)msdi_abortfn;
	ddev.eventfn = (FP)msdi_eventfn;

	err = tk_def_dev(msdi->dmsdi.devnm, &ddev, idev);
	if ( err < E_OK )	goto err_ret3;
	
	msdi->devid = (ID)err;
	if(p_msdi != NULL) {
		*p_msdi = msdi;
	}

	return E_OK;

err_ret3:
	DeleteLock(&msdi->lock);
err_ret2:
	Kfree(msdi);
err_ret1:
	return err;
}

/*
 * Deregistration
 */
EXPORT ER msdi_del_dev( T_MSDI *msdi )
{
	ER	err;

	/* Deregister device */
	err = tk_def_dev(msdi->dmsdi.devnm, NULL, NULL);
	if ( err > E_OK ) {
		DeleteLock(&msdi->lock);	/* Delete the lock for exclusive access control */
		Kfree(msdi);			/* Delete "SDI" */
		err = E_OK;
	}
	return err;
}

