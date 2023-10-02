/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2020-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/03.
 *
 *----------------------------------------------------------------------
 */

#include <sys/machine.h>
#ifdef CPU_STM32L4

#include <tk/tkernel.h>
#include "../../i2c.h"
#include "../../../include/dev_def.h"
#if DEV_IIC_ENABLE
/*
 *	i2c_stm32l4.c
  *	I2C device driver
 *	System-dependent definition for STM32L4
 */

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_I2C_UNITNM] = { I2C1_BASE, I2C2_BASE, I2C3_BASE };

#define	I2C_CR1(u)	(ba[u] + I2Cx_CR1)
#define	I2C_CR2(u)	(ba[u] + I2Cx_CR2)
#define	I2C_TIMINGR(u)	(ba[u] + I2Cx_TIMINGR)
#define	I2C_ISR(u)	(ba[u] + I2Cx_ISR)
#define	I2C_ICR(u)	(ba[u] + I2Cx_ICR)
#define	I2C_ICR(u)	(ba[u] + I2Cx_ICR)
#define	I2C_RXDR(u)	(ba[u] + I2Cx_RXDR)
#define	I2C_TXDR(u)	(ba[u] + I2Cx_TXDR)

/*----------------------------------------------------------------------
 * Device data
*/
const LOCAL struct {
	UINT	intno;		// Interrupt number
	PRI	intpri;		// Interrupt priority
	TMO	timout;		// I2C timeout time
} ll_devdat[DEV_I2C_UNITNM] = {
	{	/* I2C1 */
		.intno		= INTNO_I2C1_EV,
		.intpri		= DEVCNF_I2C1_INTPRI,
		.timout		= DEVCNF_I2C1_TMO,
	},
	{	/* I2C2 */
		.intno		= INTNO_I2C2_EV,
		.intpri		= DEVCNF_I2C2_INTPRI,
		.timout		= DEVCNF_I2C2_TMO,
	},
	{	/* I2C3 */
		.intno		= INTNO_I2C3_EV,
		.intpri		= DEVCNF_I2C3_INTPRI,
		.timout		= DEVCNF_I2C3_TMO,
	},
};

/*----------------------------------------------------------------------
 * Device control data
*/
typedef struct {
	ID	wait_tskid;	/* Wait Task ID */
	UW	state;		/* Operating state */
	UW	sadr;		/* Slave address */
	ER	ioerr;		/* Communication error */
	UW	sdat_num;	/* Number of send data */
	UW	rdat_num;	/* Number of receive data */
	UB	*sbuf;		/* Send buffer */
	UB	*rbuf;		/* Receive buffer */
} T_I2C_LLDCB;
LOCAL T_I2C_LLDCB	ll_devcb[DEV_I2C_UNITNM];


/*-------------------------------------------------------
 * Interrupt handler
 */
/*-------------------------------------------------------
 * Event interrupt handler
 */
LOCAL void i2c_evhdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	UW		i2c_st;
	INT		unit;
	BOOL		wup	= FALSE;

	for ( unit = 0; unit < DEV_I2C_UNITNM; unit++ ) {
		if ( ll_devdat[unit].intno == intno ) {
			p_cb = &ll_devcb[unit];
			break;
		}
	}
	if(unit >= DEV_I2C_UNITNM) {
		ClearInt(intno);	// Clear interrupt
		return;
	}

	i2c_st = in_w(I2C_ISR(unit));
	if( i2c_st & I2C_ISR_NACKF) {
		p_cb->ioerr = E_IO;
		wup = TRUE;

	} else if( i2c_st & I2C_ISR_TXIS) {		/* TX interrupt */
		out_w(I2C_TXDR(unit),*p_cb->sbuf++);
		if(--(p_cb->sdat_num) <= 0) {			/* final data ? */
			if(p_cb->rdat_num == 0) {
				out_w(I2C_CR1(unit), I2C_CR1_PE);
				p_cb->state = I2C_STS_STOP;
				wup = TRUE;
			} else {
				out_w(I2C_CR1(unit), I2C_CR1_PE | I2C_CR1_TCIE | I2C_CR1_RXIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE );
			}
		}

	} else if( i2c_st & I2C_ISR_TC) {		/* Transfer complete */
		out_w(I2C_CR1(unit), I2C_CR1_PE | I2C_CR1_RXIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE );
		p_cb->state = I2C_STS_START;
		wup = TRUE;

	} else if( i2c_st & I2C_ISR_RXNE) {	/* RX interrupt */
		*(p_cb->rbuf++) = (UB)in_w(I2C_RXDR(unit));
		if(--(p_cb->rdat_num) <= 0) {			/* final data ? */
			out_w(I2C_CR1(unit), I2C_CR1_PE);
			p_cb->state = I2C_STS_STOP;
			wup = TRUE;
		}
	} else {	/* Error */
		out_w(I2C_CR1(unit), I2C_CR1_PE);
		p_cb->ioerr = E_IO;
	}

	out_w(I2C_ICR(unit), I2C_ICR_ALL);	// Clear all Interrupt flag
	ClearInt(intno);			// Clear interrupt

	if(wup) {
		if(p_cb->wait_tskid) {
			tk_wup_tsk(p_cb->wait_tskid);
			p_cb->wait_tskid = 0;
		}
	}
}

/*-------------------------------------------------------
 * Error interrupt handler
 */
LOCAL void i2c_erhdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;

	intno--;
	for ( unit = 0; unit < DEV_I2C_UNITNM; unit++ ) {
		if ( ll_devdat[unit].intno == intno ) {
			p_cb = &ll_devcb[unit];
			break;
		}
	}
	if(unit >= DEV_I2C_UNITNM) return;

	out_w(I2C_ICR(unit), I2C_ICR_ALL);	// Clear all Interrupt flag
	ClearInt(intno);			// Clear interrupt
	
	out_w(I2C_CR1(unit), I2C_CR1_PE);

	p_cb->ioerr = E_IO;
	if(p_cb->wait_tskid) {
		tk_wup_tsk(p_cb->wait_tskid);
		p_cb->wait_tskid = 0;
	}
}

/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans(INT unit, T_I2C_LLDCB *p_cb)
{
	UW		i2c_ctl;
	UINT		imask;
	ER		err;

	p_cb->ioerr		= E_OK;
	p_cb->wait_tskid	= 0;

	/* I2C Device enable */
	out_w(I2C_CR1(unit), I2C_CR1_PE | I2C_CR1_TXIE | I2C_CR1_RXIE | I2C_CR1_NACKIE | I2C_CR1_ERRIE );

	while ((err = p_cb->ioerr) == E_OK ) {

		switch ( p_cb->state ) {
		case I2C_STS_START:
			DI(imask);
			i2c_ctl = p_cb->sadr <<1;
			if(p_cb->sdat_num > 0 ) {	/* Send */
				i2c_ctl |= p_cb->sdat_num <<16 ;
				if( p_cb->rdat_num == 0) {
					i2c_ctl |= I2C_CR2_AUTOEND;
				}
				out_w(I2C_CR2(unit), i2c_ctl);
				p_cb->state = I2C_STS_SEND;
			} else {			/* Receive */
				i2c_ctl |= p_cb->rdat_num <<16 | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND;
				p_cb->state = I2C_STS_RECV;
				out_w(I2C_CR2(unit), i2c_ctl);
			}

			out_w(I2C_CR2(unit), i2c_ctl | I2C_CR2_START);
			EI(imask);
			break;

		case I2C_STS_STOP:
			while(in_w(I2C_ISR(unit)) & I2C_ISR_BUSY) {
				tk_dly_tsk(1);
			}
			break;
		}
		if ( p_cb->state == I2C_STS_STOP ) break;

		p_cb->wait_tskid = tk_get_tid();
		err = tk_slp_tsk(ll_devdat[unit].timout);
		if ( err < E_OK ) break;
	}

	out_w(I2C_CR1(unit), 0);	/* I2C Device disable */

	return err;
}

/*----------------------------------------------------------------------
 * Set communication parameters
 */
Inline void set_com_start(UW unit, UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
	ll_devcb[unit].state	= I2C_STS_START;
	ll_devcb[unit].sadr	= sadr;		/* Slave address */
	ll_devcb[unit].sdat_num	= sdat_num;	/* Number of send data */
	ll_devcb[unit].rdat_num	= rdat_num;	/* Number of receive data */
	ll_devcb[unit].sbuf	= sbuf;		/* Send buffer */
	ll_devcb[unit].rbuf	= rbuf;		/* Receive buffer */
}


/*----------------------------------------------------------------------
 * Low level device control
 */
EXPORT W dev_i2c_llctl( UW unit, INT cmd, UW p1, UW p2, UW *pp)
{
	T_I2C_EXEC	*p_ex;
	ER		err	= E_OK;

	switch(cmd) {
	case LLD_I2C_OPEN:
		/* I2C interrupt enable */
		EnableInt(ll_devdat[unit].intno, ll_devdat[unit].intpri);
		EnableInt(ll_devdat[unit].intno + 1, ll_devdat[unit].intpri);
		break;

	case LLD_I2C_CLOSE:
		/* I2C interrupt disable */
		DisableInt(ll_devdat[unit].intno);
		DisableInt(ll_devdat[unit].intno + 1);
		break;

	case LLD_I2C_READ:
		set_com_start( unit, p1, 0, p2, NULL, (UB*)pp);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p2 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_WRITE:
		set_com_start( unit, p1, p2, 0, (UB*)pp, NULL);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p2 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_EXEC:
		p_ex = (T_I2C_EXEC*)pp;
		set_com_start( unit, p_ex->sadr, p_ex->snd_size, p_ex->rcv_size, p_ex->snd_data, p_ex->rcv_data);
		err = i2c_trans(unit, &ll_devcb[unit]);
		if(err >= E_OK) err = p_ex->snd_size + p_ex->rcv_size;
		break;
	}
	
	return (W)err;
}

/*----------------------------------------------------------------------
 * Device initialization
 */
EXPORT ER dev_i2c_llinit( T_I2C_DCB *p_dcb)
{
	T_DINT	dint;

	UW	unit;
	UW	intno;
	ER	err;

	unit = p_dcb->unit;

#if DEVCNF_I2C_INIT_MCLK
	/* Select clock source */
	out_w(RCC_CCIPR, (in_w(RCC_CCIPR) & ~RCC_CCIPR_I2CxSEL) | DEVCNF_I2CxSEL_INIT );

	/* Enable module clock */
	*(_UW*)RCC_APB1ENR1 |= (RCC_APB1ENR1_I2C1EN << unit);
#endif

	out_w(I2C_CR1(unit), 0);				// I2C disable
	out_w(I2C_TIMINGR(unit), I2C_TIMINGR_INIT);		// I2C Initial setting

	/* Interrupt handler definition */
	intno		= ll_devdat[unit].intno;
	dint.intatr	= TA_HLNG;
	dint.inthdr	= i2c_evhdr;
	
	err = tk_def_int(intno, &dint);		// I2C event interrupt
	if(err < E_OK) return err;

	dint.inthdr	= i2c_erhdr;
	err = tk_def_int(intno + 1, &dint);

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* CPU_STM32L4 */