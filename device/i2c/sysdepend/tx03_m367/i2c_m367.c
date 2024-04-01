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
#ifdef CPU_TMPM367FDFG

#include <tk/tkernel.h>
#include "../../i2c.h"
#include "../../../include/dev_def.h"
#if DEV_IIC_ENABLE
/*
 *	i2c_m367.c
  *	I2C device driver
 *	System-dependent definition for TX03_M367
 */

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_I2C_UNITNM] = { I2C0_BASE, I2C1_BASE, I2C2_BASE };

/*----------------------------------------------------------------------
 * Device initialization value
*/
const LOCAL struct {
	TMO	timout;		// I2C timeout time
	PRI	intpri;		// Interrupt priority
	UW	sck;		// Frequency selection
} ll_devinit[DEV_I2C_UNITNM] = {
	{	/* SBI0 */
		.timout		= DEVCNF_I2C0_TMO,
		.intpri		= DEVCNF_I2C0_INTPRI,
		.sck		= DEVCNF_I2C0_SCK,
	},
	{	/* SBI1 */
		.timout		= DEVCNF_I2C1_TMO,
		.intpri		= DEVCNF_I2C1_INTPRI,
		.sck		= DEVCNF_I2C1_SCK,
	},
	{	/* SBI2 */
		.timout		= DEVCNF_I2C2_TMO,
		.intpri		= DEVCNF_I2C2_INTPRI,
		.sck		= DEVCNF_I2C2_SCK,
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
 *-------------------------------------------------------
 */
LOCAL void i2c_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	UW		sbisr;
	INT		unit;

	for ( unit = 0; unit < DEV_I2C_UNITNM; unit++ ) {
		if ( INTNO_INTSBI(unit) == intno ) {
			p_cb = &ll_devcb[unit];
			break;
		}
	}
	if(unit >= DEV_I2C_UNITNM) return;

	sbisr = in_w(ba[unit] + I2C_SBIxSR);
	if ( (sbisr & I2C_SBIxSR_AL) != 0 ) {
		/* Communication error */
		p_cb->ioerr = E_IO;
		goto stop;
	}

	switch( p_cb->state ) {
	case I2C_STS_RECV | I2C_STS_TOP:
		if(p_cb->rdat_num == 1) {
			out_w(ba[unit] + I2C_SBIxCR1, 
					I2C_SBIxCR1_INIT & ~I2C_SBIxCR1_ACK);
		}
		in_w(ba[unit] + I2C_SBIxDBR);
		p_cb->state = I2C_STS_RECV;
		break;

	case I2C_STS_RECV:
		if(p_cb->rdat_num == 1) {
			out_w(ba[unit] + I2C_SBIxCR1, 
					I2C_SBIxCR1_INIT & ~I2C_SBIxCR1_ACK);
		}
		*(p_cb->rbuf++) = (UB)(in_w(ba[unit] + I2C_SBIxDBR) & 0xff);
		if(--(p_cb->rdat_num) <= 0) {
			p_cb->state = I2C_STS_STOP;
		}
		break;
	
	case I2C_STS_SEND:
		if ( (sbisr & I2C_SBIxSR_LRB) != 0 ) {
			p_cb->ioerr = 1; /* Interruption by NAK */
			goto stop;
		}

		out_w(ba[unit] + I2C_SBIxDBR, *p_cb->sbuf++);
		if(--(p_cb->sdat_num) <= 0) {
			if(p_cb->rdat_num == 0) {
				p_cb->state = I2C_STS_STOP;
			} else {
				p_cb->state = I2C_STS_RESTART;
			}
		}
		break;
	
	case I2C_STS_RESTART:
		out_w(ba[unit] + I2C_SBIxCR2, I2C_SBIxCR2_RESTART);
		if(p_cb->wait_tskid) {
			tk_wup_tsk(p_cb->wait_tskid);
			p_cb->wait_tskid = 0;
		}
		break;

	case I2C_STS_STOP:
	stop:
		out_w(ba[unit] + I2C_SBIxCR2, I2C_SBIxCR2_STOP);
		if(p_cb->wait_tskid) {
			tk_wup_tsk(p_cb->wait_tskid);
			p_cb->wait_tskid = 0;
		}
		break;
	}
}

/*----------------------------------------------------------------------
 * Waiting for status change
 */
LOCAL void i2c_wait_status( T_I2C_LLDCB *p_cb, INT unit, UW mask, UW value )
{
	_UW	t = CLOCK_fc;	/* Timeout value */

	while ( p_cb->ioerr == E_OK ) {
		if ( (in_w(ba[unit] + I2C_SBIxSR) & mask) == value ) break;
		if ( --t == 0 ) p_cb->ioerr = E_TMOUT;
	}
	return;
}

/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans(INT unit, T_I2C_LLDCB *p_cb)
{
	UINT		imask;
	ER		err;

	p_cb->ioerr		= E_OK;
	p_cb->wait_tskid	= 0;

	while ((err = p_cb->ioerr) == E_OK ) {

		switch ( p_cb->state ) {
		  case I2C_STS_RESTART:
			i2c_wait_status(p_cb, unit, I2C_SBIxSR_BB, 0);
			i2c_wait_status(p_cb, unit, I2C_SBIxSR_LRB, I2C_SBIxSR_LRB);
			WaitUsec(5);
			/* no break */

		  case I2C_STS_START:
			DI(imask);
			out_w(ba[unit] + I2C_SBIxCR1, I2C_SBIxCR1_INIT);
			if(p_cb->sdat_num > 0 ) {	/* Send */
				out_w(ba[unit] + I2C_SBIxDBR, p_cb->sadr);
				p_cb->state = I2C_STS_SEND;
			} else {			/* Receive */
				out_w(ba[unit] + I2C_SBIxDBR, (p_cb->sadr|1));
				p_cb->state = I2C_STS_RECV | I2C_STS_TOP;
			}
			out_w(ba[unit] + I2C_SBIxCR2, I2C_SBIxCR2_START);
			EI(imask);
			break;

		  case I2C_STS_STOP:
			i2c_wait_status(p_cb, unit, I2C_SBIxSR_BB, 0);
			break;
		}
		if ( (err = p_cb->ioerr) != E_OK || p_cb->state == I2C_STS_STOP ) break;

		p_cb->wait_tskid = tk_get_tid();
		err = tk_slp_tsk(ll_devinit[unit].timout);
		if ( err < E_OK ) break;
	}

	/* Communication stop */
	DI(imask);
	out_w(ba[unit] + I2C_SBIxCR2, I2C_SBIxCR2_STOP);
	EI(imask);

	return err;
}

/*----------------------------------------------------------------------
 * Set communication parameters
 */
Inline void set_com_start(UW unit, UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
	ll_devcb[unit].state	= I2C_STS_START;
	ll_devcb[unit].sadr	= sadr<<1;	/* Slave address */
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
		/* I2C Device reset */
		out_w(ba[unit] + I2C_SBIxCR0, I2C_SBIxCR0_SBIEN);
		out_w(ba[unit] + I2C_SBIxCR2, I2C_SBIxCR2_SBIM_I2C | I2C_SBIxCR2_SWRST(2));
		out_w(ba[unit] + I2C_SBIxCR2, I2C_SBIxCR2_SBIM_I2C | I2C_SBIxCR2_SWRST(1));
		while ( (in_w(ba[unit] + I2C_SBIxCR1) & I2C_SBIxCR1_SWRMON) == 0 );

		/* I2C Initial setting */
		out_w(ba[unit] + I2C_SBIxCR1, I2C_SBIxCR1_INIT | ll_devinit[unit].sck);
		out_w(ba[unit] + I2C_SBIxCR2, I2C_SBIxCR2_STOP);
		out_w(ba[unit] + I2C_SBIxBR0, 0);

		EnableInt(INTNO_INTSBI(unit), ll_devinit[unit].intpri);
		break;

	case LLD_I2C_CLOSE:
		/* Disable I2C interrupt */
		DisableInt(INTNO_INTSBI(unit));
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
	const T_DINT	dint = {
		.intatr	= TA_HLNG,
		.inthdr	= i2c_inthdr,
	};
	ER	err;

	/* Interrupt handler definition */
	err = tk_def_int(INTNO_INTSBI(p_dcb->unit), &dint);

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* CPU_TMPM367FDFG */