// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

static struct jz_gpio_func_def uart_gpio_func[] = {
	//[0] = { .port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x9 << 19},
	[0] = { .port = GPIO_PORT_C, .func = GPIO_FUNC_0, .pins = 0x3 << 8},
	[1] = { .port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x3 << 23},
	//[2] = { .port = GPIO_PORT_C, .func = GPIO_FUNC_3, .pins = 0xf << 26},
	[2] = { .port = GPIO_PORT_A, .func = GPIO_FUNC_3, .pins = 0x3 << 21},
};

static struct jz_gpio_func_def gpio_func[] = {
#if 0
#if defined(CONFIG_JZ_MMC_MSC0_PB_4BIT)
	{ .port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x3f << 0},
#endif
#if defined(CONFIG_JZ_MMC_MSC0_PA_8BIT)
#error CONFIG_JZ_MMC_MSC0_PA_8BIT NOT USE
	{ .port = GPIO_PORT_A, .func = GPIO_FUNC_1, .pins = (0x3<<18|0xff<<0)},
#endif
#if defined(CONFIG_JZ_MMC_MSC1_PC)
	{ .port = GPIO_PORT_C, .func = GPIO_FUNC_0, .pins = 0x3f << 2},
#endif
#endif
#if defined(CONFIG_JZ_SFC_PA_6BIT) || defined(CONFIG_JZ_SFC0_PA)
	{ .port = GPIO_PORT_A, .func = GPIO_FUNC_0, .pins = 0x3f << 23},
#endif
#ifdef CONFIG_JZ_SFC1_PC
	{ .port = GPIO_PORT_C, .func = GPIO_FUNC_1, .pins = 0x3f << 2},
#endif

#ifdef CONFIG_JZ_SPI_PC_6BIT
	{ .port = GPIO_PORT_C, .func = GPIO_FUNC_2, .pins = 0x3f << 2},
#endif
};
