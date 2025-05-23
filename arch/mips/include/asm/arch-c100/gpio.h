/*
 * C100 GPIO definitions
 * Copyright (c) 2019 Ingenic Semiconductor Co.,Ltd
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

enum gpio_function {
	GPIO_FUNC_0     = 0x00,  //0000, GPIO as function 0 / device 0
	GPIO_FUNC_1     = 0x01,  //0001, GPIO as function 1 / device 1
	GPIO_FUNC_2     = 0x02,  //0010, GPIO as function 2 / device 2
	GPIO_FUNC_3     = 0x03,  //0011, GPIO as function 3 / device 3
	GPIO_INPUT	= 0x06,	 //0110, GPIO as input
};

enum gpio_port {
	GPIO_PORT_A,
	GPIO_PORT_B,
	GPIO_PORT_C,
	GPIO_PORT_D,
	/* this must be last */
	GPIO_NR_PORTS,
};

#ifdef CONFIG_JZ_SLT
static inline enum gpio_port gpio_port_gp(int gpio)
{
	return (GPIO_PORT_A + (gpio/32));
}

static inline int gpio_pin(int gpio)
{
	return (gpio%32);
}
#endif

struct jz_gpio_func_def {
	int port;
	int func;
	unsigned long pins;
    int driver_strength;
};

/*************************************************************************
 * GPIO (General-Purpose I/O Ports)
 *************************************************************************/
#define MAX_GPIO_NUM	192

#define PXPIN		0x00   /* PIN Level Register */
#define PXINT		0x10   /* Port Interrupt Register */
#define PXINTS		0x14   /* Port Interrupt Set Register */
#define PXINTC		0x18   /* Port Interrupt Clear Register */
#define PXMSK		0x20   /* Port Interrupt Mask Reg */
#define PXMSKS		0x24   /* Port Interrupt Mask Set Reg */
#define PXMSKC		0x28   /* Port Interrupt Mask Clear Reg */
#define PXPAT1		0x30   /* Port Pattern 1 Set Reg. */
#define PXPAT1S		0x34   /* Port Pattern 1 Set Reg. */
#define PXPAT1C		0x38   /* Port Pattern 1 Clear Reg. */
#define PXPAT0		0x40   /* Port Pattern 0 Register */
#define PXPAT0S		0x44   /* Port Pattern 0 Set Register */
#define PXPAT0C		0x48   /* Port Pattern 0 Clear Register */
#define PXFLG		0x50   /* Port Flag Register */
#define PXFLGC		0x58   /* Port Flag clear Register */
#define PXPE		0x60   /* Port Pull Disable Register */           /*C100 delete*/
#define PXPES		0x64   /* Port Pull Disable Set Register */       /*C100 delete*/
#define PXPEC		0x68   /* Port Pull Disable Clear Register */     /*C100 delete*/
#define PXPDIR		0x90   /* Port Pull Direction Register */
#define PXPDIRS		0x94   /* Port Pull Direction Set Register */
#define PXPDIRC		0x98   /* Port Pull Direction Clear Register */


//#define PXPEL		0x110   /* Port Pull Disable Register */
//#define PXPELS		0x114   /* Port Pull Disable Set Register */
//#define PXPELC		0x118   /* Port Pull Disable Clear Register */
//#define PXPEH		0x120   /* Port Pull Disable Register */
//#define PXPEHS		0x124   /* Port Pull Disable Set Register */
//#define PXPEHC		0x128   /* Port Pull Disable Clear Register */

#define PXPUEN		0x110   /* Port Pull-up status Register */
#define PXPUENS		0x114   /* Port Pull-up status Set Register */
#define PXPUENC		0x118   /* Port Pull-up status Clear Register */
#define PXPDEN		0x120   /* Port Pull-down status Register */
#define PXPDENS		0x124   /* Port Pull-down status Set Register */
#define PXPDENC		0x128   /* Port Pull-down status Clear Register */
#define PXPDSL		0x130   /* Port Driver-strength low  Register */
#define PXPDSLS		0x134   /* Port Driver-strength low Set Register */
#define PXPDSLC		0x138   /* Port Driver-strength low Clear Register */
#define PXPDSH		0x140   /* Port Driver-strength high  Register */
#define PXPDSHS		0x144   /* Port Driver-strength high Set Register */
#define PXPDSHC		0x148   /* Port Driver-strength high Clear Register */

#define GPIO_PXPIN(n)	(GPIO_BASE + (PXPIN + (n)*0x1000))     /* PIN Level Register */
#define GPIO_PXINT(n)	(GPIO_BASE + (PXINT + (n)*0x1000))     /* Port Interrupt Register */
#define GPIO_PXINTS(n)	(GPIO_BASE + (PXINTS + (n)*0x1000))    /* Port Interrupt Set Register */
#define GPIO_PXINTC(n)	(GPIO_BASE + (PXINTC + (n)*0x1000))    /* Port Interrupt Clear Register */
#define GPIO_PXMSK(n)	(GPIO_BASE + (PXMSK + (n)*0x1000))     /* Port Interrupt Mask Register */
#define GPIO_PXMSKS(n)	(GPIO_BASE + (PXMSKS + (n)*0x1000))    /* Port Interrupt Mask Set Reg */
#define GPIO_PXMSKC(n)	(GPIO_BASE + (PXMSKC + (n)*0x1000))    /* Port Interrupt Mask Clear Reg */
#define GPIO_PXPAT1(n)	(GPIO_BASE + (PXPAT1 + (n)*0x1000))    /* Port Pattern 1 Register */
#define GPIO_PXPAT1S(n)	(GPIO_BASE + (PXPAT1S + (n)*0x1000))   /* Port Pattern 1 Set Reg. */
#define GPIO_PXPAT1C(n)	(GPIO_BASE + (PXPAT1C + (n)*0x1000))   /* Port Pattern 1 Clear Reg. */
#define GPIO_PXPAT0(n)	(GPIO_BASE + (PXPAT0 + (n)*0x1000))    /* Port Pattern 0 Register */
#define GPIO_PXPAT0S(n)	(GPIO_BASE + (PXPAT0S + (n)*0x1000))   /* Port Pattern 0 Set Register */
#define GPIO_PXPAT0C(n)	(GPIO_BASE + (PXPAT0C + (n)*0x1000))   /* Port Pattern 0 Clear Register */
#define GPIO_PXFLG(n)	(GPIO_BASE + (PXFLG + (n)*0x1000))     /* Port Flag Register */
#define GPIO_PXFLGC(n)	(GPIO_BASE + (PXFLGC + (n)*0x1000))    /* Port Flag clear Register */
#define GPIO_PXPE(n)	(GPIO_BASE + (PXPE + (n)*0x1000))      /* Port Pull Disable Register */          /*C100 delete*/
#define GPIO_PXPES(n)	(GPIO_BASE + (PXPES + (n)*0x1000))     /* Port Pull Disable Set Register */     /*C100 delete*/
#define GPIO_PXPEC(n)	(GPIO_BASE + (PXPEC + (n)*0x1000))	   /* Port Pull Disable Clear Register */   /*C100 delete*/
#define GPIO_PXPUEN(n)	(GPIO_BASE + (PXPUEN + (n)*0x1000))    /* Port Pull-up status Register */
#define GPIO_PXPUENS(n)	(GPIO_BASE + (PXPUENS + (n)*0x1000))   /* Port Pull-up status Set Register */
#define GPIO_PXPUENC(n)	(GPIO_BASE + (PXPUENC + (n)*0x1000))   /* Port Pull-up status Clear Register */
#define GPIO_PXPDEN(n) 	(GPIO_BASE + (PXPDEN + (n)*0x1000))    /* Port Pull-down status Register */
#define GPIO_PXPDENS(n)	(GPIO_BASE + (PXPDENS + (n)*0x1000))   /* Port Pull-down status Set Register */
#define GPIO_PXPDENC(n)	(GPIO_BASE + (PXPDENC + (n)*0x1000))   /* Port Pull-down status Clear Register */





void gpio_set_func(enum gpio_port n, enum gpio_function func, unsigned int pins);
void gpio_port_set_value(int port, int pin, int value);
void gpio_port_direction_input(int port, int pin);
void gpio_port_direction_output(int port, int pin, int value);
void gpio_init(void);
void gpio_enable_pull_up(unsigned gpio);
void gpio_disable_pull_up(unsigned gpio);
void gpio_enable_pull_down(unsigned gpio);
void gpio_disable_pull_down(unsigned gpio);
void gpio_as_irq_high_level(unsigned gpio);
void gpio_as_irq_low_level(unsigned gpio);
void gpio_as_irq_rise_edge(unsigned gpio);
void gpio_as_irq_fall_edge(unsigned gpio);
void gpio_ack_irq(unsigned gpio);
void gpio_set_pull_dir(unsigned gpio, int pull);

#endif /* __GPIO_H__ */
