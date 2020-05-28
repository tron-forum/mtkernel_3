/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.01
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/05/29.
 *
 *----------------------------------------------------------------------
 */

/*
 *	hllint_ent.h (RX231)
 *	HLL Interrupt routine entry
 */

#include "kernel.h"
#include "../../sysdepend.h"

#ifndef _SYSDEPEND_CPU_INTVECT_TBL_
#define _SYSDEPEND_CPU_INTVECT_TBL_

/* ------------------------------------------------------------------------ */
/*
 * HLL Interrupt routine entry
 * 
 */
IMPORT	void	hll_inthdr_entry_0(void);			/* Unconditional trap */
IMPORT	void	hll_inthdr_entry_1(void);			/* Dispatch (reserved) */
IMPORT	void	hll_inthdr_entry_2(void);			/* System call (reserved) */
IMPORT	void	hll_inthdr_entry_3(void);			/* System call : tk_ret_int */
IMPORT	void	hll_inthdr_entry_4(void);			/* Debugger support (reserved) */
IMPORT	void	hll_inthdr_entry_5(void);			/* INT 5 */
IMPORT	void	hll_inthdr_entry_6(void);			/* INT 6 */
IMPORT	void	hll_inthdr_entry_7(void);			/* INT 7 */
IMPORT	void	hll_inthdr_entry_8(void);			/* INT 8 */
IMPORT	void	hll_inthdr_entry_9(void);			/* INT 9 */
IMPORT	void	hll_inthdr_entry_10(void);			/* INT 10 */
IMPORT	void	hll_inthdr_entry_11(void);			/* INT 11 */
IMPORT	void	hll_inthdr_entry_12(void);			/* INT 12 */
IMPORT	void	hll_inthdr_entry_13(void);			/* INT 13 */
IMPORT	void	hll_inthdr_entry_14(void);			/* INT 14 */
IMPORT	void	hll_inthdr_entry_15(void);			/* INT 15 */
IMPORT	void	hll_inthdr_entry_16(void);			/* BSC_BUSERR */
IMPORT	void	hll_inthdr_entry_17(void);			/* INT 17 */
IMPORT	void	hll_inthdr_entry_18(void);			/* INT 18 */
IMPORT	void	hll_inthdr_entry_19(void);			/* INT 19 */
IMPORT	void	hll_inthdr_entry_20(void);			/* INT 20 */
IMPORT	void	hll_inthdr_entry_21(void);			/* INT 21 */
IMPORT	void	hll_inthdr_entry_22(void);			/* INT 22 */
IMPORT	void	hll_inthdr_entry_23(void);			/* FCU_FRDYI */
IMPORT	void	hll_inthdr_entry_24(void);			/* INT 24 */
IMPORT	void	hll_inthdr_entry_25(void);			/* INT 25 */
IMPORT	void	hll_inthdr_entry_26(void);			/* INT 26 */
IMPORT	void	hll_inthdr_entry_27(void);			/* ICU_SWINT */
IMPORT	void	hll_inthdr_entry_28(void);			/* CMT0_CMI0 */
IMPORT	void	hll_inthdr_entry_29(void);			/* CMT0_CMI1 */
IMPORT	void	hll_inthdr_entry_30(void);			/* CMT0_CMI2 */
IMPORT	void	hll_inthdr_entry_31(void);			/* CMT0_CMI3 */
IMPORT	void	hll_inthdr_entry_32(void);			/* CAC_FERRF */
IMPORT	void	hll_inthdr_entry_33(void);			/* CAC_MENDF */
IMPORT	void	hll_inthdr_entry_34(void);			/* CAC_OVFF */
IMPORT	void	hll_inthdr_entry_35(void);			/* INT 35 */
IMPORT	void	hll_inthdr_entry_36(void);			/* USB0_D0FIFO0 */
IMPORT	void	hll_inthdr_entry_37(void);			/* USB0_D1FIFO0 */
IMPORT	void	hll_inthdr_entry_38(void);			/* USB0_USBI0*/
IMPORT	void	hll_inthdr_entry_39(void);			/* INT 39 */
IMPORT	void	hll_inthdr_entry_40(void);			/* SDHI_SBFAI */
IMPORT	void	hll_inthdr_entry_41(void);			/* SDHI_CDETI */
IMPORT	void	hll_inthdr_entry_42(void);			/* SDHI_CACI */
IMPORT	void	hll_inthdr_entry_43(void);			/* SDHI_SDACI */
IMPORT	void	hll_inthdr_entry_44(void);			/* RSPI0_SPEI0 */
IMPORT	void	hll_inthdr_entry_45(void);			/* RSPI0_SPRI0 */
IMPORT	void	hll_inthdr_entry_46(void);			/* RSPI0_SPTI0 */
IMPORT	void	hll_inthdr_entry_47(void);			/* RSPI0_SPII0 */
IMPORT	void	hll_inthdr_entry_48(void);			/* INT 48 */
IMPORT	void	hll_inthdr_entry_49(void);			/* INT 49 */
IMPORT	void	hll_inthdr_entry_50(void);			/* INT 50 */
IMPORT	void	hll_inthdr_entry_51(void);			/* INT 51 */
IMPORT	void	hll_inthdr_entry_52(void);			/* RSCAN_COMFRXINT */
IMPORT	void	hll_inthdr_entry_53(void);			/* RSCAN_RXFINT */
IMPORT	void	hll_inthdr_entry_54(void);			/* RSCAN_TXINT */
IMPORT	void	hll_inthdr_entry_55(void);			/* RSCAN_CHERRINT */
IMPORT	void	hll_inthdr_entry_56(void);			/* RSCAN_GLERRINT */
IMPORT	void	hll_inthdr_entry_57(void);			/* DOC_DOPCF */
IMPORT	void	hll_inthdr_entry_58(void);			/* CMPB_CMPB0 */
IMPORT	void	hll_inthdr_entry_59(void);			/* CMPB_CMPB1 */
IMPORT	void	hll_inthdr_entry_60(void);			/* CTSU_CTSUWR */
IMPORT	void	hll_inthdr_entry_61(void);			/* CTSU_CTSURD */
IMPORT	void	hll_inthdr_entry_62(void);			/* CTSU_CTSUFN */
IMPORT	void	hll_inthdr_entry_63(void);			/* RTC_CUP */
IMPORT	void	hll_inthdr_entry_64(void);			/* ICU_IRQ0 */
IMPORT	void	hll_inthdr_entry_65(void);			/* ICU_IRQ1 */
IMPORT	void	hll_inthdr_entry_66(void);			/* ICU_IRQ2 */
IMPORT	void	hll_inthdr_entry_67(void);			/* ICU_IRQ3 */
IMPORT	void	hll_inthdr_entry_68(void);			/* ICU_IRQ4 */
IMPORT	void	hll_inthdr_entry_69(void);			/* ICU_IRQ5 */
IMPORT	void	hll_inthdr_entry_70(void);			/* ICU_IRQ6 */
IMPORT	void	hll_inthdr_entry_71(void);			/* ICU_IRQ7 */
IMPORT	void	hll_inthdr_entry_72(void);			/* INT 72 */
IMPORT	void	hll_inthdr_entry_73(void);			/* INT 73 */
IMPORT	void	hll_inthdr_entry_74(void);			/* INT 74 */
IMPORT	void	hll_inthdr_entry_75(void);			/* INT 75 */
IMPORT	void	hll_inthdr_entry_76(void);			/* INT 76 */
IMPORT	void	hll_inthdr_entry_77(void);			/* INT 77 */
IMPORT	void	hll_inthdr_entry_78(void);			/* INT 78 */
IMPORT	void	hll_inthdr_entry_79(void);			/* INT 79 */
IMPORT	void	hll_inthdr_entry_80(void);			/* ELC_ELSR8I */
IMPORT	void	hll_inthdr_entry_81(void);			/* INT 81 */
IMPORT	void	hll_inthdr_entry_82(void);			/* INT 82 */
IMPORT	void	hll_inthdr_entry_83(void);			/* INT 83 */
IMPORT	void	hll_inthdr_entry_84(void);			/* INT 84 */
IMPORT	void	hll_inthdr_entry_85(void);			/* INT 85 */
IMPORT	void	hll_inthdr_entry_86(void);			/* INT 86 */
IMPORT	void	hll_inthdr_entry_87(void);			/* INT 87 */
IMPORT	void	hll_inthdr_entry_88(void);			/* LVD_LVD1 */
IMPORT	void	hll_inthdr_entry_89(void);			/* LVD_LVD2 */
IMPORT	void	hll_inthdr_entry_90(void);			/* USB0_USBR0 */
IMPORT	void	hll_inthdr_entry_91(void);			/* VBATT_VBTLVDI */
IMPORT	void	hll_inthdr_entry_92(void);			/* RTC_ALM */
IMPORT	void	hll_inthdr_entry_93(void);			/* RTC_PRD */
IMPORT	void	hll_inthdr_entry_94(void);			/* INT 94 */
IMPORT	void	hll_inthdr_entry_95(void);			/* INT 95 */
IMPORT	void	hll_inthdr_entry_96(void);			/* INT 96 */
IMPORT	void	hll_inthdr_entry_97(void);			/* INT 97 */
IMPORT	void	hll_inthdr_entry_98(void);			/* INT 98 */
IMPORT	void	hll_inthdr_entry_99(void);			/* INT 99 */
IMPORT	void	hll_inthdr_entry_100(void);			/* INT 100 */
IMPORT	void	hll_inthdr_entry_101(void);			/* INT 101 */
IMPORT	void	hll_inthdr_entry_102(void);			/* S12AD_S12ADI0 */
IMPORT	void	hll_inthdr_entry_103(void);			/* S12AD_GBADI */
IMPORT	void	hll_inthdr_entry_104(void);			/* CMPB1_CMPB2 */
IMPORT	void	hll_inthdr_entry_105(void);			/* CMPB1_CMPB3 */
IMPORT	void	hll_inthdr_entry_106(void);			/* ELC_ELSR18I */
IMPORT	void	hll_inthdr_entry_107(void);			/* ELC_ELSR19I */
IMPORT	void	hll_inthdr_entry_108(void);			/* SSI0_SSIF0 */
IMPORT	void	hll_inthdr_entry_109(void);			/* SSI0_SSIRXI0 */
IMPORT	void	hll_inthdr_entry_110(void);			/* SSI0_SSITXI0 */
IMPORT	void	hll_inthdr_entry_111(void);			/* Secure_RD */
IMPORT	void	hll_inthdr_entry_112(void);			/* Secure_WR */
IMPORT	void	hll_inthdr_entry_113(void);			/* Secure_Error */
IMPORT	void	hll_inthdr_entry_114(void);			/* MTU0_TGIA0 */
IMPORT	void	hll_inthdr_entry_115(void);			/* MTU0_TGIB0 */
IMPORT	void	hll_inthdr_entry_116(void);			/* MTU0_TGIC0 */
IMPORT	void	hll_inthdr_entry_117(void);			/* MTU0_TGID0 */
IMPORT	void	hll_inthdr_entry_118(void);			/* MTU0_TCIV0 */
IMPORT	void	hll_inthdr_entry_119(void);			/* MTU0_TGIE0 */
IMPORT	void	hll_inthdr_entry_120(void);			/* MTU0_TGIF0 */
IMPORT	void	hll_inthdr_entry_121(void);			/* MTU1_TGIA1 */
IMPORT	void	hll_inthdr_entry_122(void);			/* MTU1_TGIB1 */
IMPORT	void	hll_inthdr_entry_123(void);			/* MTU1_TCIV1 */
IMPORT	void	hll_inthdr_entry_124(void);			/* MTU1_TCIU1 */
IMPORT	void	hll_inthdr_entry_125(void);			/* MTU2_TGIA2 */
IMPORT	void	hll_inthdr_entry_126(void);			/* MTU2_TGIB2 */
IMPORT	void	hll_inthdr_entry_127(void);			/* MTU2_TCIV2 */
IMPORT	void	hll_inthdr_entry_128(void);			/* MTU2_TCIU2 */
IMPORT	void	hll_inthdr_entry_129(void);			/* MTU3_TGIA3 */
IMPORT	void	hll_inthdr_entry_130(void);			/* MTU3_TGIB3 */
IMPORT	void	hll_inthdr_entry_131(void);			/* MTU3_TGIC3 */
IMPORT	void	hll_inthdr_entry_132(void);			/* MTU3_TGID3 */
IMPORT	void	hll_inthdr_entry_133(void);			/* MTU3_TCIV3 */
IMPORT	void	hll_inthdr_entry_134(void);			/* MTU4_TGIA4 */
IMPORT	void	hll_inthdr_entry_135(void);			/* MTU4_TGIB4 */
IMPORT	void	hll_inthdr_entry_136(void);			/* MTU4_TGIC4 */
IMPORT	void	hll_inthdr_entry_137(void);			/* MTU4_TGID4 */
IMPORT	void	hll_inthdr_entry_138(void);			/* MTU4_TCIV4 */
IMPORT	void	hll_inthdr_entry_139(void);			/* MTU5_TGIU5 */
IMPORT	void	hll_inthdr_entry_140(void);			/* MTU5_TGIV5 */
IMPORT	void	hll_inthdr_entry_141(void);			/* MTU5_TGIW5 */
IMPORT	void	hll_inthdr_entry_142(void);			/* TPU0_TGI0A */
IMPORT	void	hll_inthdr_entry_143(void);			/* TPU0_TGI0B */
IMPORT	void	hll_inthdr_entry_144(void);			/* TPU0_TGI0C */
IMPORT	void	hll_inthdr_entry_145(void);			/* TPU0_TGI0D */
IMPORT	void	hll_inthdr_entry_146(void);			/* TPU0_TCI0V */
IMPORT	void	hll_inthdr_entry_147(void);			/* TPU1_TGI1A */
IMPORT	void	hll_inthdr_entry_148(void);			/* TPU1_TGI1B */
IMPORT	void	hll_inthdr_entry_149(void);			/* TPU1_TCI1V */
IMPORT	void	hll_inthdr_entry_150(void);			/* TPU1_TCI1U */
IMPORT	void	hll_inthdr_entry_151(void);			/* TPU2_TGI2A */
IMPORT	void	hll_inthdr_entry_152(void);			/* TPU2_TGI2B */
IMPORT	void	hll_inthdr_entry_153(void);			/* TPU2_TCI2V */
IMPORT	void	hll_inthdr_entry_154(void);			/* TPU2_TCI2U */
IMPORT	void	hll_inthdr_entry_155(void);			/* TPU3_TGI3A */
IMPORT	void	hll_inthdr_entry_156(void);			/* TPU3_TGI3B */
IMPORT	void	hll_inthdr_entry_157(void);			/* TPU3_TGI3C */
IMPORT	void	hll_inthdr_entry_158(void);			/* TPU3_TGI3D */
IMPORT	void	hll_inthdr_entry_159(void);			/* TPU3_TCI3V */
IMPORT	void	hll_inthdr_entry_160(void);			/* TPU4_TGI4A */
IMPORT	void	hll_inthdr_entry_161(void);			/* TPU4_TGI4B */
IMPORT	void	hll_inthdr_entry_162(void);			/* TPU4_TCI4V */
IMPORT	void	hll_inthdr_entry_163(void);			/* TPU4_TCI4U */
IMPORT	void	hll_inthdr_entry_164(void);			/* TPU5_TGI5A */
IMPORT	void	hll_inthdr_entry_165(void);			/* TPU5_TGI5B */
IMPORT	void	hll_inthdr_entry_166(void);			/* TPU5_TCI5V */
IMPORT	void	hll_inthdr_entry_167(void);			/* TPU5_TCI5U */
IMPORT	void	hll_inthdr_entry_168(void);			/* INT 168 */
IMPORT	void	hll_inthdr_entry_169(void);			/* INT 169 */
IMPORT	void	hll_inthdr_entry_170(void);			/* POE_OEI1 */
IMPORT	void	hll_inthdr_entry_171(void);			/* POE_OEI2 */
IMPORT	void	hll_inthdr_entry_172(void);			/* INT 172 */
IMPORT	void	hll_inthdr_entry_173(void);			/* INT 173 */
IMPORT	void	hll_inthdr_entry_174(void);			/* TMR0_CMIA0 */
IMPORT	void	hll_inthdr_entry_175(void);			/* TMR0_CMIB0 */
IMPORT	void	hll_inthdr_entry_176(void);			/* TMR0_OVI0 */
IMPORT	void	hll_inthdr_entry_177(void);			/* TMR1_CMIA1 */
IMPORT	void	hll_inthdr_entry_178(void);			/* TMR1_CMIB1 */
IMPORT	void	hll_inthdr_entry_179(void);			/* TMR1_OVI1 */
IMPORT	void	hll_inthdr_entry_180(void);			/* TMR2_CMIA2 */
IMPORT	void	hll_inthdr_entry_181(void);			/* TMR2_CMIB2 */
IMPORT	void	hll_inthdr_entry_182(void);			/* TMR2_OVI2 */
IMPORT	void	hll_inthdr_entry_183(void);			/* TMR3_CMIA3 */
IMPORT	void	hll_inthdr_entry_184(void);			/* TMR3_CMIB3 */
IMPORT	void	hll_inthdr_entry_185(void);			/* TMR3_OVI3 */
IMPORT	void	hll_inthdr_entry_186(void);			/* INT 186 */
IMPORT	void	hll_inthdr_entry_187(void);			/* INT 187 */
IMPORT	void	hll_inthdr_entry_188(void);			/* INT 188 */
IMPORT	void	hll_inthdr_entry_189(void);			/* INT 189 */
IMPORT	void	hll_inthdr_entry_190(void);			/* INT 190 */
IMPORT	void	hll_inthdr_entry_191(void);			/* INT 191 */
IMPORT	void	hll_inthdr_entry_192(void);			/* INT 192 */
IMPORT	void	hll_inthdr_entry_193(void);			/* INT 193 */
IMPORT	void	hll_inthdr_entry_194(void);			/* INT 194 */
IMPORT	void	hll_inthdr_entry_195(void);			/* INT 195 */
IMPORT	void	hll_inthdr_entry_196(void);			/* INT 196 */
IMPORT	void	hll_inthdr_entry_197(void);			/* INT 197 */
IMPORT	void	hll_inthdr_entry_198(void);			/* DMAC_DMAC0I*/
IMPORT	void	hll_inthdr_entry_199(void);			/* DMAC_DMAC1I*/
IMPORT	void	hll_inthdr_entry_209(void);			/* DMAC_DMAC2I*/
IMPORT	void	hll_inthdr_entry_201(void);			/* DMAC_DMAC3I*/
IMPORT	void	hll_inthdr_entry_202(void);			/* INT 202 */
IMPORT	void	hll_inthdr_entry_203(void);			/* INT 203 */
IMPORT	void	hll_inthdr_entry_204(void);			/* INT 204 */
IMPORT	void	hll_inthdr_entry_205(void);			/* INT 205 */
IMPORT	void	hll_inthdr_entry_206(void);			/* INT 206 */
IMPORT	void	hll_inthdr_entry_207(void);			/* INT 207 */
IMPORT	void	hll_inthdr_entry_208(void);			/* INT 208 */
IMPORT	void	hll_inthdr_entry_209(void);			/* INT 209 */
IMPORT	void	hll_inthdr_entry_210(void);			/* INT 210 */
IMPORT	void	hll_inthdr_entry_211(void);			/* INT 211 */
IMPORT	void	hll_inthdr_entry_212(void);			/* INT 212 */
IMPORT	void	hll_inthdr_entry_213(void);			/* INT 213 */
IMPORT	void	hll_inthdr_entry_214(void);			/* SCI0_ERI0 */
IMPORT	void	hll_inthdr_entry_215(void);			/* SCI0_RXI0 */
IMPORT	void	hll_inthdr_entry_216(void);			/* SCI0_TXI0 */
IMPORT	void	hll_inthdr_entry_217(void);			/* SCI0_TEI0 */
IMPORT	void	hll_inthdr_entry_218(void);			/* SCI1_ERI1 */
IMPORT	void	hll_inthdr_entry_219(void);			/* SCI1_RXI1 */
IMPORT	void	hll_inthdr_entry_220(void);			/* SCI1_TXI1 */
IMPORT	void	hll_inthdr_entry_221(void);			/* SCI1_TEI1 */
IMPORT	void	hll_inthdr_entry_222(void);			/* SCI5_ERI5 */
IMPORT	void	hll_inthdr_entry_223(void);			/* SCI5_RXI5 */
IMPORT	void	hll_inthdr_entry_224(void);			/* SCI5_TXI5 */
IMPORT	void	hll_inthdr_entry_225(void);			/* SCI5_TEI5 */
IMPORT	void	hll_inthdr_entry_226(void);			/* SCI6_ERI6 */
IMPORT	void	hll_inthdr_entry_227(void);			/* SCI6_RXI6 */
IMPORT	void	hll_inthdr_entry_228(void);			/* SCI6_TXI6 */
IMPORT	void	hll_inthdr_entry_229(void);			/* SCI6_TEI6 */
IMPORT	void	hll_inthdr_entry_230(void);			/* SCI8_ERI8 */
IMPORT	void	hll_inthdr_entry_231(void);			/* SCI8_RXI8 */
IMPORT	void	hll_inthdr_entry_232(void);			/* SCI8_TXI8 */
IMPORT	void	hll_inthdr_entry_233(void);			/* SCI8_TEI8 */
IMPORT	void	hll_inthdr_entry_234(void);			/* SCI9_ERI9 */
IMPORT	void	hll_inthdr_entry_235(void);			/* SCI9_RXI9 */
IMPORT	void	hll_inthdr_entry_236(void);			/* SCI9_TXI9 */
IMPORT	void	hll_inthdr_entry_237(void);			/* SCI9_TEI9 */
IMPORT	void	hll_inthdr_entry_238(void);			/* SCI12_ERI12 */
IMPORT	void	hll_inthdr_entry_239(void);			/* SCI12_RXI12 */
IMPORT	void	hll_inthdr_entry_240(void);			/* SCI12_TXI12 */
IMPORT	void	hll_inthdr_entry_241(void);			/* SCI12_TEI12 */
IMPORT	void	hll_inthdr_entry_242(void);			/* SCI12_SCIX0 */
IMPORT	void	hll_inthdr_entry_243(void);			/* SCI12_SCIX1 */
IMPORT	void	hll_inthdr_entry_244(void);			/* SCI12_SCIX2 */
IMPORT	void	hll_inthdr_entry_245(void);			/* SCI12_SCIX3 */
IMPORT	void	hll_inthdr_entry_246(void);			/* RIIC0_EEI0 */
IMPORT	void	hll_inthdr_entry_247(void);			/* RIIC0_RXI0 */
IMPORT	void	hll_inthdr_entry_248(void);			/* RIIC0_TXI0 */
IMPORT	void	hll_inthdr_entry_249(void);			/* RIIC0_TEI0 */
IMPORT	void	hll_inthdr_entry_250(void);			/* INT 250 */
IMPORT	void	hll_inthdr_entry_251(void);			/* INT 251 */
IMPORT	void	hll_inthdr_entry_252(void);			/* INT 252 */
IMPORT	void	hll_inthdr_entry_253(void);			/* INT 253 */
IMPORT	void	hll_inthdr_entry_254(void);			/* INT 254 */
IMPORT	void	hll_inthdr_entry_255(void);			/* INT 255 */

#endif /* _SYSDEPEND_CPU_INTVECT_TBL_ */
