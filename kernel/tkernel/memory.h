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
 *	memory.h
 *	In-kernel dynamic memory management
 */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "limits.h"

/*
 * Memory allocation management information
 *
 *  Order of members must not be changed because members are used
 *  with casting from MPLCB.
 */
typedef struct {
	W		memsz;

	/* AreaQue for connecting each area where reserved pages are
	   divided Sort in ascending order of addresses in a page.
	   Do not sort between pages. */
	QUEUE		areaque;
	/* FreeQue for connecting unused area in reserved pages
	   Sort from small to large free spaces. */
	QUEUE		freeque;
} IMACB;

/*
 * Compensation for aligning "&areaque" position to 2 bytes border
 */
#define AlignIMACB(imacb)	( (IMACB*)((UW)(imacb) & ~0x00000001UL) )

/*
 * Minimum unit of subdivision
 *	The lower 1 bit of address is always 0
 *	because memory is allocated by ROUNDSZ.
 *	AreaQue uses the lower 1 bit for flag.
 */
#define ROUNDSZ		( sizeof(QUEUE) )	/* 8 bytes */
#define ROUND(sz)	( ((UW)(sz) + (UW)(ROUNDSZ-1)) & ~(UW)(ROUNDSZ-1) )

/* Minimum fragment size */
#define MIN_FRAGMENT	( sizeof(QUEUE) * 2 )

/*
 * Maximum allocatable size (to check for parameter)
 */
#define	MAX_ALLOCATE	( INT_MAX & ~(ROUNDSZ-1) )

/*
 * Adjusting the size which can be allocated 
 */
Inline W roundSize( W sz )
{
	if ( sz < (W)MIN_FRAGMENT ) {
		sz = (W)MIN_FRAGMENT;
	}
	return (W)(((UW)sz + (UW)(ROUNDSZ-1)) & ~(UW)(ROUNDSZ-1));
}


/*
 * Flag that uses the lower bits of AreaQue's 'prev'.
 */
#define AREA_USE	0x00000001UL	/* In-use */
#define AREA_MASK	0x00000001UL

#define setAreaFlag(q, f)   ( (q)->prev = (QUEUE*)((UW)(q)->prev |  (UW)(f)) )
#define clrAreaFlag(q, f)   ( (q)->prev = (QUEUE*)((UW)(q)->prev & ~(UW)(f)) )
#define chkAreaFlag(q, f)   ( ((UW)(q)->prev & (UW)(f)) != 0 )

#define Mask(x)		( (QUEUE*)((UW)(x) & ~AREA_MASK) )
#define Assign(x, y)	( (x) = (QUEUE*)(((UW)(x) & AREA_MASK) | (UW)(y)) )
/*
 * Area size
 */
#define AreaSize(aq)	( (VB*)(aq)->next - (VB*)((aq) + 1) )
#define FreeSize(fq)	( (W)((fq) + 1)->prev )


IMPORT QUEUE* knl_searchFreeArea( IMACB *imacb, W blksz );
IMPORT void knl_appendFreeArea( IMACB *imacb, QUEUE *aq );
IMPORT void knl_removeFreeQue( QUEUE *fq );
IMPORT void knl_insertAreaQue( QUEUE *que, QUEUE *ent );
IMPORT void knl_removeAreaQue( QUEUE *aq );

IMPORT IMACB *knl_imacb;

#endif /* _MEMORY_H_ */
