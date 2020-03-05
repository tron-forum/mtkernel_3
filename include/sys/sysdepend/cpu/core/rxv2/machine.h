/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.00
 *
 *    Copyright (C) 2006-2019 by Ken Sakamura.
 *    This software is distributed under the T-License 2.1.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2019/12/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	machine.h
 *
 *	Machine type definition (RXv2 core depended)
 */

#ifndef __SYS_MACHINE_CORE_H__
#define __SYS_MACHINE_CORE_H__

/*
 * CPU_xxxx		CPU type
 * ALLOW_MISALIGN	1 if access to misalignment data is allowed 
 * BIGENDIAN		1 if big endian 
 * VIRTUAL_ADDRESS	1 if virtual memory 
 * ALLOCA_NOSPT		1 if alloca() is not supported 
 */

/* ----- T-Engine Appliance RXv2 definition ----- */
#define CPU_RX_V2		1
#define ALLOW_MISALIGN		0
#define VIRTUAL_ADDRESS		0
#define ALLOCA_NOSPT		0
#define INT_BITWIDTH		32

/*
 * Endianness
 */
#define BIGENDIAN		0	/* Default (Little Endian) */

#endif /* __SYS_MACHINE_CORE_H__ */
