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

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7A

/*
 *	wusec_armv7a.c
 *
 *	Micro Wait: Busy loop wait time in micro-sec (ARMv7-A Core)
 */

#include <tk/tkernel.h>
#include <config.h>

LOCAL void wait_us( UW usec )
{
	UW	max, start, end, cur, period, prev;

	period	= (((usec)*COUNT_PER_SEC)/1000000LL);	/* Number of wait counts */
	max	= in_w(OSTM0_CMP);			/* Timer count cycle */
	if (period >= max) return;			/* ERROR: Undetectable */

	start	= in_w(OSTM0_CNT);		/* Count value at the start of waiting */
	if (start > period) {
		end = (start - period);			/* Count value when waiting time is completed */
	}
	else {
		end = max - period + start;
		prev = start;
		while (1) {		/* Waiting for underflow */
			cur = in_w(OSTM0_CNT);
			if (cur > prev) break;
			prev = cur;
		}
		start = max;
	}
	do {
		cur = in_w(OSTM0_CNT);
	} while (!((cur <= end) || (cur > start)));
}

/* maximum time (in microseconds) that wait_us() can handle at a time */
#define WAIT_US_STEP 10000

EXPORT void WaitUsec( UW usec )
{
	for ( ; usec >= WAIT_US_STEP; usec -= WAIT_US_STEP ) {
		wait_us(WAIT_US_STEP);
	}
	wait_us(usec);
}

EXPORT void WaitNsec( UW nsec )
{
	for ( ; nsec >= (WAIT_US_STEP * 1000); nsec -= (WAIT_US_STEP * 1000) ) {
		wait_us(WAIT_US_STEP);
	}
	wait_us(nsec / 1000 + 1);
}

#endif /* CPU_CORE_ARMV7A */
