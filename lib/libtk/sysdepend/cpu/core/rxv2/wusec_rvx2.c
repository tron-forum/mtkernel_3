/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_RXV2

/*
 *	waitusec.c
 *
 *	Micro Wait: Busy loop wait time in micro-sec (RXv2 Core)
 */

#include <tk/tkernel.h>

LOCAL void wait_us( UW usec )
{
	UW	max, pre, cur, ofs, rem;

	max = *(_UH*)(CMT0_COR) + 1;
	rem = max * usec / (TIMER_PERIOD * 1000);

	cur = *(_UH*)CMT0_CNT;

	for ( ;; ) {
		pre = cur;
		cur = *(_UH*)CMT0_CNT;

		ofs = (pre <= cur) ? (cur - pre) : (max + cur - pre);
		if ( ofs >= rem ) {
			break;
		}
		rem -= ofs;
	}
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

#endif /* CPU_CORE_RXV2 */
