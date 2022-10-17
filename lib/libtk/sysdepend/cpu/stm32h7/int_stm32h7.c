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

#include <sys/machine.h>
#ifdef CPU_STM32H7

/*
 *	int_stm32h7.c
 *
 *	Interrupt controller (STM32H7 )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

#include "../core/armv7m/int_armv7m.h"

/*----------------------------------------------------------------------*/
/*
 * EXTI (Extended interrupt controller) functions
 * 
 */
LOCAL void EnableInt_exti( UINT intno, INT level )
{
	if(intno < 32) {
		*(_UW*)EXTI_CPUIMR1 |= (UW)(1<<intno);
	} else if(intno < 64) {
		*(_UW*)EXTI_CPUIMR2 |= (UW)(1<<(intno-32));
	} else {
		*(_UW*)EXTI_CPUIMR3 |= (UW)(1<<(intno-64));
	}	
}

LOCAL void DisableInt_exti( UINT intno )
{
	if(intno < 32) {
		*(_UW*)EXTI_CPUIMR1 &= ~(UW)(1<<intno);
	} else if(intno < 64) {
		*(_UW*)EXTI_CPUIMR2 &= ~(UW)(1<<(intno-32));
	} else {
		*(_UW*)EXTI_CPUIMR3 &= ~(UW)(1<<(intno-64));
	}

}

LOCAL void ClearInt_exti( UINT intno )
{
	if(intno < 32) {
		*(_UW*)EXTI_CPUPR1 |= (UW)(1<<intno);
	} else if(intno < 64) {
		*(_UW*)EXTI_CPUPR2 |= (UW)(1<<(intno-32));
	} else {
		*(_UW*)EXTI_CPUPR3 |= (UW)(1<<(intno-64));
	}
}

LOCAL BOOL CheckInt_exti( UINT intno )
{
	UW	pif;

	if(intno < 32) {
		pif = *(_UW*)EXTI_CPUPR1 & (UW)(1<<intno);
	} else if(intno < 64) {
		pif = *(_UW*)EXTI_CPUPR2 & (UW)(1<<(intno-32));		
	} else {
		pif = *(_UW*)EXTI_CPUPR3 & (UW)(1<<(intno-64));		
	}
	return pif?TRUE:FALSE;
}

LOCAL void SetIntMode_exti(UINT intno, UINT mode)
{
	if(mode & IM_HI) {
		if(intno < 32) {
			*(_UW*)EXTI_RTSR1 |= (UW)(1<<intno);
		} else if(intno < 64) {
			*(_UW*)EXTI_RTSR2 |= (UW)(1<<(intno-32));
		} else {
			*(_UW*)EXTI_RTSR3 |= (UW)(1<<(intno-64));
		}
	}
	if(mode & IM_LOW) {
		if(intno < 32) {
			*(_UW*)EXTI_FTSR1 |= (UW)(1<<intno);
		} else if(intno < 64) {
			*(_UW*)EXTI_FTSR2 |= (UW)(1<<(intno-32));
		} else {
			*(_UW*)EXTI_FTSR3 |= (UW)(1<<(intno-64));
		}
	}
}

/*----------------------------------------------------------------------*/
/*
 * Interrupt control API
 * 
 */
/*
 * Enable interrupt 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	if( intno <= MAX_NVIC_INTNO) {
		EnableInt_nvic( intno, level);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		EnableInt_exti( intno - MIN_EXTI_INTNO, level);
	}
}

/*
 * Disable interrupt 
 */
EXPORT void DisableInt( UINT intno )
{
	if( intno <= MAX_NVIC_INTNO) {
		DisableInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		DisableInt_exti( intno - MIN_EXTI_INTNO);
	}
}

/*
 * Clear interrupt
 */
EXPORT void ClearInt(UINT intno)
{
	if( intno <= MAX_NVIC_INTNO) {
		ClearInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		ClearInt_exti( intno - MIN_EXTI_INTNO);
	}
}

/*
 * Issue EOI to interrupt controller
 */
EXPORT void EndOfInt(UINT intno)
{
	/* No opetarion. */
}

/*
 * Check active state
 */
EXPORT BOOL CheckInt( UINT intno )
{
	BOOL rtncd;

	if( intno <= MAX_NVIC_INTNO) {
		rtncd = CheckInt_nvic( intno);
	} else if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		rtncd = CheckInt_exti( intno - MIN_EXTI_INTNO);
	} else {
		rtncd = FALSE;
	}
	return rtncd;
}

/*
 * Set interrupt mode
 */
EXPORT void SetIntMode(UINT intno, UINT mode)
{
	if(intno >= MIN_EXTI_INTNO && intno <= MAX_EXTI_INTNO) {
		SetIntMode_exti( intno - MIN_EXTI_INTNO, mode);
	}
}

#endif /* CPU_STM32H7 */