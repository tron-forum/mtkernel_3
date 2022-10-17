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
 *	intvect_tbl.c (RX65N)
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
	hll_inthdr_entry_18,			/* RAM_RAMERR */
	hll_inthdr_entry_19,			/* INT 19 */
	hll_inthdr_entry_20,			/* INT 20 */
	hll_inthdr_entry_21,			/* FCU_FIFERR */
	hll_inthdr_entry_22,			/* INT 22 */
	hll_inthdr_entry_23,			/* FCU_FRDYI */
	hll_inthdr_entry_24,			/* INT 24 */
	hll_inthdr_entry_25,			/* INT 25 */
	hll_inthdr_entry_26,			/* ICU_SWINT2 */
	hll_inthdr_entry_27,			/* ICU_SWINT */
	hll_inthdr_entry_28,			/* CMT0_CMI0 */
	hll_inthdr_entry_29,			/* CMT0_CMI1 */
	hll_inthdr_entry_30,			/* CMTW0_CMWI0 */
	hll_inthdr_entry_31,			/* CMTW1_CMWI1 */
	hll_inthdr_entry_32,			/* INT 32 */
	hll_inthdr_entry_33,			/* INT 33 */
	hll_inthdr_entry_34,			/* USB0_D0FIFO0 */
	hll_inthdr_entry_35,			/* USB0_D1FIFO0 */
	hll_inthdr_entry_36,			/* INT 36 */
	hll_inthdr_entry_37,			/* INT 37 */
	hll_inthdr_entry_38,			/* RSPI0_SPRI0 */
	hll_inthdr_entry_39,			/* RSPI0_SPTI0 */
	hll_inthdr_entry_40,			/* RSPI1_SPRI1 */
	hll_inthdr_entry_41,			/* RSPI1_SPTI1 */
	hll_inthdr_entry_42,			/* QSPI_SPRI */
	hll_inthdr_entry_43,			/* QSPI_SPTI */
	hll_inthdr_entry_44,			/* SDHI_SBFAI */
	hll_inthdr_entry_45,			/* MMCIF_MBFAI */
	hll_inthdr_entry_46,			/* INT 46 */
	hll_inthdr_entry_47,			/* INT 47 */
	hll_inthdr_entry_48,			/* INT 48 */
	hll_inthdr_entry_49,			/* INT 49 */
	hll_inthdr_entry_50,			/* RIIC1_RXI1 */
	hll_inthdr_entry_51,			/* RIIC1_TXI1 */
	hll_inthdr_entry_52,			/* RIIC0_RXI0 */
	hll_inthdr_entry_53,			/* RIIC0_TXI0 */
	hll_inthdr_entry_54,			/* RIIC2_RXI2 */
	hll_inthdr_entry_55,			/* RIIC2_TXI2 */
	hll_inthdr_entry_56,			/* INT 56 */
	hll_inthdr_entry_57,			/* INT 57 */
	hll_inthdr_entry_58,			/* SCI0_RXI0 */
	hll_inthdr_entry_59,			/* SCI0_TXI0 */
	hll_inthdr_entry_60,			/* SCI1_RXI1 */
	hll_inthdr_entry_61,			/* SCI1_TXI1 */
	hll_inthdr_entry_62,			/* SCI2_RXI2 */
	hll_inthdr_entry_63,			/* SCI2_TXI2 */
	hll_inthdr_entry_64,			/* ICU_IRQ0 */
	hll_inthdr_entry_65,			/* ICU_IRQ1 */
	hll_inthdr_entry_66,			/* ICU_IRQ2 */
	hll_inthdr_entry_67,			/* ICU_IRQ3 */
	hll_inthdr_entry_68,			/* ICU_IRQ4 */
	hll_inthdr_entry_69,			/* ICU_IRQ5 */
	hll_inthdr_entry_70,			/* ICU_IRQ6 */
	hll_inthdr_entry_71,			/* ICU_IRQ7 */
	hll_inthdr_entry_72,			/* ICU_IRQ8 */
	hll_inthdr_entry_73,			/* ICU_IRQ9 */
	hll_inthdr_entry_74,			/* ICU_IRQ10 */
	hll_inthdr_entry_75,			/* ICU_IRQ11 */
	hll_inthdr_entry_76,			/* ICU_IRQ12 */
	hll_inthdr_entry_77,			/* ICU_IRQ13 */
	hll_inthdr_entry_78,			/* ICU_IRQ14 */
	hll_inthdr_entry_79,			/* ICU_IRQ15 */
	hll_inthdr_entry_80,			/* SCI3_RXI3 */
	hll_inthdr_entry_81,			/* SCI3_TXI3 */
	hll_inthdr_entry_82,			/* SCI4_RXI4 */
	hll_inthdr_entry_83,			/* SCI4_TXI4 */
	hll_inthdr_entry_84,			/* SCI5_RXI5 */
	hll_inthdr_entry_85,			/* SCI5_TXI5 */
	hll_inthdr_entry_86,			/* SCI6_RXI6 */
	hll_inthdr_entry_87,			/* SCI6_TXI6 */
	hll_inthdr_entry_88,			/* LVD_LVD1 */
	hll_inthdr_entry_89,			/* LVD_LVD2 */
	hll_inthdr_entry_90,			/* USB0_USBR0 */
	hll_inthdr_entry_91,			/* SCI6_RXI6 */
	hll_inthdr_entry_92,			/* RTC_ALM */
	hll_inthdr_entry_93,			/* RTC_PRD */
	hll_inthdr_entry_94,			/* INT 94 */
	hll_inthdr_entry_95,			/* IWDT_IWUNI */
	hll_inthdr_entry_96,			/* WDT_WUNI */
	hll_inthdr_entry_97,			/* RDC_PCDFI */
	hll_inthdr_entry_98,			/* SCI7_RXI7 */
	hll_inthdr_entry_99,			/* SCI7_TXI7 */
	hll_inthdr_entry_100,			/* SCI8_RXI8 */
	hll_inthdr_entry_101,			/* SCI8_TXI8 */
	hll_inthdr_entry_102,			/* SCI9_RXI9 */
	hll_inthdr_entry_103,			/* SCI9_TXI9 */
	hll_inthdr_entry_104,			/* SCI10_RXI10 */
	hll_inthdr_entry_105,			/* SCI10_TXI10 */
	hll_inthdr_entry_106,			/* ICU_GROUPBE0 */
	hll_inthdr_entry_107,			/* ICU_GROUPBL2 */
	hll_inthdr_entry_108,			/* RSPI2_SPRI2 */
	hll_inthdr_entry_109,			/* RSPI2_SPTI2 */
	hll_inthdr_entry_110,			/* ICU_GROUPBL0 */
	hll_inthdr_entry_111,			/* ICU_GROUPBL1 */
	hll_inthdr_entry_112,			/* ICU_GROUPAL0 */
	hll_inthdr_entry_113,			/* ICU_GROUPAL1 */
	hll_inthdr_entry_114,			/* SCI11_RXI11 */
	hll_inthdr_entry_115,			/* SCI11_TXI11 */
	hll_inthdr_entry_116,			/* SCI12_RXI12 */
	hll_inthdr_entry_117,			/* SCI12_TXI12 */
	hll_inthdr_entry_118,			/* INT 118 */
	hll_inthdr_entry_119,			/* INT 119 */
	hll_inthdr_entry_120,			/* DMAC_DMAC0I */
	hll_inthdr_entry_121,			/* DMAC_DMAC1I */
	hll_inthdr_entry_122,			/* DMAC_DMAC2I */
	hll_inthdr_entry_123,			/* DMAC_DMAC3I */
	hll_inthdr_entry_124,			/* DMAC_DMAC74I */
	hll_inthdr_entry_125,			/* OST_OSTDI */
	hll_inthdr_entry_126,			/* EXDMAC_EXDMAC0I */
	hll_inthdr_entry_127,			/* EXDMAC_EXDMAC1I */
	hll_inthdr_entry_128,			/* INTB 128 */
	hll_inthdr_entry_129,			/* INTB 129 */
	hll_inthdr_entry_130,			/* INTB 130 */
	hll_inthdr_entry_131,			/* INTB 131 */
	hll_inthdr_entry_132,			/* INTB 132 */
	hll_inthdr_entry_133,			/* INTB 133 */
	hll_inthdr_entry_134,			/* INTB 134 */
	hll_inthdr_entry_135,			/* INTB 135 */
	hll_inthdr_entry_136,			/* INTB 136 */
	hll_inthdr_entry_137,			/* INTB 137 */
	hll_inthdr_entry_138,			/* INTB 138 */
	hll_inthdr_entry_139,			/* INTB 139 */
	hll_inthdr_entry_140,			/* INTB 140 */
	hll_inthdr_entry_141,			/* INTB 141 */
	hll_inthdr_entry_142,			/* INTB 142 */
	hll_inthdr_entry_143,			/* INTB 143 */
	hll_inthdr_entry_144,			/* INTB 144 */
	hll_inthdr_entry_145,			/* INTB 145 */
	hll_inthdr_entry_146,			/* INTB 146 */
	hll_inthdr_entry_147,			/* INTB 147 */
	hll_inthdr_entry_148,			/* INTB 148 */
	hll_inthdr_entry_149,			/* INTB 149 */
	hll_inthdr_entry_150,			/* INTB 150 */
	hll_inthdr_entry_151,			/* INTB 151 */
	hll_inthdr_entry_152,			/* INTB 152 */
	hll_inthdr_entry_153,			/* INTB 153 */
	hll_inthdr_entry_154,			/* INTB 154 */
	hll_inthdr_entry_155,			/* INTB 155 */
	hll_inthdr_entry_156,			/* INTB 156 */
	hll_inthdr_entry_157,			/* INTB 157 */
	hll_inthdr_entry_158,			/* INTB 158 */
	hll_inthdr_entry_159,			/* INTB 159 */
	hll_inthdr_entry_160,			/* INTB 160 */
	hll_inthdr_entry_161,			/* INTB 161 */
	hll_inthdr_entry_162,			/* INTB 162 */
	hll_inthdr_entry_163,			/* INTB 163 */
	hll_inthdr_entry_164,			/* INTB 164 */
	hll_inthdr_entry_165,			/* INTB 165 */
	hll_inthdr_entry_166,			/* INTB 166 */
	hll_inthdr_entry_167,			/* INTB 167 */
	hll_inthdr_entry_168,			/* INTB 168 */
	hll_inthdr_entry_169,			/* INTB 169 */
	hll_inthdr_entry_170,			/* INTB 170 */
	hll_inthdr_entry_171,			/* INTB 171 */
	hll_inthdr_entry_172,			/* INTB 172 */
	hll_inthdr_entry_173,			/* INTB 173 */
	hll_inthdr_entry_174,			/* INTB 174 */
	hll_inthdr_entry_175,			/* INTB 175 */
	hll_inthdr_entry_176,			/* INTB 176 */
	hll_inthdr_entry_177,			/* INTB 177 */
	hll_inthdr_entry_178,			/* INTB 178 */
	hll_inthdr_entry_179,			/* INTB 179 */
	hll_inthdr_entry_180,			/* INTB 180 */
	hll_inthdr_entry_181,			/* INTB 181 */
	hll_inthdr_entry_182,			/* INTB 182 */
	hll_inthdr_entry_183,			/* INTB 183 */
	hll_inthdr_entry_184,			/* INTB 184 */
	hll_inthdr_entry_185,			/* INTB 185 */
	hll_inthdr_entry_186,			/* INTB 186 */
	hll_inthdr_entry_187,			/* INTB 187 */
	hll_inthdr_entry_188,			/* INTB 188 */
	hll_inthdr_entry_189,			/* INTB 189 */
	hll_inthdr_entry_190,			/* INTB 190 */
	hll_inthdr_entry_191,			/* INTB 191 */
	hll_inthdr_entry_192,			/* INTB 192 */
	hll_inthdr_entry_193,			/* INTB 193 */
	hll_inthdr_entry_194,			/* INTB 194 */
	hll_inthdr_entry_195,			/* INTB 195 */
	hll_inthdr_entry_196,			/* INTB 196 */
	hll_inthdr_entry_197,			/* INTB 197 */
	hll_inthdr_entry_198,			/* INTB 198 */
	hll_inthdr_entry_199,			/* INTB 199 */
	hll_inthdr_entry_209,			/* INTB 209 */
	hll_inthdr_entry_201,			/* INTB 201 */
	hll_inthdr_entry_202,			/* INTB 202 */
	hll_inthdr_entry_203,			/* INTB 203 */
	hll_inthdr_entry_204,			/* INTB 204 */
	hll_inthdr_entry_205,			/* INTB 205 */
	hll_inthdr_entry_206,			/* INTB 206 */
	hll_inthdr_entry_207,			/* INTB 207 */
	hll_inthdr_entry_208,			/* INTA 208 */
	hll_inthdr_entry_209,			/* INTA 209 */
	hll_inthdr_entry_210,			/* INTA 210 */
	hll_inthdr_entry_211,			/* INTA 211 */
	hll_inthdr_entry_212,			/* INTA 212 */
	hll_inthdr_entry_213,			/* INTA 213 */
	hll_inthdr_entry_214,			/* INTA 214 */
	hll_inthdr_entry_215,			/* INTA 215 */
	hll_inthdr_entry_216,			/* INTA 216 */
	hll_inthdr_entry_217,			/* INTA 217 */
	hll_inthdr_entry_218,			/* INTA 218 */
	hll_inthdr_entry_219,			/* INTA 219 */
	hll_inthdr_entry_220,			/* INTA 220 */
	hll_inthdr_entry_221,			/* INTA 221 */
	hll_inthdr_entry_222,			/* INTA 222 */
	hll_inthdr_entry_223,			/* INTA 223 */
	hll_inthdr_entry_224,			/* INTA 224 */
	hll_inthdr_entry_225,			/* INTA 225 */
	hll_inthdr_entry_226,			/* INTA 226 */
	hll_inthdr_entry_227,			/* INTA 227 */
	hll_inthdr_entry_228,			/* INTA 228 */
	hll_inthdr_entry_229,			/* INTA 229 */
	hll_inthdr_entry_230,			/* INTA 230 */
	hll_inthdr_entry_231,			/* INTA 231 */
	hll_inthdr_entry_232,			/* INTA 232 */
	hll_inthdr_entry_233,			/* INTA 233 */
	hll_inthdr_entry_234,			/* INTA 234 */
	hll_inthdr_entry_235,			/* INTA 235 */
	hll_inthdr_entry_236,			/* INTA 236 */
	hll_inthdr_entry_237,			/* INTA 237 */
	hll_inthdr_entry_238,			/* INTA 238 */
	hll_inthdr_entry_239,			/* INTA 239 */
	hll_inthdr_entry_240,			/* INTA 240 */
	hll_inthdr_entry_241,			/* INTA 241 */
	hll_inthdr_entry_242,			/* INTA 242 */
	hll_inthdr_entry_243,			/* INTA 243 */
	hll_inthdr_entry_244,			/* INTA 244 */
	hll_inthdr_entry_245,			/* INTA 245 */
	hll_inthdr_entry_246,			/* INTA 246 */
	hll_inthdr_entry_247,			/* INTA 247 */
	hll_inthdr_entry_248,			/* INTA 248 */
	hll_inthdr_entry_249,			/* INTA 249 */
	hll_inthdr_entry_250,			/* INTA 250 */
	hll_inthdr_entry_251,			/* INTA 251 */
	hll_inthdr_entry_252,			/* INTA 252 */
	hll_inthdr_entry_253,			/* INTA 253 */
	hll_inthdr_entry_254,			/* INTA 254 */
	hll_inthdr_entry_255,			/* INTA 255 */
};

#endif /* CPU_RX65N */
