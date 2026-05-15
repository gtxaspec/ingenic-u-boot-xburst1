// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <common.h>
#include <asm/io.h>
#include <dm.h>
#include <clk.h>
#include <phy.h>
#include <regmap.h>
#include <malloc.h>
#include <reset.h>
#include <syscon.h>
#include "designware.h"
#include <dm/device_compat.h>
#include <linux/err.h>
#include <asm/arch/clk.h>
#if CONFIG_IS_ENABLED(DM_GPIO)
#include <asm-generic/gpio.h>
#endif


DECLARE_GLOBAL_DATA_PTR;

#define INGENIC_ETH_SEL_MASK		(0x7)
#define INGENIC_ETH_SEL_RMII		(0x4)

#define INGENIC_ETH_SPEED_MASK		(0x3<<29)
#define INGENIC_ETH_SPEED_10M		(0x2<<29)
#define INGENIC_ETH_SPEED_100M		(0x3<<29)


struct dwmac_ingenic_plat {
	struct dw_eth_pdata dw_eth_pdata;
	unsigned int macphy_rate;
	unsigned int max_speed;
	volatile unsigned int *cpm_phyc_reg;
#if CONFIG_IS_ENABLED(DM_GPIO)
	struct gpio_desc reset_gpio;
#endif
};

static int dwmac_ingenic_of_to_plat(struct udevice *dev)
{
	struct dwmac_ingenic_plat *pdata = dev_get_plat(dev);
	unsigned int cpm_reg = 0;

	pdata->macphy_rate = dev_read_u32_default(dev, "macphy-rate", 0);
	pdata->max_speed = dev_read_u32_default(dev, "max-speed", 0);
	cpm_reg	= dev_read_u32_default(dev, "ingenic,mode-reg", -1);
	debug("macphy_rate = %d\n",pdata->macphy_rate);
	debug("max_speed = %d\n",pdata->max_speed);
	debug("cpm_reg = 0x%x\n",cpm_reg);

	pdata->cpm_phyc_reg = ioremap(cpm_reg, 4);

	return designware_eth_of_to_plat(dev);
}

static int ingenic_gmac_setphy(struct udevice *dev)
{
	struct dwmac_ingenic_plat *pdata = dev_get_plat(dev);
	int ret = 0;

#if CONFIG_IS_ENABLED(DM_GPIO)
	int reset_flags = GPIOD_IS_OUT | GPIOD_ACTIVE_LOW;

	ret = gpio_request_by_name(dev, "ingenic,reset-gpio", 0,
		&pdata->reset_gpio, reset_flags);
	if(ret) {
		dev_err(dev, "request gpio Failed\n");
		return ret;
	}

	/* reset the phy */
	ret = dm_gpio_set_value(&pdata->reset_gpio, 0);
	if (ret)
		return ret;
	mdelay(10);

	ret = dm_gpio_set_value(&pdata->reset_gpio, 1);
	if (ret)
		return ret;
#endif

	/* *(volatile unsigned int *)(0xb0011018) = 0x04000000; */
	/* *(volatile unsigned int *)(0xb0011024) = 0x04000000; */
	/* *(volatile unsigned int *)(0xb0011038) = 0x04000000; */
	/* *(volatile unsigned int *)(0xb0011048) = 0x04000000; */
	/* mdelay(10); */
	/* *(volatile unsigned int *)(0xb0011018) = 0x04000000; */
	/* *(volatile unsigned int *)(0xb0011024) = 0x04000000; */
	/* *(volatile unsigned int *)(0xb0011038) = 0x04000000; */
	/* *(volatile unsigned int *)(0xb0011044) = 0x04000000; */


	return ret;
}

static int ingenic_gmac_init(struct udevice *dev)
{
	struct dwmac_ingenic_plat *pdata = dev_get_plat(dev);
	struct eth_pdata *edata = &pdata->dw_eth_pdata.eth_pdata;
	int ret = 0;

	*pdata->cpm_phyc_reg &= ~(INGENIC_ETH_SEL_MASK | INGENIC_ETH_SPEED_MASK);

	switch (edata->phy_interface) {
	case PHY_INTERFACE_MODE_MII:
		break;
	case PHY_INTERFACE_MODE_RMII:
		*pdata->cpm_phyc_reg |= INGENIC_ETH_SEL_RMII;
		break;
	case PHY_INTERFACE_MODE_GMII:
		break;
	case PHY_INTERFACE_MODE_RGMII:
		break;
	default:
		dev_err(dev, "Unsupported PHY mode\n");
		return -EINVAL;
	}

	switch (edata->max_speed) {
	case 10:
		*pdata->cpm_phyc_reg |= INGENIC_ETH_SPEED_10M;
		break;
	case 100:
		*pdata->cpm_phyc_reg |= INGENIC_ETH_SPEED_100M;
		break;
	default:
		dev_err(dev, "Ingenic ETH Speed init:  Do not manage %d interface\n",
			 edata->max_speed);
		return -EINVAL;
	}

	_clk_set_rate(MACPHY, pdata->macphy_rate);

	return ret;
}

static int dwmac_ingenic_probe(struct udevice *dev)
{
	int ret;

	ret = ingenic_gmac_init(dev);
	if (ret)
		return ret;

	ret = ingenic_gmac_setphy(dev);
	if (ret)
		return ret;

	return designware_eth_probe(dev);
}

static const struct udevice_id dwmac_ingenic_ids[] = {
	{ .compatible = "ingenic,gmac" },
	{ }
};

U_BOOT_DRIVER(dwmac_ingenic) = {
	.name		= "dwmac_ingenic",
	.id		= UCLASS_ETH,
	.of_match	= dwmac_ingenic_ids,
	.of_to_plat	= dwmac_ingenic_of_to_plat,
	.probe		= dwmac_ingenic_probe,
	.ops		= &designware_eth_ops,
	.priv_auto	= sizeof(struct dw_eth_dev),
	.plat_auto	= sizeof(struct dwmac_ingenic_plat),
	.flags		= DM_FLAG_ALLOC_PRIV_DMA,
};
