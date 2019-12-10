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
 *	klock.c		Kernel Lock
 *	Locked task is the highest run priority.
 *	Unable to nest lock.
 */

#include "kernel.h"
#include "klock.h"
#include "ready_queue.h"

/*
 * Object lock
 *	Do not call from critical section
 */
EXPORT void knl_LockOBJ( OBJLOCK *loc )
{
	BOOL	klocked;

  retry:
	BEGIN_CRITICAL_SECTION;
	klocked = knl_ctxtsk->klocked;
	if ( !klocked ) {
		if ( loc->wtskq.next == NULL ) {
			/* Lock */
			QueInit(&loc->wtskq);

			knl_ctxtsk->klocked = klocked = TRUE;
			knl_ready_queue.klocktsk = knl_ctxtsk;
		} else {
			/* Ready for lock */
			knl_ready_queue_delete(&knl_ready_queue, knl_ctxtsk);
			knl_ctxtsk->klockwait = TRUE;
			QueInsert(&knl_ctxtsk->tskque, &loc->wtskq);

			knl_schedtsk = knl_ready_queue_top(&knl_ready_queue);
		}
	}
	END_CRITICAL_SECTION;
	/* Since wait could be freed without getting lock, 
	   need to re-try if lock is not got */
	if ( !klocked ) {
		goto retry;
	}
}

/*
 * Object unlock
 *	It may be called from a critical section.
 */
EXPORT void knl_UnlockOBJ( OBJLOCK *loc )
{
	TCB	*tcb;

	BEGIN_CRITICAL_SECTION;
	knl_ctxtsk->klocked = FALSE;
	knl_ready_queue.klocktsk = NULL;

	tcb = (TCB*)QueRemoveNext(&loc->wtskq);
	if ( tcb == NULL ) {
		/* Free lock */
		loc->wtskq.next = NULL;
	} else {
		/* Wake lock wait task */
		tcb->klockwait = FALSE;
		tcb->klocked = TRUE;
		knl_ready_queue_insert_top(&knl_ready_queue, tcb);
	}

	knl_schedtsk = knl_ready_queue_top(&knl_ready_queue);
	END_CRITICAL_SECTION;
}
