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
 *	msdrvif.h
 *
 *	mSDI: Simple Device driver I/F layer for micro T-Kernel
 */

#ifndef	__DEVICE_MSDI_H__
#define	__DEVICE_MSDI_H__

/*
 * Device registration information
 */
typedef struct S_MSDI	T_MSDI;

typedef struct {
	void*	exinf;		/* Extended information (may be optional) */
	UB	devnm[L_DEVNM+1]; /* Physical device name */
	ATR	drvatr;		/* Driver attributes */
	ATR	devatr;		/* Device attributes */
	INT	nsub;		/* Number of subunits */
	INT	blksz;		/* Unique data block size (-1 = unknown) */

	ER  (*openfn)( ID devid, UINT omode, T_MSDI* );
	ER  (*closefn)( ID devid, UINT option, T_MSDI* );
	INT (*readfn)( T_DEVREQ *req, T_MSDI *p_msdi );
	INT (*writefn)( T_DEVREQ *req, T_MSDI *p_msdi );
	INT (*eventfn)( INT evttyp, void *evtinf, T_MSDI* );
} T_DMSDI;

struct S_MSDI {
	ID		devid;	/* Device ID*/
	FastLock	lock;	/* Lock for exclusive access control */
	T_DMSDI		dmsdi;	/* Device registration information */
};


/*
 * Device registration & Delete
 * 
 */
IMPORT ER msdi_def_dev( T_DMSDI *ddev, T_IDEV *idev, T_MSDI** );	/* Register */
IMPORT ER msdi_del_dev( T_MSDI* );					/* Delete */

#endif /* __DEVICE_MSDI_H__ */
