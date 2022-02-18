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

#include "ser.h"
#include "../include/dev_def.h"
#if DEV_SER_ENABLE
/*
 *	ser.c
 *	Serial communication driver
*/

/*----------------------------------------------------------------------*/
/* Serial Device driver Control block
 */
#if TK_SUPPORT_MEMLIB

LOCAL T_SER_DCB	*dev_ser_cb[DEV_SER_UNITNM];
#define		get_dcb_mem(unit,a)	(dev_ser_cb[unit]->a)

#else

LOCAL T_SER_DCB	dev_ser_cb[DEV_SER_UNITNM];
#define		get_dcb_mem(unit,a)	(dev_ser_cb[unit].a)

#endif

/*----------------------------------------------------------------------*/
/* Serial device driver low-level interface
 *
 *	Called from a low level interrupt handler.
 */
EXPORT BOOL dev_ser_get_snddat(UW unit, UW *data)
{
	T_SER_BUFF	*p_buff;

	p_buff = &get_dcb_mem(unit, snd_buff);
	if(p_buff->top != p_buff->tail) {
		*data = p_buff->data[p_buff->tail];
		if(++p_buff->tail >= DEVCONF_SER_BUFFSIZE) p_buff->tail = 0;

		if(p_buff->wait_tskid) {
			tk_wup_tsk( p_buff->wait_tskid);
			p_buff->wait_tskid = 0;
		}
		return TRUE;
	} else {
		return FALSE;
	}
}

EXPORT void dev_ser_notify_rcv(UW unit, UW data)
{
	T_SER_BUFF	*p_buff;
	INT		next;

	p_buff = &get_dcb_mem( unit, rcv_buff);
	next = p_buff->top + 1;
	if(next >= DEVCONF_SER_BUFFSIZE) next = 0;
	if(next != p_buff->tail) {
		p_buff->data[p_buff->top] = data;
		p_buff->top = next;
	} else {	
		dev_ser_notify_err( unit, DEV_SER_ERR_ROVR);	/* Buffer over flow */
	}

	if(p_buff->wait_tskid) {
		tk_wup_tsk( p_buff->wait_tskid);
		p_buff->wait_tskid = 0;
	}

}

EXPORT void dev_ser_notify_err(UW unit, UW err)
{
	get_dcb_mem(unit, com_error) |= err;	
}

/*----------------------------------------------------------------------*/
/* Attribute data control
 */

LOCAL ER set_atr_size(T_DEVREQ *req)
{
	switch(req->start) {
	case TDN_EVENT:			/* MBF ID for event notification */
		req->asize = sizeof(ID);
		break;
	case TDN_SER_MODE:		/* Communication mode */
	case TDN_SER_SPEED:		/* Communication speed */
	case TDN_SER_COMERR:		/* Communication Error */
		req->asize = sizeof(UW);
		break;
	case TDN_SER_SNDTMO:		/* Send timeout */
	case TDN_SER_RCVTMO:		/* Receive timeout */
		req->asize = sizeof(TMO);
		break;
	default:
		return E_PAR;
	}
	return E_OK;
}

LOCAL ER read_atr(T_SER_DCB *p_dcb, T_DEVREQ *req)
{
	UINT	imask;
	ER	err;

	err = set_atr_size(req);
	if(err != E_OK) return err;

	if(req->size) {
		switch(req->start) {
		case TDN_EVENT:			/* MBF ID for event notification */
			*(ID*)req->buf = p_dcb->evtmbfid;
			break;
		case TDN_SER_MODE:		/* Communication mode */
			*(UW*)req->buf = p_dcb->com_mode;
			break;
		case TDN_SER_SPEED:		/* Communication speed */
			*(UW*)req->buf = p_dcb->com_speed;
			break;
		case TDN_SER_SNDTMO:		/* Send timeout */
			*(TMO*)req->buf = p_dcb->snd_tmo;
			break;
		case TDN_SER_RCVTMO:		/* Receive timeout */
			*(TMO*)req->buf = p_dcb->rcv_tmo;
			break;
		case TDN_SER_COMERR:		/* Communication Error */
			DI(imask);
			*(UW*)req->buf = p_dcb->com_error;
			p_dcb->com_error = 0;
			EI(imask);
			break;
		case TDN_SER_BREAK:		/* Send Break */
			/* no break */
		default:
			err = E_PAR;
		}
	}
	return err;
}

LOCAL ER write_atr(T_SER_DCB *p_dcb, T_DEVREQ *req)
{
	ER	err;

	err = set_atr_size(req);
	if(err != E_OK) return err;

	if(req->size) {
		switch(req->start) {
		case TDN_EVENT:			/* Message buffer ID for event notification */
			p_dcb->evtmbfid = *(ID*)req->buf;
			break;
		case TDN_SER_MODE:		/* Communication mode */
			p_dcb->com_mode = *(UW*)req->buf;
			err = dev_ser_llctl( p_dcb->unit, LLD_SER_MODE, p_dcb->com_mode);
			break;
		case TDN_SER_SPEED:		/* Communication speed */
			p_dcb->com_speed = *(UW*)req->buf;
			err = dev_ser_llctl( p_dcb->unit, LLD_SER_SPEED, p_dcb->com_speed);
			break;
		case TDN_SER_SNDTMO:		/* Send timeout */
			p_dcb->snd_tmo = *(TMO*)req->buf;
			break;
		case TDN_SER_RCVTMO:		/* Receive timeout */
			p_dcb->rcv_tmo = *(TMO*)req->buf;
			break;
		case TDN_SER_BREAK:		/* Send Break */
			err = dev_ser_llctl( p_dcb->unit, LLD_SER_BREAK, *(UW*)req->buf);
			break;
		default:
			err = E_PAR;
		}
	}
	return err;
}

/*----------------------------------------------------------------------*/
/*　Device-specific data control
 */

LOCAL ER read_data( T_SER_DCB *p_dcb, T_DEVREQ *req)
{
	T_SER_BUFF	*p_buff;
	UW		tail;
	UB		*pd;
	SZ		rsize;
	ER		err;

	p_buff	= &p_dcb->rcv_buff;
	tail	= p_buff->tail;
	pd	= req->buf;
	rsize	= req->size;
	err	= E_OK;

	if(rsize != 0 ) {		/* size != 0 : Receive data */
		while(rsize) {
			DisableInt(p_dcb->intno_rcv);	/* Disable Receive Int. */
			if(p_buff->top != tail) {
				*pd++ = p_buff->data[tail++];
				if(tail >= DEVCONF_SER_BUFFSIZE ) tail = 0;
				p_buff->tail = tail;
				EnableInt(p_dcb->intno_rcv, p_dcb->int_pri);	/* Enable Receive Int. */
				rsize--;
			} else {
				p_buff->wait_tskid = tk_get_tid();
				EnableInt(p_dcb->intno_rcv, p_dcb->int_pri);	/* Enable Receive Int. */
				err = tk_slp_tsk(p_dcb->rcv_tmo);
				if( err != E_OK) break;
			}
		}
		req->asize = req->size - rsize;
	} else {		/* size = 0: Get the number of readable data */
		/* Disable Receive Int. */
		rsize = p_buff->top - p_buff->tail;
		/* Enable Receive Int. */
		if(rsize < 0) {
			rsize += DEVCONF_SER_BUFFSIZE;
		}
		req->asize = rsize;
	}

	return err;
}

LOCAL ER write_data( T_SER_DCB *p_dcb, T_DEVREQ *req)
{
	T_SER_BUFF	*p_buff;
	UW		next;
	UB		*pd;
	INT		ssize;
	ER		err;
	BOOL		s;

	p_buff	= &p_dcb->snd_buff;
	pd	= req->buf;
	ssize	= req->size;
	err	= E_OK;

	if(ssize != 0) {		/* size != 0 : Send data */
		while(ssize) {
			next = p_buff->top + 1;
			if(next >= DEVCONF_SER_BUFFSIZE) next = 0;
			
			DisableInt(p_dcb->intno_snd);	/* Disable Send Int. */
			if(next != p_buff->tail) {
				s = FALSE;
				if(p_buff->top == p_buff->tail) {
					s = (E_OK == dev_ser_llctl( p_dcb->unit, LLD_SER_SEND, *pd));
				}
				if(s) {		/* Successful transmission. */
					pd++;
				} else {	/* FIFO is full. */
					p_buff->data[p_buff->top] = *pd++;
					p_buff->top = next;
				}
				EnableInt(p_dcb->intno_snd, p_dcb->int_pri);	/* Enable Send Int. */
				ssize--;
			} else {
				p_buff->wait_tskid = tk_get_tid();
				EnableInt(p_dcb->intno_snd, p_dcb->int_pri);	/* Enable Send Int. */
				err = tk_slp_tsk(p_dcb->snd_tmo);
				if(err != E_OK) break;
			}
		}
		req->asize = req->size - ssize;
	} else {		/* size = 0: Get the number of writable data */
		DisableInt(p_dcb->intno_snd);			/* Disable Send Int. */
		ssize = p_buff->top - p_buff->tail;
		EnableInt(p_dcb->intno_snd, p_dcb->int_pri);	/* Enable send Int. */
		if(ssize < 0) {
			ssize += DEVCONF_SER_BUFFSIZE;
		}
		req->asize = DEVCONF_SER_BUFFSIZE - ssize;
	}

	return err;
}

/*----------------------------------------------------------------------
 * mSDI I/F function
 */
/*----------------------------------------------------------------------
 * Open device
 */
ER dev_ser_openfn( ID devid, UINT omode, T_MSDI *p_msdi)
{
	T_SER_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_SER_DCB*)p_msdi->dmsdi.exinf;

	/* Device Open operation */
	p_dcb->omode = omode;
	buff_init(&p_dcb->snd_buff);
	buff_init(&p_dcb->rcv_buff);

	err = dev_ser_llctl(p_dcb->unit, LLD_SER_START, 0);

	return err;
}

/*----------------------------------------------------------------------
 * Close Device
 */
ER dev_ser_closefn( ID devid, UINT option, T_MSDI *p_msdi)
{
	T_SER_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_SER_DCB*)(p_msdi->dmsdi.exinf);

	/* Device Close operation */
	err = dev_ser_llctl(p_dcb->unit, LLD_SER_STOP, 0);
	
	return err;
}

/*----------------------------------------------------------------------
 * Read Device
 */
ER dev_ser_readfn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_SER_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_SER_DCB*)(p_msdi->dmsdi.exinf);

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
ER dev_ser_writefn( T_DEVREQ *req, T_MSDI *p_msdi)
{
	T_SER_DCB	*p_dcb;
	ER		err;

	p_dcb = (T_SER_DCB*)(p_msdi->dmsdi.exinf);

	if(req->start >= 0) {	// Device specific data
		if( p_dcb->omode & TD_WRITE) {
			err = write_data( p_dcb, req);
		} else {
			err = E_OACV;
		}
	} else {		// Device attribute data
		err = write_atr( p_dcb, req);
	}

	return err;
}

/*----------------------------------------------------------------------
 * Event Device
 */
ER dev_ser_eventfn( INT evttyp, void *evtinf, T_MSDI *p_msdi)
{
	return E_NOSPT;
}

/*----------------------------------------------------------------------
 * Serial communication Device initialization and registration
 */
EXPORT ER dev_init_ser( UW unit )
{
	T_SER_DCB	*p_dcb;
	T_IDEV		idev;
	T_MSDI		*p_msdi;
	T_DMSDI		dmsdi;
	ER		err;
	INT		i;

	if( unit >= DEV_SER_UNITNM) return E_PAR;

#if TK_SUPPORT_MEMLIB
	p_dcb = (T_SER_DCB*)Kmalloc(sizeof(T_SER_DCB));
	if( p_dcb == NULL) return E_NOMEM;
	dev_ser_cb[unit]	= p_dcb;
#else
	p_dcb = &dev_ser_cb[unit];
#endif

	/* Device registration information */
	dmsdi.exinf	= p_dcb;
	dmsdi.drvatr	= 0;			/* Driver attributes */
	dmsdi.devatr	= TDK_UNDEF;		/* Device attributes */
	dmsdi.nsub	= 0;			/* Number of subunits */
	dmsdi.blksz	= 1;			/* Unique data block size (-1 = unknown) */
	dmsdi.openfn	= dev_ser_openfn;
	dmsdi.closefn	= dev_ser_closefn;
	dmsdi.readfn	= dev_ser_readfn;
	dmsdi.writefn	= dev_ser_writefn;
	dmsdi.eventfn	= dev_ser_eventfn;
	
	knl_strcpy( (char*)dmsdi.devnm, DEVCNF_SER_DEVNAME);
	i = knl_strlen(DEVCNF_SER_DEVNAME);
	dmsdi.devnm[i] = (UB)('a' + unit);
	dmsdi.devnm[i+1] = 0;

	err = msdi_def_dev( &dmsdi, &idev, &p_msdi);
	if(err != E_OK) goto err_2;

	p_dcb->unit	= unit;
	p_dcb->evtmbfid	= idev.evtmbfid;

	/* Low-level device initialization */
	err = dev_ser_llinit( p_dcb);
	if(err != E_OK) goto err_1;

	/* Set default value for attribute data */
	p_dcb->com_speed = DEVCNF_SER_SPEED;
	dev_ser_llctl( unit, LLD_SER_SPEED, DEVCNF_SER_SPEED);
	p_dcb->com_mode = DEVCNF_SER_MODE;
	dev_ser_llctl( unit, LLD_SER_MODE, DEVCNF_SER_MODE);
	p_dcb->snd_tmo = DEVCNF_SER_SND_TMO;
	p_dcb->rcv_tmo = DEVCNF_SER_RCV_TMO;

	return E_OK;

err_1:
	msdi_del_dev(p_msdi);
err_2:
	Kfree(p_dcb);
	return err;
}

#endif		/* DEV_SER_ENABLE */