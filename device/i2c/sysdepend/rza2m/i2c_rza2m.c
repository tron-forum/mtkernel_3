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
#ifdef CPU_RZA2M

#include <tk/tkernel.h>
#include "../../i2c.h"
#include "../../../include/dev_def.h"
#if DEV_IIC_ENABLE
/*
 *	i2c_rza2m.c
  *	I2C device driver
 *	System-dependent definition for RZ/A2M
 */

/*----------------------------------------------------------------------
 * Device register base address
*/
const LOCAL UW ba[DEV_I2C_UNITNM] = { RIIC0_BASE, RIIC1_BASE, RIIC2_BASE, RIIC3_BASE };

#define	RIIC_CR1(u)	(ba[u] + RIICx_CR1)
#define	RIIC_CR2(u)	(ba[u] + RIICx_CR2)
#define RIIC_MR1(u)	(ba[u] + RIICx_MR1)
#define RIIC_MR2(u)	(ba[u] + RIICx_MR2)
#define RIIC_MR3(u)	(ba[u] + RIICx_MR3)
#define	RIIC_FER(u)	(ba[u] + RIICx_FER)
#define	RIIC_SER(u)	(ba[u] + RIICx_SER)
#define	RIIC_IER(u)	(ba[u] + RIICx_IER)
#define	RIIC_SR1(u)	(ba[u] + RIICx_SR1)
#define	RIIC_SR2(u)	(ba[u] + RIICx_SR2)
#define	RIIC_SAR0(u)	(ba[u] + RIICx_SAR0)
#define	RIIC_SAR1(u)	(ba[u] + RIICx_SAR1)
#define	RIIC_SAR2(u)	(ba[u] + RIICx_SAR2)
#define	RIIC_BRL(u)	(ba[u] + RIICx_BRL)
#define	RIIC_BRH(u)	(ba[u] + RIICx_BRH)
#define	RIIC_DRT(u)	(ba[u] + RIICx_DRT)
#define	RIIC_DRR(u)	(ba[u] + RIICx_DRR)

/*----------------------------------------------------------------------
 * Device data
*/
const LOCAL struct {
	UINT	intno;		// Interrupt number
	PRI	intpri;		// Interrupt priority
	UW	cks;		// Frequency selection
	UW	br;		//  Clock width
} ll_devdat[DEV_I2C_UNITNM] = {
	{.intno = INTNO_RIIC_TEI0, .intpri = DEVCNF_I2C0_INTPRI, .cks =DEVCNF_I2C0_CKS, .br = DEVCNF_I2C0_BR},
	{.intno = INTNO_RIIC_TEI1, .intpri = DEVCNF_I2C1_INTPRI, .cks =DEVCNF_I2C1_CKS, .br = DEVCNF_I2C1_BR},
	{.intno = INTNO_RIIC_TEI2, .intpri = DEVCNF_I2C2_INTPRI, .cks =DEVCNF_I2C2_CKS, .br = DEVCNF_I2C2_BR},
	{.intno = INTNO_RIIC_TEI3, .intpri = DEVCNF_I2C3_INTPRI, .cks =DEVCNF_I2C3_CKS, .br = DEVCNF_I2C3_BR},
};

#define	INTNO_TEI(n)	(ll_devdat[n].intno)
#define	INTNO_RI(n)	(ll_devdat[n].intno+1)
#define	INTNO_TI(n)	(ll_devdat[n].intno+2)
#define	INTNO_SPI(n)	(ll_devdat[n].intno+3)
#define	INTNO_STI(n)	(ll_devdat[n].intno+4)
#define	INTNO_NAKI(n)	(ll_devdat[n].intno+5)
#define	INTNO_ALI(n)	(ll_devdat[n].intno+6)
#define	INTNO_TMOI(n)	(ll_devdat[n].intno+7)

#define	INT_PRI(n)	(ll_devdat[n].intpri)

#define I2C_CKS(n)	(ll_devdat[n].cks)
#define	I2C_BRH(n)	(ll_devdat[n].br>>8 & 0x1F)
#define	I2C_BRL(n)	(ll_devdat[n].br & 0x1F)

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

T_I2C_LLDCB	ll_devcb[DEV_I2C_UNITNM];

/*-------------------------------------------------------
 * Interrupt handler
 *-------------------------------------------------------
 */
/*
 * TI: Transmit data empty interrupt
 */
LOCAL void i2c_ti_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;

	ClearInt(intno);	// Clear interrupt
	for( unit = 0; unit < DEV_I2C_UNITNM; unit++) {
		if( intno == INTNO_TI(unit)) break;
	}
	if(unit < DEV_I2C_UNITNM) {
		p_cb = &ll_devcb[unit];
	} else {
		EndOfInt(intno);
		return;
	}

	switch(p_cb->state) {
	case I2C_STS_START:
		if(p_cb->sdat_num > 0 ) {	/* Send */
			out_w(RIIC_DRT(unit), p_cb->sadr);
			p_cb->state = I2C_STS_SEND;
		} else {			/* Receive */
			out_w(RIIC_DRT(unit), (p_cb->sadr|1));
			out_w(RIIC_IER(unit), (in_w(RIIC_IER(unit)) | RIIC_IER_RIE) & ~RIIC_IER_TIE);
			p_cb->state = I2C_STS_RECV | I2C_STS_TOP;
		}
		break;

	case I2C_STS_SEND:
		if(--(p_cb->sdat_num) <= 0) {
			if(p_cb->rdat_num == 0) {
				p_cb->state = I2C_STS_STOP;
			} else {
				p_cb->state = I2C_STS_RESTART;
			}
			out_w(RIIC_IER(unit), (in_w(RIIC_IER(unit)) | RIIC_IER_TEIE) & ~RIIC_IER_TIE);
		}
		out_w(RIIC_DRT(unit), *(p_cb->sbuf)++);
		break;
	}
	EndOfInt(intno);
}

/*
 * TEI: Transmission end interrupt
 */
LOCAL void i2c_tei_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;

	ClearInt(intno);	// Clear interrupt
	for( unit = 0; unit < DEV_I2C_UNITNM; unit++) {
		if( intno == INTNO_TEI(unit)) break;
	}
	if(unit < DEV_I2C_UNITNM) {
		p_cb = &ll_devcb[unit];
	} else {
		EndOfInt(intno);
		return;
	}

	*(_UB*)RIIC_SR2(unit) &= ~RIIC_SR2_TEND;

	switch(p_cb->state) {
	case I2C_STS_STOP:
		out_w(RIIC_IER(unit), (in_w(RIIC_IER(unit)) & ~RIIC_IER_TEIE) |RIIC_IER_SPIE);
		*(_UW*)RIIC_CR2(unit) |= RIIC_CR2_SP;	// Stop condition
		break;
	
	case I2C_STS_RESTART:
		out_w( RIIC_IER(unit) , RIIC_IER_TIE | RIIC_IER_NAKIE);	/* Enable TXI & NAKI */
		*(_UW*)RIIC_CR2(unit) |= RIIC_CR2_RS;	// Rester condition
		p_cb->state = I2C_STS_START;
		break;
	}
	EndOfInt(intno);
}

/*
 * RI: Received data full interrupt
 */
LOCAL void i2c_ri_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;

	ClearInt(intno);	// Clear interrupt
	for( unit = 0; unit < DEV_I2C_UNITNM; unit++) {
		if( intno == INTNO_RI(unit)) break;
	}
	if(unit < DEV_I2C_UNITNM) {
		p_cb = &ll_devcb[unit];
	} else {
		EndOfInt(intno);
		return;
	}

	switch(p_cb ->state) {
	case I2C_STS_RECV | I2C_STS_TOP:
		if(p_cb->rdat_num <= 2) {	// Last data -2
			*(_UW*)RIIC_MR3(unit) |= RIIC_MR3_WAIT;
		}
		if(p_cb->rdat_num == 1) {	//Next is the final data
			*(_UW*)RIIC_MR3(unit) |= RIIC_MR3_ACKBT;
			p_cb->state = I2C_STS_RECV | I2C_STS_LAST;
		} else {
			p_cb->state = I2C_STS_RECV;
		}
		in_w(RIIC_DRR(unit));	// Dummy read
		break;

	case I2C_STS_RECV:
		if(p_cb->rdat_num == 3) {	// Last data -2
			*(_UW*)RIIC_MR3(unit) |= RIIC_MR3_WAIT;
		}
		if(p_cb->rdat_num == 2) {	//Next is the final data
			*(_UW*)RIIC_MR3(unit) |= RIIC_MR3_ACKBT;
			p_cb->state = I2C_STS_RECV | I2C_STS_LAST;
		}
		*(p_cb->rbuf++) = in_w(RIIC_DRR(unit));
		p_cb->rdat_num--;
		break;

	case I2C_STS_RECV | I2C_STS_LAST:
		*(_UW*)RIIC_SR2(unit) &= ~RIIC_SR2_STOP;
		out_w(RIIC_IER(unit), (in_w(RIIC_IER(unit)) | RIIC_IER_SPIE) & ~RIIC_IER_RIE);
		*(_UW*)RIIC_CR2(unit) |= RIIC_CR2_SP;
		*(p_cb->rbuf++) = in_w(RIIC_DRR(unit));
		*(_UW*)RIIC_MR3(unit) &= ~RIIC_MR3_WAIT;
		p_cb->state = I2C_STS_STOP;
		break;
	}
	EndOfInt(intno);
}

LOCAL void i2c_spi_inthdr( UINT intno )
{
	T_I2C_LLDCB	*p_cb;
	INT		unit;

	ClearInt(intno);	// Clear interrupt
	for( unit = 0; unit < DEV_I2C_UNITNM; unit++) {
		if( intno == INTNO_SPI(unit)) break;
	}
	if(unit < DEV_I2C_UNITNM) {
		p_cb = &ll_devcb[unit];
	} else {
		EndOfInt(intno);
		return;
	}

	*(_UW*)RIIC_SR2(unit) &= ~RIIC_SR2_STOP;

	if (p_cb->state != I2C_STS_STOP) {
		p_cb->ioerr = E_IO;
	}
	out_w( RIIC_IER(unit), 0);
	if(p_cb->wait_tskid) {
		tk_wup_tsk(p_cb->wait_tskid);
		p_cb->wait_tskid = 0;
	}
	EndOfInt(intno);
}

LOCAL void i2c_err_inthdr( UINT intno )
{
	INT		unit;

	ClearInt(intno);	// Clear interrupt
	if(intno < INTNO_RIIC_STI0 || intno > INTNO_RIIC_TMOI3) {
		EndOfInt(intno);
		return;
	}

	if(intno <= INTNO_RIIC_TMOI0 ) {
		unit = DEV_I2C_0;
	} else if(intno <= INTNO_RIIC_TMOI1 ) {
		unit = DEV_I2C_1;
	} else if(intno <= INTNO_RIIC_TMOI2 ) {
		unit = DEV_I2C_2;
	} else {
		unit = DEV_I2C_3;
	}

	ll_devcb[unit].ioerr = E_IO;
	ll_devcb[unit].state = I2C_STS_STOP;
	out_w( RIIC_SR2(unit) ,0);
	out_w( RIIC_IER(unit), RIIC_IER_SPIE);
	*(_UW*)RIIC_CR2(unit) |= RIIC_CR2_SP;	/* Stop condition */
	EndOfInt(intno);
}


/*----------------------------------------------------------------------
 * Execution of communication
 */
LOCAL ER i2c_trans( UW unit)
{
	ER		err;

	if( in_w(RIIC_CR2(unit)) & RIIC_CR2_BBSY) {
		ll_devcb[unit].ioerr = E_BUSY;		/* I2C bus busy */
		return E_BUSY;
	}

	ll_devcb[unit].ioerr		= E_OK;
	ll_devcb[unit].wait_tskid 	= tk_get_tid();

	out_w(RIIC_IER(unit),(RIIC_IER_TIE|RIIC_IER_NAKIE|RIIC_IER_TMOIE));	/* Enable TI,NAKI  */
	*(_UW*)RIIC_CR2(unit) |= RIIC_CR2_ST;			/* start condition */

	err = tk_slp_tsk(DEVCNF_I2C_TMO);
	if ( ll_devcb[unit].ioerr != E_OK ) {
		err = ll_devcb[unit].ioerr;
	}

	out_w( RIIC_IER(unit), 0);
	out_w( RIIC_SR2(unit), 0);

	return err;
}

/*----------------------------------------------------------------------
 * Set communication start parameters
 */
Inline void set_com_start(UW unit, UW sadr, UW sdat_num, UW rdat_num, UB *sbuf, UB *rbuf)
{
	ll_devcb[unit].state		= I2C_STS_START;
	ll_devcb[unit].sadr		= sadr<<1;	/* Slave address */
	ll_devcb[unit].sdat_num	= 	sdat_num;	/* Number of send data */
	ll_devcb[unit].rdat_num	= 	rdat_num;	/* Number of receive data */
	ll_devcb[unit].sbuf		= sbuf;		/* Send buffer */
	ll_devcb[unit].rbuf		= rbuf;		/* Receive buffer */
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
		out_w( RIIC_CR1(unit), RIIC_CR1_RST | RIIC_CR1_SOWP);
		out_w( RIIC_CR1(unit), RIIC_CR1_RST| RIIC_CR1_ICE | RIIC_CR1_SOWP);

		/* I2C Initial setting */
		out_w( RIIC_SER(unit), 0);
		out_w( RIIC_MR1(unit), I2C_CKS(unit)<<4 );
		out_w( RIIC_BRH(unit), I2C_BRH(unit));
		out_w( RIIC_BRL(unit), I2C_BRL(unit));
		*(_UW*)RIIC_FER(unit) |= RIIC_FER_TMOE;

		/* Interrupt initialize */
		out_w( RIIC_IER(unit), 0);
		out_w( RIIC_SR2(unit), 0);
		EnableInt( INTNO_TEI(unit), INT_PRI(unit));
		EnableInt( INTNO_RI(unit), INT_PRI(unit));
		EnableInt( INTNO_TI(unit), INT_PRI(unit));
		EnableInt( INTNO_SPI(unit), INT_PRI(unit));
		EnableInt( INTNO_STI(unit), INT_PRI(unit));
		EnableInt( INTNO_NAKI(unit), INT_PRI(unit));
		EnableInt( INTNO_ALI(unit), INT_PRI(unit));
		EnableInt( INTNO_TMOI(unit), INT_PRI(unit));

		/* Release reset */
		out_w( RIIC_CR1(unit), RIIC_CR1_ICE | RIIC_CR1_SOWP);
		break;

	case LLD_I2C_CLOSE:
		/* Disable I2C interrupt */
		DisableInt( INTNO_TEI(unit));
		DisableInt( INTNO_RI(unit));
		DisableInt( INTNO_TI(unit));
		DisableInt( INTNO_SPI(unit));
		DisableInt( INTNO_STI(unit));
		DisableInt( INTNO_NAKI(unit));
		DisableInt( INTNO_ALI(unit));
		DisableInt( INTNO_TMOI(unit));
		break;

	case LLD_I2C_READ:
		set_com_start( unit, p1, 0, p2, NULL, (UB*)pp);
		err = i2c_trans(unit);
		if(err >= E_OK) err = p2 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_WRITE:
		set_com_start( unit, p1, p2, 0, (UB*)pp, NULL);
		err = i2c_trans(unit);
		if(err >= E_OK) err = p2 - ll_devcb[unit].sdat_num;
		break;

	case LLD_I2C_EXEC:
		p_ex = (T_I2C_EXEC*)pp;
		set_com_start( unit, (p_ex->sadr), p_ex->snd_size, p_ex->rcv_size, p_ex->snd_data, p_ex->rcv_data);
		err = i2c_trans(unit);
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
	ER	err;

	unit = p_dcb->unit;

#if DEVCONF_I2C_INIT_MSTP		// Initialize module stop
	_UB	dmy;

	out_b(CPG_STBCR8, in_b(CPG_STBCR8) & ~(0x80>>unit));
	dmy = in_b(CPG_STBCR8);	// dummy read

#endif	/* DEVCONF_I2C_INIT_MSTP */

	*(_UW*)RIIC_CR1(unit) &= ~RIIC_CR1_ICE;		// Disable RIIC

	/* Interrupt handler definition */
	dint.intatr	= TA_HLNG;

	dint.inthdr	= i2c_ri_inthdr;
	err = tk_def_int(INTNO_RI(unit), &dint);

	dint.inthdr	= i2c_ti_inthdr;
	err = tk_def_int(INTNO_TI(unit), &dint);

	dint.inthdr	= i2c_tei_inthdr;
	err = tk_def_int(INTNO_TEI(unit), &dint);

	dint.inthdr	= i2c_spi_inthdr;
	err = tk_def_int(INTNO_SPI(unit), &dint);

	dint.inthdr	= i2c_err_inthdr;
	err = tk_def_int(INTNO_STI(unit), &dint);
	err = tk_def_int(INTNO_NAKI(unit), &dint);
	err = tk_def_int(INTNO_ALI(unit), &dint);
	err = tk_def_int(INTNO_TMOI(unit), &dint);

	return err;
}

#endif		/* DEV_IIC_ENABLE */
#endif		/* CPU_RX231 */