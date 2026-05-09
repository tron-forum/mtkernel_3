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
 *	dbgspt_depend.h
 *
 *	micro T-Kernel Debugger Support  Definition (RTv2 core depended)
 */

#ifndef __TK_DBGSPT_DEPEND_CORE_H__
#define __TK_DBGSPT_DEPEND_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * System call/extension SVC caller information 
 */
typedef struct td_calinf {
	void	*ssp;		/* System stack pointer */
} TD_CALINF;


#ifdef __cplusplus
}
#endif
#endif /* __TK_DBGSPT_DEPEND_CORE_H__ */
