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
 *	tstdlib.h
 *
 *	T-Kernel standard library
 *
 */

#ifndef	_TSTDLIB_
#define _TSTDLIB_

#include <tk/tkernel.h>


IMPORT void	knl_bitclr( void *base, W offset );
IMPORT void	knl_bitset( void *base, W offset );

IMPORT W	knl_bitsearch1( void *base, W offset, W width );


IMPORT void	*knl_memset( void *s, int c, SZ n );
IMPORT void	*knl_memcpy( void *dst, const void *src, SZ n );

IMPORT SZ	knl_strlen( const char *s );
IMPORT char	*knl_strcpy( char *dst, const char *src );
IMPORT char	*knl_strncpy( char *dst, const char *src, SZ n );
IMPORT int	knl_strcmp( const char *s1, const char *s2 );
IMPORT char	*knl_strcat( char *dst, const char *src );

#endif /* _TSTDLIB_ */
