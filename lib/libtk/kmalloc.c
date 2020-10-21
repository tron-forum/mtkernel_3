/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.02
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21 .
 *
 *----------------------------------------------------------------------
 */

/*
 *	kmalloc.c
 *
 *	Memory allocation (Kmalloc)
 */

#include <tk/tkernel.h>

#if TK_SUPPORT_MEMLIB

/* --------------------------------------------------------------------- */
/*
 * Systmem internal memory management call 
 * 	These calls must not be called directly from the user program.
 */
IMPORT void* knl_Imalloc( SZ size );
IMPORT void* knl_Icalloc( SZ nmemb, SZ size );
IMPORT void* knl_Irealloc( void *ptr, SZ size );
IMPORT void  knl_Ifree( void *ptr );


/* --------------------------------------------------------------------- */
/*
 * Memory allocation API definition
 */
EXPORT void* Kmalloc( size_t size )
{
	return knl_Imalloc(size);
}

EXPORT void* Kcalloc( size_t nmemb, size_t size )
{
	return knl_Icalloc(nmemb, size);
}

EXPORT void *Krealloc( void *ptr, size_t size)
{
	return knl_Irealloc(ptr, size);
}

EXPORT void Kfree( void *ptr )
{
	knl_Ifree(ptr);
}

#endif /* TK_SUPPORT_MEMLIB */
