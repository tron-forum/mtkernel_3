/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 */

/*
 *    libtm.h
 *    T-Monitor compatible calls library
 */

#include <config_tm.h>

#define	CHR_CR		(0x0D)		/* Carriage Return */
#define	CHR_ETX		(0x03)		/* End of TeXt */
#define	CHR_LF		(0x0A)		/* Line Feed */

IMPORT void tm_com_init(void);

IMPORT void tm_snd_dat( const UB* buf, INT size );
IMPORT void tm_rcv_dat( UB* buf, INT size );

