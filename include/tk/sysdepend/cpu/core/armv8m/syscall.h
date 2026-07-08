/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07.
 *
 *----------------------------------------------------------------------
 */
/*
 *	syscall.h
 *
 *	CPU dependent definition  (ARMv8-M)
 */
#ifndef __TK_SYSCALL_CORE_H__
#define __TK_SYSCALL_CORE_H__

#if TRUSTZONE_SCALL

/*  Task attributes */
#define TA_TZCALL	0x00010000U		/* Use TrustZone secure functions */
#define	TA_TSKSYDEP	(TA_TZCALL)

/* Task creation information */
#define CTSK_SYSDEPEND_INFO	SZ tzstksz;	/* TrustZone secure stack size */

#else	/* TRUSTZONE_SCALL */

#define	TA_TSKSYDEP	0

#endif /* TRUSTZONE_SCALL */
#endif /* __TK_SYSCALL_CORE_H__ */