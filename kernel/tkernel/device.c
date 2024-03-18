/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.02
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21.
 *
 *----------------------------------------------------------------------
 */

/*
 *	device.c
 *	Device Management Function
 */

#include "kernel.h"
#include "sysmgr.h"
#include "device.h"

#if USE_DEVICE

/* Lock for device management exclusive control */
Noinit(EXPORT	FastMLock	knl_DevMgrLock);

/* Device initial setting information */
Noinit(EXPORT	T_IDEV		knl_DefaultIDev);

/* ------------------------------------------------------------------------ */
/*
 *	Device registration management
 */

Noinit(EXPORT	DevCB		knl_DevCBtbl[MAX_REGDEV]);	/* Device registration information table */
Noinit(EXPORT	QUEUE		knl_UsedDevCB);	/* In-use queue */
Noinit(EXPORT	QUEUE		knl_FreeDevCB);	/* Unused queue */


/*
 * Search registration device
 */
EXPORT DevCB* knl_searchDevCB( CONST UB *devnm )
{
	QUEUE	*q;
	DevCB	*devcb;

	for ( q = knl_UsedDevCB.next; q != &knl_UsedDevCB; q = q->next ) {
		devcb = (DevCB*)q;

		if ( devcb->devnm[0] == devnm[0] && knl_strcmp((char*)devcb->devnm, (char*)devnm) == 0 ) {
			return devcb; /* Found */
		}
	}

	return NULL;
}

/*
 * Get DevCB for new registration
 */
LOCAL DevCB* newDevCB( CONST UB *devnm )
{
	DevCB	*devcb;

	devcb = (DevCB*)QueRemoveNext(&knl_FreeDevCB);
	if ( devcb == NULL ) {
		return NULL; /* No space */
	}

	knl_strncpy((char*)devcb->devnm, (char*)devnm, L_DEVNM+1);
	QueInit(&devcb->openq);

	QueInsert(&devcb->q, &knl_UsedDevCB);

	return devcb;
}

/*
 * Free DevCB
 */
LOCAL void delDevCB( DevCB *devcb )
{
	QueRemove(&devcb->q);
	QueInsert(&devcb->q, &knl_FreeDevCB);
	devcb->devnm[0] = '\0';
}

/*
 * Device registration
 */
SYSCALL ID tk_def_dev( CONST UB *devnm, CONST T_DDEV *pk_ddev, T_IDEV *pk_idev )
{
	DevCB	*devcb;
	INT	len;
	ER	ercd;

	LockREG();

	len = knl_strlen((char*)devnm);
	if ( len == 0 || len > L_DEVNM ) {
		ercd = E_PAR;
		goto err_ret1;
	}

	if ( pk_ddev != NULL ) {
		if ( pk_ddev->nsub < 0 || pk_ddev->nsub > MAX_UNIT ) {
			ercd = E_PAR;
			goto err_ret1;
		}

		/* Make sure that the length of the logical device name
		   does not exceed the character limit */
		if ( pk_ddev->nsub > 0   ) {
			++len;
		}
		if ( pk_ddev->nsub > 10  ) {
			++len;
		}
		if ( pk_ddev->nsub > 100 ) {
			++len;
		}
		if ( len > L_DEVNM ) {
			ercd = E_PAR;
			goto err_ret1;
		}
	}

	LockDM();

	/* Search whether 'devnm' device is registered */
	devcb = knl_searchDevCB(devnm);
	if ( devcb == NULL ) {
		if ( pk_ddev == NULL ) {
			ercd = E_NOEXS;
			goto err_ret2;
		}

		/* Get 'devcb' for new registration because it is not
		   registered */
		devcb = newDevCB(devnm);
		if ( devcb == NULL ) {
			ercd = E_LIMIT;
			goto err_ret2;
		}
	}

	if ( pk_ddev != NULL ) {
		/* Set/update device registration information */
		devcb->ddev = *pk_ddev;

		if ( pk_idev != NULL ) {
			/* Device initial setting information */
			*pk_idev = knl_DefaultIDev;
		}
	} else {
		if ( !isQueEmpty(&devcb->openq) ) {
			/* In use (open) */
			ercd = E_BUSY;
			goto err_ret2;
		}

		/* Device unregistration */
		delDevCB(devcb);
	}

	UnlockDM();
	UnlockREG();

	return DID(devcb);

err_ret2:
	UnlockDM();
err_ret1:
	UnlockREG();
	return ercd;
}

/*
 * Check device initial information
 */
SYSCALL ER tk_ref_idv( T_IDEV *pk_idev )
{
	LockDM();
	*pk_idev = knl_DefaultIDev;
	UnlockDM();

	return E_OK;
}

/* ------------------------------------------------------------------------ */

/*
 * Get physical device name
 *	Get the subunit number (return value) 
 *	from the logical device name (ldevnm) and the physical
 *	device name (pdevnm).
 */
EXPORT INT knl_phydevnm( UB *pdevnm, CONST UB *ldevnm )
{
	UB	c;
	INT	unitno;

	while ( (c = *ldevnm) != '\0' ) {
		if ( c >= '0' && c <= '9' ) {
			break;
		}
		*pdevnm++ = c;
		ldevnm++;
	}
	*pdevnm = '\0';

	unitno = 0;
	if (c != '\0') {
		while ( (c = *ldevnm) != '\0' ) {
			unitno = unitno * 10 + (c - '0');
			ldevnm++;
		}
		++unitno;
	}

	return unitno;
}

/*
 * Get logical device name
 *	Get the logical device name from
 *	the physical device name (pdevnm) and the subunit number (unitno).
 */
LOCAL void logdevnm( UB *ldevnm, UB *pdevnm, INT unitno )
{
	UB	unostr[12], *cp;

	knl_strcpy((char*)ldevnm, (char*)pdevnm);
	if ( unitno > 0 ) {
		cp = &unostr[11];
		*cp = '\0';
		while (*ldevnm != '\0') {
			++ldevnm;
		}
		--unitno;
		do {
			*(--cp) = (UB)('0' + (unitno % 10));
			unitno /= 10;
		} while (unitno);
		knl_strcat((char*)ldevnm, (char*)cp);
	}
}

/*
 * Get device name
 */
SYSCALL ID tk_get_dev( ID devid, UB *devnm )
{
	DevCB	*devcb;
	ER	ercd;

	ercd = knl_check_devid(devid);
	if ( ercd < E_OK ) {
		goto err_ret1;
	}

	LockDM();

	devcb = DEVCB(devid);
	if ( (devcb->devnm[0] == '\0')||(UNITNO(devid) > devcb->ddev.nsub) ) {
		ercd = E_NOEXS;
		goto err_ret2;
	}

	logdevnm(devnm, devcb->devnm, UNITNO(devid));

	UnlockDM();

	return DID(devcb);

err_ret2:
	UnlockDM();
err_ret1:
	return ercd;
}

/*
 * Get device information
 */
SYSCALL ID tk_ref_dev( CONST UB *devnm, T_RDEV *pk_rdev )
{
	UB	pdevnm[L_DEVNM + 1];
	DevCB	*devcb;
	INT	unitno;
	ER	ercd;

	unitno = knl_phydevnm(pdevnm, devnm);

	LockDM();

	devcb = knl_searchDevCB(pdevnm);
	if ( devcb == NULL || unitno > devcb->ddev.nsub ) {
		ercd = E_NOEXS;
		goto err_ret2;
	}

	if ( pk_rdev != NULL ) {
		pk_rdev->devatr = devcb->ddev.devatr;
		pk_rdev->blksz  = devcb->ddev.blksz;
		pk_rdev->nsub   = devcb->ddev.nsub;
		pk_rdev->subno  = unitno;
	}

	UnlockDM();

	return DEVID(devcb, unitno);

err_ret2:
	UnlockDM();
	return ercd;
}

/*
 * Get device information
 */
SYSCALL ID tk_oref_dev( ID dd, T_RDEV *pk_rdev )
{
	OpnCB	*opncb;
	DevCB	*devcb;
	INT	unitno;
	ER	ercd;

	LockDM();

	ercd = knl_check_devdesc(dd, 0, &opncb);
	if ( ercd < E_OK ) {
		goto err_ret2;
	}

	devcb  = opncb->devcb;
	unitno = opncb->unitno;

	if ( pk_rdev != NULL ) {
		pk_rdev->devatr = devcb->ddev.devatr;
		pk_rdev->blksz  = devcb->ddev.blksz;
		pk_rdev->nsub   = devcb->ddev.nsub;
		pk_rdev->subno  = unitno;
	}

	UnlockDM();

	return DEVID(devcb, unitno);

err_ret2:
	UnlockDM();
	return ercd;
}

/*
 * Get registration device list
 */
SYSCALL INT tk_lst_dev( T_LDEV *pk_ldev, INT start, INT ndev )
{
	DevCB	*devcb;
	QUEUE	*q;
	INT	n, end;
	ER	ercd;

	if ( start < 0 || ndev < 0 ) {
		ercd = E_PAR;
		goto err_ret;
	}
	LockDM();

	end = start + ndev;
	n = 0;
	for ( q = knl_UsedDevCB.next; q != &knl_UsedDevCB; q = q->next ) {
		if ( n >= start && n < end ) {
			devcb = (DevCB*)q;
			pk_ldev->devatr = devcb->ddev.devatr;
			pk_ldev->blksz  = devcb->ddev.blksz;
			pk_ldev->nsub   = devcb->ddev.nsub;
			knl_strncpy((char*)pk_ldev->devnm, (char*)devcb->devnm, L_DEVNM);
			pk_ldev++;
		}
		n++;
	}

	UnlockDM();

	if ( start >= n ) {
		ercd = E_NOEXS;
		goto err_ret;
	}

	return n - start;

err_ret:
	return ercd;
}

/* ------------------------------------------------------------------------ */

/*
 * Send driver request event
 */
SYSCALL INT tk_evt_dev( ID devid, INT evttyp, void *evtinf )
{
	DevCB	*devcb;
	EVTFN	eventfn;
	void	*exinf;
	ER	ercd;

	ercd = knl_check_devid(devid);
	if ( ercd < E_OK ) {
		goto err_ret1;
	}
	if ( evttyp < 0 ) {
		ercd = E_PAR;
		goto err_ret1;
	}

	LockDM();

	devcb = DEVCB(devid);
	if ( (devcb->devnm[0] == '\0')||(UNITNO(devid) > devcb->ddev.nsub) ) {
		ercd = E_NOEXS;
		goto err_ret2;
	}

	eventfn = (EVTFN)devcb->ddev.eventfn;
	exinf = devcb->ddev.exinf;

	UnlockDM();

	/* Device driver call */
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode++;
	ENABLE_INTERRUPT;
	ercd = (*eventfn)(evttyp, evtinf, exinf);
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode--;
	ENABLE_INTERRUPT;

	return ercd;

err_ret2:
	UnlockDM();
err_ret1:
	return ercd;
}

/* ------------------------------------------------------------------------ */

/*
 * Initialization of device registration information table
 */
LOCAL ER initDevCB( void )
{
	DevCB	*devcb;
	INT	num = MAX_REGDEV;

	QueInit(&knl_UsedDevCB);
	QueInit(&knl_FreeDevCB);

	devcb = knl_DevCBtbl;
	while ( num-- > 0 ) {
		QueInsert(&devcb->q, &knl_FreeDevCB);
		devcb->devnm[0] = '\0';
		devcb++;
	}

	return E_OK;
}

/*
 * Initialization of device initial setting information
 */
LOCAL ER initIDev( void )
{
	ER	ercd;

#if DEVT_MBFSZ0 >= 0
	T_CMBF	cmbf;

	/* Generate message buffer for event notification */
	knl_strncpy((char*)&cmbf.exinf, (char*)OBJNAME_DMMBF, sizeof(cmbf.exinf));
	cmbf.mbfatr = TA_TFIFO;
	cmbf.bufsz  = DEVT_MBFSZ0;
	cmbf.maxmsz = DEVT_MBFSZ1;
	ercd = tk_cre_mbf(&cmbf);
	if ( ercd < E_OK ) {
		knl_DefaultIDev.evtmbfid = 0;
		goto err_ret;
	}
#else	/* Do not use message buffer for event notification */
	ercd = E_OK;
#endif
	knl_DefaultIDev.evtmbfid = ercd;

#if DEVT_MBFSZ0 >= 0
err_ret:
#endif
	return ercd;
}

/*
 * Initialization of Device management
 */
EXPORT ER knl_initialize_devmgr( void )
{
	ER	ercd;

	/* Generate lock for device management exclusive control */
	ercd = CreateMLock(&knl_DevMgrLock, (UB*)OBJNAME_DMLOCK);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	/* Generate device registration information table */
	ercd = initDevCB();
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	/* Initialization of device input/output-related */
	ercd = knl_initDevIO();
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	/* Initialization of device initial setting information */
	ercd = initIDev();
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	knl_devmgr_startup();

	return E_OK;

err_ret:
	knl_finish_devmgr();
	return ercd;
}

/*
 * Unregister device initial setting information
 */
LOCAL ER delIDev( void )
{
	ER	ercd = E_OK;

#if DEVT_MBFSZ0 >= 0
	/* Delete message buffer for event notification */
	if ( knl_DefaultIDev.evtmbfid > 0 ) {
		ercd = tk_del_mbf(knl_DefaultIDev.evtmbfid);
		knl_DefaultIDev.evtmbfid = 0;
	}


#endif /* DEVT_MBFSZ0 >= 0 */

	return ercd;
}

/*
 * Finalization sequence of system management 
 */
EXPORT ER knl_finish_devmgr( void )
{
	ER	ercd;

	knl_devmgr_cleanup();

	/* Unregister device initial setting information */
	ercd = delIDev();

	/* Finalization sequence of device input/output-related */
	ercd = knl_finishDevIO();

	/* Delete lock for device management exclusive control */
	DeleteMLock(&knl_DevMgrLock);

	return ercd;
}

#endif /* USE_DEVICE */
