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

#include <tk/tkernel.h>
#include <tm/tmonitor.h>
#include <tk/device.h>

LOCAL void task_1(INT stacd, void *exinf);
LOCAL ID	tskid_1;
LOCAL T_CTSK	ctsk_1 = {
	.itskpri	= 10,
	.stksz		= 1024,
	.task		= task_1,
	.tskatr		= TA_HLNG | TA_RNG3,
};

/*-------------------------------------------------------------*/
/* A/D C (in Board)                                            */
/*-------------------------------------------------------------*/
LOCAL void test_adc(void)
{
	ID	dd_ad1;
	UW	data[10];
	UB	wd[10], rd[10];
	SZ	asz;
	ER	err;

	dd_ad1 = tk_opn_dev((UB*)"adca", TD_READ);

	while(1) {
		// LED (on Board)
//		out_w(GPIO_ODR(D), (in_w(GPIO_ODR(D)))^((1<<11)|(1<<15)));

		// A/D (on Board)
		err = tk_srea_dev(dd_ad1, 0, &data[0], 3, &asz);
		tm_printf((UB*)"(%d) %d  %d  %d\n", asz, data[0], data[1], data[2]);

		tk_dly_tsk(500);
	}
}

/*--------------------------------------------------------------*/
/* I2C							*/
/*--------------------------------------------------------------*/
#include "gesture.h"

LOCAL void test_i2c(void)
{
	ID	dd_ad1;
	UW	data[10];
	UW	val;
	SZ	asz;
	ER	err;


	gesture_sensor_init(0);

	while(1) {
		// Gesture sensor
		gesture_sensor_get(&val);

		if(val & GES_RIGHT_FLAG ) {
			tm_printf((UB*)"Right\n");
		} else if(val & GES_LEFT_FLAG) {
			tm_printf((UB*)"Left\n");
		} else if(val & GES_UP_FLAG) {
			tm_printf((UB*)"Up\n");
		} else if(val & GES_DOWN_FLAG) {
			tm_printf((UB*)"Down\n");
		}

		if(val & GES_FORWARD_FLAG) {
			tm_printf((UB*)"Forward\n");
		} else if(val & GES_BACKWARD_FLAG) {
			tm_printf((UB*)"Balckward\n");
		}
		if(val & GES_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Clockwise\n");
		} else if(val & GES_COUNT_CLOCKWISE_FLAG) {
			tm_printf((UB*)"Anti-Cockwise\n");
		}

		tk_dly_tsk(500);
	}

}

LOCAL void test_uart(void)
{
	ID	dd;
	UB	data;
	SZ	asz;
	INT	i;

	dd = tk_opn_dev((UB*)"serd", TD_UPDATE);
	for(i = 0; i < 10; i++) {
		tk_srea_dev(dd, 0, &data, 1, &asz);
		tk_swri_dev(dd, 0, &data, 1, &asz);
	}
	tm_printf((UB*)"\n\r");
	for(data = 'A', i = 0; i <26; i++, data++) {
		tk_swri_dev(dd, 0, &data, 1, &asz);
	}
	tm_printf((UB*)"\n\r");
}

LOCAL void task_1(INT stacd, void *exinf)
{


//	test_adc();
	test_uart();
	test_i2c();

	tk_ext_tsk();
}

/* usermain関数 */
EXPORT INT usermain(void)
{
	tm_printf((UB*)"User program started\n");

	tskid_1 = tk_cre_tsk(&ctsk_1);
	tk_sta_tsk(tskid_1, 0);

	tk_slp_tsk(TMO_FEVR);
	return 0;
}