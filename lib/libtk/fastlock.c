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
 *	fastlock.c
 *
 *	High-speed exclusive control lock 
 */

#include <config.h>
#include <tk/tkernel.h>

#if USE_FAST_LOCK
/* ------------------------------------------------------------------------ */
/*
 *	Inc	Increment cnt, in result if cnt >  0, returns positive value.
 *		If cnt <= 0, returns 0 or negative value.
 *	Dec	Decrement cnt, in result if cnt >= 0, returns positive value.
 *		If cnt <  0, returns 0 or negative value. 
 *	Increment/Decrement and evaluation of the associated result must
 *	be executed exclusively.
 */

Inline INT Inc( FastLock *lock )
{
	UINT	imask;
	INT	c;
	DI(imask);
	c = ++lock->cnt;
	EI(imask);
	return c;
}
Inline INT Dec( FastLock *lock )
{
	UINT	imask;
	INT	c;
	DI(imask);
	c = lock->cnt--;
	EI(imask);
	return c;
}

/*
 * Lock 
 */
EXPORT void Lock( FastLock *lock )
{
	if ( Inc(lock) > 0 ) {
		tk_wai_sem(lock->id, 1, TMO_FEVR);
	}
}

/*
 * Lock release
 */
EXPORT void Unlock( FastLock *lock )
{
	if ( Dec(lock) > 0 ) {
		tk_sig_sem(lock->id, 1);
	}
}

/*
 * Create high-speed lock 
 */
EXPORT ER CreateLock( FastLock *lock, CONST UB *name )
{
	T_CSEM	csem;
	ER	ercd;

	csem.sematr  = TA_TPRI;
	csem.isemcnt = 0;
	csem.maxsem  = 1;
	SetOBJNAME(csem.exinf, "FLOC");

	ercd = tk_cre_sem(&csem);
	if ( ercd < E_OK ) {
		return ercd;
	}

	lock->id = ercd;
	lock->cnt = -1;
	lock->name = name;
  
	return E_OK;
}

/*
 * Delete high-speed lock
 */
EXPORT void DeleteLock( FastLock *lock )
{
	if ( lock->id > 0 ) {
		tk_del_sem(lock->id);
	}
	lock->id = 0;
}

#endif /* USE_FAST_LOCK */