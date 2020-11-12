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


/* ----------------------------------------------------------
 *
 * User Task-1 Definition
 *
 */
void tsk1(INT stacd, void *exinf)
{
	ID	da, db;
	UW	data[3];
	SZ	asz;
	ER	err;

	tm_putstring((UB*)"Start Task-1\n");

	da = tk_opn_dev((UB*)"adca", TD_UPDATE);
	if(da <= E_OK)	tm_printf((UB*)"open adca err %x\n",da);

	db = tk_opn_dev((UB*)"adcb", TD_UPDATE);
	if(db <= E_OK)	tm_printf((UB*)"open adcb err %x\n",da);

	while(1) {
#if 0
		err = tk_srea_dev(da, 9, &data[0], 1, &asz);	// Light sensor
		if(err < E_OK) tm_printf((UB*)"read-1 err %x\n", err);

		err = tk_srea_dev(da, 6, &data[1], 2, &asz);	// Joystick, Temp sensor
		if(err < E_OK) tm_printf((UB*)"read-2 err %x\n", err);

		tm_printf((UB*)"LS %x JS %x  TS %x\n", data[0], data[1], data[2]);
#endif
		err = tk_srea_dev(da, 16, &data[0], 1, &asz);	// Joystick
		if(err < E_OK) tm_printf((UB*)"read-3 err %x\n", err);
		tm_printf((UB*)"JS-y %x\n", data[0]);

		err = tk_srea_dev(da, 6, &data[0], 1, &asz);	// Joystick
		if(err < E_OK) tm_printf((UB*)"read-1 err %x\n", err);
		tm_printf((UB*)"JS-x %x\n", data[0]);

		err = tk_srea_dev(da, 9, &data[0], 1, &asz);	// Light sensor
		if(err < E_OK) tm_printf((UB*)"read-2 err %x\n", err);
		tm_printf((UB*)"LS %x\n", data[0]);

		tk_dly_tsk(200);
	}

	err = tk_cls_dev(da, 0);
	if(err < E_OK)	tm_printf((UB*)"close err %x\n",err);

	tm_putstring((UB*)"End Task-1\n");
	tk_exd_tsk();	/* Exit task */
}

/* ---------------------------------------------------------
 *
 * User Task-2 Definition
 *
 */
void tsk2(INT stacd, void *exinf)
{
	tm_putstring((UB*)"Start Task-2\n");

	tm_putstring((UB*)"End Task-2\n");
	tk_exd_tsk();	/* Exit Task */
}

const T_CTSK	ctsk1	= {0, (TA_HLNG | TA_RNG1), &tsk1, 10, 1024, 0};
const T_CTSK	ctsk2	= {0, (TA_HLNG | TA_RNG1), &tsk2, 11, 1024, 0};

/* ----------------------------------------------------------
 *
 * User-Main Definition (Run on initial task)
 *
 */

EXPORT INT usermain( void )
{
	T_RVER	rver;
	ID	id1, id2;
	ER	err;

	tm_putstring((UB*)"Start User-main program.\n");

	tk_ref_ver(&rver);		/* Get the OS Version. */

	tm_printf((UB*)"Make Code: %04x  Product ID: %04x\n", rver.maker, rver.prid);
	tm_printf((UB*)"Product Ver. %04x\nProduct Num. %04x %04x %04x %04x\n", 
			rver.prver, rver.prno[0],rver.prno[1],rver.prno[2],rver.prno[3]);

	/* Pin function */
	// Light sensor	: PA4 ADC1_IN9
	// Temp Sensor	: PA2 ADC1_IN7
	// Joystick - X	: PA1 ADC1_IN6
	out_w(GPIO_ASCR(A), (1<<4)|(1<<2)|(1<<1));

	// Joystick - Y	: PB1 ADC1_IN16
	out_w(GPIO_ASCR(B), (1<<1));

	err = dev_init_adc(0);
	if(err < E_OK) tm_printf((UB*)"Init A/DC error %x\n", err);

	err = dev_init_adc(1);
	if(err < E_OK) tm_printf((UB*)"Init A/DC error %x\n", err);

	id1 = tk_cre_tsk(&ctsk1);
	tk_sta_tsk(id1, 0);

	id2 = tk_cre_tsk(&ctsk2);
//	tk_sta_tsk(id2, 0);

	tk_slp_tsk(TMO_FEVR);

	return 0;
}
