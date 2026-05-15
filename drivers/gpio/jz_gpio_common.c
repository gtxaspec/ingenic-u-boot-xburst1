// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <common.h>
#include <config.h>
#include <asm/io.h>
#include <asm/arch/gpio.h>

#if defined (CONFIG_PRJ)
#define JZGPIO_GROUP_OFFSET     (0x1000)
#include "jz_gpio/PRJ_gpio.c"
#endif

DECLARE_GLOBAL_DATA_PTR;

static inline int is_gpio_from_chip(int gpio_num)
{
	return gpio_num < (GPIO_NR_PORTS * 32) ? 1 : 0;
}


#if defined(PXPEL) && defined(PXPEH)
#define BIT(nr)         (1UL << (nr))
void gpio_set_driver_state(enum gpio_port n, unsigned int pins, unsigned int state)
{
	void __iomem *base = (void __iomem *)(GPIO_BASE + JZGPIO_GROUP_OFFSET * n);
	unsigned int tmp = pins;
	unsigned int val = 0;
	unsigned int pin = 0;
	if (tmp & 0xffff) {
		val = readl(base + PXPEL);
		while (!!(pin = fls((tmp & 0xffff)))) {
			pin = pin - 1;
			tmp &= ~(BIT(pin));
			val |= state << (pin << 1);
		}
		writel(val, base + PXPEL);
	}
	if (tmp & 0xffff0000) {
		val = readl(base + PXPEH);
		while (!!(pin = fls((tmp)))) {
			pin = pin - 1;
			tmp &= ~(BIT(pin));
			val |= state << ((pin - 16) << 1);
		}
		writel(val, base + PXPEH);
	}
}
#endif

#if defined(PXDSL) && defined(PXDSH)
void gpio_set_driver_strength(enum gpio_port n, unsigned int pins, unsigned int ds)
{
	void __iomem *base = (void __iomem *)(GPIO_BASE + JZGPIO_GROUP_OFFSET * n);
	unsigned int tmp = pins;
	unsigned int val = 0;
	unsigned int pin = 0;
	if (tmp & 0xffff) {
		val = readl(base + PXDSL);
		while (!!(pin = fls((tmp & 0xffff)))) {
			pin = pin - 1;
			tmp &= ~(BIT(pin));
			val |= ds << (pin << 1);
		}
		writel(val, base + PXDSL);
	}
	if (tmp & 0xffff0000) {
		val = readl(base + PXDSH);
		while (!!(pin = fls((tmp)))) {
			pin = pin - 1;
			tmp &= ~(BIT(pin));
			val |= ds << ((pin - 16) << 1);
		}
		writel(val, base + PXDSH);
	}
}
#endif


void gpio_set_func(enum gpio_port n, enum gpio_function func, unsigned int pins)
{
	void __iomem *base = (void __iomem *)(GPIO_BASE + JZGPIO_GROUP_OFFSET * n);
	writel(func & 0x8 ? pins : 0, base + PXINTS);
	writel(func & 0x4 ? pins : 0, base + PXMSKS);
	writel(func & 0x2 ? pins : 0, base + PXPAT1S);
	writel(func & 0x1 ? pins : 0, base + PXPAT0S);
	writel(func & 0x8 ? 0 : pins, base + PXINTC);
	writel(func & 0x4 ? 0 : pins, base + PXMSKC);
	writel(func & 0x2 ? 0 : pins, base + PXPAT1C);
	writel(func & 0x1 ? 0 : pins, base + PXPAT0C);
#if defined(PXPES) && defined(PXPEC) && defined(PXPE)
	writel(func & 0x10 ? pins : 0, base + PXPES);
	writel(func & 0x10 ? 0 : pins, base + PXPEC);
#elif defined(PXPU) && defined(PXPUC) && defined(PXPUS)
	writel(func & 0x10 ? pins : 0, base + PXPUS);
	writel(func & 0x10 ? 0 : pins, base + PXPUC);
#elif defined(PXPEL) && defined(PXPEH) && defined(PXPE_PULLUP)
	gpio_set_driver_state(n, pins, PXPE_PULLUP);
#endif
#if 0 //defined(PXDSL) && defined(PXDSH)
	gpio_set_driver_strength(n, pins, PXDS_8mA);
#endif
}

#if !CONFIG_IS_ENABLED(DM_GPIO)
int gpio_request(unsigned gpio, const char *label)
{
	printf("%s lable = %s gpio = %d\n", __func__, label, gpio);
	return gpio;
}

int gpio_free(unsigned gpio)
{
	return 0;
}
#endif

void gpio_port_set_value(int port, int pin, int value)
{
	if (value)
		writel(1 << pin, (void __iomem *)GPIO_PXPAT0S(port));
	else
		writel(1 << pin, (void __iomem *)GPIO_PXPAT0C(port));
}

void gpio_port_direction_input(int port, int pin)
{
	writel(1 << pin, (void __iomem *)GPIO_PXINTC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1S(port));
}

void gpio_port_direction_output(int port, int pin, int value)
{
	writel(1 << pin, (void __iomem *)GPIO_PXINTC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1C(port));
	gpio_port_set_value(port, pin, value);
}

#if !CONFIG_IS_ENABLED(DM_GPIO)
int gpio_set_value(unsigned gpio, int value)
{
	int port = gpio / 32;
	int pin = gpio % 32;
	if (is_gpio_from_chip(gpio)) {
		gpio_port_set_value(port, pin, value);
	}

	return 0;
}

int gpio_get_value(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	if (is_gpio_from_chip(gpio)) {
		return !!(readl((void __iomem *)GPIO_PXPIN(port)) & (1 << pin));
	}

	return 0;
}
#endif

int gpio_get_flag(unsigned int gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	return (readl((void __iomem *)GPIO_PXFLG(port)) & (1 << pin));
}

int gpio_clear_flag(unsigned gpio)
{
	int port = gpio / 32;
	int pin = gpio % 32;
	writel(1 << pin, (void __iomem *)GPIO_PXFLGC(port));
	return 0;
}

#if !CONFIG_IS_ENABLED(DM_GPIO)
int gpio_direction_input(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	if (is_gpio_from_chip(gpio)) {
		gpio_port_direction_input(port, pin);
	}

	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	if (is_gpio_from_chip(gpio)) {
		gpio_port_direction_output(port, pin, value);
	}

	return 0;
}
#endif

void gpio_enable_pull(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
#if defined(PXPES) && defined(PXPEC) && defined(PXPE)
	writel(1 << pin, (void __iomem *)GPIO_PXPEC(port));
#elif defined(PXPU) && defined(PXPUC) && defined(PXPUS)
	writel(1 << pin, (void __iomem *)GPIO_PXPUC(port));
#elif defined(PXPEL) && defined(PXPEH) && defined(PXPE_PULLUP)
	gpio_set_driver_state(port, pins, PXPE_PULLUP);
#else
	printf("ERROR: %s nothing!\n",__func__);
#endif
}

void gpio_disable_pull(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
#if defined(PXPES) && defined(PXPEC) && defined(PXPE)
	writel(1 << pin, (void __iomem *)GPIO_PXPES(port));
#elif defined(PXPU) && defined(PXPUC) && defined(PXPUS)
	writel(1 << pin, (void __iomem *)GPIO_PXPUS(port));
#elif defined(PXPEL) && defined(PXPEH) && defined(PXPE_PULLUP)
	gpio_set_driver_state(port, pin, PXPE_PULLHZ);
#else
	printf("ERROR: %s nothing!\n",__func__);
#endif
}

void gpio_as_irq_high_level(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	writel(1 << pin, (void __iomem *)GPIO_PXINTS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1C(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT0S(port));
}

void gpio_as_irq_low_level(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	writel(1 << pin, (void __iomem *)GPIO_PXINTS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1C(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT0C(port));
}

void gpio_as_irq_rise_edge(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	writel(1 << pin, (void __iomem *)GPIO_PXINTS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1S(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT0S(port));
}

void gpio_as_irq_fall_edge(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	writel(1 << pin, (void __iomem *)GPIO_PXINTS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1S(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT0C(port));
}

void gpio_ack_irq(unsigned gpio)
{
	unsigned port = gpio / 32;
	unsigned pin = gpio % 32;
	writel(1 << pin, (void __iomem *)GPIO_PXFLGC(port));
}

void dump_gpio_func(unsigned int gpio);
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
	//	g = &uart_gpio_func[gd->arch.gi->uart_idx];
	g = &gd->arch.gi->uart_gpio[0];
#endif
	gpio_set_func(g->port, g->func, g->pins);
}

void dump_gpio_func(unsigned int gpio)
{
	unsigned group = gpio / 32;
	unsigned pin = gpio % 32;
	int d = 0;
	void __iomem *base = (void __iomem *)(GPIO_BASE + JZGPIO_GROUP_OFFSET * group);
	d = d | ((readl(base + PXINT) >> pin) & 1) << 3;
	d = d | ((readl(base + PXMSK) >> pin) & 1) << 2;
	d = d | ((readl(base + PXPAT1) >> pin) & 1) << 1;
	d = d | ((readl(base + PXPAT0) >> pin) & 1) << 0;
	printf("gpio[%d] fun %x\n", gpio, d);
}
