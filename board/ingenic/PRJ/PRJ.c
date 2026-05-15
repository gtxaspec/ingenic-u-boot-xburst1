// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <common.h>
#include <asm/spl.h>

void board_boot_order(u32 *spl_boot_list)
{
#if defined(CONFIG_SPL_MMC_SUPPORT)
	spl_boot_list[0] = BOOT_DEVICE_MMC1;
#endif
#ifdef CONFIG_SPL_SFC_NOR_SUPPORT
	spl_boot_list[0] = BOOT_DEVICE_NOR;
#endif
#ifdef CONFIG_SPL_SFC_NAND_SUPPORT
	spl_boot_list[0]  = BOOT_DEVICE_NAND;
#endif
}
