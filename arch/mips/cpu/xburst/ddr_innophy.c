/*
 * DDR driver for Synopsys DWC DDR PHY.
 * Used by Jz4775, JZ4780...
 *
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
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

#include <config.h>
#include <common.h>
#include <ddr/ddr_common.h>
#include <ddr/ddrc.h>
#include <generated/ddr_reg_values.h>

#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/arch/cpm.h>
/* #define CONFIG_DWC_DEBUG 1 */
#include "ddr_debug.h"

DECLARE_GLOBAL_DATA_PTR;

#if CONFIG_DWC_DEBUG
#define PRJ007_ddrc(fmt, addr) dwc_debug("%s addr(0x%x) 0x%x\n", fmt, addr, PRJ007_ddr_readl(addr))
#else
#define PRJ007_ddrc(fmt, addr)
#endif

static void PRJ007_ddrc_dump()
{
	dwc_debug("=============== DDRC Register ================\n");
	PRJ007_ddrc("MSTR        ", MSTR);
	PRJ007_ddrc("INIT1       ", INIT1);
	PRJ007_ddrc("INIT0       ", INIT0);
	PRJ007_ddrc("INIT3       ", INIT3);
	PRJ007_ddrc("INIT4       ", INIT4);
	PRJ007_ddrc("ADDRMAP1    ", ADDRMAP1);
	PRJ007_ddrc("ADDRMAP2    ", ADDRMAP2);
	PRJ007_ddrc("ADDRMAP3    ", ADDRMAP3);
	PRJ007_ddrc("ADDRMAP4    ", ADDRMAP4);
	PRJ007_ddrc("ADDRMAP5    ", ADDRMAP5);
	PRJ007_ddrc("ADDRMAP6    ", ADDRMAP6);
	PRJ007_ddrc("DRAMTMG0    ", DRAMTMG0);
	PRJ007_ddrc("DRAMTMG1    ", DRAMTMG1);
	PRJ007_ddrc("DRAMTMG2    ", DRAMTMG2);
	PRJ007_ddrc("DRAMTMG3    ", DRAMTMG3);
	PRJ007_ddrc("DRAMTMG4    ", DRAMTMG4);
	PRJ007_ddrc("DRAMTMG5    ", DRAMTMG5);
	PRJ007_ddrc("DRAMTMG8    ", DRAMTMG8);
	PRJ007_ddrc("DFITMG0     ", DFITMG0);
	PRJ007_ddrc("DFITMG1     ", DFITMG1);
	PRJ007_ddrc("DFIUPD0     ", DFIUPD0);
	PRJ007_ddrc("RFSHTMG     ", RFSHTMG);
	PRJ007_ddrc("RFSHCTL3    ", RFSHCTL3);
	dwc_debug("==============================================\n");
}

static int get_ddr_type(void)
{
	unsigned int type = DDR_TYPE;

	switch (type)
	{
		case TYPE_DDR2:
			dwc_debug("DDR: %s type is : DDR2\n", DDR_NAME);
			break;
		case TYPE_DDR3:
			dwc_debug("DDR: %s type is : DDR3\n", DDR_NAME);
			break;
		case TYPE_LPDDR2:
			dwc_debug("DDR: %s type is : LPDDR2\n", DDR_NAME);
			break;
		case TYPE_LPDDR3:
			dwc_debug("DDR: %s type is : LPDDR3\n", DDR_NAME);
			break;
		default:
			type = UNKOWN;
			dwc_debug("unsupport ddr type!\n");
	}
	return type;
}

static void ddrp_trining_d2lpd2_read_calib_bypass()
{

	SET_INNOPHY_REG(reg_train_reg_update_en, 0x1);

	SET_INNOPHY_REG(reg_a_l_rxmen0_delay_bp, READ_INNOPHY_REG(reg_a_l_cycsel));
	SET_INNOPHY_REG(reg_a_l_rxmen0_ophsel_bp, READ_INNOPHY_REG(reg_a_l_ophsel));
	SET_INNOPHY_REG(reg_a_l_rxmen0_sdlltap_bp, READ_INNOPHY_REG(reg_a_l_dllsel));
	SET_INNOPHY_REG(reg_a_h_rxmen0_delay_bp, READ_INNOPHY_REG(reg_a_h_cycsel));
	SET_INNOPHY_REG(reg_a_h_rxmen0_ophsel_bp, READ_INNOPHY_REG(reg_a_h_ophsel));
	SET_INNOPHY_REG(reg_a_h_rxmen0_sdlltap_bp, READ_INNOPHY_REG(reg_a_h_dllsel));

	SET_INNOPHY_REG(reg_calib_freq_update, 0x1);
	udelay(1);
	SET_INNOPHY_REG(reg_calib_freq_update, 0x0);
	SET_INNOPHY_REG(reg_calib_bypass, 0x1);
	SET_INNOPHY_REG(reg_train_reg_update_en, 0x0);

}

static void ddrp_trining_d2lpd2_read_train_bypass(unsigned int dqs0, unsigned int dqs1, unsigned int dq)
{
	SET_INNOPHY_REG(reg_train_reg_update_en, 0x1);

	SET_INNOPHY_REG(reg_a_l_cs0_dqs_invdelayselrx, dqs0);
	SET_INNOPHY_REG(reg_a_l_cs0_dqsb_invdelayselrx, dqs0);

	SET_INNOPHY_REG(reg_a_h_cs0_dqs_invdelayselrx, dqs1);
	SET_INNOPHY_REG(reg_a_h_cs0_dqsb_invdelayselrx, dqs1);

	SET_INNOPHY_REG(reg_a_l_cs0_dm_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq0_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq1_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq2_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq3_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq4_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq5_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq6_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq7_invdelayselrx, dq);

	SET_INNOPHY_REG(reg_a_h_cs0_dm_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq0_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq1_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq2_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq3_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq4_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq5_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq6_invdelayselrx, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq7_invdelayselrx, dq);

	SET_INNOPHY_REG(reg_rd_train_freq_update, 0x1);
	udelay(1);
	SET_INNOPHY_REG(reg_rd_train_freq_update, 0x0);
	SET_INNOPHY_REG(reg_train_reg_update_en, 0x0);

	dwc_debug("================ Rx bypass ===================\n");
	dwc_debug("reg_a_l_cs0_dqs_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dqs_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dqs_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dqs_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dm_invdelayselrx        0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dm_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq0_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq0_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq1_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq1_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq2_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq2_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq3_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq3_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq4_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq4_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq5_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq5_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq6_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq6_invdelayselrx));
	dwc_debug("reg_a_l_cs0_dq7_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq7_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dm_invdelayselrx        0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dm_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq0_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq0_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq1_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq1_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq2_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq2_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq3_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq3_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq4_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq4_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq5_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq5_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq6_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq6_invdelayselrx));
	dwc_debug("reg_a_h_cs0_dq7_invdelayselrx       0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dq7_invdelayselrx));
}

static void ddrp_trining_d2lpd2_write_train_bypass_dqs(unsigned int dqs0, unsigned int dqs1)
{
	SET_INNOPHY_REG(reg_train_reg_update_en, 1);

	SET_INNOPHY_REG(reg_a_l_cs0_dqs_invdelaysel, dqs0);  // rx dqs0
	SET_INNOPHY_REG(reg_a_l_cs0_dqsb_invdelaysel, dqs0); // rx dqsb0
	SET_INNOPHY_REG(reg_a_h_cs0_dqs_invdelaysel, dqs1);  // rx dqs1
	SET_INNOPHY_REG(reg_a_h_cs0_dqsb_invdelaysel, dqs1); // rx dqsb1

	SET_INNOPHY_REG(reg_wl_freq_update, 1);
	udelay(1);
	SET_INNOPHY_REG(reg_wl_freq_update, 0);
	SET_INNOPHY_REG(reg_train_reg_update_en, 0);

	dwc_debug("================ Tx bypass ===================\n");
	dwc_debug("reg_a_l_cs0_dqs_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dqs_invdelaysel));
	dwc_debug("reg_a_h_cs0_dqs_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_h_cs0_dqs_invdelaysel));
}

static void ddrp_trining_d2lpd2_write_train_bypass_dq(unsigned int dq)
{
	SET_INNOPHY_REG(reg_train_reg_update_en, 1);

	SET_INNOPHY_REG(reg_a_l_cs0_dm_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq0_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq1_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq2_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq3_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq4_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq5_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq6_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_l_cs0_dq7_invdelaysel, dq);

	SET_INNOPHY_REG(reg_a_h_cs0_dm_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq0_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq1_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq2_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq3_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq4_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq5_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq6_invdelaysel, dq);
	SET_INNOPHY_REG(reg_a_h_cs0_dq7_invdelaysel, dq);

	SET_INNOPHY_REG(reg_wl_freq_update, 1);
	udelay(1);
	SET_INNOPHY_REG(reg_wl_freq_update, 0);
	SET_INNOPHY_REG(reg_train_reg_update_en, 0);
	dwc_debug("reg_a_l_cs0_dm_invdelaysel          0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dm_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq0_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq0_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq1_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq1_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq2_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq2_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq3_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq3_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq4_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq4_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq5_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq5_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq6_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq6_invdelaysel));
	dwc_debug("reg_a_l_cs0_dq7_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq7_invdelaysel));
	dwc_debug("reg_a_h_cs0_dm_invdelaysel          0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dm_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq0_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq0_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq1_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq1_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq2_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq2_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq3_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq3_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq4_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq4_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq5_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq5_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq6_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq6_invdelaysel));
	dwc_debug("reg_a_h_cs0_dq7_invdelaysel         0x%x\n", READ_INNOPHY_REG(reg_a_l_cs0_dq7_invdelaysel));
	dwc_debug("==============================================\n");
}

static void ddrp_trining_d3lpd3_write_leveling()
{
	/* write_leveling */
	SET_INNOPHY_REG(reg_wlcs_sel, 0x2);
#ifdef CONFIG_DDR_TYPE_DDR3
	// For DDR3/4, bit[13:0] should keep the same value with MR1[13:0],and this register[15:14] should be set to 2’b01.
	SET_INNOPHY_REG(reg_wl_loadmode, ((DDRC_INIT3 & 0x3fff) | (1 << 14)));
#elif defined(CONFIG_DDR_TYPE_LPDDR3)
	// For LPDDR3/4(X), bit[7:0] should keep the same value with the MR2[7:0],bit[15:8] should be set to 8’h0.
	SET_INNOPHY_REG(reg_wl_loadmode, (DDRC_INIT3 & 0xff));
#endif

	SET_INNOPHY_REG(reg_phy_refresh_en, 0x1);
	SET_INNOPHY_REG(reg_wl_enable, 0x1);
	dwc_debug("wait_write_leveling_done:\n");
	BNE_INNOPHY_REG(wl_done_byte, 0x3);
	dwc_debug("wait_write_leveling2_done:\n");
	BNE_INNOPHY_REG(reg_wl_end, 0x1);
	SET_INNOPHY_REG(reg_wl_enable, 0x0);
	SET_INNOPHY_REG(reg_wlcs_sel, 0x0);
}

static void ddrp_trining_d3lpd3_read_training()
{
	dwc_debug("=============== Read Training ================\n");
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x1);
	SET_INNOPHY_REG(reg_rdtrain_cs_sel, 0x2);
	SET_INNOPHY_REG(reg_a_l_rd_train_dqs_default, 0x1f);
	SET_INNOPHY_REG(reg_a_h_rd_train_dqs_default, 0x1f);
	SET_INNOPHY_REG(reg_dq_rd_train_en, 0x1);
	dwc_debug("wait_read_training_done:   \n");
	BNE_INNOPHY_REG(train_true_done, 0x1);

	SET_INNOPHY_REG(reg_dq_rd_train_en, 0x0);
	SET_INNOPHY_REG(reg_rd_train_dqs_range_bypass, 0x0);
	SET_INNOPHY_REG(reg_rdtrain_cs_sel, 0x0);
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x0);

	dwc_debug("reg_a_l_train_min_for_rd_dq0        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq0));
	dwc_debug("reg_a_l_train_min_for_rd_dq1        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq1));
	dwc_debug("reg_a_l_train_min_for_rd_dq2        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq2));
	dwc_debug("reg_a_l_train_min_for_rd_dq3        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq3));
	dwc_debug("reg_a_l_train_min_for_rd_dq4        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq4));
	dwc_debug("reg_a_l_train_min_for_rd_dq5        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq5));
	dwc_debug("reg_a_l_train_min_for_rd_dq6        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq6));
	dwc_debug("reg_a_l_train_min_for_rd_dq7        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq7));
	dwc_debug("reg_a_h_train_min_for_rd_dq0        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq0));
	dwc_debug("reg_a_h_train_min_for_rd_dq1        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq1));
	dwc_debug("reg_a_h_train_min_for_rd_dq2        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq2));
	dwc_debug("reg_a_h_train_min_for_rd_dq3        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq3));
	dwc_debug("reg_a_h_train_min_for_rd_dq4        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq4));
	dwc_debug("reg_a_h_train_min_for_rd_dq5        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq5));
	dwc_debug("reg_a_h_train_min_for_rd_dq6        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq6));
	dwc_debug("reg_a_h_train_min_for_rd_dq7        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_min_for_rd_dq7));

	dwc_debug("reg_a_l_train_max_for_rd_dq0        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq0));
	dwc_debug("reg_a_l_train_max_for_rd_dq1        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq1));
	dwc_debug("reg_a_l_train_max_for_rd_dq2        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq2));
	dwc_debug("reg_a_l_train_max_for_rd_dq3        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq3));
	dwc_debug("reg_a_l_train_max_for_rd_dq4        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq4));
	dwc_debug("reg_a_l_train_max_for_rd_dq5        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq5));
	dwc_debug("reg_a_l_train_max_for_rd_dq6        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq6));
	dwc_debug("reg_a_l_train_max_for_rd_dq7        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq7));
	dwc_debug("reg_a_h_train_max_for_rd_dq0        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq0));
	dwc_debug("reg_a_h_train_max_for_rd_dq1        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq1));
	dwc_debug("reg_a_h_train_max_for_rd_dq2        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq2));
	dwc_debug("reg_a_h_train_max_for_rd_dq3        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq3));
	dwc_debug("reg_a_h_train_max_for_rd_dq4        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq4));
	dwc_debug("reg_a_h_train_max_for_rd_dq5        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq5));
	dwc_debug("reg_a_h_train_max_for_rd_dq6        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq6));
	dwc_debug("reg_a_h_train_max_for_rd_dq7        0x%x\n", READ_INNOPHY_REG(reg_a_l_train_max_for_rd_dq7));
	dwc_debug("==============================================\n");

}

static void ddrp_trining_d3lpd3_write_training()
{
	/* write_training */
	SET_INNOPHY_REG(reg_wrtrain_check_data_value_random_gen, 0x1);
	SET_INNOPHY_REG(reg_wrtrain_cs_sel, 0x2);
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x1);
	SET_INNOPHY_REG(reg_wr_train_dqs_default_bypass, 0x0);
	SET_INNOPHY_REG(reg_dq_wr_train_auto, 0x1);
	SET_INNOPHY_REG(reg_dq_wr_train_en, 0x1);
	dwc_debug("wait_write_training_done1:\n");
	BNE_INNOPHY_REG(train_step1_delay_done, 0x1);

	dwc_debug("wait_write_training_done2:\n");
	BNE_INNOPHY_REG(train_all_step_done, 0x1);

	SET_INNOPHY_REG(reg_dq_wr_train_en, 0x0);
	SET_INNOPHY_REG(reg_wr_train_dqs_default_bypass, 0x0);
	SET_INNOPHY_REG(reg_wrtrain_cs_sel, 0x0);
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x0);

	dwc_debug("train_step1_error       %x\n", READ_INNOPHY_REG(train_step1_error));
	dwc_debug("train_step2_error       %x\n", READ_INNOPHY_REG(train_step2_error));
	dwc_debug("train_step2_error       %x\n", READ_INNOPHY_REG(train_step2_error));
	dwc_debug("reg_wr_train_error_byte %x\n", READ_INNOPHY_REG(reg_wr_train_error_byte));
}

static void ddrp_dqs_calibration()
{
	// The related registers hold default value
	/* dqs_gating（calibration） */
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x1);
	SET_INNOPHY_REG(reg_calcs_sel, 0x2); // choose rank0
	SET_INNOPHY_REG(reg_start_calib, 0x1);

	dwc_debug("wait_dqs_gating_done1:\n");
	BNE_INNOPHY_REG(calib_end, 0x1);

	dwc_debug("wait_dqs_gating_done2:\n");
	BNE_INNOPHY_REG(calib_done_byte, 0x3);

	SET_INNOPHY_REG(reg_phy_refresh_en, 0x0);
	SET_INNOPHY_REG(reg_start_calib, 0x0);
	SET_INNOPHY_REG(reg_calcs_sel, 0x0);
	dwc_debug("=============== Calibration ==================\n");
	dwc_debug("reg_a_l_cycsel                      0x%x\n", READ_INNOPHY_REG(reg_a_l_cycsel));
	dwc_debug("reg_a_l_ophsel                      0x%x\n", READ_INNOPHY_REG(reg_a_l_ophsel));
	dwc_debug("reg_a_l_dllsel                      0x%x\n", READ_INNOPHY_REG(reg_a_l_dllsel));
	dwc_debug("reg_a_h_cycsel                      0x%x\n", READ_INNOPHY_REG(reg_a_h_cycsel));
	dwc_debug("reg_a_h_ophsel                      0x%x\n", READ_INNOPHY_REG(reg_a_h_ophsel));
	dwc_debug("reg_a_h_dllsel                      0x%x\n", READ_INNOPHY_REG(reg_a_h_dllsel));
}

#ifdef CONFIG_DDR_SOFT_TRAIN
static int ddr_mem_pattern()
{
	volatile unsigned int tmp = 0;
	unsigned int tmp_data = 0, ret = 0;
	unsigned int td = 0xa0000080;

	for (tmp = 0xa0000000; tmp < 0xa0000020; tmp += 0x4)
	{
		td = tmp;
		*(u32 *)tmp = td;

		tmp_data = *(u32 *)tmp;
		if (tmp_data != td)
		{
			return -1;
		}
	}

	return ret;
}

static void ddrp_trining_d2lpd2_soft_read_train(unsigned int dq)
{
	unsigned int min = 0xff, max = 0, med = 0x55, width = 0;
	unsigned int dqs = 0;

	ddrp_trining_d2lpd2_read_train_bypass(dqs,dqs,dq);

	for (dqs = 0; dqs <= 0x7f; dqs++)
	{
		SET_INNOPHY_REG(reg_train_reg_update_en, 1);
		SET_INNOPHY_REG(reg_a_l_cs0_dqs_invdelayselrx, dqs);  // rx dqs0
		SET_INNOPHY_REG(reg_a_l_cs0_dqsb_invdelayselrx, dqs); // rx dqsb0
		SET_INNOPHY_REG(reg_a_h_cs0_dqs_invdelayselrx, dqs);  // rx dqs1
		SET_INNOPHY_REG(reg_a_h_cs0_dqsb_invdelayselrx, dqs); // rx dqsb1
		SET_INNOPHY_REG(reg_rd_train_freq_update, 1);
		udelay(1);
		SET_INNOPHY_REG(reg_rd_train_freq_update, 0);
		SET_INNOPHY_REG(reg_train_reg_update_en, 0);

		if (!ddr_mem_pattern())
		{
			width++;
			max = dqs;
			if (0xff == min)
				min = dqs;
		}
	}
	if (0xff == min)
		min = 0;
	med = min + width / 2;

	dwc_debug("read soft training 0x%x  min %x  max %x\n", med, min, max);

	SET_INNOPHY_REG(reg_train_reg_update_en, 1);
	SET_INNOPHY_REG(reg_a_l_cs0_dqs_invdelayselrx, med);  // rx dqs0
	SET_INNOPHY_REG(reg_a_l_cs0_dqsb_invdelayselrx, med); // rx dqsb0
	SET_INNOPHY_REG(reg_a_h_cs0_dqs_invdelayselrx, med);  // rx dqs1
	SET_INNOPHY_REG(reg_a_h_cs0_dqsb_invdelayselrx, med); // rx dqsb1
	SET_INNOPHY_REG(reg_rd_train_freq_update, 1);
	udelay(1);
	SET_INNOPHY_REG(reg_rd_train_freq_update, 0);
	SET_INNOPHY_REG(reg_train_reg_update_en, 0);
}

static void ddrp_trining_d2lpd2_soft_write_train()
{
	unsigned int min = 0xff, max = 0, med = 0x7, width = 0;
	unsigned int dqs = 0;

	SET_INNOPHY_REG(reg_wl_bypass, 1);
	for (dqs = 0; dqs <= 0xff; dqs++)
	{
		SET_INNOPHY_REG(reg_train_reg_update_en, 1);

		SET_INNOPHY_REG(reg_a_l_cs0_dqs_invdelaysel, dqs);	// rx dqs0
		SET_INNOPHY_REG(reg_a_l_cs0_dqsb_invdelaysel, dqs); // rx dqsb0
		SET_INNOPHY_REG(reg_a_h_cs0_dqs_invdelaysel, dqs);	// rx dqs1
		SET_INNOPHY_REG(reg_a_h_cs0_dqsb_invdelaysel, dqs); // rx dqsb1

		SET_INNOPHY_REG(reg_wl_freq_update, 1);
		udelay(1);
		SET_INNOPHY_REG(reg_wl_freq_update, 0);
		SET_INNOPHY_REG(reg_train_reg_update_en, 0);

		if (!ddr_mem_pattern())
		{
			width++;
			max = dqs;
			if (0xff == min)
				min = dqs;
		}
	}
	if (0xff == min)
		min = 0;
	med = min + width / 2;
	dwc_debug("write soft training 0x%x  min %x  max %x\n", med, min, max);

	SET_INNOPHY_REG(reg_train_reg_update_en, 1);

	SET_INNOPHY_REG(reg_a_l_cs0_dqs_invdelaysel, med);	// tx dqs0
	SET_INNOPHY_REG(reg_a_l_cs0_dqsb_invdelaysel, med); // tx dqsb0
	SET_INNOPHY_REG(reg_a_h_cs0_dqs_invdelaysel, med);	// tx dqs1
	SET_INNOPHY_REG(reg_a_h_cs0_dqsb_invdelaysel, med); // tx dqsb1

	SET_INNOPHY_REG(reg_wl_freq_update, 1);
	udelay(1);
	SET_INNOPHY_REG(reg_wl_freq_update, 0);
	SET_INNOPHY_REG(reg_train_reg_update_en, 0);
}
#endif

static void ddrp_trining(unsigned int *p)
{
	unsigned int stat = 0;

	PRJ007_ddr_writel(PCTRL0, 0x1);
	PRJ007_ddr_writel(PCTRL1, 0x1);
	PRJ007_ddr_writel(PCTRL2, 0x1);
	PRJ007_ddr_writel(PCTRL3, 0x1);
	PRJ007_ddr_writel(PCTRL4, 0x1);
	PRJ007_ddr_writel(PCTRL5, 0x1);
	PRJ007_ddr_writel(PCTRL6, 0x1);

	SET_INNOPHY_REG(reg_dq_wr_train_auto, 0x0);

	stat = PRJ007_ddr_readl(STAT);
	while (!(stat & 0x1))
	{
		stat = PRJ007_ddr_readl(STAT);
	}

#if defined(CONFIG_DDR_TYPE_DDR3) || defined(CONFIG_DDR_TYPE_LPDDR3)
	ddrp_trining_d3lpd3_write_leveling();
#endif

	ddrp_dqs_calibration();

#if defined(CONFIG_DDR_TYPE_DDR3) || defined(CONFIG_DDR_TYPE_LPDDR3)
	ddrp_trining_d3lpd3_read_training();
	ddrp_trining_d3lpd3_write_training();
#elif defined(CONFIG_DDR_TYPE_DDR2) || defined(CONFIG_DDR_TYPE_LPDDR2)
	ddrp_trining_d2lpd2_read_calib_bypass();

#ifdef CONFIG_DDR_SOFT_TRAIN
	printf("DDR soft training ...\n");
	ddrp_trining_d2lpd2_soft_read_train(0x40);
	ddrp_trining_d2lpd2_write_train_bypass_dq(0x80);
	ddrp_trining_d2lpd2_soft_write_train();
#else
	ddrp_trining_d2lpd2_read_train_bypass(p[SKEW_DQS0R], p[SKEW_DQS1R], p[SKEW_DQRX]);
	ddrp_trining_d2lpd2_write_train_bypass_dq(p[SKEW_DQTX]);
	ddrp_trining_d2lpd2_write_train_bypass_dqs(p[SKEW_DQS0T], p[SKEW_DQS1T]);
#endif
#endif

	dwc_debug("%d calib_error %x\n", __LINE__, READ_INNOPHY_REG(calib_error));
	dwc_debug("%d reg_a_l_tdqs_invdelaysel0 %x\n", __LINE__, READ_INNOPHY_REG(reg_a_l_tdqs_invdelaysel0));
	dwc_debug("%d reg_a_h_tdqs_invdelaysel0 %x\n", __LINE__, READ_INNOPHY_REG(reg_a_h_tdqs_invdelaysel0));
}

static void ddrc_init(void)
{
	int data = 0;

	PRJ007_ddr_writel(MSTR, DDRC_MSTR);

#if defined(CONFIG_DDR_TYPE_DDR3) || defined(CONFIG_DDR_TYPE_DDR2)
	PRJ007_ddr_writel(INIT1, DDRC_INIT1);
	PRJ007_ddr_writel(INIT0, DDRC_INIT0);
#elif defined(CONFIG_DDR_TYPE_LPDDR2) || defined(CONFIG_DDR_TYPE_LPDDR3)
	PRJ007_ddr_writel(INIT2, DDRC_INIT2);
	PRJ007_ddr_writel(INIT5, DDRC_INIT5);
#endif
	PRJ007_ddr_writel(INIT3, DDRC_INIT3);
	PRJ007_ddr_writel(INIT4, DDRC_INIT4);

	PRJ007_ddr_writel(ADDRMAP1, DDRC_ADDRMAP1);
	PRJ007_ddr_writel(ADDRMAP2, DDRC_ADDRMAP2);
	PRJ007_ddr_writel(ADDRMAP3, DDRC_ADDRMAP3);
	PRJ007_ddr_writel(ADDRMAP4, DDRC_ADDRMAP4);
	PRJ007_ddr_writel(ADDRMAP5, DDRC_ADDRMAP5);
	PRJ007_ddr_writel(ADDRMAP6, DDRC_ADDRMAP6);

	PRJ007_ddr_writel(DRAMTMG0, DDRC_TIMING0);
	PRJ007_ddr_writel(DRAMTMG1, DDRC_TIMING1);
	PRJ007_ddr_writel(DRAMTMG2, DDRC_TIMING2);
	PRJ007_ddr_writel(DRAMTMG3, DDRC_TIMING3);
	PRJ007_ddr_writel(DRAMTMG4, DDRC_TIMING4);
	PRJ007_ddr_writel(DRAMTMG5, DDRC_TIMING5);
#if defined(CONFIG_DDR_TYPE_DDR3) || defined(CONFIG_DDR_TYPE_DDR2)
	PRJ007_ddr_writel(DRAMTMG8, DDRC_TIMING8);
	PRJ007_ddr_writel(DFITMG0, DDRC_DFITMG0);
	PRJ007_ddr_writel(DFITMG1, DDRC_DFITMG1);
	PRJ007_ddr_writel(DFIUPD0, DDRC_DFIUPD0);
#elif defined(CONFIG_DDR_TYPE_LPDDR2) || defined(CONFIG_DDR_TYPE_LPDDR3)
	PRJ007_ddr_writel(DRAMTMG6, DDRC_TIMING6);
	PRJ007_ddr_writel(DRAMTMG7, DDRC_TIMING7);
	PRJ007_ddr_writel(DRAMTMG14, DDRC_TIMING14);
#endif

#if defined(CONFIG_DDR_TYPE_LPDDR2)
	PRJ007_ddr_writel(DFITMG0, 6 << 24 | 2 << 16 | 0 << 8 | 0 << 0);
	PRJ007_ddr_writel(DFITMG1, 8 << 16 | 3 << 8 | 7 << 0);
	PRJ007_ddr_writel(DFIUPD0, 1 << 31 | 0x40 << 16 | 3 << 0);
#elif defined(CONFIG_DDR_TYPE_LPDDR3)
	PRJ007_ddr_writel(DFITMG0, 6 << 24 | 5 << 16 | 0 << 8 | 2 << 0);
	PRJ007_ddr_writel(DFITMG1, 8 << 16 | 3 << 8 | 7 << 0);
	PRJ007_ddr_writel(DFIUPD0, 1 << 31 | 0x40 << 16 | 3 << 0);
#endif

	PRJ007_ddr_writel(RFSHTMG, DDRC_RFSHTMG);
	PRJ007_ddr_writel(RFSHCTL3, DDRC_RFSHCTL3);

#if defined(CONFIG_DDR_TYPE_DDR2) || defined(CONFIG_DDR_TYPE_LPDDR2)
	PRJ007_ddr_writel(ODTCFG, DDRC_ODTCFG);
#endif

	data = PRJ007_ddr_readl(SCHED);
	data &= (~(1 << 2));
	PRJ007_ddr_writel(SCHED, data);

}

static void ddrp_init(unsigned int ddrc_reset)
{
	int data = 0;

	SET_INNOPHY_REG(reg_train_reg_update_en, 0x0);
	SET_INNOPHY_REG(reg_channel_en, DDRP_REG_CHANNEL_EN);
	SET_INNOPHY_REG(CWL_FRE_OP0, DDRP_CWL_FRE_OP0);
	SET_INNOPHY_REG(CL_FRE_OP0, DDRP_CL_FRE_OP0);
	SET_INNOPHY_REG(AL_FRE_OP0, DDRP_AL_FRE_OP0);
	SET_INNOPHY_REG(reg_phy_trfc, DDRP_REG_PHY_TRFC);
	SET_INNOPHY_REG(reg_phy_trefi, DDRP_REG_PHY_TREFI);
	SET_INNOPHY_REG(mem_select_t, DDRP_MEM_SELECT_T);

	SET_INNOPHY_REG(reg_pllpostdiven_fsp0, DDRP_REG_PLLPOSTDIVEN);
	SET_INNOPHY_REG(reg_pllpostdiven_fsp1, DDRP_REG_PLLPOSTDIVEN);
	SET_INNOPHY_REG(reg_pllpostdiven_fsp2, DDRP_REG_PLLPOSTDIVEN);
	SET_INNOPHY_REG(reg_pllpostdiven_fsp3, DDRP_REG_PLLPOSTDIVEN);
	SET_INNOPHY_REG(reg_pllpostdiv_fsp0, DDRP_REG_PLLPOSTDIV);
	SET_INNOPHY_REG(reg_pllpostdiv_fsp1, DDRP_REG_PLLPOSTDIV);
	SET_INNOPHY_REG(reg_pllpostdiv_fsp2, DDRP_REG_PLLPOSTDIV);
	SET_INNOPHY_REG(reg_pllpostdiv_fsp3, DDRP_REG_PLLPOSTDIV);

	dwc_debug("reg_pllpostdiven_fsp0    %x\n", READ_INNOPHY_REG(reg_pllpostdiven_fsp0));
	dwc_debug("reg_pllpostdiven_fsp1    %x\n", READ_INNOPHY_REG(reg_pllpostdiven_fsp1));
	dwc_debug("reg_pllpostdiven_fsp2    %x\n", READ_INNOPHY_REG(reg_pllpostdiven_fsp2));
	dwc_debug("reg_pllpostdiven_fsp3    %x\n", READ_INNOPHY_REG(reg_pllpostdiven_fsp3));
	dwc_debug("reg_pllpostdiv_fsp0      %x\n", READ_INNOPHY_REG(reg_pllpostdiv_fsp0));
	dwc_debug("reg_pllpostdiv_fsp1      %x\n", READ_INNOPHY_REG(reg_pllpostdiv_fsp1));
	dwc_debug("reg_pllpostdiv_fsp2      %x\n", READ_INNOPHY_REG(reg_pllpostdiv_fsp2));
	dwc_debug("reg_pllpostdiv_fsp3      %x\n", READ_INNOPHY_REG(reg_pllpostdiv_fsp3));

	// SET_INNOPHY_REG(reg_a_l_rxen_lp4, 0x0);
	// SET_INNOPHY_REG(reg_a_h_rxen_lp4, 0x0);

	ddrc_reset &= ~(0x10); /* ddrc reset ok */
	writel(ddrc_reset, CPM_BASE + CPM_SRBC0);
	udelay(500);
	data = PRJ007_ddr_readl(DFIMISC);
	data |= 0x20;
	PRJ007_ddr_writel(DFIMISC, data);

	dwc_debug("wait_dfi_init_complete:\n");
	while (!(PRJ007_ddr_readl(DFISTAT) & 0x1))
		;

	dwc_debug("INNO PHY DDR DFI complete...\n");
	data = PRJ007_ddr_readl(DFIMISC);
	data &= ~(1 << 5);
	PRJ007_ddr_writel(DFIMISC, data);
}

static void ddrp_set_drv_odt(unsigned int *p)
{
	SET_INNOPHY_REG(reg_a_l_odt_pu, p[ODT_PU]);    // [ 7: 0]ODT pull up     The bit[5:0] of the ODT pull-up resistance of byte0.
	SET_INNOPHY_REG(reg_a_l_odt_pd, p[ODT_PD]);    // [ 7: 0]ODT pull down   The bit[5:0] of the ODT pull-down resistance of byte0.
	SET_INNOPHY_REG(reg_a_h_odt_pu, p[ODT_PU]);    // [15: 8]ODT pull up     The bit[5:0] of the ODT pull-up resistance of byte1.
	SET_INNOPHY_REG(reg_a_h_odt_pd, p[ODT_PD]);    // [15: 8]ODT pull down   The bit[5:0] of the ODT pull-down resistance of byte1.

	SET_INNOPHY_REG(reg_a_l_drv_pu, p[DQX_RC_PU]); // [ 7: 0]DQ  pull up     The bit[5:0] of the driver pull-up resistance of byte0.
	SET_INNOPHY_REG(reg_a_l_drv_pd, p[DQX_RC_PD]); // [ 7: 0]DQ  pull down   The bit[5:0] of the driver pull-down resistance of byte0.
	SET_INNOPHY_REG(reg_a_h_drv_pu, p[DQX_RC_PU]); // [15: 8]DQ  pull up     The bit[5:0] of the driver pull-up resistance of byte1.
	SET_INNOPHY_REG(reg_a_h_drv_pd, p[DQX_RC_PD]); // [15: 8]DQ  pull down   The bit[5:0] of the driver pull-down resistance of byte1.


	SET_INNOPHY_REG(reg_ck0_pu_reg, p[CMD_RC_PU]); // [ 7: 0]CMD pull up     The bit[5:0] of the pull-up resistance of CMD except CK.
	SET_INNOPHY_REG(reg_ck0_pd_reg, p[CMD_RC_PD]); // [ 7: 0]CMD pull down   The bit[5:0] of the pull-down resistance of CMD except CK.

	SET_INNOPHY_REG(reg_cmd_pu_reg, p[CLK_RC_PU]); // [ 7: 0]CK0 pull up     The bit[5:0] of the pull-up resistance of CK.
	SET_INNOPHY_REG(reg_cmd_pd_reg, p[CLK_RC_PD]); // [ 7: 0]CK0 pull down   The bit[5:0] of the pull-down resistance of CK.
}

static void ddrc_set_port_priority()
{
	unsigned int port = 0, val = 0;

	port = 0;
	val = PRJ007_ddr_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val &= ~(0x3ff);
	PRJ007_ddr_writel(DDRCAPB_BASE + (0x404 + 0xb0 * port), val);
	dwc_debug("port0 read       %x\n", PRJ007_ddr_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port)));

	val = PRJ007_ddr_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val &= ~(0x3ff);
	PRJ007_ddr_writel(DDRCAPB_BASE + (0x408 + 0xb0 * port), val);
	dwc_debug("port0 write       %x\n", PRJ007_ddr_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port)));

#if 0
	port = 1;
	val = PRJ007_ddr_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val |= 0x3ff;
	PRJ007_ddr_writel(DDRCAPB_BASE + (0x404 + 0xb0 * port), val);
	dwc_debug("port1 read       %x\n", PRJ007_ddr_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port)));

	val = PRJ007_ddr_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val |= 0x3ff;
	PRJ007_ddr_writel(DDRCAPB_BASE + (0x408 + 0xb0 * port), val);
	dwc_debug("port1 write       %x\n", PRJ007_ddr_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port)));
#endif
}

static void ddrc_dfi_init()
{
	unsigned int ddrc_reset = 0;
	unsigned int p[INDEX_EN] = {0}; //ddr parameter


	SET_INNOPHY_REG2((IO_BASE+0x20), 19, 19, 0); // ISP
	SET_INNOPHY_REG2((IO_BASE+0x20), 28, 28, 0); // VPU
	SET_INNOPHY_REG2((IO_BASE+0x28), 03, 03, 0); // IVDC
	SET_INNOPHY_REG2((IO_BASE+0x28), 11, 11, 0); // TNPU

	/* ddrc&p soft reset */
	ddrc_reset = readl(CPM_BASE + CPM_SRBC0);
	ddrc_reset |= 0x18; /* bit3/4 apb reset; ddrc & ddrp soft reset */
	writel(ddrc_reset, CPM_BASE + CPM_SRBC0);

	udelay(500);
	ddrc_reset &= ~(0x8); /* apb reset ok */
	writel(ddrc_reset, CPM_BASE + CPM_SRBC0);
	udelay(500);

	/* Start ddr control initialization */
	PRJ007_ddr_writel(SWCTLSTATIC, 0x1);
	PRJ007_ddr_writel(SWCTL, 0x0);

	ddrc_init();
	PRJ007_ddrc_dump();
	ddrp_init(ddrc_reset);
	/* ODT & skip training set */
	get_ddr_par(p, sizeof(p)); // ddr parameter in efuse
	ddrp_set_drv_odt(p);
	ddrp_trining(p);

#ifndef CONFIG_ZERATUL
	SET_INNOPHY_REG2((IO_BASE+0x20), 19, 19, 1); // ISP
	SET_INNOPHY_REG2((IO_BASE+0x20), 28, 28, 1); // VPU
	SET_INNOPHY_REG2((IO_BASE+0x28), 03, 03, 1); // IVDC
	SET_INNOPHY_REG2((IO_BASE+0x28), 11, 11, 1); // TNPU
#endif

	ddrc_set_port_priority();
	PRJ007_ddr_writel(SWCTLSTATIC, 0x0);
	PRJ007_ddr_writel(SWCTL, 0x1);
	/* DDR controller initialization complete */
}

/* DDR sdram init */
void sdram_init(void)
{
	unsigned int rate = gd->arch.gi->ddrfreq;
	int type;

	printf("sdram init start\n");
	type = get_ddr_type();
#ifndef CONFIG_FPGA
	clk_set_rate(DDR, gd->arch.gi->ddrfreq);
	rate = clk_get_rate(DDR);
#endif
	printf("DDR clk rate %d\n", rate * 2);

	ddrc_dfi_init();
	printf("sdram init Finished\n");
}

phys_size_t initdram(int board_type)
{
#ifdef CONFIG_DDR_HOST_CC
	/* SDRAM size was calculated when compiling. */
#ifndef EMC_LOW_SDRAM_SPACE_SIZE
#define EMC_LOW_SDRAM_SPACE_SIZE 0x10000000 /* 256M */
#endif /* EMC_LOW_SDRAM_SPACE_SIZE */

#ifndef DDR_CHIP_0_SIZE
#define DDR_CHIP_0_SIZE DDR_SIZE
#endif

	unsigned int ram_size;
	ram_size = (unsigned int)(DDR_CHIP_0_SIZE);

	if (ram_size > EMC_LOW_SDRAM_SPACE_SIZE)
		ram_size = EMC_LOW_SDRAM_SPACE_SIZE;

	return ram_size;
#else
	return DDR_SIZE;
#endif
}
