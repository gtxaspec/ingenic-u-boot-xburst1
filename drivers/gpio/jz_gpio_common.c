/*
 * JZ4775 common routines
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Sonil <ztyan@ingenic.cn>
 * Based on: newxboot/modules/gpio/jz4775_gpio.c|jz4780_gpio.c
 *
 * SPDX-License-Identifier:	GPL-2.0+
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
#elif defined (CONFIG_M150)
#include "jz_gpio/m150_gpio.c"
#elif defined (CONFIG_M200)
#include "jz_gpio/m200_gpio.c"
#elif defined (CONFIG_T10)
#include "jz_gpio/t10_gpio.c"
#elif defined (CONFIG_T15)
#include "jz_gpio/t15_gpio.c"
#elif defined (CONFIG_T20)
#include "jz_gpio/t20_gpio.c"
#elif defined (CONFIG_T21)
#include "jz_gpio/t21_gpio.c"
#elif defined (CONFIG_T23)
#include "jz_gpio/t23_gpio.c"
#elif defined (CONFIG_T30)
#include "jz_gpio/t30_gpio.c"
#elif defined (CONFIG_T31)
#include "jz_gpio/t31_gpio.c"
#elif defined (CONFIG_C100)
#include "jz_gpio/c100_gpio.c"
#endif

DECLARE_GLOBAL_DATA_PTR;

int static inline is_gpio_from_chip(unsigned int gpio_num)
{
	return gpio_num < (GPIO_NR_PORTS * 32) ? 1 : 0;
}

void gpio_set_func(enum gpio_port n, enum gpio_function func, unsigned int pins)
{
#ifdef CONFIG_SYS_UART_CONTROLLER_STEP
	u32 step = CONFIG_SYS_UART_CONTROLLER_STEP;
#else /* default */
	u32 step = 0x100;
#endif
	unsigned int base = GPIO_BASE + step * n;

	writel(func & 0x8? pins : 0, base + PXINTS);
	writel(func & 0x4? pins : 0, base + PXMSKS);
	writel(func & 0x2? pins : 0, base + PXPAT1S);
	writel(func & 0x1? pins : 0, base + PXPAT0S);

	writel(func & 0x8? 0 : pins, base + PXINTC);
	writel(func & 0x4? 0 : pins, base + PXMSKC);
	writel(func & 0x2? 0 : pins, base + PXPAT1C);
	writel(func & 0x1? 0 : pins, base + PXPAT0C);

	writel(func & 0x10? 0 : pins, base + PXPUENC);
	writel(func & 0x10? pins : 0, base + PXPUENS);

	writel(func & 0x20? 0 : pins, base + PXPDENC);
	writel(func & 0x20? pins : 0, base + PXPDENS);
}

void gpio_set_driver_strength(enum gpio_port n, unsigned int pins, int ds)
{
#ifdef CONFIG_SYS_UART_CONTROLLER_STEP
	u32 step = CONFIG_SYS_UART_CONTROLLER_STEP;
#else /* default */
	u32 step = 0x100;
#endif
	unsigned int base = GPIO_BASE + step * n;
	int i = 0;
	for (i = 0; i < sizeof(pins) * 8; i++) {
		if (pins & (0x1 << i)) {
			if (2 == ds) {
				writel(0x3 << ((i % 16) * 2), base + (i < 16 ? PXPDSLC : PXPDSHC));
			} else if (4 == ds) {
				writel(0x1 << ((i % 16) * 2), base + (i < 16 ? PXPDSLS : PXPDSHS));
				writel(0x1 << ((i % 16) * 2 + 1), base + (i < 16 ? PXPDSLC : PXPDSHC));
			} else if (8 == ds) {
				writel(0x1 << ((i % 16) * 2), base + (i < 16 ? PXPDSLC : PXPDSHC));
				writel(0x1 << ((i % 16) * 2 + 1), base + (i < 16 ? PXPDSLS : PXPDSHS));
			} else if (12 == ds) {
				writel(0x3 << ((i % 16) * 2), base + (i < 16 ? PXPDSLS : PXPDSHS));
			}
		}
	}
}

int gpio_request(unsigned gpio, const char *label)
{
	// Redundant print statement, lets silence it for now
	// printf("GPIO:  Requesting GPIO %d = [%s]\n",gpio,label);
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
	if (is_gpio_from_chip(gpio)) {
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
	if (is_gpio_from_chip(gpio)) {
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
	if (is_gpio_from_chip(gpio)) {
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
	if (is_gpio_from_chip(gpio)) {
		gpio_port_direction_output(port, pin, value);
	} else {
#ifdef CONFIG_JZ_PCA953X
		pca953x_direction_output(TO_PCA953X_GPIO(gpio), value);
#endif
	}
	return 0;
}

void gpio_enable_pull_up(unsigned gpio)
{
	unsigned port= gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXPUENS(port));
}

void gpio_disable_pull_up(unsigned gpio)
{
	unsigned port= gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXPUENC(port));
}

void gpio_enable_pull_down(unsigned gpio)
{
	unsigned port= gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXPDENS(port));
}

void gpio_disable_pull_down(unsigned gpio)
{
	unsigned port= gpio / 32;
	unsigned pin = gpio % 32;

	writel(1 << pin, GPIO_PXPDENC(port));
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
		if (g->driver_strength)
			gpio_set_driver_strength(g->port, g->pins, g->driver_strength);
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

#if defined(CONFIG_T23)
	gpio_enable_pull_up(GPIO_PB(0));
	gpio_enable_pull_up(GPIO_PB(1));
	gpio_enable_pull_up(GPIO_PB(2));
	gpio_enable_pull_up(GPIO_PB(3));
	gpio_enable_pull_up(GPIO_PB(5));
#endif
}

void dump_gpio_func(unsigned int gpio)
{
	unsigned group = gpio / 32;
	unsigned pin = gpio % 32;
	int d = 0;
	unsigned int base = GPIO_BASE + 0x100 * group;
	d = d | ((readl(base + PXINT)  >> pin) & 1) << 3;
	d = d | ((readl(base + PXMSK)  >> pin) & 1) << 2;
	d = d | ((readl(base + PXPAT1) >> pin) & 1) << 1;
	d = d | ((readl(base + PXPAT0) >> pin) & 1) << 0;
	printf("gpio[%d] fun %x\n",gpio,d);
}

#define MAX_GPIO_SET_LEN	256  // Define a maximum length for the GPIO settings string

void process_gpio_token(char* token) {
	char *endptr;
	long gpio = simple_strtol(token, &endptr, 10);
	
	// Ignore negative GPIO values (like -1)
	if (gpio < 0) {
		printf(" [Ignoring GPIO %ld]", gpio);
		return;
	}

	// Check if the mode character is present and valid
	char mode = (*endptr) ? *endptr : 'i'; // Default to 'i' (input) if no mode specified

#if defined(CONFIG_T31) || defined(CONFIG_C100)
	bool disablePullUp = false;
	bool disablePullDown = false;

	// Check for additional characters for pull-up/pull-down configuration
	char* ptr = endptr + 1;
	while (*ptr) {
		switch (*ptr) {
			case 'u':
			case 'U':
				disablePullUp = true;
				break;
			case 'd':
		    case 'D':
				disablePullDown = true;
				break;
		}
		ptr++;
	}
#endif

	gpio_request((unsigned)gpio, "gpio_set");
	printf(" ");
	switch (mode) {
		case 'i': // Input
		case 'I': // Also treat uppercase 'I' as Input for consistency
			gpio_direction_input((unsigned)gpio);
			printf("%lui", gpio);
#if defined(CONFIG_T31) || defined(CONFIG_C100)
			if (disablePullUp) {
				gpio_disable_pull_up((unsigned)gpio);
				printf("u");
			}
			if (disablePullDown) {
				gpio_disable_pull_down((unsigned)gpio);
				printf("d");
			}
			#endif
			break;
		case 'o': // Output low
			gpio_direction_output((unsigned)gpio, 0);
			printf("%luo", gpio);
			break;
		case 'O': // Output high
			gpio_direction_output((unsigned)gpio, 1);
			printf("%luO", gpio);
			break;
		default:
			printf("%lu?%c", gpio, mode);
			break;
	}
}

void handle_gpio_settings(const char *env_var_name) {
	if (!env_var_name) {
		printf("GPIO:  Error: gpio_settings called without variable name\n");
		return;
	}

	const char *env_gpio_str = getenv(env_var_name);
	if (!env_gpio_str || *env_gpio_str == '\0') {
		printf("GPIO:  %s: No GPIO env settings provided\n", env_var_name);
		return;
	}

	char gpio_str_copy[MAX_GPIO_SET_LEN];
	strncpy(gpio_str_copy, env_gpio_str, MAX_GPIO_SET_LEN - 1);
	gpio_str_copy[MAX_GPIO_SET_LEN - 1] = '\0';

	char *token = strtok(gpio_str_copy, " ");

	printf("GPIO:  %s:", env_var_name);
	while (token) {
		if (strncmp(token, "gpio", 4) == 0) {
			token = strtok(NULL, " ");
			continue;
		}
		process_gpio_token(token);
		udelay(1000); // Add a delay after setting each GPIO
		token = strtok(NULL, " ");
	}
	printf("\n");
}
