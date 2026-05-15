// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <config.h>
#include <common.h>
#include <linux/delay.h>
#include <ddr/ddr_common.h>
#include <ddr/ddrc.h>
#include <generated/ddr_reg_values.h>
#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/arch/cpm.h>
#include "ddr_config.h"

DECLARE_GLOBAL_DATA_PTR;

static void sys_ddrc_dump(void)
{
	dwc_debug("=============== DDRC Register ================\n");
	sys_ddrc("MSTR        ", MSTR);
	sys_ddrc("INIT1       ", INIT1);
	sys_ddrc("INIT0       ", INIT0);
	sys_ddrc("INIT3       ", INIT3);
	sys_ddrc("INIT4       ", INIT4);
	sys_ddrc("ADDRMAP1    ", ADDRMAP1);
	sys_ddrc("ADDRMAP2    ", ADDRMAP2);
	sys_ddrc("ADDRMAP3    ", ADDRMAP3);
	sys_ddrc("ADDRMAP4    ", ADDRMAP4);
	sys_ddrc("ADDRMAP5    ", ADDRMAP5);
	sys_ddrc("ADDRMAP6    ", ADDRMAP6);
	sys_ddrc("DRAMTMG0    ", DRAMTMG0);
	sys_ddrc("DRAMTMG1    ", DRAMTMG1);
	sys_ddrc("DRAMTMG2    ", DRAMTMG2);
	sys_ddrc("DRAMTMG3    ", DRAMTMG3);
	sys_ddrc("DRAMTMG4    ", DRAMTMG4);
	sys_ddrc("DRAMTMG5    ", DRAMTMG5);
	sys_ddrc("DRAMTMG6    ", DRAMTMG6);
	sys_ddrc("DRAMTMG7    ", DRAMTMG7);
	sys_ddrc("DRAMTMG8    ", DRAMTMG8);
	sys_ddrc("DRAMTMG14   ", DRAMTMG14);
	sys_ddrc("DFITMG0     ", DFITMG0);
	sys_ddrc("DFITMG1     ", DFITMG1);
	sys_ddrc("DFIUPD0     ", DFIUPD0);
	sys_ddrc("RFSHTMG     ", RFSHTMG);
	sys_ddrc("RFSHCTL3    ", RFSHCTL3);
}

static void get_ddr_type(void)
{
	switch (DDR_TYPE)
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
			dwc_debug("unsupport ddr type! %x\n", DDR_TYPE);
	}
}

#ifdef CONFIG_DDR_SOFT_TRAIN
static void ddrp_training_read_calib_bypass(void)
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

static void ddrp_training_read_train_bypass(u32 dqs0, u32 dqs1, u32 dq)
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
	PRINT_DDRP(reg_a_l_cs0_dqs_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dqs_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dm_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq0_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq1_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq2_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq3_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq4_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq5_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq6_invdelayselrx);
	PRINT_DDRP(reg_a_l_cs0_dq7_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dm_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq0_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq1_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq2_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq3_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq4_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq5_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq6_invdelayselrx);
	PRINT_DDRP(reg_a_h_cs0_dq7_invdelayselrx);
}

#if 0
static void ddrp_training_write_train_bypass_dqs(u32 dqs0, u32 dqs1)
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
	PRINT_DDRP(reg_a_l_cs0_dqs_invdelaysel);
	PRINT_DDRP(reg_a_h_cs0_dqs_invdelaysel);
}
#endif

static void ddrp_training_write_train_bypass_dq(u32 dq)
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

	PRINT_DDRP(reg_a_l_cs0_dm_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq0_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq1_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq2_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq3_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq4_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq5_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq6_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq7_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dm_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq0_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq1_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq2_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq3_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq4_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq5_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq6_invdelaysel);
	PRINT_DDRP(reg_a_l_cs0_dq7_invdelaysel);
	dwc_debug("==============================================\n");
}
#endif

#ifdef CONFIG_DDR_HARDWARE_TRAIN
static void ddrp_training_write_leveling(void)
{
	dwc_debug("=============== Write Leveling ===============\n");
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

	PRINT_DDRP(reg_a_l_tdqs_invdelaysel0);
	PRINT_DDRP(reg_a_h_tdqs_invdelaysel0);
}

static void ddrp_training_read_training(void)
{
	dwc_debug("=============== Read Training ================\n");
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x1);
	SET_INNOPHY_REG(reg_rdtrain_cs_sel, 0x2);
	SET_INNOPHY_REG(reg_a_l_rd_train_dqs_default, 0x1f);
	SET_INNOPHY_REG(reg_a_h_rd_train_dqs_default, 0x1f);
	SET_INNOPHY_REG(reg_dq_rd_train_en, 0x1);
	dwc_debug("wait_read_training_done:   \n");
	BNE_INNOPHY_REG(train_true_done, 0x1);

	PRINT_DDRP(reg_train_error_for_rd_byte);

	SET_INNOPHY_REG(reg_dq_rd_train_en, 0x0);
	SET_INNOPHY_REG(reg_rd_train_dqs_range_bypass, 0x0);
	SET_INNOPHY_REG(reg_rdtrain_cs_sel, 0x0);
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x0);

	PRINT_DDRP(reg_a_l_train_min_for_rd_dqs);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq0);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq1);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq2);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq3);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq4);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq5);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq6);
	PRINT_DDRP(reg_a_l_train_min_for_rd_dq7);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dqs);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq0);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq1);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq2);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq3);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq4);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq5);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq6);
	PRINT_DDRP(reg_a_h_train_min_for_rd_dq7);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dqs);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq0);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq1);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq2);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq3);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq4);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq5);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq6);
	PRINT_DDRP(reg_a_l_train_max_for_rd_dq7);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dqs);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq0);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq1);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq2);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq3);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq4);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq5);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq6);
	PRINT_DDRP(reg_a_h_train_max_for_rd_dq7);
}

static void ddrp_training_write_training(void)
{
	dwc_debug("=============== Write Training ===============\n");
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
	SET_INNOPHY_REG(reg_dq_wr_train_auto, 0x0); // must set to 0 after write training
	SET_INNOPHY_REG(reg_wr_train_dqs_default_bypass, 0x0);
	SET_INNOPHY_REG(reg_wrtrain_cs_sel, 0x0);
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x0);

	PRINT_DDRP(train_step1_error);
	PRINT_DDRP(train_step2_error);
	PRINT_DDRP(train_step2_error);
	PRINT_DDRP(reg_wr_train_error_byte);

	PRINT_DDRP(reg_a_l_train_min_for_dqs);
	PRINT_DDRP(reg_a_l_train_min_for_dq0);
	PRINT_DDRP(reg_a_l_train_min_for_dq1);
	PRINT_DDRP(reg_a_l_train_min_for_dq2);
	PRINT_DDRP(reg_a_l_train_min_for_dq3);
	PRINT_DDRP(reg_a_l_train_min_for_dq4);
	PRINT_DDRP(reg_a_l_train_min_for_dq5);
	PRINT_DDRP(reg_a_l_train_min_for_dq6);
	PRINT_DDRP(reg_a_l_train_min_for_dq7);
	PRINT_DDRP(reg_a_h_train_min_for_dqs);
	PRINT_DDRP(reg_a_h_train_min_for_dq0);
	PRINT_DDRP(reg_a_h_train_min_for_dq1);
	PRINT_DDRP(reg_a_h_train_min_for_dq2);
	PRINT_DDRP(reg_a_h_train_min_for_dq3);
	PRINT_DDRP(reg_a_h_train_min_for_dq4);
	PRINT_DDRP(reg_a_h_train_min_for_dq5);
	PRINT_DDRP(reg_a_h_train_min_for_dq6);
	PRINT_DDRP(reg_a_h_train_min_for_dq7);
	PRINT_DDRP(reg_a_l_train_max_for_dqs);
	PRINT_DDRP(reg_a_l_train_max_for_dq0);
	PRINT_DDRP(reg_a_l_train_max_for_dq1);
	PRINT_DDRP(reg_a_l_train_max_for_dq2);
	PRINT_DDRP(reg_a_l_train_max_for_dq3);
	PRINT_DDRP(reg_a_l_train_max_for_dq4);
	PRINT_DDRP(reg_a_l_train_max_for_dq5);
	PRINT_DDRP(reg_a_l_train_max_for_dq6);
	PRINT_DDRP(reg_a_l_train_max_for_dq7);
	PRINT_DDRP(reg_a_h_train_max_for_dqs);
	PRINT_DDRP(reg_a_h_train_max_for_dq0);
	PRINT_DDRP(reg_a_h_train_max_for_dq1);
	PRINT_DDRP(reg_a_h_train_max_for_dq2);
	PRINT_DDRP(reg_a_h_train_max_for_dq3);
	PRINT_DDRP(reg_a_h_train_max_for_dq4);
	PRINT_DDRP(reg_a_h_train_max_for_dq5);
	PRINT_DDRP(reg_a_h_train_max_for_dq6);
	PRINT_DDRP(reg_a_h_train_max_for_dq7);
}
#endif

static void ddrp_dqs_calibration(void)
{
	dwc_debug("=============== Calibration ==================\n");
	// The related registers hold default value
	/* dqs_gating（calibration） */
	SET_INNOPHY_REG(reg_phy_refresh_en, 0x1);
	SET_INNOPHY_REG(reg_calcs_sel, 0x2); // choose rank0
	SET_INNOPHY_REG(reg_start_calib, 0x1);

	dwc_debug("wait_dqs_gating_done1:\n");
	BNE_INNOPHY_REG(calib_end, 0x1);

	dwc_debug("wait_dqs_gating_done2:\n");
	BNE_INNOPHY_REG(calib_done_byte, 0x3);
	PRINT_DDRP(calib_error);

	SET_INNOPHY_REG(reg_phy_refresh_en, 0x0);
	SET_INNOPHY_REG(reg_start_calib, 0x0);
	SET_INNOPHY_REG(reg_calcs_sel, 0x0);
	PRINT_DDRP(reg_a_l_cycsel);
	PRINT_DDRP(reg_a_l_ophsel);
	PRINT_DDRP(reg_a_l_dllsel);
	PRINT_DDRP(reg_a_h_cycsel);
	PRINT_DDRP(reg_a_h_ophsel);
	PRINT_DDRP(reg_a_h_dllsel);
}

#ifdef CONFIG_DDR_SOFT_TRAIN
static int ddr_mem_pattern(void)
{
	volatile u32 tmp = 0;
	u32 tmp_data = 0, ret = 0;
	u32 td = 0xa0000080;

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

static void ddrp_training_soft_read_train(u32 dq)
{
	u32 min = 0xff, max = 0, med = 0x55, width = 0;
	u32 dqs = 0;

	ddrp_training_read_train_bypass(dqs, dqs, dq);

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

static void ddrp_training_soft_write_train(void)
{
	u32 min = 0xff, max = 0, med = 0x7, width = 0;
	u32 dqs = 0;

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

static void ddrp_training(u32 *p)
{
	u32 stat = 0;

	ddrc_writel(PCTRL0, 0x1);
	ddrc_writel(PCTRL1, 0x1);
	ddrc_writel(PCTRL2, 0x1);
	ddrc_writel(PCTRL3, 0x1);
	ddrc_writel(PCTRL4, 0x1);
	ddrc_writel(PCTRL5, 0x1);
	ddrc_writel(PCTRL6, 0x1);

	SET_INNOPHY_REG(reg_dq_wr_train_auto, 0x0);

	stat = ddrc_readl(STAT);
	while (!(stat & 0x1))
	{
		stat = ddrc_readl(STAT);
	}

	if ((DDR_TYPE == TYPE_DDR3) || (DDR_TYPE == TYPE_LPDDR3))
		ddrp_training_write_leveling();

	ddrp_dqs_calibration();

	if ((DDR_TYPE == TYPE_DDR3) || (DDR_TYPE == TYPE_LPDDR3))
	{
		ddrp_training_read_training();
		ddrp_training_write_training();
	}
	else if ((DDR_TYPE == TYPE_DDR2) || (DDR_TYPE == TYPE_LPDDR2))
	{
		printf("DDR soft training ...\n");
		ddrp_training_read_calib_bypass();
		ddrp_training_soft_read_train(0x40);
		ddrp_training_write_train_bypass_dq(0x80);
		ddrp_training_soft_write_train();
	}
	dwc_debug("==============================================\n");
}

static void ddrc_init(u32 kgd_rtt_dic)
{
	u32 data = 0;

	ddrc_writel(MSTR, DDRC_MSTR);

	ddrc_writel(INIT0, DDRC_INIT0);
	ddrc_writel(INIT1, DDRC_INIT1);

	if (DDR_TYPE == TYPE_LPDDR2)
		ddrc_writel(INIT2, DDRC_INIT2);

	if (DDR_TYPE == TYPE_DDR3)
		ddrc_writel(INIT3, ((DDRC_INIT3 & (~0x266)) | kgd_rtt_dic));
	else if (DDR_TYPE == TYPE_DDR2)
		ddrc_writel(INIT3, ((DDRC_INIT3 & (~0x46)) | kgd_rtt_dic));
	else
		ddrc_writel(INIT3, DDRC_INIT3);

	if ((DDR_TYPE == TYPE_LPDDR3) || (DDR_TYPE == TYPE_LPDDR2))
		ddrc_writel(INIT4, ((DDRC_INIT4 & (~0xf0000)) | (kgd_rtt_dic << 16)));
	else
		ddrc_writel(INIT4, DDRC_INIT4);

	if (DDR_TYPE != TYPE_DDR2)
		ddrc_writel(INIT5, DDRC_INIT5);

	ddrc_writel(ADDRMAP1, DDRC_ADDRMAP1);
	ddrc_writel(ADDRMAP2, DDRC_ADDRMAP2);
	ddrc_writel(ADDRMAP3, DDRC_ADDRMAP3);
	ddrc_writel(ADDRMAP4, DDRC_ADDRMAP4);
	ddrc_writel(ADDRMAP5, DDRC_ADDRMAP5);
	ddrc_writel(ADDRMAP6, DDRC_ADDRMAP6);

	ddrc_writel(DRAMTMG0, DDRC_TIMING0);
	ddrc_writel(DRAMTMG1, DDRC_TIMING1);
	ddrc_writel(DRAMTMG2, DDRC_TIMING2);
	ddrc_writel(DRAMTMG3, DDRC_TIMING3);
	ddrc_writel(DRAMTMG4, DDRC_TIMING4);
	ddrc_writel(DRAMTMG5, DDRC_TIMING5);
	ddrc_writel(DRAMTMG7, DDRC_TIMING7);
	if ((DDR_TYPE == TYPE_DDR3) || (DDR_TYPE == TYPE_DDR2))
		ddrc_writel(DRAMTMG8, DDRC_TIMING8);
	else if ((DDR_TYPE == TYPE_LPDDR2) || (DDR_TYPE == TYPE_LPDDR3))
	{
		ddrc_writel(DRAMTMG6, DDRC_TIMING6);
		ddrc_writel(DRAMTMG14, DDRC_TIMING14);
	}

	if (DDR_TYPE == TYPE_LPDDR2)
	{
		ddrc_writel(DFITMG0, 6 << 24 | 2 << 16 | 0 << 8 | 0 << 0);
		ddrc_writel(DFITMG1, 8 << 16 | 3 << 8 | 7 << 0);
		ddrc_writel(DFIUPD0, 1 << 31 | 0x40 << 16 | 3 << 0);
	}
	else
	{
		ddrc_writel(DFITMG0, DDRC_DFITMG0);
		ddrc_writel(DFITMG1, DDRC_DFITMG1);
		ddrc_writel(DFIUPD0, DDRC_DFIUPD0);
	}

	ddrc_writel(RFSHTMG, DDRC_RFSHTMG);
	ddrc_writel(RFSHCTL3, DDRC_RFSHCTL3);

	if ((DDR_TYPE == TYPE_DDR2) || (DDR_TYPE == TYPE_LPDDR2))
		ddrc_writel(ODTCFG, DDRC_ODTCFG);

	data = ddrc_readl(SCHED);
	data &= (~(1 << 2));
	ddrc_writel(SCHED, data);
}

static void ddrp_map_set(u32 ddr_id)
{
	unsigned char w631gu6ng_map[] = W631GU6NG_MAP;
	unsigned char nk5cc64m16hq3_map[] = NK5CC64M16HQ3_MAP;
	unsigned char m14d5121632a_map[] = M14D5121632A_MAP;
	unsigned char *ddrp_map;

	ddrp_map = w631gu6ng_map;

	switch(ddr_id)
	{
		case WINBOND_20NM_1G_D3_1066:
			ddrp_map = w631gu6ng_map;
			break;
		case NTC_20NM_1G_D3:
			ddrp_map = nk5cc64m16hq3_map;
			break;
		case ESMT_25NM_512_D2:
		case WINBOND_25NM_512_D2:
		case ETRON_25NM_512_D2:
			ddrp_map = m14d5121632a_map;
			break;
		default:
			printf("unknow kgd info(%x)\n", ddr_id);
#if defined(CONFIG_DDR3_W631GU6NG)
			ddrp_map = w631gu6ng_map;
#elif defined(CONFIG_DDR_TYPE_DDR3)
			ddrp_map = nk5cc64m16hq3_map;
#elif defined(CONFIG_DDR_TYPE_DDR2)
			ddrp_map = m14d5121632a_map;
#endif
	}

	SET_INNOPHY_REG(reg_cmd0_wrap_sel, (u32)ddrp_map[0]);
	SET_INNOPHY_REG(reg_cmd1_wrap_sel, (u32)ddrp_map[1]);
	SET_INNOPHY_REG(reg_cmd2_wrap_sel, (u32)ddrp_map[2]);
	SET_INNOPHY_REG(reg_cmd3_wrap_sel, (u32)ddrp_map[3]);
	SET_INNOPHY_REG(reg_cmd4_wrap_sel, (u32)ddrp_map[4]);
	SET_INNOPHY_REG(reg_cmd5_wrap_sel, (u32)ddrp_map[5]);
	SET_INNOPHY_REG(reg_cmd6_wrap_sel, (u32)ddrp_map[6]);
	SET_INNOPHY_REG(reg_cmd7_wrap_sel, (u32)ddrp_map[7]);
	SET_INNOPHY_REG(reg_cmd8_wrap_sel, (u32)ddrp_map[8]);
	SET_INNOPHY_REG(reg_cmd9_wrap_sel, (u32)ddrp_map[9]);
	SET_INNOPHY_REG(reg_cmd10_wrap_sel, (u32)ddrp_map[10]);
	SET_INNOPHY_REG(reg_cmd11_wrap_sel, (u32)ddrp_map[11]);
	SET_INNOPHY_REG(reg_cmd12_wrap_sel, (u32)ddrp_map[12]);
	SET_INNOPHY_REG(reg_cmd13_wrap_sel, (u32)ddrp_map[13]);
	SET_INNOPHY_REG(reg_cmd16_wrap_sel, (u32)ddrp_map[14]); // WEB
	SET_INNOPHY_REG(reg_cmd17_wrap_sel, (u32)ddrp_map[15]); // CASB
	SET_INNOPHY_REG(reg_cmd18_wrap_sel, (u32)ddrp_map[16]); // RASB
	SET_INNOPHY_REG(reg_cmd19_wrap_sel, (u32)ddrp_map[17]); // BA0
	SET_INNOPHY_REG(reg_cmd20_wrap_sel, (u32)ddrp_map[18]); // BA1
	SET_INNOPHY_REG(reg_cmd21_wrap_sel, (u32)ddrp_map[19]); // BA2
	SET_INNOPHY_REG(reg_a_l_dm_bit_wrap_sel, (u32)ddrp_map[20]);
	SET_INNOPHY_REG(reg_a_l_dq0_bit_wrap_sel, (u32)ddrp_map[21]);
	SET_INNOPHY_REG(reg_a_l_dq1_bit_wrap_sel, (u32)ddrp_map[22]);
	SET_INNOPHY_REG(reg_a_l_dq2_bit_wrap_sel, (u32)ddrp_map[23]);
	SET_INNOPHY_REG(reg_a_l_dq3_bit_wrap_sel, (u32)ddrp_map[24]);
	SET_INNOPHY_REG(reg_a_l_dq4_bit_wrap_sel, (u32)ddrp_map[25]);
	SET_INNOPHY_REG(reg_a_l_dq5_bit_wrap_sel, (u32)ddrp_map[26]);
	SET_INNOPHY_REG(reg_a_l_dq6_bit_wrap_sel, (u32)ddrp_map[27]);
	SET_INNOPHY_REG(reg_a_l_dq7_bit_wrap_sel, (u32)ddrp_map[28]);
	SET_INNOPHY_REG(reg_a_h_dm_bit_wrap_sel, (u32)ddrp_map[29]);
	SET_INNOPHY_REG(reg_a_h_dq0_bit_wrap_sel, (u32)ddrp_map[30]);
	SET_INNOPHY_REG(reg_a_h_dq1_bit_wrap_sel, (u32)ddrp_map[31]);
	SET_INNOPHY_REG(reg_a_h_dq2_bit_wrap_sel, (u32)ddrp_map[32]);
	SET_INNOPHY_REG(reg_a_h_dq3_bit_wrap_sel, (u32)ddrp_map[33]);
	SET_INNOPHY_REG(reg_a_h_dq4_bit_wrap_sel, (u32)ddrp_map[34]);
	SET_INNOPHY_REG(reg_a_h_dq5_bit_wrap_sel, (u32)ddrp_map[35]);
	SET_INNOPHY_REG(reg_a_h_dq6_bit_wrap_sel, (u32)ddrp_map[36]);
	SET_INNOPHY_REG(reg_a_h_dq7_bit_wrap_sel, (u32)ddrp_map[37]);

	dwc_debug("=============== DDR PHY WRAP =================\n");
	PRINT_DDRP(reg_cmd0_wrap_sel);
	PRINT_DDRP(reg_cmd1_wrap_sel);
	PRINT_DDRP(reg_cmd2_wrap_sel);
	PRINT_DDRP(reg_cmd3_wrap_sel);
	PRINT_DDRP(reg_cmd4_wrap_sel);
	PRINT_DDRP(reg_cmd5_wrap_sel);
	PRINT_DDRP(reg_cmd6_wrap_sel);
	PRINT_DDRP(reg_cmd7_wrap_sel);
	PRINT_DDRP(reg_cmd8_wrap_sel);
	PRINT_DDRP(reg_cmd9_wrap_sel);
	PRINT_DDRP(reg_cmd10_wrap_sel);
	PRINT_DDRP(reg_cmd11_wrap_sel);
	PRINT_DDRP(reg_cmd12_wrap_sel);
	PRINT_DDRP(reg_cmd13_wrap_sel);
	PRINT_DDRP(reg_cmd16_wrap_sel);
	PRINT_DDRP(reg_cmd17_wrap_sel);
	PRINT_DDRP(reg_cmd18_wrap_sel);
	PRINT_DDRP(reg_cmd19_wrap_sel);
	PRINT_DDRP(reg_cmd20_wrap_sel);
	PRINT_DDRP(reg_cmd21_wrap_sel);
	PRINT_DDRP(reg_a_l_dm_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq0_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq1_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq2_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq3_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq4_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq5_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq6_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq7_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dm_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq0_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq1_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq2_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq3_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq4_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq5_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq6_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq7_bit_wrap_sel);
}

static void ddrp_init(u32 ddrc_reset)
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

	SET_INNOPHY_REG(reg_pllcpp_bias_dqcmd, 0x4); // inno: 获得更大带宽
	SET_INNOPHY_REG(reg_pllcpi_bias_fsp0, 0x5); // inno: 获得较好的jitter性能
	SET_INNOPHY_REG(reg_pllcpi_bias_fsp1, 0x5); // inno: 获得较好的jitter性能
	SET_INNOPHY_REG(reg_pllcpi_bias_fsp2, 0x5); // inno: 获得较好的jitter性能
	SET_INNOPHY_REG(reg_pllcpi_bias_fsp3, 0x5); // inno: 获得较好的jitter性能

	dwc_debug("=============== DDR PHY Pll Config ===========\n");
	PRINT_DDRP(reg_pllpostdiven_fsp0);
	PRINT_DDRP(reg_pllpostdiven_fsp1);
	PRINT_DDRP(reg_pllpostdiven_fsp2);
	PRINT_DDRP(reg_pllpostdiven_fsp3);
	PRINT_DDRP(reg_pllpostdiv_fsp0);
	PRINT_DDRP(reg_pllpostdiv_fsp1);
	PRINT_DDRP(reg_pllpostdiv_fsp2);
	PRINT_DDRP(reg_pllpostdiv_fsp3);

	if (!gd->arch.gi->soc.xburst.PRJ007)
		ddrp_map_set(gd->arch.gi->soc.xburst.ddr_id);

	// SET_INNOPHY_REG(reg_a_l_rxen_lp4, 0x0);
	// SET_INNOPHY_REG(reg_a_h_rxen_lp4, 0x0);

	ddrc_reset &= ~(0x10); /* ddrc reset ok */
	writel(ddrc_reset, CPM_BASE + CPM_SRBC0);
	udelay(500);
	data = ddrc_readl(DFIMISC);
	data |= 0x20;
	ddrc_writel(DFIMISC, data);

	dwc_debug("wait_dfi_init_complete:\n");
	while (!(ddrc_readl(DFISTAT) & 0x1))
		;

	dwc_debug("INNO PHY DDR DFI complete...\n");
	data = ddrc_readl(DFIMISC);
	data &= ~(1 << 5);
	ddrc_writel(DFIMISC, data);
}

static void ddrp_set_drv_odt(u32 *p)
{
	SET_INNOPHY_REG(reg_a_l_abutodtpudq_reg,    p[ODT_PU]);    // [ 7: 0]ODT pull up     The bit[5:0] of the ODT pull-up resistance of byte0.
	SET_INNOPHY_REG(reg_a_h_abutodtpudq_reg,    p[ODT_PU]);    // [15: 8]ODT pull up     The bit[5:0] of the ODT pull-up resistance of byte1.
	SET_INNOPHY_REG(reg_a_l_abutodtpddq_reg,    p[ODT_PD]);    // [ 7: 0]ODT pull down   The bit[5:0] of the ODT pull-down resistance of byte0.
	SET_INNOPHY_REG(reg_a_h_abutodtpddq_reg,    p[ODT_PD]);    // [15: 8]ODT pull down   The bit[5:0] of the ODT pull-down resistance of byte1.
	SET_INNOPHY_REG(reg_cmd_abutprcomp_reg,     p[CMD_RC_PU]); // [ 7: 0]CMD pull up     The bit[5:0] of the pull-up resistance of CMD except CK.
	SET_INNOPHY_REG(reg_cmd_abutnrcomp_reg,     p[CMD_RC_PD]); // [ 7: 0]CMD pull down   The bit[5:0] of the pull-down resistance of CMD except CK.
	SET_INNOPHY_REG(reg_cmd_abutprcomp_ck0_reg, p[CLK_RC_PU]); // [ 7: 0]CK0 pull up     The bit[5:0] of the pull-up resistance of CK.
	SET_INNOPHY_REG(reg_cmd_abutnrcomp_ck0_reg, p[CLK_RC_PD]); // [ 7: 0]CK0 pull down   The bit[5:0] of the pull-down resistance of CK.
	SET_INNOPHY_REG(reg_a_l_abutprcompdq_reg,   p[DQX_RC_PU]); // [ 7: 0]DQ  pull up     The bit[5:0] of the driver pull-up resistance of byte0.
	SET_INNOPHY_REG(reg_a_h_abutprcompdq_reg,   p[DQX_RC_PU]); // [15: 8]DQ  pull up     The bit[5:0] of the driver pull-up resistance of byte1.
	SET_INNOPHY_REG(reg_a_l_abutnrcompdq_reg,   p[DQX_RC_PD]); // [ 7: 0]DQ  pull down   The bit[5:0] of the driver pull-down resistance of byte0.
	SET_INNOPHY_REG(reg_a_h_abutnrcompdq_reg,   p[DQX_RC_PD]); // [15: 8]DQ  pull down   The bit[5:0] of the driver pull-down resistance of byte1.

	dwc_debug("=============== DDR IO Drive Strength ========\n");
	PRINT_DDRP(reg_a_l_abutodtpudq_reg);
	PRINT_DDRP(reg_a_h_abutodtpudq_reg);
	PRINT_DDRP(reg_a_l_abutodtpddq_reg);
	PRINT_DDRP(reg_a_h_abutodtpddq_reg);
	PRINT_DDRP(reg_cmd_abutprcomp_reg);
	PRINT_DDRP(reg_cmd_abutnrcomp_reg);
	PRINT_DDRP(reg_cmd_abutprcomp_ck0_reg);
	PRINT_DDRP(reg_cmd_abutnrcomp_ck0_reg);
	PRINT_DDRP(reg_a_l_abutprcompdq_reg);
	PRINT_DDRP(reg_a_h_abutprcompdq_reg);
	PRINT_DDRP(reg_a_l_abutnrcompdq_reg);
	PRINT_DDRP(reg_a_h_abutnrcompdq_reg);
}

static void ddrc_set_port_priority(void)
{
	u32 port = 0, val = 0;

	/* port aging */
	/* ISP port high */
	port = 0;
	val = ddrc_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val &= ~(0x3ff);
	ddrc_writel(DDRCAPB_BASE + (0x404 + 0xb0 * port), val);
	dwc_debug("port0 read       %x\n", ddrc_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port)));

	val = ddrc_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val &= ~(0x3ff);
	ddrc_writel(DDRCAPB_BASE + (0x408 + 0xb0 * port), val);
	dwc_debug("port0 write       %x\n", ddrc_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port)));
	/* CPU port high*/
	port = 6;
	val = ddrc_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val &= ~(0x3ff);
	ddrc_writel(DDRCAPB_BASE + (0x404 + 0xb0 * port), val);
	dwc_debug("port6 read       %x\n", ddrc_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port)));

	val = ddrc_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port));
	val |= ((0 << 13) | (1 << 12));
	val &= ~(0x3ff);
	ddrc_writel(DDRCAPB_BASE + (0x408 + 0xb0 * port), val);
	dwc_debug("port6 write       %x\n", ddrc_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port)));

	/* VPU port high*/
	port = 4;
	val = 0x500d;
	ddrc_writel(DDRCAPB_BASE + (0x404 + 0xb0 * port), val);
	dwc_debug("port4 read       %x\n", ddrc_readl(DDRCAPB_BASE + (0x404 + 0xb0 * port)));

	ddrc_writel(DDRCAPB_BASE + (0x408 + 0xb0 * port), val);
	dwc_debug("port4 write       %x\n", ddrc_readl(DDRCAPB_BASE + (0x408 + 0xb0 * port)));

	/* map_region: 13~15 HPR; 0~12 LPR */
	for(port = 0; port < 7; port++){
		if(port == 5)
			continue;
		val = ddrc_readl(DDRCAPB_BASE + (0x494 + 0xb0 * port));
		val |= ((2 << 20) | (0 << 16) | 12);
		ddrc_writel(DDRCAPB_BASE + (0x494 + 0xb0 * port), val);
	}

	/* ar/w qos: CPU 15; ISP 14; VPU 13 */
	writel(0xee, (void __iomem *)(QOS_BASE + 0x30)); //ISP 14
	writel(0xffdd, (void __iomem *)(QOS_BASE + 0x70)); //CPU 15, VPU 13
}

static void ddrc_dfi_init(void)
{
	u32 ddrc_reset = 0;
	u32 p[INDEX_EN] = {0}; //ddr parameter

	SET_INNOPHY_REG2((IO_BASE + 0x20), 19, 19, 0); // ISP
	SET_INNOPHY_REG2((IO_BASE + 0x20), 28, 28, 0); // VPU
	SET_INNOPHY_REG2((IO_BASE + 0x28), 03, 03, 0); // IVDC
	SET_INNOPHY_REG2((IO_BASE + 0x28), 11, 11, 0); // TNPU

	/* ddrc&p soft reset */
	ddrc_reset = readl(CPM_BASE + CPM_SRBC0);
	ddrc_reset |= 0x18; /* bit3/4 apb reset; ddrc & ddrp soft reset */
	writel(ddrc_reset, CPM_BASE + CPM_SRBC0);

	udelay(500);
	ddrc_reset &= ~(0x8); /* apb reset ok */
	writel(ddrc_reset, CPM_BASE + CPM_SRBC0);
	udelay(500);

	/* Start ddr control initialization */
	ddrc_writel(SWCTLSTATIC, 0x1);
	ddrc_writel(SWCTL, 0x0);

	get_ddr_par(p, sizeof(p)); // ddr parameter in efuse
	ddrc_init(p[KGD_RTT_DIC]);
	sys_ddrc_dump();
	ddrp_init(ddrc_reset);
	/* ODT & skip training set */
	ddrp_set_drv_odt(p);
	ddrp_training(p);

	SET_INNOPHY_REG2((IO_BASE + 0x20), 19, 19, 1); // ISP
	SET_INNOPHY_REG2((IO_BASE + 0x20), 28, 28, 1); // VPU
	SET_INNOPHY_REG2((IO_BASE + 0x28), 03, 03, 1); // IVDC
	SET_INNOPHY_REG2((IO_BASE + 0x28), 11, 11, 1); // TNPU

	ddrc_set_port_priority();
	ddrc_writel(SWCTLSTATIC, 0x0);
	ddrc_writel(SWCTL, 0x1);
	/* DDR controller initialization complete */
}

#ifdef CONFIG_DDR_PARAMS_STRUCT
void matches_ddr_params(u32 ddr_id)
{
	u32 size = 0;
	u32 found = 0;
	u32 i;

	size = ARRAY_SIZE(supported_ddr_reg_values);

	if (size == 1)
	{
		found = 1;
		global_reg_value = supported_ddr_reg_values;
		return ;
	}
	else
	{
		for (i = 0; i < size; i++)
		{
			if (ddr_id == supported_ddr_reg_values[i].h.id)
			{
				global_reg_value = &supported_ddr_reg_values[i];
				found = 1;
				break;
			}
		}
		if (!found)
			printf("unknown kgd info %x\n", ddr_id);
	}

}
#endif


/* DDR sdram init */
void sdram_init(void)
{
	u32 rate = 0;

	printf("sdram init start\n");
	
	matches_ddr_params(gd->arch.gi->soc.xburst.ddr_id);
	get_ddr_type();
#ifndef CONFIG_FPGA
	_clk_set_rate(DDR, gd->arch.gi->ddrfreq);
	rate = _clk_get_rate(DDR);
#endif
	printf("DDR clk rate %d\n", rate * 2);
	printf("%08x ", gd->arch.gi->soc.d32);

	ddrc_dfi_init();
	printf("sdram init Finished\n");
}

phys_size_t initdram(int board_type)
{
	return DDR_SIZE;
}

ulong board_get_usable_ram_top(ulong total_size)
{
	unsigned long top = CFG_SYS_SDRAM_BASE + CFG_SDRAM_MAX_SIZE;

	return (gd->ram_top > top) ? top : gd->ram_top;
}

int dram_init(void)
{
	gd->ram_size = initdram(0);
	return 0;
}

