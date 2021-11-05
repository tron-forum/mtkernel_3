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
 *	syslib.h
 *
 *	micro T-Kernel System Library
 */


#ifndef __TK_SYSLIB_H__
#define __TK_SYSLIB_H__

#include "typedef.h"


/* ------------------------------------------------------------------------ */
/*
 * System dependencies  (CPU Intrrupt contrl , I/O port access)
 */
#define SYSLIB_PATH_(a)		#a
#define SYSLIB_PATH(a)		SYSLIB_PATH_(a)
#define SYSLIB_SYSDEP()		SYSLIB_PATH(sysdepend/TARGET_DIR/syslib.h)
#include SYSLIB_SYSDEP()


/*----------------------------------------------------------------------*/
/*
 * Interrupt controller Control
 *
 */
#if TK_SUPPORT_INTCTRL
#if TK_HAS_ENAINTLEVEL
IMPORT void EnableInt( UINT intno, INT level );	/* Enable the interrupt specified by 'intno.' */
#else
IMPORT void EnableInt( UINT intno );		/* Enable the interrupt specified by 'intno.' */
#endif /* TK_HAS_ENAINTLEVEL */

IMPORT void DisableInt( UINT intno );	/* Disable the interrupt specified by 'intno.' */
IMPORT void ClearInt(UINT intno);	/* Clear the state that the specified interrupt has been asserted. */
IMPORT void EndOfInt(UINT intno);	/* Issue EOI to Interrupt Controller. */
IMPORT BOOL CheckInt( UINT intno );	/* Check active state for the associated interrupt */

#endif /* TK_SUPPORT_INTCTRL */

#if TK_SUPPORT_INTMODE
IMPORT void SetIntMode(UINT intno, UINT mode);	/* Set interrupt mode */
#endif /* TK_SUPPORT_INTMODE */

#if TK_SUPPORT_CPUINTLEVEL
IMPORT void SetCpuIntLevel( INT level );
IMPORT INT GetCpuIntLevel( void );
#endif /* TK_SUPPORT_CPUINTLEVEL */

#if TK_SUPPORT_CTRLINTLEVEL
IMPORT void SetCtrlIntLevel(INT level);	/* Set interrupt mask level in interrupt controller */
IMPORT INT  GetCtrlIntLevel(void);	/* Get interrupt mask level in interrupt controller */
#endif /* TK_SUPPORT_CTRLINTLEVEL */


/* ------------------------------------------------------------------------ */
/*
 * Micro Wait
 */

IMPORT void WaitUsec( UW usec );	/* micro second wait */
IMPORT void WaitNsec( UW nsec );	/* nano second wait */


/* ------------------------------------------------------------------------ */
/*
 * Fast Lock
 */
typedef struct {
	INT		cnt;
	ID		id;
	CONST UB	*name;
} FastLock;

IMPORT ER CreateLock( FastLock *lock, CONST UB *name );
IMPORT void DeleteLock( FastLock *lock );
IMPORT void Lock( FastLock *lock );
IMPORT void Unlock( FastLock *lock );


/* ------------------------------------------------------------------------ */
/*
 * Multi Lock
 *	Can use the maximum of 16 or 32 independent locks with a single FastMLock.
 *	Divided by the lock number (no). Can specify 0-15 or 0-31 for 'no.'
 *	(Slightly less efficient than FastLock)
 */
typedef struct {
	UINT		flg;
	INT		wai;
	ID		id;
	CONST UB	*name;
} FastMLock;

IMPORT ER CreateMLock( FastMLock *lock, CONST UB *name );
IMPORT ER DeleteMLock( FastMLock *lock );
IMPORT ER MLockTmo( FastMLock *lock, INT no, TMO tmout );
IMPORT ER MLock( FastMLock *lock, INT no );
IMPORT ER MUnlock( FastMLock *lock, INT no );


/* ------------------------------------------------------------------------ */
/*
 * Memory allocation
 */
#if TK_SUPPORT_MEMLIB

#ifndef PROHIBIT_DEF_SIZE_T
typedef SZ		size_t;
#endif

IMPORT void *Kmalloc( size_t size );
IMPORT void *Kcalloc( size_t nmemb, size_t size );
IMPORT void *Krealloc( void *ptr, size_t size);
IMPORT void Kfree( void *ptr );

#endif /* TK_SUPPORT_MEMLIB */


/* ------------------------------------------------------------------------ */
/*
 * Physical timer
 */
#if TK_SUPPORT_PTIMER

#define TA_ALM_PTMR	0
#define TA_CYC_PTMR	1

typedef struct {
	void	*exinf;		/* Extended Information */
	ATR	ptmratr;	/* Physical Timer Attribute */
	FP	ptmrhdr;	/* Physical Timer Handler Address */
} T_DPTMR;

typedef struct {
	UW	ptmrclk;	/* Physical Timer Clock Frequency */
	UW	maxcount;	/* Maximum Count */
	BOOL	defhdr;		/* Handler Support */
} T_RPTMR;

IMPORT ER StartPhysicalTimer( UINT ptmrno, UW limit, UINT mode);
IMPORT ER StopPhysicalTimer( UINT ptmrno );
IMPORT ER GetPhysicalTimerCount( UINT ptmrno, UW *p_count );
IMPORT ER DefinePhysicalTimerHandler( UINT ptmrno, CONST T_DPTMR *pk_dptmr );
IMPORT ER GetPhysicalTimerConfig(UINT ptmrno, T_RPTMR *pk_rptmr);

#endif /* TK_SUPPORT_PTIMER */


/* ------------------------------------------------------------------------ */
/*
 * 4-character object name
 *	(Example)
 *	T_CTSK	ctsk;
 *	SetOBJNAME(ctsk.exinf, "TEST");
 */
union objname {
	char	s[4];
	void	*i;
};

#define SetOBJNAME(exinf, name)					\
	{							\
		UB *d, *s; INT i;				\
		d = (UB*)&(exinf);				\
		s = (UB*)name;					\
		for(i=0; i<4; i++) *d++ = *s++;			\
	}

#endif /* __TK_SYSLIB_H__ */
