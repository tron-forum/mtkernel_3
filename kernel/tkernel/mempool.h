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
 *	mempool.h
 *	Variable Size Memory Pool
 */

#ifndef _MEMPOOL_H_
#define _MEMPOOL_H_

/*
 * Variable size memory pool control block
 *	'areaque' connects memory blocks in address ascending order
 *	'freeque' connects memory blocks in size increasing order
 *
 *  Order of members must not be changed because a part of members
 *  are used with casting to IMACB.
 */
typedef struct memorypool_control_block {
	QUEUE	wait_queue;	/* Memory pool wait queue */
	ID	mplid;		/* Variable size memory pool ID */
	void	*exinf;		/* Extended information */
	ATR	mplatr;		/* Memory pool attribute */
	W	mplsz;		/* Whole memory pool size */
	QUEUE	areaque;	/* Queue connecting all blocks */
	QUEUE	freeque;	/* Queue connecting free blocks */
	QUEUE	areaque_end;	/* the last element of areaque */
	void	*mempool;	/* Top address of memory pool */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} MPLCB;

IMPORT MPLCB knl_mplcb_table[];	/* Variable size memory pool control block */
IMPORT QUEUE knl_free_mplcb;	/* FreeQue */

#define get_mplcb(id)	( &knl_mplcb_table[INDEX_MPL(id)] )


/*
 * Maximum free area size
 */
Inline W knl_MaxFreeSize( MPLCB *mplcb )
{
	if ( isQueEmpty(&mplcb->freeque) ) {
		return 0;
	}
	return FreeSize(mplcb->freeque.prev);
}

/*
 * Definition of variable size memory pool wait specification
 */
IMPORT void knl_mpl_wakeup( MPLCB *mplcb );

#endif /* _MEMPOOL_H_ */
