/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#include <tk/tkernel.h>
#include <tstdlib.h>

#if USE_SDEV_DRV
#include "i2c.h"
#include "../include/dev_def.h"
#if DEV_IIC_ENABLE

/*
 *	i2c.c
 *	I2C device driver
*/

/*---------------------------------------------------------------------*/
/*I2C Device driver Control block
 */
#if TK_SUPPORT_MEMLIB

LOCAL T_I2C_DCB	*dev_i2c_cb[DEV_I2C_UNITNM];
#define		get_dcb_ptr(unit)	(dev_i2c_cb[unit])
#define		get_dcb_mem(unit,a)	(&dev_i2c_cb[unit]->a)

#else

LOCAL T_I2C_DCB	dev_i2c_cb[DEV_I2C_UNITNM];
#define		get_dcb_ptr(unit)	(&dev_I2C_cb[unit])
#define		get_dcb_mem(unit,a)	(&dev_i2c_cb[unit].a)

#endif

/*---------------------------------------------------------------------*/
/* Attribute data control
 */
LOCAL ER read_atr(T_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	ER	err	= E_OK;

	switch(req->start) {
	case TDN_EVENT:			/* MBF ID for event notification */
		if(req->size >= sizeof(ID)) {
			*(ID*)req->buf = p_dcb->evtmbfid;
		} else if(req->size != 0) {
			err = E_PAR;
			break;
		}
		req->asize = sizeof(ID);
		break;
	default:
		err = E_PAR;
		break;
	}

	return err;
}


LOCAL ER write_atr(T_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	T_I2C_EXEC	*p_ex;
	W		rtn;
	ER		err	= E_OK;

	switch(req->start) {
	case TDN_EVENT:			/* MBF ID for event notification */
		if(req->size >= sizeof(ID)) {
			p_dcb->evtmbfid = *(ID*)(req->buf);
		} else if(req->size != 0) {
			err = E_PAR;
			break;
		}
		req->asize = sizeof(ID);
		break;
	case TDN_I2C_EXEC:
		if(req->size >= sizeof(T_I2C_EXEC)) {
			p_ex = (T_I2C_EXEC*)(req->buf);
			if(p_ex->snd_size <=0 || p_ex->snd_size > DEVCNF_I2C_MAX_SDATSZ) return E_PAR;
			if(p_ex->rcv_size <=0 || p_ex->rcv_size > DEVCNF_I2C_MAX_RDATSZ) return E_PAR;

			rtn = dev_i2c_llctl(p_dcb->unit, LLD_I2C_EXEC, req->start, req->size, (UW*)p_ex);
			if(rtn > 0) {
				req->asize = sizeof(T_I2C_EXEC);
			} else {
				err = (ER)rtn;
			}
		} else if(req->size == 0){
			req->asize = sizeof(T_I2C_EXEC);
		} else {
			err = E_PAR;
		}
		break;

	default:
		err = E_PAR;
		break;
	}

	return err;
}

/*---------------------------------------------------------------------*/
/*Device-specific data control
 */
LOCAL ER read_data(T_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	W	rtn;
	ER	err	= E_OK;

	if(req->size <=0 || req->size > DEVCNF_I2C_MAX_RDATSZ) return E_PAR;

	if(req->size) {
		rtn = dev_i2c_llctl(p_dcb->unit, LLD_I2C_READ, req->start, req->size, req->buf);
		if(rtn > 0) {
			req->asize = rtn;
		} else {
			err = (ER)rtn;
		}

	} else {
		req->asize = req->size;
	}

	return err;
}


LOCAL ER write_data(T_I2C_DCB *p_dcb, T_DEVREQ *req)
{
	W	rtn;
	ER	err	= E_OK;

	if(req->size <=0 || req->size > DEVCNF_I2C_MAX_SDATSZ) return E_PAR;

	if(req->size) {
		rtn = dev_i2c_llctl(p_dcb->unit, LLD_I2C_WRITE, req->start, req->size, req->buf);
		if(rtn > 0) {
			req->asize = rtn;
		} else {
			err = (ER)rtn;
		}

	} else {
		req->asize = req->size;
	}


	return err;
}

/*-------------------------------------------------------
 * mSDI I/F function
 *-------------------------------------------------------
 */
/*----------------------------------------------------------------------
 * Open device
 */
ER dev_i2c_openfn( ID devid, UINT omode, T_MSDI *msdi)
{
	T_I2C_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_I2C_DCB*)(msdi->dmsdi.exinf);

	/* Device Open operation */
	p_dcb->omode = omode;
	err = (ER)dev_i2c_llctl(p_dcb->unit, LLD_I2C_OPEN, 0, 0, 0);

	return err;
}

/*----------------------------------------------------------------------
 * Close Device
 */
ER dev_i2c_closefn( ID devid, UINT option, T_MSDI *msdi)
{
	T_I2C_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_I2C_DCB*)(msdi->dmsdi.exinf);

	/* Device Close operation */
	err = (ER)dev_i2c_llctl(p_dcb->unit, LLD_I2C_CLOSE, 0, 0, 0);

	return err;
}

/*----------------------------------------------------------------------
 * Read Device
 */
ER dev_i2c_readfn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_I2C_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_I2C_DCB*)(p_msdi->dmsdi.exinf);

	if(req->start >= 0) {
		// Device specific data
		err = read_data( p_dcb, req);
	} else {
		// Device attribute data
		err = read_atr( p_dcb, req);
	}

	return err;
}

/*----------------------------------------------------------------------
 * Write Device
 */
ER dev_i2c_writefn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_I2C_DCB	*p_dcb;
	ER		rtn;

	p_dcb = (T_I2C_DCB*)(p_msdi->dmsdi.exinf);

	if(req->start >= 0) {
		// Device specific data
		rtn = write_data( p_dcb, req);
	} else {
		// Device attribute data
		rtn = write_atr( p_dcb, req);
	}

	return rtn;
}

/*----------------------------------------------------------------------
 * Event Device
 */
ER dev_i2c_eventfn( INT evttyp, void *evtinf, T_MSDI *msdi)
{
	return E_NOSPT;
}

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */
EXPORT ER dev_init_i2c( UW unit )
{
	T_I2C_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_I2C_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_I2C_DCB*)Kmalloc(sizeof(T_I2C_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_i2c_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_i2c_cb[unit];
#endif

	/* Device registration information */
	dmsdi.exinf	= p_dcb;
	dmsdi.drvatr	= 0;			/* Driver attributes */
	dmsdi.devatr	= TDK_UNDEF;		/* Device attributes */
	dmsdi.nsub	= 0;			/* Number of sub units */
	dmsdi.blksz	= 1;			/* Unique data block size (-1 = unknown) */
	dmsdi.openfn	= dev_i2c_openfn;
	dmsdi.closefn	= dev_i2c_closefn;
	dmsdi.readfn	= dev_i2c_readfn;
	dmsdi.writefn	= dev_i2c_writefn;
	dmsdi.eventfn	= dev_i2c_eventfn;
	
	knl_strcpy( (char*)dmsdi.devnm, DEVCNF_I2C_DEVNAME);
	i = knl_strlen(DEVCNF_I2C_DEVNAME);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_2;

	p_dcb->unit	= unit;
	p_dcb->evtmbfid = idev.evtmbfid;

	/* Low-level device initialization */
	err = dev_i2c_llinit( get_dcb_ptr(unit));
	if(err != E_OK) goto err_1;

	/* Set default value for attribute data */

	return E_OK;

err_1:
	msdi_del_dev(p_msdi);
err_2:
	Kfree(p_dcb);
	return err;
}

/*----------------------------------------------------------------------
 * I2C register access support function
 */

EXPORT ER i2c_read_reg(ID dd, UW sadr, UW radr, UB *data)
{
	T_I2C_EXEC	exec;
	SZ		asz;
	ER		err;

	exec.sadr	= sadr;
	exec.snd_size	= 1;
	exec.snd_data	= (UB*)&radr;
	exec.rcv_size	= 1;
	exec.rcv_data	= data;

	err = tk_swri_dev(dd, TDN_I2C_EXEC, &exec, sizeof(T_I2C_EXEC), &asz);

	return err;
}

EXPORT ER i2c_write_reg(ID dd, UW sadr, UW radr, UB data)
{
	UB	snd_data[2];
	SZ	asz;
	ER	err;

	snd_data[0] = radr;
	snd_data[1] = data;
	
	err = tk_swri_dev(dd, sadr, snd_data, sizeof(snd_data), &asz);

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* USE_SDEV_DRV */