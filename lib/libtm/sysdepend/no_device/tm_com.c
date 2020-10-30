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
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (No device)
 * 	This is a dummy program when there is no communication device.
 */

#include <tk/typedef.h>
#include <sys/sysdef.h>
#include "../../libtm.h"

#ifdef USE_COM_NO_DEVICE

EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
}


EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
}


EXPORT	void	tm_com_init(void)
{
}

#endif /* USE_COM_NO_DEVICE */