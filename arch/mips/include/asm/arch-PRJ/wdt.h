// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __WDT_H__
#define __WDT_H__

#include <asm/arch/base.h>

#define TCU_TSCR			0x3c

#define WDT_TCSR			0xc
#define WDT_TCER			0x4
#define WDT_TDR				0x0
#define WDT_TCNT			0x8

#define TSCR_WDTSC			(1 << 16)

#define WDT_CLK_DIV_1       0
#define WDT_CLK_DIV_4       1
#define WDT_CLK_DIV_16      2
#define WDT_CLK_DIV_64      3
#define WDT_CLK_DIV_256     4
#define WDT_CLK_DIV_1024    5

#define TCSR_PRESCALE_1			(0 << 3)
#define TCSR_PRESCALE_4			(1 << 3)
#define TCSR_PRESCALE_16		(2 << 3)
#define TCSR_PRESCALE_64		(3 << 3)
#define TCSR_PRESCALE_256		(4 << 3)
#define TCSR_PRESCALE_1024		(5 << 3)

#define TCSR_EXT_EN			(1 << 2)
#define TCSR_RTC_EN			(1 << 1)
#define TCSR_CLRZ			(1 << 10)

#define TCSR_PCK_EN			(1 << 0)

#define TCER_TCEN			(1 << 0)

#define WDT_DIV				64
#if (WDT_DIV == 1)
#define TCSR_PRESCALE			TCSR_PRESCALE_1
#elif (WDT_DIV == 4)
#define TCSR_PRESCALE			TCSR_PRESCALE_4
#elif (WDT_DIV == 16)
#define TCSR_PRESCALE			TCSR_PRESCALE_16
#elif (WDT_DIV == 64)
#define TCSR_PRESCALE			TCSR_PRESCALE_64
#elif (WDT_DIV == 256)
#define TCSR_PRESCALE			TCSR_PRESCALE_256
#elif (WDT_DIV == 1024)
#define TCSR_PRESCALE			TCSR_PRESCALE_1024
#endif

#define RESET_DELAY_MS			4
#define RTC_FREQ			32768
#define WDT_MAX_COUNT			(0xFFFF)

#endif /* __WDT_H__ */
