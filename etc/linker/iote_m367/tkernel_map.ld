/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *      tkernel_map.ld
 *	micro T-Kernel linker script
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)

ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
  ROM (rx)		: ORIGIN = 0x00000000, LENGTH = 512K
  RAM (xrw)		: ORIGIN = 0x20000800, LENGTH = 126K
}

SECTIONS {
	.text : {
		. = ALIGN(8);
		__vector_org = .;
		*(.vector)
		__start = .;
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
	} >ROM
	. = ALIGN(4);

	vector 0x20000800 (NOLOAD) : {
		__vector_start = .;
		*(.data_vector)
		__vector_end = .;
	} >RAM

	__data_org = .;
	.data : AT(__data_org) {
		__data_start = .;
		*(.data)
		*(.data.*)
		. = ALIGN(4);
		__data_end = .;
	} >RAM
	
	.bss ALIGN(4) (NOLOAD) : {
		__bss_start = .;
		PROVIDE (__noinit_start = .);
		*(.noinit)
		PROVIDE (__noinit_end = .);
		*(.bss)
		*(.bss.*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
		__end = .;
	} >RAM
}

