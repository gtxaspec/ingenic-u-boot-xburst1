/*
 * T31 GPIO definitions
 * Copyright (c) 2019  Ingenic Semiconductor Co.,Ltd
 *
 * Author: Sonil <ztyan@ingenic.cn>
 * Based on: newxboot/modules/gpio/jz4775_gpio.c|jz4780_gpio.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

static struct jz_gpio_func_def uart_gpio_func[] = {
	[0] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x9 << 19},
	[1] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x3 << 23},
	[2] = {.port = GPIO_PORT_A, .func = GPIO_FUNC_2, .pins = 0x3 << 10},/*QFN*/
	/*[2] = {.port = GPIO_PORT_C, .func = GPIO_FUNC_2, .pins = 0x3 << 13},*/ /*BGA*/
};

static struct jz_gpio_func_def gpio_func[] = {
#if defined(CONFIG_JZ_MMC_MSC0_PB)
	{.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = (0x3 << 4 | 0xf << 0), .driver_strength = 8},
#endif
#if defined(CONFIG_JZ_MMC_MSC1_PB)
	{.port = GPIO_PORT_B, .func = GPIO_FUNC_1, .pins = (0x3 << 13|0xf << 8)},
#endif
#if defined(CONFIG_JZ_SFC_PA)
	{.port = GPIO_PORT_A, .func = GPIO_FUNC_1, .pins = (0x3 << 23) | (0x3 << 27), .driver_strength = 4},
#endif
};
