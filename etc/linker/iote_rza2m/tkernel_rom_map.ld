/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *      tkernel_map.ld
 *	micro T-Kernel linker script for RZ/A2M IoT-Engine
 */

MEMORY {
	SERIAL_FLASH (r)   : ORIGIN = 0x20000000, LENGTH = 0x02000000
	BACKUP_RAM   (rw)  : ORIGIN = 0x80000000, LENGTH = 0x00020000
	CACHED_RAM   (rw)  : ORIGIN = 0x80020000, LENGTH = 0x002e0000
	UNCACHED_RAM (rw)  : ORIGIN = 0x80300000, LENGTH = 0x000fc000
	TTB_RAM      (rw)  : ORIGIN = 0x803fc000, LENGTH = 0x00004000
}

SECTIONS
{
	.sf_text : ALIGN(0x20) {
		__sf_vector_start = .;
		*(SF_VECTOR_TABLE)
		__sf_vector_end = .;
		__sf_start = .;
		* (SF_RESET_HANDLER)
	} > SERIAL_FLASH

	.text : ALIGN(0x20) {
		__vector_start = .;
		*(VECTOR_TABLE)
		__vector_end = .;
		__start = .;
		* (RESET_HANDLER)
		* (EXC_ENTRY)
		* (INIT_TTB)
		* (IRQ_FIQ_HANDLER)
		* (OTHER_HANDLER)
		* (.text .text.*)
		* (.rodata .rodata.*)
	} > CACHED_RAM AT > SERIAL_FLASH

	.data : ALIGN(0x20) {
		* (.data .data.*)
		. = ALIGN(0x10);
	} > CACHED_RAM AT > SERIAL_FLASH

	.noload (NOLOAD) : ALIGN(0x20) {
		*(.noinit)
	} > CACHED_RAM

	.bss (NOLOAD) : ALIGN(0x20) {
		* (.EIT_VecTable)
		. = ALIGN(0x10);
		* (.bss .bss.*)
		* (COMMON)
		. = ALIGN( 0x20 );
	} > CACHED_RAM

	.stack (NOLOAD) : ALIGN(0x20) {
		__tmp_stack_end = .;
		* (.tmp_stack_section)
		__tmp_stack_start = .;

		__fiq_stack_end = .;
		* (.fiq_stack_section)
		__fiq_stack_start = .;

		__irq_stack_end = .;
		* (.irq_stack_section)
		__irq_stack_start = .;

		__und_stack_end = .;
		* (.und_stack_section)
		__und_stack_start = .;

		__abt_stack_end = .;
		* (.abt_stack_section)
		__abt_stack_start = .;

		_HeapStart = .;					/* start of heap area */
	} > CACHED_RAM
	_HeapEnd = ORIGIN(CACHED_RAM) + LENGTH(CACHED_RAM);	/* end of heap area */

	.uncached_RAM (NOLOAD) : {
	} > UNCACHED_RAM

	.ttb (NOLOAD) : {
		__ttb_area_top = .;
		. += 0x00004000;
		__ttb_area_bottom = .;
	} > TTB_RAM

	_text_load	= ABSOLUTE( LOADADDR(.text) );	/* load base address of .text section    */
	_text_start	= ABSOLUTE( ADDR(.text) );	/* execute base address of .text section */
	_text_size	= SIZEOF(.text);
	_text_end	= _text_start + _text_size;

	_data_org	= ABSOLUTE( LOADADDR(.data) );	/* load base address of .data section    */
	_data_start	= ABSOLUTE( ADDR(.data) );	/* execute base address of .data section */
	_data_size	= SIZEOF(.data);
	_data_end	= _data_start + _data_size;

	_bss_start	= ABSOLUTE( ADDR(.bss) );		/* execute base address of .bss section  */
	_bss_size	= SIZEOF(.bss);
	_bss_end	= _bss_start + _bss_size;
}
