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

#ifndef CONFIG_DDR_SOFT_TRAIN
#define ddrp_trining_d2lpd2_read_calib_bypass()
#define ddrp_trining_d2lpd2_read_train_bypass(dqs0, dqs1, dq)
#define ddrp_trining_d2lpd2_soft_read_train(dq)
#define ddrp_trining_d2lpd2_write_train_bypass_dq(dq)
#define ddrp_trining_d2lpd2_soft_write_train()
#define ddr_mem_pattern()
#endif

#ifndef CONFIG_DDR_HARDWARE_TRAIN
#define ddrp_trining_d3lpd3_write_leveling()
#define ddrp_trining_d3lpd3_read_training()
#define ddrp_trining_d3lpd3_write_training()
#endif

#if defined(CONFIG_DDR_SOFT_TRAIN)
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

static void ddrp_trining_d2lpd2_soft_read_train(unsigned int dq)
{
	unsigned int min = 0xff, max = 0, med = 0x55, width = 0;
	unsigned int dqs = 0;

	ddrp_trining_d2lpd2_read_train_bypass(dqs, dqs, dq);

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

#if defined(CONFIG_DDR_HARDWARE_TRAIN)
static void ddrp_trining_d3lpd3_write_leveling()
{
	dwc_debug("=============== Write Leveling ===============\n");
	/* write_leveling */
	SET_INNOPHY_REG(reg_wlcs_sel, 0x2);
	if(DDR_TYPE == TYPE_DDR3)
	{
		// For DDR3/4, bit[13:0] should keep the same value with MR1[13:0],and this register[15:14] should be set to 2’b01.
		SET_INNOPHY_REG(reg_wl_loadmode, ((DDRC_INIT3 & 0x3fff) | (1 << 14)));
	}
	else if(DDR_TYPE == TYPE_LPDDR3)
	{
		// For LPDDR3/4(X), bit[7:0] should keep the same value with the MR2[7:0],bit[15:8] should be set to 8’h0.
		SET_INNOPHY_REG(reg_wl_loadmode, (DDRC_INIT3 & 0xff));
	}

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

static void ddrp_trining_d3lpd3_write_training()
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

static void ddrp_dqs_calibration()
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

void ddrp_trining(unsigned int *p)
{
	unsigned int stat = 0;

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

	ddrp_dqs_calibration();

	if ((DDR_TYPE == TYPE_DDR3) || (DDR_TYPE == TYPE_LPDDR3))
	{
		ddrp_trining_d3lpd3_write_leveling();
		ddrp_trining_d3lpd3_read_training();
		ddrp_trining_d3lpd3_write_training();
	}
	else if ((DDR_TYPE == TYPE_DDR2) || (DDR_TYPE == TYPE_LPDDR2))
	{
		ddrp_trining_d2lpd2_read_calib_bypass();
		printf("DDR soft training ...\n");
		ddrp_trining_d2lpd2_soft_read_train(0x40);
		ddrp_trining_d2lpd2_write_train_bypass_dq(0x80);
		ddrp_trining_d2lpd2_soft_write_train();
	}

	dwc_debug("==============================================\n");
}
