// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __CLK_H__
#define __CLK_H__

#include <asm/arch/base.h>

enum clk_id {
	DDR,
	HELIX,
	MACPHY,
	LCD,
//	OTG,
	I2S,
	MSC,
	MSC0 = MSC,
	MSC1,
	SFC0,
	SFC1,
	PWM,
	SSI,
	CIM,
	ISP,
	CGU_CNT,
	CPU = CGU_CNT,
	H2CLK,
	APLL,
	MPLL,
	VPLL,
	EXCLK,
	USBPHY,
};

struct clk_cgu_setting {
	unsigned int addr;
	unsigned int val;
	unsigned ce:8;
	unsigned busy:8;
	unsigned stop:8;
	unsigned sel_src:8;
	unsigned sel_val;
};

#define SRC_EOF -1

void clk_prepare(void);
unsigned int _clk_get_rate(int clk);
void _clk_set_rate(int clk, unsigned long rate);
void clk_init(void);
void enable_uart_clk(void);
enum otg_mode_t {
	OTG_MODE = 0,
	DEVICE_ONLY_MODE,
	HOST_ONLY_MODE,
};
void otg_phy_init(enum otg_mode_t mode,unsigned extclk);
#endif /* __CLK_H__ */
