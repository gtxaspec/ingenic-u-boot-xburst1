/*
 * Copyright (C) 2013 Ingenic Semiconductor Inc.
 * Author: Huddy <hyli@ingenic.cn>
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
#include <asm/io.h>
#include <asm/gpio.h>
#include <asm/arch/clk.h>
#include <jz_pwm.h>
//#define DEBUG

#define PWM_BASE 0xb3450000
#define PWMCCFG0  0x000
#define PWMCCFG1  0x004
#define PWMENS    0x010
#define PWMENC    0x014
#define PWMEN     0x018
#define PWMUPDATE 0x020
#define PWMBUSYR  0x024
#define PWMLVL    0x030
#define PWMWCFG   0x0B0
#define PWMDES    0x100
#define PWMDEC    0x104
#define PWMDE     0x108
#define PWMDCR0   0x110
#define PWMDCR1   0x114
#define PWMDTRIG  0x120
#define PWMDFER   0x124
#define PWMDFSM   0x128
#define PWMDSR    0x130
#define PWMDSCR   0x134
#define PWMDINTC  0x138
#define PWMnDAR   0x140
#define PWMnDTLR  0x190
#define PWMOEN    0x300

#define CLK_1 0
#define CLK_2 1
#define CLK_4 2
#define CLK_8 3
#define CLK_16 4
#define CLK_32 5
#define CLK_64 6
#define CLK_128 7
#define CLK_n CLK_1

#define PWM_CLK_RATE 50000000

struct jz_gpio_func_def pwm_pins[PWM_CHN_NUM] = {
	// [0] = {.port = GPIO_PORT_A, .func = GPIO_FUNC_3, .pins = 17},
	// [0] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins =17 },
	// [0] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_2, .pins =30 },
	[0] = {.port = GPIO_PORT_C, .func = GPIO_FUNC_2, .pins =8 },
	// [0] = {.port = GPIO_PORT_C, .func = GPIO_FUNC_0, .pins =25 },

	// [1] = {.port = GPIO_PORT_A, .func = GPIO_FUNC_3, .pins = 19 },
	// [1] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins = 18 },
	// [1] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_1, .pins = 26 },
	[1] = {.port = GPIO_PORT_C, .func = GPIO_FUNC_1, .pins = 29},

	// [2] = {.port = GPIO_PORT_A, .func = GPIO_FUNC_3, .pins = 15 },
	// [2] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_2, .pins = 11 },
	// [2] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_1, .pins = 27 },
	[2] = {.port = GPIO_PORT_C, .func = GPIO_FUNC_2, .pins = 9},

	[3] = {.port = GPIO_PORT_A, .func = GPIO_FUNC_3, .pins =14 },
	// [3] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_2, .pins =13 },
	// [3] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_1, .pins =28 },
	// [3] = {.port = GPIO_PORT_B, .func = GPIO_FUNC_0, .pins =31 },
};

struct ingenic_pwm_chip
{
	unsigned int clk100k;
	unsigned int period_ns_max;
	unsigned int period_ns_min;
};
struct ingenic_pwm_chip ingenic_pwm;

struct pwm_chn_config
{
	unsigned int period_ns;
	unsigned int duty_ns;
	unsigned char polarity;
	unsigned char pwm_chn_en;
};
struct pwm_chn_config pwm_conf[PWM_CHN_NUM];

int pwm_init(void)
{
	int n;
	ingenic_pwm.clk100k = PWM_CLK_RATE / (1 << CLK_n) / 100000; // unit:100KHz
	ingenic_pwm.period_ns_max = 10000 * 0xffff / ingenic_pwm.clk100k;
	ingenic_pwm.period_ns_min = 10000 * 2 / ingenic_pwm.clk100k + 1;
	printf("period_ns:max=%u,min=%u\n", ingenic_pwm.period_ns_max, ingenic_pwm.period_ns_min);
	for (n = 0; n < PWM_CHN_NUM; ++n)
	{
		pwm_conf[n].pwm_chn_en = 0;
		pwm_conf[n].polarity = 1;
		pwm_conf[n].period_ns = ingenic_pwm.period_ns_max;
		pwm_conf[n].duty_ns=0;
	}
	clk_set_rate(PWM, PWM_CLK_RATE);
	writel(0, PWM_BASE + PWMDCR0);
	writel(0, PWM_BASE + PWMDINTC);
	return 0;
}

//channel: 0-7
int pwm_enable(unsigned int channel)
{
	unsigned int pwm_reg;
	if (channel > PWM_CHN_NUM-1)
		return -1;

	pwm_reg = readl(PWM_BASE + PWMCCFG0);
	pwm_reg = pwm_reg & (~(0x0f << 4 * channel));
	pwm_reg = pwm_reg | (CLK_n << 4 * channel);
	writel(pwm_reg, PWM_BASE + PWMCCFG0);

	pwm_config(channel, pwm_conf[channel].duty_ns, pwm_conf[channel].period_ns);
	writel(1 << channel, PWM_BASE + PWMENS);

	pwm_reg = readl(PWM_BASE + PWMOEN);
	pwm_reg |= (1 << channel);
	writel(pwm_reg, PWM_BASE + PWMOEN);
	pwm_conf[channel].pwm_chn_en = 1;

	gpio_set_func(pwm_pins[channel].port, pwm_pins[channel].func, 1 << pwm_pins[channel].pins);
	return 0;
}

int pwm_config(unsigned int channel, int duty_ns, int period_ns)
{
	unsigned int high, low, period, pwm_reg;
	if (channel > PWM_CHN_NUM - 1)
		return -1;
	if (period_ns > ingenic_pwm.period_ns_max || period_ns < ingenic_pwm.period_ns_min || duty_ns > period_ns)
	{
		printf("%s:Invalid argument\n",__func__);
		return -1;
	}
	pwm_conf[channel].period_ns = period_ns;
	pwm_conf[channel].duty_ns = duty_ns;

	period = (ingenic_pwm.clk100k * period_ns + 5000) / 10000;
	if(pwm_conf[channel].polarity)
	{
		high = (ingenic_pwm.clk100k * duty_ns + 5000) / 10000;
		low = period - high;
	}
	else
	{
		low = (ingenic_pwm.clk100k * duty_ns + 5000) / 10000;
		high = period - low;
	}
#ifdef DEBUG
	printf("high=%u,low=%u\n", high, low);
#endif
	if (high == 0)
	{
		pwm_reg = readl(PWM_BASE + PWMLVL);
		pwm_reg &= ~(1 << channel);
		writel(pwm_reg, PWM_BASE + PWMLVL);
	}
	else if (low == 0)
	{
		pwm_reg = readl(PWM_BASE + PWMLVL);
		pwm_reg |= (1 << channel);
		writel(pwm_reg, PWM_BASE + PWMLVL);
	}
#if 1
	if (pwm_conf[channel].pwm_chn_en)
	{
		while (readl(PWM_BASE + PWMBUSYR) & (1 << channel));
	}
#endif
	writel((high << 16) | low, PWM_BASE + PWMWCFG + 4 * channel);
	if (pwm_conf[channel].pwm_chn_en)
		writel(1 << channel, PWM_BASE + PWMUPDATE);
	return 0;
}

int pwm_disable(unsigned int channel)
{
	unsigned int pwm_reg;
	if (channel > PWM_CHN_NUM - 1)
		return -1;
	pwm_conf[channel].pwm_chn_en = 0;

	writel(1 << channel, PWM_BASE + PWMENC);
	pwm_reg = readl(PWM_BASE + PWMOEN);
	pwm_reg &= ~(1 << channel);
	writel(pwm_reg, PWM_BASE + PWMOEN);

	gpio_port_direction_output(pwm_pins[channel].port, pwm_pins[channel].pins, pwm_conf[channel].polarity ? 0:1);
	return 0;
}

int pwm_set_polarity(unsigned int channel, unsigned char polarity)
{
	if (channel > PWM_CHN_NUM - 1)
		return -1;
	pwm_conf[channel].polarity = !!polarity;
	return 0;
}
