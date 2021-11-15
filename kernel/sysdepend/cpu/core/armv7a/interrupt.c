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
 *	interrupt.c  (ARMv7-A)
 *	Interrupt control
 */

#include <kernel.h>
#include "../../../sysdepend.h"

/* Interrupt vector table */
Noinit(EXPORT FP knl_intvec_tbl[N_INTVEC]);

/* High level programming language interrupt handler table */
Noinit(EXPORT FP knl_hll_inthdr_tbl[N_INTVEC]);

/* SVC handler table */
EXPORT const FP knl_svcvec_tbl[N_SVCHDR] = {
	NULL, NULL, NULL, NULL, NULL, NULL,	/* 0 ~ 5 : reserved. */
	NULL,					/* 6 : micro T-Kernel system call */
	knl_dispatch_to_schedtsk,		/* 7 : force dispatch */
	knl_dispatch_entry,			/* 8 : task dispatcher */
	NULL,					/* 9 : debug support function */
	NULL					/* 10: Extended SVC */
};

/* ----------------------------------------------------------------------- */
/*
 * Set interrupt handler (Used in tk_def_int())
 */
EXPORT ER knl_define_inthdr( INT intno, ATR intatr, FP inthdr )
{
	if((inthdr != NULL) && ((intatr & TA_HLNG) != 0 )) {
		knl_hll_inthdr_tbl[intno] = inthdr;
		inthdr = knl_hll_inthdr;
	}
	knl_intvec_tbl[intno] = inthdr;

	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Interrupt initialize
 */

 /* GICD_ICFGR(n) Register default value */
LOCAL const UW GICD_ICFGR_inival[] =
{				//           Interrupt ID
	0xAAAAAAAAuL,		// GICD_ICFGR0  :  15 to   0
	0x55540000uL,		// GICD_ICFGR1  :  19 to  16
	0xFD555555uL,		// GICD_ICFGR2  :  47 to  32
	0x7FFFFFFFuL,		// GICD_ICFGR3  :  63 to  48
	0x55555555uL,		// GICD_ICFGR4  :  79 to  64
	0xD57F5555uL,		// GICD_ICFGR5  :  95 to  80
	0xFFFFFFFFuL,		// GICD_ICFGR6  : 111 to  96
	0xFFFFFFFFuL,		// GICD_ICFGR7  : 127 to 112
	0xFFFFFFFFuL,		// GICD_ICFGR8  : 143 to 128
	0xFFFFFFFFuL,		// GICD_ICFGR9  : 159 to 144
	0xFFFFFFFFuL,		// GICD_ICFGR10 : 175 to 160
	0xFFFFFFFFuL,		// GICD_ICFGR11 : 191 to 176
	0xFFFFFFFFuL,		// GICD_ICFGR12 : 207 to 192
	0xFFFFFFFFuL,		// GICD_ICFGR13 : 223 to 208
	0xFFFFFFFFuL,		// GICD_ICFGR14 : 239 to 224
	0x7D5FD57FuL,		// GICD_ICFGR15 : 255 to 240
	0x557D7DDFuL,		// GICD_ICFGR16 : 271 to 256
	0x557D557DuL,		// GICD_ICFGR17 : 287 to 272
	0x5555557DuL,		// GICD_ICFGR18 : 303 to 288
	0x55555555uL,		// GICD_ICFGR19 : 319 to 304
	0xF5555555uL,		// GICD_ICFGR20 : 335 to 320
	0x5555FFFFuL,		// GICD_ICFGR21 : 351 to 336
	0x55555555uL,		// GICD_ICFGR22 : 367 to 352
	0xFFDD5555uL,		// GICD_ICFGR23 : 383 to 368
	0xFFFFFFFFuL,		// GICD_ICFGR24 : 399 to 384
	0xFFFFFFFFuL,		// GICD_ICFGR25 : 415 to 400
	0xFFFFFFFFuL,		// GICD_ICFGR26 : 431 to 416
	0x5FFFFFDFuL,		// GICD_ICFGR27 : 447 to 432
	0x55555555uL,		// GICD_ICFGR28 : 463 to 448
	0x55555555uL,		// GICD_ICFGR29 : 479 to 464
	0x55555555uL,		// GICD_ICFGR30 : 495 to 480
	0x55555555uL,		// GICD_ICFGR31 : 511 to 496
};

EXPORT ER knl_init_interrupt( void )
{
	INT	i;
	_UW	*reg;

	/* Initialization of interrupt vector table */
	for(i = 0; i < N_INTVEC; i++) knl_intvec_tbl[i] = (FP)NULL;

	/* Register exception handler used on OS */
	knl_define_inthdr(INTNO_SYSTICK, TA_HLNG, (FP)knl_timer_handler);	/* System Timer Interruput */

	/* GIC initialization */
	reg = (_UW*)GICD_IGROUPR(0);
	for (i = 0; i < GICD_IGROUPR_N; i++) {
		reg[i] = 0x00000000uL;		/* All interrupts are Group 0 */
	}

	reg = (_UW*)GICD_ICFGR(0);
	for (i = 0; i < GICD_ICFGR_N; i++) {
		reg[i] = GICD_ICFGR_inival[i];	/* Copy from the default table */
	}

	reg = (_UW*)GICD_IPRIORITYR(0);
	for (i = 0; i < GICD_IPRIORITYR_N; i++) {
		reg[i] = 0xF8F8F8F8uL;		/* All priorities are 31 */
	}

	reg = (_UW*)GICD_ITARGETR(0);
	for (i = 8; i < GICD_ITARGETR_N; i++) {
		reg[i] = 0x01010101uL;		/* Target processor is CPU-0 */
	}
	
	reg = (_UW*)GICD_ICENABLER(0);
	for (i = 0; i < GICD_ICENABLER_N; i++) {
		reg[i] = 0xFFFFFFFFuL;
	}

	out_w(GICC_PMR, 31<<3);			/* Allow all interrupts */
	out_w(GICC_BPR, 0x00000002UL);
	out_w(GICC_CTLR, 0x00000003UL);
	out_w(GICD_CTLR, 0x00000001uL);

	return E_OK;
}

#endif /* CPU_CORE_ARMV7A */