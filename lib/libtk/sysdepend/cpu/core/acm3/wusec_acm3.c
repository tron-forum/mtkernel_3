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
#include <sys/machine.h>
#ifdef CPU_CORE_ACM3

/*
 *	waitusec.c
 *
 *	Micro Wait: Busy loop wait time in micro-sec (ARM Cortex-M3)
 */

#include <tk/tkernel.h>
#include <config.h>

LOCAL void wait_us( UW usec )
{
	UW	max, pre, cur, ofs, rem;

	max = in_w(SYST_RVR);
	rem = max * usec / (TIMER_PERIOD * 1000) + 1;

	cur = in_w(SYST_CVR) & 0x00ffffff;

	for ( ;; ) {
		pre = cur;
		cur = in_w(SYST_CVR) & 0x00ffffff;

		ofs = (pre >= cur) ? (pre - cur) : (pre + max - cur);
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

#endif /* CPU_CORE_ACM3 */
