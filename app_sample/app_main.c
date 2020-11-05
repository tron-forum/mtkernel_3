/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.02
 *
 *    Copyright (C) 2006-2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/21.
 *
 *----------------------------------------------------------------------
 */

#include <tk/tkernel.h>
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

ID	id1;
UW	tbl1[100], tbl2[100], tbl3[100], tbl4[100];

void ptmrhdr1( void *exinf)
{
	static	INT	cnt1	= 0;
	static	INT	cnt2	= 0;
	SYSTIM		tim;

	if(++cnt1 >= 10) {
		cnt1 = 0;
		tk_get_tim(&tim);
		tbl1[cnt2++] = tim.lo;
		if(cnt2 >= 100) {
			StopPhysicalTimer(1);
			tk_wup_tsk(id1);
		}
	}
}

void ptmrhdr2( void *exinf)
{
	static	INT	cnt1	= 0;
	static	INT	cnt2	= 0;
	SYSTIM		tim;

	if(++cnt1 >= 10) {
		cnt1 = 0;
		tk_get_tim(&tim);
		tbl2[cnt2++] = tim.lo;
		if(cnt2 >= 100) {
			StopPhysicalTimer(2);
			tk_wup_tsk(id1);
		}
	}
}

void ptmrhdr3( void *exinf)
{
	static	INT	cnt1	= 0;
	static	INT	cnt2	= 0;
	SYSTIM		tim;

	if(++cnt1 >= 10) {
		cnt1 = 0;
		tk_get_tim(&tim);
		tbl3[cnt2++] = tim.lo;
		if(cnt2 >= 100) {
			StopPhysicalTimer(3);
			tk_wup_tsk(id1);
		}
	}
}

void ptmrhdr4( void *exinf)
{
	static	INT	cnt1	= 0;
	static	INT	cnt2	= 0;
	SYSTIM		tim;

	if(++cnt1 >= 10) {
		cnt1 = 0;
		tk_get_tim(&tim);
		tbl4[cnt2++] = tim.lo;
		if(cnt2 >= 100) {
			StopPhysicalTimer(4);
			tk_wup_tsk(id1);
		}
	}
}

/* ----------------------------------------------------------
 *
 * User Task-1 Definition
 *
 */
void tsk1(INT stacd, void *exinf)
{
	T_DPTMR	dptmr;
	SYSTIM	tim;
	ER	err;

	tm_putstring((UB*)"Start Task-1\n");

	dptmr.exinf	= NULL;
	dptmr.ptmratr	= TA_HLNG;

	dptmr.ptmrhdr	= ptmrhdr1;
	err = DefinePhysicalTimerHandler(1, &dptmr);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}

	dptmr.ptmrhdr	= ptmrhdr2;
	err = DefinePhysicalTimerHandler(2, &dptmr);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}

	dptmr.ptmrhdr	= ptmrhdr3;
	err = DefinePhysicalTimerHandler(3, &dptmr);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}

	dptmr.ptmrhdr	= ptmrhdr4;
	err = DefinePhysicalTimerHandler(4, &dptmr);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}

	tim.hi = tim.lo = 0;
	tk_set_tim(&tim);

	err = StartPhysicalTimer(1, 8000, TA_CYC_PTMR);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}
	err = StartPhysicalTimer(2, 8000, TA_CYC_PTMR);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}
	err = StartPhysicalTimer(3, 8000, TA_CYC_PTMR);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}
	err = StartPhysicalTimer(4, 8000, TA_CYC_PTMR);
	if(err != E_OK){
		tm_printf((UB*)"ERR %d\n",err);
	}


	tk_slp_tsk(TMO_FEVR);
	tm_putstring((UB*)"Wakeup 1\n");
	for(INT i=0; i< 100; i++) {
		tm_printf((UB*)"%d\n", tbl1[i]);
	}

	tk_slp_tsk(TMO_FEVR);
	tm_putstring((UB*)"Wakeup 2\n");
	for(INT i=0; i< 100; i++) {
		tm_printf((UB*)"%d\n", tbl2[i]);
	}

	tk_slp_tsk(TMO_FEVR);
	tm_putstring((UB*)"Wakeup 3\n");
	for(INT i=0; i< 100; i++) {
		tm_printf((UB*)"%d\n", tbl3[i]);
	}

	tk_slp_tsk(TMO_FEVR);
	tm_putstring((UB*)"Wakeup 4\n");
	for(INT i=0; i< 100; i++) {
		tm_printf((UB*)"%d\n", tbl4[i]);
	}

	tk_exd_tsk();	/* Exit task */
}

const T_CTSK	ctsk1	= {0, (TA_HLNG | TA_RNG1), &tsk1, 10, 1024, 0};

/* ----------------------------------------------------------
 *
 * User-Main Definition (Run on initial task)
 *
 */

EXPORT INT usermain( void )
{
	T_RVER	rver;

	tm_putstring((UB*)"Start User-main program.\n");

	tk_ref_ver(&rver);		/* Get the OS Version. */

	tm_printf((UB*)"Make Code: %04x  Product ID: %04x\n", rver.maker, rver.prid);
	tm_printf((UB*)"Product Ver. %04x\nProduct Num. %04x %04x %04x %04x\n", 
			rver.prver, rver.prno[0],rver.prno[1],rver.prno[2],rver.prno[3]);

	id1 = tk_cre_tsk(&ctsk1);
	tk_sta_tsk(id1, 0);

//	id2 = tk_cre_tsk(&ctsk2);
//	tk_sta_tsk(id2, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
