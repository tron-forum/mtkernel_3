/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */

/*
 *	libtm_printf.c
 *
 *	printf() / sprintf() T-Monitor compatible calls.
 *
 *	- Unsupported specifiers: floating point, long long and others.
 *		Coversion:	 a, A, e, E, f, F, g, G, n
 *		Size qualifier:  hh, ll, j, z, t, L
 *	- No limitation of output string length.
 *	- Minimize stack usage.
 *		Depending on available stack size, define TM_OUTBUF_SZ by
 *		appropriate value.
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#if USE_TMONITOR
#include "libtm.h"

#if USE_TM_PRINTF
#include <stdarg.h>

/* Output function */
typedef	struct {
	H	len;		/* Total output length */
	H	cnt;		/* Buffer counts */
	UB	*bufp;		/* Buffer pointer for tm_sprintf */
} OutPar;
typedef	void	(*OutFn)( UB *str, INT len, OutPar *par );

/*
 *	Output integer value
 */
LOCAL	UB	*outint( UB *ep, UW val, UB base )
{
LOCAL const UB  digits[32] = "0123456789abcdef0123456789ABCDEF";
	UB	caps;

	caps = (base & 0x40) >> 2;		/* 'a' or 'A' */
	for (base &= 0x3F; val >= base; val /= base) {
		*--ep = digits[(val % base) + caps];
	}
	*--ep = digits[val + caps];
	return ep;				/* buffer top pointer */
}

/*
 *	Output with format (limitted version)
 */
LOCAL	void	tm_vsprintf( OutFn ostr, OutPar *par, const UB *fmt, va_list ap )
{
#define	MAX_DIGITS	14
	UW		v;
	H		wid, prec, n;
	UB		*fms, *cbs, *cbe, cbuf[MAX_DIGITS];
	UB		c, base, flg, sign, qual;

/* flg */
#define	F_LEFT		0x01
#define	F_PLUS		0x02
#define	F_SPACE		0x04
#define	F_PREFIX	0x08
#define	F_ZERO		0x10

	for (fms = NULL; (c = *fmt++) != '\0'; ) {

		if (c != '%') {	/* Fixed string */
			if (fms == NULL) fms = (UB*)fmt - 1;
			continue;
		}

		/* Output fix string */
		if (fms != NULL) {
			(*ostr)(fms, fmt - fms - 1, par);
			fms = NULL;
		}

		/* Get flags */
		for (flg = 0; ; ) {
			switch (c = *fmt++) {
			case '-': flg |= F_LEFT;	continue;
			case '+': flg |= F_PLUS;	continue;
			case ' ': flg |= F_SPACE;	continue;
			case '#': flg |= F_PREFIX;	continue;
			case '0': flg |= F_ZERO;	continue;
			}
			break;
		}

		/* Get field width */
		if (c == '*') {
			wid = va_arg(ap, INT);
			if (wid < 0) {
				wid = -wid;
				flg |= F_LEFT;
			}
			c = *fmt++;
		} else {
			for (wid = 0; c >= '0' && c <= '9'; c = *fmt++)
				wid = wid * 10 + c - '0';
		}

		/* Get precision */
		prec = -1;
		if (c == '.') {
			c = *fmt++;
			if (c == '*') {
				prec = va_arg(ap, INT);
				if (prec < 0) prec = 0;
				c = *fmt++;
			} else {
				for (prec = 0;c >= '0' && c <= '9';c = *fmt++)
					prec = prec * 10 + c - '0';
			}
			flg &= ~F_ZERO;		/* No ZERO padding */
		}

		/* Get qualifier */
		qual = 0;
		if (c == 'h' || c == 'l') {
			qual = c;
			c = *fmt++;
		}

		/* Format items */
		base = 10;
		sign = 0;
		cbe = &cbuf[MAX_DIGITS];	/* buffer end pointer */

		switch (c) {
		case 'i':
		case 'd':
		case 'u':
		case 'X':
		case 'x':
		case 'o':
			if (qual == 'l') {
				v = va_arg(ap, UW);
			} else {
				v = va_arg(ap, UINT);
				if (qual == 'h') {
					v = (c == 'i' || c == 'd') ?
						(H)v :(UH)v;
				}
			}
			switch (c) {
			case 'i':
			case 'd':
				if ((W)v < 0) {
					v = - (W)v;
					sign = '-';
				} else if ((flg & F_PLUS) != 0) {
					sign = '+';
				} else if ((flg & F_SPACE) != 0) {
					sign = ' ';
				} else {
					break;
				}
				wid--;		/* for sign */
			case 'u':
				break;
			case 'X':
				base += 0x40;	/* base = 16 + 0x40 */
			case 'x':
				base += 8;	/* base = 16 */
			case 'o':
				base -= 2;	/* base = 8 */
				if ((flg & F_PREFIX) != 0 && v != 0) {
					wid -= (base == 8) ? 1 : 2;
					base |= 0x80;
				}
				break;
			}
			/* Note: None outputs when v == 0 && prec == 0 */
			cbs = (v == 0 && prec == 0) ?
						cbe : outint(cbe, v, base);
			break;
		case 'p':
			v = (UW)va_arg(ap, void *);
			if (v != 0) {
				base = 16 | 0x80;
				wid -= 2;
			}
			cbs = outint(cbe, v, base);
			break;
		case 's':
			cbe = cbs = va_arg(ap, UB *);
			if (prec < 0) {
				while (*cbe != '\0') cbe++;
			} else {
				while (--prec >= 0 && *cbe != '\0') cbe++;
			}
			break;
		case 'c':
			cbs = cbe;
			*--cbs = (UB)va_arg(ap, INT);
			prec = 0;
			break;
		case '\0':
			fmt--;
			continue;
		default:
			/* Output as fixed string */
			fms = (UB*)fmt - 1;
			continue;
		}

		n = cbe - cbs;				/* item length */
		if ((prec -= n) > 0) n += prec;
		wid -= n;				/* pad length */

		/* Output preceding spaces */
		if ((flg & (F_LEFT | F_ZERO)) == 0 ) {
			while (--wid >= 0) (*ostr)((UB*)" ", 1, par);
		}

		/* Output sign */
		if (sign != 0) {
			(*ostr)(&sign, 1, par);
		}

		/* Output prefix "0x", "0X" or "0" */
		if ((base & 0x80) != 0) {
			(*ostr)((UB*)"0", 1, par);
			if ((base & 0x10) != 0) {
				(*ostr)((base & 0x40) ? (UB*)"X" : (UB*)"x", 1, par);
			}
		}

		/* Output preceding zeros for precision or padding */
		if ((n = prec) <= 0) {
			if ((flg & (F_LEFT | F_ZERO)) == F_ZERO ) {
				n = wid;
				wid = 0;
			}
		}
		while (--n >= 0) (*ostr)((UB*)"0", 1, par);

		/* Output item string */
		(*ostr)(cbs, cbe - cbs, par);

		/* Output tailing spaces */
		while (--wid >= 0) (*ostr)((UB*)" ", 1, par);
	}

	/* Output last fix string */
	if (fms != NULL) {
		(*ostr)(fms, fmt - fms - 1, par);
	}
#if	TM_OUTBUF_SZ > 0
	/* Flush output */
	(*ostr)(NULL, 0, par);
#endif
}

/*
 *	Output to console
 */
LOCAL	void	out_cons( UB *str, INT len,  OutPar *par )
{
#if	TM_OUTBUF_SZ == 0
	/* Direct output to console */
	par->len += len;
	while (--len >= 0) tm_putchar(*str++);
#else
	/* Buffered output to console */
	if (str == NULL) {	/* Flush */
		if (par->cnt > 0) {
			par->bufp[par->cnt] = '\0';
			tm_putstring(par->bufp);
			par->cnt = 0;
		}
	} else {
		par->len += len;
		while (--len >= 0) {
			if (par->cnt >= TM_OUTBUF_SZ - 1) {
				par->bufp[par->cnt] = '\0';
				tm_putstring(par->bufp);
				par->cnt = 0;
			}
			par->bufp[par->cnt++] = *str++;
		}
	}
#endif
}

EXPORT INT	tm_printf( const UB *format, ... )
{
	va_list	ap;

#if	TM_OUTBUF_SZ == 0
	H	len = 0;

	va_start(ap, format);
	tm_vsprintf(out_cons, (OutPar*)&len, format, ap);
	va_end(ap);
	return len;
#else
	UB	obuf[TM_OUTBUF_SZ];
	OutPar	par;

	par.len = par.cnt = 0;
	par.bufp = obuf;
	va_start(ap, format);
	tm_vsprintf(out_cons, (OutPar*)&par, format, ap);
	va_end(ap);
	return par.len;
#endif
}

/*
 *	Output to buffer
 */
LOCAL	void	out_buf( UB *str, INT len, OutPar *par )
{
	par->len += len;
	while (--len >= 0) *(par->bufp)++ = *str++;
}

EXPORT INT	tm_sprintf( UB *str, const UB *format, ... )
{
	OutPar	par;
	va_list	ap;

	par.len = 0;
	par.bufp = str;
	va_start(ap, format);
	tm_vsprintf(out_buf, &par, format, ap);
	va_end(ap);
	str[par.len] = '\0';
	return par.len;
}

#else
EXPORT INT	tm_printf( const UB *format, ... )
{
	return (-1);
}

EXPORT INT	tm_sprintf( UB *str, const UB *format, ... )
{
	return (-1);
}

#endif /* USE_TM_PRINTF */
#endif /* USE_TMONITOR */