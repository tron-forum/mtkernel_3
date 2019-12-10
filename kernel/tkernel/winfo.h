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
 *	winfo.h
 *	Definition of Wait Information for Synchronization/Communication Object
 */

#ifndef _WINFO_
#define _WINFO_

/*
 * Semaphore wait (TTW_SEM)
 */
typedef struct {
	INT	cnt;		/* Request resource number */
} WINFO_SEM;

/*
 * Event flag wait (TTW_FLG)
 */
typedef struct {
	UINT	waiptn;		/* Wait bit pattern */
	UINT	wfmode;		/* Wait mode */
	UINT	*p_flgptn;	/* Address that has a bit pattern
				   at wait released */
} WINFO_FLG;

/*
 * Mailbox wait (TTW_MBX)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* Address that has the head of a
				   message packet */
} WINFO_MBX;

/*
 * Message buffer receive/send wait (TTW_RMBF, TTW_SMBF)
 */
typedef struct {
	void	*msg;		/* Address that has a received message */
	INT	*p_msgsz;	/* Address that has a received message size */
} WINFO_RMBF;

typedef struct {
	CONST void *msg;	/* Send message head address */
	INT	msgsz;		/* Send message size */
} WINFO_SMBF;

/*
 * Rendezvous call/accept/end wait (TTW_CAL, TTW_ACP, TTW_RDV)
 */
typedef struct {
	UINT	calptn;		/* Bit pattern that indicates caller
				   select condition */
	void	*msg;		/* Address that has a message */
	INT	cmsgsz;		/* Call message size */
	INT	*p_rmsgsz;	/* Address that has a reply message size */
} WINFO_CAL;

typedef struct {
	UINT	acpptn;		/* Bit pattern that indicates receiver
				   select condition */
	void	*msg;		/* Address that has a call message */
	RNO	*p_rdvno;	/* Address that has the rendezvous number */
	INT	*p_cmsgsz;	/* Address that has the call message size */
} WINFO_ACP;

typedef struct {
	RNO	rdvno;		/* Rendezvous number */
	void	*msg;		/* Address that has a message */
	INT	maxrmsz;	/* Maximum length of reply message */
	INT	*p_rmsgsz;	/* Address that has a reply message size */
} WINFO_RDV;

/*
 * Variable size memory pool wait (TTW_MPL)
 */
typedef struct {
	W	blksz;		/* Memory block size */
	void	**p_blk;		/* Address that has the head of a
				   memory block */
} WINFO_MPL;

/*
 * Fixed size memory pool wait (TTW_MPF)
 */
typedef struct {
	void	**p_blf;		/* Address that has the head of a
				   memory block */
} WINFO_MPF;

/*
 * Definition of wait information in task control block
 */
typedef union {
#if USE_SEMAPHORE
	WINFO_SEM	sem;
#endif
#if USE_EVENTFLAG
	WINFO_FLG	flg;
#endif
#if USE_MAILBOX
	WINFO_MBX	mbx;
#endif
#if USE_MESSAGEBUFFER
	WINFO_RMBF	rmbf;
	WINFO_SMBF	smbf;
#endif
#if USE_LEGACY_API && USE_RENDEZVOUS
	WINFO_CAL	cal;
	WINFO_ACP	acp;
	WINFO_RDV	rdv;
#endif
#if USE_MEMORYPOOL
	WINFO_MPL	mpl;
#endif
#if USE_FIX_MEMORYPOOL
	WINFO_MPF	mpf;
#endif
} WINFO;

/*
 * Definition of wait specification structure
 */
typedef struct {
	UW	tskwait;			/* Wait factor */
	void	(*chg_pri_hook)(TCB *, INT);	/* Process at task priority
						   change */
	void	(*rel_wai_hook)(TCB *);		/* Process at task wait
						   release */
} WSPEC;

#endif /* _WINFO_ */
