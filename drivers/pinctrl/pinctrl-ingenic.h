// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __PINCTRL_INGENIC_H__
#define __PINCTRL_INGENIC_H__

#include <linux/types.h>
#include <linux/bitops.h>
#include <dm.h>
#include <dm/of.h>
#include <dm/ofnode.h>
#include <asm-generic/gpio.h>
#include <asm-generic/atomic.h>
#include <asm/arch/gpio.h>
#include <dm/pinctrl.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <dt-bindings/pinctrl/ingenic-pinctrl.h>

#define PxPIN           0x00   /* PIN Level Register */
#define PxINT           0x10   /* Port Interrupt Register */
#define PxINTS          0x14   /* Port Interrupt Set Register */
#define PxINTC          0x18   /* Port Interrupt Clear Register */
#define PxMSK           0x20   /* Port Interrupt Mask Reg */
#define PxMSKS          0x24   /* Port Interrupt Mask Set Reg */
#define PxMSKC          0x28   /* Port Interrupt Mask Clear Reg */
#define PxPAT1          0x30   /* Port Pattern 1 Set Reg. */
#define PxPAT1S         0x34   /* Port Pattern 1 Set Reg. */
#define PxPAT1C         0x38   /* Port Pattern 1 Clear Reg. */
#define PxPAT0          0x40   /* Port Pattern 0 Register */
#define PxPAT0S         0x44   /* Port Pattern 0 Set Register */
#define PxPAT0C         0x48   /* Port Pattern 0 Clear Register */
#define PxFLG           0x50   /* Port Flag Register */
#define PxFLGC          0x58   /* Port Flag clear Register */

/* TODO: move these regs to soc special. */

#define PxPU		0x110	/* PORT PULL-UP State Register */
#define PxPUS		0x114	/* PORT PULL-UP State Register Set */
#define PxPUC		0x118	/* PORT PULL-UP State Register Clear */
#define PxPD		0x120	/* PORT PULL-DOWN State Register */
#define PxPDS		0x124	/* PORT PULL-DOWN State Register Set */
#define PxPDC		0x128	/* PORT PULL-DOWN State Register Clear */

#define PXDS0		0x130   /* PORT Drive Strength State Register0*/
#define PXDS0S		0x134   /* PORT Drive Strength State set Register0*/
#define PXDS0C		0x138   /* PORT Drive Strength State clear Register0*/
#define PXDS1		0x140   /* PORT Drive Strength State Register1*/
#define PXDS1S		0x144   /* PORT Drive Strength State set Register1*/
#define PXDS1C		0x148   /* PORT Drive Strength State clear Register1*/
#define PXDS2		0x150   /* PORT Drive Strength State Register2*/
#define PXDS2S		0x154   /* PORT Drive Strength State set Register2*/
#define PXDS2C		0x158   /* PORT Drive Strength State clear Register2*/

#define PSHADOW_OFF(regsoffset)		(7*(regsoffset))
#define PZGIDLD         0xF0   /* GPIOZ Group ID to load */

#define PIN_NAMES_LEN		10
#define MAX_GPIOS_ON_CHIP	32
#define MAX_FUNCTIONS_ON_GPIO	4
#define PIN_ARGS_FROM_INDEX	0
#define PIN_ARGS_TO_INDEX	1
#define PIN_ARGS_CFG_INDEX	2

static inline u32 pin_bitmap(u32 from, u32 to)
{
	if (to == 31)
		return ~((1 << (from)) - 1);
	return ((~((1 << (from)) - 1)) & ((1 << ((to) + 1)) - 1));
}

static inline unsigned bit_count(unsigned v)
{
	unsigned int c;
	for (c = 0; v; c++)
		v &= v - 1;
	return c;
}

typedef enum ingenic_gpio_pm_arr {
	PM_SLEEP_PULL = 0,	/*sleep state*/		/*input pull*/
	PM_SLEEP_NOPULL,				/*input no pull*/
	PM_SLEEP_PULLUP,				/*input pullup*/
	PM_SLEEP_PULLDOWN,				/*input pulldown*/
	PM_SLEEP_HIZ,					/*input hiz*/
	PM_SLEEP_HIGH,					/*output high*/
	PM_SLEEP_LOW,					/*output low*/
	PM_SLEEP_WAKEUP,				/*sleep wakeup*/
	PM_RESUME_INT,		/*keep register for resume*/
	PM_RESUME_MSK,
	PM_RESUME_PAT0,
	PM_RESUME_PAT1,
	PM_RESUME_PULL,
	PM_RESUME_PULLUP,
	PM_RESUME_PULLDOWN,
	PM_SLEEP_FUNC0,		/* gpio function 0 */
	PM_SLEEP_FUNC1,		/* gpio function 1 */
	PM_SLEEP_FUNC2,		/* gpio function 2 */
	PM_SLEEP_FUNC3,		/* gpio function 3 */
	PM_STATE_CNT,
} ingenic_gpio_pm_arr_t;

struct kref {
	atomic_t refcount;
};

struct pinctrl_gpio_range {
	struct list_head	node;
	const char			*name;
	unsigned int		id;
	unsigned int 		base;
	unsigned int 		pin_base;
	unsigned const		*pins;
	unsigned int		npins;
};

struct pinctrl_pin_desc {
	unsigned int		number;
	const char			*name;
	void				*drv_data;
};

struct pinconf_generic_params {
	const char *const	property;
	enum pin_config_param param;
	u32					default_value;
};

struct pin_config_item {
	const enum pin_config_param param;
	const char *const display;
	const char *const format;
	bool has_arg;
};


struct pinctrl_desc {
	const char			*name;
	const struct pinctrl_pin_desc *pins;
	unsigned int		npins;
	unsigned int		num_custom_params;
	const struct pinconf_generic_params *custom_params;
	const struct pin_config_item *custom_conf_items;
};

struct pinctrl {
	struct list_head	node;
	struct udevice		*dev;
	struct list_head	states;
	struct pinctrl_state *state;
	struct list_head	dt_maps;
	struct kref			users;
};

struct pinctrl_state {
	struct list_head	node;
	const char			*name;
	struct list_head	settings;
};

struct pinctrl_dev {
	struct list_head		node;
	struct pinctrl_desc		*desc;
	struct list_head		gpio_ranges;
	struct udevice			*dev;
	struct module			*owner;
	void					*driver_data;
	struct pinctrl			*p;
	struct pinctrl_state	*hog_default;
	struct pinctrl_state 	*hog_sleep;
	struct mutex			mutex;
};


/*ingenic gpio chip*/
struct ingenic_gpio_chip {
	char			name[4];			/*name format "GPX"(X = 'A' + idx)*/
	u8				idx;				/*gpio index of this chip*/
	const char      *label;
	u16				ngpio;
	int				base;
	struct udevice	*dev;
	int				of_gpio_n_cells;
	ofnode			of_node;			/*gpio chip device node*/
	int				irq;				/*gpio chip irq*/
	struct irq_domain	*irq_domain;
	struct pinctrl_gpio_range	grange;
	struct ingenic_pinctrl		*pctl;
	spinlock_t			lock;				/*gpio func write lock*/
	u32				*mcu_gpio_reg;			/*used for gpio irq to mcu
									  mcu's pending register*/
	u32				used_pins_bitmap;			/*bitmap of pins for been had requested*/
	u32				resume_pending;			/*bitmap of pins wakeup pending when sleep*/
	u32				sleep_level;			/*bitmap of pins level when sleep*/
	u32				filter_bitmap;			/*bitmap of pins filter support*/
	u32				pull_bitmap;			/*bitmap of pins pill support*/
	u32				wakeup_bitmap;			/*bitmap of pins wakeup pins*/
	u32				irq_bitmap;		    	/*bitmap of pins irq pins*/
	u32				pm_irq_bitmap;			/*bitmap of pins used for irq sleep&resume*/
	u32				pm_bitmap[PM_STATE_CNT];	/*bitmap of pins used for sleep&resume*/
};

#define gc_to_ingenic_gc(gpiochip) container_of(gpiochip, struct ingenic_gpio_chip, gc)

/*function information*/
struct ingenic_pinctrl_func {
	const char              *name;
	ofnode					of_node;	/* function device node */
	const char              **groups;	/* all sub groups name*/
	u8                      num_groups;	/* num groups of this function */
};

/*group information*/
struct ingenic_pinctrl_group {
	const char              *name;
	ofnode					of_node;	/* group device node */
	unsigned				*pins;		/* Contained pins software gpio num*/
	u8                      num_pins;	/* num gpios of the set group*/
	u32						pinmux_bitmap;	/* pins bitmap of this group*/
	enum gpio_function		pinmux_func;	/* pins function select of this group*/
};


struct soc_special_regs {
	unsigned int reg;			/* register offset */
	unsigned int reg_set;		/* register set offset */
	unsigned int reg_clear;		/* register clear offset */
};

/*soc private data*/
struct ingenic_priv {
	bool				group_shadow;		/*support shadow register shadow every gpio group*/
	bool				have_shadow;		/*support shadow register or not*/
	bool				have_filter;		/*support input pin filter of nor*/
	bool				pullen_set;		/*set bit to enable pull function.*/
	bool				voltage_switchable;	/*ports support 1.8v and 3.3v switch.*/
	bool				dual_edge_interrupt ;	/*support Dual-Edge Interrupt.*/
	bool				pull_tristate;		/*support pullup/pulldown/hiz settable.*/
	bool				have_ds;		/*support drive strength settings.*/
	bool				have_slew;		/*support drive strength settings.*/
	bool				have_schmit;		/*support drive strength settings.*/
	int (*set_filter)(struct ingenic_gpio_chip *jzgc,	/*set input pin filter function*/
			  unsigned pin,
			  u16 filter);
	ssize_t (*dump_filter)(struct ingenic_pinctrl *pctl,
			       char *buf,
			       ssize_t size);
};

struct ingenic_pinctrl {
	void __iomem			*io_base;
	struct device_node		*of_node;			/*pinctrl device_node*/
	struct udevice			*dev;

	spinlock_t				shadow_lock;		/*shadow register access lock*/
	struct ingenic_gpio_chip	*gpio_chips;	/*gpio chips of this pinctrl*/
	unsigned				num_chips;			/*num gpio chips*/
	unsigned				total_pins;			/*total pins of this pinctrl*/
	u32						*bitmap_priv;		/*******private*******/

	struct pinctrl_desc		pctl_desc;
	struct pinctrl_dev		*pctl_dev;

	struct ingenic_pinctrl_group	*groups;
	unsigned				num_groups;

	struct ingenic_pinctrl_func	*functions;
	unsigned				num_funs;
	u32						group_regs_offset;	/* gpio group regs offset get from dts  */

	const struct ingenic_priv		*priv;		/*soc data*/
};

static inline struct ingenic_pinctrl_group *find_group_by_ofnode(
	struct ingenic_pinctrl *pctl, ofnode node)
{
	int i;
	for (i = 0; i < pctl->num_groups; i++) {
		if (&pctl->groups[i].of_node == &node)
			return &pctl->groups[i];
	}
	return NULL;
}

static inline struct ingenic_pinctrl_func *find_func_by_ofnode(
	struct ingenic_pinctrl *pctl, ofnode node)
{
	int i;
	for (i = 0; i < pctl->num_funs; i++) {
		if (&pctl->functions[i].of_node == &node)
			return &pctl->functions[i];
	}
	return NULL;
}

static struct ingenic_gpio_chip *check_gpio_chip(const char *name, struct ingenic_pinctrl *pctl)
{
	int name_len;
	char last_char;
	int i;
	name_len = strlen(name);
	last_char = name[name_len - 1];
	// 根据字符赋值
	switch (last_char) {
		case 'a':
			i = 0;
			break;
		case 'b':
			i = 1;
			break;
		case 'c':
			i = 2;
			break;
		case 'd':
			i = 3;
			break;
		case 'e':
			i = 4;
			break;
		default:
			i = -1;
	}
	return &pctl->gpio_chips[i];
}


static inline u32 ingenic_gpio_readl(struct ingenic_gpio_chip *chip, int offset)
{
	struct ingenic_pinctrl *pctl = chip->pctl;
	return readl(pctl->io_base + (chip->idx * pctl->group_regs_offset) + offset);
}

static inline void ingenic_gpio_writel(struct ingenic_gpio_chip *chip, int offset, int value)
{
	struct ingenic_pinctrl *pctl = chip->pctl;
	writel(value, pctl->io_base + (chip->idx * pctl->group_regs_offset) + offset);
}

#endif /*__PINCTRL_INGENIC_H__*/
