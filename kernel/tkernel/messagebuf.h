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
 *	messagebuf.h
 *	Message Buffer
 */

#ifndef _MESSAGEBUF_H_
#define _MESSAGEBUF_H_

/*
 * Message buffer control block
 *
 *	Because Receive wait task (TTW_MBF) and Send wait task (TTW_SMBF)
 *	do not co-exist for one message buffer, the wait queue may be
 *	allowed to share.
 *	However, when the size of message buffer is 0, it is difficult
 *	to judge the wait queue if it is for receive or send, 
 *	therefore do not use this method.
 */
typedef struct messagebuffer_control_block {
	QUEUE	send_queue;	/* Message buffer send wait queue */
	ID	mbfid;		/* message buffer ID */
	void	*exinf;		/* Extended information */
	ATR	mbfatr;		/* Message buffer attribute */
	QUEUE	recv_queue;	/* Message buffer receive wait queue */
	W	bufsz;		/* Message buffer size */
	INT	maxmsz;		/* Maximum length of message */
	W	frbufsz;	/* Free buffer size */
	W	head;		/* First message store address */
	W	tail;		/* Next to the last message store address */
	VB	*buffer;	/* Message buffer address */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} MBFCB;

IMPORT MBFCB knl_mbfcb_table[];	/* Message buffer control block */
IMPORT QUEUE knl_free_mbfcb;	/* FreeQue */

#define get_mbfcb(id)	( &knl_mbfcb_table[INDEX_MBF(id)] )


/*
 * Message header format
 */
typedef INT		HEADER;
#define HEADERSZ	(sizeof(HEADER))

#define ROUNDSIZE	(sizeof(HEADER))
#define ROUNDSZ(sz)	(((UW)(sz) + (UW)(ROUNDSIZE-1)) & ~(UW)(ROUNDSIZE-1))

/*
 * Check message buffer free space
 *	If 'msgsz' message is able to be stored, return TRUE.
 */
Inline BOOL knl_mbf_free( MBFCB *mbfcb, INT msgsz )
{
	return ( HEADERSZ + (UW)msgsz <= (UW)mbfcb->frbufsz );
}

/*
 * If message buffer is empty, return TRUE.
 */
Inline BOOL knl_mbf_empty( MBFCB *mbfcb )
{
	return ( mbfcb->frbufsz == mbfcb->bufsz );
}

#endif /* _MESSAGEBUF_H_ */
