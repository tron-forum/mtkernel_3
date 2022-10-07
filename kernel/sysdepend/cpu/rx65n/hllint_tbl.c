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
 *	hllint_tbl.c (RX65N)
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
	Default_Handler,			/* RAM_RAMERR */
	Default_Handler,			/* INT 19 */
	Default_Handler,			/* INT 20 */
	Default_Handler,			/* FCU_FIFERR */
	Default_Handler,			/* INT 22 */
	Default_Handler,			/* FCU_FRDYI */
	Default_Handler,			/* INT 24 */
	Default_Handler,			/* INT 25 */
	Default_Handler,			/* ICU_SWINT2 */
	Default_Handler,			/* ICU_SWINT */
	knl_timer_handler,			/* CMT0_CMI0 **** System timer *****/
	Default_Handler,			/* CMT0_CMI1 */
	Default_Handler,			/* CMTW0_CMWI0 */
	Default_Handler,			/* CMTW1_CMWI1 */
	Default_Handler,			/* INT 32 */
	Default_Handler,			/* INT 33 */
	Default_Handler,			/* USB0_D0FIFO0 */
	Default_Handler,			/* USB0_D1FIFO0 */
	Default_Handler,			/* INT 36 */
	Default_Handler,			/* INT 37 */
	Default_Handler,			/* RSPI0_SPRI0 */
	Default_Handler,			/* RSPI0_SPTI0 */
	Default_Handler,			/* RSPI1_SPRI1 */
	Default_Handler,			/* RSPI1_SPTI1 */
	Default_Handler,			/* QSPI_SPRI */
	Default_Handler,			/* QSPI_SPTI */
	Default_Handler,			/* SDHI_SBFAI */
	Default_Handler,			/* MMCIF_MBFAI */
	Default_Handler,			/* INT 46 */
	Default_Handler,			/* INT 47 */
	Default_Handler,			/* INT 48 */
	Default_Handler,			/* INT 49 */
	Default_Handler,			/* RIIC1_RXI1 */
	Default_Handler,			/* RIIC1_TXI1 */
	Default_Handler,			/* RIIC0_RXI0 */
	Default_Handler,			/* RIIC0_TXI0 */
	Default_Handler,			/* RIIC2_RXI2 */
	Default_Handler,			/* RIIC2_TXI2 */
	Default_Handler,			/* INT 56 */
	Default_Handler,			/* INT 57 */
	Default_Handler,			/* SCI0_RXI0 */
	Default_Handler,			/* SCI0_TXI0 */
	Default_Handler,			/* SCI1_RXI1 */
	Default_Handler,			/* SCI1_TXI1 */
	Default_Handler,			/* SCI2_RXI2 */
	Default_Handler,			/* SCI2_TXI2 */
	Default_Handler,			/* ICU_IRQ0 */
	Default_Handler,			/* ICU_IRQ1 */
	Default_Handler,			/* ICU_IRQ2 */
	Default_Handler,			/* ICU_IRQ3 */
	Default_Handler,			/* ICU_IRQ4 */
	Default_Handler,			/* ICU_IRQ5 */
	Default_Handler,			/* ICU_IRQ6 */
	Default_Handler,			/* ICU_IRQ7 */
	Default_Handler,			/* ICU_IRQ8 */
	Default_Handler,			/* ICU_IRQ9 */
	Default_Handler,			/* ICU_IRQ10 */
	Default_Handler,			/* ICU_IRQ11 */
	Default_Handler,			/* ICU_IRQ12 */
	Default_Handler,			/* ICU_IRQ13 */
	Default_Handler,			/* ICU_IRQ14 */
	Default_Handler,			/* ICU_IRQ15 */
	Default_Handler,			/* SCI3_RXI3 */
	Default_Handler,			/* SCI3_TXI3 */
	Default_Handler,			/* SCI4_RXI4 */
	Default_Handler,			/* SCI4_TXI4 */
	Default_Handler,			/* SCI5_RXI5 */
	Default_Handler,			/* SCI5_TXI5 */
	Default_Handler,			/* SCI6_RXI6 */
	Default_Handler,			/* SCI6_TXI6 */
	Default_Handler,			/* LVD_LVD1 */
	Default_Handler,			/* LVD_LVD2 */
	Default_Handler,			/* USB0_USBR0 */
	Default_Handler,			/* SCI6_RXI6 */
	Default_Handler,			/* RTC_ALM */
	Default_Handler,			/* RTC_PRD */
	Default_Handler,			/* INT 94 */
	Default_Handler,			/* IWDT_IWUNI */
	Default_Handler,			/* WDT_WUNI */
	Default_Handler,			/* RDC_PCDFI */
	Default_Handler,			/* SCI7_RXI7 */
	Default_Handler,			/* SCI7_TXI7 */
	Default_Handler,			/* SCI8_RXI8 */
	Default_Handler,			/* SCI8_TXI8 */
	Default_Handler,			/* SCI9_RXI9 */
	Default_Handler,			/* SCI9_TXI9 */
	Default_Handler,			/* SCI10_RXI10 */
	Default_Handler,			/* SCI10_TXI10 */
	knl_group_inthdr,			/* ICU_GROUPBE0 *** group interrupt  **/
	knl_group_inthdr,			/* ICU_GROUPBL2 *** group interrupt  **/
	Default_Handler,			/* RSPI2_SPRI2 */
	Default_Handler,			/* RSPI2_SPTI2 */
	knl_group_inthdr,			/* ICU_GROUPBL0 *** group interrupt  **/
	knl_group_inthdr,			/* ICU_GROUPBL1 *** group interrupt  **/
	knl_group_inthdr,			/* ICU_GROUPAL0 *** group interrupt  **/
	knl_group_inthdr,			/* ICU_GROUPAL1 *** group interrupt  **/
	Default_Handler,			/* SCI11_RXI11 */
	Default_Handler,			/* SCI11_TXI11 */
	Default_Handler,			/* SCI12_RXI12 */
	Default_Handler,			/* SCI12_TXI12 */
	Default_Handler,			/* INT 118 */
	Default_Handler,			/* INT 119 */
	Default_Handler,			/* DMAC_DMAC0I */
	Default_Handler,			/* DMAC_DMAC1I */
	Default_Handler,			/* DMAC_DMAC2I */
	Default_Handler,			/* DMAC_DMAC3I */
	Default_Handler,			/* DMAC_DMAC74I */
	Default_Handler,			/* OST_OSTDI */
	Default_Handler,			/* EXDMAC_EXDMAC0I */
	Default_Handler,			/* EXDMAC_EXDMAC1I */
	Default_Handler,			/* INTB 128 */
	Default_Handler,			/* INTB 129 */
	Default_Handler,			/* INTB 130 */
	Default_Handler,			/* INTB 131 */
	Default_Handler,			/* INTB 132 */
	Default_Handler,			/* INTB 133 */
	Default_Handler,			/* INTB 134 */
	Default_Handler,			/* INTB 135 */
	Default_Handler,			/* INTB 136 */
	Default_Handler,			/* INTB 137 */
	Default_Handler,			/* INTB 138 */
	Default_Handler,			/* INTB 139 */
	Default_Handler,			/* INTB 140 */
	Default_Handler,			/* INTB 141 */
	Default_Handler,			/* INTB 142 */
	Default_Handler,			/* INTB 143 */
	Default_Handler,			/* INTB 144 */
	Default_Handler,			/* INTB 145 */
	Default_Handler,			/* INTB 146 */
	Default_Handler,			/* INTB 147 */
	Default_Handler,			/* INTB 148 */
	Default_Handler,			/* INTB 149 */
	Default_Handler,			/* INTB 150 */
	Default_Handler,			/* INTB 151 */
	Default_Handler,			/* INTB 152 */
	Default_Handler,			/* INTB 153 */
	Default_Handler,			/* INTB 154 */
	Default_Handler,			/* INTB 155 */
	Default_Handler,			/* INTB 156 */
	Default_Handler,			/* INTB 157 */
	Default_Handler,			/* INTB 158 */
	Default_Handler,			/* INTB 159 */
	Default_Handler,			/* INTB 160 */
	Default_Handler,			/* INTB 161 */
	Default_Handler,			/* INTB 162 */
	Default_Handler,			/* INTB 163 */
	Default_Handler,			/* INTB 164 */
	Default_Handler,			/* INTB 165 */
	Default_Handler,			/* INTB 166 */
	Default_Handler,			/* INTB 167 */
	Default_Handler,			/* INTB 168 */
	Default_Handler,			/* INTB 169 */
	Default_Handler,			/* INTB 170 */
	Default_Handler,			/* INTB 171 */
	Default_Handler,			/* INTB 172 */
	Default_Handler,			/* INTB 173 */
	Default_Handler,			/* INTB 174 */
	Default_Handler,			/* INTB 175 */
	Default_Handler,			/* INTB 176 */
	Default_Handler,			/* INTB 177 */
	Default_Handler,			/* INTB 178 */
	Default_Handler,			/* INTB 179 */
	Default_Handler,			/* INTB 180 */
	Default_Handler,			/* INTB 181 */
	Default_Handler,			/* INTB 182 */
	Default_Handler,			/* INTB 183 */
	Default_Handler,			/* INTB 184 */
	Default_Handler,			/* INTB 185 */
	Default_Handler,			/* INTB 186 */
	Default_Handler,			/* INTB 187 */
	Default_Handler,			/* INTB 188 */
	Default_Handler,			/* INTB 189 */
	Default_Handler,			/* INTB 190 */
	Default_Handler,			/* INTB 191 */
	Default_Handler,			/* INTB 192 */
	Default_Handler,			/* INTB 193 */
	Default_Handler,			/* INTB 194 */
	Default_Handler,			/* INTB 195 */
	Default_Handler,			/* INTB 196 */
	Default_Handler,			/* INTB 197 */
	Default_Handler,			/* INTB 198 */
	Default_Handler,			/* INTB 199 */
	Default_Handler,			/* INTB 209 */
	Default_Handler,			/* INTB 201 */
	Default_Handler,			/* INTB 202 */
	Default_Handler,			/* INTB 203 */
	Default_Handler,			/* INTB 204 */
	Default_Handler,			/* INTB 205 */
	Default_Handler,			/* INTB 206 */
	Default_Handler,			/* INTB 207 */
	Default_Handler,			/* INTA 208 */
	Default_Handler,			/* INTA 209 */
	Default_Handler,			/* INTA 210 */
	Default_Handler,			/* INTA 211 */
	Default_Handler,			/* INTA 212 */
	Default_Handler,			/* INTA 213 */
	Default_Handler,			/* INTA 214 */
	Default_Handler,			/* INTA 215 */
	Default_Handler,			/* INTA 216 */
	Default_Handler,			/* INTA 217 */
	Default_Handler,			/* INTA 218 */
	Default_Handler,			/* INTA 219 */
	Default_Handler,			/* INTA 220 */
	Default_Handler,			/* INTA 221 */
	Default_Handler,			/* INTA 222 */
	Default_Handler,			/* INTA 223 */
	Default_Handler,			/* INTA 224 */
	Default_Handler,			/* INTA 225 */
	Default_Handler,			/* INTA 226 */
	Default_Handler,			/* INTA 227 */
	Default_Handler,			/* INTA 228 */
	Default_Handler,			/* INTA 229 */
	Default_Handler,			/* INTA 230 */
	Default_Handler,			/* INTA 231 */
	Default_Handler,			/* INTA 232 */
	Default_Handler,			/* INTA 233 */
	Default_Handler,			/* INTA 234 */
	Default_Handler,			/* INTA 235 */
	Default_Handler,			/* INTA 236 */
	Default_Handler,			/* INTA 237 */
	Default_Handler,			/* INTA 238 */
	Default_Handler,			/* INTA 239 */
	Default_Handler,			/* INTA 240 */
	Default_Handler,			/* INTA 241 */
	Default_Handler,			/* INTA 242 */
	Default_Handler,			/* INTA 243 */
	Default_Handler,			/* INTA 244 */
	Default_Handler,			/* INTA 245 */
	Default_Handler,			/* INTA 246 */
	Default_Handler,			/* INTA 247 */
	Default_Handler,			/* INTA 248 */
	Default_Handler,			/* INTA 249 */
	Default_Handler,			/* INTA 250 */
	Default_Handler,			/* INTA 251 */
	Default_Handler,			/* INTA 252 */
	Default_Handler,			/* INTA 253 */
	Default_Handler,			/* INTA 254 */
	Default_Handler,			/* INTA 255 */
};

#endif /* CPU_RX65N */
