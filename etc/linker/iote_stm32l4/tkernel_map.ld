/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.03
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */

/*
 *      tkernel_map.ld
 *	micro T-Kernel linker script for STM32L4 IoT-Engine
 */

ENTRY(Reset_Handler)

/* Memories definition */
MEMORY
{
  RAM (xrw)		: ORIGIN = 0x20000000, LENGTH = 96K
  ROM (rx)		: ORIGIN = 0x08000000, LENGTH = 1024K
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
	__data_org = .;
	vector 0x20000000 (NOLOAD) : {
		__vector_start = .;
		*(.data_vector)
		__vector_end = .;
	} >RAM
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

