/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */
/*
 *	offset.h (RXv2)
 *	TCB Offset Definition
 */

#ifndef _OFFSET_
#define _OFFSET_

/* Adjust offset of TCB member variables in offset.h for dispatch.S */

/*	TCB.wrdvno	*/
#if CFN_MAX_PORID > 0
#define TCBSZ_POR	(4)	/* = (mtxlist-wrdvno) */
#else
#define TCBSZ_POR	(0)
#endif

/*	TCB.mtxlist	*/
#if CFN_MAX_MTXID > 0
#define TCBSZ_MTX	(4)	/* = sizeof(MTXCB*) */
#else
#define TCBSZ_MTX	(0)
#endif

/*	TCB.winfo.xxx	*/
#if CFN_MAX_PORID > 0
#define TCBSZ_WINFO	(16)
#else
#if CFN_MAX_FLGID > 0
#define TCBSZ_WINFO	(12)
#else
#if CFN_MAX_MBFID > 0 || CFN_MAX_MPLID > 0
#define TCBSZ_WINFO	(8)
#else
#if CFN_MAX_SEMID > 0 || CFN_MAX_MBXID > 0 || CFN_MAX_MPFID > 0
#define TCBSZ_WINFO	(4)
#else
#define TCBSZ_WINFO	(0)
#endif
#endif
#endif
#endif

/*	TCB.stime, TCB.utime */
#if USE_DBGSPT && defined(USE_FUNC_TD_INF_TSK)
#define TCBSZ_EXECTIME	(8)
#else
#define TCBSZ_EXECTIME	(0)
#endif

#define _ALIGN_CPU(x)	(((x)+3)&0xFFFFFFFC)	/* RX63N : 32 bit align	*/

/*----------------------------------------------------------------------*/
/*	offset data in TCB						*/
/*----------------------------------------------------------------------*/

#define TCB_tskid	8
#define TCB_tskatr	16	
#define TCB_tskctxb	24
#define TCB_state	39

#define CTXB_ssp	0

#endif /* _OFFSET_ */
