// アドレス/サイズ情報
#define	IRAM_START_ADDR			0x80000000
#define	IRAM_SIZE				0x00400000
#define	IRAM_UNCACHED_SIZE		0x00200000

#define	IRAM_CACHED_START_ADDR		IRAM_START_ADDR
#define	IRAM_CACHED_SIZE		(IRAM_SIZE - IRAM_UNCACHED_SIZE)
#define	IRAM_CACHED_END_ADDR		(IRAM_CACHED_START_ADDR + IRAM_CACHED_SIZE)
#define	IRAM_UNCACHED_START_ADDR	IRAM_CACHED_END_ADDR
#define	IRAM_UNCACHED_END_ADDR		(IRAM_UNCACHED_START_ADDR + IRAM_UNCACHED_SIZE)

#if 0 // DRAM not on board
#define	DRAM_START_ADDR			0x08000000
#define	DRAM_SIZE			0x08000000
#define	DRAM_UNCACHED_SIZE		0x01000000

#define	DRAM_CACHED_START_ADDR		DRAM_START_ADDR
#define	DRAM_CACHED_SIZE		(DRAM_SIZE - DRAM_UNCACHED_SIZE)
#define	DRAM_CACHED_END_ADDR		(DRAM_CACHED_START_ADDR + DRAM_CACHED_SIZE)
#define	DRAM_UNCACHED_START_ADDR	DRAM_CACHED_END_ADDR
#define	DRAM_UNCACHED_END_ADDR		(DRAM_UNCACHED_START_ADDR + DRAM_UNCACHED_SIZE)
#endif /* 0 */

#define	PAGESIZE_SECTION		0x00100000	/* 1MB size */
#define	ADDRMASK_SECTION		0xfff00000	/* Bit Mask for 1MB page address. */

                                                           // discriptor format... see ARMARM P.B3-1326
                                                           // ----_----_----_*0--_----_----_----_--1- NS
                                                           // ----_----_----_-0*-_----_----_----_--1- nG
                                                           // ----_----_----_-0-*_----_----_----_--1- S
                                                           // ----_----_----_-0--_*---_**--_----_--1- AP
                                                           // ----_----_----_-0--_-***_----_----_--1- TEX
                                                           // ----_----_----_-0--_----_---*_***-_--1- DOMAIN
                                                           // ----_----_----_-0--_----_----_---*_--1- XN
                                                           // ----_----_----_-0--_----_----_----_*-1- C
                                                           // ----_----_----_-0--_----_----_----_-*1- B
                                                           // ----_----_----_-0--_----_----_----_--1* PXN
#define TTB_PARA_STRGLY                            0x00DF2 // 0000_0000_0000_0000_0000_1101_1111_0010    Secure Strongly-ordered memory
#define TTB_PARA_STRGLY_NS                         0x80DF2 // 0000_0000_0000_1000_0000_1101_1111_0010    Non-secure Strongly-ordered memory
#define TTB_PARA_DEVICE                            0x00DF6 // 0000_0000_0000_0000_0000_1101_1111_0110    Secure Device memory
#define TTB_PARA_DEVICE_NS                         0x80DF6 // 0000_0000_0000_1000_0000_1101_1111_0110    Non-secure Device memory
#define TTB_PARA_NORMAL_NOT_CACHE                  0x04DE2 // 0000_0000_0000_0000_0100_1101_1110_0010    Outer and inner Non-cacheable, and Secure Normal memory
#define TTB_PARA_NORMAL_NOT_CACHE_NS               0x84DE2 // 0000_0000_0000_1000_0100_1101_1110_0010    Outer and inner Non-cacheable, and Non-secure Normal memory
#define TTB_PARA_NORMAL_CACHE                      0x01DEE // 0000_0000_0000_0000_0001_1101_1110_1110    Outer and inner Write-Back Write-Allocate Cacheable, and Secure Normal memory
#define TTB_PARA_NORMAL_CACHE_NS                   0x81DEE // 0000_0000_0000_1000_0001_1101_1110_1110    Outer and inner Write-Back Write-Allocate Cacheable, and Non-secure Normal memory
#define TTB_PARA_NORMAL_L1_CACHE_L2_NOT_CACHE      0x04DE6 // 0000_0000_0000_0000_0100_1101_1110_0110    Outer Non-cacheable, Inner Write-Back Write-Allocate Cacheable, and Secure Normal memory
#define TTB_PARA_NORMAL_L1_CACHE_L2_NOT_CACHE_NS   0x84DE6 // 0000_0000_0000_1000_0100_1101_1110_0110    Outer Non-cacheable, Inner Write-Back Write-Allocate Cacheable, and Non-secure Normal memory

    .section    INIT_TTB, "ax"
    .arm
/*===================================================================
   MMU設定情報
=================================================================== */
mmu_setting_info:
    //     物理/論理アドレス,        サイズ,             Attribute,                 padding
#if 0 // NOR flash / DRAM not on board
    .word  0x00000000,               0x08000000,         TTB_PARA_NORMAL_CACHE,     0x00000000    // CS0-CS1 area (NOR-Flash) 0x00000000 - 0x07ffffff
    .word  DRAM_CACHED_START_ADDR,   DRAM_CACHED_SIZE,   TTB_PARA_NORMAL_CACHE,     0x00000000    // CS2-CS3 area (SDRAM CACHED)
    .word  DRAM_UNCACHED_START_ADDR, DRAM_UNCACHED_SIZE, TTB_PARA_NORMAL_NOT_CACHE, 0x00000000    // CS2-CS3 area (SDRAM UNCACHED)
                                                                                                  // CS4 area (None) 0x10000000 - 0x13ffffff
                                                                                                  // CS5 area (None) 0x14000000 - 0x17ffffff
#endif /* 0 */
	.word  0x1f000000,               0x00100000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0x1f000000 - 0x1f0fffff
	.word  0x1f400000,               0x00100000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0x1f400000 - 0x1f4fffff
	.word  0x1f800000,               0x00100000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0x1f800000 - 0x1f8ffff
	.word  0x20000000,               0x02000000,         TTB_PARA_NORMAL_CACHE,     0x00000000    // SPI multi I/O bus area (Serial flash) 0x20000000 - 0x2fffffff  32MB on board
	.word  0x30000000,               0x00800000,         TTB_PARA_NORMAL_CACHE,     0x00000000    // Hyper Flash area  0x30000000 - 0x3fffffff  8MB on board
	.word  0x40000000,               0x00800000,         TTB_PARA_NORMAL_CACHE,     0x00000000    // Hyper RAM area    0x40000000 - 0x4fffffff  8MB on board
//	.word  0x50000000,               0x00800000,         TTB_PARA_NORMAL_CACHE,     0x00000000    // Octa Flash area   0x50000000 - 0x5fffffff  8MB on board
//	.word  0x60000000,               0x00800000,         TTB_PARA_NORMAL_CACHE,     0x00000000    // Octa RAM area     0x60000000 - 0x6fffffff  8MB on board

	.word  IRAM_CACHED_START_ADDR,   IRAM_CACHED_SIZE,   TTB_PARA_NORMAL_CACHE,     0x00000000    // Internal RAM (CACHED) area
	.word  IRAM_UNCACHED_START_ADDR, IRAM_UNCACHED_SIZE, TTB_PARA_NORMAL_NOT_CACHE, 0x00000000    // Internal RAM (UBCACHED) area
	.word  0xe8000000,               0x00300000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0xe8000000 - 0xe82fffff
	.word  0xea000000,               0x01000000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0xea000000 - 0xea0fffff
	.word  0xf0000000,               0x00100000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0xf0000000 - 0xf00fffff
	.word  0xfc000000,               0x00100000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0xfc000000 - 0xfc0fffff
	.word  0xfcf00000,               0x00100000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0xfcf00000 - 0xfcffffff
	.word  0xfff00000,               0x00100000,         TTB_PARA_STRGLY,           0x00000000    // Internal I/O area 0xfff00000 - 0xffffffff
	.word  0x00000000,               0x00000000,         0x00000000,                0x00000000    // terminate

/*===================================================================
   translation table base(TTB)の設定
=================================================================== */
    .global     knl_init_ttb
knl_init_ttb:
    MOV  r0, #0x0
    MCR  p15, 0, r0, c2, c0, 2      // TTBCR

    // TTBレジスタ0 に ページテーブルのベースアドレスを設定
    LDR  r0, =__ttb_area_top
    MOV  r1, #0x08                  // RGN=b01  (outer cacheable write-back cached, write allocate)
                                    // S=0      (translation table walk to non-shared memory)
    ORR  r1, r1, #0x40              // IRGN=b01 (inner cacheability for the translation table walk is Write-back Write-allocate)
    ORR  r0, r0, r1
    MCR  p15, 0, r0, c2, c0, 0      // Write CP15 Translation Table Base Register 0,(TTBR0)

    //  戻りアドレスの退避
    mov  r11, lr

    // ディスクリプタ領域のクリア
    LDR  r0, =__ttb_area_top
    LDR  r1, =__ttb_area_bottom

    MOV  r2, #0
    MOV  r3, #0
    MOV  r4, #0
    MOV  r5, #0
    MOV  r6, #0
    MOV  r7, #0
    MOV  r8, #0
    MOV  r9, #0

MMUDESC_clr_loop:
    STMIA r0!, {r2-r9}					// ZERO clear
    CMP   r0, r1
    BLO   MMUDESC_clr_loop


    // ディスクリプタ領域のセット
    LDR  r0, =__ttb_area_top  // TTB領域の先頭アドレス
    LDR  r9, =mmu_setting_info      //  設定情報テーブル
MMUDESC_set_loop:
    LDR  r1, [r9, #0x00]       // address
    LDR  r2, [r9, #0x04]       // size
    LDR  r3, [r9, #0x08]       // attribute
    cmp  r2, #0                     // if size == 0
    BXEQ r11                        //   then return
    ADD  r9, #0x10                  // update pointer
    BL   Set_SECDESC
    B    MMUDESC_set_loop

/* *******************************************************************
    MMU ディスクリプタの設定
    物理アドレス＝論理アドレスとしてMMUディスクリプタを設定する
    セクションとして指定するので、アドレス/サイズは1MByte単位
    r0 : TTB領域の先頭アドレス
    r1 : 開始アドレス
    r2 : サイズ
    r3 : Attribute
    保持レジスタ：r0
    破壊レジスタ：r1-r6
******************************************************************* */
Set_SECDESC:		//FUNCTION

    CMP  r2, #0                     // サイズが0なら終了
    BXEQ lr

    LDR  r5, =ADDRMASK_SECTION      // アドレスマスク
    AND  r1, r1, r5
    ADD  r4, r0, r1, lsr #(20-2)    // r4:ディスクリプタのアドレス(base+論理アドレス/1M*4)

loop_SECDESC:
    ORR  r6, r1, r3                 // アドレスとAttributeからDescriptor設定値を生成
    STR  r6, [r4], #4               // Descriptor 設定値書き込み(ポインタ更新付き)

    ADD  r1, r1, #PAGESIZE_SECTION  // アドレス更新
    SUBS r2, r2, #PAGESIZE_SECTION  // 残りサイズ更新
    BHI  loop_SECDESC               // サイズが0より大きければループ

Done_SECDESC:
    BX   lr                         // 終了

    .END

