/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysinit.c
 *	micro T-Kernel Startup / Finalization
 */
#include "kernel.h"
#include <tm/tmonitor.h>

/*
 * Start micro T-Kernel
 *    Initialize sequence before micro T-Kernel start.
 *    Perform preparation necessary to start micro T-Kernel.
 */
EXPORT INT main( void )
{
	ER	ercd;

	DISABLE_INTERRUPT;

#if USE_TMONITOR
	/* Initialize T-Monitor Compatible Library */
	libtm_init();
#endif

#if USE_IMALLOC
	/* Initialize Internal memory allocation (Imalloc) */
	ercd = knl_init_Imalloc();
	if ( ercd < E_OK ) {
		SYSTEM_MESSAGE("!ERROR! init_Imalloc\n");
		goto err_ret;
	}
#endif /* USE_IMALLOC */

	/* Initialize Device before micro T-Kernel starts */
	ercd = knl_init_device();
	if ( ercd < E_OK ) {
		SYSTEM_MESSAGE("!ERROR! init_device\n");
		goto err_ret;
	}

	/* Interrupt initialize */
	ercd = knl_init_interrupt();
	if ( ercd < E_OK ) {
		SYSTEM_MESSAGE("!ERROR! init_initialize\n");
		goto err_ret;
	}

	/* Initialize Kernel-objects  */
	ercd = knl_init_object();
	if ( ercd < E_OK ) {
		SYSTEM_MESSAGE("!ERROR! kernel object initialize\n");
		goto err_ret1;
	}

	/* Start System Timer */
	ercd = knl_timer_startup();
	if ( ercd < E_OK ) {
		SYSTEM_MESSAGE("!ERROR! System timer startup\n");
		goto err_ret1;
	}

	/* Create & start initial task */
	ercd = tk_cre_tsk((CONST T_CTSK *)&knl_init_ctsk);
	if ( ercd >= E_OK ) {
		ercd = tk_sta_tsk((ID)ercd, 0);
		if ( ercd >= E_OK ) {
			knl_force_dispatch();
			/**** Start Initial Task. ****/
			/**** No return ****/
		} else {
			SYSTEM_MESSAGE("!ERROR! Initial Task can not start\n");
		}
	} else {
		SYSTEM_MESSAGE("!ERROR! Initial Task can not creat\n");
	}

	/* After this, Error handling */

#if USE_SHUTDOWN
	knl_timer_shutdown();	/* Stop System timer */
err_ret1:
	knl_shutdown_hw();	/* Hardware-dependent Finalization */
	/**** No return ****/
#else
err_ret1:
#endif /* USE_SHUTDOWN */

err_ret:
	while(1);
	return 0;
}

/*
 * Exit micro T-Kernel from Initial Task.
 */
#if USE_SHUTDOWN
EXPORT void knl_tkernel_exit( void )
{
	knl_timer_shutdown();	/* Stop System timer */
	knl_shutdown_hw();	/* Hardware-dependent Finalization */
	/**** No return ****/

	while(1);
}
#endif /* USE_SHUTDOWN */
