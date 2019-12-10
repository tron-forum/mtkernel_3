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
 *	objname.c
 *	Object name support
 */

#include "kernel.h"

#if USE_DBGSPT

#if USE_OBJECT_NAME
LOCAL ER knl_object_getname( UINT objtype, ID objid, UB **name)
{
	ER	ercd;

	switch (objtype) {
	  case TN_TSK:
		{
			IMPORT ER knl_task_getname(ID id, UB **name);
			ercd = knl_task_getname(objid, name);
			break;
		}

#if USE_SEMAPHORE
	  case TN_SEM:
		{
			IMPORT ER knl_semaphore_getname(ID id, UB **name);
			ercd = knl_semaphore_getname(objid, name);
			break;
		}
#endif

#if USE_EVENTFLAG
	  case TN_FLG:
		{
			IMPORT ER knl_eventflag_getname(ID id, UB **name);
			ercd = knl_eventflag_getname(objid, name);
			break;
		}
#endif

#if USE_MAILBOX
	  case TN_MBX:
		{
			IMPORT ER knl_mailbox_getname(ID id, UB **name);
			ercd = knl_mailbox_getname(objid, name);
			break;
		}
#endif

#if USE_MESSAGEBUFFER
	  case TN_MBF:
		{
			IMPORT ER knl_messagebuffer_getname(ID id, UB **name);
			ercd = knl_messagebuffer_getname(objid, name);
			break;
		}
#endif

#if USE_LEGACY_API && USE_RENDEZVOUS
	  case TN_POR:
		{
			IMPORT ER knl_rendezvous_getname(ID id, UB **name);
			ercd = knl_rendezvous_getname(objid, name);
			break;
		}
#endif

#if USE_MUTEX
	  case TN_MTX:
		{
			IMPORT ER knl_mutex_getname(ID id, UB **name);
			ercd = knl_mutex_getname(objid, name);
			break;
		}
#endif

#if USE_MEMORYPOOL
	  case TN_MPL:
		{
			IMPORT ER knl_memorypool_getname(ID id, UB **name);
			ercd = knl_memorypool_getname(objid, name);
			break;
		}
#endif

#if USE_FIX_MEMORYPOOL
	  case TN_MPF:
		{
			IMPORT ER knl_fix_memorypool_getname(ID id, UB **name);
			ercd = knl_fix_memorypool_getname(objid, name);
			break;
		}
#endif

#if USE_CYCLICHANDLER
	  case TN_CYC:
		{
			IMPORT ER knl_cyclichandler_getname(ID id, UB **name);
			ercd = knl_cyclichandler_getname(objid, name);
			break;
		}
#endif

#if USE_ALARMHANDLER
	  case TN_ALM:
		{
			IMPORT ER knl_alarmhandler_getname(ID id, UB **name);
			ercd = knl_alarmhandler_getname(objid, name);
			break;
		}
#endif

	  default:
		ercd = E_PAR;

	}

	return ercd;
}
#endif /* USE_OBJECT_NAME */


SYSCALL ER td_ref_dsname( UINT type, ID id, UB *dsname )
{
#if USE_OBJECT_NAME
	UB	*name_cb;
	ER	ercd;

	ercd = knl_object_getname(type, id, &name_cb);
	if (ercd == E_OK) {
		knl_strncpy((char*)dsname, (char*)name_cb, OBJECT_NAME_LENGTH);
	}

	return ercd;
#else
	return E_NOSPT;
#endif /* USE_OBJECT_NAME */
}


SYSCALL ER td_set_dsname( UINT type, ID id, CONST UB *dsname )
{
#if USE_OBJECT_NAME
	UB	*name_cb;
	ER	ercd;

	ercd = knl_object_getname(type, id, &name_cb);
	if (ercd == E_OK) {
		knl_strncpy((char*)name_cb, (char*)dsname, OBJECT_NAME_LENGTH);
	}

	return ercd;
#else
	return E_NOSPT;
#endif /* USE_OBJECT_NAME */
}

#endif /* USE_DBGSPT */
