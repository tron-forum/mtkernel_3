/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03.
 *
 *----------------------------------------------------------------------
 */
#include <tk/tkernel.h>
#include <kernel.h>

#ifdef IOTE_RX231

/*
 *	power.c (RX231 IoT-Engine)
 *	Power-Saving Function
 */

#include "sysdepend.h"

/*
 * Switch to power-saving mode
 */
EXPORT void low_pow( void )
{
}

/*
 * Move to suspend mode
 */
EXPORT void off_pow( void )
{
}


#endif /* IOTE_RX231 */
