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
 *	dbgspt.h
 *
 *	micro T-Kernel Debugger Support
 */

#ifndef __TK_DBGSPT_H__
#define __TK_DBGSPT_H__

#include "tkernel.h"

#ifdef __cplusplus
extern "C" {
#endif

/* System dependencies */
#define DBGSPT_PATH_(a)		#a
#define DBGSPT_PATH(a)		DBGSPT_PATH_(a)
#define DBGSPT_SYSDEP()		DBGSPT_PATH(sysdepend/TARGET_DIR/dbgspt.h)
#include DBGSPT_SYSDEP()

/*
 * Object name information		td_ref_dsname, td_set_dsname
 */
#define TN_TSK 0x01
#define TN_SEM 0x02
#define TN_FLG 0x03
#define TN_MBX 0x04
#define TN_MBF 0x05
#define TN_POR 0x06
#define TN_MTX 0x07
#define TN_MPL 0x08
#define TN_MPF 0x09
#define TN_CYC 0x0a
#define TN_ALM 0x0b

/*
 * Semaphore state information		td_ref_sem
 */
typedef	struct td_rsem {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	INT	semcnt;		/* Current semaphore value */
} TD_RSEM;

/*
 * Event flag state information		td_ref_flg
 */
typedef	struct td_rflg {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	UINT	flgptn;		/* Current event flag pattern */
} TD_RFLG;

/*
 * Mail box state information		td_ref_mbx
 */
typedef	struct td_rmbx {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	T_MSG	*pk_msg;	/* Next received message */
} TD_RMBX;

/*
 * Mutex state information		td_ref_mtx
 */
typedef struct td_rmtx {
	void	*exinf;		/* Extended information */
	ID	htsk;		/* Locking task ID */
	ID	wtsk;		/* Lock wait task ID */
} TD_RMTX;

/*
 * Message buffer state information 	td_ref_mbf
 */
typedef struct td_rmbf {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Receive wait task ID */
	ID	stsk;		/* Send wait task ID */
	INT	msgsz;		/* Next received message size (byte) */
	W	frbufsz;	/* Free buffer size (byte) */
	INT	maxmsz;		/* Maximum length of message (byte) */
} TD_RMBF;

/*
 * Rendezvous port state information	td_ref_por
 */
typedef struct td_rpor {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Call wait task ID */
	ID	atsk;		/* Receive wait task ID */
	INT	maxcmsz;	/* Maximum length of call message (byte) */
	INT	maxrmsz;	/* Maximum length of replay message (byte) */
} TD_RPOR;

/*
 * Fixed size memory pool state information	td_ref_mpf
 */
typedef struct td_rmpf {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	W	frbcnt;		/* Number of free blocks */
} TD_RMPF;

/*
 * Variable size memory pool state information	td_ref_mpl
 */
typedef struct td_rmpl {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	W	frsz;		/* Total size of free area (byte) */
	W	maxsz;		/* Size of maximum continuous
				   free area (byte) */
} TD_RMPL;

/*
 * Cycle handler state information	td_ref_cyc
 */
typedef struct td_rcyc {
	void	*exinf;		/* Extended information */
	RELTIM	lfttim;		/* Remaining time until next handler startup */
	UINT	cycstat;	/* Cycle handler status */
} TD_RCYC;

/*
 * Alarm handler state information	td_ref_alm
 */
typedef struct td_ralm {
	void	*exinf;		/* Extended information */
	RELTIM	lfttim;		/* Remaining time until handler startup */
	UINT	almstat;	/* Alarm handler status */
} TD_RALM;

/*
 * Subsystem state information		td_ref_ssy
 */
typedef struct td_rssy {
	PRI	ssypri;		/* Subsystem priority */
	W	resblksz;	/* Resource management block size (byte) */
} TD_RSSY;

/*
 * Task state information		td_ref_tsk
 */
typedef	struct td_rtsk {
	void	*exinf;		/* Extended information */
	PRI	tskpri;		/* Current priority */
	PRI	tskbpri;	/* Base priority */
	UINT	tskstat;	/* Task state */
	UW	tskwait;	/* Wait factor */
	ID	wid;		/* Wait object ID */
	INT	wupcnt;		/* Number of wakeup requests queuing */
	INT	suscnt;		/* Number of SUSPEND request nests */
	FP	task;		/* Task startup address */
	W	stksz;		/* stack size (byte) */
	void	*istack;		/* stack pointer initial value */
} TD_RTSK;

/*
 * Task statistics information		td_inf_tsk
 */
typedef struct td_itsk {
	RELTIM	stime;		/* Cumulative system execution time
				   (milliseconds) */
	RELTIM	utime;		/* Cumulative user execution time
				   (milliseconds) */
} TD_ITSK;

/*
 * System state information		td_ref_sys
 */
typedef struct td_rsys {
	UINT	sysstat;	/* System state */
	ID	runtskid;	/* ID of task in execution state */
	ID	schedtskid;	/* ID of task that should be in
				   execution state */
} TD_RSYS;

/*
 * System call/extended SVC trace definition 	td_hok_svc
 */
typedef struct td_hsvc {
	FP	enter;		/* Hook routine before calling */
	FP	leave;		/* Hook routine after calling */
} TD_HSVC;

/*
 * Task dispatch trace definition		td_hok_dsp
 */
typedef struct td_hdsp {
	FP	exec;		/* Hook routine when starting execution */
	FP	stop;		/* Hook routine when stopping execution */
} TD_HDSP;

/*
 * Exception/Interrupt trace definition			td_hok_int
 */
typedef struct td_hint {
	FP	enter;		/* Hook routine before calling handler */
	FP	leave;		/* Hook routine after calling handler */
} TD_HINT;

/* ------------------------------------------------------------------------ */

/*
 * Definition for interface library automatic generation (mktdsvc)
 */
/*** DEFINE_TDSVC ***/

/* [BEGIN SYSCALLS] */

/* Refer each object usage state */
IMPORT INT td_lst_tsk( ID list[], INT nent );
IMPORT INT td_lst_sem( ID list[], INT nent );
IMPORT INT td_lst_flg( ID list[], INT nent );
IMPORT INT td_lst_mbx( ID list[], INT nent );
IMPORT INT td_lst_mtx( ID list[], INT nent );
IMPORT INT td_lst_mbf( ID list[], INT nent );
IMPORT INT td_lst_por( ID list[], INT nent );
IMPORT INT td_lst_mpf( ID list[], INT nent );
IMPORT INT td_lst_mpl( ID list[], INT nent );
IMPORT INT td_lst_cyc( ID list[], INT nent );
IMPORT INT td_lst_alm( ID list[], INT nent );
IMPORT INT td_lst_ssy( ID list[], INT nent );

/* Refer each object state */
IMPORT ER td_ref_sem( ID semid, TD_RSEM *rsem );
IMPORT ER td_ref_flg( ID flgid, TD_RFLG *rflg );
IMPORT ER td_ref_mbx( ID mbxid, TD_RMBX *rmbx );
IMPORT ER td_ref_mtx( ID mtxid, TD_RMTX *rmtx );
IMPORT ER td_ref_mbf( ID mbfid, TD_RMBF *rmbf );
IMPORT ER td_ref_por( ID porid, TD_RPOR *rpor );
IMPORT ER td_ref_mpf( ID mpfid, TD_RMPF *rmpf );
IMPORT ER td_ref_mpl( ID mplid, TD_RMPL *rmpl );
IMPORT ER td_ref_cyc( ID cycid, TD_RCYC *rcyc );
IMPORT ER td_ref_alm( ID almid, TD_RALM *ralm );
IMPORT ER td_ref_ssy( ID ssid, TD_RSSY *rssy );

/* Refer task state */
IMPORT ER td_ref_tsk( ID tskid, TD_RTSK *rtsk );
IMPORT ER td_inf_tsk( ID tskid, TD_ITSK *itsk, BOOL clr );

#if TK_SUPPORT_REGOPS
IMPORT ER td_get_reg( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs );
IMPORT ER td_set_reg( ID tskid, CONST T_REGS *regs, CONST T_EIT *eit, CONST T_CREGS *cregs );
#endif  /* TK_SUPPORT_REGOPS */

/* Refer system state */
IMPORT ER td_ref_sys( TD_RSYS *rsys );
IMPORT ER td_get_tim( SYSTIM *tim, UW *ofs );
IMPORT ER td_get_otm( SYSTIM *tim, UW *ofs );

/* Refer ready queue */
IMPORT INT td_rdy_que( PRI pri, ID list[], INT nent );

/* Refer wait queue */
IMPORT INT td_sem_que( ID semid, ID list[], INT nent );
IMPORT INT td_flg_que( ID flgid, ID list[], INT nent );
IMPORT INT td_mbx_que( ID mbxid, ID list[], INT nent );
IMPORT INT td_mtx_que( ID mtxid, ID list[], INT nent );
IMPORT INT td_smbf_que( ID mbfid, ID list[], INT nent );
IMPORT INT td_rmbf_que( ID mbfid, ID list[], INT nent );
IMPORT INT td_cal_que( ID porid, ID list[], INT nent );
IMPORT INT td_acp_que( ID porid, ID list[], INT nent );
IMPORT INT td_mpf_que( ID mpfid, ID list[], INT nent );
IMPORT INT td_mpl_que( ID mplid, ID list[], INT nent );

/* Execution trace */
IMPORT ER td_hok_svc( CONST TD_HSVC *hsvc );
IMPORT ER td_hok_dsp( CONST TD_HDSP *hdsp );
IMPORT ER td_hok_int( CONST TD_HINT *hint );

/* Object name */
IMPORT ER td_ref_dsname( UINT type, ID id, UB *dsname );
IMPORT ER td_set_dsname( UINT type, ID id, CONST UB *dsname );

/* [END SYSCALLS] */

#ifdef __cplusplus
}
#endif
#endif /* __TK_DBGSPT_H__ */
