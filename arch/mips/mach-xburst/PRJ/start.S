// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <config.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>
#include <asm/cache.h>
#include <asm/arch/base.h>

#define RESERVED_FOR_SC(x) .space 1536, x

#ifdef CONFIG_SPL_MMC_SUPPORT
.macro SETUP_SPL_MMC_HEADER reg=0
	.word 0x4d53504c
	.space 508, 0
	RESERVED_FOR_SC(0)
.endm
#endif

#ifdef	CONFIG_SPL_SFC_NAND_SUPPORT
.macro SETUP_SPL_SFC_NAND_HEADER reg=0
	.word 0x03040506
	.word 0x55aa5502
#define FLASH_PPB (CFG_SPI_NAND_PPB / 32)
#define FLASH_BPP (CFG_SPI_NAND_BPP / 1024)
        .word (0x00000000 | (FLASH_PPB << 16) | (FLASH_BPP << 24))
        .word 0x00000000
        .space (512-16),0
	RESERVED_FOR_SC(0)
.endm
#endif

#ifdef CONFIG_SPL_SFC_NOR_SUPPORT
.macro SETUP_SPL_SFC_NOR_HEADER reg=0
	.word 0x03040506
	.word 0x55aa5502
	.word 0xffff00aa
    .word 0x00000000

	.space (512-16),0
	RESERVED_FOR_SC(0)
.endm
#endif

	.set noreorder
	.globl _start
	.text
_start:
	/* Setup spl header(params data) for different bootup media. */
#ifdef CONFIG_SPL_MMC_SUPPORT
	SETUP_SPL_MMC_HEADER()
#endif
#ifdef CONFIG_SPL_SFC_NOR_SUPPORT
	SETUP_SPL_SFC_NOR_HEADER()
#endif
#ifdef	CONFIG_SPL_SFC_NAND_SUPPORT
	SETUP_SPL_SFC_NAND_HEADER()
#endif


	/* === Real Executable Code Starts at 2048 offset, below === */

	/* Invalidate BTB */
	mfc0	v0, CP0_CONFIG, 7
	nop
	ori	v0, v0, 2
	mtc0	v0, CP0_CONFIG, 7
	nop

	/*
	 * CU0=UM=EXL=IE=0, BEV=ERL=1, IP2~7=1
	 */
	li	t0, 0x0040FC04
	mtc0	t0, CP0_STATUS

	/* CAUSE register */
	/* IV=1, use the specical interrupt vector (0x200) */
	li	t1, 0x00800000
	mtc0	t1, CP0_CAUSE

	li      t0, CONFIG_SOCID_ADDRESS
	lw      t1, 0(t0)

	li      t2, CONFIG_SOCID_PRJ008
	beq     t1, t2, init_gpio_PRJ008
	nop

	li      t3, CONFIG_SOCID_PRJ007
	beq     t1, t3, init_gpio_PRJ007
	nop

	j       init_gpio_end // 如果没有匹配则直接跳过init_gpio
	nop

init_gpio_PRJ008:
	/* Configuring the GPIO Schmitt Trigger Register */
	/* PA10,PA11~PA28 */
	li      t8, 0x1ffff400
	li      t9, 0xb0010170
	sw      t8, 4(t9)

	/* PB00~PB11,PB13~PB16,PB23~PB31 */
	li      t8, 0xff81efff
	li      t9, 0xb0011170
	sw      t8, 4(t9)

	/* PC00~PC09 */
	li      t8, 0x000003ff
	li      t9, 0xb0012170
	sw      t8, 4(t9)

	/* Configure GPIO drive strength */
	/* PA10,PA11~PA22 drive strength b'11 */
	li      t8, 0x007ff400
	li      t9, 0xb0010144
	sw      t8, 0(t9)
	li      t9, 0xb0010134
	sw      t8, 0(t9)
	/* PA23~PA28 drive strength b'01 */
	li      t8, 0x1f800000
	li      t9, 0xb0010148
	sw      t8, 0(t9)
	li      t9, 0xb0010134
	sw      t8, 0(t9)

	/* PB00~PB11,PB13~PB16,PB23~PB31 drive strength b'01 */
	li      t8, 0xff81efff
	li      t9, 0xb0011148
	sw      t8, 0(t9)
	li      t9, 0xb0011134
	sw      t8, 0(t9)

	/* PC00~PC09 drive strength b'01 */
	li      t8, 0x000003ff
	li      t9, 0xb0012148
	sw      t8, 0(t9)
	li      t9, 0xb0012134
	sw      t8, 0(t9)

	j       init_gpio_end
	nop

init_gpio_PRJ007:
	/* Configuring the GPIO Schmitt Trigger Register */
	/* PA10~PA28 Schmitt */
	la t8, 0x1ffffc00
	la t9, 0xb0010164
	sw t8, 0(t9)

	/* PB00~PB11 PB13~PB31 Schmitt */
	la t8, 0xffffefff
	la t9, 0xb0011164
	sw t8, 0(t9)

	/* PC00~PC09 PC11~PC29 Schmitt */
	la t8, 0x3ffffbff
	la t9, 0xb0012164
	sw t8, 0(t9)

	/* Configure GPIO drive strength */
	/* PA10~PA15 PA18~PA22 4mA */
	la t8, 0x55500000
	la t9, 0xb0010134
	sw t8, 0(t9)

	la t8, 0x00001550
	la t9, 0xb0010144
	sw t8, 0(t9)

	/* PA16~PA17 PA23~PA28 8mA */
	la t8, 0x02aa800a
	la t9, 0xb0010144
	sw t8, 0(t9)

	/* PB00~PB03 PB05~PB06 PB08~PB11 PB13~PB15 4mA */
	la t8, 0x54551455
	la t9, 0xb0011134
	sw t8, 0(t9)

	/* PB04 PB07 8mA */
	la t8, 0x00008200
	la t9, 0xb0011134
	sw t8, 0(t9)

	/* PB16~PB31 4mA */
	la t8, 0x55555555
	la t9, 0xb0011144
	sw t8, 0(t9)

	/*PC00~PC01 PC03~PC09 PC12~PC15 4mA */
	la t8, 0x55055545
	la t9, 0xb0012134
	sw t8, 0(t9)

	/*PC02 PC11 8mA */
	la t8, 0x00020020
	la t9, 0xb0012134
	sw t8, 0(t9)

	/*PC16~PC29 4mA */
	la t8, 0x05555555
	la t9, 0xb0012144
	sw t8, 0(t9)

	j       init_gpio_end
	nop

init_gpio_end:

#ifdef CONFIG_PALLADIUM
	.set	mips32
init_caches:
	la	$25, 0x80000000
        li      $2, 3                   // cacheable for kseg0 access
        mtc0    $2, $16                 // CP0_CONFIG
        nop

#if 1
//	li      $2, 0x20000000          // enable idx-store-data cache insn
//	mtc0    $2, $26                 // CP0_ERRCTL
#else
	mtc0    $0, $26                 // CP0_ERRCTL
#endif
	mfc0    $3, $4,7		//enable rdhwr
	ori     $3, $3,1
	mtc0    $3, $4,7

        ori     $2, $25, 0              // start address
        ori     $3, $2, 0x7fe0          // end address, total 32KB
        mtc0    $0, $28, 0              // CP0_TAGLO
        mtc0    $0, $28, 1              // CP0_DATALO
cache_clear_a_line:
        cache   0x8, 0($2)              // Index_Store_Tag_I
        cache   0x9, 0($2)              // Index_Store_Tag_D
        bne     $2, $3, cache_clear_a_line
        addiu   $2, $2, 32              // increment CACHE_LINE_SIZE
        ori     $2, $25, 0              // start address
        ori     $3, $2, 0x7fe0          // end address, total 32KB spl stack space
        la      $4, 0x1ffff000          // physical address and 4KB page mask
cache_alloc_a_line:
        and     $5, $2, $4
        ori     $5, $5, 1               // V bit of the physical tag
        mtc0    $5, $28, 0              // CP0_TAGLO
        cache   0x8, 0($2)              // Index_Store_Tag_I
        cache   0x9, 0($2)              // Index_Store_Tag_D
        bne     $2, $3, cache_alloc_a_line
        addiu   $2, $2, 32              // increment CACHE_LINE_SIZE

	li	sp, 0x80008000
#endif

	/* Set up stack */
#ifdef CONFIG_SPL_STACK
	li	sp, CONFIG_SPL_STACK
#endif
#if 0
#ifdef CONFIG_SPL_NOR_SUPPORT
	.extern __data_start
	.extern __data_end
	/*Init data section, nor spl*/
	la	t0, __data_start
	la	t1, __data_end
	la	t2, 0x80000000
1:
	lw	t3, 0(t0)
	addiu	t0, t0, 4
	sw	t3, 0(t2)
	addiu	t2, t2, 4
	bne	t0, t1, 1b
	nop
#endif
#endif

	b	board_init_f
	nop
