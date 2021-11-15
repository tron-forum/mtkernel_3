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
 *	machine.h
 *
 *	Machine type definition (ARMv7-M core depended)
 */

#ifndef __SYS_MACHINE_CORE_H__
#define __SYS_MACHINE_CORE_H__

/*
 * CPU_xxxx		CPU type
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
 * BIGENDIAN		1 if big endian 
 */

/* ----- ARMv7-M definition ----- */

#define ALLOW_MISALIGN		0
#define INT_BITWIDTH		32

/*
 * Endianness
 */
#define BIGENDIAN		0	/* Default (Little Endian) */

#endif /* __SYS_MACHINE_CORE_H__ */
