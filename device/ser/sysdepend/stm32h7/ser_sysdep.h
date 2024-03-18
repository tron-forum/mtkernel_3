/*
 *----------------------------------------------------------------------
 *    Device Driver for μT-Kernel 3.0
 *
 *    Copyright (C) 2022 by Ken Sakamura.
 *    This software is distributed under the T-License 2.2.
 *----------------------------------------------------------------------
 *
 *    Released by TRON Forum(http://www.tron.org) at 2022/02.
 *
 *----------------------------------------------------------------------
 */

/*
 *	ser_sysdep.h
 *	Serial communication device driver
 *	System-dependent definition for STM32H7
 */

#ifndef	__DEV_SER_STM32H7_H__
#define	__DEV_SER_STM32H7_H__

#define	DEV_SER_UNITNM	(10)	/* Number of device channels */
#define DEV_SER_UNIT0	(0)	/* Ch.0 - USART1 */
#define DEV_SER_UNIT1	(1)	/* Ch.1 - USART2 */
#define DEV_SER_UNIT2	(2)	/* Ch.2 - USART3 */
#define DEV_SER_UNIT3	(3)	/* Ch.3 - UART4 */
#define DEV_SER_UNIT4	(4)	/* Ch.4 - UART5 */
#define DEV_SER_UNIT5	(5)	/* Ch.5 - USART6 */
#define DEV_SER_UNIT6	(6)	/* Ch.6 - UART7 */
#define DEV_SER_UNIT7	(7)	/* Ch.7 - UART8 */
#define DEV_SER_UNIT8	(8)	/* Ch.8 - UART9 */
#define DEV_SER_UNIT9	(9)	/* Ch.9 - USART10 */

/*
 * USART registers
 */

/* Base address for each channel */
#define	USART1_BASE	(0x40011000UL)
#define	USART2_BASE	(0x40004400UL)
#define	USART3_BASE	(0x40004800UL)
#define	UART4_BASE	(0x40004C00UL)
#define	UART5_BASE	(0x40005000UL)
#define	USART6_BASE	(0x40011400UL)
#define	UART7_BASE	(0x40007800UL)
#define	UART8_BASE	(0x40007C00UL)
#define	UART9_BASE	(0x40007800UL)
#define	USART10_BASE	(0x40011C00UL)

/* Register offset */
#define	USARTx_CR1	(0x0000)	/* Control register 1 */
#define	USARTx_CR2	(0x0004)	/* Control register 2 */
#define	USARTx_CR3	(0x0008)	/* Control register 3 */
#define	USARTx_BRR	(0x000C)	/* baud rate register */
#define	USARTx_GTPR	(0x0010)	/* Guard time and Priscaler register */
#define	USARTx_RTOR	(0x0014)	/* Receiver timeout register */
#define	USARTx_RQR	(0x0018)	/* Request register */
#define	USARTx_ISR	(0x001C)	/* Interrupts and status register */
#define	USARTx_ICR	(0x0020)	/* Interrupt flag clear register */
#define	USARTx_RDR	(0x0024)	/* Received data register */
#define	USARTx_TDR	(0x0028)	/* Transmission data register */
#define	USARTx_PRESC	(0x002C)	/* Prescaler register */

/* Register bit definition */
#define	USART_CR1_UE		(1<<0)	/* USART enable */
#define	USART_CR1_UESM		(1<<1)	/* USART enable in STOP mode */
#define	USART_CR1_RE		(1<<2)	/* Receiver enable */
#define	USART_CR1_TE		(1<<3)	/* Transmitter enable */
#define	USART_CR1_IDLEIE	(1<<4)	/* IDLE interrupt enable */
#define	USART_CR1_RXFNEIE	(1<<5)	/* RXFIFO not empty interrupt enable */
#define	USART_CR1_TCIE		(1<<6)	/* TCOE interrupt enable */
#define	USART_CR1_TXFNFIE	(1<<7)	/* TXFIFO not full interrupt enable */
#define	USART_CR1_PEIE		(1<<8)	/* PE interrupt enable */
#define	USART_CR1_PS		(1<<9)	/* Parity selection */
#define	USART_CR1_PCE		(1<<10)	/* Parity control enable */
#define	USART_CR1_WAKE		(1<<11)	/* Receive wakeup method */
#define	USART_CR1_M0		(1<<12)	/* Word length 0 */
#define	USART_CR1_MME		(1<<13)	/* Mute mode enable */
#define	USART_CR1_CMIE		(1<<14)	/* Character match interrupt enable */
#define	USART_CR1_OVER8		(1<<15)	/* Oversampling mode */
#define	USART_CR1_RTOIE		(1<<26)	/* Receiver timeout interrupt enable */
#define	USART_CR1_EOBIE		(1<<27)	/* End of Block interrupt enabled */
#define	USART_CR1_M1		(1<<28)	/* Word length 1 */
#define	USART_CR1_FIFOEN	(1<<29)	/* FIFO mode enable */
#define	USART_CR1_TXFEIE	(1<<30)	/* TXFIFO empty interrupt enable */
#define	USART_CR1_RXFFIE	(1<<31)	/* RXFIFO Full interrupt enable */

#define	USART_CR2_STOP		(3<<12)	/* RW Stop-bit */

#define	USART_CR3_RTSE		(1<<8)	/* RTS enable */
#define	USART_CR3_CTSE		(1<<9)	/* CTS enable */

#define	USART_RQR_SBKRQ		(1<<1)	/* Break send request */

#define	USART_ISR_PE		(1<<0)	/* Parity error */
#define	USART_ISR_FE		(1<<1)	/* Framing error */
#define	USART_ISR_NF		(1<<2)	/* Start bit noise detection */
#define	USART_ISR_ORE		(1<<3)	/* Overrun error */
#define	USART_ISR_IDLE		(1<<4)	/* Idle line detection */
#define	USART_ISR_RXNE		(1<<5)	/* Received data register not empty */
#define	USART_ISR_TC		(1<<6)	/* Transmission completely */
#define	USART_ISR_TXE		(1<<7)	/* Transmission data register empty */
#define	USART_ISR_LBDF		(1<<8)	/* LIN break detection */
#define	USART_ISR_CTSIF		(1<<9)	/* CTS interrupt */
#define	USART_ISR_CTS		(1<<10)	/* CTS flag */
#define	USART_ISR_RTOF		(1<<11)	/* Receiver timeout */
#define	USART_ISR_EOBF		(1<<12)	/* End of block */
#define	USART_ISR_UDR		(1<<13)	/* SPI slave underrun error flag */
#define	USART_ISR_ABRE		(1<<14)	/* Automatic baud rate error */
#define	USART_ISR_ABRF		(1<<15)	/* Automatic baud rate flag */
#define	USART_ISR_BUSY		(1<<16)	/* Busy flag */
#define	USART_ISR_CMF		(1<<17)	/* Character match flag */
#define	USART_ISR_SBKF		(1<<18)	/* Break transmission flag */
#define	USART_ISR_RWU		(1<<19)	/* Receiver mute mode flag */
#define	USART_ISR_WUF		(1<<20)	/* Wakeup flag */
#define	USART_ISR_TEACK		(1<<21)	/* Transmission ACK flag */
#define	USART_ISR_REACK		(1<<22)	/* Receive ACK flag */
#define	USART_ISR_ERR		(USART_ISR_PE|USART_ISR_FE|USART_ISR_NF|USART_ISR_ORE)

#define	USART_ICR_PECF		(1<<0)	/* Parity error clear flag */
#define	USART_ICR_FECF		(1<<1)	/* Framing error clear flag */
#define	USART_ICR_NECF		(1<<2)	/* Noise detected clear flag */
#define	USART_ICR_ORECF		(1<<3)	/* Overrun error clear flag */
#define	USART_ICR_IDLECF	(1<<4)	/* Idle line detected clear flag */
#define	USART_ICR_TXFECF	(1<<5)	/* TXFIFO empty clear flag */
#define	USART_ICR_TCCF		(1<<6)	/* Transmission complete clear flag */
#define	USART_ICR_TCBGTCF	(1<<7)	/* Transmission complete before Guard time clear flag */
#define	USART_ICR_LBDCF		(1<<8)	/* LIN break detection clear flag */
#define	USART_ICR_CTSCF		(1<<9)	/* CTS clear flag */
#define	USART_ICR_RTOCF		(1<<11)	/* Receiver timeout clear flag */
#define	USART_ICR_EOBCF		(1<<12)	/* End of block clear flag */
#define	USART_ICR_UDRCF		(1<<13)	/* SPI slave underrun clear flag */
#define	USART_ICR_CMCF		(1<<17)	/* Character match clear flag */
#define	USART_ICR_WUCF		(1<<20)	/* Wakeup from low-power mode clear flag */
#define	USART_ICR_ALL		(0x00126BFF)

/*Initial register value when using debug */
#define USART_CR1_DEBUG		0x0000000D	/* USART enable, 8bit, Non parity */
#define USART_CR2_DEBUG		0		/* Stop bit 1 */
#define USART_CR3_DEBUG		0		/* No hard flow control */


/* USART interrupt number */
#define	INTNO_USART1		37
#define	INTNO_USART2		38
#define	INTNO_USART3		39
#define	INTNO_UART4		52
#define	INTNO_UART5		53
#define	INTNO_USART6		78
#define	INTNO_UART7		89
#define	INTNO_UART8		90
#define	INTNO_UART9		162
#define	INTNO_USART10		163

#endif		/* __DEV_SER_STM32H7_H__ */
