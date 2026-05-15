/*
 * PRJ007 GPIO definitions
 *
 * Copyright (c) 2015 Ingenic Semiconductor Co.,Ltd
 * Author: Matthew <xu.guo@ingenic.com>
 * Based on: newxboot/modules/gpio/jz4775_gpio.c|jz4780_gpio.c
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

static struct jz_gpio_func_def uart_gpio_func[] = {
	//[0] = { .port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x9 << 19},
	[0] = { .port = GPIO_PORT_C, .func = GPIO_FUNC_0, .pins = 0x3 << 8},
	[1] = { .port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 0x3 << 23},
	//[2] = { .port = GPIO_PORT_C, .func = GPIO_FUNC_3, .pins = 0xf << 26},
	[2] = { .port = GPIO_PORT_A, .func = GPIO_FUNC_3, .pins = 0x3 << 21},
};

static struct jz_gpio_func_def gpio_func[] = {
#if defined(CONFIG_JZ_SFC_PA_6BIT) || defined(CONFIG_JZ_SFC0_PA)
	{ .port = GPIO_PORT_A, .func = GPIO_FUNC_0, .pins = 0x3f << 23},
#endif
#ifdef CONFIG_JZ_SFC1_PC
	{ .port = GPIO_PORT_C, .func = GPIO_FUNC_1, .pins = 0x3f << 2},
#endif

#ifdef CONFIG_LCD
	{.port = GPIO_PORT_B, .func = GPIO_FUNC_3, .pins = (0x3f<<6 | 0xf<<13 | 0x7<<25)},
#endif

#ifdef CONFIG_JZ_SPI_PC_6BIT
	{ .port = GPIO_PORT_C, .func = GPIO_FUNC_2, .pins = 0x3f << 2},
#endif
};
