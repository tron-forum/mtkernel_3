/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */
#include <sys/machine.h>
#include <tk/tkernel.h>
#include <tstdlib.h>

#include "adc.h"
#include "../include/dev_def.h"
#if DEV_ADC_ENABLE
/*
 *	adc.c
 *	A/D converter device driver 
*/

/*----------------------------------------------------------------------*/
/* Device driver Control block
 */
#if TK_SUPPORT_MEMLIB

LOCAL T_ADC_DCB	*dev_adc_cb[DEV_ADC_UNITNM];
#define		get_dcb_ptr(unit)	(dev_adc_cb[unit])
#define		get_dcb_mem(unit,a)	(dev_adc_cb[unit]->a)

#else

LOCAL T_ADC_DCB	dev_adc_cb[DEV_ADC_UNITNM];
#define		get_dcb_ptr(unit)	(&dev_adc_cb[unit])
#define		get_dcb_mem(unit,a)	(dev_adc_cb[unit].a)

#endif	/* TK_SUPPORT_MEMLIB */

/*----------------------------------------------------------------------*/
/* Attribute data control
 */
LOCAL ER read_atr(T_ADC_DCB *p_dcb, T_DEVREQ *req)
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


LOCAL ER write_atr(T_ADC_DCB *p_dcb, T_DEVREQ *req)
{
	ER	err	= E_OK;

	switch(req->start) {
	case TDN_EVENT:			/* MBF ID for event notification */
		if(req->size >= sizeof(ID)) {
			p_dcb->evtmbfid = *(ID*)req->buf;
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

/*----------------------------------------------------------------------*/
/* Device-specific data control
 */
LOCAL ER read_data(T_ADC_DCB *p_dcb, T_DEVREQ *req)
{
	W	rtn;
	ER	err = E_OK;

	if(req->size) {
		rtn = dev_adc_llctl( p_dcb->unit, LLD_ADC_READ, req->start, req->size, req->buf);
		if(rtn > 0) {
			req->asize = rtn;
			if(req->size != rtn) err = E_IO;
		} else {
			err = (ER)rtn;
		}
	} else {
		req->asize = dev_adc_llctl( p_dcb->unit, LLD_ADC_RSIZE, req->start, 0, 0);
	}

	return err;
}


/*----------------------------------------------------------------------
 * mSDI I/F function
 */
/*----------------------------------------------------------------------
 * Open device
 */
ER dev_adc_openfn( ID devid, UINT omode, T_MSDI *msdi)
{
	T_ADC_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_ADC_DCB*)(msdi->dmsdi.exinf);

	/* Device Open operation */
	p_dcb->omode = omode;
	err = (ER)dev_adc_llctl( p_dcb->unit, LLD_ADC_OPEN, 0, 0, 0);

	return err;
}

/*----------------------------------------------------------------------
 * Close Device
 */
ER dev_adc_closefn( ID devid, UINT option, T_MSDI *msdi)
{
	T_ADC_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_ADC_DCB*)(msdi->dmsdi.exinf);

	/* Device Close operation */
	err =(ER)dev_adc_llctl( p_dcb->unit, LLD_ADC_CLOSE, 0, 0, 0);
	
	return err;
}

/*----------------------------------------------------------------------
 * Read Device
 */
ER dev_adc_readfn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_ADC_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_ADC_DCB*)(p_msdi->dmsdi.exinf);

	if(req->start >= 0) {	// Device specific data
		if( p_dcb->omode & TD_READ ) {
			err = read_data( p_dcb, req);
		} else {
			err = E_OACV;
		}
	} else {		// Device attribute data
		err = read_atr( p_dcb, req);
	}

	return err;
}

/*----------------------------------------------------------------------
 * Write Device
 */
ER dev_adc_writefn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_ADC_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_ADC_DCB*)(p_msdi->dmsdi.exinf);

	if(req->start >= 0) {	// Device specific data
		err = E_PAR;
	} else {		// Device attribute data
		err = write_atr( p_dcb, req);
	}

	return err;
}

/*----------------------------------------------------------------------
 * Event Device
 */
ER dev_adc_eventfn( INT evttyp, void *evtinf, T_MSDI *msdi)
{
	return E_NOSPT;
}

/*----------------------------------------------------------------------
 * Device driver initialization and registration
 */
EXPORT ER dev_init_adc( UW unit )
{
	T_ADC_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_ADC_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_ADC_DCB*)Kmalloc(sizeof(T_ADC_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_adc_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_adc_cb[unit];
#endif	/* TK_SUPPORT_MEMLIB */

	/* Device registration information */
	dmsdi.exinf	= p_dcb;
	dmsdi.drvatr	= 0;			/* Driver attributes */
	dmsdi.devatr	= TDK_UNDEF;		/* Device attributes */
	dmsdi.nsub	= 0;			/* Number of subunits */
	dmsdi.blksz	= 1;			/* Unique data block size (-1 = unknown) */
	dmsdi.openfn	= dev_adc_openfn;
	dmsdi.closefn	= dev_adc_closefn;
	dmsdi.readfn	= dev_adc_readfn;
	dmsdi.writefn	= dev_adc_writefn;
	dmsdi.eventfn	= dev_adc_eventfn;
	
	knl_strcpy( (char*)dmsdi.devnm, DEVCNF_ADC_DEVNAME);
	i = knl_strlen(DEVCNF_ADC_DEVNAME);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_2;

	p_dcb->unit	= unit;
	p_dcb->evtmbfid = idev.evtmbfid;

	/* Low-level device initialization */
	err = (ER)dev_adc_llinit( get_dcb_ptr(unit));
	if(err != E_OK) goto err_1;

	return E_OK;

err_1:
	msdi_del_dev(p_msdi);
err_2:
	Kfree(p_dcb);
	return err;
}

#endif	/* DEV_ADC_ENABLE */