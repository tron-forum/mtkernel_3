/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.06
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/10.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RX65N

/*
 *	int.c
 *
 *	Interrupt control (RX65N )
 */
#include <tk/tkernel.h>
#include <tk/syslib.h>

/*----------------------------------------------------------------------*/
/* 
 * Software Configurable Interrupt Control
 *
 */

#if USE_SFTCNF_INT
/* 
 * Set Software Configurable Interrupt (PERIA & PERIB)
 *	intno: Interrupt number  128~207(PERIB), 208~255(PERIA)
 *	fctno: Interrupt factor number
 */
EXPORT ER SetPERI(UINT intno, UINT fctno)
{
	if((intno < 128) || (intno >255)) return E_PAR;		// Interrupt number is out of range
	if(in_b(ICU_SLIPRCR)!=0) return E_RONLY;		// Changes are prohibited

	out_b(ICU_IER(intno), (in_b(ICU_IER(intno)) & ~(ICU_IER_IEN(intno))));

	if(intno >= 128 && intno <= 143) {
		out_b(ICU_SLIBXR(intno), fctno);
	} else if(intno >= 144 && intno <= 207) {
		out_b(ICU_SLIBR(intno), fctno);
	} else {
		out_b(ICU_SLIAR(intno), fctno);
	}

	out_b(ICU_IR(intno), 0);
	out_b(ICU_IER(intno), (in_b(ICU_IER(intno)) | (ICU_IER_IEN(intno))));

	return E_OK;
}
#endif /* USE_SFTCNF_INT */
 

/*----------------------------------------------------------------------*/
/* 
 * Group Interrupt Control
 *	These APIs are internal APIs.. Do not call from the user program!
 */

#if USE_GROUP_INT
/* 
 *  Enable group interrupts
 */
EXPORT void knl_enable_gint( UINT intno)
{
	UW	gen;
	UW	fctno;
	UINT	imask;

	if( intno < INTNO_GROUP_TOP + 32) {
		gen = ICU_GENBE0;
		fctno = intno - INTNO_GROUP_TOP;
	} else if( intno < INTNO_GROUP_TOP + 64) {
		gen = ICU_GENBL0;
		fctno = intno - (INTNO_GROUP_TOP + 32);
	} else if( intno < INTNO_GROUP_TOP + 96) {
		gen = ICU_GENBL1;
		fctno = intno - (INTNO_GROUP_TOP + 64);
	} else if( intno < INTNO_GROUP_TOP + 128) {
		gen = ICU_GENBL2;
		fctno = intno - (INTNO_GROUP_TOP + 96);
	} else if( intno < INTNO_GROUP_TOP + 160) {
		gen = ICU_GENAL0;
		fctno = intno - (INTNO_GROUP_TOP + 128);
	} else if( intno < INTNO_GROUP_TOP + 192) {
		gen = ICU_GENAL1;
		fctno = intno - (INTNO_GROUP_TOP + 160);
	} else {
		return;
	}

	DI(imask);
	out_w( gen, in_w(gen)|(1<<fctno));
	EI(imask);
}

/* 
 *  Disable group interrupts
 */
EXPORT void knl_disable_gint( UINT intno)
{
	UW	gen;
	UW	fctno;
	UINT	imask;

	if( intno < INTNO_GROUP_TOP + 32) {
		gen = ICU_GENBE0;
		fctno = intno - INTNO_GROUP_TOP;
	} else if( intno < INTNO_GROUP_TOP + 64) {
		gen = ICU_GENBL0;
		fctno = intno - (INTNO_GROUP_TOP + 32);
	} else if( intno < INTNO_GROUP_TOP + 96) {
		gen = ICU_GENBL1;
		fctno = intno - (INTNO_GROUP_TOP + 64);
	} else if( intno < INTNO_GROUP_TOP + 128) {
		gen = ICU_GENBL2;
		fctno = intno - (INTNO_GROUP_TOP + 96);
	} else if( intno < INTNO_GROUP_TOP + 160) {
		gen = ICU_GENAL0;
		fctno = intno - (INTNO_GROUP_TOP + 128);
	} else if( intno < INTNO_GROUP_TOP + 192) {
		gen = ICU_GENAL1;
		fctno = intno - (INTNO_GROUP_TOP + 160);
	} else {
		return;
	}

	DI(imask);
	out_w( gen, in_w(gen)& ~(1<<fctno));
	EI(imask);
}

/*
 *  Clear group interrupts
 */
EXPORT void knl_clear_gint( UINT intno)
{
	UW	gcr;
	UW	fctno;
	UINT	imask;

	if( intno < INTNO_GROUP_TOP + 32) {
		gcr = ICU_GCRBE0;
		fctno = intno - INTNO_GROUP_TOP;
	} else {
		return;
	}

	DI(imask);
	out_w( gcr, in_w(gcr)| (1<<fctno));
	EI(imask);
}

/*
 *  Check group interrupts
 */
EXPORT BOOL knl_check_gint( UINT intno)
{
	UW	grp;
	UW	fctno;
	UINT	intno_o;

	if( intno < INTNO_GROUP_TOP + 32) {
		intno_o = INTNO_GROUPBE0;
		grp = ICU_GRPBE0;
		fctno = intno - INTNO_GROUP_TOP;
	} else if( intno < INTNO_GROUP_TOP + 64) {
		intno_o = INTNO_GROUPBL0;
		grp = ICU_GRPBL0;
		fctno = intno - (INTNO_GROUP_TOP + 32);
	} else if( intno < INTNO_GROUP_TOP + 96) {
		intno_o = INTNO_GROUPBL1;
		grp = ICU_GRPBL1;
		fctno = intno - (INTNO_GROUP_TOP + 64);
	} else if( intno < INTNO_GROUP_TOP + 128) {
		intno_o = INTNO_GROUPBL2;
		grp = ICU_GRPBL2;
		fctno = intno - (INTNO_GROUP_TOP + 96);
	} else if( intno < INTNO_GROUP_TOP + 160) {
		intno_o = INTNO_GROUPAL0;
		grp = ICU_GRPAL0;
		fctno = intno - (INTNO_GROUP_TOP + 128);
	} else if( intno < INTNO_GROUP_TOP + 192) {
		intno_o = INTNO_GROUPAL1;
		grp = ICU_GRPAL1;
		fctno = intno - (INTNO_GROUP_TOP + 160);
	} else {
		return FALSE;
	}

	if((*(_UB*)(ICU_IR(intno_o)))==0) {
		grp = 0;
	}
	return (in_w(grp) & (1<<fctno) != 0);
}

#endif /* USE_GROUP_INT */
#endif /* CPU_RX65N */