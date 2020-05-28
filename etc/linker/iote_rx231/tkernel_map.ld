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
 *      tkernel_map.ld
 *	micro T-Kernel linker script for RX231 IoT-Engine
 */

MEMORY
{
	RAM : ORIGIN = 0x0, LENGTH = 65536
	ROM : ORIGIN = 0xFFF80000, LENGTH = 524288
}
SECTIONS
{
	.exvectors 0xFFFFFF80: AT(0xFFFFFF80) {
		_exvectors_start = .;
		KEEP(*(.exvectors))
		_exvectors_end = .;
	} > ROM
	.fvectors 0xFFFFFFFC: AT(0xFFFFFFFC) {
		KEEP(*(.fvectors))
	} > ROM
	.text 0xFFF80000: AT(0xFFF80000) {
		*(.text)
		*(.text.*)
		*(P)
		etext = .;
	} > ROM
	.rvectors ALIGN(4): {
		_rvectors_start = .;
		KEEP(*(.rvectors))
		_rvectors_end = .;
		. = ALIGN(4);
		KEEP(*(.hvectors))
	} > ROM
	.init :
	{
		KEEP(*(.init))
		__preinit_array_start = .;
		KEEP(*(.preinit_array))
		__preinit_array_end = .;
		__init_array_start = (. + 3) & ~ 3;
		KEEP(*(.init_array))
		KEEP(*(SORT(.init_array.*)))
		__init_array_end = .;
		__fini_array_start = .;
		KEEP(*(.fini_array))
		KEEP(*(SORT(.fini_array.*)))
		__fini_array_end = .;
	} > ROM
	.fini :
	{
		KEEP(*(.fini))
	} > ROM
	.got :
	{
		*(.got)
		*(.got.plt)
	} > ROM
	.rodata : {
		*(.rodata)
		*(.rodata.*)
		*(C_1)
		*(C_2)
		*(C)
		_erodata = .;
	} > ROM
	.eh_frame_hdr :
	{
		*(.eh_frame_hdr)
	} > ROM
	.eh_frame :
	{
		*(.eh_frame)
	} > ROM
	.jcr :
	{
		*(.jcr)
	} > ROM
	.tors : {
		__CTOR_LIST__ = .;
		. = ALIGN(2);
		___ctors = .;
		*(.ctors)
		___ctors_end = .;
		__CTOR_END__ = .;
		__DTOR_LIST__ = .;
		___dtors = .;
		*(.dtors)
		___dtors_end = .;
		__DTOR_END__ = .;
		. = ALIGN(2);
		__data_org = .;
	} > ROM
	.vector 0x0 : {
		. = ALIGN(4);
		*(.data_ivector)
		. = ALIGN(4);
		*(.data_hvector)
	} > RAM
	.data : AT(__data_org) {
		. = ALIGN(4);
		__data_start = .;
		*(.data)
		*(.data.*)
		*(D)
		*(D_1)
		*(D_2)
		__data_end = .;
	} > RAM
	.gcc_exc : {
		*(.gcc_exc)
	} > RAM
	.bss ALIGN(4) (NOLOAD) : {
		__bss_start = .;
		PROVIDE (__noinit_start = .);
		*(.noinit)
		PROVIDE (__noinit_end = .);
		*(.bss)
		*(.bss.**)
		*(COMMON)
		*(B)
		*(B_1)
		*(B_2)
		__bss_end = .;
		_end = .;
	} > RAM
}
