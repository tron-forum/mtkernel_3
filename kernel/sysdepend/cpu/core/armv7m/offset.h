/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */

/*
 *	offset.h (ARMv7-M)
 *	TCB Offset Definition
 */

#ifndef _OFFSET_
#define _OFFSET_

/*----------------------------------------------------------------------*/
/*	offset data in TCB						*/
/*----------------------------------------------------------------------*/

#define TCB_tskid	8
#define TCB_tskatr	16
#define TCB_tskctxb	24
#define TCB_state	39

#define CTXB_ssp	0

#endif /* _OFFSET_ */
