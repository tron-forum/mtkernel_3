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

#include <sys/machine.h>
#ifdef CPU_RZA2M

/*
 *	ttb_ini.c (RZ/A2M)
 *	TTB(translation table base) initial value
 */
#include <tk/tkernel.h>

typedef struct {
	UW	addr;		/* Address */
	UW	size;		/* Area size */
	UW	attr;		/* Attribute*/
	UW	dmy;		/* Padding */
} T_TTB_VAL;

EXPORT const T_TTB_VAL	knl_ttb_ini[] = {
	{0x1f000000,               0x00100000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0x1f000000 - 0x1f0fffff
	{0x1f400000,               0x00100000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0x1f400000 - 0x1f4fffff
	{0x1f800000,               0x00100000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0x1f800000 - 0x1f8ffff
	{0x20000000,               0x02000000,         TTB_ATR_NORMAL_CACHE,     0x00000000},	// SPI multi I/O bus area (Serial flash) 0x20000000 - 0x2fffffff  32MB on board
	{0x30000000,               0x00800000,         TTB_ATR_NORMAL_CACHE,     0x00000000},	// Hyper Flash area  0x30000000 - 0x3fffffff  8MB on board
	{0x40000000,               0x00800000,         TTB_ATR_NORMAL_CACHE,     0x00000000},	// Hyper RAM area    0x40000000 - 0x4fffffff  8MB on board
	{IRAM_CACHED_START_ADDR,   IRAM_CACHED_SIZE,   TTB_ATR_NORMAL_CACHE,     0x00000000},	// Internal RAM (CACHED) area
	{IRAM_UNCACHED_START_ADDR, IRAM_UNCACHED_SIZE, TTB_ATR_NORMAL_NOT_CACHE, 0x00000000},	// Internal RAM (UBCACHED) area
	{0xe8000000,               0x00300000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0xe8000000 - 0xe82fffff
	{0xea000000,               0x01000000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0xea000000 - 0xea0fffff
	{0xf0000000,               0x00100000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0xf0000000 - 0xf00fffff
	{0xfc000000,               0x00100000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0xfc000000 - 0xfc0fffff
	{0xfcf00000,               0x00100000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0xfcf00000 - 0xfcffffff
	{0xfff00000,               0x00100000,         TTB_ATR_STRGLY,           0x00000000},	// Internal I/O area 0xfff00000 - 0xffffffff
	{0x00000000,               0x00000000,         0x00000000,               0x00000000},	// terminate

};

#endif /* CPU_RZA2M */