// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <common.h>
#include <dm/device.h>
#include <dm/device-internal.h>
#include <dm/device_compat.h>
#include <dm/lists.h>
#include <dm/pinctrl.h>
#include <dm/devres.h>
#include <linux/bug.h>
#include <linux/bitmap.h>
#include <log.h>
#include <malloc.h>
#include <dt-bindings/gpio/ingenic-gpio.h>
#include "pinctrl-ingenic.h"


extern void gpio_set_driver_state(enum gpio_port n, unsigned int pins, unsigned int state);
#define INGENIC_GPIO_GROUP_OFFSET     (0x1000)

static char pin_name[32];

static inline int is_ingenic_gpio_from_chip(int gpio_num)
{
	return gpio_num < (GPIO_NR_PORTS * 32) ? 1 : 0;
}

static void ingenic_gpio_port_set_value(int port, int pin, int value)
{
	if (value)
		writel(1 << pin, (void __iomem *)GPIO_PXPAT0S(port));
	else
		writel(1 << pin, (void __iomem *)GPIO_PXPAT0C(port));
}

static void ingenic_gpio_port_direction_input(int port, int pin)
{
	writel(1 << pin, (void __iomem *)GPIO_PXINTC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1S(port));
}

static void ingenic_gpio_port_direction_output(int port, int pin, int value)
{
	writel(1 << pin, (void __iomem *)GPIO_PXINTC(port));
	writel(1 << pin, (void __iomem *)GPIO_PXMSKS(port));
	writel(1 << pin, (void __iomem *)GPIO_PXPAT1C(port));
	ingenic_gpio_port_set_value(port, pin, value);
}

static int ingenic_gpio_direction_input(struct udevice *dev, unsigned offset)
{
	struct ingenic_gpio_chip *jzgc = dev_get_priv(dev);
	if (offset > jzgc->ngpio)
		return -EINVAL;
	else
		ingenic_gpio_port_direction_input(jzgc->idx, offset);
	return 0;
}

static int ingenic_gpio_direction_output(struct udevice *dev, unsigned offset, int value)
{
	struct ingenic_gpio_chip *jzgc = dev_get_priv(dev);
	if (offset > jzgc->ngpio)
		return -EINVAL;
	else
		ingenic_gpio_port_direction_output(jzgc->idx, offset, value);
	return 0;
}

static int ingenic_gpio_set_value(struct udevice *dev, unsigned offset, int value)
{
	struct ingenic_gpio_chip *jzgc = dev_get_priv(dev);
	if (offset > jzgc->ngpio)
		return -EINVAL;
	else
		ingenic_gpio_port_set_value(jzgc->idx, offset, value);
	return 0;
}

static int ingenic_gpio_get_value(struct udevice *dev, unsigned offset)
{
	struct ingenic_gpio_chip *jzgc = dev_get_priv(dev);
	if (offset > jzgc->ngpio)
		return -EINVAL;
	else
		return !!(readl((void __iomem *)GPIO_PXPIN(jzgc->idx)) & (1 << offset));
}

static int ingenic_gpio_get_func(unsigned port, unsigned pin)
{
	int func = 0;
	void __iomem *base = (void __iomem *)(GPIO_BASE + INGENIC_GPIO_GROUP_OFFSET * port);
	func = func | ((readl(base + PXINT) >> pin) & 1) << 3;
	func = func | ((readl(base + PXMSK) >> pin) & 1) << 2;
	func = func | ((readl(base + PXPAT1) >> pin) & 1) << 1;
	func = func | ((readl(base + PXPAT0) >> pin) & 1) << 0;
	char grp[10];
	snprintf(grp, 10, "gpio[%c]", 'a' + port);
	return func;
}

static void ingenic_gpio_set_func(unsigned port, unsigned int pins, enum gpio_function func)
{
	void __iomem *base = (void __iomem *)(GPIO_BASE + INGENIC_GPIO_GROUP_OFFSET * port);
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
#endif
}


static int ingenic_gpio_get_function(struct udevice *dev, unsigned offset)
{
	struct ingenic_gpio_chip *jzgc = dev_get_priv(dev);
	if (offset > jzgc->ngpio)
		return -EINVAL;
	int func = ingenic_gpio_get_func((unsigned)jzgc->idx, offset);
	/* 判断这个pin的function类型 */
	switch (func) {
	case GPIO_FUNC_0:
	case GPIO_FUNC_1:
	case GPIO_FUNC_2:
	case GPIO_FUNC_3:
		return GPIOF_FUNC;
	case GPIO_OUTPUT0:
	case GPIO_OUTPUT1:
		return GPIOF_OUTPUT;
	case GPIO_INPUT:
		return GPIOF_INPUT;
	default:
		return GPIOF_UNKNOWN;
	}
}

static int ingenic_gpio_get_pull_state(struct ingenic_gpio_chip *chip, u32 pins)
{
	unsigned int pull_down = 0;
	unsigned int pull_up = 0;
	int state = 0;
	pull_down = (ingenic_gpio_readl(chip, PxPD) & BIT(pins)) >> pins;
	pull_up = (ingenic_gpio_readl(chip, PxPU) & BIT(pins)) >> pins;
	if (pull_down && pull_up) {
		printf("Set both pull_up and pull_down on %s %d\n", chip->name, pins);
	}
	if (pull_down) {
		state = INGENIC_GPIO_PULLDOWN;
	} else if (pull_up) {
		state = INGENIC_GPIO_PULLUP;
	} else {
		state = INGENIC_GPIO_HIZ;
	}
	return state;
}

static int ingenic_gpio_get_flags(struct udevice *dev, unsigned int offset, ulong *flagsp)
{
	struct ingenic_gpio_chip *jzgc = dev_get_priv(dev);
	ulong dir_flags = 0;
	int func;
	func = ingenic_gpio_get_func((unsigned)jzgc->idx, offset);
	switch (func) {
	case GPIO_OUTPUT0:
		dir_flags |= GPIOD_IS_OUT;
		break;
	case GPIO_OUTPUT1:
		dir_flags |= GPIOD_IS_OUT;
		dir_flags |= GPIOD_IS_OUT_ACTIVE;
		break;
	case GPIO_INPUT:
		dir_flags |= GPIOD_IS_IN;
		break;
	}
	switch (ingenic_gpio_get_pull_state(jzgc, offset)) {
	case INGENIC_GPIO_PULLUP:
		dir_flags |= GPIOD_PULL_UP;
		break;
	case INGENIC_GPIO_PULLDOWN:
		dir_flags |= GPIOD_PULL_DOWN;
		break;
	default:
		break;
	}
	*flagsp = dir_flags;
	return 0;
}

#if 0
static void ingenic_gpio_set_pull(struct ingenic_gpio_chip *chip,
		u32 pins, unsigned int state)
{
	const struct ingenic_priv *priv = chip->pctl->priv;
	if (priv->have_shadow)
		printf("have_shadow\n");
	/*1. try pull_up pull_down hiz settings first.*/
	if (priv->pull_tristate) {
		if (state == INGENIC_GPIO_PULLUP) {
			ingenic_gpio_writel(chip, PxPDC, pins);
			ingenic_gpio_writel(chip, PxPUS, pins);
		} else if (state == INGENIC_GPIO_PULLDOWN) {
			ingenic_gpio_writel(chip, PxPUC, pins);
			ingenic_gpio_writel(chip, PxPDS, pins);
		} else if (state == INGENIC_GPIO_HIZ) {
			ingenic_gpio_writel(chip, PxPUC, pins);
			ingenic_gpio_writel(chip, PxPDC, pins);
		}
		return;
	}
	/*2. For gpios pullup/pulldown state can not be changed.*/
	if (priv->pullen_set) {
		if (state == INGENIC_GPIO_PULLEN) {
			ingenic_gpio_writel(chip, PxPENS, pins);
		} else {
			ingenic_gpio_writel(chip, PxPENC, pins);
		}
	} else {
		if (state == INGENIC_GPIO_PULLEN) {
			ingenic_gpio_writel(chip, PxPENC, pins);
		} else {
			ingenic_gpio_writel(chip, PxPENS, pins);
		}
	}
}

static int ingenic_gpio_set_flags(struct udevice *dev, unsigned int offset, ulong flags)
{
	struct ingenic_gpio_chip *jzgc = dev_get_priv(dev);
	if (flags & GPIOD_IS_OUT) {
		bool value = flags & GPIOD_IS_OUT_ACTIVE;
		ingenic_gpio_direction_output(dev, offset, value);
	} else if (flags & GPIOD_IS_IN) {
		ingenic_gpio_direction_input(dev, offset);
	}
	if (flags & GPIOD_PULL_UP) {
		ingenic_gpio_set_pull(jzgc, offset, INGENIC_GPIO_PULLUP);
	} else if (flags & GPIOD_PULL_DOWN) {
		ingenic_gpio_set_pull(jzgc, offset, INGENIC_GPIO_PULLDOWN);
	}
	return 0;
}
#endif

static const struct dm_gpio_ops ingenic_gpio_ops = {
	.direction_input	= ingenic_gpio_direction_input,
	.direction_output	= ingenic_gpio_direction_output,
	.get_value			= ingenic_gpio_get_value,
	.set_value			= ingenic_gpio_set_value,
	.get_function		= ingenic_gpio_get_function,
	//.set_flags			= ingenic_gpio_set_flags,	/* 开启此函数，则gpio_set_value无法正确执行 */
	.get_flags			= ingenic_gpio_get_flags,
};

static int ingenic_gpio_probe(struct udevice *dev)
{
	struct gpio_dev_priv *uc_priv;
	struct udevice *parent_dev;
	struct ingenic_pinctrl *pctl;
	struct ingenic_gpio_chip *jzgc;
	ofnode child, parent;
	int ret;
	child = dev_ofnode(dev);
	parent = ofnode_get_parent(child);
	ret = uclass_get_device_by_ofnode(UCLASS_PINCTRL, parent, &parent_dev);
	if (ret) {
		printf("Failed to get parent udevice: %d\n", ret);
		return ret;
	}
	pctl = dev_get_priv(parent_dev);
	jzgc = check_gpio_chip(dev->name, pctl);
	dev_set_priv(dev, jzgc);
	uc_priv = devm_kzalloc(dev, sizeof(struct gpio_dev_priv), GFP_KERNEL);
	if (!uc_priv) {
		printf("Failed to allocate memory for uc_priv");
		return -ENOMEM;
	}
	uc_priv->bank_name = jzgc->name;
	uc_priv->gpio_count = jzgc->ngpio;
	uc_priv->gpio_base = jzgc->base;
	uc_priv->name = devm_kzalloc(dev, sizeof(char *) * uc_priv->gpio_count, GFP_KERNEL);
	if (!uc_priv->name) {
		printf("Failed to allocate memory for uc_priv->name\n");
		devm_kfree(dev, uc_priv);
		return -ENOMEM;
	}
	dev_set_uclass_priv(dev, uc_priv);
	return 0;
}

U_BOOT_DRIVER(gpio_ingenic) = {
	.name		= "gpio_ingenic",
	.id			= UCLASS_GPIO,
	.probe		= ingenic_gpio_probe,
	.ops		= &ingenic_gpio_ops,
};

static int ingenic_pinctrl_get_pins_count(struct udevice *dev)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	return pctl->total_pins;
}

static const char *ingenic_pinctrl_get_pin_name(struct udevice *dev,
		unsigned int selector)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	struct ingenic_gpio_chip *jzgc;
	int idx, num;
	idx = selector / MAX_GPIOS_ON_CHIP;
	num = selector % MAX_GPIOS_ON_CHIP;
	jzgc = &pctl->gpio_chips[idx];
	snprintf(pin_name, 10, "%s%d", jzgc->name, num);
	return pin_name;
}

static int ingenic_pinctrl_get_pin_muxing(struct udevice *dev,
		unsigned int selector,
		char *buf,
		int size)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	struct udevice *gpioc_dev;
	int grp_idx;
	int	gpio_idx;
	char gpioc_name[10];
	char gpio_name[10];
	const char *label;
	int mode;
	int func;
	if (selector > pctl->total_pins)
		return -EINVAL;
	grp_idx = selector / MAX_GPIOS_ON_CHIP;
	gpio_idx = selector % MAX_GPIOS_ON_CHIP;
	snprintf(gpioc_name, 10, "gp%c", 'a' + grp_idx);
	snprintf(gpio_name, 10, "%s%d", gpioc_name, gpio_idx);
	if (device_find_child_by_name(dev, gpioc_name, &gpioc_dev)) {
		printf("Failed to find child device '%s'\n", gpioc_name);
		return -EINVAL;
	}
	mode = gpio_get_raw_function(gpioc_dev, gpio_idx, &label);
	switch (mode) {
	case GPIOF_FUNC:
		func = ingenic_gpio_get_func(grp_idx, gpio_idx);
		snprintf(buf, size, "%s%d [%s]", "gpio function", func, label ? label : gpio_name);
		break;
	case GPIOF_OUTPUT:
		snprintf(buf, size, "%s [%s]", "gpio output", label ? label : gpio_name);
		break;
	case GPIOF_INPUT:
		snprintf(buf, size, "%s [%s]", "gpio input", label ? label : gpio_name);
		break;
	case GPIOF_UNKNOWN:
	case GPIOF_UNUSED:
	default:
		snprintf(buf, size, "unknown");
		break;
	}
	return 0;
}

static int ingenic_pinctrl_get_group_count(struct udevice *dev)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	return pctl->num_groups;
}

static const char *ingenic_pinctrl_get_group_name(struct udevice *dev, unsigned selector)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	if (selector >= pctl->num_groups)
		return ERR_PTR(-EINVAL);
	return pctl->groups[selector].name;
}


static int ingenic_gpio_chip_add(struct udevice *dev, ofnode node, int base, int idx)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	struct ingenic_gpio_chip *jzgc = &pctl->gpio_chips[idx];
	u32 ngpio;
	/* 解析gpio控制器设备树并填充对应ingenic_gpio_chip结构体 */
	snprintf(jzgc->name, sizeof(jzgc->name), "gp%c", 'a' + idx);
	if (dev_read_u32(dev, "ingenic,num-gpios", &ngpio))
		ngpio = MAX_GPIOS_ON_CHIP;
	if (dev_read_u32(dev, "ingenic,filter-gpios", &jzgc->filter_bitmap))
		jzgc->filter_bitmap = 0;
	if (dev_read_u32(dev, "ingenic,pull-gpios", &jzgc->pull_bitmap))
		jzgc->pull_bitmap = 0;
	if (dev_read_u32(dev, "#gpio-cells", &jzgc->of_gpio_n_cells))
		jzgc->of_gpio_n_cells = 3;
	jzgc->used_pins_bitmap = 0;
	debug("%s (%d) config:\nfilter %08x\npull %08x\ncells=%d\nbase%d\n",
			jzgc->name,
			ngpio,
			jzgc->filter_bitmap,
			jzgc->pull_bitmap,
			jzgc->of_gpio_n_cells,
			base);
	jzgc->of_node = node;
	jzgc->idx = idx;
	jzgc->pctl = pctl;
	jzgc->ngpio = (u16)ngpio;
	jzgc->base = base;
	jzgc->dev = pctl->dev;
	jzgc->label = ofnode_get_name(node);
	return 0;
}

static int ingenic_init_group(struct udevice *dev, ofnode node,
		struct ingenic_pinctrl_group *grp)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	struct ingenic_gpio_chip *jzgc;
	ofnode gpio_node;
	struct ofnode_phandle_args out_args;
	u32 func;
	int pin, idx = 0, index = 0;
	grp->name = ofnode_get_name(node);
	grp->of_node = node;
	/* 解析ingenic,pinmux参数 */
	while (!ofnode_parse_phandle_with_args(node, "ingenic,pinmux", "#ingenic,pinmux-cells", 0, index++, &out_args)) {
		/* 解析ingenic,pinmux-funcsel参数 */
		if (ofnode_read_u32(node, "ingenic,pinmux-funcsel", &func))
			return -EINVAL;
		gpio_node = out_args.node;
		/* 这组gpio对应的pin */
		grp->pinmux_bitmap |= pin_bitmap(out_args.args[PIN_ARGS_FROM_INDEX],
				out_args.args[PIN_ARGS_TO_INDEX]);
	}
	/* 按这一组pin数量申请空间 */
	grp->num_pins = bit_count(grp->pinmux_bitmap);
	grp->pins = devm_kzalloc(dev, sizeof(unsigned) * grp->num_pins, GFP_KERNEL);
	if (!grp->pins)
		return -ENOMEM;
	/* 判断这组pin的function类型 */
	switch (func) {
	case PINCTL_FUNCTION0:
		grp->pinmux_func = GPIO_FUNC_0;
		break;
	case PINCTL_FUNCTION1:
		grp->pinmux_func = GPIO_FUNC_1;
		break;
	case PINCTL_FUNCTION2:
		grp->pinmux_func = GPIO_FUNC_2;
		break;
	case PINCTL_FUNCTION3:
		grp->pinmux_func = GPIO_FUNC_3;
		break;
	case PINCTL_FUNCLOLVL:
		grp->pinmux_func = GPIO_OUTPUT0;
		break;
	case PINCTL_FUNCHILVL:
		grp->pinmux_func = GPIO_OUTPUT1;
		break;
	case PINCTL_FUNCINPUT:
		grp->pinmux_func = GPIO_INPUT;
		break;
	case PINCTL_FUNCINPUT_LO:
		grp->pinmux_func = GPIO_INT_LO;
		break;
	case PINCTL_FUNCINPUT_HI:
		grp->pinmux_func = GPIO_INT_HI;
		break;
	case PINCTL_FUNCINPUT_FE:
		grp->pinmux_func = GPIO_INT_FE;
		break;
	case PINCTL_FUNCINPUT_RE:
		grp->pinmux_func = GPIO_INT_RE;
		break;
	case PINCTL_FUNCINPUT_RE_FE:
		grp->pinmux_func = GPIO_INT_RE_FE;
		break;
	}
	jzgc = check_gpio_chip(ofnode_get_name(gpio_node), pctl);
	for_each_set_bit(pin, (unsigned long *)&grp->pinmux_bitmap, jzgc->ngpio)
		grp->pins[idx++] = jzgc->base + pin;
	return 0;
}

static struct ingenic_pinctrl_group *ingenic_pinctrl_create_groups(
		struct ingenic_pinctrl *pctl, unsigned *cnt)
{
	struct ingenic_pinctrl_group *grp;
	ofnode child, child1;
	unsigned int i = 0;
	int ret;
	*cnt = 0;
	/* 遍历pinctrl下子节点, 计算非gpio控制器节点数量 */
	ofnode_for_each_subnode(child, dev_ofnode(pctl->dev)) {
		unsigned count = 0;
		if (!!ofnode_get_property(child, "gpio-controller", NULL))
			continue;
		if (!!(count = ofnode_get_child_count(child)))
			* cnt += count;
		else
			(*cnt)++;
	}
	/* 申请group结构体空间 */
	grp = devm_kzalloc(pctl->dev, sizeof(*grp) * (*cnt), GFP_KERNEL);
	if (!grp)
		return ERR_PTR(-ENOMEM);
	int child_cnt = 0;
	/* 遍历pinctrl下子节点 */
	ofnode_for_each_subnode(child, dev_ofnode(pctl->dev)) {
		if (!!ofnode_get_property(child, "gpio-controller", NULL))
			continue;
		/* 判断子节点数量, 遍历子节点并解析设备属性 */
		if (!ofnode_get_child_count(child)) {
			if (ingenic_init_group(pctl->dev, child, &grp[i++]))
				printf("ingenic_init_group failed ret: %d\n", ret);
			continue;
		}
		ofnode_for_each_subnode(child1, child) {
			if (ingenic_init_group(pctl->dev, child1, &grp[i++]))
				printf("ingenic_init_group failed ret: %d\n", ret);
		}
		child_cnt++;
	}
	return grp;
}

static struct ingenic_pinctrl_func *ingenic_pinctrl_create_functions(
		struct ingenic_pinctrl *pctl, unsigned int *cnt)
{
	struct ingenic_pinctrl_func *func;
	ofnode child, child1;
	int i = 0;
	*cnt = 0;
	/* 遍历pinctrl下子节点, 计算非gpio控制器节点数量 */
	ofnode_for_each_subnode(child, dev_ofnode(pctl->dev)) {
		if (!!ofnode_get_property(child, "gpio-controller", NULL))
			continue;
		(*cnt)++;
	}
	func = devm_kzalloc(pctl->dev, sizeof(*func) * (*cnt), GFP_KERNEL);
	if (!func)
		return ERR_PTR(-ENOMEM);
	/* 遍历pinctrl下子节点 */
	ofnode_for_each_subnode(child, dev_ofnode(pctl->dev)) {
		u8 num_child, j = 0;
		if (!!ofnode_get_property(child, "gpio-controller", NULL))
			continue;
		func[i].name = ofnode_get_name(child);
		func[i].of_node = child;
		num_child = ofnode_get_child_count(child);
		func[i].num_groups = num_child ? : 1;
		func[i].groups = devm_kzalloc(pctl->dev, sizeof(char *) * func[i].num_groups, GFP_KERNEL);
		if (!func[i].groups) {
			devm_kfree(pctl->dev, func);
			return ERR_PTR(-ENOMEM);
		}
		if (num_child) {
			ofnode_for_each_subnode(child1, child) {
				func[i].groups[j++] = ofnode_get_name(child1);
			}
		} else {
			func[i].groups[0] = func[i].name;
		}
		i++;
	}
	return func;
}

static int ingenic_pinctrl_parse_dt(struct ingenic_pinctrl *pctl)
{
	struct ingenic_pinctrl_group *groups;
	struct ingenic_pinctrl_func *functions;
	unsigned int grp_cnt = 0, func_cnt = 0;
	/* 解析pinctrl子节点gpio组pin function */
	groups = ingenic_pinctrl_create_groups(pctl, &grp_cnt);
	if (IS_ERR(groups)) {
		printf("failed to parse pin groups\n");
		return PTR_ERR(groups);
	}
	/* 解析pinctrl子节点function */
	functions = ingenic_pinctrl_create_functions(pctl, &func_cnt);
	if (IS_ERR(functions)) {
		printf("failed to parse pin functions\n");
		return PTR_ERR(groups);
	}
	pctl->groups = groups;
	pctl->num_groups = grp_cnt;
	pctl->functions = functions;
	pctl->num_funs = func_cnt;
	return 0;
}

static int ingenic_pinctrl_register(struct udevice *dev)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	int ret;
	pctl->dev = dev;
	/* 解析pinctrl子节点 */
	ret = ingenic_pinctrl_parse_dt(pctl);
	if (ret)
		return ret;
	return 0;
}

static int ingenic_gpioc_register(struct udevice *dev)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	struct ingenic_gpio_chip *gpio_chips;
	const char *name;
	ofnode child;
	int idx = 0, ret;
	u32 offset;
	struct udevice *gpio_dev;

	/* 寄存器偏移 */
	if (!dev_read_u32(dev, "ingenic,regs-offset", &offset))
		pctl->group_regs_offset = offset;
	/* 获取gpio控制器总数 */
	if (dev_read_u32(dev, "ingenic,num-chips", &pctl->num_chips)) {
		devm_kfree(dev, pctl);
		return -EINVAL;
	}
	/* 为gpio控制器结构体分配空间 */
	gpio_chips = devm_kzalloc(dev, pctl->num_chips * sizeof(struct ingenic_gpio_chip), GFP_KERNEL);
	if (!gpio_chips) {
		printf("devm_kzalloc ingenic_gpio_chip failed\n");
		devm_kfree(dev, pctl);
		return -ENOSPC;
	}
	pctl->gpio_chips = gpio_chips;
	pctl->total_pins = 0;
	spin_lock_init(&pctl->shadow_lock);
	/* 遍历pinctrl下子节点*/
	dev_for_each_subnode(child, dev) {
		if (!ofnode_get_property(child, "gpio-controller", NULL))
			continue;
		name = ofnode_get_name(child);
		if (!name) {
			devm_kfree(dev, pctl);
			devm_kfree(dev, gpio_chips);
			return -EINVAL;
		}
		ret = device_bind_driver_to_node(dev, "gpio_ingenic",
				name, child, NULL);
		if (ret)
			return ret;
		if (WARN_ON(idx >= pctl->num_chips))
			break;
		/* 解析gp(a~e)子节点 */
		ret = ingenic_gpio_chip_add(dev, child, pctl->total_pins, idx);
		if (ret) {
			printf("%s gpio add failed\n", gpio_chips[idx].name);
			devm_kfree(dev, pctl);
			devm_kfree(dev, gpio_chips);
			return ret;
		}
		/* 计算引脚总数 */
		pctl->total_pins += gpio_chips[idx].ngpio;
		spin_lock_init(&gpio_chips[idx].lock);
		uclass_get_device_by_ofnode(UCLASS_GPIO, child, &gpio_dev);
		idx++;
	}
	printf("ingenic gpioc register success, pins %d\n", pctl->total_pins);

	return 0;
}

static int ingenic_pinctrl_probe(struct udevice *dev)
{
	int ret;

	ret = ingenic_gpioc_register(dev);
	if (ret) {
		printf("ingenic_gpioc_register faild\n");
		return ret;
	}

	ret = ingenic_pinctrl_register(dev);
	if (ret) {
		printf("ingenic_pinctrl_register faild\n");
		return ret;
	}
	return 0;
}

static int ingenic_pinctrl_bind(struct udevice *dev)
{
	struct ingenic_pinctrl *pctl;

	pctl = devm_kzalloc(dev, sizeof(struct ingenic_pinctrl), GFP_KERNEL);
	if (!pctl) {
		printf("devm_kzalloc ingenic_pinctrl failed\n");
		return -ENOSPC;
	}
	dev_set_priv(dev, pctl);
	return 0;
}

static int ingenic_pinctrl_set_config(struct udevice *dev, ofnode node)
{
	struct ingenic_pinctrl *pctl = dev_get_priv(dev);
	const char *name = ofnode_get_name(node);
	struct gpio_dev_priv *uc_priv;
	struct ingenic_gpio_chip *jzgc;
	struct udevice *gpio_dev;
	int i, j, ret;
	unsigned port;
	for (i = 0; i < pctl->num_groups; i++) {
		struct ingenic_pinctrl_group *grp = &pctl->groups[i];
		if (strcmp(grp->name, name))
			continue;
		port = grp->pins[0] / MAX_GPIOS_ON_CHIP;
		jzgc = &pctl->gpio_chips[port];
		ret = uclass_get_device_by_ofnode(UCLASS_GPIO, jzgc->of_node, &gpio_dev);
		if (ret) {
			printf("uclass_get_device_by_ofnode failed, ofnode name: %s, ret: %d\n", ofnode_get_name(jzgc->of_node), ret);
			return -EINVAL;
		}
		uc_priv = dev_get_uclass_priv(gpio_dev);
		ingenic_gpio_set_func(port, grp->pinmux_bitmap, grp->pinmux_func);
		for (j = 0; j < grp->num_pins; j++) {
			int offset = grp->pins[j] % MAX_GPIOS_ON_CHIP;
			uc_priv->name[offset] = strdup(name);
		}
		break;
	}
	return 0;
}


static int ingenic_pinctrl_set_state(struct udevice *dev, struct udevice *config)
{
	return ingenic_pinctrl_set_config(dev, dev_ofnode(config));
}

static int ingenic_pinctrl_set_state_simple(struct udevice *dev,
		struct udevice *periph)
{
	const fdt32_t *list;
	uint32_t phandle;
	ofnode config_node;
	int size, i, ret;
	ofnode per_node = dev_ofnode(periph);
	list = ofnode_get_property(per_node, "pinctrl-0", &size);
	if (!list)
		return -EINVAL;
	size /= sizeof(*list);
	for (i = 0; i < size; i++) {
		phandle = fdt32_to_cpu(*list++);
		config_node = ofnode_get_by_phandle(phandle);
		if (!ofnode_valid(config_node)) {
			printf("prop pinctrl-0 index %d invalid phandle\n", i);
			return -EINVAL;
		}
		ret = ingenic_pinctrl_set_config(dev, config_node);
		if (ret)
			return ret;
	}
	return 0;
}

static struct pinctrl_ops ingenic_pinctrl_ops = {
	.get_pins_count		= ingenic_pinctrl_get_pins_count,
	.get_pin_name		= ingenic_pinctrl_get_pin_name,
	.get_pin_muxing		= ingenic_pinctrl_get_pin_muxing,
	.get_groups_count	= ingenic_pinctrl_get_group_count,
	.get_group_name		= ingenic_pinctrl_get_group_name,
	.set_state_simple	= ingenic_pinctrl_set_state_simple,
	.set_state		= ingenic_pinctrl_set_state,
};

static const struct ingenic_priv PRJ_priv_data[] = {
	{
		.have_shadow = true,
		.pull_tristate = true,
		.voltage_switchable = true,
		.dual_edge_interrupt = true,
		.have_ds = true,	// support driver strength settings.
		.have_slew = true,
		.have_schmit = true,
	},
};

static const struct udevice_id ingenic_pinctrl_ids[] = {
	{ .compatible = "ingenic,PRJ-pinctrl", .data = (ulong)PRJ_priv_data },
	{ }
};

U_BOOT_DRIVER(pinctrl_ingenic) = {
	.name		= "ingenic-pinctrl",
	.id			= UCLASS_PINCTRL,
	.of_match	= ingenic_pinctrl_ids,
	.ops		= &ingenic_pinctrl_ops,
	.probe		= ingenic_pinctrl_probe,
	.bind		= ingenic_pinctrl_bind,
};

