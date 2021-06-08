
/*** temp ***/


#include <basic.h>
#include <tk/sysdef.h>
#include <sys/sysinfo.h>


/******************************************************************************
* 各モジュールへのクロック供給の設定
* Arguments    : なし
* Return Value : なし
* すべてのモジュールにクロックを供給するように設定してあるので
* 必要に応じて設定値を変更すること
******************************************************************************/
static void STB_Init(void)
{
	volatile unsigned char dummy_buf_8b;

	// ---- The clock of all modules is permitted. ---- 
	CPG.STBCR2.BYTE = 0x6A;			// Port level is keep in standby mode, [1], [1], [0], [1], [0], [1], CoreSight
	dummy_buf_8b = CPG.STBCR2.BYTE;		// (Dummy read)

	CPG.STBCR3.BYTE = 0x80;			// [1], OSTM0, OSTM1, OSTM3, MTU3, CAN-FD, ADC, GPT
	dummy_buf_8b = CPG.STBCR3.BYTE;		// (Dummy read)

	CPG.STBCR4.BYTE = 0x00;			// SCIF0, SCIF1, SCIF2, SCIF3, SCIF4, SCI0, SCI1,IrDA
	dummy_buf_8b = CPG.STBCR4.BYTE;		// (Dummy read)

	CPG.STBCR5.BYTE = 0x31;			// A/D, CEU, [1], [1], RTC0, RTC1, JCU,
	dummy_buf_8b = CPG.STBCR5.BYTE;		// (Dummy read)

	CPG.STBCR6.BYTE = 0x80;			// [1], VIN, ETHER0, ETHER1, EtherPTR, EtherM, USB0, USB1
	dummy_buf_8b = CPG.STBCR6.BYTE;		// (Dummy read)

	CPG.STBCR7.BYTE = 0x10;			// IMR-LS2, DAVE-2D, MIPI, [1], SSIF0, SSIF1, SSIF2, SSIF3
	dummy_buf_8b = CPG.STBCR7.BYTE;		// (Dummy read)

	CPG.STBCR8.BYTE = 0x05;			// IIC0, IIC1, IIC2, IIC3, SPIBSC, [1], VDC6, [1]
	dummy_buf_8b = CPG.STBCR8.BYTE;		// (Dummy read)

	CPG.STBCR9.BYTE = 0x11;			// RSPI0, RSPI1, RSPI2, [1], HYPER, OCTA, SPDIF, DRP
	dummy_buf_8b = CPG.STBCR9.BYTE;		 // (Dummy read)

	CPG.STBCR10.BYTE = 0x00;		// TSIP, [0], [0], NAND, SDHI00, SDHI01, SDHI10, SDHI11
	dummy_buf_8b = CPG.STBCR10.BYTE;		// (Dummy read)

	CPG.STBCR11.BYTE = 0x3F;		// POE3, POEG, [1], [1], [1], [1], [1] , [1]
	dummy_buf_8b = CPG.STBCR11.BYTE;		// (Dummy read)

	(void)dummy_buf_8b;			// 未使用ワーニング回避
}
