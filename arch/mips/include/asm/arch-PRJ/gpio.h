/*
 * T5 GPIO definitions
 *
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

#ifndef __GPIO_H__
#define __GPIO_H__

#include <asm/arch/base.h>

#define GPIO_PA(n) 	(0*32 + n)
#define GPIO_PB(n) 	(1*32 + n)
#define GPIO_PC(n) 	(2*32 + n)
#define GPIO_PD(n) 	(3*32 + n)
#define GPIO_PE(n) 	(4*32 + n)
#define GPIO_PF(n) 	(5*32 + n)
#define GPIO_PG(n) 	(6*32 + n)

enum gpio_function {
	GPIO_FUNC_0     = 0x00,  //0000, GPIO as function 0 / device 0
	GPIO_FUNC_1     = 0x01,  //0001, GPIO as function 1 / device 1
	GPIO_FUNC_2     = 0x02,  //0010, GPIO as function 2 / device 2
	GPIO_FUNC_3     = 0x03,  //0011, GPIO as function 3 / device 3
	GPIO_OUTPUT0    = 0x04,  //0100, GPIO as output0
	GPIO_OUTPUT1    = 0x05,  //0101, GPIO as output1
	GPIO_INPUT	= 0x06,	 //0110, GPIO as input
};

enum gpio_port {
	GPIO_PORT_A,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D,
	GPIO_PORT_E,
	GPIO_PORT_F,
	GPIO_PORT_G,
	/* this must be last */
	GPIO_NR_PORTS,
};

enum gpio_pull {
	GPIO_PULL_UP,  //Pull Up
	GPIO_PULL_DOWN,//Pull Down
	GPIO_DISABLE_PULL,//Disable UP and Down
};

struct jz_gpio_func_def {
	int port;
	int func;
	unsigned long pins;
};

/*************************************************************************
 * GPIO (General-Purpose I/O Ports)
 *************************************************************************/
#define MAX_GPIO_NUM	192

#define PXPIN		0x00
#define PXINT		0x10
#define PXINTS		0x14
#define PXINTC		0x18
#define PXMSK		0x20
#define PXMSKS		0x24
#define PXMSKC		0x28
#define PXPAT1		0x30
#define PXPAT1S		0x34
#define PXPAT1C		0x38
#define PXPAT0		0x40
#define PXPAT0S		0x44
#define PXPAT0C		0x48
#define PXFLG		0x50
#define PXFLGC		0x58
#define PXGFCFG0	0x70
#define PXGFCFG0S	0x74
#define PXGFCFG0C	0x78
#define PXGFCFG1	0x80
#define PXGFCFG1S	0x84
#define PXGFCFG1C	0x88
#define PXGFCFG2	0x90
#define PXGFCFG2S	0x94
#define PXGFCFG2C	0x98
#define PXGFCFG3	0xa0
#define PXGFCFG3S	0xa4
#define PXGFCFG3C	0xa8
#define PXVDDIO		0x100
#define PXVDDIOS	0x104
#define PXVDDIOC	0x108
#define PXPUEN		0x110
#define PXPUENS		0x114
#define PXPUENC		0x118
#define PXPDEN		0x120
#define PXPDENS		0x124
#define PXPDENC		0x128
#define PXDSL		0x130
#define PXDSLS		0x134
#define PXDSLC		0x138
#define PXDSH		0x140
#define PXDSHS		0x144
#define PXDSHC		0x148
#define PXSR		0x150
#define PXSRS		0x154
#define PXSRC		0x158
#define PXSMT		0x160
#define PXSMTS		0x164
#define PXSMTC		0x168
#define PXIE		0x180
#define PXIES		0x184
#define PXIEC		0x188

/*shadow reg*/
#define PXSHD       0x1d0
#define PXSHDS      0X1d4
#define PXSHDC      0X1d8
#define PXUPD       0X1e4

/*************************************************************************
 * GPIO (General-Purpose I/O Ports)
 *************************************************************************/
//n = 0,1,2,7
#define GPIO_PXPIN(n)		(GPIO_BASE + (0x00 + (n)*0x1000)) /* PIN Level Register */
#define GPIO_PXINT(n)		(GPIO_BASE + (0x10 + (n)*0x1000)) /* PORT Interrupt Register */
#define GPIO_PXINTS(n)		(GPIO_BASE + (0x14 + (n)*0x1000)) /* PORT Interrupt Set Register */
#define GPIO_PXINTC(n)		(GPIO_BASE + (0x18 + (n)*0x1000)) /* PORT Interrupt Clear Register */
#define GPIO_PXMSK(n)		(GPIO_BASE + (0x20 + (n)*0x1000)) /* PORT Interrupt Mask Register */
#define GPIO_PXMSKS(n)		(GPIO_BASE + (0x24 + (n)*0x1000)) /* PORT Interrupt Mask Set Reg */
#define GPIO_PXMSKC(n)		(GPIO_BASE + (0x28 + (n)*0x1000)) /* PORT Interrupt Mask Clear Reg */
#define GPIO_PXPAT1(n)		(GPIO_BASE + (0x30 + (n)*0x1000)) /* PORT Pattern 1 Register */
#define GPIO_PXPAT1S(n)		(GPIO_BASE + (0x34 + (n)*0x1000)) /* PORT Pattern 1 Set Reg. */
#define GPIO_PXPAT1C(n)		(GPIO_BASE + (0x38 + (n)*0x1000)) /* PORT Pattern 1 Clear Reg. */
#define GPIO_PXPAT0(n)		(GPIO_BASE + (0x40 + (n)*0x1000)) /* PORT Pattern 0 Register */
#define GPIO_PXPAT0S(n)		(GPIO_BASE + (0x44 + (n)*0x1000)) /* PORT Pattern 0 Set Register */
#define GPIO_PXPAT0C(n)		(GPIO_BASE + (0x48 + (n)*0x1000)) /* PORT Pattern 0 Clear Register */
#define GPIO_PXFLG(n)		(GPIO_BASE + (0x50 + (n)*0x1000)) /* PORT Flag Register */
#define GPIO_PXFLGC(n)		(GPIO_BASE + (0x58 + (n)*0x1000)) /* PORT Flag clear Register */
#define GPIO_PXGFCFG0(n)	(GPIO_BASE + (0x70 + (n)*0x1000)) /* PORT Glitch Filter Configure 0 */
#define GPIO_PXGFCFG0S(n)	(GPIO_BASE + (0x74 + (n)*0x1000)) /* PORT Glitch Filter Configure 0 Set */
#define GPIO_PXGFCFG0C(n)	(GPIO_BASE + (0x78 + (n)*0x1000)) /* PORT Glitch Filter Configure 0 Clear */
#define GPIO_PXGFCFG1(n)	(GPIO_BASE + (0x80 + (n)*0x1000)) /* PORT Glitch Filter Configure 1 */
#define GPIO_PXGFCFG1S(n)	(GPIO_BASE + (0x84 + (n)*0x1000)) /* PORT Glitch Filter Configure 1 Set */
#define GPIO_PXGFCFG1C(n)	(GPIO_BASE + (0x88 + (n)*0x1000)) /* PORT Glitch Filter Configure 1 Clear */
#define GPIO_PXGFCFG2(n)	(GPIO_BASE + (0x90 + (n)*0x1000)) /* PORT Glitch Filter Configure 2 */
#define GPIO_PXGFCFG2S(n)	(GPIO_BASE + (0x94 + (n)*0x1000)) /* PORT Glitch Filter Configure 2 Set */
#define GPIO_PXGFCFG2C(n)	(GPIO_BASE + (0x98 + (n)*0x1000)) /* PORT Glitch Filter Configure 2 Clear */
#define GPIO_PXGFCFG3(n)	(GPIO_BASE + (0xa0 + (n)*0x1000)) /* PORT Glitch Filter Configure 3 */
#define GPIO_PXGFCFG3S(n)	(GPIO_BASE + (0xa4 + (n)*0x1000)) /* PORT Glitch Filter Configure 3 Set */
#define GPIO_PXGFCFG3C(n)	(GPIO_BASE + (0xa8 + (n)*0x1000)) /* PORT Glitch Filter Configure 3 Clear */
#define GPIO_PXVDDIO(n)		(GPIO_BASE + (0x100 + (n)*0x1000)) /* PORT VDDIO-SEL Register */
#define GPIO_PXVDDIOS(n)	(GPIO_BASE + (0x104 + (n)*0x1000)) /* PORT VDDIO-SEL Set Register */
#define GPIO_PXVDDIOC(n)	(GPIO_BASE + (0x108 + (n)*0x1000)) /* PORT VDDIO-SEL Clear Register */
#define GPIO_PXPUEN(n)		(GPIO_BASE + (0x110 + (n)*0x1000)) /* PORT Driver Disabled State Register0 */
#define GPIO_PXPUENS(n)		(GPIO_BASE + (0x114 + (n)*0x1000)) /* PORT Driver Disabled State Set Register0 */
#define GPIO_PXPUENC(n)		(GPIO_BASE + (0x118 + (n)*0x1000)) /* PORT Driver Disabled State Clear Register0 */
#define GPIO_PXPDEN(n)		(GPIO_BASE + (0x120 + (n)*0x1000)) /* PORT Driver Disabled State Register1 */
#define GPIO_PXPDENS(n)		(GPIO_BASE + (0x124 + (n)*0x1000)) /* PORT Driver Disabled State Set Register1 */
#define GPIO_PXPDENC(n)		(GPIO_BASE + (0x128 + (n)*0x1000)) /* PORT Driver Disabled State Clear Register1 */
#define GPIO_PXDSL(n)		(GPIO_BASE + (0x130 + (n)*0x1000)) /* PORT Drive Strength Register0 */
#define GPIO_PXDSLS(n)		(GPIO_BASE + (0x134 + (n)*0x1000)) /* PORT Drive Strength Set Register0 */
#define GPIO_PXDSLC(n)		(GPIO_BASE + (0x138 + (n)*0x1000)) /* PORT Drive Strength Clear Register0 */
#define GPIO_PXDSH(n)		(GPIO_BASE + (0x140 + (n)*0x1000)) /* PORT Drive Strength Register1 */
#define GPIO_PXDSHS(n)		(GPIO_BASE + (0x144 + (n)*0x1000)) /* PORT Drive Strength Set Register1 */
#define GPIO_PXDSHC(n)		(GPIO_BASE + (0x148 + (n)*0x1000)) /* PORT Drive Strength Clear Register1 */
#define GPIO_PXSR(n)		(GPIO_BASE + (0x150 + (n)*0x1000)) /* PORT Slew Rate Register */
#define GPIO_PXSRS(n)		(GPIO_BASE + (0x154 + (n)*0x1000)) /* PORT Slew Rate Set Register */
#define GPIO_PXSRC(n)		(GPIO_BASE + (0x158 + (n)*0x1000)) /* PORT Slew Rate Clear Register */
#define GPIO_PXSMT(n)		(GPIO_BASE + (0x160 + (n)*0x1000)) /* PORT Schmitt Trigger Register */
#define GPIO_PXSMTS(n)		(GPIO_BASE + (0x164 + (n)*0x1000)) /* PORT Schmitt Trigger Set Register */
#define GPIO_PXSMTC(n)		(GPIO_BASE + (0x168 + (n)*0x1000)) /* PORT Schmitt Trigger Clear Register */
#define GPIO_PXIE(n)		(GPIO_BASE + (0x180 + (n)*0x1000)) /* PORT Receiver enable state Register */
#define GPIO_PXIES(n)		(GPIO_BASE + (0x184 + (n)*0x1000)) /* PORT Receiver enable state Set Register */
#define GPIO_PXIEC(n)		(GPIO_BASE + (0x188 + (n)*0x1000)) /* PORT Receiver enable state Clear Register */
/*shadow reg*/
#define GPIO_PXSHD(n)		(GPIO_BASE + (0x1d0 + (n)*0x1000))
#define GPIO_PXSHDS(n)		(GPIO_BASE + (0x1d4 + (n)*0x1000))
#define GPIO_PXSHDC(n)		(GPIO_BASE + (0x1d8 + (n)*0x1000))
#define GPIO_PXUPD(n)		(GPIO_BASE + (0x1e4 + (n)*0x1000))

#define REG_GPIO_PXPIN(n)	REG32(GPIO_PXPIN((n)))  /* PIN level */
/* 0:as device functions or normal gpio, 1:interrupt */
#define REG_GPIO_PXINT(n)	REG32(GPIO_PXINT((n)))
#define REG_GPIO_PXINTS(n)	REG32(GPIO_PXINTS((n)))
#define REG_GPIO_PXINTC(n)	REG32(GPIO_PXINTC((n)))
/*
 * When INT = 1: 0: Enable the pin as an interrupt source, 1: Disable the pin as an interrupt source
 * When INT = 0: 0: Corresponding pin will be used as device function, 1: Corresponding pin will be used as gpio
 */
#define REG_GPIO_PXMSK(n)	REG32(GPIO_PXMSK((n)))
#define REG_GPIO_PXMSKS(n)	REG32(GPIO_PXMSKS((n)))
#define REG_GPIO_PXMSKC(n)	REG32(GPIO_PXMSKC((n)))
/*
 * When INT = 1 (Interrupt function): 0: Level trigger interrupt, 1: Edge trigger interrupt
 * When INT = 0 and MSK = 0 (Device function): 0: Corresponding pin is used as device 0 or device 1 function,
 *                                             1: Corresponding pin is used as device 2 or device 3 function
 * When INT n = 0 and MSK = 0 (GPIO function): 0: Corresponding pin is used as gpio output
 *                                             1: Corresponding pin is used as gpio input
 */
#define REG_GPIO_PXPAT1(n)	REG32(GPIO_PXPAT1((n)))
#define REG_GPIO_PXPAT1S(n)	REG32(GPIO_PXPAT1S((n)))
#define REG_GPIO_PXPAT1C(n)	REG32(GPIO_PXPAT1C((n)))
/*
 * When INT = 1 and PAT1 = 0:0: Port is low level triggered interrupt input; 1: Port is low high triggered interrupt input
 * When INT = 1 and PAT1 = 1:0: Port is falling edge triggered interrupt input; 1: Port is rising edge triggered interrupt input
 * When INT = 0 and MSK = 0 and PAT1 = 0: 0: Port is pin of device 0; 1: Port is pin of device 1
 * When INT = 0 and MSK = 0 and PAT1 = 1: 0: Port is pin of device 2; 1: Port is pin of device 3
 * When INT = 0 and MSK = 1 and PAT1 = 0: 0: Port is GPIO output 0; 1: Port is GPIO output 1
 */
#define REG_GPIO_PXPAT0(n)	REG32(GPIO_PXPAT0((n)))
#define REG_GPIO_PXPAT0S(n)	REG32(GPIO_PXPAT0S((n)))
#define REG_GPIO_PXPAT0C(n)	REG32(GPIO_PXPAT0C((n)))
/* 0: the interrupt not happen; 1: the interrupt happened */
#define REG_GPIO_PXFLG(n)	REG32(GPIO_PXFLG((n))) /* 0:GPIO/Fun0,1:intr/fun1*/
#define REG_GPIO_PXFLGC(n)	REG32(GPIO_PXFLGC((n)))

/* 00: Hi-Z; 01: Pull-up; 10: Pull-down; 11: Repeater */
#define REG_GPIO_PXPEL(n)	REG32(GPIO_PXPEL((n)))
#define REG_GPIO_PXPELS(n)	REG32(GPIO_PXPELS((n)))
#define REG_GPIO_PXPELC(n)	REG32(GPIO_PXPELC((n)))
#define REG_GPIO_PXPEH(n)	REG32(GPIO_PXPEH((n)))
#define REG_GPIO_PXPEHS(n)	REG32(GPIO_PXPEHS((n)))
#define REG_GPIO_PXPEHC(n)	REG32(GPIO_PXPEHC((n)))

//shadow
#define REG_GPIO_PZINTS		REG32(GPIO_PXINTS((7)))
#define REG_GPIO_PZINTC		REG32(GPIO_PXINTC((7)))
#define REG_GPIO_PZMSKS		REG32(GPIO_PXMSKS((7)))
#define REG_GPIO_PZMSKC		REG32(GPIO_PXMSKC((7)))
#define REG_GPIO_PZPAT1S	REG32(GPIO_PXPAT1S((7)))
#define REG_GPIO_PZPAT1C	REG32(GPIO_PXPAT1C((7)))
#define REG_GPIO_PZPAT0S	REG32(GPIO_PXPAT0S((7)))
#define REG_GPIO_PZPAT0C	REG32(GPIO_PXPAT0C((7)))
#define REG_GPIO_PZGID2LD	REG32(GPIO_BASE + 0x70f0) /* GPIOZ Group ID to Load Register */

void gpio_set_func(enum gpio_port n, enum gpio_function func, unsigned int pins);
void gpio_port_set_value(int port, int pin, int value);
void gpio_port_direction_input(int port, int pin);
void gpio_port_direction_output(int port, int pin, int value);
void gpio_set_driver_strength(unsigned gpio, unsigned lev);
void gpio_init(void);
void gpio_set_pull(unsigned gpio, enum gpio_pull pull);
void gpio_as_irq_high_level(unsigned gpio);
void gpio_as_irq_low_level(unsigned gpio);
void gpio_as_irq_rise_edge(unsigned gpio);
void gpio_as_irq_fall_edge(unsigned gpio);
void gpio_ack_irq(unsigned gpio);

#endif /* __GPIO_H__ */
