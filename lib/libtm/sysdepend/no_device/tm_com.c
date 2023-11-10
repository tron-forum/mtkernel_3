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
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (No device)
 * 	This is a dummy program when there is no communication device.
 */

#include <tk/tkernel.h>

#if USE_TMONITOR
#include "../../libtm.h"

#if TM_COM_NO_DEV

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
}


EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
}


EXPORT	void	tm_com_init(void)
{
}

#endif /* TM_COM_NO_DEV */
#endif /* USE_TMONITOR */