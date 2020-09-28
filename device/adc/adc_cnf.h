/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.0
 *
 *    Copyright (C) 2020 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2020/10/.
 *
 *----------------------------------------------------------------------
 */

/*
 *	adc_cnf.h 
 *	A/D converter device driver configuration file
 */
#ifndef	__DEV_ADC_CNF_H__
#define	__DEV_ADC_CNF_H__

#define	DEVCNF_ADC_DEVNAME	"adc"		// Device name ("adc") */


/*---------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#ifdef CPU_TMPM369FDFG
#include "sysdepend/tx03_m367/adc_cnf_m367.h"
#endif		/* CPU_TMPM369FDFG */
#ifdef CPU_RX231
#include "sysdepend/rx231/adc_cnf_rx231.h"
#endif		/* CPU_RX231 */

#endif		/* __DEV_ADC_CNF_H__ */
