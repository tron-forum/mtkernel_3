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
 *	longlong.h
 *
 *	64 bit length integer 
 */

#ifndef _LONGLONG_
#define _LONGLONG_

#if TK_HAS_DOUBLEWORD

#define ltoll(a)	( (D)(a) )
#define ultoll(a)	( (D)(a) )
#define uitoll(a)	( (D)(a) )
#define lltol(a)	( (long)(a) )
#define lltoul(a)	( (unsigned long)(a) )
#define ll_add(a,b)	( (a) + (b) )
#define ll_sub(a,b)	( (a) - (b) )
#define ll_mul(a,b)	( (a) * (b) )
#define li_mul(a,b)	( (a) * (b) )
#define lui_mul(a,b)	( (a) * (b) )
#define ll_div(a,b)	( (a) / (b) )
#define li_div(a,b)	( (a) / (b) )
#define lui_div(a,b)	( (a) / (b) )
#define ll_mod(a,b)	( (a) % (b) )
#define li_mod(a,b)	( (a) % (b) )
#define lui_mod(a,b)	( (a) % (b) )
#define ll_cmp(a,b)	( (a) - (b) )	/* +:a>b,0:a=b,-:a<b */
#define ll_sign(a)	( (a) )		/* +:a>0,0:a=0,-:a<0 */
#define ll_neg(a)	( -(a) )
#define ll_inc(a)	( (*(a))++ )
#define ll_dec(a)	( (*(a))-- )

#define hilo_ll(ll, h, l)	( (ll) = ((D)(h) << 32) | (l) )
#define ll_hilo(h, l, ll)	( (h) = (W)((ll) >> 32), \
				  (l) = (UW)(ll) )

#else /* TK_HAS_DOUBLEWORD */

typedef struct {
	long		hi;
	unsigned long	lo;
} longlong;

extern longlong	ltoll( long a );			/* (longlong)a */
extern longlong	ultoll( unsigned long a );		/* (longlong)a */
extern longlong	uitoll( UINT a );			/* (longlong)a */
extern long	lltol( longlong a );			/* (long)a */
extern unsigned long	lltoul( longlong a );	/* (unsigned long)a */
extern longlong	ll_add( longlong a, longlong b );	/* a+b */
extern longlong	ll_sub( longlong a, longlong b );	/* a-b */
extern longlong	ll_mul( longlong a, longlong b );	/* a*b */
extern longlong	li_mul( longlong a, long b );		/* a*b */
extern longlong	lui_mul( longlong a, UINT b );		/* a*b */
extern longlong	ll_div( longlong a, longlong b );	/* a/b */
extern longlong	li_div( longlong a, long b );		/* a/b */
extern longlong	lui_div( longlong a, UINT b );		/* a/b */
extern longlong	ll_mod( longlong a, longlong b );	/* a%b */
extern long	li_mod( longlong a, long b );		/* a%b */
extern long	lui_mod( longlong a, UINT b );		/* a%b */
extern int	ll_cmp( longlong a, longlong b );	/* +:a>b,0:a=b,-:a<b */
extern int	ll_sign( longlong a );			/* +:a>0,0:a=0,-:a<0 */
extern longlong ll_neg( longlong a );			/* -a */
extern void	ll_inc( longlong *a );			/* (*a)++ */
extern void	ll_dec( longlong *a );			/* (*a)-- */

#define hilo_ll(ll, h, l)	( (ll).hi = (h), (ll).lo = (l) )
#define ll_hilo(h, l, ll)	( (h) = (ll).hi, (l) = (ll).lo )

#endif /* TK_HAS_DOUBLEWORD */

#endif /* _LONGLONG_ */
