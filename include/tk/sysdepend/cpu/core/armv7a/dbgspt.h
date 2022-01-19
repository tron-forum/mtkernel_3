/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	dbgspt_depend.h
 *
 *	micro T-Kernel Debugger Support  Definition (ARMv7-A core depended)
 */

#ifndef __TK_DBGSPT_DEPEND_CORE_H__
#define __TK_DBGSPT_DEPEND_CORE_H__

/*
 * System call/extension SVC caller information 
 */
typedef struct td_calinf {
	void	*sp;		/* stack pointer when calling */
	void	*pc;		/* program counter when calling */
} TD_CALINF;

#endif /* __TK_DBGSPT_DEPEND_CORE_H__ */
