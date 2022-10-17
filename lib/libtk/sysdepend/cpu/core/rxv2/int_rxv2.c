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
#ifdef CPU_CORE_RXV2

/*
 *	int_rxv2.c
 *
 *	Interrupt control (RXv2 Core)
 */

#include <tk/tkernel.h>

/*----------------------------------------------------------------------*/
/*
 * CPU Interrupt Control for RXv2 core.
 *
 */
#define PSW_IPL_MASK		0x0F000000

/* 
 * Disable interrupt (Set PSW.IPL value to 15. And return the original PSW.IPL value.)
 */
EXPORT UW disint(void)
{
	UW	psw, ans;

	Asm("mvfc psw, %0": "=r"(psw));

	ans = (psw & PSW_IPL_MASK) >> 24;
	psw = (psw & ~PSW_IPL_MASK) |(INTPRI_MAX_INT_PRI << 24);

	Asm("mvtc %0, psw":: "r"(psw));

	return ans;
}

/* 
 * Set interrupt level (Set PSW.IPL value to ipl.)
 */
EXPORT void setint(UW ipl)
{
	UW	psw;

	Asm("mvfc psw, %0": "=r"(psw));

	psw = (psw & ~PSW_IPL_MASK) |(ipl << 24);

	Asm("mvtc %0, psw":: "r"(psw));
}

/*
 * Set Interrupt Mask Level in CPU
 */
EXPORT void SetCpuIntLevel( INT level )
{
	level--;
	if(level >= INTLEVEL_EI && level <= INTLEVEL_DI) {
		setint(level);
	}
}

/*
 * Get Interrupt Mask Level in CPU
 */
EXPORT INT GetCpuIntLevel( void )
{
	UW	psw;

	Asm("mvfc psw, %0": "=r"(psw));

	return ((psw & PSW_IPL_MASK) >> 24) + 1;
}


/*----------------------------------------------------------------------*/
/*
 * Interrupt controller (ICUb) Control
 *
 */

IMPORT const H	IPR_index[];

/*
 * Enable interrupt 
 *	Enables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void EnableInt( UINT intno, INT level )
{
	UINT	imask;

#if USE_GROUP_INT	// Group Interrupt
	if(intno >= INTNO_GROUP_TOP) {
		knl_enable_gint( intno);
		return;
	}
#endif /* USE_GROUP_INT */

	if((intno < 16) && (intno > 255)) {
		return;
	}

	DI(imask);

	if( IPR_index[intno-16] >= 0 ){
	/* Disable the interrupt before set priority level. */
		*(_UB*)(ICU_IER(intno)) &= ~ICU_IER_IEN(intno);
	/* Set interrupt priority level. */
		*(_UB*)(ICU_IPR(IPR_index[intno-16])) = (UB)level;
	/* Enables the specified interrupt. */
		*(_UB*)(ICU_IER(intno)) |= ICU_IER_IEN(intno);
	}

	EI(imask);
}

/*
 * Disable interrupt 
 *	Disables the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void DisableInt( UINT intno )
{
#if USE_GROUP_INT	// Group INterrupt
	if(intno >= INTNO_GROUP_TOP) {
		knl_disable_gint( intno);
		return;
	}
#endif /* USE_GROUP_INT */

	if((intno < 16) && (intno > 255)) {
		return;
	}

	if( IPR_index[intno-16] >= 0){
	/* Disable the interrupt before set priority level. */
		*(_UB*)(ICU_IER(intno)) &= ~ICU_IER_IEN(intno);
	}
}

/*
 * Clear interrupt
 *	Clear the interrupt specified in intno.
 *	External Interrupt can be specified. 
 */
EXPORT void ClearInt( UINT intno )
{
#if USE_GROUP_INT	// Group INterrupt
	if(intno >= INTNO_GROUP_TOP) {
		knl_clear_gint( intno);
		return;
	}
#endif /* USE_GROUP_INT */

	*(_UB*)(ICU_IR(intno)) = 0;
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
 *	Current active state for the associated interrupt
 *	External Interrupt can be specified. 
 */
EXPORT BOOL CheckInt( UINT intno )
{
#if USE_GROUP_INT	// Group INterrupt
	if(intno >= INTNO_GROUP_TOP) {
		return knl_check_gint( intno);
	}
#endif /* USE_GROUP_INT */

	return (*(_UB*)(ICU_IR(intno)));
}

/*
 * Set interrupt mode
 */
EXPORT void SetIntMode(UINT intno, UINT mode)
{
	UINT	imask;
	UB	irqmd;

#if USE_GROUP_INT	// Group INterrupt
	if(intno >= INTNO_GROUP_TOP) {
		return;
	}
#endif /* USE_GROUP_INT */

	switch (mode) {
	case (IM_LEVEL | IM_LOW):
		irqmd = 0x00;
		break;
	case (IM_EDGE | IM_LOW):
		irqmd = 0x04;
		break;
	case (IM_EDGE | IM_HI):
		irqmd = 0x80;
		break;
	case (IM_EDGE | IM_BOTH):
		irqmd = 0xC0;
		break;
	default:
		return;
	}

	DI(imask);

	if(IPR_index[intno] >= 0){
	/* Disable the interrupt before set priority level. */
		*(_UB*)(ICU_IER(intno)) &= ~ICU_IER_IEN(intno);
	/* Set interrupt priority level. */
		*(_UB*)(ICU_IRQCR(intno)) = irqmd;
	/* Enables the specified interrupt. */
		*(_UB*)(ICU_IER(intno)) |= ICU_IER_IEN(intno);
	}

	EI(imask);
}


#endif /* CPU_CORE_RXV2 */
