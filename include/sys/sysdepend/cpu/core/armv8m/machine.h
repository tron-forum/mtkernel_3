/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2006-2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2027/07.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (ARMv8-M)
 */

#ifndef _SYS_MACHINE_CORE_H_
#define _SYS_MACHINE_CORE_H_

/*
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
 * INT_BITWIDTH		Integer size (bit)
 * BIGENDIAN		1 if big endian 
 */

/* ----- ARMv8-M definition ----- */

#define ALLOW_MISALIGN		0
#define INT_BITWIDTH		32
#define BIGENDIAN		0	/* Default (Little Endian) */

#endif /* _SYS_MACHINE_CORE_H_ */
