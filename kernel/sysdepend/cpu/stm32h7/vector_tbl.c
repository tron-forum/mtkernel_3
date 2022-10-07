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
#ifdef CPU_STM32H7

/*
 *	vector_tbl.h (STM32H7)
 *	Exception/Interrupt Vector Table
 */

#include "kernel.h"
#include "../../sysdepend.h"

/* ------------------------------------------------------------------------ */
/*
 * Exception/Interrupt Vector Table
 * 
 */

void (* const vector_tbl[])()  __attribute__((section(".vector"))) = {
	(void(*)()) (INITIAL_SP),		/* 0: Top of Stack */
	Reset_Handler,				/* 1: Reset Handler */
	NMI_Handler,				/* 2: NMI Handler */
	HardFault_Handler,			/* 3: Hard Fault Handler */
	MemManage_Handler,			/* 4: MPU Fault Handler */
	BusFault_Handler,			/* 5: Bus Fault Handler */
	UsageFault_Handler,			/* 6: Usage Fault Handler */
	0,					/* 7: Reserved */
	0,					/* 8: Reserved */
	0,					/* 9: Reserved */
	0,					/* 10: Reserved */
	Svcall_Handler,				/* 11: Svcall */
	DebugMon_Handler,			/* 12: Debug Monitor Handler */
	0,					/* 13: Reserved */
	knl_dispatch_entry,			/* 14: Pend SV */
	knl_systim_inthdr,			/* 15: Systick */

	/* External Interrupts */
	Default_Handler,			/* IRQ 0 */
	Default_Handler,			/* IRQ 1 */
	Default_Handler,			/* IRQ 2 */
	Default_Handler,			/* IRQ 3 */
	Default_Handler,			/* IRQ 4 */
	Default_Handler,			/* IRQ 5 */
	Default_Handler,			/* IRQ 6 */
	Default_Handler,			/* IRQ 7 */
	Default_Handler,			/* IRQ 8 */
	Default_Handler,			/* IRQ 9 */
	Default_Handler,			/* IRQ 10 */
	Default_Handler,			/* IRQ 11 */
	Default_Handler,			/* IRQ 12 */
	Default_Handler,			/* IRQ 13 */
	Default_Handler,			/* IRQ 14 */
	Default_Handler,			/* IRQ 15 */
	Default_Handler,			/* IRQ 16 */
	Default_Handler,			/* IRQ 17 */
	Default_Handler,			/* IRQ 18 */
	Default_Handler,			/* IRQ 19 */
	Default_Handler,			/* IRQ 20 */
	Default_Handler,			/* IRQ 21 */
	Default_Handler,			/* IRQ 22 */
	Default_Handler,			/* IRQ 23 */
	Default_Handler,			/* IRQ 24 */
	Default_Handler,			/* IRQ 25 */
	Default_Handler,			/* IRQ 26 */
	Default_Handler,			/* IRQ 27 */
	Default_Handler,			/* IRQ 28 */
	Default_Handler,			/* IRQ 29 */
	Default_Handler,			/* IRQ 30 */
	Default_Handler,			/* IRQ 31 */
	Default_Handler,			/* IRQ 32 */
	Default_Handler,			/* IRQ 33 */
	Default_Handler,			/* IRQ 34 */
	Default_Handler,			/* IRQ 35 */
	Default_Handler,			/* IRQ 36 */
	Default_Handler,			/* IRQ 37 */
	Default_Handler,			/* IRQ 38 */
	Default_Handler,			/* IRQ 39 */
	Default_Handler,			/* IRQ 40 */
	Default_Handler,			/* IRQ 41 */
	Default_Handler,			/* IRQ 42 */
	Default_Handler,			/* IRQ 43 */
	Default_Handler,			/* IRQ 44 */
	Default_Handler,			/* IRQ 45 */
	Default_Handler,			/* IRQ 46 */
	Default_Handler,			/* IRQ 47 */
	Default_Handler,			/* IRQ 48 */
	Default_Handler,			/* IRQ 49 */
	Default_Handler,			/* IRQ 50 */
	Default_Handler,			/* IRQ 51 */
	Default_Handler,			/* IRQ 52 */
	Default_Handler,			/* IRQ 53 */
	Default_Handler,			/* IRQ 54 */
	Default_Handler,			/* IRQ 55 */
	Default_Handler,			/* IRQ 56 */
	Default_Handler,			/* IRQ 57 */
	Default_Handler,			/* IRQ 58 */
	Default_Handler,			/* IRQ 59 */
	Default_Handler,			/* IRQ 60 */
	Default_Handler,			/* IRQ 61 */
	Default_Handler,			/* IRQ 62 */
	Default_Handler,			/* IRQ 63 */
	Default_Handler,			/* IRQ 64 */
	Default_Handler,			/* IRQ 65 */
	Default_Handler,			/* IRQ 66 */
	Default_Handler,			/* IRQ 67 */
	Default_Handler,			/* IRQ 68 */
	Default_Handler,			/* IRQ 69 */
	Default_Handler,			/* IRQ 70 */
	Default_Handler,			/* IRQ 71 */
	Default_Handler,			/* IRQ 72 */
	Default_Handler,			/* IRQ 73 */
	Default_Handler,			/* IRQ 74 */
	Default_Handler,			/* IRQ 75 */
	Default_Handler,			/* IRQ 76 */
	Default_Handler,			/* IRQ 77 */
	Default_Handler,			/* IRQ 78 */
	Default_Handler,			/* IRQ 79 */
	Default_Handler,			/* IRQ 80 */
	Default_Handler,			/* IRQ 81 */
	Default_Handler,			/* IRQ 82 */
	Default_Handler,			/* IRQ 83 */
	Default_Handler,			/* IRQ 84 */
	Default_Handler,			/* IRQ 85 */
	Default_Handler,			/* IRQ 86 */
	Default_Handler,			/* IRQ 87 */
	Default_Handler,			/* IRQ 88 */
	Default_Handler,			/* IRQ 89 */
	Default_Handler,			/* IRQ 90 */
	Default_Handler,			/* IRQ 91 */
	Default_Handler,			/* IRQ 92 */
	Default_Handler,			/* IRQ 93 */
	Default_Handler,			/* IRQ 94 */
	Default_Handler,			/* IRQ 95 */
	Default_Handler,			/* IRQ 96 */
	Default_Handler,			/* IRQ 97 */
	Default_Handler,			/* IRQ 98 */
	Default_Handler,			/* IRQ 99 */
	Default_Handler,			/* IRQ 100 */
	Default_Handler,			/* IRQ 101 */
	Default_Handler,			/* IRQ 102 */
	Default_Handler,			/* IRQ 103 */
	Default_Handler,			/* IRQ 104 */
	Default_Handler,			/* IRQ 105 */
	Default_Handler,			/* IRQ 106 */
	Default_Handler,			/* IRQ 107 */
	Default_Handler,			/* IRQ 108 */
	Default_Handler,			/* IRQ 109 */
	Default_Handler,			/* IRQ 110 */
	Default_Handler,			/* IRQ 111 */
	Default_Handler,			/* IRQ 112 */
	Default_Handler,			/* IRQ 113 */
	Default_Handler,			/* IRQ 114 */
	Default_Handler,			/* IRQ 115 */
	Default_Handler,			/* IRQ 116 */
	Default_Handler,			/* IRQ 117 */
	Default_Handler,			/* IRQ 118 */
	Default_Handler,			/* IRQ 119 */
	Default_Handler,			/* IRQ 120 */
	Default_Handler,			/* IRQ 121 */
	Default_Handler,			/* IRQ 122 */
	Default_Handler,			/* IRQ 123 */
	Default_Handler,			/* IRQ 124 */
	Default_Handler,			/* IRQ 125 */
	Default_Handler,			/* IRQ 126 */
	Default_Handler,			/* IRQ 127 */
	Default_Handler,			/* IRQ 128 */
	Default_Handler,			/* IRQ 129 */
	Default_Handler,			/* IRQ 130 */
	Default_Handler,			/* IRQ 131 */
	Default_Handler,			/* IRQ 132 */
	Default_Handler,			/* IRQ 133 */
	Default_Handler,			/* IRQ 134 */
	Default_Handler,			/* IRQ 135 */
	Default_Handler,			/* IRQ 136 */
	Default_Handler,			/* IRQ 137 */
	Default_Handler,			/* IRQ 138 */
	Default_Handler,			/* IRQ 139 */
	Default_Handler,			/* IRQ 140 */
	Default_Handler,			/* IRQ 141 */
	Default_Handler,			/* IRQ 142 */
	Default_Handler,			/* IRQ 143 */
	Default_Handler,			/* IRQ 144 */
	Default_Handler,			/* IRQ 145 */
	Default_Handler,			/* IRQ 146 */
	Default_Handler,			/* IRQ 147 */
	Default_Handler,			/* IRQ 148 */
	Default_Handler,			/* IRQ 149 */
	Default_Handler,			/* IRQ 150 */
	Default_Handler,			/* IRQ 151 */
	Default_Handler,			/* IRQ 152 */
	Default_Handler,			/* IRQ 153 */
	Default_Handler,			/* IRQ 154 */
	Default_Handler,			/* IRQ 155 */
	Default_Handler,			/* IRQ 156 */
	Default_Handler,			/* IRQ 157 */
	Default_Handler,			/* IRQ 158 */
	Default_Handler,			/* IRQ 159 */
	Default_Handler,			/* IRQ 160 */
	Default_Handler,			/* IRQ 161 */
	Default_Handler,			/* IRQ 162 */
	Default_Handler,			/* IRQ 163 */
	Default_Handler,			/* IRQ 164 */
	Default_Handler,			/* IRQ 165 */
	Default_Handler,			/* IRQ 166 */
	Default_Handler,			/* IRQ 167 */
	Default_Handler,			/* IRQ 168 */
	Default_Handler,			/* IRQ 169 */
	Default_Handler,			/* IRQ 170 */
	Default_Handler,			/* IRQ 171 */
	Default_Handler,			/* IRQ 172 */
	Default_Handler,			/* IRQ 173 */
	Default_Handler,			/* IRQ 174 */
	Default_Handler,			/* IRQ 175 */
	Default_Handler,			/* IRQ 176 */
	Default_Handler,			/* IRQ 177 */
	Default_Handler,			/* IRQ 178 */
	Default_Handler,			/* IRQ 179 */
	Default_Handler,			/* IRQ 180 */
	Default_Handler,			/* IRQ 181 */
	Default_Handler,			/* IRQ 182 */
	Default_Handler,			/* IRQ 183 */
	Default_Handler,			/* IRQ 184 */
	Default_Handler,			/* IRQ 185 */
	Default_Handler,			/* IRQ 186 */
	Default_Handler,			/* IRQ 187 */
	Default_Handler,			/* IRQ 188 */
	Default_Handler,			/* IRQ 189 */

};

#endif /* CPU_STM32H7 */
