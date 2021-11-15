/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	offset.h (ARMv7-A)
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
#define	TCB_isstack	100

#define CTXB_ssp	0


/*----------------------------------------------------------------------*/
/*	The size of the FPU context on the stack			*/
/*----------------------------------------------------------------------*/
#define	FPU_context	264

#endif /* _OFFSET_ */
