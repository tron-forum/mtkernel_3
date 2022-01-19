/*
 *----------------------------------------------------------------------
 *    Device Driver for micro T-Kernel for μT-Kernel 3.00.05
 *
 *    Copyright (C) 2020-2021 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2021/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *	adc.h
 *	A/D converter device driver 
 *	Driver local definition
*/

#ifndef	__DEV_ADC_H__
#define	__DEV_ADC_H__

#include "../common/drvif/msdrvif.h"
#include "../include/dev_adc.h"
#include "adc_cnf.h"

/*----------------------------------------------------------------------*/
/* Hardware dependent definition
 */
#ifdef CPU_TMPM367FDFG
#include "sysdepend/tx03_m367/adc_m367.h"
#endif	/* CPU_TMPM367FDFG */
#ifdef CPU_RX231
#include "sysdepend/rx231/adc_rx231.h"
#endif	/* CPU_RX231 */
#ifdef CPU_STM32L4
#include "sysdepend/stm32l4/adc_stm32l4.h"
#endif	/* CPU_STM32L4 */
#ifdef CPU_RZA2M
#include "sysdepend/rza2m/adc_rza2m.h"
#endif	/* CPU_RZA2M */

/*----------------------------------------------------------------------*/
/* Device driver Control block
 */
typedef struct {
	UW	unit;		/* Unit No. */
	UINT	omode;		/* Open mode */

	/* Attribute data */
	ID	evtmbfid;	/* MBF ID for event notification */
} T_ADC_DCB;

/*----------------------------------------------------------------------*/
/* Device driver low-level interface
 */

/* The following functions are defined in the system-dependent part. */
IMPORT W dev_adc_llctl( UW, INT, UW, UW, UW*);	/* Low-Level device control */
IMPORT ER dev_adc_llinit( T_ADC_DCB*);		/* Low-level device initialization */

/* Low-level device control command */
typedef enum {
	LLD_ADC_OPEN,
	LLD_ADC_CLOSE,
	LLD_ADC_READ,
	LLD_ADC_RSIZE,
} T_LLD_ADC_CMD;

#endif		/* __DEV_ADC_H__ */
