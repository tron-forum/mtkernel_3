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
 *	klock.h		Kernel Lock
 *	Locked task is the highest run priority.
 *	Unable to nest lock.
 */

#ifndef _KLOCK_
#define _KLOCK_

typedef struct objlock {
	QUEUE		wtskq;		/* Wait task queue */
} OBJLOCK;

Inline void knl_InitOBJLOCK( OBJLOCK *loc )
{
	loc->wtskq.next = NULL;
}
IMPORT void knl_LockOBJ( OBJLOCK* );
IMPORT void knl_UnlockOBJ( OBJLOCK* );

Inline BOOL knl_isLockedOBJ( OBJLOCK *loc )
{
	return ( loc->wtskq.next != NULL )? TRUE: FALSE;
}

#endif /* KLOCK */
