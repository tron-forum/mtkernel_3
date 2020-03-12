/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00.B1
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/03/13.
 *
 *----------------------------------------------------------------------
 */

/*
 *	dbgspt_depend.h
 *
 *	micro T-Kernel Debugger Support  Definition (RTv2 core depended)
 */

#ifndef __TK_DBGSPT_DEPEND_CORE_H__
#define __TK_DBGSPT_DEPEND_CORE_H__

/*
 * System call/extension SVC caller information 
 */
typedef struct td_calinf {
	void	*ssp;		/* System stack pointer */
} TD_CALINF;

#endif /* __TK_DBGSPT_DEPEND_CORE_H__ */
