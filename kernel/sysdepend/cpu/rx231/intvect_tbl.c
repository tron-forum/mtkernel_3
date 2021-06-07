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
 *	intvect_tbl.c (RX231)
 *	Interrupt Vector Table (ROM)
 */

#include "kernel.h"
#include "../../sysdepend.h"

#include "hllint_ent.h"

/* ------------------------------------------------------------------------ */
/*
 * Interrupt Vector Table (ROM)
 * 
 */
EXPORT void (* const knl_int_vect_rom[])()  __attribute__((section(".rvectors"))) = {
	hll_inthdr_entry_0,			/* Unconditional trap */
	hll_inthdr_entry_1,			/* Dispatch */
	hll_inthdr_entry_2,			/* System call (reserved) */
	hll_inthdr_entry_3,			/* System call : tk_ret_int */
	hll_inthdr_entry_4,			/* Debugger support (reserved) */
	hll_inthdr_entry_5,			/* INT 5 */
	hll_inthdr_entry_6,			/* INT 6 */
	hll_inthdr_entry_7,			/* INT 7 */
	hll_inthdr_entry_8,			/* INT 8 */
	hll_inthdr_entry_9,			/* INT 9 */
	hll_inthdr_entry_10,			/* INT 10 */
	hll_inthdr_entry_11,			/* INT 11 */
	hll_inthdr_entry_12,			/* INT 12 */
	hll_inthdr_entry_13,			/* INT 13 */
	hll_inthdr_entry_14,			/* INT 14 */
	hll_inthdr_entry_15,			/* INT 15 */
	hll_inthdr_entry_16,			/* BSC_BUSERR */
	hll_inthdr_entry_17,			/* INT 17 */
	hll_inthdr_entry_18,			/* INT 18 */
	hll_inthdr_entry_19,			/* INT 19 */
	hll_inthdr_entry_20,			/* INT 20 */
	hll_inthdr_entry_21,			/* INT 21 */
	hll_inthdr_entry_22,			/* INT 22 */
	hll_inthdr_entry_23,			/* FCU_FRDYI */
	hll_inthdr_entry_24,			/* INT 24 */
	hll_inthdr_entry_25,			/* INT 25 */
	hll_inthdr_entry_26,			/* INT 26 */
	hll_inthdr_entry_27,			/* ICU_SWINT */
	hll_inthdr_entry_28,			/* CMT0_CMI0 */
	hll_inthdr_entry_29,			/* CMT0_CMI1 */
	hll_inthdr_entry_30,			/* CMT0_CMI2 */
	hll_inthdr_entry_31,			/* CMT0_CMI3 */
	hll_inthdr_entry_32,			/* CAC_FERRF */
	hll_inthdr_entry_33,			/* CAC_MENDF */
	hll_inthdr_entry_34,			/* CAC_OVFF */
	hll_inthdr_entry_35,			/* INT 35 */
	hll_inthdr_entry_36,			/* USB0_D0FIFO0 */
	hll_inthdr_entry_37,			/* USB0_D1FIFO0 */
	hll_inthdr_entry_38,			/* USB0_USBI0*/
	hll_inthdr_entry_39,			/* INT 39 */
	hll_inthdr_entry_40,			/* SDHI_SBFAI */
	hll_inthdr_entry_41,			/* SDHI_CDETI */
	hll_inthdr_entry_42,			/* SDHI_CACI */
	hll_inthdr_entry_43,			/* SDHI_SDACI */
	hll_inthdr_entry_44,			/* RSPI0_SPEI0 */
	hll_inthdr_entry_45,			/* RSPI0_SPRI0 */
	hll_inthdr_entry_46,			/* RSPI0_SPTI0 */
	hll_inthdr_entry_47,			/* RSPI0_SPII0 */
	hll_inthdr_entry_48,			/* INT 48 */
	hll_inthdr_entry_49,			/* INT 49 */
	hll_inthdr_entry_50,			/* INT 50 */
	hll_inthdr_entry_51,			/* INT 51 */
	hll_inthdr_entry_52,			/* RSCAN_COMFRXINT */
	hll_inthdr_entry_53,			/* RSCAN_RXFINT */
	hll_inthdr_entry_54,			/* RSCAN_TXINT */
	hll_inthdr_entry_55,			/* RSCAN_CHERRINT */
	hll_inthdr_entry_56,			/* RSCAN_GLERRINT */
	hll_inthdr_entry_57,			/* DOC_DOPCF */
	hll_inthdr_entry_58,			/* CMPB_CMPB0 */
	hll_inthdr_entry_59,			/* CMPB_CMPB1 */
	hll_inthdr_entry_60,			/* CTSU_CTSUWR */
	hll_inthdr_entry_61,			/* CTSU_CTSURD */
	hll_inthdr_entry_62,			/* CTSU_CTSUFN */
	hll_inthdr_entry_63,			/* RTC_CUP */
	hll_inthdr_entry_64,			/* ICU_IRQ0 */
	hll_inthdr_entry_65,			/* ICU_IRQ1 */
	hll_inthdr_entry_66,			/* ICU_IRQ2 */
	hll_inthdr_entry_67,			/* ICU_IRQ3 */
	hll_inthdr_entry_68,			/* ICU_IRQ4 */
	hll_inthdr_entry_69,			/* ICU_IRQ5 */
	hll_inthdr_entry_70,			/* ICU_IRQ6 */
	hll_inthdr_entry_71,			/* ICU_IRQ7 */
	hll_inthdr_entry_72,			/* INT 72 */
	hll_inthdr_entry_73,			/* INT 73 */
	hll_inthdr_entry_74,			/* INT 74 */
	hll_inthdr_entry_75,			/* INT 75 */
	hll_inthdr_entry_76,			/* INT 76 */
	hll_inthdr_entry_77,			/* INT 77 */
	hll_inthdr_entry_78,			/* INT 78 */
	hll_inthdr_entry_79,			/* INT 79 */
	hll_inthdr_entry_80,			/* ELC_ELSR8I */
	hll_inthdr_entry_81,			/* INT 81 */
	hll_inthdr_entry_82,			/* INT 82 */
	hll_inthdr_entry_83,			/* INT 83 */
	hll_inthdr_entry_84,			/* INT 84 */
	hll_inthdr_entry_85,			/* INT 85 */
	hll_inthdr_entry_86,			/* INT 86 */
	hll_inthdr_entry_87,			/* INT 87 */
	hll_inthdr_entry_88,			/* LVD_LVD1 */
	hll_inthdr_entry_89,			/* LVD_LVD2 */
	hll_inthdr_entry_90,			/* USB0_USBR0 */
	hll_inthdr_entry_91,			/* VBATT_VBTLVDI */
	hll_inthdr_entry_92,			/* RTC_ALM */
	hll_inthdr_entry_93,			/* RTC_PRD */
	hll_inthdr_entry_94,			/* INT 94 */
	hll_inthdr_entry_95,			/* INT 95 */
	hll_inthdr_entry_96,			/* INT 96 */
	hll_inthdr_entry_97,			/* INT 97 */
	hll_inthdr_entry_98,			/* INT 98 */
	hll_inthdr_entry_99,			/* INT 99 */
	hll_inthdr_entry_100,			/* INT 100 */
	hll_inthdr_entry_101,			/* INT 101 */
	hll_inthdr_entry_102,			/* S12AD_S12ADI0 */
	hll_inthdr_entry_103,			/* S12AD_GBADI */
	hll_inthdr_entry_104,			/* CMPB1_CMPB2 */
	hll_inthdr_entry_105,			/* CMPB1_CMPB3 */
	hll_inthdr_entry_106,			/* ELC_ELSR18I */
	hll_inthdr_entry_107,			/* ELC_ELSR19I */
	hll_inthdr_entry_108,			/* SSI0_SSIF0 */
	hll_inthdr_entry_109,			/* SSI0_SSIRXI0 */
	hll_inthdr_entry_110,			/* SSI0_SSITXI0 */
	hll_inthdr_entry_111,			/* Secure_RD */
	hll_inthdr_entry_112,			/* Secure_WR */
	hll_inthdr_entry_113,			/* Secure_Error */
	hll_inthdr_entry_114,			/* MTU0_TGIA0 */
	hll_inthdr_entry_115,			/* MTU0_TGIB0 */
	hll_inthdr_entry_116,			/* MTU0_TGIC0 */
	hll_inthdr_entry_117,			/* MTU0_TGID0 */
	hll_inthdr_entry_118,			/* MTU0_TCIV0 */
	hll_inthdr_entry_119,			/* MTU0_TGIE0 */
	hll_inthdr_entry_120,			/* MTU0_TGIF0 */
	hll_inthdr_entry_121,			/* MTU1_TGIA1 */
	hll_inthdr_entry_122,			/* MTU1_TGIB1 */
	hll_inthdr_entry_123,			/* MTU1_TCIV1 */
	hll_inthdr_entry_124,			/* MTU1_TCIU1 */
	hll_inthdr_entry_125,			/* MTU2_TGIA2 */
	hll_inthdr_entry_126,			/* MTU2_TGIB2 */
	hll_inthdr_entry_127,			/* MTU2_TCIV2 */
	hll_inthdr_entry_128,			/* MTU2_TCIU2 */
	hll_inthdr_entry_129,			/* MTU3_TGIA3 */
	hll_inthdr_entry_130,			/* MTU3_TGIB3 */
	hll_inthdr_entry_131,			/* MTU3_TGIC3 */
	hll_inthdr_entry_132,			/* MTU3_TGID3 */
	hll_inthdr_entry_133,			/* MTU3_TCIV3 */
	hll_inthdr_entry_134,			/* MTU4_TGIA4 */
	hll_inthdr_entry_135,			/* MTU4_TGIB4 */
	hll_inthdr_entry_136,			/* MTU4_TGIC4 */
	hll_inthdr_entry_137,			/* MTU4_TGID4 */
	hll_inthdr_entry_138,			/* MTU4_TCIV4 */
	hll_inthdr_entry_139,			/* MTU5_TGIU5 */
	hll_inthdr_entry_140,			/* MTU5_TGIV5 */
	hll_inthdr_entry_141,			/* MTU5_TGIW5 */
	hll_inthdr_entry_142,			/* TPU0_TGI0A */
	hll_inthdr_entry_143,			/* TPU0_TGI0B */
	hll_inthdr_entry_144,			/* TPU0_TGI0C */
	hll_inthdr_entry_145,			/* TPU0_TGI0D */
	hll_inthdr_entry_146,			/* TPU0_TCI0V */
	hll_inthdr_entry_147,			/* TPU1_TGI1A */
	hll_inthdr_entry_148,			/* TPU1_TGI1B */
	hll_inthdr_entry_149,			/* TPU1_TCI1V */
	hll_inthdr_entry_150,			/* TPU1_TCI1U */
	hll_inthdr_entry_151,			/* TPU2_TGI2A */
	hll_inthdr_entry_152,			/* TPU2_TGI2B */
	hll_inthdr_entry_153,			/* TPU2_TCI2V */
	hll_inthdr_entry_154,			/* TPU2_TCI2U */
	hll_inthdr_entry_155,			/* TPU3_TGI3A */
	hll_inthdr_entry_156,			/* TPU3_TGI3B */
	hll_inthdr_entry_157,			/* TPU3_TGI3C */
	hll_inthdr_entry_158,			/* TPU3_TGI3D */
	hll_inthdr_entry_159,			/* TPU3_TCI3V */
	hll_inthdr_entry_160,			/* TPU4_TGI4A */
	hll_inthdr_entry_161,			/* TPU4_TGI4B */
	hll_inthdr_entry_162,			/* TPU4_TCI4V */
	hll_inthdr_entry_163,			/* TPU4_TCI4U */
	hll_inthdr_entry_164,			/* TPU5_TGI5A */
	hll_inthdr_entry_165,			/* TPU5_TGI5B */
	hll_inthdr_entry_166,			/* TPU5_TCI5V */
	hll_inthdr_entry_167,			/* TPU5_TCI5U */
	hll_inthdr_entry_168,			/* INT 168 */
	hll_inthdr_entry_169,			/* INT 169 */
	hll_inthdr_entry_170,			/* POE_OEI1 */
	hll_inthdr_entry_171,			/* POE_OEI2 */
	hll_inthdr_entry_172,			/* INT 172 */
	hll_inthdr_entry_173,			/* INT 173 */
	hll_inthdr_entry_174,			/* TMR0_CMIA0 */
	hll_inthdr_entry_175,			/* TMR0_CMIB0 */
	hll_inthdr_entry_176,			/* TMR0_OVI0 */
	hll_inthdr_entry_177,			/* TMR1_CMIA1 */
	hll_inthdr_entry_178,			/* TMR1_CMIB1 */
	hll_inthdr_entry_179,			/* TMR1_OVI1 */
	hll_inthdr_entry_180,			/* TMR2_CMIA2 */
	hll_inthdr_entry_181,			/* TMR2_CMIB2 */
	hll_inthdr_entry_182,			/* TMR2_OVI2 */
	hll_inthdr_entry_183,			/* TMR3_CMIA3 */
	hll_inthdr_entry_184,			/* TMR3_CMIB3 */
	hll_inthdr_entry_185,			/* TMR3_OVI3 */
	hll_inthdr_entry_186,			/* INT 186 */
	hll_inthdr_entry_187,			/* INT 187 */
	hll_inthdr_entry_188,			/* INT 188 */
	hll_inthdr_entry_189,			/* INT 189 */
	hll_inthdr_entry_190,			/* INT 190 */
	hll_inthdr_entry_191,			/* INT 191 */
	hll_inthdr_entry_192,			/* INT 192 */
	hll_inthdr_entry_193,			/* INT 193 */
	hll_inthdr_entry_194,			/* INT 194 */
	hll_inthdr_entry_195,			/* INT 195 */
	hll_inthdr_entry_196,			/* INT 196 */
	hll_inthdr_entry_197,			/* INT 197 */
	hll_inthdr_entry_198,			/* DMAC_DMAC0I*/
	hll_inthdr_entry_199,			/* DMAC_DMAC1I*/
	hll_inthdr_entry_209,			/* DMAC_DMAC2I*/
	hll_inthdr_entry_201,			/* DMAC_DMAC3I*/
	hll_inthdr_entry_202,			/* INT 202 */
	hll_inthdr_entry_203,			/* INT 203 */
	hll_inthdr_entry_204,			/* INT 204 */
	hll_inthdr_entry_205,			/* INT 205 */
	hll_inthdr_entry_206,			/* INT 206 */
	hll_inthdr_entry_207,			/* INT 207 */
	hll_inthdr_entry_208,			/* INT 208 */
	hll_inthdr_entry_209,			/* INT 209 */
	hll_inthdr_entry_210,			/* INT 210 */
	hll_inthdr_entry_211,			/* INT 211 */
	hll_inthdr_entry_212,			/* INT 212 */
	hll_inthdr_entry_213,			/* INT 213 */
	hll_inthdr_entry_214,			/* SCI0_ERI0 */
	hll_inthdr_entry_215,			/* SCI0_RXI0 */
	hll_inthdr_entry_216,			/* SCI0_TXI0 */
	hll_inthdr_entry_217,			/* SCI0_TEI0 */
	hll_inthdr_entry_218,			/* SCI1_ERI1 */
	hll_inthdr_entry_219,			/* SCI1_RXI1 */
	hll_inthdr_entry_220,			/* SCI1_TXI1 */
	hll_inthdr_entry_221,			/* SCI1_TEI1 */
	hll_inthdr_entry_222,			/* SCI5_ERI5 */
	hll_inthdr_entry_223,			/* SCI5_RXI5 */
	hll_inthdr_entry_224,			/* SCI5_TXI5 */
	hll_inthdr_entry_225,			/* SCI5_TEI5 */
	hll_inthdr_entry_226,			/* SCI6_ERI6 */
	hll_inthdr_entry_227,			/* SCI6_RXI6 */
	hll_inthdr_entry_228,			/* SCI6_TXI6 */
	hll_inthdr_entry_229,			/* SCI6_TEI6 */
	hll_inthdr_entry_230,			/* SCI8_ERI8 */
	hll_inthdr_entry_231,			/* SCI8_RXI8 */
	hll_inthdr_entry_232,			/* SCI8_TXI8 */
	hll_inthdr_entry_233,			/* SCI8_TEI8 */
	hll_inthdr_entry_234,			/* SCI9_ERI9 */
	hll_inthdr_entry_235,			/* SCI9_RXI9 */
	hll_inthdr_entry_236,			/* SCI9_TXI9 */
	hll_inthdr_entry_237,			/* SCI9_TEI9 */
	hll_inthdr_entry_238,			/* SCI12_ERI12 */
	hll_inthdr_entry_239,			/* SCI12_RXI12 */
	hll_inthdr_entry_240,			/* SCI12_TXI12 */
	hll_inthdr_entry_241,			/* SCI12_TEI12 */
	hll_inthdr_entry_242,			/* SCI12_SCIX0 */
	hll_inthdr_entry_243,			/* SCI12_SCIX1 */
	hll_inthdr_entry_244,			/* SCI12_SCIX2 */
	hll_inthdr_entry_245,			/* SCI12_SCIX3 */
	hll_inthdr_entry_246,			/* RIIC0_EEI0 */
	hll_inthdr_entry_247,			/* RIIC0_RXI0 */
	hll_inthdr_entry_248,			/* RIIC0_TXI0 */
	hll_inthdr_entry_249,			/* RIIC0_TEI0 */
	hll_inthdr_entry_250,			/* INT 250 */
	hll_inthdr_entry_251,			/* INT 251 */
	hll_inthdr_entry_252,			/* INT 252 */
	hll_inthdr_entry_253,			/* INT 253 */
	hll_inthdr_entry_254,			/* INT 254 */
	hll_inthdr_entry_255,			/* INT 255 */
};

#endif /* CPU_CORE_RX231 */
