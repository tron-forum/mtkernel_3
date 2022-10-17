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

/*
 *	hllint_ent.h (RX65N)
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
IMPORT	void	hll_inthdr_entry_0(void);
IMPORT	void	hll_inthdr_entry_1(void);
IMPORT	void	hll_inthdr_entry_2(void);
IMPORT	void	hll_inthdr_entry_3(void);
IMPORT	void	hll_inthdr_entry_4(void);
IMPORT	void	hll_inthdr_entry_5(void);
IMPORT	void	hll_inthdr_entry_6(void);
IMPORT	void	hll_inthdr_entry_7(void);
IMPORT	void	hll_inthdr_entry_8(void);
IMPORT	void	hll_inthdr_entry_9(void);
IMPORT	void	hll_inthdr_entry_10(void);
IMPORT	void	hll_inthdr_entry_11(void);
IMPORT	void	hll_inthdr_entry_12(void);
IMPORT	void	hll_inthdr_entry_13(void);
IMPORT	void	hll_inthdr_entry_14(void);
IMPORT	void	hll_inthdr_entry_15(void);
IMPORT	void	hll_inthdr_entry_16(void);
IMPORT	void	hll_inthdr_entry_17(void);
IMPORT	void	hll_inthdr_entry_18(void);
IMPORT	void	hll_inthdr_entry_19(void);
IMPORT	void	hll_inthdr_entry_20(void);
IMPORT	void	hll_inthdr_entry_21(void);
IMPORT	void	hll_inthdr_entry_22(void);
IMPORT	void	hll_inthdr_entry_23(void);
IMPORT	void	hll_inthdr_entry_24(void);
IMPORT	void	hll_inthdr_entry_25(void);
IMPORT	void	hll_inthdr_entry_26(void);
IMPORT	void	hll_inthdr_entry_27(void);
IMPORT	void	hll_inthdr_entry_28(void);
IMPORT	void	hll_inthdr_entry_29(void);
IMPORT	void	hll_inthdr_entry_30(void);
IMPORT	void	hll_inthdr_entry_31(void);
IMPORT	void	hll_inthdr_entry_32(void);
IMPORT	void	hll_inthdr_entry_33(void);
IMPORT	void	hll_inthdr_entry_34(void);
IMPORT	void	hll_inthdr_entry_35(void);
IMPORT	void	hll_inthdr_entry_36(void);
IMPORT	void	hll_inthdr_entry_37(void);
IMPORT	void	hll_inthdr_entry_38(void);
IMPORT	void	hll_inthdr_entry_39(void);
IMPORT	void	hll_inthdr_entry_40(void);
IMPORT	void	hll_inthdr_entry_41(void);
IMPORT	void	hll_inthdr_entry_42(void);
IMPORT	void	hll_inthdr_entry_43(void);
IMPORT	void	hll_inthdr_entry_44(void);
IMPORT	void	hll_inthdr_entry_45(void);
IMPORT	void	hll_inthdr_entry_46(void);
IMPORT	void	hll_inthdr_entry_47(void);
IMPORT	void	hll_inthdr_entry_48(void);
IMPORT	void	hll_inthdr_entry_49(void);
IMPORT	void	hll_inthdr_entry_50(void);
IMPORT	void	hll_inthdr_entry_51(void);
IMPORT	void	hll_inthdr_entry_52(void);
IMPORT	void	hll_inthdr_entry_53(void);
IMPORT	void	hll_inthdr_entry_54(void);
IMPORT	void	hll_inthdr_entry_55(void);
IMPORT	void	hll_inthdr_entry_56(void);
IMPORT	void	hll_inthdr_entry_57(void);
IMPORT	void	hll_inthdr_entry_58(void);
IMPORT	void	hll_inthdr_entry_59(void);
IMPORT	void	hll_inthdr_entry_60(void);
IMPORT	void	hll_inthdr_entry_61(void);
IMPORT	void	hll_inthdr_entry_62(void);
IMPORT	void	hll_inthdr_entry_63(void);
IMPORT	void	hll_inthdr_entry_64(void);
IMPORT	void	hll_inthdr_entry_65(void);
IMPORT	void	hll_inthdr_entry_66(void);
IMPORT	void	hll_inthdr_entry_67(void);
IMPORT	void	hll_inthdr_entry_68(void);
IMPORT	void	hll_inthdr_entry_69(void);
IMPORT	void	hll_inthdr_entry_70(void);
IMPORT	void	hll_inthdr_entry_71(void);
IMPORT	void	hll_inthdr_entry_72(void);
IMPORT	void	hll_inthdr_entry_73(void);
IMPORT	void	hll_inthdr_entry_74(void);
IMPORT	void	hll_inthdr_entry_75(void);
IMPORT	void	hll_inthdr_entry_76(void);
IMPORT	void	hll_inthdr_entry_77(void);
IMPORT	void	hll_inthdr_entry_78(void);
IMPORT	void	hll_inthdr_entry_79(void);
IMPORT	void	hll_inthdr_entry_80(void);
IMPORT	void	hll_inthdr_entry_81(void);
IMPORT	void	hll_inthdr_entry_82(void);
IMPORT	void	hll_inthdr_entry_83(void);
IMPORT	void	hll_inthdr_entry_84(void);
IMPORT	void	hll_inthdr_entry_85(void);
IMPORT	void	hll_inthdr_entry_86(void);
IMPORT	void	hll_inthdr_entry_87(void);
IMPORT	void	hll_inthdr_entry_88(void);
IMPORT	void	hll_inthdr_entry_89(void);
IMPORT	void	hll_inthdr_entry_90(void);
IMPORT	void	hll_inthdr_entry_91(void);
IMPORT	void	hll_inthdr_entry_92(void);
IMPORT	void	hll_inthdr_entry_93(void);
IMPORT	void	hll_inthdr_entry_94(void);
IMPORT	void	hll_inthdr_entry_95(void);
IMPORT	void	hll_inthdr_entry_96(void);
IMPORT	void	hll_inthdr_entry_97(void);
IMPORT	void	hll_inthdr_entry_98(void);
IMPORT	void	hll_inthdr_entry_99(void);
IMPORT	void	hll_inthdr_entry_100(void);
IMPORT	void	hll_inthdr_entry_101(void);
IMPORT	void	hll_inthdr_entry_102(void);
IMPORT	void	hll_inthdr_entry_103(void);
IMPORT	void	hll_inthdr_entry_104(void);
IMPORT	void	hll_inthdr_entry_105(void);
IMPORT	void	hll_inthdr_entry_106(void);
IMPORT	void	hll_inthdr_entry_107(void);
IMPORT	void	hll_inthdr_entry_108(void);
IMPORT	void	hll_inthdr_entry_109(void);
IMPORT	void	hll_inthdr_entry_110(void);
IMPORT	void	hll_inthdr_entry_111(void);
IMPORT	void	hll_inthdr_entry_112(void);
IMPORT	void	hll_inthdr_entry_113(void);
IMPORT	void	hll_inthdr_entry_114(void);
IMPORT	void	hll_inthdr_entry_115(void);
IMPORT	void	hll_inthdr_entry_116(void);
IMPORT	void	hll_inthdr_entry_117(void);
IMPORT	void	hll_inthdr_entry_118(void);
IMPORT	void	hll_inthdr_entry_119(void);
IMPORT	void	hll_inthdr_entry_120(void);
IMPORT	void	hll_inthdr_entry_121(void);
IMPORT	void	hll_inthdr_entry_122(void);
IMPORT	void	hll_inthdr_entry_123(void);
IMPORT	void	hll_inthdr_entry_124(void);
IMPORT	void	hll_inthdr_entry_125(void);
IMPORT	void	hll_inthdr_entry_126(void);
IMPORT	void	hll_inthdr_entry_127(void);
IMPORT	void	hll_inthdr_entry_128(void);
IMPORT	void	hll_inthdr_entry_129(void);
IMPORT	void	hll_inthdr_entry_130(void);
IMPORT	void	hll_inthdr_entry_131(void);
IMPORT	void	hll_inthdr_entry_132(void);
IMPORT	void	hll_inthdr_entry_133(void);
IMPORT	void	hll_inthdr_entry_134(void);
IMPORT	void	hll_inthdr_entry_135(void);
IMPORT	void	hll_inthdr_entry_136(void);
IMPORT	void	hll_inthdr_entry_137(void);
IMPORT	void	hll_inthdr_entry_138(void);
IMPORT	void	hll_inthdr_entry_139(void);
IMPORT	void	hll_inthdr_entry_140(void);
IMPORT	void	hll_inthdr_entry_141(void);
IMPORT	void	hll_inthdr_entry_142(void);
IMPORT	void	hll_inthdr_entry_143(void);
IMPORT	void	hll_inthdr_entry_144(void);
IMPORT	void	hll_inthdr_entry_145(void);
IMPORT	void	hll_inthdr_entry_146(void);
IMPORT	void	hll_inthdr_entry_147(void);
IMPORT	void	hll_inthdr_entry_148(void);
IMPORT	void	hll_inthdr_entry_149(void);
IMPORT	void	hll_inthdr_entry_150(void);
IMPORT	void	hll_inthdr_entry_151(void);
IMPORT	void	hll_inthdr_entry_152(void);
IMPORT	void	hll_inthdr_entry_153(void);
IMPORT	void	hll_inthdr_entry_154(void);
IMPORT	void	hll_inthdr_entry_155(void);
IMPORT	void	hll_inthdr_entry_156(void);
IMPORT	void	hll_inthdr_entry_157(void);
IMPORT	void	hll_inthdr_entry_158(void);
IMPORT	void	hll_inthdr_entry_159(void);
IMPORT	void	hll_inthdr_entry_160(void);
IMPORT	void	hll_inthdr_entry_161(void);
IMPORT	void	hll_inthdr_entry_162(void);
IMPORT	void	hll_inthdr_entry_163(void);
IMPORT	void	hll_inthdr_entry_164(void);
IMPORT	void	hll_inthdr_entry_165(void);
IMPORT	void	hll_inthdr_entry_166(void);
IMPORT	void	hll_inthdr_entry_167(void);
IMPORT	void	hll_inthdr_entry_168(void);
IMPORT	void	hll_inthdr_entry_169(void);
IMPORT	void	hll_inthdr_entry_170(void);
IMPORT	void	hll_inthdr_entry_171(void);
IMPORT	void	hll_inthdr_entry_172(void);
IMPORT	void	hll_inthdr_entry_173(void);
IMPORT	void	hll_inthdr_entry_174(void);
IMPORT	void	hll_inthdr_entry_175(void);
IMPORT	void	hll_inthdr_entry_176(void);
IMPORT	void	hll_inthdr_entry_177(void);
IMPORT	void	hll_inthdr_entry_178(void);
IMPORT	void	hll_inthdr_entry_179(void);
IMPORT	void	hll_inthdr_entry_180(void);
IMPORT	void	hll_inthdr_entry_181(void);
IMPORT	void	hll_inthdr_entry_182(void);
IMPORT	void	hll_inthdr_entry_183(void);
IMPORT	void	hll_inthdr_entry_184(void);
IMPORT	void	hll_inthdr_entry_185(void);
IMPORT	void	hll_inthdr_entry_186(void);
IMPORT	void	hll_inthdr_entry_187(void);
IMPORT	void	hll_inthdr_entry_188(void);
IMPORT	void	hll_inthdr_entry_189(void);
IMPORT	void	hll_inthdr_entry_190(void);
IMPORT	void	hll_inthdr_entry_191(void);
IMPORT	void	hll_inthdr_entry_192(void);
IMPORT	void	hll_inthdr_entry_193(void);
IMPORT	void	hll_inthdr_entry_194(void);
IMPORT	void	hll_inthdr_entry_195(void);
IMPORT	void	hll_inthdr_entry_196(void);
IMPORT	void	hll_inthdr_entry_197(void);
IMPORT	void	hll_inthdr_entry_198(void);
IMPORT	void	hll_inthdr_entry_199(void);
IMPORT	void	hll_inthdr_entry_209(void);
IMPORT	void	hll_inthdr_entry_201(void);
IMPORT	void	hll_inthdr_entry_202(void);
IMPORT	void	hll_inthdr_entry_203(void);
IMPORT	void	hll_inthdr_entry_204(void);
IMPORT	void	hll_inthdr_entry_205(void);
IMPORT	void	hll_inthdr_entry_206(void);
IMPORT	void	hll_inthdr_entry_207(void);
IMPORT	void	hll_inthdr_entry_208(void);
IMPORT	void	hll_inthdr_entry_209(void);
IMPORT	void	hll_inthdr_entry_210(void);
IMPORT	void	hll_inthdr_entry_211(void);
IMPORT	void	hll_inthdr_entry_212(void);
IMPORT	void	hll_inthdr_entry_213(void);
IMPORT	void	hll_inthdr_entry_214(void);
IMPORT	void	hll_inthdr_entry_215(void);
IMPORT	void	hll_inthdr_entry_216(void);
IMPORT	void	hll_inthdr_entry_217(void);
IMPORT	void	hll_inthdr_entry_218(void);
IMPORT	void	hll_inthdr_entry_219(void);
IMPORT	void	hll_inthdr_entry_220(void);
IMPORT	void	hll_inthdr_entry_221(void);
IMPORT	void	hll_inthdr_entry_222(void);
IMPORT	void	hll_inthdr_entry_223(void);
IMPORT	void	hll_inthdr_entry_224(void);
IMPORT	void	hll_inthdr_entry_225(void);
IMPORT	void	hll_inthdr_entry_226(void);
IMPORT	void	hll_inthdr_entry_227(void);
IMPORT	void	hll_inthdr_entry_228(void);
IMPORT	void	hll_inthdr_entry_229(void);
IMPORT	void	hll_inthdr_entry_230(void);
IMPORT	void	hll_inthdr_entry_231(void);
IMPORT	void	hll_inthdr_entry_232(void);
IMPORT	void	hll_inthdr_entry_233(void);
IMPORT	void	hll_inthdr_entry_234(void);
IMPORT	void	hll_inthdr_entry_235(void);
IMPORT	void	hll_inthdr_entry_236(void);
IMPORT	void	hll_inthdr_entry_237(void);
IMPORT	void	hll_inthdr_entry_238(void);
IMPORT	void	hll_inthdr_entry_239(void);
IMPORT	void	hll_inthdr_entry_240(void);
IMPORT	void	hll_inthdr_entry_241(void);
IMPORT	void	hll_inthdr_entry_242(void);
IMPORT	void	hll_inthdr_entry_243(void);
IMPORT	void	hll_inthdr_entry_244(void);
IMPORT	void	hll_inthdr_entry_245(void);
IMPORT	void	hll_inthdr_entry_246(void);
IMPORT	void	hll_inthdr_entry_247(void);
IMPORT	void	hll_inthdr_entry_248(void);
IMPORT	void	hll_inthdr_entry_249(void);
IMPORT	void	hll_inthdr_entry_250(void);
IMPORT	void	hll_inthdr_entry_251(void);
IMPORT	void	hll_inthdr_entry_252(void);
IMPORT	void	hll_inthdr_entry_253(void);
IMPORT	void	hll_inthdr_entry_254(void);
IMPORT	void	hll_inthdr_entry_255(void);

#endif /* _SYSDEPEND_CPU_INTVECT_TBL_ */
