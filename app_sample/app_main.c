/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.05.B0
 *
 *    Copyright (C) 2006-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/.
 *
 *----------------------------------------------------------------------
 */

#include <tk/tkernel.h>
#include <tk/dbgspt.h>
#include <tm/tmonitor.h>

/* ---------------------------------------------------------
 * Sample User Program
 * ---------------------------------------------------------
 * 
 * Entry routine for the user application.
 * At this point, Initialize and start the user application.
 *
 * Entry routine is called from the initial task for Kernel,
 * so system call for stopping the task should not be issued 
 * from the contexts of entry routine.
 * We recommend that:
 * (1)'usermain()' only generates the user initial task.
 * (2)initialize and start the user application by the user
 * initial task.
 */

#if 1
void raiseint1( void )
{
	UW	dummy;
	out_w(GICD_SGIR, 0x02000001);
	dummy = in_w(GICD_SGIR);
	dummy;
}

LOCAL volatile int n = 0;

void inthdr1(INT intno)
{
	TD_ITSK	itsk;
	ER	err;

	ClearInt(intno);
	n++;
	EndOfInt(intno);
}

INT usermain( void)
{
	T_DINT	dint;
	
	dint.intatr = TA_HLNG;
	dint.inthdr = inthdr1;
	tk_def_int(1, &dint);

	ClearInt(1);

	UW	int_status, reg;

	SetCtrlIntLevel(2);
	EnableInt(1, 3);
	raiseint1();

	EnableInt(1, 2);
	raiseint1();

	EnableInt(1, 1);
	raiseint1();

	DisableInt(1);
	reg = in_w(GICD_ICENABLER(0));
	out_w(GICD_ICENABLER(0),1);
	reg = in_w(GICD_ISENABLER(0));

	raiseint1();

	int_status = CheckInt(1);
	ClearInt(1);
	int_status = CheckInt(1);

	while(1);
	tk_slp_tsk(TMO_FEVR);
}

#else

#if USE_TMONITOR
#define TM_PUTSTRING(a)	tm_putstring((UB*)a)

void print_err( char* str, ER err)
{
	tm_printf((UB*)str, err);
}

#else
#define TM_PUTSTRING(a)

void print_err( char* str, INT par) {}

#endif /* USE_TMONITOR */

LOCAL void ptmrhdr(void *exinf)
{
	tk_wup_tsk(1);
}

EXPORT INT usermain( void )
{
	T_DPTMR		dptmr;

	TM_PUTSTRING("Start User-main program.\n");


	tm_putstring((UB*)"\n!!!START!!!\n");
	tk_dly_tsk(25*1000);
	tm_putstring((UB*)"\n!!!FINISHED!!!\n");

	dptmr.ptmratr = TA_HLNG;
	dptmr.ptmrhdr = ptmrhdr;
	DefinePhysicalTimerHandler(1, &dptmr);

	StartPhysicalTimer(1, (UW)(COUNT_PER_SEC*5), TA_CYC_PTMR);
	for(INT i = 0; i <5; i++) {
		tk_slp_tsk(TMO_FEVR);
		TM_PUTSTRING("WAKE UP - 1\n");
	}

	StopPhysicalTimer(1);
	TM_PUTSTRING("Stop PTMER - 1\n");

	dptmr.ptmratr = TA_HLNG;
	dptmr.ptmrhdr = ptmrhdr;
	DefinePhysicalTimerHandler(2, &dptmr);

	StartPhysicalTimer(2, (UW)(COUNT_PER_SEC*5), TA_CYC_PTMR);
	for(INT i = 0; i <5; i++) {
		tk_slp_tsk(TMO_FEVR);
		TM_PUTSTRING("WAKE UP - 2\n");
	}

	StopPhysicalTimer(2);
	TM_PUTSTRING("Stop PTMER - 2\n");

	tk_slp_tsk(TMO_FEVR);
	tk_slp_tsk(TMO_FEVR);
	return 0;
}
#endif
