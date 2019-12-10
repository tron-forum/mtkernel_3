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
 *	fastmlock.c
 *
 *	High-speed exclusive control multi-lock 
 */

#include <config.h>
#include <tk/tkernel.h>

#if USE_MULTI_LOCK
/* ------------------------------------------------------------------------ */
/*
 *	void INC( INT *val )		increment 
 *	void DEC( INT *val )		decrement 
 *	BOOL BTS( UINT *val, INT no )	bit test and set 
 *	void BR( UINT *val, INT no )	bit reset 
 *
 *	The above must be operated exclusively.
 */

Inline void INC( INT *val )
{
	UINT	imask;

	DI(imask);
	(*val)++;
	EI(imask);
}

Inline void DEC( INT *val )
{
	UINT	imask;

	DI(imask);
	(*val)--;
	EI(imask);
}

Inline BOOL BTS( UINT *val, INT no )
{
	UINT	imask;
	UINT	b;
	UINT	bm = (UINT)(1 << no);

	DI(imask);
	b = *val & bm;
	*val |= bm;
	EI(imask);
	return (BOOL)b;
}

Inline void BR( UINT *val, INT no )
{
	UINT	imask;

	DI(imask);
	*val &= ~(UINT)(1 << no);
	EI(imask);
}


/*
 * Lock with wait time designation 
 *	no	lock number 0 - 31 
 */
EXPORT ER MLockTmo( FastMLock *lock, INT no, TMO tmo )
{
	UINT	ptn = (UINT)(1 << no);
	UINT	flg;
	ER	ercd;

	INC(&lock->wai);
	for ( ;; ) {
		if ( !BTS(&lock->flg, no) ) {
			ercd = E_OK;
			break;
		}

		ercd = tk_wai_flg(lock->id, ptn, TWF_ORW|TWF_BITCLR, &flg, tmo);
		if ( ercd < E_OK ) {
			break;
		}
	}
	DEC(&lock->wai);

	return ercd;
}

/*
 * Lock 
 *	no	Lock number 0 - 31 
 */
EXPORT ER MLock( FastMLock *lock, INT no )
{
	return MLockTmo(lock, no, TMO_FEVR);
}

/*
 * Lock release 
 *	no	Lock number 0 - 31 
 */
EXPORT ER MUnlock( FastMLock *lock, INT no )
{
	UINT	ptn = (UINT)(1 << no);
	ER	ercd;

	BR(&lock->flg, no);
	ercd = ( lock->wai == 0 )? E_OK: tk_set_flg(lock->id, ptn);

	return ercd;
}

/*
 * Create multi-lock 
 */
EXPORT ER CreateMLock( FastMLock *lock, CONST UB *name )
{
	T_CFLG	cflg;
	ER	ercd;

	cflg.flgatr  = TA_TPRI | TA_WMUL;
	cflg.iflgptn = 0;
	SetOBJNAME(cflg.exinf, "MLOC");

	lock->id = ercd = tk_cre_flg(&cflg);
	if ( ercd < E_OK ) {
		return ercd;
	}

	lock->wai = 0;
	lock->flg = 0;
	lock->name = name;

	return E_OK;
}

/*
 * Delete multi-lock 
 */
EXPORT ER DeleteMLock( FastMLock *lock )
{
	ER	ercd;

	if ( lock->id <= 0 ) {
		return E_PAR;
	}

	ercd = tk_del_flg(lock->id);
	if ( ercd < E_OK ) {
		return ercd;
	}

	lock->id = 0;

	return E_OK;
}

#endif /* USE_MULTI_LOCK */