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
 *	tkinit.c
 *	Initialize micro T-Kernel objects.
 */

#include "kernel.h"
#include "timer.h"

/*
 * Each kernel-object initialization
 */
EXPORT ER knl_init_object( void)
{	
	ER	ercd;

	ercd = knl_task_initialize();
	if(ercd < E_OK) return ercd;

#if USE_SEMAPHORE
	ercd = knl_semaphore_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_EVENTFLAG
	ercd = knl_eventflag_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_MAILBOX
	ercd = knl_mailbox_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_MESSAGEBUFFER
	ercd = knl_messagebuffer_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_LEGACY_API && USE_RENDEZVOUS
	ercd = knl_rendezvous_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_MUTEX
	ercd = knl_mutex_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_MEMORYPOOL
	ercd = knl_memorypool_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_FIX_MEMORYPOOL
	ercd = knl_fix_memorypool_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_CYCLICHANDLER
	ercd = knl_cyclichandler_initialize();
	if(ercd < E_OK) return ercd;
#endif
#if USE_ALARMHANDLER
	ercd = knl_alarmhandler_initialize();
	if(ercd < E_OK) return ercd;
#endif

	return E_OK;
}
