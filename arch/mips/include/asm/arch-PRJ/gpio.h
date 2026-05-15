// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include <asm/arch/base.h>

#define GPIO_PA(n)         (0*32 + n)
#define GPIO_PB(n)         (1*32 + n)
#define GPIO_PC(n)         (2*32 + n)

enum gpio_function {
	GPIO_FUNC_0     = 0x00,  //0000, GPIO as function 0 / device 0
	GPIO_FUNC_1     = 0x01,  //0001, GPIO as function 1 / device 1
	GPIO_FUNC_2     = 0x02,  //0010, GPIO as function 2 / device 2
	GPIO_FUNC_3     = 0x03,  //0011, GPIO as function 3 / device 3
	GPIO_OUTPUT0    = 0x04,  //0100, GPIO output low  level
	GPIO_OUTPUT1    = 0x05,  //0101, GPIO output high level
	GPIO_INPUT      = 0x06,  //0110, GPIO as input
	GPIO_INT_LO     = 0x08,  //1000, GPIO as Low  Level trigger interrupt
	GPIO_INT_HI     = 0x09,  //1001, GPIO as High Level trigger interrupt
	GPIO_INT_FE     = 0x0a,  //1010, GPIO as Fall Edge trigger interrupt
	GPIO_INT_RE     = 0x0b,  //1011, GPIO as rise edge trigger interrupt
	GPIO_INT_RE_FE  = 0x1a,  //Fall&Rise Edge trigger interrupt
	GPIO_PULL       = 0x10,  //
};

enum gpio_port {
	GPIO_PORT_A,
	GPIO_PORT_B,
	GPIO_PORT_C,
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
#define MAX_GPIO_NUM      192

#define PXPIN             0x00   /* PIN Level Register */
#define PXINT             0x10   /* Port Interrupt Register */
#define PXINTS            0x14   /* Port Interrupt Set Register */
#define PXINTC            0x18   /* Port Interrupt Clear Register */
#define PXMSK             0x20   /* Port Interrupt Mask Reg */
#define PXMSKS            0x24   /* Port Interrupt Mask Set Reg */
#define PXMSKC            0x28   /* Port Interrupt Mask Clear Reg */
#define PXPAT1            0x30   /* Port Pattern 1 Set Reg. */
#define PXPAT1S           0x34   /* Port Pattern 1 Set Reg. */
#define PXPAT1C           0x38   /* Port Pattern 1 Clear Reg. */
#define PXPAT0            0x40   /* Port Pattern 0 Register */
#define PXPAT0S           0x44   /* Port Pattern 0 Set Register */
#define PXPAT0C           0x48   /* Port Pattern 0 Clear Register */
#define PXFLG             0x50   /* Port Flag Register */
#define PXFLGC            0x58   /* Port Flag clear Register */
#define PXPUEN            0x110  /* Port Pull-up Enable Register */
#define PXPUENS           0x114  /* Port Pull-up Enable Set Register */
#define PXPUENC           0x118  /* Port Pull-up Enable Clear Register */
#define PXPDEN            0x120  /* Port Pull-down Enable Register */
#define PXPDENS           0x124  /* Port Pull-down Enable Set Register */
#define PXPDENC           0x128  /* Port Pull-down Enable Clear Register */
#define PXDSL             0x130  /* Port Drive Strength Low Register */
#define PXDSLS            0x134  /* Port Drive Strength Low Set Register */
#define PXDSLC            0x138  /* Port Drive Strength Low Clear Register */
#define PXDSH             0x140  /* Port Drive Strength High Register */
#define PXDSHS            0x144  /* Port Drive Strength High Set Register */
#define PXDSHC            0x148  /* Port Drive Strength High Clear Register */
/* shadow */
#define PXSHD             0x1d0  /* Port Shadow register */
#define PXSHDS            0X1d4  /* Port Shadow set register */
#define PXSHDC            0X1d8  /* Port Shadow clear register */
#define PXUPD             0X1e4  /* Port Shadow update register */

#define GPIO_PXPIN(n)     (GPIO_BASE + (PXPIN + (n)*0x1000))
#define GPIO_PXINT(n)     (GPIO_BASE + (PXINT + (n)*0x1000))
#define GPIO_PXINTS(n)    (GPIO_BASE + (PXINTS + (n)*0x1000))
#define GPIO_PXINTC(n)    (GPIO_BASE + (PXINTC + (n)*0x1000))
#define GPIO_PXMSK(n)     (GPIO_BASE + (PXMSK + (n)*0x1000))
#define GPIO_PXMSKS(n)    (GPIO_BASE + (PXMSKS + (n)*0x1000))
#define GPIO_PXMSKC(n)    (GPIO_BASE + (PXMSKC + (n)*0x1000))
#define GPIO_PXPAT1(n)    (GPIO_BASE + (PXPAT1 + (n)*0x1000))
#define GPIO_PXPAT1S(n)   (GPIO_BASE + (PXPAT1S + (n)*0x1000))
#define GPIO_PXPAT1C(n)   (GPIO_BASE + (PXPAT1C + (n)*0x1000))
#define GPIO_PXPAT0(n)    (GPIO_BASE + (PXPAT0 + (n)*0x1000))
#define GPIO_PXPAT0S(n)   (GPIO_BASE + (PXPAT0S + (n)*0x1000))
#define GPIO_PXPAT0C(n)   (GPIO_BASE + (PXPAT0C + (n)*0x1000))
#define GPIO_PXFLG(n)     (GPIO_BASE + (PXFLG + (n)*0x1000))
#define GPIO_PXFLGC(n)    (GPIO_BASE + (PXFLGC + (n)*0x1000))
#define GPIO_PXPUEN(n)    (GPIO_BASE + (PXPUEN + (n)*0x1000))
#define GPIO_PXPUENS(n)   (GPIO_BASE + (PXPUENS + (n)*0x1000))
#define GPIO_PXPUENC(n)   (GPIO_BASE + (PXPUENC + (n)*0x1000))
#define GPIO_PXPDEN(n)    (GPIO_BASE + (PXPDEN + (n)*0x1000))
#define GPIO_PXPDENS(n)   (GPIO_BASE + (PXPDENS + (n)*0x1000))
#define GPIO_PXPDENC(n)   (GPIO_BASE + (PXPDENC + (n)*0x1000))
#define GPIO_PXDSL(n)     (GPIO_BASE + (PXDSL + (n)*0x1000))
#define GPIO_PXDSLS(n)    (GPIO_BASE + (PXDSLS + (n)*0x1000))
#define GPIO_PXDSLC(n)    (GPIO_BASE + (PXDSLC + (n)*0x1000))
#define GPIO_PXDSH(n)     (GPIO_BASE + (PXDSH + (n)*0x1000))
#define GPIO_PXDSHS(n)    (GPIO_BASE + (PXDSHS + (n)*0x1000))
#define GPIO_PXDSHC(n)    (GPIO_BASE + (PXDSHC + (n)*0x1000))
/* shadow */
#define GPIO_PXSHD(n)     (GPIO_BASE + (PXSHD + (n)*0x1000))
#define GPIO_PXSHDS(n)    (GPIO_BASE + (PXSHDS + (n)*0x1000))
#define GPIO_PXSHDC(n)    (GPIO_BASE + (PXSHDC + (n)*0x1000))
#define GPIO_PXUPD(n)     (GPIO_BASE + (PXUPD + (n)*0x1000))

void gpio_set_func(enum gpio_port n, enum gpio_function func, unsigned int pins);
void gpio_port_set_value(int port, int pin, int value);
void gpio_port_direction_input(int port, int pin);
void gpio_port_direction_output(int port, int pin, int value);
void gpio_init(void);
void gpio_enable_pull(unsigned gpio);
void gpio_disable_pull(unsigned gpio);
void gpio_as_irq_high_level(unsigned gpio);
void gpio_as_irq_low_level(unsigned gpio);
void gpio_as_irq_rise_edge(unsigned gpio);
void gpio_as_irq_fall_edge(unsigned gpio);
void gpio_ack_irq(unsigned gpio);
int gpio_clear_flag(unsigned gpio);
int gpio_get_flag(unsigned int gpio);
void gpio_set_driver_strength(unsigned gpio, unsigned lev);
void gpio_set_pull(unsigned gpio, enum gpio_pull pull);

#endif /* __GPIO_H__ */
