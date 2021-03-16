/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03.B0
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03.
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
#define	TM_COM_SERIAL_DEV
//#define	TM_COM_NO_DEV

/* tm_printf() call */
#define	USE_TM_PRINTF		(1)	/* Use tm_printf() & tm_sprintf() calls */
#define	TM_OUTBUF_SZ		(0)	/* Output Buffer size in stack */

#endif /* __TM_CONF_H__ */
