/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.04
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/05/17.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_RX231

/*
 *	hllint_tbl.c (RX231)
 *	HLL Interrupt Handler Table (ROM)
 */

#include "kernel.h"
#include "../../sysdepend.h"

/* ------------------------------------------------------------------------ */
/*
 * HLL Interrupt Handler Table (ROM)
 * 
 */
EXPORT void (* const knl_hll_inthdr_rom[])()  __attribute__((section(".hvectors"))) = {
	Default_Handler,			/* Unconditional trap */
	Default_Handler,			/* Dispatch */
	Default_Handler,			/* System call (reserved) */
	Default_Handler,			/* System call : tk_ret_int */
	Default_Handler,			/* Debugger support (reserved) */
	Default_Handler,			/* INT 5 */
	Default_Handler,			/* INT 6 */
	Default_Handler,			/* INT 7 */
	Default_Handler,			/* INT 8 */
	Default_Handler,			/* INT 9 */
	Default_Handler,			/* INT 10 */
	Default_Handler,			/* INT 11 */
	Default_Handler,			/* INT 12 */
	Default_Handler,			/* INT 13 */
	Default_Handler,			/* INT 14 */
	Default_Handler,			/* INT 15 */
	Default_Handler,			/* BSC_BUSERR */
	Default_Handler,			/* INT 17 */
	Default_Handler,			/* INT 18 */
	Default_Handler,			/* INT 19 */
	Default_Handler,			/* INT 20 */
	Default_Handler,			/* INT 21 */
	Default_Handler,			/* INT 22 */
	Default_Handler,			/* FCU_FRDYI */
	Default_Handler,			/* INT 24 */
	Default_Handler,			/* INT 25 */
	Default_Handler,			/* INT 26 */
	Default_Handler,			/* ICU_SWINT */
	knl_timer_handler,			/* CMT0_CMI0 **** System timer *****/
	Default_Handler,			/* CMT0_CMI1 */
	Default_Handler,			/* CMT0_CMI2 */
	Default_Handler,			/* CMT0_CMI3 */
	Default_Handler,			/* CAC_FERRF */
	Default_Handler,			/* CAC_MENDF */
	Default_Handler,			/* CAC_OVFF */
	Default_Handler,			/* INT 35 */
	Default_Handler,			/* USB0_D0FIFO0 */
	Default_Handler,			/* USB0_D1FIFO0 */
	Default_Handler,			/* USB0_USBI0*/
	Default_Handler,			/* INT 39 */
	Default_Handler,			/* SDHI_SBFAI */
	Default_Handler,			/* SDHI_CDETI */
	Default_Handler,			/* SDHI_CACI */
	Default_Handler,			/* SDHI_SDACI */
	Default_Handler,			/* RSPI0_SPEI0 */
	Default_Handler,			/* RSPI0_SPRI0 */
	Default_Handler,			/* RSPI0_SPTI0 */
	Default_Handler,			/* RSPI0_SPII0 */
	Default_Handler,			/* INT 48 */
	Default_Handler,			/* INT 49 */
	Default_Handler,			/* INT 50 */
	Default_Handler,			/* INT 51 */
	Default_Handler,			/* RSCAN_COMFRXINT */
	Default_Handler,			/* RSCAN_RXFINT */
	Default_Handler,			/* RSCAN_TXINT */
	Default_Handler,			/* RSCAN_CHERRINT */
	Default_Handler,			/* RSCAN_GLERRINT */
	Default_Handler,			/* DOC_DOPCF */
	Default_Handler,			/* CMPB_CMPB0 */
	Default_Handler,			/* CMPB_CMPB1 */
	Default_Handler,			/* CTSU_CTSUWR */
	Default_Handler,			/* CTSU_CTSURD */
	Default_Handler,			/* CTSU_CTSUFN */
	Default_Handler,			/* RTC_CUP */
	Default_Handler,			/* ICU_IRQ0 */
	Default_Handler,			/* ICU_IRQ1 */
	Default_Handler,			/* ICU_IRQ2 */
	Default_Handler,			/* ICU_IRQ3 */
	Default_Handler,			/* ICU_IRQ4 */
	Default_Handler,			/* ICU_IRQ5 */
	Default_Handler,			/* ICU_IRQ6 */
	Default_Handler,			/* ICU_IRQ7 */
	Default_Handler,			/* INT 72 */
	Default_Handler,			/* INT 73 */
	Default_Handler,			/* INT 74 */
	Default_Handler,			/* INT 75 */
	Default_Handler,			/* INT 76 */
	Default_Handler,			/* INT 77 */
	Default_Handler,			/* INT 78 */
	Default_Handler,			/* INT 79 */
	Default_Handler,			/* ELC_ELSR8I */
	Default_Handler,			/* INT 81 */
	Default_Handler,			/* INT 82 */
	Default_Handler,			/* INT 83 */
	Default_Handler,			/* INT 84 */
	Default_Handler,			/* INT 85 */
	Default_Handler,			/* INT 86 */
	Default_Handler,			/* INT 87 */
	Default_Handler,			/* LVD_LVD1 */
	Default_Handler,			/* LVD_LVD2 */
	Default_Handler,			/* USB0_USBR0 */
	Default_Handler,			/* VBATT_VBTLVDI */
	Default_Handler,			/* RTC_ALM */
	Default_Handler,			/* RTC_PRD */
	Default_Handler,			/* INT 94 */
	Default_Handler,			/* INT 95 */
	Default_Handler,			/* INT 96 */
	Default_Handler,			/* INT 97 */
	Default_Handler,			/* INT 98 */
	Default_Handler,			/* INT 99 */
	Default_Handler,			/* INT 100 */
	Default_Handler,			/* INT 101 */
	Default_Handler,			/* S12AD_S12ADI0 */
	Default_Handler,			/* S12AD_GBADI */
	Default_Handler,			/* CMPB1_CMPB2 */
	Default_Handler,			/* CMPB1_CMPB3 */
	Default_Handler,			/* ELC_ELSR18I */
	Default_Handler,			/* ELC_ELSR19I */
	Default_Handler,			/* SSI0_SSIF0 */
	Default_Handler,			/* SSI0_SSIRXI0 */
	Default_Handler,			/* SSI0_SSITXI0 */
	Default_Handler,			/* Secure_RD */
	Default_Handler,			/* Secure_WR */
	Default_Handler,			/* Secure_Error */
	Default_Handler,			/* MTU0_TGIA0 */
	Default_Handler,			/* MTU0_TGIB0 */
	Default_Handler,			/* MTU0_TGIC0 */
	Default_Handler,			/* MTU0_TGID0 */
	Default_Handler,			/* MTU0_TCIV0 */
	Default_Handler,			/* MTU0_TGIE0 */
	Default_Handler,			/* MTU0_TGIF0 */
	Default_Handler,			/* MTU1_TGIA1 */
	Default_Handler,			/* MTU1_TGIB1 */
	Default_Handler,			/* MTU1_TCIV1 */
	Default_Handler,			/* MTU1_TCIU1 */
	Default_Handler,			/* MTU2_TGIA2 */
	Default_Handler,			/* MTU2_TGIB2 */
	Default_Handler,			/* MTU2_TCIV2 */
	Default_Handler,			/* MTU2_TCIU2 */
	Default_Handler,			/* MTU3_TGIA3 */
	Default_Handler,			/* MTU3_TGIB3 */
	Default_Handler,			/* MTU3_TGIC3 */
	Default_Handler,			/* MTU3_TGID3 */
	Default_Handler,			/* MTU3_TCIV3 */
	Default_Handler,			/* MTU4_TGIA4 */
	Default_Handler,			/* MTU4_TGIB4 */
	Default_Handler,			/* MTU4_TGIC4 */
	Default_Handler,			/* MTU4_TGID4 */
	Default_Handler,			/* MTU4_TCIV4 */
	Default_Handler,			/* MTU5_TGIU5 */
	Default_Handler,			/* MTU5_TGIV5 */
	Default_Handler,			/* MTU5_TGIW5 */
	Default_Handler,			/* TPU0_TGI0A */
	Default_Handler,			/* TPU0_TGI0B */
	Default_Handler,			/* TPU0_TGI0C */
	Default_Handler,			/* TPU0_TGI0D */
	Default_Handler,			/* TPU0_TCI0V */
	Default_Handler,			/* TPU1_TGI1A */
	Default_Handler,			/* TPU1_TGI1B */
	Default_Handler,			/* TPU1_TCI1V */
	Default_Handler,			/* TPU1_TCI1U */
	Default_Handler,			/* TPU2_TGI2A */
	Default_Handler,			/* TPU2_TGI2B */
	Default_Handler,			/* TPU2_TCI2V */
	Default_Handler,			/* TPU2_TCI2U */
	Default_Handler,			/* TPU3_TGI3A */
	Default_Handler,			/* TPU3_TGI3B */
	Default_Handler,			/* TPU3_TGI3C */
	Default_Handler,			/* TPU3_TGI3D */
	Default_Handler,			/* TPU3_TCI3V */
	Default_Handler,			/* TPU4_TGI4A */
	Default_Handler,			/* TPU4_TGI4B */
	Default_Handler,			/* TPU4_TCI4V */
	Default_Handler,			/* TPU4_TCI4U */
	Default_Handler,			/* TPU5_TGI5A */
	Default_Handler,			/* TPU5_TGI5B */
	Default_Handler,			/* TPU5_TCI5V */
	Default_Handler,			/* TPU5_TCI5U */
	Default_Handler,			/* INT 168 */
	Default_Handler,			/* INT 169 */
	Default_Handler,			/* POE_OEI1 */
	Default_Handler,			/* POE_OEI2 */
	Default_Handler,			/* INT 172 */
	Default_Handler,			/* INT 173 */
	Default_Handler,			/* TMR0_CMIA0 */
	Default_Handler,			/* TMR0_CMIB0 */
	Default_Handler,			/* TMR0_OVI0 */
	Default_Handler,			/* TMR1_CMIA1 */
	Default_Handler,			/* TMR1_CMIB1 */
	Default_Handler,			/* TMR1_OVI1 */
	Default_Handler,			/* TMR2_CMIA2 */
	Default_Handler,			/* TMR2_CMIB2 */
	Default_Handler,			/* TMR2_OVI2 */
	Default_Handler,			/* TMR3_CMIA3 */
	Default_Handler,			/* TMR3_CMIB3 */
	Default_Handler,			/* TMR3_OVI3 */
	Default_Handler,			/* INT 186 */
	Default_Handler,			/* INT 187 */
	Default_Handler,			/* INT 188 */
	Default_Handler,			/* INT 189 */
	Default_Handler,			/* INT 190 */
	Default_Handler,			/* INT 191 */
	Default_Handler,			/* INT 192 */
	Default_Handler,			/* INT 193 */
	Default_Handler,			/* INT 194 */
	Default_Handler,			/* INT 195 */
	Default_Handler,			/* INT 196 */
	Default_Handler,			/* INT 197 */
	Default_Handler,			/* DMAC_DMAC0I*/
	Default_Handler,			/* DMAC_DMAC1I*/
	Default_Handler,			/* DMAC_DMAC2I*/
	Default_Handler,			/* DMAC_DMAC3I*/
	Default_Handler,			/* INT 202 */
	Default_Handler,			/* INT 203 */
	Default_Handler,			/* INT 204 */
	Default_Handler,			/* INT 205 */
	Default_Handler,			/* INT 206 */
	Default_Handler,			/* INT 207 */
	Default_Handler,			/* INT 208 */
	Default_Handler,			/* INT 209 */
	Default_Handler,			/* INT 210 */
	Default_Handler,			/* INT 211 */
	Default_Handler,			/* INT 212 */
	Default_Handler,			/* INT 213 */
	Default_Handler,			/* SCI0_ERI0 */
	Default_Handler,			/* SCI0_RXI0 */
	Default_Handler,			/* SCI0_TXI0 */
	Default_Handler,			/* SCI0_TEI0 */
	Default_Handler,			/* SCI1_ERI1 */
	Default_Handler,			/* SCI1_RXI1 */
	Default_Handler,			/* SCI1_TXI1 */
	Default_Handler,			/* SCI1_TEI1 */
	Default_Handler,			/* SCI5_ERI5 */
	Default_Handler,			/* SCI5_RXI5 */
	Default_Handler,			/* SCI5_TXI5 */
	Default_Handler,			/* SCI5_TEI5 */
	Default_Handler,			/* SCI6_ERI6 */
	Default_Handler,			/* SCI6_RXI6 */
	Default_Handler,			/* SCI6_TXI6 */
	Default_Handler,			/* SCI6_TEI6 */
	Default_Handler,			/* SCI8_ERI8 */
	Default_Handler,			/* SCI8_RXI8 */
	Default_Handler,			/* SCI8_TXI8 */
	Default_Handler,			/* SCI8_TEI8 */
	Default_Handler,			/* SCI9_ERI9 */
	Default_Handler,			/* SCI9_RXI9 */
	Default_Handler,			/* SCI9_TXI9 */
	Default_Handler,			/* SCI9_TEI9 */
	Default_Handler,			/* SCI12_ERI12 */
	Default_Handler,			/* SCI12_RXI12 */
	Default_Handler,			/* SCI12_TXI12 */
	Default_Handler,			/* SCI12_TEI12 */
	Default_Handler,			/* SCI12_SCIX0 */
	Default_Handler,			/* SCI12_SCIX1 */
	Default_Handler,			/* SCI12_SCIX2 */
	Default_Handler,			/* SCI12_SCIX3 */
	Default_Handler,			/* RIIC0_EEI0 */
	Default_Handler,			/* RIIC0_RXI0 */
	Default_Handler,			/* RIIC0_TXI0 */
	Default_Handler,			/* RIIC0_TEI0 */
	Default_Handler,			/* INT 250 */
	Default_Handler,			/* INT 251 */
	Default_Handler,			/* INT 252 */
	Default_Handler,			/* INT 253 */
	Default_Handler,			/* INT 254 */
	Default_Handler,			/* INT 255 */
};

#endif /* CPU_CORE_RX231 */
