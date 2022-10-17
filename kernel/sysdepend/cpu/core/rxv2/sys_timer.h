/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2006-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sys_timer.h (RXv2)
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

	/* set Timer Count */
	out_h(CMT0_COR, SYSCLK_ICLK/PCLK_DIV/1000*TIMER_PERIOD/CMCR_DIV - 1);

	/* Interrupt is Enable, Set Frequency Dividing */
	out_h(CMT0_CR, CMT0_CR_CMIE | CMCR_CKS_PLCK);

	/* CMI0 Interrupt Enable, Interrupt Level is 15. */
	EnableInt( INTNO_SYS_TICK , INTLEVEL_SYS_TICK);

	/* Start timer count */
	out_h(CMT_STR0, CMT_STR0_STR0);
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
	/* Nothing required to do at this point */
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
	/* Nothing required to do at this point */
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void knl_terminate_hw_timer( void )
{
	UH	cmstr0;

	DisableInt( INTNO_SYS_TICK );	/* Disable timer interrupt */

	out_h(CMT0_CR, 0);		/* Disable compare match interrupt */
	cmstr0 = in_h(CMT_STR0) & ~(CMT_STR0_STR0);
	out_h(CMT_STR0, cmstr0);	/* Stop timer count */
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < TIMER_PERIOD * 2
 */
Inline UW knl_get_hw_timer_nsec( void )
{
	UW	ofs, max;
	UB	unf;
	UINT	imask;

	DI(imask);

	max = in_h(CMT0_COR) + 1;			/* Timer Count */
	do {
		unf = CheckInt(INTNO_SYS_TICK);		/* Get Interrupt Status */
		ofs = in_h(CMT0_CNT);			/* Current Count */
	} while ( unf != CheckInt(INTNO_SYS_TICK));	/* Check Interrupt Status */
	if ( unf != 0 ) {
		ofs += max;
	}

	EI(imask);

	return ofs * 1000 * CMCR_DIV / (SYSCLK_ICLK/MHz/PCLK_DIV);
}

#endif /* _SYSDEPEND_CPU_CORE_SYSTIMER_ */
