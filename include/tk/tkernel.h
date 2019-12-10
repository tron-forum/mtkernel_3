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
 *	tkernel.h
 *
 *	T-Kernel Definition
 */

#ifndef __TK_TKERNEL_H__
#define __TK_TKERNEL_H__

#include <sys/machine.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "typedef.h"
#include "errno.h"

#include <sys/sysdef.h>
#include <sys/profile.h>

#include "syscall.h"
#include "syslib.h"

#ifndef offsetof
#define offsetof(type, member) ((SZ)(&((type *)0)->member))
#endif

#ifdef __cplusplus
}
#endif

#endif /* __TK_TKERNEL_H__ */
