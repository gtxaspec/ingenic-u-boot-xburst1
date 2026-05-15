/*
 * Ingenic PRJ007 configuration
 *
 * Copyright (c) 2015 Ingenic Semiconductor Co.,Ltd
 * Author: Matthew <xu.guo@ingenic.com>
 * Based on: include/configs/urboard.h
 *           Written by Paul Burton <paul.burton@imgtec.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_PRJ_H__
#define __CONFIG_PRJ_H__

#define INGENIC_UBOOT_VERSIONS "H20260203a"

/**
 * Basic configuration(SOC, Cache, Stack, UART, DDR).
 */
#define CONFIG_MIPS32                           /* MIPS32 CPU core */
#define CONFIG_CPU_XBURST
#define CONFIG_SYS_LITTLE_ENDIAN
#define CONFIG_SOC_NAME             PRJ
#define CONFIG_K0BASE               0x80000000
#define CONFIG_L1CACHE_SIZE         32768       /* 32K dcache and icache */
#define CONFIG_L1CACHELINE_SIZE     32          /* 32bytes dcache and icache line size */
#define CONFIG_L1CACHE_SETS         128         /* 128 dcache and icache sets */
#define CONFIG_L1CACHE_WAYNUM       8           /* 8 way dcache and icache associativity */
#define CONFIG_L1CACHE_TAG_MASK     ((~(((CONFIG_L1CACHE_SIZE) / (CONFIG_L1CACHE_WAYNUM)) - 1)) & 0x7fffffff)
#define CONFIG_L2CACHE_SIZE         131072      /* 128K l2cache size */
#define CONFIG_L2CACHE_SETS         512         /* 512 l2cache sets */
#define CONFIG_L2CACHELINE_SIZE     32          /* 32bytes l2cache line size */
#define CONFIG_L2CACHE_WAYNUM       8           /* 8 way l2cache associativity */
#define CONFIG_STACK_SIZE           CONFIG_L1CACHE_SIZE
/* #define CONFIG_DDR_AUTO_SELF_REFRESH */
/*#define CONFIG_SPL_DDR_SOFT_TRAINING*/

/*aes secure boot parameters select*/
/*#define CONFIG_USE_AES_SECURE_BOOT*/

#ifdef CONFIG_USE_AES_SECURE_BOOT
#define CONFIG_AES_EFUSE_KEY_BIT	KEY_SIZE_256	//KEY_SIZE_128 or KEY_SIZE_256
#define CONFIG_AES_DEC_MODE			DEC_CBC			//DEC_CBC or DEC_ECB ,recommended to use DEC_CBC
#endif

/* PRJ008 */
#if defined(CONFIG_PRJ008N)
#define APLL_950M
#define DDR_900M

#elif defined(CONFIG_PRJ008L) || defined(CONFIG_PRJ008DL)
#define APLL_950M
#define DDR_650M

#elif defined(CONFIG_PRJ008ZN) || defined(CONFIG_PRJ008VN)
#ifdef CONFIG_HP
#define APLL_950M
#define DDR_900M
#else
#define APLL_891M
#define DDR_700M
#endif

#elif defined(CONFIG_PRJ008ZL) || defined(CONFIG_PRJ008VL)
#ifdef CONFIG_HP
#define APLL_950M
#define DDR_650M
#else
#define APLL_891M
#define DDR_550M
#endif

/* PRJ007 */
#elif defined(CONFIG_PRJ007NQ) || defined(CONFIG_PRJ007XQ)
#define APLL_1200M
#define DDR_900M

#elif defined(CONFIG_PRJ007ZN) || defined(CONFIG_PRJ007VN) || defined(CONFIG_PRJ007VNP) || defined(CONFIG_PRJ007ZX) || defined(CONFIG_PRJ007VX)
#define APLL_900M
#define DDR_700M

#elif defined(CONFIG_PRJ007ZL) || defined(CONFIG_PRJ007VL)
#define APLL_900M
#define DDR_600M

#elif defined(CONFIG_PRJ007LQ)
#if defined(CONFIG_HP)
#define APLL_1200M
#define DDR_650M
#else
#define APLL_900M
#define DDR_600M
#endif

#elif defined(CONFIG_PRJ007A)
#define APLL_900M
#define DDR_700M

#else
#define APLL_900M
#define DDR_500M
#endif

#ifdef APLL_600M
#define CONFIG_SYS_APLL_FREQ		600000000       /*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((75 << 20) | (1 << 14) | (3 << 11) | (1<<8))
#elif defined APLL_804M
#define CONFIG_SYS_APLL_FREQ		804000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((67 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_864M
#define CONFIG_SYS_APLL_FREQ		864000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((72 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_891M
#define CONFIG_SYS_APLL_FREQ		891000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((297 << 20) | (2 << 14) | (4 << 11) | (1<<8))
#elif defined APLL_900M
#define CONFIG_SYS_APLL_FREQ		900000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((75 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_950M
#define CONFIG_SYS_APLL_FREQ		950000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((475 << 20) | (3 << 14) | (4 << 11) | (1<<8))
#elif defined APLL_1000M
#define CONFIG_SYS_APLL_FREQ		1000000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((125 << 20) | (1 << 14) | (3 << 11) | (1<<8))
#elif defined APLL_1008M
#define CONFIG_SYS_APLL_FREQ		1008000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((84 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_1050M
#define CONFIG_SYS_APLL_FREQ		1050000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((525 << 20) | (3 << 14) | (4 << 11) | (1<<8))
#elif defined APLL_1100M
#define CONFIG_SYS_APLL_FREQ		1100000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((550 << 20) | (4 << 14) | (3 << 11) | (1<<8))
#elif defined APLL_1104M
#define CONFIG_SYS_APLL_FREQ		1104000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((92 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_1188M
#define CONFIG_SYS_APLL_FREQ		1188000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((99 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_1200M
#define CONFIG_SYS_APLL_FREQ		1200000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((100 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_1392M
#define CONFIG_SYS_APLL_FREQ		1392000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((116 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_1400M
#define CONFIG_SYS_APLL_FREQ		1404000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((117 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined APLL_1500M
#define CONFIG_SYS_APLL_FREQ		1500000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_APLL_MNOD		((125 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#else
#error please define APLL_FREQ
#endif

#ifdef DDR_400M
#define CONFIG_SYS_MPLL_FREQ		1200000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((100 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_450M
#define CONFIG_SYS_MPLL_FREQ		900000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((75 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_500M
#define CONFIG_SYS_MPLL_FREQ		1000000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((125 << 20) | (1 << 14) | (3 << 11) | (1<<8))
#elif defined DDR_540M
#define CONFIG_SYS_MPLL_FREQ		1080000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((90 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_550M
#define CONFIG_SYS_MPLL_FREQ		1100000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((275 << 20) | (3 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_560M
#define CONFIG_SYS_MPLL_FREQ		1120000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((280 << 20) | (3 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_570M
#define CONFIG_SYS_MPLL_FREQ		1140000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((285 << 20) | (3 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_580M
#define CONFIG_SYS_MPLL_FREQ		1160000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((290 << 20) | (3 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_590M
#define CONFIG_SYS_MPLL_FREQ		1180000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((295 << 20) | (3 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_600M
#define CONFIG_SYS_MPLL_FREQ		1200000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((100 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_650M
#define CONFIG_SYS_MPLL_FREQ		1300000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((325 << 20) | (3 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_667M
#define CONFIG_SYS_MPLL_FREQ		1334000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((667 << 20) | (6 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_700M
#define CONFIG_SYS_MPLL_FREQ		1400000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((350 << 20) | (3 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_750M
#define CONFIG_SYS_MPLL_FREQ		1500000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((125 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_762M
#define CONFIG_SYS_MPLL_FREQ		1524000000	/*If APLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((127 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_800M
#define CONFIG_SYS_MPLL_FREQ		800000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((100 << 20) | (1 << 14) | (3 << 11) | (1<<8))
#elif defined DDR_900M
#define CONFIG_SYS_MPLL_FREQ		900000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((75 << 20) | (1 << 14) | (2 << 11) | (1<<8))
#elif defined DDR_933M
#define CONFIG_SYS_MPLL_FREQ		933000000	/*If MPLL not use mast be set 0*/
#define CONFIG_SYS_MPLL_MNOD		((933 << 20) | (6 << 14) | (4 << 11) | (1<<8))
#else
#error please define DDR_FREQ
#endif

#define CONFIG_SYS_VPLL_FREQ		1188000000	/*If VPLL not use mast be set 0*/
#define CONFIG_SYS_VPLL_MNOD		((198 << 20) | (2 << 14) | (2 << 11) | (1<<8))

#define SEL_SCLKA		2
#define SEL_CPU			1
#define SEL_H0			2
#define SEL_H2			2

#ifdef DDR_400M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_450M
#define DIV_PCLK        10
#define DIV_H2          5
#define DIV_H0          5
#elif defined DDR_500M
#define DIV_PCLK        10
#define DIV_H2          5
#define DIV_H0          5
#elif defined DDR_540M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_550M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_560M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_570M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_580M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_590M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_600M
#define DIV_PCLK        12
#define DIV_H2          6
#define DIV_H0          6
#elif defined DDR_650M
#define DIV_PCLK        14
#define DIV_H2          7
#define DIV_H0          7
#elif defined DDR_667M
#define DIV_PCLK        14
#define DIV_H2          7
#define DIV_H0          7
#elif defined DDR_700M
#define DIV_PCLK        14
#define DIV_H2          7
#define DIV_H0          7
#elif defined DDR_750M
#define DIV_PCLK        16
#define DIV_H2          8
#define DIV_H0          8
#elif defined DDR_762M
#define DIV_PCLK        16
#define DIV_H2          8
#define DIV_H0          8
#elif defined DDR_800M
#define DIV_PCLK        8
#define DIV_H2          4
#define DIV_H0          4
#elif defined DDR_900M
#define DIV_PCLK        10
#define DIV_H2          5
#define DIV_H0          5
#elif defined DDR_933M
#define DIV_PCLK        10
#define DIV_H2          5
#define DIV_H0          5
#else
#error please define DDR_FREQ
#endif

#define DIV_L2			2
#define DIV_CPU			1
#define CONFIG_SYS_CPCCR_SEL		(((SEL_SCLKA & 3) << 30)			\
									 | ((SEL_CPU & 3) << 28)			\
									 | ((SEL_H0 & 3) << 26)				\
									 | ((SEL_H2 & 3) << 24)				\
									 | (((DIV_PCLK - 1) & 0xf) << 16)	\
									 | (((DIV_H2 - 1) & 0xf) << 12)		\
									 | (((DIV_H0 - 1) & 0xf) << 8)		\
									 | (((DIV_L2 - 1) & 0xf) << 4)		\
									 | (((DIV_CPU - 1) & 0xf) << 0))

#define CONFIG_CPU_SEL_PLL		APLL
#define CONFIG_DDR_SEL_PLL		MPLL
#define CONFIG_SYS_CPU_FREQ		CONFIG_SYS_APLL_FREQ

#ifdef DDR_400M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 6)
#elif defined DDR_450M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_500M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_540M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_550M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_560M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_570M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_580M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_590M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_600M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_650M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_667M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_700M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_750M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_762M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 4)
#elif defined DDR_800M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined DDR_900M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#elif defined DDR_933M
#define CONFIG_SYS_MEM_FREQ		(CONFIG_SYS_MPLL_FREQ / 2)
#else
#error please define DDR_FREQ
#endif

#define CONFIG_SYS_EXTAL		24000000	/* EXTAL freq: 24 MHz */
#define CONFIG_SYS_HZ			1000		/* incrementer freq */

/* #define CONFIG_DDRP_PLL1 */ /* ddrp pll 配置1 */
/* #define CONFIG_DDRP_PLL2 */ /* ddrp pll 配置2 */
#define CONFIG_DDRP_PLL3 /* ddrp pll 配置3 */


/* CACHE */
#define CONFIG_SYS_DCACHE_SIZE      CONFIG_L1CACHE_SIZE
#define CONFIG_SYS_DCACHELINE_SIZE  CONFIG_L1CACHELINE_SIZE
#define CONFIG_SYS_DCACHE_WAYS      CONFIG_L1CACHE_WAYNUM
#define CONFIG_SYS_ICACHE_SIZE      CONFIG_L1CACHE_SIZE
#define CONFIG_SYS_ICACHELINE_SIZE  CONFIG_L1CACHELINE_SIZE
#define CONFIG_SYS_ICACHE_WAYS      CONFIG_L1CACHE_WAYNUM
#define CONFIG_SYS_CACHELINE_SIZE   CONFIG_L1CACHELINE_SIZE
#define CONFIG_BOARD_L2CACHE
#define CONFIG_SYS_L2CACHE_SIZE     CONFIG_L2CACHE_SIZE
#define CONFIG_SYS_L2CACHELINE_SIZE CONFIG_L2CACHELINE_SIZE
#define CONFIG_SYS_L2CACHE_WAYS     CONFIG_L2CACHE_WAYNUM
#if 0
#define CONFIG_SYS_DCACHE_SIZE		CONFIG_L1CACHE_SIZE
#define CONFIG_SYS_DCHCHAELINE_
#define CONFIG_SYS_ICACHE_SIZE		CONFIG_L1CACHE_SIZE
#define CONFIG_SYS_CACHELINE_SIZE	CONFIG_L1CACHELINE_SIZE
#define CONFIG_SYS_L2CACHE_SIZE		CONFIG_L2CACHE_SIZE
#define CONFIG_SYS_L2CACHELINE_SIZE	CONFIG_L2CACHELINE_SIZE
#endif

/* UART */
#define CONFIG_SYS_UART_INDEX       1
#define CONFIG_BAUDRATE             115200
#define CONFIG_SYS_UART_CONTROLLER_STEP 0x1000

/* DDR */
/*
 *#define CONFIG_DDR_TEST_CPU
 *#define CONFIG_DDR_TEST
 */
#define CONFIG_DDR_DLL_OFF
#define CONFIG_DDR_HOST_CC
#define CONFIG_DDR_FORCE_SELECT_CS1

#define CONFIG_DDRC_CTRL_PDT DDRC_CTRL_PDT_128

#if defined(CONFIG_PRJ007NQ) || defined(CONFIG_PRJ008N)
#define CONFIG_DDR3_W631GU6NG
#define CONFIG_DDR_TYPE_DDR3
#elif defined(CONFIG_PRJ007VN) || defined(CONFIG_PRJ007ZN) || defined(CONFIG_PRJ008VN) || defined(CONFIG_PRJ008ZN)
#define CONFIG_DDR3_NK5CC64M16HQ3
#define CONFIG_DDR_TYPE_DDR3
#elif defined(CONFIG_PRJ007VNP)
#define CONFIG_LPDDR3_W63AH6N2B_BJ
#define CONFIG_DDR_TYPE_LPDDR3
#elif defined(CONFIG_PRJ007XQ) || defined(CONFIG_PRJ007ZX) || defined(CONFIG_PRJ007VX)
#define CONFIG_DDR3_W632GU6NG
#define CONFIG_DDR_TYPE_DDR3
#elif defined(CONFIG_PRJ007LQ)
#define CONFIG_DDR2_W9751V6NG
#define CONFIG_DDR_TYPE_DDR2
#elif defined(CONFIG_PRJ007ZL) || defined(CONFIG_PRJ007VL) || defined(CONFIG_PRJ008ZL) || defined(CONFIG_PRJ008VL) || defined(CONFIG_PRJ008L) || defined(CONFIG_PRJ008DL)
#define CONFIG_DDR2_M14D5121632A
#define CONFIG_DDR_TYPE_DDR2
#elif defined(CONFIG_PRJ007A)
#define CONFIG_DDR3_W638GU6QB
/* #define CONFIG_DDR3_W634GU6RB */
#define CONFIG_DDR_TYPE_DDR3
#else
/* #define CONFIG_DDR3_W631GU6NG */
#define CONFIG_DDR3_NK5CC64M16HQ3
#define CONFIG_DDR_TYPE_DDR3
#endif

#define CONFIG_DDR_PARAMS_CREATOR
#define CONFIG_DDR_REMAP

#if defined(CONFIG_DDR_TYPE_DDR3) || defined(CONFIG_DDR_TYPE_LPDDR3)
#define CONFIG_DDR_HARDWARE_TRAIN
#endif

#if defined(CONFIG_DDR_TYPE_DDR2) || defined(CONFIG_DDR_TYPE_LPDDR2)
#define CONFIG_DDR_SOFT_TRAIN
#endif

#define CONFIG_DDR_INNOPHY

#define CONFIG_DDR_PHY_IMPEDANCE    40000
#define CONFIG_DDR_PHY_ODT_IMPEDANCE 50000      /* 75000 */


#define CONFIG_SYS_MAX_FLASH_SECT 0
/* #define CONFIG_SFC_NOR*/
#if defined(CONFIG_SPL_SFC_NOR) || defined(CONFIG_SPL_SFC_NAND)
#define CDT_OFF 1 /*1:not use CDT;0:use CDT*/
#define CONFIG_SPL_SFC_SUPPORT
#if CDT_OFF
#define CONFIG_JZ_SFC
#else
#define CONFIG_JZ_SFC_CDT
#endif /* endif CDT_OFF */
#define CONFIG_SPL_PAD_TO_BLOCK
#define CONFIG_SPL_VERSION 1
#ifdef CONFIG_SPL_SFC_NOR
#define CONFIG_SFC_NOR
#define CONFIG_SFC_NOR_COMMAND
#else
#define CONFIG_SFC_NAND
#define CONFIG_SFC_NAND_COMMAND
#endif /* defined CONFIG_SPL_SFC_NOR */
#endif /* defined(CONFIG_SPL_SFC_NOR) || defined(CONFIG_SPL_SFC_NAND) */

/* #define CONFIG_MSC_SFC */

/**
 * Boot arguments definitions.
 */
#if defined(CONFIG_DDR3_W631GU6NG) || defined(CONFIG_DDR3_NK5CC64M16HQ3) || defined(CONFIG_LPDDR3_W63AH6N2B_BJ)
#define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=64M@0x0 rmem=64M@0x4000000"
#elif defined(CONFIG_DDR3_W632GU6NG)
#define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=128M@0x0 rmem=128M@0x8000000"
#elif defined(CONFIG_DDR2_W9751V6NG) || defined(CONFIG_DDR2_M14D5121632A)
#define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=41M@0x0 rmem=23M@0x2900000"
#elif defined(CONFIG_DDR3_W638GU6QB)
#define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=256M@0x0 mem=768M@0x50000000"
/* #define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=256M@0x0 mem=256M@0x50000000" */
#else
#define BOOTARGS_COMMON "console=ttyS1,115200n8 mem=106M@0x0 rmem=22M@0x6a00000"
#endif

#ifdef CONFIG_FAST_BOOT
#ifdef CONFIG_SPL_MMC_SUPPORT
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc root=/dev/mmcblk0p2 rw rootdelay=1"
#elif defined(CONFIG_SFC_NOR)
#ifdef CONFIG_OF_LIBFDT
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=sfc0_nor:256k(boot),2560k(kernel),2048k(root),64k(dtb),-(appfs) lpj=11968512 quiet"
#else
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=sfc0_nor:256k(boot),2560k(kernel),2048k(root),-(appfs) lpj=11968512 quiet"
#endif
#elif defined(CONFIG_SFC_NAND)
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc ubi.mtd=2 root=ubi0:rootfs rootfstype=ubifs rw mtdparts=sfc0_nand:1M(uboot),3M(kernel),20M(root),-(appfs) lpj=11968512 quiet"
#endif
#else
#ifdef CONFIG_SPL_MMC_SUPPORT
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc root=/dev/mmcblk0p2 rw rootdelay=1"
#elif defined(CONFIG_SFC_NOR)
#ifdef CONFIG_OF_LIBFDT
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=sfc0_nor:256k(boot),2560k(kernel),2048k(root),64k(dtb),-(appfs)"
#else
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=sfc0_nor:256k(boot),2560k(kernel),2048k(root),-(appfs)"
#endif
#elif defined(CONFIG_SFC_NAND)
	#define CONFIG_BOOTARGS BOOTARGS_COMMON " init=/linuxrc ubi.mtd=2 root=ubi0:rootfs rootfstype=ubifs rw mtdparts=sfc0_nand:1M(uboot),3M(kernel),20M(root),-(appfs)"
#endif
#endif

/**
 * Boot command definitions.
 */
#define CONFIG_CMD_WATCHDOG
#define CONFIG_BOOTDELAY 1

#ifdef CONFIG_SPL_MMC_SUPPORT
    #define CONFIG_BOOTCOMMAND "mmc read 0x80600000 0x1800 0x3000; bootm 0x80600000"
#endif  /* CONFIG_SPL_MMC_SUPPORT */

/*#define CONFIG_BIG_SPL*/
#ifdef CONFIG_BIG_SPL
#define CONFIG_SPL_MAX_SIZE		102400	/* 100KB */
#elif defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_SPL_MAX_SIZE		34816
#else
#define CONFIG_SPL_MAX_SIZE		26624	/* 26624=26KB*/
#endif

#define CONFIG_SPL_PAD_TO		CONFIG_SPL_MAX_SIZE /* equal to spl max size in T15g */
#define CONFIG_UBOOT_OFFSET             CONFIG_SPL_MAX_SIZE /* equal to spl max size in T15g */

/* used for debug */
/* #define CONFIG_SPL_LIBCOMMON_SUPPORT */


#ifdef CONFIG_SFC_NOR
#ifdef CONFIG_OF_LIBFDT
	#define CONFIG_BOOTCOMMAND "sf0 probe;sf0 read 0x80600000 0x40000 0x280000;sf0 read 0x83000000 0x540000 0x10000;bootm 0x80600000 - 0x83000000"
#else
	#define CONFIG_BOOTCOMMAND "sf0 probe;sf0 read 0x80600000 0x40000 0x280000;bootm 0x80600000"
#endif
#endif /* CONFIG_SFC_NOR */

#ifdef CONFIG_SFC_NAND
	#define CONFIG_BOOTCOMMAND "nand read 0x80600000 0x100000 0x300000;bootm 0x80600000"
#endif /* CONFIG_SFC_NAND */



/**
 * Drivers configuration.
 */
/* SPI */
#if defined(CONFIG_SPL_SPI_SUPPORT)
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_JZ_SPI_PC_6BIT
#define CONFIG_SYS_SPI_BOOT_FREQ	1000000

#define CONFIG_SSI_BASE         SSI0_BASE
#define CONFIG_SYS_MONITOR_LEN		(230 * 1024)
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE                 (256 * 1024)
#define CONFIG_ENV_OFFSET		(256 * 1024)
#define CONFIG_ENV_SECT_SIZE		(4 * 1024)
#endif
/* END SPI */

#ifdef CONFIG_SFC_COMMAND/* SD card start */
#ifndef CONFIG_NAND_COMMAND
#define CONFIG_SFC_NOR_COMMAND			/* support nor command */
#define CONFIG_SFC0_NOR
#define CONFIG_SFC1_NOR
#else
#define CONFIG_SFC_NAND_COMMAND		/* support nand command */
#define CONFIG_SFC0_NAND
#define CONFIG_SFC1_NAND
#endif
#endif /* CONFIG_SFC_COMMAND */

/* SFC */
#define CONFIG_JZ_SFC_PA_6BIT
#define CONFIG_JZ_SFC1_PC
#ifdef CONFIG_SFC_NOR_COMMAND
#define CONFIG_SFC_MIN_ALIGN			0x1000	/*0x1000->4K Erase,0x8000->32k 0x10000->64k*/
#define CONFIG_CMD_SF
/* #define CONFIG_CMD_SFC_NOR_FEATURES */		/* nor flash features operations CMD */
#define CONFIG_SPI_FLASH
#define CONFIG_JZ_SFC
#define CONFIG_SFC_NOR
#define CONFIG_SPI_FLASH_INGENIC
#endif /* CONFIG_SFC_NOR_COMMAND */

/* #define CONFIG_CMD_SFC_NAND_FEATURES */		/* nand flash features operations CMD */
/* SFC */
#define CONFIG_SFC_MIN_ALIGN        0x1000      /*0x1000->4K Erase,0x8000->32k 0x10000->64k*/
#if defined(CONFIG_SPL_SFC_SUPPORT) || defined(CONFIG_SFC_NAND_COMMAND)
#ifndef CONFIG_SIMU
#define CONFIG_SPL_SERIAL_SUPPORT
#endif
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_JZ_SFC_PA_6BIT
#define CONFIG_ENV_SIZE             (32 << 10)
#define CONFIG_ENV_OFFSET           (CONFIG_SYS_NAND_BLOCK_SIZE * 5)

#if defined(CONFIG_SPL_SFC_NAND) || defined(CONFIG_SFC_NAND_COMMAND)
#define CONFIG_NAND_BURNER
#define CONFIG_SPIFLASH_PART_OFFSET (26 * 1024)
#define CONFIG_SPI_NAND_BPP         (2048 + 64) /*Bytes Per Page*/
#define CONFIG_SPI_NAND_PPB         (64)        /*Page Per Block*/
#define CONFIG_SFC_NAND_RATE        50000000
#define CONFIG_MTD_SFCNAND
#define CONFIG_CMD_SFCNAND
#define CONFIG_CMD_NAND
#define CONFIG_SPI_SPL_CHECK
#define CONFIG_SYS_MAX_NAND_DEVICE  1
#define CONFIG_SYS_NAND_BASE        0xb3441000
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define MTDIDS_DEFAULT              "nand0=nand"
#define MTDPARTS_DEFAULT            "mtdparts=nand:1M(uboot),3M(kernel),20M(root),-(appfs)"
#if 1
#define CONFIG_SPI_STANDARD /* if the nand is QUAD mode, please annotate it. the default is one lan. */
#endif

#ifdef CONFIG_SPL_SFC_SUPPORT
/*SFCNAND env*/
/* spi nand environment */
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_ENV_SECT_SIZE        0x20000     /* 128K*/
#define SPI_NAND_BLK                0x20000     /*the spi nand block size */
#define CONFIG_ENV_SIZE             SPI_NAND_BLK/* uboot is 1M but the last block size is the env*/
#define CONFIG_ENV_OFFSET           0xc0000     /* offset is 768k */
#define CONFIG_ENV_OFFSET_REDUND    (CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)
#define CONFIG_ENV_IS_IN_SFC_NAND
#endif

/* MTD support */
#define CONFIG_SYS_NAND_SELF_INIT
#define CONFIG_UBOOT_OFFSET     CONFIG_SPL_MAX_SIZE /* (26  * 1024) */

#elif defined(CONFIG_SPL_SFC_NOR)
#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH_INGENIC
#define CONFIG_SPI_FLASH
#define CONFIG_UBOOT_OFFSET     CONFIG_SPL_MAX_SIZE /* (26  * 1024) */
#endif
#endif /* CONFIG_SPL_SFC_SUPPORT */
/* END SFC */

/* MMC */
#define CONFIG_GENERIC_MMC 1
#define CONFIG_MMC 1
#define CONFIG_SDHCI
#define CONFIG_JZ_SDHCI

#if 0
#define CONFIG_JZ_MMC_POWER_GPIO GPIO_PB(27)
#define CONFIG_JZ_MMC_POWER_GPIO_ENLEVEL 0
#endif

#ifdef CONFIG_JZ_MMC_MSC0
#define CONFIG_JZ_MMC_SPLMSC 0
#define CONFIG_JZ_MMC_MSC0_PB_4BIT 1
#define CONFIG_SPI_SPL_CHECK
#endif
#ifdef CONFIG_JZ_MMC_MSC1
#define CONFIG_JZ_MMC_SPLMSC 1
#define CONFIG_JZ_MMC_MSC1_PC 1
#define CONFIG_SPI_SPL_CHECK
#endif

/* I2C */
/* #define CONFIG_SOFT_I2C */
/* #define CONFIG_SYS_I2C_SPEED		50   */  /* the function is not implemented */
/* #define CONFIG_SYS_I2C_SLAVE		0x00  */ /* the function is not implemented */

/* #define CONFIG_SOFT_I2C_GPIO_SCL	GPIO_PA(13) */
/* #define CONFIG_SOFT_I2C_GPIO_SDA	GPIO_PA(12) */

/* PMU */
/* #define CONFIG_REGULATOR */
/* #define CONFIG_PMU_D2041 */

/* GMAC */
#define CONFIG_GMAC_PHY_RATE     25000000
#if (CONFIG_SYS_APLL_FREQ % CONFIG_GMAC_PHY_RATE)
#define CONFIG_MACPHY_SEL_PLL    MPLL
#else
#define CONFIG_MACPHY_SEL_PLL    APLL
#endif

#define GMAC_PHY_MII	1
#define GMAC_PHY_RMII	2
#define GMAC_PHY_GMII	3
#define GMAC_PHY_RGMII	4

#define PHY_TYPE_DM9161      1
#define PHY_TYPE_88E1111     2
#define PHY_TYPE_DP83867     3
#define PHY_TYPE_LAN8720     4

#ifdef CONFIG_FPGA_V10_PLAT
#define CONFIG_NET_GMAC_PHY_MODE GMAC_PHY_GMII
#define CONFIG_NET_PHY_TYPE   PHY_TYPE_LAN8720
#else
#define CONFIG_NET_GMAC_PHY_MODE GMAC_PHY_RMII
#define CONFIG_NET_PHY_TYPE   PHY_TYPE_LAN8720
#endif

#define CONFIG_NET_GMAC
#if (defined CONFIG_PRJ007VN || defined CONFIG_PRJ007VX) && !defined(CONFIG_JZ_MMC_MSC1)
#define CONFIG_GPIO_PHY_RESET	GPIO_PC(07)
#else
#define CONFIG_GPIO_PHY_RESET	GPIO_PB(26)
#endif
#define CONFIG_GPIO_PHY_RESET_ENLEVEL	0

/* DEBUG ETHERNET */
#define CONFIG_SERVERIP 10.3.2.4
#define CONFIG_IPADDR 192.168.2.200
#define CONFIG_GATEWAYIP 192.168.2.1
#define CONFIG_NETMASK 255.255.255.0
#define CONFIG_ETHADDR 00:11:22:56:96:68

/* GPIO */
#define CONFIG_JZ_GPIO

/**
 * Command configuration.
 */
/* #define CONFIG_CMD_NAND
#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_SYS_NAND_BASE            0xb3441000 */
#define CONFIG_CMD_BOOTD   /* bootd			*/
#define CONFIG_CMD_CONSOLE /* coninfo			*/
#if 0
#define CONFIG_CMD_DHCP    /* DHCP support			*/
#endif
#define CONFIG_CMD_ECHO    /* echo arguments		*/
#if 0
#define CONFIG_CMD_EXT4    /* ext4 support			*/
#endif
#define CONFIG_CMD_FAT     /* FAT support			*/
#if 0
#define CONFIG_CMD_LOADB   /* loadb			*/
#define CONFIG_CMD_LOADS   /* loads			*/
#endif
#define CONFIG_CMD_MEMORY  /* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC    /* Misc functions like sleep etc*/
#define CONFIG_CMD_MMC		/* MMC/SD support */
#define CONFIG_CMD_NET                     /* networking support			*/
#define CONFIG_CMD_PING
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#if 0
#define CONFIG_CMD_SETGETDCR	/* DCR support on 4xx		*/
#define CONFIG_CMD_SOURCE	/* "source" command support	*/
#define CONFIG_CMD_GETTIME
#endif
/* #define CONFIG_CMD_EEPROM */
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
#define CONFIG_CMDLINE_EDITING
#define CONFIG_CMD_BUS_DEBUG
#define CONFIG_AUTO_COMPLETE
/*#define CONFIG_CMD_I2C*/

/* #define CONFIG_AUTO_UPDATE */
#ifdef CONFIG_AUTO_UPDATE
#define CONFIG_CMD_SDUPDATE     1
#endif

/************************ LCD CONFIG *************************/



/*
 *#define CONFIG_LCD
 */


#ifdef CONFIG_LCD
#define LCD_BPP             4 /* 4: 16BPP, 5: 24BPP. */
#define CONFIG_LCD_LOGO
#define CONFIG_SMART_LCD
#ifdef CONFIG_SMART_LCD
/*
 *#define CONFIG_SLCD_VDDEN_PIN		GPIO_PB(21)
 */
#define CONFIG_SLCD_RST_PIN			GPIO_PB(27)
#define CONFIG_SLCD_CS_PIN			GPIO_PB(26)
/* #define CONFIG_SLCD_RD_PIN			GPIO_PB(27) */
#define CONFIG_SLCD_BL_PIN			GPIO_PB(28)	// backlight
#endif

/*
 *#define CONFIG_CMD_LOGO_RLE
 */
#define CONFIG_RLE_LCD_LOGO
/*#define CONFIG_LOGO_EXTEND*/
/*#define CONFIG_LCD_INFO_BELOW_LOGO*/

#define CONFIG_GPIO_LCD_PWM     GPIO_PD(14)		// for backlight
#define CONFIG_SYS_PWM_PERIOD       10000 /* Pwm period in ns */
#define CONFIG_SYS_PWM_CHN      3  /* Pwm channel ok*/
#define CONFIG_SYS_PWM_FULL 256
#define CONFIG_SYS_BACKLIGHT_LEVEL 80 /* Backlight brightness is (80 / 256) */

#define CONFIG_JZ_LCD_V14
/*
 *#define CONFIG_JZ_PWM
 *#define CONFIG_VIDEO_FW050
 *#define CONFIG_VIDEO_OTA7290B
 *#define CONFIG_SYS_WHITE_ON_BLACK
 */
#define CONFIG_VIDEO_ILI9342C

/*
 *#ifdef CONFIG_VIDEO_FW050
 *#define CONFIG_JZ_MIPI_DSI
 *#endif
 */

/*
 *#ifdef CONFIG_VIDEO_OTA7290B
 *#define CONFIG_JZ_MIPI_DSI
 *#endif
 */


/*#define CONFIG_JZ_PWM*/
#define CONFIG_SYS_CONSOLE_INFO_QUIET
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

/*#define CONFIG_LCD_GPIO_FUNC1_TFT*/

#ifdef CONFIG_VIDEO_ILI9342C
#define CONFIG_GPIO_LCD_CS     GPIO_PB(26)
/*#define CONFIG_GPIO_LCD_RD     GPIO_PB(27) */
#define CONFIG_GPIO_LCD_RST     GPIO_PB(27)
#define CONFIG_GPIO_BL_PWR_EN   GPIO_PB(28)
#endif

#ifdef CONFIG_VIDEO_FW050
#define CONFIG_GPIO_LCD_VDD     GPIO_PC(3)
#define CONFIG_GPIO_LCD_PWM     GPIO_PB(31)
#define CONFIG_GPIO_LCD_RST     GPIO_PC(2)
#define CONFIG_GPIO_LCD_TE      GPIO_PC(10)
#endif

#endif /* CONFIG_LCD */

/*eeprom*/
#ifdef CONFIG_CMD_EEPROM
#define CONFIG_SYS_I2C_EEPROM_ADDR  0x50
/*#define CONFIG_ENV_EEPROM_IS_ON_I2C*/
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN	1
#endif

/* USB */
/*#define CONFIG_CMD_FASTBOOT*/
#define CONFIG_USB_GADGET
#define CONFIG_USB_GADGET_DUALSPEED
#define CONFIG_USB_JZ_DWC2_UDC_V1_1
#define CONFIG_FASTBOOT_GADGET
#define CONFIG_FASTBOOT_FUNCTION
#define CONFIG_G_FASTBOOT_VENDOR_NUM	(0x18d1)
#define CONFIG_G_FASTBOOT_PRODUCT_NUM	(0xdddd)
#define CONFIG_USB_GADGET_VBUS_DRAW 500

/* #define CONFIG_JZ_PWM */
/* #define CONFIG_JZ_ADC */

/**
 * Serial download configuration
 */
#define CONFIG_LOADS_ECHO	1	/* echo on for serial download */

/**
 * Miscellaneous configurable options
 */
#define CONFIG_DOS_PARTITION

#define CONFIG_LZO
#define CONFIG_RBTREE
#define CONFIG_LZMA
#define CONFIG_HARD_LZMA

#define CONFIG_SKIP_LOWLEVEL_INIT
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_SYS_NO_FLASH
#define CONFIG_SYS_FLASH_BASE	0 /* init flash_base as 0 */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_MISC_INIT_R 1

#define CONFIG_BOOTP_MASK	(CONFIG_BOOTP_DEFAUL)

#define CONFIG_SYS_MAXARGS 16
#define CONFIG_SYS_LONGHELP

#if defined(CONFIG_PRJ007)
#define CONFIG_SYS_PROMPT "PRJ007# "
#elif defined(CONFIG_PRJ008)
#define CONFIG_SYS_PROMPT "PRJ008# "
#else
#define CONFIG_SYS_PROMPT CONFIG_SYS_BOARD "# "
#endif

#define CONFIG_SYS_CBSIZE 1024 /* Console I/O Buffer Size */
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/* #define CONFIG_SYS_MONITOR_LEN		(768 * 1024)
#define CONFIG_SYS_MALLOC_LEN		(64 * 1024 * 1024) */
#define CONFIG_SYS_BOOTPARAMS_LEN	(128 * 1024)

/*#define DBG_USE_UNCACHED_MEMORY*/
#ifdef DBG_USE_UNCACHED_MEMORY
#define CONFIG_SYS_SDRAM_BASE		0xA0000000 /* uncached (KSEG0) address */
#define CONFIG_SYS_SDRAM_MAX_TOP	0xB0000000 /* don't run into IO space */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000
#define CONFIG_SYS_LOAD_ADDR		0xA8000000
#define CONFIG_SYS_MEMTEST_START	0xA0000000
#define CONFIG_SYS_MEMTEST_END		0xA8000000

#define CONFIG_SYS_TEXT_BASE		0xA0100000
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#else /* DBG_USE_UNCACHED_MEMORY */
#define CONFIG_SYS_SDRAM_BASE		0x80000000 /* cached (KSEG0) address */
#define CONFIG_SYS_SDRAM_MAX_TOP	0x90000000 /* don't run into IO space */
#define CONFIG_SYS_INIT_SP_OFFSET	0x400000
#define CONFIG_SYS_LOAD_ADDR		0x80600000
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x88000000

#define CONFIG_SYS_TEXT_BASE		0x80100000
#define CONFIG_SYS_MONITOR_BASE		CONFIG_SYS_TEXT_BASE
#endif /* DBG_USE_UNCACHED_MEMORY */

/**
 * Environment
 */
#ifdef  CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE         (32 << 10)
#define CONFIG_ENV_OFFSET       (CONFIG_SYS_MONITOR_LEN + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)
#elif CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SECT_SIZE    (1024 * 16)
#define CONFIG_ENV_SIZE         (1024 * 16)
#define CONFIG_ENV_OFFSET       (CONFIG_SYS_MONITOR_LEN + CONFIG_UBOOT_OFFSET)
#endif /* endif CONFIG_ENV_IS_IN_MMC */

/**
 * Miscellaneous configurable options
 */

#if defined(CONFIG_SFC_NAND) || defined(CONFIG_SFC_NAND_COMMAND)
#define CONFIG_SYS_MONITOR_LEN      (400 * 1024)
#else
#ifdef CONFIG_OF_LIBFDT /* support device tree */
#define CONFIG_SYS_MONITOR_LEN      (230 * 1024)
#else
#define CONFIG_SYS_MONITOR_LEN      (214 * 1024)
#endif
#endif /* CONFIG_SFC_NAND || CONFIG_SFC_NAND_COMMOD */
#define CONFIG_SYS_MALLOC_LEN       (32 * 1024 * 1024)
#define CONFIG_SYS_BOOTPARAMS_LEN   (128 * 1024)


/**
 * SPL configuration
 */
#define CONFIG_SPL_FRAMEWORK
/*#define CONFIG_SPL_STACK		0x80001000*/  /* 4K stack_size */

#define CONFIG_SPL_NO_CPU_SUPPORT_CODE
#define CONFIG_SPL_START_S_PATH		"$(CPUDIR)/$(SOC)"
#ifdef CONFIG_SPL_NOR_SUPPORT
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/$(SOC)/u-boot-nor-spl.lds"
#else
#define CONFIG_SPL_LDSCRIPT		"$(CPUDIR)/$(SOC)/u-boot-spl.lds"
#endif

#define CONFIG_UBOOT_OFFSET                      CONFIG_SPL_MAX_SIZE
#define CONFIG_MMC_RAW_UBOOT_OFFSET              (CONFIG_UBOOT_OFFSET / 1024 + 17)
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR  (CONFIG_MMC_RAW_UBOOT_OFFSET * 2)
#define CONFIG_SYS_U_BOOT_MAX_SIZE_SECTORS       0x400 /* 512 KB */
#define CONFIG_SYS_NAND_U_BOOT_OFFS              (CONFIG_SYS_NAND_BLOCK_SIZE * 4)
#define CONFIG_SYS_NAND_U_BOOT_DST               CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_NAND_U_BOOT_START             CONFIG_SYS_NAND_U_BOOT_DST
#define CONFIG_SYS_NAND_U_BOOT_SIZE              (512 * 1024)

#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SPL_GPIO_SUPPORT
/* #define CONFIG_SPL_I2C_SUPPORT */
/* #define CONFIG_SPL_REGULATOR_SUPPORT */
#define CONFIG_SPL_CORE_VOLTAGE		1100

#ifdef CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_SIZE			(32 << 10)
#define CONFIG_ENV_OFFSET		(CONFIG_SYS_MONITOR_LEN + CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR * 512)
#endif /* endif CONFIG_ENV_IS_IN_MMC */
#if 1
#undef CONFIG_SPL_LZOP
#else
#define CONFIG_SPL_LZOP
#endif
#ifdef CONFIG_SPL_LZOP
	#define CONFIG_DECMP_BUFFER_ADRS        0x80200000
#endif

#ifdef CONFIG_SPL_NOR_SUPPORT
#define CONFIG_SPL_TEXT_BASE		0xba000000
#else
#define CONFIG_SPL_TEXT_BASE		0x80001000
#endif	/*CONFIG_SPL_NOR_SUPPORT*/

#ifdef CONFIG_SPL_MMC_SUPPORT
#ifndef CONFIG_SIMU
#define CONFIG_SPL_SERIAL_SUPPORT
#endif
#endif /* CONFIG_SPL_MMC_SUPPORT */

#ifdef CONFIG_SPL_NOR_SUPPORT
#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SYS_UBOOT_BASE		(CONFIG_SPL_TEXT_BASE + CONFIG_SPL_PAD_TO - 0x40)	//0x40 = sizeof (image_header)
#define CONFIG_SYS_OS_BASE		0
#define CONFIG_SYS_SPL_ARGS_ADDR	0
#define CONFIG_SYS_FDT_BASE		0
#endif

/**
 * GPT configuration
 */
#ifdef CONFIG_GPT_CREATOR
#define CONFIG_GPT_TABLE_PATH	"$(TOPDIR)/board/$(BOARDDIR)"
#else
/* USE MBR + zero-GPT-table instead if no gpt table defined*/
#define CONFIG_MBR_P0_OFF	64mb
#define CONFIG_MBR_P0_END	556mb
#define CONFIG_MBR_P0_TYPE 	linux

#define CONFIG_MBR_P1_OFF	580mb
#define CONFIG_MBR_P1_END 	1604mb
#define CONFIG_MBR_P1_TYPE 	linux

#define CONFIG_MBR_P2_OFF	28mb
#define CONFIG_MBR_P2_END	58mb
#define CONFIG_MBR_P2_TYPE 	linux

#define CONFIG_MBR_P3_OFF	1609mb
#define CONFIG_MBR_P3_END	7800mb
#define CONFIG_MBR_P3_TYPE 	fat
#endif

/**
 * Keys.
 */
#define CONFIG_GPIO_USB_DETECT		GPIO_PA(14)
#define CONFIG_GPIO_USB_DETECT_ENLEVEL	1

#define CONFIG_GPIO_PWR_WAKE		GPIO_PA(30)
#define CONFIG_GPIO_PWR_WAKE_ENLEVEL	0

/*
#define CONFIG_GPIO_DC_DETECT           GPIO_PB(1)
#define CONFIG_GPIO_DC_DETECT_ENLEVEL   0
*/

/* TEST
#define CONFIG_GPIO_DC_DETECT           GPIO_PG(10)
#define CONFIG_GPIO_DC_DETECT_ENLEVEL   1

#define CONFIG_GPIO_CHARGE_DETECT               GPIO_PG(12)
#define CONFIG_GPIO_CHARGE_DETECT_ENLEVEL       0
*/
#define CONFIG_SOCID_ADDRESS 0xb300002c
#define CONFIG_SOCID_PRJ007 0x10032004
#define CONFIG_SOCID_PRJ008 0x10033004
#endif /* __CONFIG_PRJ_H__ */
