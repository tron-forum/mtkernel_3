/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_CORE_ARMV7A

/*
 *	interrupt.c  (ARMv7-A)
 *	Interrupt control
 */

/* Interrupt vector table */
Noinit(EXPORT FP knl_intvec_tbl[N_INTVEC]);

/* High level programming language interrupt handler table */
Noinit(EXPORT FP knl_inthdr_tbl[N_INTVEC]);

/* ----------------------------------------------------------------------- */
/*
 * Set interrupt handler (Used in tk_def_int())
 */
EXPORT ER knl_define_inthdr( INT intno, ATR intatr, FP inthdr )
{
	volatile FP	*intvet;

	if((inthdr != NULL) && ((intatr & TA_HLNG) != 0 )) {
		knl_inthdr_tbl[intno] = inthdr;
		inthdr = knl_hll_inthdr;
	}
	knl_intvec_tbl[intno] = inthdr;

	return E_OK;
}

/* ------------------------------------------------------------------------ */
/*
 * Interrupt initialize
 */
EXPORT ER knl_init_interrupt( void )
{
	INT	i;
	_UW	*base;

	/* Initialization of interrupt vector table */
	for(i = 0; i < N_INTVEC; i++) knl_intvec_tbl[i] = (FP)NULL;

    // GICD_IGROUPRnレジスタの初期化
    base = (volatile unsigned long *)&INTC.GICD_IGROUPR0.LONG;
    for (i = 0; i < INTC_GICD_IGROUPR_REG_NUM; i++) {
        base[i] = 0x00000000uL;                         // すべての割り込みをGroup0に
    }

    // GICD_ICFGRnレジスタの初期化
    base = (volatile unsigned long *)&INTC.GICD_ICFGR0.LONG;
    for (i = 0; i < INTC_GICD_ICFGR_REG_NUM; i++) {
        base[i] = GICD_ICFGR_init_values[i];          // 初期設定値テーブルからコピー
    }

    // GICD_IPRIORITYRnレジスタの初期化
    base = (volatile unsigned long *)&INTC.GICD_IPRIORITYR0.LONG;
    for (i = 0; i < INTC_GICD_IPRIORITYR_REG_NUM; i++) {
        base[i] = 0xF8F8F8F8uL;                         // すべてのプライオリティを31に
    }

    // GICD_ITARGETRnレジスタの初期化
    // GICD_ITARGETR0～7は初期化しない(固定orリザーブのため)
    base = (volatile unsigned long *)&INTC.GICD_ITARGETR0.LONG;
    for (i = 8; i < INTC_GICD_ITARGETR_REG_NUM; i++) {
        base[i] = 0x01010101uL;                         // シングルCPUのため、各設定値は1(CPU0)固定
    }

    // 割り込み禁止(INTCレベル)設定
    base = (volatile unsigned long *)&INTC.GICD_ICENABLER0.LONG;
    for (i = 0; i < INTC_GICD_ICENABLER_REG_NUM; i++) {
        base[i] = 0xFFFFFFFFuL;
    }

    // 割り込み優先度マスクの設定(すべての優先度を許可)
    INTC_SetMaskLevel(31);

    // GICC_BPRレジスタの設定(サブプライオリティをbit2-0に設定)
    INTC.GICC_BPR.LONG = 0x00000002uL;

    // GICC_CTLRレジスタの設定(CPUインタフェース有効化)
    INTC.GICC_CTLR.LONG = 0x00000003uL;

    // GICD_CTLRレジスタの設定(フォワード有効)
    INTC.GICD_CTLR.LONG = 0x00000001uL;

	return E_OK;
}

#endif /* CPU_CORE_ARMV7A */