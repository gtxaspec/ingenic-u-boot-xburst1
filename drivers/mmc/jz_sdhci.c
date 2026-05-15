/*
 * Ingenic JZ MMC driver
 * wqshao  <wangquan.shao@ingenic.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <common.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/arch/clk.h>

#include "jz_sdhci_regs.h"

#ifdef CONFIG_SPL_BUILD
/* SPL will only use a single MMC device (CONFIG_JZ_MMC_SPLMSC) */
struct sdhci_host jz_sdhci_host[1];
#endif

static char *JZ_NAME = "MSC";

#ifdef CONFIG_FPGA
#define clk_set_rate(a, b)
#endif

void ingenic_msc_gpio_enable(int enable)
{
#ifdef CONFIG_JZ_MMC_POWER_GPIO
	if (enable)
		gpio_direction_output(CONFIG_JZ_MMC_POWER_GPIO, CONFIG_JZ_MMC_POWER_GPIO_ENLEVEL);
	mdelay(50);
#endif

#if defined(CONFIG_JZ_MMC_MSC0_PB_4BIT)
	if (enable)
		gpio_set_func(GPIO_PORT_B, GPIO_FUNC_0, (0x3 << 4 | 0xf << 0));
	else
		gpio_set_func(GPIO_PORT_B, GPIO_OUTPUT0, (0x3 << 4 | 0xf << 0));
#endif

#if defined(CONFIG_JZ_MMC_MSC1_PC)
	if (enable)
		gpio_set_func(GPIO_PORT_C, GPIO_FUNC_0, (0x3f << 2));
	else
		gpio_set_func(GPIO_PORT_C, GPIO_OUTPUT0, (0x3f << 2));
#endif

#ifdef CONFIG_JZ_MMC_POWER_GPIO
	if (!enable)
		gpio_direction_output(CONFIG_JZ_MMC_POWER_GPIO, !CONFIG_JZ_MMC_POWER_GPIO_ENLEVEL);
#endif

	return;
}

static void jz_set_mmc_clk(struct sdhci_host *host, unsigned int clock)
{
	sdhci_writew(host, 0x7, SDHCI_CLOCK_CONTROL);
#ifdef CONFIG_JZ_MMC_MSC0
	clk_set_rate(MSC, clock);
#endif
#ifdef CONFIG_JZ_MMC_MSC1
	clk_set_rate(MSC1, clock);
#endif
	//printf("clk: %d, CPM_MSC0_CLK_R: 0x%x\n", clock, readl(CPM_MSC0_CLK_R));
	/* TODO: set clock */
	udelay(5000);
}

static int jz_sdhci_init(u32 regbase, int index)
{
	struct sdhci_host *host = NULL;

#ifdef CONFIG_SPL_BUILD
	host = jz_sdhci_host;
#else
	host = (struct sdhci_host *)malloc(sizeof(struct sdhci_host));
#endif
	if (!host) {
		printf("sdhci__host malloc fail!\n");
		return 1;
	}

	host->name = JZ_NAME;
	host->ioaddr = (void *)regbase;
	host->quirks = SDHCI_QUIRK_NO_HISPD_BIT | SDHCI_QUIRK_BROKEN_VOLTAGE
		| SDHCI_QUIRK_WAIT_SEND_CMD;

	host->voltages = MMC_VDD_27_28 | MMC_VDD_28_29 |MMC_VDD_29_30 |
        MMC_VDD_30_31 |MMC_VDD_31_32 | MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;
	host->version = sdhci_readw(host, SDHCI_HOST_VERSION);

	host->set_control_reg = NULL;
	host->set_clock = &jz_set_mmc_clk;
	host->index = index;

	host->host_caps = MMC_MODE_4BIT | MMC_MODE_HC; //for emmc OCR

	return add_sdhci(host, 25000000, 200000);
}

void jz_mmc_init(void)
{
	/* 使能MSC gpio */
	ingenic_msc_gpio_enable(1);

#if defined(CONFIG_JZ_MMC_MSC0) && (!defined(CONFIG_SPL_BUILD) || (CONFIG_JZ_MMC_SPLMSC == 0))
	jz_sdhci_init(MSC0_BASE, 0);
#endif

#if defined(CONFIG_JZ_MMC_MSC1) && (!defined(CONFIG_SPL_BUILD) || (CONFIG_JZ_MMC_SPLMSC == 1))
	jz_sdhci_init(MSC1_BASE, 1);
#endif

#if defined(CONFIG_JZ_MMC_MSC2) && (!defined(CONFIG_SPL_BUILD) || (CONFIG_JZ_MMC_SPLMSC == 2))
	jz_sdhci_init(MSC2_BASE, 2);
#endif
}
