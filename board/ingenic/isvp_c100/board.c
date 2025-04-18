/*
 * Ingenic isvp setup code
 *
 * Copyright (c) 2017 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
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

#include <common.h>
#include <nand.h>
#include <net.h>
#include <netdev.h>
#include <asm/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/arch/nand.h>
#include <asm/arch/mmc.h>
#include <asm/arch/clk.h>
#include <power/d2041_core.h>

extern int jz_net_initialize(bd_t *bis);

struct cgu_clk_src cgu_clk_src[] = {
	{AVPU, MPLL},
	{MACPHY, MPLL},
	{MSC, APLL},
	{SSI, MPLL},
	{CIM, VPLL},
	{ISP, MPLL},
	{I2S_SPK, APLL}, // i2s uses APLL
	{I2S_MIC, APLL}, // i2s uses APLL
	{SRC_EOF,SRC_EOF}
};

int board_early_init_f(void)
{
/*
	gpio_direction_input(25);
	gpio_direction_input(26);
	gpio_disable_pull_down(25);
	gpio_disable_pull_up(25);
	gpio_disable_pull_down(26);
	gpio_disable_pull_up(26);
*/
	return 0;
}

#ifdef CONFIG_USB_GADGET
int jz_udc_probe(void);
void board_usb_init(void)
{
	printf("USB_udc_probe\n");
	jz_udc_probe();
}
#endif /* CONFIG_USB_GADGET */

int misc_init_r(void)
{
#if 0 /* TO DO */
	uint8_t mac[6] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc };

	/* set MAC address */
	eth_setenv_enetaddr("ethaddr", mac);
#endif
	/* used for usb_dete */
	gpio_enable_pull_up(GPIO_PB(24));//UART1 rx

	return 0;
}



#ifdef CONFIG_MMC
int board_mmc_init(bd_t *bd)
{
	jz_mmc_init();
	return 0;
}
#endif

#ifdef CONFIG_SYS_NAND_SELF_INIT
void board_nand_init(void)
{
	return 0;
}
#endif

int board_eth_init(bd_t *bis)
{
	int ret = 0;
#ifdef CONFIG_USB_ETHER_ASIX
	if (0 == strncmp(getenv("ethact"), "asx", 3)) {
		run_command("usb start", 0);
	}
#endif
	ret += jz_net_initialize(bis);
	return ret;
}

#ifdef CONFIG_SPL_NOR_SUPPORT
int spl_start_uboot(void)
{
	return 1;
}
#endif

#ifdef CONFIG_SPL_BUILD

void spl_board_init(void)
{
}

#endif /* CONFIG_SPL_BUILD */
