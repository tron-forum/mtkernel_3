/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03.
 *
 *----------------------------------------------------------------------
 */

/*
 *	tm_conf.h
 *	T-Monitor Configuration Definition
 */

#ifndef __TM_CONF_H__
#define __TM_CONF_H__

/* Select a communication port */
#ifdef _IOTE_M367_
#define USE_COM_IOTE_M367
#endif

#ifdef _IOTE_RX231_
#define USE_COM_IOTE_RX231
#endif


/* tm_printf() call */
#define	USE_TM_PRINTF		(1)	/* Use tm_printf() & tm_sprintf() calls */
#define	TM_OUTBUF_SZ		(0)	/* Output Buffer size in stack */

#endif /* __TM_CONF_H__ */
