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
 *	mailbox.h
 *	Mailbox
 */

#ifndef _MAILBOX_H_
#define _MAILBOX_H_
/*
 * Mailbox control block
 *
 *	'mq_head' is the first message queue pointer that
 *	points a message.
 *	It is NULL if the message queue is empty.
 *	'mq_tail' is a pointer that points end of message
 *	queue that is not empty.
 *	The message queue value is not guaranteed if the
 *	message queue is empty.
 *	It is used only if the message queue is FIFO (TA_MFIFO).
 */
typedef struct mailbox_control_block {
	QUEUE	wait_queue;	/* Mailbox wait queue */
	ID	mbxid;		/* Mailbox ID */
	void	*exinf;		/* Extended information */
	ATR	mbxatr;		/* Mailbox attribute */
	T_MSG	mq_head;	/* Head of message queue */
	T_MSG	*mq_tail;	/* End of message queue */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} MBXCB;

IMPORT MBXCB knl_mbxcb_table[];	/* Mailbox control block */
IMPORT QUEUE knl_free_mbxcb;	/* FreeQue */

#define get_mbxcb(id)	( &knl_mbxcb_table[INDEX_MBX(id)] )

/*
 * Head message
 */
#define headmsg(mbxcb)	( (mbxcb)->mq_head.msgque[0] )

/*
 * Next message
 */
#define nextmsg(msg)	( ((T_MSG*)(msg))->msgque[0] )

/*
 * Insert a message queue following priority
 */
Inline void knl_queue_insert_mpri( T_MSG_PRI *pk_msg, T_MSG *head )
{
	T_MSG_PRI	*msg;
	T_MSG		*prevmsg = head;

	while ( (msg = (T_MSG_PRI*)nextmsg(prevmsg)) != NULL ) {
		if ( msg->msgpri > pk_msg->msgpri ) {
			break;
		}
		prevmsg = (T_MSG*)msg;
	}
	nextmsg(pk_msg) = msg;
	nextmsg(prevmsg) = pk_msg;
}

#endif /* _MAILBOX_H_ */
