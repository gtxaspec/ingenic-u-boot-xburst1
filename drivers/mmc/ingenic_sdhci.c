// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2015 Atmel Corporation
 *		      Wenyou.Yang <wenyou.yang@ingenic.com>
 */

#include <common.h>
#include <clk.h>
#include <dm.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/global_data.h>
#include <asm/io.h>

#include <linux/delay.h>
#include <asm/arch/clk.h>
#include <pwrseq.h>
#include <dm/pinctrl.h>

#define INGENIC_SDHC_MIN_FREQ	300000
#define INGENIC_SDHC_GCK_RATE	24000000

DECLARE_GLOBAL_DATA_PTR;

struct ingenic_sdhci_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};


static void ingenic_sdhci_set_clock(struct sdhci_host *host, unsigned int clock)
{
	int clk_id;
	/* 不打开控制器CLK使能，CPM会busy超时 */
	sdhci_writew(host, 0x7, SDHCI_CLOCK_CONTROL);
	if(host->index == 0)
		clk_id = MSC0;
	else if(host->index == 1)
		clk_id = MSC1;

	_clk_set_rate(clk_id, clock);
}

static const struct sdhci_ops ingenic_sdhci_ops = {
	.set_clock = ingenic_sdhci_set_clock,
};

static int ingenic_sdhci_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
	struct ingenic_sdhci_plat *plat = dev_get_plat(dev);
	struct sdhci_host *host = dev_get_priv(dev);
	int index;
	int ret;

	host->name = dev->name;
	host->ioaddr = map_physmem(dev_read_addr(dev), 0x10000, MAP_NOCACHE);
	host->quirks = SDHCI_QUIRK_WAIT_SEND_CMD;
	host->bus_width = dev_read_u32_default(dev, "bus-width", 4);

	ret = dev_read_alias_seq(dev, &index);
	if (ret < 0) {
		printf("%s: Could not get alias for %s: %d\n",
				__func__, dev->name, ret);
		return ret;
	}
	host->index = index;

	ret = mmc_of_parse(dev, &plat->cfg);
	if (ret)
		return ret;
#ifdef CONFIG_MMC_PWRSEQ
	if (plat->cfg.pwr_dev) {
        ret = pwrseq_set_power(plat->cfg.pwr_dev, true);
        if (ret) {
            printf("%s: Failed to power on SD card: %d\n", __func__, ret);
            return ret;
        }
	}
#endif

	ret = pinctrl_select_state(dev, "active");
        if (ret) {
            printf("%s: Failed to set pinctrl to active: %d\n", __func__, ret);
        }
	host->max_clk = INGENIC_SDHC_GCK_RATE;
	host->mmc = &plat->mmc;
	host->mmc->dev = dev;
	ret = sdhci_setup_cfg(&plat->cfg, host, INGENIC_SDHC_GCK_RATE, INGENIC_SDHC_MIN_FREQ);
	if (ret)
		return ret;
	host->mmc->priv = host;
	host->ops = &ingenic_sdhci_ops;
	upriv->mmc = host->mmc;

	return sdhci_probe(dev);
}

static int ingenic_sdhci_bind(struct udevice *dev)
{
	struct ingenic_sdhci_plat *plat = dev_get_plat(dev);
	return sdhci_bind(dev, &plat->mmc, &plat->cfg);
}

static int ingenic_sdhci_remove(struct udevice *dev)
{
		int ret;

		ret = pinctrl_select_state(dev, "default");
		if (ret) {
				printf("%s: Warning: Failed to switch to 'default' pinctrl state: %d\n", __func__, ret);
		}

#ifdef CONFIG_MMC_PWRSEQ
		struct ingenic_sdhci_plat *plat = dev_get_plat(dev);
		if (plat->cfg.pwr_dev) {
				ret = pwrseq_set_power(plat->cfg.pwr_dev, false);
				if (ret) {
						printf("%s: Warning: Failed to power off SD card: %d\n", __func__, ret);
				}
		}
#endif
		return 0;
}

static const struct udevice_id ingenic_sdhci_ids[] = {
	{ .compatible = "ingenic,sdhci" },
	{ }
};

U_BOOT_DRIVER(ingenic_sdhci_drv) = {
	.name		= "ingenic_sdhci",
	.id		= UCLASS_MMC,
	.of_match	= ingenic_sdhci_ids,
	.ops		= &sdhci_ops,
	.bind		= ingenic_sdhci_bind,
	.probe		= ingenic_sdhci_probe,
	.remove		= ingenic_sdhci_remove,
	.flags		= DM_FLAG_OS_PREPARE,
	.priv_auto	= sizeof(struct sdhci_host),
	.plat_auto	= sizeof(struct ingenic_sdhci_plat),
};
