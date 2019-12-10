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
 *	offset.h (ARM Cortex-M3)
 *	TCB Offset Definition
 */

#ifndef _OFFSET_
#define _OFFSET_

/* Adjust offset of TCB member variables in offset.h for cpu_support.S */

/*	TCB.wrdvno	*/
#if MAX_PORID > 0
#define TCBSZ_POR	(4)	/* = sizeof(RNO) */
#else
#define TCBSZ_POR	(0)
#endif

/*	TCB.mtxlist	*/
#if MAX_MTXID > 0
#define TCBSZ_MTX	(4)	/* = sizeof(MTXCB*) */
#else
#define TCBSZ_MTX	(0)
#endif

/*	TCB.winfo.xxx	*/
#if MAX_PORID > 0
#define TCBSZ_WINFO	(16)
#else
#if MAX_FLGID > 0
#define TCBSZ_WINFO	(12)
#else
#if MAX_MBFID > 0 || MAX_MPLID > 0
#define TCBSZ_WINFO	(8)
#else
#if MAX_SEMID > 0 || MAX_MBXID > 0 || MAX_MPFID > 0
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

#define _ALIGN_CPU(x)	(((x)+3)&0xFFFFFFFC)	/* Cortex-M3 : 32 bit CPU	*/
#define _ALIGN_64(x)	(((x)+7)&0xFFFFFFF8)	/* Struct use 64bit align */


/*----------------------------------------------------------------------*/
/*	machine dependent data						*/
/*----------------------------------------------------------------------*/
#define	TCB_winfo	(60)		/* tskque - wercd		*/
#define	TCB_wtmeb	(TCB_winfo+TCBSZ_WINFO)
#define	TCBsz_wtmeb2isstack	(20+TCBSZ_MTX+TCBSZ_POR+TCBSZ_EXECTIME)
					/* wtmeb - istack		*/
#define TCBSZ_GP	(0)		/* No global pointer support	*/

/*----------------------------------------------------------------------*/
/*	offset data in TCB						*/
/*----------------------------------------------------------------------*/
#define TCB_isstack	(TCB_wtmeb+TCBsz_wtmeb2isstack)
#define TCB_tskctxb	(TCB_isstack+4+TCBSZ_GP)

#define TCB_tskid	8
#define TCB_tskatr	16
#define TCB_state	35
#define CTXB_ssp	0

#endif /* _OFFSET_ */
