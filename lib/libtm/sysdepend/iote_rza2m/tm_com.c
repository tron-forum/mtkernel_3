/*
 *----------------------------------------------------------------------
 *    micro T-Kernel 3.00.07.B0
 *
 *    Copyright (C) 2006-2023 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2023/11.
 *
 *----------------------------------------------------------------------
 */

/*
 *    tm_com.c
 *    T-Monitor Communication low-level device driver (RZ/A2M IoT-Engine)
 */

#include <tk/tkernel.h>

#if USE_TMONITOR
#include "../../libtm.h"

#ifdef IOTE_RZA2M
#if TM_COM_SERIAL_DEV

/* SCIFA register definition (Use SCIFA4) */
#define	SCIFA_BASE	0xE8009000	/* SCIFA4 register base address */
#define SCIFA_SMR	(SCIFA_BASE + 0x0000)
#define SCIFA_BRR	(SCIFA_BASE + 0x0002)
#define SCIFA_SCR	(SCIFA_BASE + 0x0004)
#define SCIFA_FTDR	(SCIFA_BASE + 0x0006)
#define SCIFA_FSR	(SCIFA_BASE + 0x0008)
#define SCIFA_FRDR	(SCIFA_BASE + 0x000A)
#define SCIFA_FCR	(SCIFA_BASE + 0x000C)
#define SCIFA_FDR	(SCIFA_BASE + 0x000E)
#define SCIFA_SPTR	(SCIFA_BASE + 0x0010)
#define SCIFA_LSR	(SCIFA_BASE + 0x0012)
#define SCIFA_SEMR	(SCIFA_BASE + 0x0014)
#define SCIFA_FTCR	(SCIFA_BASE + 0x0016)

#define SCR_TE		0x0020
#define SCR_RE		0x0010

#define FSR_ER		0x0080
#define FSR_TEND	0x0040
#define FSR_TDFE	0x0020
#define FSR_BRK		0x0010
#define FSR_FER		0x0008
#define FSR_PER		0x0004
#define FSR_RDF		0x0002
#define FSR_DR		0x0001
#define FSR_ERROR	(FSR_ER|FSR_BRK|FSR_FER|FSR_PER)

#define FCR_TFRST	0x0004
#define FCR_RFRST	0x0002

#define LSR_ORER	0x0001


EXPORT	void	tm_snd_dat( const UB* buf, INT size )
{
	_UH	d;
	while(size--) {
//		while(!(in_h(SCIFA_FSR) & FSR_TDFE)) ;	/* Waiting for FIFO space */
		do {
			d = in_h(SCIFA_FSR);
		} while(!(d&FSR_TDFE));
		out_b(SCIFA_FTDR, *buf++);
		and_h(SCIFA_FSR, ~(FSR_TEND|FSR_TDFE));
	}
	while(!(in_h(SCIFA_FSR) & FSR_TEND));		/* Waiting for transmission completion */
}

EXPORT	void	tm_rcv_dat( UB* buf, INT size )
{
	while(size--) {
		while(!(in_h(SCIFA_FSR) & (FSR_RDF|FSR_DR))) {
			if((in_h(SCIFA_FSR) & FSR_ERROR) || (in_h(SCIFA_LSR) & LSR_ORER)) {
				/* Clear error */
				and_h(SCIFA_SCR, ~SCR_RE);	/* Stop receive */
				or_h(SCIFA_FCR, FCR_RFRST);	/* Reset FIFO */
				and_h(SCIFA_FCR, ~FCR_RFRST);
				and_h(SCIFA_FSR, ~FSR_ERROR);	/* Clear error bit */
				and_h(SCIFA_LSR, ~LSR_ORER);	/* Clear overrun error */
				or_h(SCIFA_SCR, SCR_RE);	/* Start receive */
			}
		}
		*buf++ = in_b(SCIFA_FRDR);		/* Read received data */
		and_h(SCIFA_FSR, ~(FSR_RDF|FSR_DR));	/* Clear RDF/DR */
	}
	return;
}

EXPORT	void	tm_com_init(void)
{
	out_h(SCIFA_SCR, 0);			/* Stop send/receive, Select internal clock */
	out_h(SCIFA_FCR, FCR_RFRST|FCR_TFRST);	/* Reset FIFO */

	and_h(SCIFA_FSR, ~(FSR_ER | FSR_BRK | FSR_DR));	/* Clear error bit */
	and_h(SCIFA_LSR, ~LSR_ORER);			/* Clear overrun error */

	/* Baud rate setting (115200bps) */
	/* P1 clock=66.67MHz CKS=0 SCBRR=17 Bit rate error=0.46% => Baud rate=115200bps */
	out_h(SCIFA_SMR, 0);			/* UART mode, DATA=8bit, Parity=NONE, STOP=1, CLOCK=P1φ/1 (66.67MHz) */
	out_b(SCIFA_SEMR, 0);			/* Baud rate generator = normal mode, Operates with a frequency 16 times the transfer rate as the basic clock */
	out_b(SCIFA_BRR, 17);			/* Dividend ratio */

	out_h(SCIFA_FCR, 0x00000030);		/* Release FIFO reset */
	or_h(SCIFA_SPTR, 0x00000003);		/* Break output settings */
	out_h(SCIFA_SCR, (SCR_TE|SCR_RE));	/* Enable send/receive */

	return;
}

#endif /* TM_COM_SERIAL_DEV */
#endif /* IOTE_RZA2M */
#endif /* USE_TMONITOR */