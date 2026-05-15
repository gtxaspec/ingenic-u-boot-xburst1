/*
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
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

#ifndef __PWM_H__
#define __PWM_H__

#include <asm/arch/base.h>

enum pwm_clk{
	EXTAL,
	RTCCLK,
	PCLK,
};

enum pwm_mode{
	CPU_MODE,
	DMA_MODE,
};

struct pwm {
	int channels;
	int div;
	enum pwm_clk pwm_clock;
	int full_data;
	int half_data;
};

/*struct pwm pwm_backlight;*/

/*************************************************************************
 * PWM (Timer Counter Unit)
 *************************************************************************/
#define GENMASK(hi, lo)   (((1ULL << ((hi) - (lo) + 1)) - 1) << (lo))
#define	PWM_ENS		(0x00)
#define	PWM_ENC		(0x04)
#define	PWM_EN		(0x08)
#define	PWM_UPT		(0x10)
#define	PWM_BUSY	(0x14)
#define	PWM_MS		(0x20)
#define PWM_INL		(0x24)
#define PWM_IDL		(0x28)
#define PWM_CCFG0	(0x40)
#define PWM_WCFG0	(0x80)
#define PWM_DR0		(0xc0)
#define PWM_DFN0	(0x100)
#define PWM_DRTN0	(0x140)
#define PWM_DRE		(0x180)
#define PWM_DRS		(0x184)
#define PWM_DFIE	(0x188)
#define PWM_DFS		(0x18c)
#define PWM_DAFF	(0x190)
#define PWM_DCFF	(0x194)
#define PWM_SS		(0x200)
#define PWM_SIE		(0x204)
#define PWM_SC0		(0x210)
#define PWM_SN0		(0x250)
#define PWM_ON0		(0x290)
#define PWM_CCFG(n)	(PWM_CCFG0 + 4*n)
#define PWM_WCFG(n)	(PWM_WCFG0 + 4*n)
#define PWM_DR(n)	(PWM_DR0 + 4*n)
#define PWM_DFN(n)	(PWM_DFN0 + 4*n)
#define PWM_DRTN(n)	(PWM_DRTN0 + 4*n)
#define PWM_SC(n)	(PWM_SC0 + 4*n)
#define PWM_SN(n)	(PWM_SN0 + 4*n)
#define PWM_ON(n)	(PWM_ON0 + 4*n)

#define PWM_WCFG_HIGH		(16)
#define PWM_WCFG_LOW		(0)

#define PWM_STORE_SE	BIT(18)
#define PWM_STORE_SPE	BIT(17)
#define PWM_STORE_SNE	BIT(16)
#define PWM_STORE_SFN_LBIT	0
#define PWM_STORE_SFN_HBIT	9
#define PWM_STORE_SFN_MASK	\
	GENMASK(PWM_STORE_SFN_HBIT, PWM_STORE_SFN_LBIT)

int pwm_request(int num);
void pwm_enable(int num);
void pwm_disable(int num);
void pwm_set_init_level(int num,int value);
void pwm_set_finish_level(int num,int value);
void pwm_config(int num,int div,int full_data,int half_data);
void pwm_init(struct pwm *pwm_data);

#endif /* __PWM_H__ */
