/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06A
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/03.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7M

/*
 *	exc_hdr.c (ARMv7-M)
 *	Exception handler
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <kernel.h>
#include "../../../sysdepend.h"

#if (USE_EXCEPTION_DBG_MSG && USE_TMONITOR)
	#define EXCEPTION_DBG_MSG(a)	tm_putstring((UB*)a)
#else
	#define EXCEPTION_DBG_MSG(a)
#endif

/*
 * NMI handler
 */
WEAK_FUNC EXPORT void NMI_Handler(void)
{
	EXCEPTION_DBG_MSG("NMI\n");
	while(1);
}

/*
 * Hard fault handler
 */
WEAK_FUNC EXPORT void HardFault_Handler(void)
{
#if (USE_EXCEPTION_DBG_MSG  && USE_TMONITOR)

	UW	hfsr, cfsr;
	ID	ctskid;

	hfsr	= *(_UW *)SCB_HFSR;
	if(knl_ctxtsk != NULL) {
		ctskid = knl_ctxtsk->tskid;
	} else {
		ctskid = 0;
	}

	if(hfsr & 0x40000000) {
		cfsr = *(_UW*)SCB_CFSR;
		tm_printf((UB*)"*** Hard fault ***  ctxtsk:%d  HFSR:%x  CFSR:%x\n", ctskid, hfsr, cfsr);
	} else {
		tm_printf((UB*)"*** Hard fault ***  ctxtsk:%d  HFSR:%x%x\n", ctskid, hfsr);
	}
#endif
	while(1);
}

/*
 * MPU Fault Handler
 */
WEAK_FUNC EXPORT void MemManage_Handler(void)
{
	EXCEPTION_DBG_MSG("MPU Fault\n");
	while(1);
}

/* 
 * Bus Fault Handler
 */
WEAK_FUNC EXPORT void BusFault_Handler(void)
{
	EXCEPTION_DBG_MSG("Bus Fault\n");
	while(1);
}

/*
 * Usage Fault Handler
 */
WEAK_FUNC EXPORT void UsageFault_Handler(void)
{
	EXCEPTION_DBG_MSG("Usage Fault\n");
	while(1);
}

/*
 * Svcall
 */
WEAK_FUNC EXPORT void Svcall_Handler(void)
{
	EXCEPTION_DBG_MSG("SVCall\n");
	while(1);
}

/* 
 * Debug Monitor
 */
WEAK_FUNC EXPORT void DebugMon_Handler(void)
{
	EXCEPTION_DBG_MSG("Debug Monitor\n");
	while(1);
}

/*
 * Default Handler
 */
WEAK_FUNC EXPORT void Default_Handler(void)
{
#if (USE_EXCEPTION_DBG_MSG  && USE_TMONITOR)
	INT	i;
	_UW	*icpr;

	icpr = (_UW*)NVIC_ICPR_BASE;

	EXCEPTION_DBG_MSG("Undefine Exceptio ICPR: ");
	for(i=0; i < 8; i++) {
		tm_printf((UB*)"%x ", *icpr++);
	}
	EXCEPTION_DBG_MSG("\n");
#endif
	while(1);
}

#endif /* CPU_CORE_ACMV7M */
