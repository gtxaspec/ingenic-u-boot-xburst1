// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2021 SAMSUNG Electronics
 * Jaehoon Chung <jh80.chung@samsung.com>
 */

#include <common.h>
#include <dm.h>
#include <mmc.h>
#include <pwrseq.h>
#include <asm/gpio.h>
#include <linux/delay.h>

struct mmc_pwrseq_priv {
		struct gpio_desc reset_gpio;
		struct gpio_desc power_gpio;
};

int mmc_pwrseq_get_power(struct udevice *dev, struct mmc_config *cfg)
{
	/* Enable power if needed */
	return uclass_get_device_by_phandle(UCLASS_PWRSEQ, dev, "mmc-pwrseq",
					   &cfg->pwr_dev);
}

static int mmc_pwrseq_set_power(struct udevice *dev, bool enable)
{
		struct mmc_pwrseq_priv *priv = dev_get_priv(dev);
		int gpio_num,port_num;

		if (dm_gpio_is_valid(&priv->power_gpio)) {
				gpio_num = gpio_get_number(&priv->power_gpio);
				port_num = gpio_num / 32;
				if (enable) {
						gpio_set_func(port_num, GPIO_OUTPUT0, 0x1 << priv->power_gpio.offset);
						mdelay(50);
				} else {
						mdelay(50);
						gpio_set_func(port_num, GPIO_OUTPUT1, 0x1 << priv->power_gpio.offset);
				}
		}

		if (dm_gpio_is_valid(&priv->reset_gpio) && enable) {
				dm_gpio_set_value(&priv->reset_gpio, 1);
				udelay(1);
				dm_gpio_set_value(&priv->reset_gpio, 0);
				udelay(200);
		}

		return 0;
}

static int mmc_pwrseq_probe(struct udevice *dev)
{
		struct mmc_pwrseq_priv *priv = dev_get_priv(dev);
		int ret;

		ret = gpio_request_by_name(dev, "power-gpios", 0, &priv->power_gpio, GPIOD_IS_OUT);
		if (ret < 0 && ret != -ENOENT)
				return ret;

		ret = gpio_request_by_name(dev, "reset-gpios", 0, &priv->reset_gpio, GPIOD_IS_OUT);
		if (ret < 0 && ret != -ENOENT)
				return ret;

		return 0;
}

static const struct pwrseq_ops mmc_pwrseq_ops = {
	.set_power	= mmc_pwrseq_set_power,
};

static const struct udevice_id mmc_pwrseq_ids[] = {
	{ .compatible = "mmc-pwrseq-mmc" },
	{ .compatible = "mmc-pwrseq-sd"},
	{ }
};

U_BOOT_DRIVER(mmc_pwrseq_drv) = {
	.name		= "mmc_pwrseq_mmc",
	.id		= UCLASS_PWRSEQ,
	.of_match	= mmc_pwrseq_ids,
	.ops		= &mmc_pwrseq_ops,
	.probe		= mmc_pwrseq_probe,
	.priv_auto		= sizeof(struct mmc_pwrseq_priv),
};
