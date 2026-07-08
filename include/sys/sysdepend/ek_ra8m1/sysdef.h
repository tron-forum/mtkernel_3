/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.08
 *
 *    Copyright (C) 2026 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2026/07.
 *
 *----------------------------------------------------------------------
 */

/*
 *	sysdef.h
 *
 *	System dependencies definition (EK-RA8M1 depended)
 *	Included also from assembler program.
 */

#ifndef __SYS_SYSDEF_DEPEND_H__
#define __SYS_SYSDEF_DEPEND_H__

/* ------------------------------------------------------------------------ */
/* TrustZone settings
 * (This setting reflects the configuration setting.)
 */
/* TrustZone defined */
#define TRUSTZONE_ENABLE	(CNF_TZ_ENABLE)				// TrustZone Enabled
#define TRUSTZONE_SECURE	(CNF_TZ_ENABLE && CNF_TZ_STATE)		// OS in Secure status
#define TRUSTZONE_NONSECURE	(CNF_TZ_ENABLE && !CNF_TZ_STATE)	// OS in Non-Secure status

// Enable Secure Calls from Tasks
#define TRUSTZONE_SCALL	(TRUSTZONE_NONSECURE && CNF_TZ_SCALL)

#ifndef _in_asm_source_
/* ------------------------------------------------------------------------ */
/* System clock settings
 */
IMPORT UW knl_sysclk;		// System clock

#define	SYSCLK			knl_sysclk	// System clock

#define SYST_CLK_SRC		0x00000000	// SysTick CLKSOURCE bit

#define TMCLK			(1)		/* System timer clock input (MHz) */
#define TMCLK_KHz		(TMCLK * 1000)	/* System timer clock input (kHz) */

/* ------------------------------------------------------------------------ */
/* System Memory Area information
 *     UNUSED_RAM_TOP: Start address of unused area in RAM
 *     (This information is obtained from the linker information.)
 */
IMPORT const void		*__mtk3_SYSMEM_START;
#define UNUSED_RAM_TOP		((UW)&__mtk3_SYSMEM_START)

#endif	/* _in_asm_source_ */

/* CPU-dependent definition */
#include "../cpu/ra8m1/sysdef.h"

#endif /* __SYS_SYSDEF_DEPEND_H__ */
