/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.00.03
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/03/31.
 *
 *----------------------------------------------------------------------
 */

/*
 *	adc_cnf.h 
 *	A/D converter device driver configuration file
 */
#ifndef	__DEV_ADC_CNF_H__
#define	__DEV_ADC_CNF_H__

#define	DEVCNF_ADC_DEVNAME	"adc"		/* Device name ("adc") */


/*---------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#ifdef CPU_TMPM367FDFG
#include "sysdepend/tx03_m367/adc_cnf_m367.h"
#endif		/* CPU_TMPM367FDFG */
#ifdef CPU_RX231
#include "sysdepend/rx231/adc_cnf_rx231.h"
#endif		/* CPU_RX231 */
#ifdef CPU_STM32L4
#include "sysdepend/stm32l4/adc_cnf_stm32l4.h"
#endif		/* CPU_STM32L4 */

#endif		/* __DEV_ADC_CNF_H__ */
