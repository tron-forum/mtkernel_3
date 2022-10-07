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
#ifdef CPU_RX65N

/*
 *	groupint_tbl.c (RX65N)
 *	Group Interrupt Handler Table
 */

#include "kernel.h"
#include "../../sysdepend.h"

#if USE_GROUP_INT
/*
 * Group Interrupt Handler Table
 */
LOCAL FP gint_tbl[N_GROUP_INT];

/*
 * Set Group int. handler table (Used knl_define_inthdr())
 */
EXPORT ER knl_define_gint( INT intno, FP inthdr)
{
	if(intno < INTNO_GROUP_TOP + N_GROUP_INT) {
		gint_tbl[intno - INTNO_GROUP_TOP] = inthdr;
	} else {
		return E_PAR;
	}
	return E_OK;
}

/*
 * Common Group interrupt handler
 */
EXPORT void knl_group_inthdr(UW intno)
{
	FP	gint_hdr;
	UW	grp;
	UW	hdrno, i;

	ClearInt(intno);

	switch(intno) {
	case INTNO_GROUPBE0:
		grp = in_w(ICU_GRPBE0);
		hdrno = 0;
		break;
	case INTNO_GROUPBL0:
		grp = in_w(ICU_GRPBL0);
		hdrno = 32;
		break;
	case INTNO_GROUPBL1:
		grp = in_w(ICU_GRPBL1);
		hdrno = 64;
		break;
	case INTNO_GROUPBL2:
		grp = in_w(ICU_GRPBL2);
		hdrno = 96;
		break;
	case INTNO_GROUPAL0:
		grp = in_w(ICU_GRPAL0);
		hdrno = 128;
		break;
	case INTNO_GROUPAL1:
		grp = in_w(ICU_GRPAL1);
		hdrno = 160;
		break;
	}

	for(i = 0; i < 32; i++) {
		if(grp & (1<<i)) {
			gint_hdr = gint_tbl[hdrno + i];
			if(gint_hdr != NULL) {
				(*gint_hdr)(INTNO_GROUP_TOP + hdrno + i);
			} else {
				Default_Handler(intno);
			}
		}
	}
}

#endif /* USE_GROUP_INT */
#endif /* CPU_RX65N */