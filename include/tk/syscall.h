/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	syscall.h
 *
 *	micro T-Kernel System Calls
 */

#ifndef __TK_SYSCALL_H__
#define __TK_SYSCALL_H__

/* Task creation */
#define TSK_SELF	0		/* Its own task specify */
#define TPRI_INI	0		/* Specify priority at task startup */
#define TPRI_RUN	0		/* Specify highest priority during running */

#define TA_ASM		0x00000000UL	/* Program by assembler */
#define TA_HLNG		0x00000001UL	/* Program by high level programming language */
#define TA_USERBUF	0x00000020UL	/* Specify user buffer */
#define TA_DSNAME	0x00000040UL	/* Use object name */

#define TA_RNG0		0x00000000UL	/* Execute by protection level 0 */
#define TA_RNG1		0x00000100UL	/* Execute by protection level 1 */
#define TA_RNG2		0x00000200UL	/* Execute by protection level 2 */
#define TA_RNG3		0x00000300UL	/* Execute by protection level 3 */

#define TA_COP0		0x00001000U	/* Use coprocessor (ID=0) */
#define TA_COP1		0x00002000U	/* Use coprocessor (ID=1) */
#define TA_COP2		0x00004000U	/* Use coprocessor (ID=2) */
#define TA_COP3		0x00008000U	/* Use coprocessor (ID=3) */

/* Task state tskstat */
#define TTS_RUN		0x00000001U	/* RUN */
#define TTS_RDY		0x00000002U	/* READY */
#define TTS_WAI		0x00000004U	/* WAIT */
#define TTS_SUS		0x00000008U	/* SUSPEND */
#define TTS_WAS		0x0000000cU	/* WAIT-SUSPEND */
#define TTS_DMT		0x00000010U	/* DORMANT */
#define TTS_NODISWAI	0x00000080U	/* Wait disable rejection state */

/* Wait factor tskwait */
#define TTW_SLP		0x00000001UL	/* Wait caused by wakeup wait */
#define TTW_DLY		0x00000002UL	/* Wait caused by task delay */
#define TTW_SEM		0x00000004UL	/* Semaphore wait */
#define TTW_FLG		0x00000008UL	/* Event flag wait */
#define TTW_MBX		0x00000040UL	/* Mail box wait */
#define TTW_MTX		0x00000080UL	/* Mutex wait */
#define TTW_SMBF	0x00000100UL	/* Message buffer send wait */
#define TTW_RMBF	0x00000200UL	/* Message buffer receive wait */
#define TTW_CAL		0x00000400UL	/* Rendezvous call wait */
#define TTW_ACP		0x00000800UL	/* Rendezvous accept wait */
#define TTW_RDV		0x00001000UL	/* Rendezvous end wait */
#define TTW_MPF		0x00002000UL	/* Fixed size memory pool wait */
#define TTW_MPL		0x00004000UL	/* Variable size memory pool wait */

/* Semaphore generation */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_FIRST	0x00000000UL	/* Give priority to task at head of wait queue */
#define TA_CNT		0x00000002UL	/* Give priority to task whose request counts is less */
#define TA_DSNAME	0x00000040UL	/* Use object name */

/* Mutex */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_INHERIT	0x00000002UL	/* Priority inherited protocol */
#define TA_CEILING	0x00000003UL	/* Upper limit priority protocol */
#define TA_DSNAME	0x00000040UL	/* Use object name */

/* Event flag */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_WSGL		0x00000000UL	/* Disable multiple tasks wait */
#define TA_WMUL		0x00000008UL	/* Enable multiple tasks wait */
#define TA_DSNAME	0x00000040UL	/* Use object name */

/* Event flag wait mode */
#define TWF_ANDW	0x00000000U	/* AND wait */
#define TWF_ORW		0x00000001U	/* OR wait */
#define TWF_CLR		0x00000010U	/* All clear specify */
#define TWF_BITCLR	0x00000020U	/* Only condition bit clear specify */

/* Mail box */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_MFIFO	0x00000000UL	/* Manage messages by FIFO */
#define TA_MPRI		0x00000002UL	/* Manage messages by priority order */
#define TA_DSNAME	0x00000040UL	/* Use object name */

/* Message buffer */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_USERBUF	0x00000020UL	/* Specify user buffer */
#define TA_DSNAME	0x00000040UL	/* Use object name */

/* Rendezvous */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_DSNAME	0x00000040UL	/* Use object name */

/* Handler */
#define TA_ASM		0x00000000UL	/* Program by assembler */
#define TA_HLNG		0x00000001UL	/* Program by high level programming language */

/* Variable size memory pool */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_USERBUF	0x00000020UL	/* Specify user buffer */
#define TA_DSNAME	0x00000040UL	/* Use object name */
#define TA_RNG0		0x00000000UL	/* Protection level 0 */
#define TA_RNG1		0x00000100UL	/* Protection level 1 */
#define TA_RNG2		0x00000200UL	/* Protection level 2 */
#define TA_RNG3		0x00000300UL	/* Protection level 3 */

/* Fixed size memory pool */
#define TA_TFIFO	0x00000000UL	/* Manage wait task by FIFO */
#define TA_TPRI		0x00000001UL	/* Manage wait task by priority order */
#define TA_USERBUF	0x00000020UL	/* Specify user buffer */
#define TA_DSNAME	0x00000040UL	/* Use object name */
#define TA_RNG0		0x00000000UL	/* Protection level 0 */
#define TA_RNG1		0x00000100UL	/* Protection level 1 */
#define TA_RNG2		0x00000200UL	/* Protection level 2 */
#define TA_RNG3		0x00000300UL	/* Protection level 3 */

/* Cycle handler */
#define TA_ASM		0x00000000UL	/* Program by assembler */
#define TA_HLNG		0x00000001UL	/* Program by high level programming language */
#define TA_STA		0x00000002UL	/* Cycle handler startup */
#define TA_PHS		0x00000004UL	/* Save cycle handler phase */
#define TA_DSNAME	0x00000040UL	/* Use object name */

#define TCYC_STP	0x00U		/* Cycle handler is not operating */
#define TCYC_STA	0x01U		/* Cycle handler is operating */

/* Alarm handler address */
#define TA_ASM		0x00000000UL	/* Program by assembler */
#define TA_HLNG		0x00000001UL	/* Program by high level programming language */
#define TA_DSNAME	0x00000040UL	/* Use object name */

#define TALM_STP	0x00U		/* Alarm handler is not operating */
#define TALM_STA	0x01U		/* Alarm handler is operating */

/* System state */
#define TSS_TSK		0x00U	/* During execution of task part(context) */
#define TSS_DDSP	0x01U	/* During dispatch disable */
#define TSS_DINT	0x02U	/* During Interrupt disable */
#define TSS_INDP	0x04U	/* During execution of task independent part */
#define TSS_QTSK	0x08U	/* During execution of semi-task part */

/* Power-saving mode */
#define TPW_DOSUSPEND	1	/* Transit to suspend state */
#define TPW_DISLOWPOW	2	/* Power-saving mode switch disable */
#define TPW_ENALOWPOW	3	/* Power-saving mode switch enable */

/*
 * Task creation information 		tk_cre_tsk
 */
typedef struct t_ctsk {
	void	*exinf;		/* Extended information */
	ATR	tskatr;		/* Task attribute */
	FP	task;		/* Task startup address */
	PRI	itskpri;	/* Priority at task startup */
	SZ	stksz;		/* User stack size (byte) */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
	void	*bufptr;	/* User buffer */
} T_CTSK;

/*
 * Task state information 		tk_ref_tsk
 */
typedef	struct t_rtsk {
	void	*exinf;		/* Extended information */
	PRI	tskpri;		/* Current priority */
	PRI	tskbpri;	/* Base priority */
	UINT	tskstat;	/* Task state */
	UW	tskwait;	/* Wait factor */
	ID	wid;		/* Wait object ID */
	INT	wupcnt;		/* Number of wakeup requests queuing */
	INT	suscnt;		/* Number of SUSPEND request nests */
} T_RTSK;

/*
 * Semaphore creation information		tk_cre_sem
 */
typedef	struct t_csem {
	void	*exinf;		/* Extended information */
	ATR	sematr;		/* Semaphore attribute */
	INT	isemcnt;	/* Semaphore initial count value */
	INT	maxsem;		/* Semaphore maximum count value */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
} T_CSEM;

/*
 * Semaphore state information		tk_ref_sem
 */
typedef	struct t_rsem {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	INT	semcnt;		/* Current semaphore value */
} T_RSEM;

/*
 * Mutex creation information		tk_cre_mtx
 */
typedef struct t_cmtx {
	void	*exinf;		/* Extended information */
	ATR	mtxatr;		/* Mutex attribute */
	PRI	ceilpri;	/* Upper limit priority of mutex */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
} T_CMTX;

/*
 * Mutex state information		tk_ref_mtx
 */
typedef struct t_rmtx {
	void	*exinf;		/* Extended information */
	ID	htsk;		/* Locking task ID */
	ID	wtsk;		/* Lock wait task ID */
} T_RMTX;

/*
 * Event flag creation information	tk_cre_flg
 */
typedef	struct t_cflg {
	void	*exinf;		/* Extended information */
	ATR	flgatr;		/* Event flag attribute */
	UINT	iflgptn;	/* Event flag initial value */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
} T_CFLG;

/*
 * Event flag state information		tk_ref_flg
 */
typedef	struct t_rflg {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	UINT	flgptn;		/* Current event flag pattern */
} T_RFLG;

/*
 * Mail box creation information	tk_cre_mbx
 */
typedef	struct t_cmbx {
	void	*exinf;		/* Extended information */
	ATR	mbxatr;		/* Mail box attribute */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
} T_CMBX;

/*
 * Mail box message header
 */
typedef struct t_msg {
	void	*msgque[1];	/* Area for message queue */
} T_MSG;

typedef struct t_msg_pri {
	T_MSG	msgque;		/* Area for message queue */
	PRI	msgpri;		/* Message priority */
} T_MSG_PRI;

/*
 * Mail box state information		tk_ref_mbx
 */
typedef	struct t_rmbx {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	T_MSG	*pk_msg;	/* Next received message */
} T_RMBX;

/*
 * Message buffer creation information	tk_cre_mbf
 */
typedef	struct t_cmbf {
	void	*exinf;		/* Extended information */
	ATR	mbfatr;		/* Message buffer attribute */
	SZ	bufsz;		/* Message buffer size (byte) */
	INT	maxmsz;		/* Maximum length of message (byte) */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
	void	*bufptr;		/* User buffer */
} T_CMBF;

/*
 * Message buffer state information 	tk_ref_mbf
 */
typedef struct t_rmbf {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Receive wait task ID */
	ID	stsk;		/* Send wait task ID */
	INT	msgsz;		/* Next received message size (byte) */
	SZ	frbufsz;	/* Free buffer size (byte) */
	INT	maxmsz;		/* Maximum length of message (byte) */
} T_RMBF;

/*
 * Rendezvous port creation information	tk_cre_por
 */
typedef	struct t_cpor {
	void	*exinf;		/* Extended information */
	ATR	poratr;		/* Port attribute */
	INT	maxcmsz;	/* Maximum length of call message (byte) */
	INT	maxrmsz;	/* Maximum length of replay message (byte) */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
} T_CPOR;

/*
 * Rendezvous port state information	tk_ref_por
 */
typedef struct t_rpor {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Call wait task ID */
	ID	atsk;		/* Receive wait task ID */
	INT	maxcmsz;	/* Maximum length of call message (byte) */
	INT	maxrmsz;	/* Maximum length of replay message (byte) */
} T_RPOR;

/*
 * Interrupt handler definition information	tk_def_int
 */
typedef struct t_dint {
	ATR	intatr;		/* Interrupt handler attribute */
	FP	inthdr;		/* Interrupt handler address */
} T_DINT;

/*
 * Variable size memory pool creation information	tk_cre_mpl
 */
typedef	struct t_cmpl {
	void	*exinf;		/* Extended information */
	ATR	mplatr;		/* Memory pool attribute */
	SZ	mplsz;		/* Size of whole memory pool (byte) */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
	void	*bufptr;		/* User buffer */
} T_CMPL;

/*
 * Variable size memory pool state information	tk_ref_mpl
 */
typedef struct t_rmpl {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	SZ	frsz;		/* Total size of free area (byte) */
	SZ	maxsz;		/* Size of maximum continuous free area
				   (byte) */
} T_RMPL;

/*
 * Fixed size memory pool state information	tk_cre_mpf
 */
typedef	struct t_cmpf {
	void	*exinf;		/* Extended information */
	ATR	mpfatr;		/* Memory pool attribute */
	SZ	mpfcnt;		/* Number of blocks in whole memory pool */
	SZ	blfsz;		/* Fixed size memory block size (byte) */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
	void	*bufptr;		/* User buffer */
} T_CMPF;

/*
 * Fixed size memory pool state information	tk_ref_mpf
 */
typedef struct t_rmpf {
	void	*exinf;		/* Extended information */
	ID	wtsk;		/* Wait task ID */
	SZ	frbcnt;		/* Number of free area blocks */
} T_RMPF;

/*
 * Cycle handler creation information 	tk_cre_cyc
 */
typedef struct t_ccyc {
	void	*exinf;		/* Extended information */
	ATR	cycatr;		/* Cycle handler attribute */
	FP	cychdr;		/* Cycle handler address */
	RELTIM	cyctim;		/* Cycle interval */
	RELTIM	cycphs;		/* Cycle phase */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
} T_CCYC;

/*
 * Cycle handler state information	tk_ref_cyc
 */
typedef struct t_rcyc {
	void	*exinf;		/* Extended information */
	RELTIM	lfttim;		/* Remaining time until next handler startup */
	UINT	cycstat;	/* Cycle handler status */
} T_RCYC;

/*
 * Alarm handler creation information		tk_cre_alm
 */
typedef struct t_calm {
	void	*exinf;		/* Extended information */
	ATR	almatr;		/* Alarm handler attribute */
	FP	almhdr;		/* Alarm handler address */
#if USE_OBJECT_NAME
	UB	dsname[OBJECT_NAME_LENGTH];	/* Object name */
#endif
} T_CALM;

/*
 * Alarm handler state information	tk_ref_alm
 */
typedef struct t_ralm {
	void	*exinf;		/* Extended information */
	RELTIM	lfttim;		/* Remaining time until handler startup */
	UINT	almstat;	/* Alarm handler state */
} T_RALM;

/*
 * Version information		tk_ref_ver
 */
typedef struct t_rver {
	UH	maker;		/* OS manufacturer */
	UH	prid;		/* OS identification number */
	UH	spver;		/* Specification version */
	UH	prver;		/* OS product version */
	UH	prno[4];	/* Product number, Product management
				   information */
} T_RVER;

/*
 * System state information		tk_ref_sys
 */
typedef struct t_rsys {
	UINT	sysstat;	/* System state */
	ID	runtskid;	/* ID of task in execution state */
	ID	schedtskid;	/* ID of the task that should be the
				   execution state */
} T_RSYS;

/*
 * Subsystem definition information 		tk_def_ssy
 */
typedef struct t_dssy {
	ATR	ssyatr;		/* Subsystem attribute */
	PRI	ssypri;		/* Subsystem priority */
	FP	svchdr;		/* Extended SVC handler address */
	FP	breakfn;	/* Break function address */
	FP	eventfn;	/* Event function address */
} T_DSSY;

/*
 * Subsystem state information		tk_ref_ssy
 */
typedef struct t_rssy {
	PRI	ssypri;		/* Subsystem priority */
} T_RSSY;

/* ------------------------------------------------------------------------ */

/*
 * Device manager
 */

#define L_DEVNM		8	/* Device name length */

/*
 * Device attribute (ATR)
 *
 *	IIII IIII IIII IIII PRxx xxxx KKKK KKKK
 *
 *	The first 16-bit is the device-dependent attribute and
 *	defined by each device.
 *	The last 16-bit is the standard attribute and defined
 *	like as followings.
 */
#define TD_PROTECT	0x8000U		/* P: Write protected */
#define TD_REMOVABLE	0x4000U		/* R: Media remove enabled */

#define TD_DEVKIND	0x00ffU		/* K: Device/media type */
#define TD_DEVTYPE	0x00f0U		/*    Device type */

/* Device type */
#define TDK_UNDEF	0x0000U		/* Undefined/Unknown */
#define TDK_DISK	0x0010U		/* Disk device */

/* Disk type */
#define TDK_DISK_UNDEF	0x0010U		/* Other disks */
#define TDK_DISK_RAM	0x0011U		/* RAM disk (Use main memory) */
#define TDK_DISK_ROM	0x0012U		/* ROM disk (Use main memory) */
#define TDK_DISK_FLA	0x0013U		/* Flash ROM, other silicon disks */
#define TDK_DISK_FD	0x0014U		/* Floppy disk */
#define TDK_DISK_HD	0x0015U		/* Hard disk */
#define TDK_DISK_CDROM	0x0016U		/* CD-ROM */

/*
 * Device open mode
 */
#define TD_READ		0x0001U		/* Read only */
#define TD_WRITE	0x0002U		/* Write only */
#define TD_UPDATE	0x0003U		/* Read and write */
#define TD_EXCL		0x0100U		/* Exclusive */
#define TD_WEXCL	0x0200U		/* Exclusive write */
#define TD_REXCL	0x0400U		/* Exclusive read */

/*
 * Device close option
 */
#define TD_EJECT	0x0001U		/* Media eject */

/*
 * Suspend mode
 */
#define TD_SUSPEND	0x0001U		/* Suspend */
#define TD_DISSUS	0x0002U		/* Disable suspend */
#define TD_ENASUS	0x0003U		/* Enable suspend */
#define TD_CHECK	0x0004U		/* Get suspend disable request count */
#define TD_FORCE	0x8000U		/* Specify forced suspend */

/*
 * Device information
 */
typedef struct t_rdev {
	ATR	devatr;		/* Device attribute */
	W	blksz;		/* Specific data block size (-1: Unknown) */
	INT	nsub;		/* Number of subunits */
	INT	subno;		/* 0: Physical device,
				   1 - nsub: Subunit number +1 */
} T_RDEV;

/*
 * Registration device information
 */
typedef struct t_ldev {
	ATR	devatr;		/* Device attribute */
	W	blksz;		/* Specific data block size (-1: Unknown) */
	INT	nsub;		/* Number of subunits */
	UB	devnm[L_DEVNM];	/* Physical device name */
} T_LDEV;

/*
 * Common attribute data number
 *	RW: Readable (tk_rea_dev)/writable (tk_wri_dev)
 *	R-: Readable (tk_rea_dev) only
 */
#define TDN_EVENT	(-1)	/* RW:Message buffer ID
				      for event notification */
#define TDN_DISKINFO	(-2)	/* R-:Disk information */
#define TDN_DISPSPEC	(-3)	/* R-:Display device specification */
#define TDN_PCMCIAINFO	(-4)	/* R-:PC card information */

/*
 * Device event type
 */
typedef	enum tdevttyp {
	TDE_unknown	= 0,		/* Undefined */
	TDE_MOUNT	= 0x01,		/* Media insert */
	TDE_EJECT	= 0x02,		/* Media eject */
	TDE_ILLMOUNT	= 0x03,		/* Media incorrect insert */
	TDE_ILLEJECT	= 0x04,		/* Media incorrect eject */
	TDE_REMOUNT	= 0x05,		/* Media re-insert */
	TDE_CARDBATLOW	= 0x06,		/* Card battery low */
	TDE_CARDBATFAIL	= 0x07,		/* Card battery abnormal */
	TDE_REQEJECT	= 0x08,		/* Media eject request */
	TDE_PDBUT	= 0x11,		/* PD button state change */
	TDE_PDMOVE	= 0x12,		/* PD position move */
	TDE_PDSTATE	= 0x13,		/* PD state change */
	TDE_PDEXT	= 0x14,		/* PD extended event */
	TDE_KEYDOWN	= 0x21,		/* Key down */
	TDE_KEYUP	= 0x22,		/* Key up */
	TDE_KEYMETA	= 0x23,		/* Meta key state change */
	TDE_POWEROFF	= 0x31,		/* Power switch off */
	TDE_POWERLOW	= 0x32,		/* Power low */
	TDE_POWERFAIL	= 0x33,		/* Power abnormal */
	TDE_POWERSUS	= 0x34,		/* Automatic suspend */
	TDE_POWERUPTM	= 0x35,		/* Clock update */
	TDE_CKPWON	= 0x41		/* Automatic power on notification */
} TDEvtTyp;

/*
 * Device event message format
 */
typedef struct t_devevt {
	TDEvtTyp	evttyp;		/* Event type */
	/* Information by each event type is added below */
} T_DEVEVT;

/*
 * Device event message format with device ID
 */
typedef struct t_devevt_id {
	TDEvtTyp	evttyp;		/* Event type */
	ID		devid;		/* Device ID */
	/* Information by each event type is added below */
} T_DEVEVT_ID;

/* ------------------------------------------------------------------------ */

/*
 * Device registration information
 */
typedef struct t_ddev {
	void	*exinf;		/* Extended information */
	ATR	drvatr;		/* Driver attribute */
	ATR	devatr;		/* Device attribute */
	INT	nsub;		/* Number of subunits */
	W	blksz;		/* Specific data block size (-1: Unknown) */
	FP	openfn;		/* Open function */
	FP	closefn;	/* Close function */
	FP	execfn;		/* Execute function */
	FP	waitfn;		/* Completion wait function */
	FP	abortfn;	/* Abort function */
	FP	eventfn;	/* Event function */
} T_DDEV;

/*
 * Open function:
 *	ER  openfn( ID devid, UINT omode, void *exinf )
 * Close function:
 *	ER  closefn( ID devid, UINT option, void *exinf )
 * Execute function:
 *	ER  execfn( T_DEVREQ *devreq, TMO tmout, void *exinf )
 * Completion wait function:
 *	INT waitfn( T_DEVREQ *devreq, INT nreq, TMO tmout, void *exinf )
 * Abort function:
 *	ER  abortfn( ID tskid, T_DEVREQ *devreq, INT nreq, void *exinf) 
 * Event function:
 *	INT eventfn( INT evttyp, void *evtinf, void *exinf )
 */

/*
 * Driver attribute
 */
#define TDA_OPENREQ	0x0001U	/* Every time open/close */

/*
 * Device initial setting information
 */
typedef struct t_idev {
	ID	evtmbfid;	/* Message buffer ID for event notification */
} T_IDEV;

/*
 * Device request packet
 *	 I: Input parameter
 *	 O: Output parameter
 */
typedef struct t_devreq {
	struct t_devreq	*next;	/* I:Link to request packet (NULL:End) */
	void	*exinf;		/* X:Extended information */
	ID	devid;		/* I:Target device ID */
	INT	cmd:4;		/* I:Request command */
	BOOL	abort:1;	/* I:When executing abort request, TRUE */
	W	start;		/* I:Start data number */
	W	size;		/* I:Request size */
	void	*buf;		/* I:Input/output buffer address */
	W	asize;		/* O:Result size */
	ER	error;		/* O:Result error */
} T_DEVREQ;

/*
 * Request command
 */
#define TDC_READ	1	/* Read request */
#define TDC_WRITE	2	/* Write request */

/*
 * Driver request event
 */
#define TDV_SUSPEND	(-1)	/* Suspend */
#define TDV_RESUME	(-2)	/* Resume */
#define TDV_CARDEVT	1	/* PC card event (Refer card manager) */
#define TDV_USBEVT	2	/* USB event     (Refer USB manager) */

/*
 * System call prototype declaration
 */
IMPORT ID tk_cre_tsk( CONST T_CTSK *pk_ctsk );
IMPORT ER tk_del_tsk( ID tskid );
IMPORT ER tk_sta_tsk( ID tskid, INT stacd );
IMPORT void tk_ext_tsk( void );
IMPORT void tk_exd_tsk( void );
IMPORT ER tk_ter_tsk( ID tskid );
IMPORT ER tk_dis_dsp( void );
IMPORT ER tk_ena_dsp( void );
IMPORT ER tk_chg_pri( ID tskid, PRI tskpri );
IMPORT ER tk_rot_rdq( PRI tskpri );
IMPORT ER tk_rel_wai( ID tskid );
IMPORT ID tk_get_tid( void );
IMPORT ER tk_ref_tsk( ID tskid, T_RTSK *pk_rtsk );
IMPORT ER tk_sus_tsk( ID tskid );
IMPORT ER tk_rsm_tsk( ID tskid );
IMPORT ER tk_frsm_tsk( ID tskid );
IMPORT ER tk_slp_tsk( TMO tmout );
IMPORT ER tk_wup_tsk( ID tskid );
IMPORT INT tk_can_wup( ID tskid );
IMPORT ER tk_dly_tsk( RELTIM dlytim );

#if TK_SUPPORT_REGOPS
IMPORT ER tk_get_reg( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs );
IMPORT ER tk_set_reg( ID tskid, CONST T_REGS *pk_regs, CONST T_EIT *pk_eit, CONST T_CREGS *pk_cregs );
#endif /* TK_SUPPORT_REGOPS */

#if NUM_COPROCESSOR > 0
IMPORT ER tk_get_cpr( ID tskid, INT copno, T_COPREGS *pk_copregs);
IMPORT ER tk_set_cpr(ID tskid, INT copno, CONST T_COPREGS *pk_copregs);
#endif

IMPORT ID tk_cre_sem( CONST T_CSEM *pk_csem );
IMPORT ER tk_del_sem( ID semid );
IMPORT ER tk_sig_sem( ID semid, INT cnt );
IMPORT ER tk_wai_sem( ID semid, INT cnt, TMO tmout );
IMPORT ER tk_ref_sem( ID semid, T_RSEM *pk_rsem );

IMPORT ID tk_cre_mtx( CONST T_CMTX *pk_cmtx );
IMPORT ER tk_del_mtx( ID mtxid );
IMPORT ER tk_loc_mtx( ID mtxid, TMO tmout );
IMPORT ER tk_unl_mtx( ID mtxid );
IMPORT ER tk_ref_mtx( ID mtxid, T_RMTX *pk_rmtx );

IMPORT ID tk_cre_flg( CONST T_CFLG *pk_cflg );
IMPORT ER tk_del_flg( ID flgid );
IMPORT ER tk_set_flg( ID flgid, UINT setptn );
IMPORT ER tk_clr_flg( ID flgid, UINT clrptn );
IMPORT ER tk_wai_flg( ID flgid, UINT waiptn, UINT wfmode, UINT *p_flgptn, TMO tmout );
IMPORT ER tk_ref_flg( ID flgid, T_RFLG *pk_rflg );

IMPORT ID tk_cre_mbx( CONST T_CMBX* pk_cmbx );
IMPORT ER tk_del_mbx( ID mbxid );
IMPORT ER tk_snd_mbx( ID mbxid, T_MSG *pk_msg );
IMPORT ER tk_rcv_mbx( ID mbxid, T_MSG **ppk_msg, TMO tmout );
IMPORT ER tk_ref_mbx( ID mbxid, T_RMBX *pk_rmbx );
IMPORT ID tk_cre_mbf( CONST T_CMBF *pk_cmbf );
IMPORT ER tk_del_mbf( ID mbfid );
IMPORT ER tk_snd_mbf( ID mbfid, CONST void *msg, INT msgsz, TMO tmout );
IMPORT INT tk_rcv_mbf( ID mbfid, void *msg, TMO tmout );
IMPORT ER tk_ref_mbf( ID mbfid, T_RMBF *pk_rmbf );

IMPORT ID tk_cre_por( CONST T_CPOR *pk_cpor );
IMPORT ER tk_del_por( ID porid );
IMPORT INT tk_cal_por( ID porid, UINT calptn, void *msg, INT cmsgsz, TMO tmout );
IMPORT INT tk_acp_por( ID porid, UINT acpptn, RNO *p_rdvno, void *msg, TMO tmout );
IMPORT ER tk_fwd_por( ID porid, UINT calptn, RNO rdvno, CONST void *msg, INT cmsgsz );
IMPORT ER tk_rpl_rdv( RNO rdvno, CONST void *msg, INT rmsgsz );
IMPORT ER tk_ref_por( ID porid, T_RPOR *pk_rpor );

IMPORT ER tk_def_int( UINT intno, CONST T_DINT *pk_dint );
IMPORT void tk_ret_int( void );

IMPORT ID tk_cre_mpl( CONST T_CMPL *pk_cmpl );
IMPORT ER tk_del_mpl( ID mplid );
IMPORT ER tk_get_mpl( ID mplid, SZ blksz, void **p_blk, TMO tmout );
IMPORT ER tk_rel_mpl( ID mplid, void *blk );
IMPORT ER tk_ref_mpl( ID mplid, T_RMPL *pk_rmpl );

IMPORT ID tk_cre_mpf( CONST T_CMPF *pk_cmpf );
IMPORT ER tk_del_mpf( ID mpfid );
IMPORT ER tk_get_mpf( ID mpfid, void **p_blf, TMO tmout );
IMPORT ER tk_rel_mpf( ID mpfid, void *blf );
IMPORT ER tk_ref_mpf( ID mpfid, T_RMPF *pk_rmpf );

IMPORT ER tk_set_utc( CONST SYSTIM *pk_tim );
IMPORT ER tk_get_utc( SYSTIM *pk_tim );
IMPORT ER tk_set_tim( CONST SYSTIM *pk_tim );
IMPORT ER tk_get_tim( SYSTIM *pk_tim );
IMPORT ER tk_get_otm( SYSTIM *pk_tim );

IMPORT ID tk_cre_cyc( CONST T_CCYC *pk_ccyc );
IMPORT ER tk_del_cyc( ID cycid );
IMPORT ER tk_sta_cyc( ID cycid );
IMPORT ER tk_stp_cyc( ID cycid );
IMPORT ER tk_ref_cyc( ID cycid, T_RCYC *pk_rcyc );

IMPORT ID tk_cre_alm( CONST T_CALM *pk_calm );
IMPORT ER tk_del_alm( ID almid );
IMPORT ER tk_sta_alm( ID almid, RELTIM almtim );
IMPORT ER tk_stp_alm( ID almid );
IMPORT ER tk_ref_alm( ID almid, T_RALM *pk_ralm );

IMPORT ER tk_ref_sys( T_RSYS *pk_rsys );
IMPORT ER tk_set_pow( UINT powmode);
IMPORT ER tk_ref_ver( T_RVER *pk_rver );

IMPORT ER tk_def_ssy( ID ssid, CONST T_DSSY *pk_dssy );
IMPORT ER tk_ref_ssy( ID ssid, T_RSSY *pk_rssy );

IMPORT ID tk_opn_dev( CONST UB *devnm, UINT omode );
IMPORT ER tk_cls_dev( ID dd, UINT option );
IMPORT ID tk_rea_dev( ID dd, W start, void *buf, SZ size, TMO tmout );
IMPORT ER tk_srea_dev( ID dd, W start, void *buf, SZ size, SZ *asize );
IMPORT ID tk_wri_dev( ID dd, W start, CONST void *buf, SZ size, TMO tmout );
IMPORT ER tk_swri_dev( ID dd, W start, CONST void *buf, SZ size, SZ *asize );
IMPORT ID tk_wai_dev( ID dd, ID reqid, SZ *asize, ER *ioer, TMO tmout );
IMPORT INT tk_sus_dev( UINT mode );
IMPORT ID tk_get_dev( ID devid, UB *devnm );
IMPORT ID tk_ref_dev( CONST UB *devnm, T_RDEV *pk_rdev );
IMPORT ID tk_oref_dev( ID dd, T_RDEV *pk_rdev );
IMPORT INT tk_lst_dev( T_LDEV *pk_ldev, INT start, INT ndev );
IMPORT INT tk_evt_dev( ID devid, INT evttyp, void *evtinf );
IMPORT ID tk_def_dev( CONST UB *devnm, CONST T_DDEV *pk_ddev, T_IDEV *pk_idev );
IMPORT ER tk_ref_idv( T_IDEV *pk_idev );

#endif /* __TK_SYSCALL_H__ */
