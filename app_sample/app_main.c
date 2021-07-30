#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include <tk/tkernel.h>
#include <tm/tmonitor.h>

#include <tk/device.h>

#define ADC_MAX_UNIT	1
#define ADC_MAX_CH	8

LOCAL void read_ad(ID dd, UINT chno)
{
	UW	data;
	SZ	asize;
	ER	err;

	for(INT i = 0; i < 10; i++) {
		err = tk_srea_dev(dd, chno, &data, 1, &asize);
		if(err >= E_OK && asize == 1) {
			tm_printf((UB*)"ADC-%d %x\n", chno, data); 
		} else {
			tm_printf((UB*)"!!ERR Read-%d ercd %d  asize %d\n", chno, err, asize);
			return;
		}
		tk_dly_tsk(500);
	}
}

LOCAL void read_atr(ID dd)
{
	UW	data;
	SZ	asize;
	ER	err;

	err = tk_srea_dev(dd, TDN_EVENT, &data, sizeof(data), &asize);
	if(err >= E_OK && asize == sizeof(UW)) {
		tm_printf((UB*)"Read Atr %x\n", data); 
	} else {
		tm_printf((UB*)"!!ERR Read Atr ercd %d  asize %d\n", err, asize);
		return;
	}

	err = tk_srea_dev(dd, TDN_EVENT-1, &data, sizeof(data), &asize);
	if(err != E_PAR ) {
		tm_printf((UB*)"!!ERR Read Atr range err1 %d\n", err);
		return;
	}

	err = tk_srea_dev(dd, TDN_EVENT, &data, sizeof(data)-1, &asize);
	if(err != E_PAR ) {
		tm_printf((UB*)"!!ERR Read Atr range err2 %d\n", err);
		return;
	}
}

LOCAL void write_atr(ID dd)
{
	UW	data;
	SZ	asize;
	ER	err;

	data = 0xA5A50F0F;
	err = tk_swri_dev(dd, TDN_EVENT, &data, sizeof(data), &asize);
	if(err >= E_OK && asize == sizeof(UW)) {
		tm_printf((UB*)"Write Atr %x\n", data); 
	} else {
		tm_printf((UB*)"!!ERR Write Atr ercd %d  asize %d\n", err, asize);
		return;
	}

	err = tk_swri_dev(dd, TDN_EVENT-1, &data, sizeof(data), &asize);
	if(err != E_PAR ) {
		tm_printf((UB*)"!!ERR Write Atr range err1 %d\n", err);
		return;
	}

	err = tk_swri_dev(dd, TDN_EVENT, &data, sizeof(data)-1, &asize);
	if(err != E_PAR ) {
		tm_printf((UB*)"!!ERR Write Atr range err2 %d\n", err);
		return;
	}
}


EXPORT ER test_adc(void)
{
	UINT	unitno;
	UINT	chno;
	ID	dd;
	ER	err;

	UB	devnm[] = "adc ";

	for(unitno = 0; unitno < ADC_MAX_UNIT; unitno++) {
		tm_printf((UB*)"==== ADC Unit %d TEST\n", unitno);
		
		err = dev_init_adc(unitno);
		if(err < E_OK) {
			tm_printf((UB*)"!!ERR Init-%d err %d\n", unitno, err);
			return err;
		}

		devnm[3] = 'a'+unitno;
		err = tk_opn_dev(devnm, TD_UPDATE);
		if(err < E_OK) {
			tm_printf((UB*)"!!ERR Open-%d err %d\n", unitno, err);
			return err;
		}
		dd = (ID)err;

		for(chno = 0; chno <= ADC_MAX_CH; chno++) {
			tm_printf((UB*)"======= ADC_Ch %d\n", chno);
			if(chno < 3) {
				read_ad(dd, chno);
			}
		}

		read_atr(dd);
		write_atr(dd);
		read_atr(dd);

		err = tk_cls_dev(dd, 0);
		if(err < E_OK) {
			tm_printf((UB*)"!!ERR Close-%d err %d\n", unitno, err);
			return err;
		}
		tm_printf((UB*)"==== Unit %d End\n", unitno);
	}
	return err;
}
EXPORT INT usermain(void)
{
	ER	err;

	err = test_adc();
	tm_printf((UB*)"test end %d\n", err);
	return 0;
}