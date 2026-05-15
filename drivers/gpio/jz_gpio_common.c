/*
 * JZ4775 common routines
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Sonil <ztyan@ingenic.cn>
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

#include <common.h>
#include <config.h>
#include <asm/io.h>
#include <asm/gpio.h>

#include <ingenic_soft_i2c.h>
#include <jz_pca953x.h>

#if defined (CONFIG_JZ4775)
#include "jz_gpio/jz4775_gpio.c"
#elif defined (CONFIG_JZ4780)
#include "jz_gpio/jz4780_gpio.c"
#elif defined (CONFIG_M200)
#include "jz_gpio/m200_gpio.c"
#elif defined (CONFIG_M150)
#include "jz_gpio/m150_gpio.c"
#elif defined (CONFIG_T15)
#include "jz_gpio/t15_gpio.c"
#elif defined (CONFIG_T10)
#include "jz_gpio/t10_gpio.c"
#elif defined (CONFIG_T20)
#include "jz_gpio/t20_gpio.c"
#endif

DECLARE_GLOBAL_DATA_PTR;

static inline is_gpio_from_chip(int gpio_num)
{
	return gpio_num < (GPIO_NR_PORTS * 32) ? 1 : 0;
}

void gpio_set_func(enum gpio_port n, enum gpio_function func, unsigned int pins)
{
	unsigned int base = GPIO_BASE + 0x100 * n;

	writel(func & 0x8? pins : 0, base + PXINTS);
	writel(func & 0x4? pins : 0, base + PXMSKS);
	writel(func & 0x2? pins : 0, base + PXPAT1S);
	writel(func & 0x1? pins : 0, base + PXPAT0S);

	writel(func & 0x8? 0 : pins, base + PXINTC);
	writel(func & 0x4? 0 : pins, base + PXMSKC);
	writel(func & 0x2? 0 : pins, base + PXPAT1C);
	writel(func & 0x1? 0 : pins, base + PXPAT0C);

	writel(func & 0x10? pins : 0, base + PXPEC);
	writel(func & 0x10? 0 : pins, base + PXPES);
}

int gpio_request(unsigned gpio, const char *label)
{
	printf("%s lable = %s gpio = %d\n",__func__,label,gpio);
	return gpio;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

void gpio_port_set_value(int port, int pin, int value)
{
	if (value)
		writel(1 << pin, GPIO_PXPAT0S(port));
	else
		writel(1 << pin, GPIO_PXPAT0C(port));
}

void gpio_port_direction_input(int port, int pin)
{
	writel(1 << pin, GPIO_PXINTC(port));
	writel(1 << pin, GPIO_PXMSKS(port));
	writel(1 << pin, GPIO_PXPAT1S(port));
}

void gpio_port_direction_output(int port, int pin, int value)
{
	writel(1 << pin, GPIO_PXINTC(port));
	writel(1 << pin, GPIO_PXMSKS(port));
	writel(1 << pin, GPIO_PXPAT1C(port));

	gpio_port_set_value(port, pin, value);
}

int gpio_set_value(unsigned gpio, int value)
{
	int port = gpio / 32;
	int pin = gpio % 32;
	if(is_gpio_from_chip(gpio)) {
		gpio_port_set_value(port, pin, value);
	} else {
#ifdef CONFIG_JZ_PCA953X
	pca953x_set_value(gpio, value);
#endif
	}
	return 0;
}

int gpio_get_value(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	if(is_gpio_from_chip(gpio)) {
	return !!(readl(GPIO_PXPIN(port)) & (1 << pin));
	} else {
#ifdef CONFIG_JZ_PCA953X
	return pca953x_get_value(gpio);
#endif
	}
}

int gpio_get_flag(unsigned int gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;

	return (readl(GPIO_PXFLG(port)) & (1 << pin));
}

int gpio_clear_flag(unsigned gpio)
{
	int port = gpio / 32;
	int pin = gpio % 32;
	writel(1 << pin, GPIO_PXFLGC(port));
	return 0;
}


int gpio_direction_input(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	if(is_gpio_from_chip(gpio)) {
		gpio_port_direction_input(port, pin);
	} else {
#ifdef CONFIG_JZ_PCA953X
	pca953x_direction_input(TO_PCA953X_GPIO(gpio));
#endif
	}

	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	if(is_gpio_from_chip(gpio)) {
		gpio_port_direction_output(port, pin, value);
	} else {
#ifdef CONFIG_JZ_PCA953X
	pca953x_direction_output(TO_PCA953X_GPIO(gpio), value);
#endif
	}
	return 0;
}

void gpio_enable_pull(unsigned gpio)
{
	unsigned port= gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXPEC(port));
}

void gpio_disable_pull(unsigned gpio)
{
	unsigned port= gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXPES(port));
}

void gpio_set_pull_dir(unsigned gpio, int pull)
{
	unsigned port= gpio / 32;
	unsigned pin = gpio % 32;
	if (pull) {
		writel(1 << pin, GPIO_PXPDIRS(port));
	} else {
		writel(1 << pin, GPIO_PXPDIRC(port));
	}
}

void gpio_as_irq_high_level(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXINTS(port));
	writel(1 << pin, GPIO_PXMSKC(port));
	writel(1 << pin, GPIO_PXPAT1C(port));
	writel(1 << pin, GPIO_PXPAT0S(port));
}

void gpio_as_irq_low_level(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXINTS(port));
	writel(1 << pin, GPIO_PXMSKC(port));
	writel(1 << pin, GPIO_PXPAT1C(port));
	writel(1 << pin, GPIO_PXPAT0C(port));
}

void gpio_as_irq_rise_edge(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXINTS(port));
	writel(1 << pin, GPIO_PXMSKC(port));
	writel(1 << pin, GPIO_PXPAT1S(port));
	writel(1 << pin, GPIO_PXPAT0S(port));
}

void gpio_as_irq_fall_edge(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXINTS(port));
	writel(1 << pin, GPIO_PXMSKC(port));
	writel(1 << pin, GPIO_PXPAT1S(port));
	writel(1 << pin, GPIO_PXPAT0C(port));
}

void gpio_ack_irq(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXFLGC(port));
}

void dump_gpio_func( unsigned int gpio);
void gpio_init(void)
{
	int i, n;
	struct jz_gpio_func_def *g;
#ifndef CONFIG_BURNER
	n = ARRAY_SIZE(gpio_func);

	for (i = 0; i < n; i++) {
		g = &gpio_func[i];
		gpio_set_func(g->port, g->func, g->pins);
	}
	g = &uart_gpio_func[CONFIG_SYS_UART_INDEX];
#else
	n = gd->arch.gi->nr_gpio_func;

	for (i = 0; i < n; i++) {
		g = &gd->arch.gi->gpio[i];
		gpio_set_func(g->port, g->func, g->pins);
	}
	g = &uart_gpio_func[gd->arch.gi->uart_idx];
#endif
	gpio_set_func(g->port, g->func, g->pins);

#ifndef CONFIG_SPL_BUILD
#ifdef CONFIG_JZ_PCA953X
	pca953x_init();
#endif
#endif
}
void dump_gpio_func( unsigned int gpio)
{
	unsigned group = gpio / 32;
	unsigned pin = gpio % 32;
	int d = 0;
	unsigned int base = GPIO_BASE + 0x100 * group;
	d = d | ((readl(base + PXINT) >> pin) & 1) << 3;
	d = d | ((readl(base + PXMSK) >> pin) & 1) << 2;
	d = d | ((readl(base + PXPAT1) >> pin) & 1) << 1;
	d = d | ((readl(base + PXPAT0) >> pin) & 1) << 0;
    printf("gpio[%d] fun %x\n",gpio,d);
}
