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

#include <sys/machine.h>
#ifdef CPU_CORE_RXV2

/*
 *	exc_hdr.c (RXv2)
 *	Exception handler
 */

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <kernel.h>

#if (USE_EXCEPTION_DBG_MSG && USE_TMONITOR)
	#define EXCEPTION_DBG_MSG(a)	tm_printf((UB*)a)
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
 * Supervisor Instruction Exception Handler
 */
WEAK_FUNC EXPORT void SuperVisorInst_Handler(void)
{
	EXCEPTION_DBG_MSG("Supervisor Instruction Exception\n");
	while(1);
}

/* 
 * Access Instruction Exception Handler
 */
WEAK_FUNC EXPORT void AccessInst_Handler(void)
{
	EXCEPTION_DBG_MSG("Access Instruction Exception\n");
	while(1);
}

/*
 * Undefined Instruction Exception Handler
 */
WEAK_FUNC EXPORT void UndefinedInst_Handler(void)
{
	EXCEPTION_DBG_MSG("Undefined Instruction Exception\n");
	while(1);
}

/*
 * Floating Point Exception Handler
 */
WEAK_FUNC EXPORT void FloatingPoint_Handler(void)
{
	EXCEPTION_DBG_MSG("Floating Point Exception\n");
	while(1);
}

/*
 * Default HLL Interrupt Handler
 */
WEAK_FUNC EXPORT void Default_Handler(UW intno)
{

#if (USE_EXCEPTION_DBG_MSG && USE_TMONITOR)
	tm_printf((UB*)"Un-defined Interrupt %d\n",intno);
#endif

	while(1);
}

#endif /* CPU_CORE_RXV2 */
