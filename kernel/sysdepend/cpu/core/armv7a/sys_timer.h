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
 *	sys_timer.h (ARMv7-A)
 *	Hardware-Dependent System Timer (SysTick) Processing
 */

#ifndef _SYSDEPEND_CPU_CORE_SYSTIMER_
#define _SYSDEPEND_CPU_CORE_SYSTIMER_

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
Inline void knl_start_hw_timer( void )
{
	UINT	imask;
	UD	cnt;

	DI(imask);

	/* Convert microseconds to timer values */
	cnt = (((TIMER_PERIOD * 1000) * COUNT_PER_SEC) / 1000000LL);

	out_b(OSTM0_TT, 0x01);		/* Stop timer */
	out_b(OSTM0_CTL, 0x01);		/* Set Interval timer mode & Interrupt enabled */
	out_w(OSTM0_CMP, (UW)cnt);	/* Set compare register */
	out_b(OSTM0_TS, 0x01);		/* Start Timer */

	EnableInt(INTNO_SYSTICK, INTPRI_SYSTICK);	/* Enable interrupt(INTC level) */

	EI(imask);
}

/*
 * Clear timer interrupt
 *	Clear the timer interrupt request. Depending on the type of
 *	hardware, there are two timings for clearing: at the beginning
 *	and the end of the interrupt handler.
 *	'clear_hw_timer_interrupt()' is called at the beginning of the
 *	timer interrupt handler.
 *	'end_of_hw_timer_interrupt()' is called at the end of the timer
 *	interrupt handler.
 *	Use either or both according to hardware.
 */
Inline void knl_clear_hw_timer_interrupt( void )
{
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
	disint();	/* disint() in front of the EOI to prevent inadvertent interrupts. */
	EndOfInt(INTNO_SYSTICK);
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void knl_terminate_hw_timer( void )
{
	out_b(OSTM0_TT, 0x01);		/* Stop Timer */
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < CFN_TIMER_PERIOD
 */
Inline UW knl_get_hw_timer_nsec( void )
{
	UW	            ofs, max;

	max = in_w(OSTM0_CMP);		/* Timer cycle value */
	ofs = in_w(OSTM0_CNT);		/* Current timer value (Down count) */

	return ((max - ofs) * NSEC_PER_COUNT);
}

#endif /* _SYSDEPEND_CPU_CORE_SYSTIMER_ */
