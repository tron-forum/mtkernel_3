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
 *	rendezvous.h
 *	Rendezvous
 */

#ifndef _RENDEZVOUS_H_
#define _RENDEZVOUS_H_

/*
 * Rendezvous port control block
 */
typedef struct port_control_block {
	QUEUE	call_queue;	/* Port call wait queue */
	ID	porid;		/* Port ID */
	void	*exinf;		/* Extended information */
	ATR	poratr;		/* Port attribute */
	QUEUE	accept_queue;	/* Port accept wait queue */
	INT	maxcmsz;	/* Maximum length of call message */
	INT	maxrmsz;	/* Maximum length of reply message */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} PORCB;
/** [END Common Definitions] */

IMPORT PORCB knl_porcb_table[];	/* Rendezvous port control block */
IMPORT QUEUE knl_free_porcb;	/* FreeQue */

#define get_porcb(id)	( &knl_porcb_table[INDEX_POR(id)] )

#if USE_LEGACY_API

#define RDVNO_SHIFT	(sizeof(RNO)*8/2)

/*
 * Create rendezvous number
 */
Inline RNO knl_gen_rdvno( TCB *tcb )
{
	RNO	rdvno;

	rdvno = tcb->wrdvno;
	tcb->wrdvno += (1U << RDVNO_SHIFT);

	return rdvno;
}

/*
 * Get task ID from rendezvous number 
 */
Inline ID knl_get_tskid_rdvno( RNO rdvno )
{
	return (ID)((UINT)rdvno & ((1U << RDVNO_SHIFT) - 1));
}

/*
 * Check validity of rendezvous number
 */
#define CHECK_RDVNO(rdvno) {					\
	if ( !CHK_TSKID(knl_get_tskid_rdvno(rdvno)) ) {		\
		return E_OBJ;					\
	}							\
}

#endif	/* USE_LEGACY_API */

/*
 * Definition of rendezvous wait specification
 */
IMPORT CONST WSPEC knl_wspec_cal_tfifo;
IMPORT CONST WSPEC knl_wspec_cal_tpri;
IMPORT CONST WSPEC knl_wspec_rdv;


#endif /* _RENDEZVOUS_H_ */
