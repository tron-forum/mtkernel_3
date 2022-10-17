/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
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
#define DEVDEF_ADC_PATH_(a)	#a
#define DEVDEF_ADC_PATH(a)	DEVDEF_ADC_PATH_(a)
#define DEVDEF_ADC_SYSDEP()	DEVDEF_ADC_PATH(sysdepend/TARGET_CPU_DIR/adc_sysdep.h)
#include DEVDEF_ADC_SYSDEP()

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
