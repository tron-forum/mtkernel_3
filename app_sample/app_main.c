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
/* EEPROM R/W                                                  */
/*-------------------------------------------------------------*/
#define EEPROM_ADDR		0x50
#define	EEPROM_SIZE		256

ER read_eeprom( ID dd_eeprom, W addr, void *buf, SZ size)
{
	T_I2C_EXEC	exec;
	UB		snd_data;
	SZ	asz;
	ER	err;

	if(addr < 0 || addr > EEPROM_ADDR) return E_PAR;
	if((addr + size -1) >= EEPROM_SIZE ) return E_PAR;

	exec.sadr		= EEPROM_ADDR;
	snd_data		= addr;
	exec.snd_size	= 1;
	exec.snd_data	= &snd_data;
	exec.rcv_size	= size;
	exec.rcv_data	= buf;
	err = tk_swri_dev(dd_eeprom, TDN_I2C_EXEC, &exec, sizeof(T_I2C_EXEC), &asz);

	return err;
}

ER write_eeprom( ID dd_eeprom, W addr, void *buf, SZ size)
{
	static UB	data[EEPROM_SIZE];
	INT			i;
	SZ			asz;
	ER			err;

	if(addr < 0 || addr > EEPROM_ADDR) return E_PAR;
	if((addr + size -1) >= EEPROM_SIZE ) return E_PAR;

	while(size > 0) {
		data[0] = addr;
		if(size > 8) {
			knl_memcpy(&data[1], buf, 8);
			err = tk_swri_dev(dd_eeprom, EEPROM_ADDR, data, 9, &asz);
			if(err != E_OK) return err;
			size -= 8; buf += 8; addr += 8;
		} else {
			knl_memcpy(&data[1], buf, size);
			err = tk_swri_dev(dd_eeprom, EEPROM_ADDR, data, size+1, &asz);
			if(err != E_OK) return err;
			size = 0;
		}
		tk_dly_tsk(1);
	}
	return err;
}

LOCAL void test_eeprom(void)
{
	ID	dd_i2c;
	UB	wd[10], rd[10];
	ER	err;

	dd_i2c = tk_opn_dev((UB*)"iica", TD_UPDATE);
	err = read_eeprom(dd_i2c, 0, rd, 5);
	for(INT i = 0; i < 5; i++) {
		tm_printf((UB*)"%x ", rd[i]);
	}
	tm_putchar('\n');

	for(INT i = 0; i < 5; i++) {
		wd[i] = 'a'+i;
	}
	err = write_eeprom(dd_i2c, 0, wd, 5);
	err = read_eeprom(dd_i2c, 0, rd, 5);
	for(INT i = 0; i < 5; i++) {
		tm_printf((UB*)"%c ", rd[i]);
	}
	tm_putchar('\n');

	for(INT i = 0; i < 5; i++) {
		wd[i] = 'A'+i;
	}
	err = write_eeprom(dd_i2c, 0, wd, 5);
	err = read_eeprom(dd_i2c, 0, rd, 5);
	for(INT i = 0; i < 5; i++) {
		tm_printf((UB*)"%c ", rd[i]);
	}
	tm_putchar('\n');
}

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
//	dev_init_adc(1);
//	dd_ad1 = tk_opn_dev((UB*)"adcb", TD_READ);

	while(1) {
		// LED (on Board)
		out_w(GPIO_ODR(D), (in_w(GPIO_ODR(D)))^((1<<11)|(1<<15)));

		// A/D (on Board)
		err = tk_srea_dev(dd_ad1, 6, &data[0], 4, &asz);
		tk_srea_dev(dd_ad1, 16, &data[4], 1, NULL);
		tm_printf((UB*)"(%d) %d  %d  %d  %d\n", asz, data[0], data[4], data[1], data[3]);

		tk_dly_tsk(500);
	}
}

/*--------------------------------------------------------------*/
/* Arduino							*/
/*--------------------------------------------------------------*/
#include "gesture.h"

LOCAL void test_arduino(void)
{
	ID	dd_ad1;
	UW	data[10];
	UW	val;
	SZ	asz;
	ER	err;

	dev_init_adc(1);
	dd_ad1 = tk_opn_dev((UB*)"adcb", TD_READ);

	gesture_sensor_init(1);

	while(1) {
		// A/D (on Board)
		err = tk_srea_dev(dd_ad1, 4, &data[0], 2, &asz);
		tk_srea_dev(dd_ad1, 13, &data[2], 1, NULL);
		tm_printf((UB*)"(%d) %d  %d  %d\n", asz, data[0], data[1], data[2]);

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

	dd = tk_opn_dev((UB*)"serb", TD_UPDATE);
	for(i = 0; i < 10; i++) {
		tk_srea_dev(dd, 0, &data, 1, &asz);
		tk_swri_dev(dd, 0, &data, 1, &asz);
	}
	tm_printf((UB*)"\n");
	for(data = 'A', i = 0; i <26; i++, data++) {
		tk_swri_dev(dd, 0, &data, 1, &asz);
	}
	tm_printf((UB*)"\n");
}

LOCAL void task_1(INT stacd, void *exinf)
{


//	test_eeprom();
//	test_adc();
//	test_arduino();
	test_uart();

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