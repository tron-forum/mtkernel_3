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
#ifdef CPU_RX231

#include <tk/tkernel.h>
#include "../../i2c.h"
#include "../../../include/dev_def.h"
#if DEV_IIC_ENABLE
/*
 *	i2c_rx231.c
  *	I2C device driver
 *	System-dependent definition for RX231
 */

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

T_I2C_LLDCB	ll_devcb;

/*-------------------------------------------------------
 * Interrupt handler
 *-------------------------------------------------------
 */
/*
 * TXI: Transmit data empty interrupt
 */
LOCAL void i2c_txi_inthdr( UINT intno )
{
	UB	reg;

	switch(ll_devcb.state) {
	case I2C_STS_START:
		if(ll_devcb.sdat_num > 0 ) {	/* Send */
			out_b(RIIC_ICDRT, ll_devcb.sadr);
			ll_devcb.state = I2C_STS_SEND;
		} else {			/* Receive */
			out_b(RIIC_ICDRT, (ll_devcb.sadr|1));
			reg = in_b(RIIC_ICIER);
			out_b(RIIC_ICIER, (reg | RIIC_ICIER_RIE) & ~RIIC_ICIER_TIE);
			ll_devcb.state = I2C_STS_RECV | I2C_STS_TOP;
		}
		break;

	case I2C_STS_SEND:
		if(--(ll_devcb.sdat_num) <= 0) {
			if(ll_devcb.rdat_num == 0) {
				ll_devcb.state = I2C_STS_STOP;
			} else {
				ll_devcb.state = I2C_STS_RESTART;
			}
			reg = in_b(RIIC_ICIER);
			out_b(RIIC_ICIER, (reg | RIIC_ICIER_TEIE) & ~RIIC_ICIER_TIE);
		}
		out_b(RIIC_ICDRT, *(ll_devcb.sbuf)++);
		break;
	}
}

/*
 * TEI: Transmission end interrupt
 */
LOCAL void i2c_tei_inthdr( UINT intno )
{
	UB	reg;

	switch(ll_devcb.state) {
	case I2C_STS_STOP:
		reg = in_b(RIIC_ICIER);
		out_b(RIIC_ICIER, (reg & ~RIIC_ICIER_TEIE) |RIIC_ICIER_SPIE);
		*(UB*)RIIC_ICCR2 |= RIIC_IICR2_SP;	// Stop condition
		break;
	
	case I2C_STS_RESTART:
		out_b( RIIC_ICIER , RIIC_ICIER_TIE | RIIC_ICIER_NAKIE);	/* Enable TXI & NAKI */
		*(UB*)RIIC_ICCR2 |= RIIC_IICR2_RS;	// Rester condition
		ll_devcb.state = I2C_STS_START;
		break;
	}
}


LOCAL void i2c_rxi_inthdr( UINT intno )
{
	UB	reg;

	switch(ll_devcb.state) {
	case I2C_STS_RECV | I2C_STS_TOP:
		if(ll_devcb.rdat_num <= 2) {	// Last data -2
			*(UB*)RIIC_ICMR3 |= RIIC_ICMR3_WAIT;
		}
		if(ll_devcb.rdat_num == 1) {	//Next is the final data
			*(UB*)RIIC_ICMR3 |= RIIC_ICMR3_ACKBT;
			ll_devcb.state = I2C_STS_RECV | I2C_STS_LAST;
		} else {
			ll_devcb.state = I2C_STS_RECV;
		}
		in_b(RIIC_ICDRR);	// Dummy read
		break;

	case I2C_STS_RECV:
		if(ll_devcb.rdat_num == 3) {	// Last data -2
			*(UB*)RIIC_ICMR3 |= RIIC_ICMR3_WAIT;
		}
		if(ll_devcb.rdat_num == 2) {	//Next is the final data
			*(UB*)RIIC_ICMR3 |= RIIC_ICMR3_ACKBT;
			ll_devcb.state = I2C_STS_RECV | I2C_STS_LAST;
		}
		*ll_devcb.rbuf++ = in_b(RIIC_ICDRR);
		ll_devcb.rdat_num--;
		break;

	case I2C_STS_RECV | I2C_STS_LAST:
		*(UB*)RIIC_ICSR2 &= ~RIIC_ICSR2_STOP;
		reg = in_b(RIIC_ICIER);
		out_b(RIIC_ICIER, (reg | RIIC_ICIER_SPIE) & ~RIIC_ICIER_RIE);
		*(UB*)RIIC_ICCR2 |= RIIC_IICR2_SP;
		*ll_devcb.rbuf++ = in_b(RIIC_ICDRR);
		*(UB*)RIIC_ICMR3 &= ~RIIC_ICMR3_WAIT;
		ll_devcb.state = I2C_STS_STOP;
		break;
	}
}

LOCAL void i2c_eei_inthdr( UINT intno )
{
	UB	reg;

	reg = in_b(RIIC_ICSR2);

	if(reg & RIIC_ICSR2_STOP) {		/* Detects stop condition */
		if (ll_devcb.state != I2C_STS_STOP) {
			ll_devcb.ioerr = E_IO;
		}
		out_b( RIIC_ICIER, 0);
		if(ll_devcb.wait_tskid) {
			tk_wup_tsk(ll_devcb.wait_tskid);
			ll_devcb.wait_tskid = 0;
		}
	} else {
		ll_devcb.ioerr = E_IO;
		ll_devcb.state = I2C_STS_STOP;
		*(UB*)RIIC_ICSR2 &= ~RIIC_ICSR2_STOP;
		out_b( RIIC_ICIER, RIIC_ICIER_SPIE);
		*(UB*)RIIC_ICCR2 |= RIIC_IICR2_SP;	/* Stop condition */
	}
}


/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans( void)
{
	UB		reg;
	ER		err;

	if( in_b(RIIC_ICCR2) & RIIC_IICR2_BBSY) {
		ll_devcb.ioerr = E_BUSY;		/* I2C bus busy */
		return E_BUSY;
	}

	ll_devcb.ioerr		= E_OK;
	ll_devcb.wait_tskid 	= tk_get_tid();

	out_b( RIIC_ICIER , RIIC_ICIER_TIE | RIIC_ICIER_NAKIE);	/* Enable TXI & NAKI */
//	out_b( RIIC_ICIER , 0xFF);	/* Enable TXI & NAKI */
	*(UB*)RIIC_ICCR2 |= RIIC_IICR2_ST;		/* start condition */

	err = tk_slp_tsk(DEVCNF_I2C_TMO);
	if ( ll_devcb.ioerr != E_OK ) {
		err = ll_devcb.ioerr;
	}

	out_b( RIIC_ICIER, 0);
	reg = in_b(RIIC_ICSR2);
	out_b( RIIC_ICSR2, reg & ~RIIC_ICSR2_STOP & ~RIIC_ICSR2_NACKF);

	return err;
}

/*----------------------------------------------------------------------
 * Set communication start parameters
 */
Inline void set_com_start(UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
	ll_devcb.state		= I2C_STS_START;
	ll_devcb.sadr		= sadr<<1;	/* Slave address */
	ll_devcb.sdat_num	= sdat_num;	/* Number of send data */
	ll_devcb.rdat_num	= rdat_num;	/* Number of receive data */
	ll_devcb.sbuf		= sbuf;		/* Send buffer */
	ll_devcb.rbuf		= rbuf;		/* Receive buffer */
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
		out_b( RIIC_ICCR1, RIIC_ICCR1_IICRST | RIIC_ICCR1_SOWP);
		out_b( RIIC_ICCR1, RIIC_ICCR1_IICRST| RIIC_ICCR1_ICE | RIIC_ICCR1_SOWP);

		/* I2C Initial setting */
		out_b( RIIC_ICSER, 0);
		out_b( RIIC_ICMR1, DEVCNF_I2C_CKS<<4 );
		//out_b( RIIC_ICMR3, RIIC_ICMR3_ACKWP);
		out_b( RIIC_ICBRH, RIIC_ICBRH_INI);
		out_b( RIIC_ICBRL, RIIC_ICBRL_INI);

		/* Interrupt initialize */
		out_b( RIIC_ICIER, 0);
		out_b( RIIC_ICSR2, 0);
		EnableInt( INTNO_RIIC_EEI, DEVCNF_I2C_INTPRI);
		EnableInt( INTNO_RIIC_RXI, DEVCNF_I2C_INTPRI);
		EnableInt( INTNO_RIIC_TXI, DEVCNF_I2C_INTPRI);
		EnableInt( INTNO_RIIC_TEI, DEVCNF_I2C_INTPRI);

		/* Release reset */
		out_b( RIIC_ICCR1,  RIIC_ICCR1_ICE | RIIC_ICCR1_SOWP);
		break;

	case LLD_I2C_CLOSE:
		/* Disable I2C interrupt */
		DisableInt(INTNO_RIIC_EEI);
		DisableInt(INTNO_RIIC_RXI);
		DisableInt(INTNO_RIIC_TXI);
		DisableInt(INTNO_RIIC_TEI);
		break;

	case LLD_I2C_READ:
		set_com_start( p1, 0, p2, NULL, (UB*)pp);
		err = i2c_trans();
		if(err >= E_OK) err = p2 - ll_devcb.sdat_num;
		break;

	case LLD_I2C_WRITE:
		set_com_start( p1, p2, 0, (UB*)pp, NULL);
		err = i2c_trans();
		if(err >= E_OK) err = p2 - ll_devcb.sdat_num;
		break;

	case LLD_I2C_EXEC:
		p_ex = (T_I2C_EXEC*)pp;
		set_com_start( (p_ex->sadr), p_ex->snd_size, p_ex->rcv_size, p_ex->snd_data, p_ex->rcv_data);
		err = i2c_trans();
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
	ER	err;

#if DEVCONF_I2C_INIT_MSTP		// Initialize module stop
	UINT	sts;

	if(in_w(MSTPCRB) & (1<<21)) {
		DI(sts);
		out_h(SYSTEM_PRCR, 0xA502);		// Disable Register Protect
		*(UW*)(MSTPCRB) &= ~(1<<21);		// Release module stop
		out_h(SYSTEM_PRCR, 0xA500);		// Enable Register protect
		EI(sts);
	}
#endif	/* DEVCONF_I2C_INIT_MSTP */

	*(UB*)RIIC_ICCR1 &= ~RIIC_ICCR1_ICE;		// Disable RIIC

	/* Interrupt handler definition */
	dint.intatr	= TA_HLNG;
	dint.inthdr	= i2c_eei_inthdr;
	err = tk_def_int(INTNO_RIIC_EEI, &dint);

	dint.inthdr	= i2c_rxi_inthdr;
	err = tk_def_int(INTNO_RIIC_RXI, &dint);

	dint.inthdr	= i2c_txi_inthdr;
	err = tk_def_int(INTNO_RIIC_TXI, &dint);

	dint.inthdr	= i2c_tei_inthdr;
	err = tk_def_int(INTNO_RIIC_TEI, &dint);

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* CPU_RX231 */