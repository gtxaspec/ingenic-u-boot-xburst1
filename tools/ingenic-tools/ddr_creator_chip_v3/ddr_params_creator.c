#include "ddr_params_creator.h"

unsigned int __ps_per_tck = -1;
/* ddr_params_creator.c */
static struct ddr_creator_ops *p_ddr_creator[5] = {NULL};
static int ops_count = 0;

int RoundDown(int a, int b)
{
		return a / b;
}

int RoundUp(int a, int b)
{
	if(a % b)
		return a / b + 1;
	else
		return a / b;

	// return (2 * a + b) / (2 * b); // return rounded value
}

int ps2cycle_ceil(int ps, int div_tck)
{
	// return (ps + div_tck * __ps_per_tck - 1) / __ps_per_tck;
	if(div_tck)
		return RoundUp(ps, __ps_per_tck);
	else
		return RoundDown(ps, __ps_per_tck);
}

/* ddr_params_creator.c */
void register_ddr_creator(struct ddr_creator_ops *ops)
{
	if (ops_count < 5)
	{
		p_ddr_creator[ops_count++] = ops;
	}
	else
	{
		out_error("Error: DDR CREATEOR cann't register %d, ops_cout: %d\n", ops->type, ops_count);
	}
}

void fill_ddr_addrmap(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	/*
	-------------------------------------------------------------
	------------*-------ddr3 remap------*-------ddr2 remap-------
	------------*-----------------------*------------------------
	*    DES    *   1G BIT  *   2G BIT  * 256M BIT  * 512M BIT  *
	*-----------*-----------*-----------*-----------*-----------*
	*    COL    *  [ 9: 0]  *  [ 9: 0]  *  [ 8: 0]  *  [ 9: 0]  *
	*-----------*-----------*-----------*-----------*-----------*
	*    BANK   *  [12:10]  *  [12:10]  *  [10: 9]  *  [11:10]  *
	*-----------*-----------*-----------*-----------*-----------*
	*    ROW    *  [25:13]  *  [25:13]  *  [23:11]  *  [24:12]  *
	-------------------------------------------------------------
	*
	-------------------------------------------------------------
	------------*-----ddr3 normal map---*-----ddr2 normal map----
	------------*-----------------------*------------------------
	*    DES    *   1G BIT  *   2G BIT  * 256M BIT  * 512M BIT  *
	*-----------*-----------*-----------*-----------*-----------*
	*    COL    *  [ 9: 0]  *  [ 9: 0]  *  [ 8: 0]  *  [ 9: 0]  *
	*-----------*-----------*-----------*-----------*-----------*
	*    ROW    *  [22:10]  *  [23:10]  *  [21: 9]  *  [22:10]  *
	*-----------*-----------*-----------*-----------*-----------*
	*    BANK   *  [25:23]  *  [26:24]  *  [23:22]  *  [24:23]  *
	-------------------------------------------------------------
	*/

	unsigned int i = 0;
	unsigned int bank_bits = 0, bank_base = 0;
	unsigned int row_base = 0, col_base = 0;
	unsigned int dis_col = 0x1f, dis_row = 0xf, dis_bank = 0x3f; // If unused, set to dis_col/dis_row/dis_bank

	bank_bits = p->DDR_BANK == 8 ? 3 : 2;
#ifdef CONFIG_DDR_REMAP
	bank_base = p->DDR_COL - 2;
	row_base = p->DDR_COL + bank_bits - 6;
#else
	bank_base = p->DDR_COL + p->DDR_ROW - 2;
	row_base = p->DDR_COL - 6;
#endif

	/* set bank addr */
	ddrc->ADDRMAP1.b32.addrmap_bank_b0 = dis_bank;
	ddrc->ADDRMAP1.b32.addrmap_bank_b1 = dis_bank;
	ddrc->ADDRMAP1.b32.addrmap_bank_b2 = dis_bank;

	switch (p->DDR_BANK)
	{
		case 8:
			ddrc->ADDRMAP1.b32.addrmap_bank_b2 = bank_base;
		case 4:
			ddrc->ADDRMAP1.b32.addrmap_bank_b1 = bank_base;
		case 2:
			ddrc->ADDRMAP1.b32.addrmap_bank_b0 = bank_base;
			break;
		default:
			out_error("DDR Error: Does not support %d ddr_bank. %s +%d\n", p->DDR_BANK, __FILE__, __LINE__);
			break;
	}

	/* set col addr */
	ddrc->ADDRMAP2.d32 = col_base; // addrmap_col_b2 b3 b4 b5; base: 2 3 4 5
	ddrc->ADDRMAP3.d32 = col_base; // addrmap_col_b6 b7 b8 b9; base: 6 7 8 9

	// Column address lines are not used by default. 9 10 11
	ddrc->ADDRMAP3.b32.addrmap_col_b9 = dis_col;  // default disable col_b9
	ddrc->ADDRMAP4.b32.addrmap_col_b10 = dis_col; // addrmap_col_b10 b11; base: 10 11; NOT USE
	ddrc->ADDRMAP4.b32.addrmap_col_b11 = dis_col;

	switch (p->DDR_COL)
	{
		case 12:
			ddrc->ADDRMAP4.b32.addrmap_col_b11 = col_base;
		case 11:
			ddrc->ADDRMAP4.b32.addrmap_col_b10 = col_base;
		case 10:
			ddrc->ADDRMAP3.b32.addrmap_col_b9 = col_base;
		case 9:
			break;
		default:
			out_error("DDR Error: Does not support %d column address lines. %s +%d\n", p->DDR_COL, __FILE__, __LINE__);
			break;
	}

	/* set row addr */
	ddrc->ADDRMAP5.b32.addrmap_row_b0 = row_base;
	ddrc->ADDRMAP5.b32.addrmap_row_b1 = row_base;
	ddrc->ADDRMAP5.b32.addrmap_row_b2_b10 = row_base;

	// The default line address line is not used. 11 12 13 14 15
	ddrc->ADDRMAP5.b32.addrmap_row_b11 = dis_row;
	ddrc->ADDRMAP6.b32.addrmap_row_b12 = dis_row;
	ddrc->ADDRMAP6.b32.addrmap_row_b13 = dis_row;
	ddrc->ADDRMAP6.b32.addrmap_row_b14 = dis_row;
	ddrc->ADDRMAP6.b32.addrmap_row_b15 = dis_row;

	switch (p->DDR_ROW)
	{
		case 16:
			ddrc->ADDRMAP6.b32.addrmap_row_b15 = row_base;
		case 15:
			ddrc->ADDRMAP6.b32.addrmap_row_b14 = row_base;
		case 14:
			ddrc->ADDRMAP6.b32.addrmap_row_b13 = row_base;
		case 13:
			ddrc->ADDRMAP6.b32.addrmap_row_b12 = row_base;
		case 12:
			ddrc->ADDRMAP5.b32.addrmap_row_b11 = row_base;
		case 11:
			break;
		default:
			out_error("DDR Error: Does not support %d row address lines. %s +%d\n", p->DDR_ROW, __FILE__, __LINE__);
			break;
	}

}

void fill_tref(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	ddrc->RFSHTMG.b32.t_rfc_nom_x1_sel = 0;

	tmp = ps2cycle_ceil(p->DDR_tREFI, ROUND_DOWN);
	ddrc->RFSHTMG.b32.t_rfc_nom_x1_x32 = RoundDown(RoundDown(tmp, 2), 32);

	tmp = ps2cycle_ceil(p->DDR_tRFC, ROUND_UP);
	ddrc->RFSHTMG.b32.t_rfc_min = RoundUp(tmp, 2);

	ddrc->RFSHCTL3.b32.dis_auto_refresh = 0;
	ddrc->RFSHCTL3.b32.refresh_update_level = 1;
}

void ddrp_set_pll(struct ddrp_reg *ddrp, struct ddr_chip_info *p)
{
/* set Register Configuration for PLL */
#ifdef CONFIG_DDRP_PLL3
	switch(p->freq / 2000000)
	{
		case 50 ... 90:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x3;
			break;
		case 91 ... 180:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x2;
			break;
		case 181 ... 360:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x1;
			break;
		case 361 ... 700:
			ddrp->reg_pllpostdiven = 0x0;
			ddrp->reg_pllpostdiv = 0x0;
			break;
		default:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x1;
	}
#elif defined(CONFIG_DDRP_PLL2)
	switch(p->freq)
	{
		case 250000000 ... 500000000:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x2;
			break;
		case 125000000 ... 249999999:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x3;
			break;
		case 100000000 ... 124999999:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x4;
			break;
		default:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x1;
	}
#elif defined(CONFIG_DDRP_PLL1)
	switch(p->freq)
	{
		case 800000001 ... 1066000000:
			ddrp->reg_pllpostdiven = 0x0;
			ddrp->reg_pllpostdiv = 0x0;
			break;
		case 400000001 ... 800000000:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x1;
			break;
		case 200000001 ... 400000000:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x2;
			break;
		case 100000001 ... 200000000:
			ddrp->reg_pllpostdiven = 0x1;
			ddrp->reg_pllpostdiv = 0x3;
			break;
		default:
			ddrp->reg_pllpostdiven = 0x0;
			ddrp->reg_pllpostdiv = 0x0;
	}
#endif
}

void ddrc_set_d2d3_dfitmg0(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	// Based on Tables 55 and 56 on pages 75 and 76 of the INNO_PKG_DDR_PHY_PRJ2311CCN1_S2311_GF22FDX_V1P0_R20231114.pdf document
	if(p->type == DDR2 || p->type == DDR3)
	{
		ddrc->DFITMG0.b32.dfi_tphy_wrlat = RoundDown((p->DDR_CWL + p->DDR_AL - 3), 2);
		ddrc->DFITMG0.b32.dfi_t_rddata_en = RoundDown((p->DDR_CL + p->DDR_AL - 3), 2);
	}
	else
	{
		ddrc->DFITMG0.b32.dfi_t_rddata_en = RoundDown((p->DDR_RL - 2), 2);
		ddrc->DFITMG0.b32.dfi_tphy_wrlat = RoundDown((p->DDR_WL - 2), 2);
	}
	ddrc->DFITMG0.b32.dfi_tphy_wrdata = 0x0;
	ddrc->DFITMG0.b32.dfi_wrdata_use_dfi_phy_clk = 0x0;
	ddrc->DFITMG0.b32.dfi_rddata_use_dfi_phy_clk = 0x0;
	ddrc->DFITMG0.b32.dfi_t_ctrl_delay = 0x6;
}

void ddrc_set_d2d3_dfitmg1(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->DFITMG1.b32.dfi_t_dram_clk_enable = 0x7;
	ddrc->DFITMG1.b32.dfi_t_dram_clk_disable = 0x3;
	ddrc->DFITMG1.b32.dfi_t_wrdata_delay = 0x8;
	ddrc->DFITMG1.b32.dfi_t_parin_lat = 0x0;
	ddrc->DFITMG1.b32.dfi_t_cmd_lat = 0x0;
}

void ddrc_set_d2d3_dfiupd0(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->DFIUPD0.b32.dfi_t_ctrlup_min = 0x3;
	ddrc->DFIUPD0.b32.dfi_t_ctrlup_max = 0x40;
	ddrc->DFIUPD0.b32.ctrlupd_pre_srx = 0x0;
	ddrc->DFIUPD0.b32.dis_auto_ctrlupd_srx = 0x0;
	ddrc->DFIUPD0.b32.dis_auto_ctrlupd = 0x1;
}

static unsigned int sdram_size(struct ddr_chip_info *p)
{
	unsigned long long bits;
	unsigned long long banks;
	unsigned long long size = 0;
	unsigned long long row, col;

	row = p->DDR_ROW;
	col = p->DDR_COL;

	banks = p->DDR_BANK;
	bits = p->DDR_BIT;
	size = (1 << (row + col)) * bits * banks / 8;

	return size;
}

static void fill_reg_value(struct ddr_reg_value *reg, struct ddrc_reg *ddrc, struct ddrp_reg *ddrp, struct ddr_chip_info *p)
{
	unsigned int i = 0;

	reg->h.freq = p->freq;
	reg->h.id = p->id;
	reg->h.type = p->type;
	reg->h.size = sdram_size(p);

	reg->DDRC_SWCTLSTATIC = ddrc->SWCTLSTAIC.d32;
	reg->DDRC_SWCTL = ddrc->SWCTL.d32;
	reg->DDRC_MSTR = ddrc->MSTR.d32;

	reg->DDRC_INIT0 = ddrc->INIT0.d32;
	reg->DDRC_INIT1 = ddrc->INIT1.d32;
	reg->DDRC_INIT2 = ddrc->INIT2.d32;
	reg->DDRC_INIT3 = ddrc->INIT3.d32;
	reg->DDRC_INIT4 = ddrc->INIT4.d32;
	reg->DDRC_INIT5 = ddrc->INIT5.d32;

	reg->DDRC_TIMING0 = ddrc->TIMING0.d32;
	reg->DDRC_TIMING1 = ddrc->TIMING1.d32;
	reg->DDRC_TIMING2 = ddrc->TIMING2.d32;
	reg->DDRC_TIMING3 = ddrc->TIMING3.d32;
	reg->DDRC_TIMING4 = ddrc->TIMING4.d32;
	reg->DDRC_TIMING5 = ddrc->TIMING5.d32;
	reg->DDRC_TIMING6 = ddrc->TIMING6.d32;
	reg->DDRC_TIMING7 = ddrc->TIMING7.d32;
	reg->DDRC_TIMING8 = ddrc->TIMING8.d32;
	reg->DDRC_TIMING14 = ddrc->TIMING14.d32;

	reg->DDRC_ADDRMAP0 = ddrc->ADDRMAP0.d32;
	reg->DDRC_ADDRMAP1 = ddrc->ADDRMAP1.d32;
	reg->DDRC_ADDRMAP2 = ddrc->ADDRMAP2.d32;
	reg->DDRC_ADDRMAP3 = ddrc->ADDRMAP3.d32;
	reg->DDRC_ADDRMAP4 = ddrc->ADDRMAP4.d32;
	reg->DDRC_ADDRMAP5 = ddrc->ADDRMAP5.d32;
	reg->DDRC_ADDRMAP6 = ddrc->ADDRMAP6.d32;

	reg->DDRC_RFSHCTL3 = ddrc->RFSHCTL3.d32;
	reg->DDRC_RFSHTMG = ddrc->RFSHTMG.d32;
	reg->DDRC_ODTCFG = ddrc->ODTCFG.d32;

	reg->DDRC_DFITMG0 = ddrc->DFITMG0.d32;
	reg->DDRC_DFITMG1 = ddrc->DFITMG1.d32;
	reg->DDRC_DFIUPD0 = ddrc->DFIUPD0.d32;

	reg->DDRP_REG_CHANNEL_EN = ddrp->reg_channel_en;
	reg->DDRP_MEM_SELECT_T = ddrp->mem_select_t;
	reg->DDRP_CWL_FRE_OP0 = ddrp->CWL_FRE_OP0;
	reg->DDRP_CL_FRE_OP0 = ddrp->CL_FRE_OP0;
	reg->DDRP_AL_FRE_OP0 = ddrp->AL_FRE_OP0;
	reg->DDRP_REG_PHY_TRFC = ddrp->reg_phy_trfc;
	reg->DDRP_REG_PHY_TREFI = ddrp->reg_phy_trefi;

	reg->DDRP_REG_PLLPOSTDIVEN = ddrp->reg_pllpostdiven;
	reg->DDRP_REG_PLLPOSTDIV = ddrp->reg_pllpostdiv;

	memcpy(reg->h.name, p->name, sizeof(reg->h.name));
	ddrp_wrap(reg, p);
}

int create_one_ddr_params(struct ddr_chip_info *p, struct ddr_reg_value *reg)
{
	struct ddrc_reg ddrc;
	struct ddrp_reg ddrp;

	struct ddr_creator_ops *ddr_creator = NULL;
	int creator_found = 0;
	int i;

	/* 1. search ddr creator by type. */
	for (i = 0; i < ops_count; i++)
	{
		ddr_creator = p_ddr_creator[i];

		if (ddr_creator->type == p->type)
		{
			creator_found = 1;
			break;
		}
	}
	if (!creator_found)
	{
		printf("cannot find ddr_creator for ddr type: %d, please_check!\n", p->type);
		return -1;
	}

	memset(&ddrc, 0, sizeof(struct ddrc_reg));
	memset(&ddrp, 0, sizeof(struct ddrp_reg));

	ddr_creator->ddrc_params_creator(&ddrc, p);
	ddr_creator->ddrp_params_creator(&ddrp, p);

	fill_reg_value(reg, &ddrc, &ddrp, p);

	return 0;
}


int main(int argc, char *argv[])
{
	int ddr_nums = 1;
	int i;

	struct ddr_reg_value *reg_values;

	ddr_nums = init_supported_ddr();

	reg_values = malloc(ddr_nums * sizeof(struct ddr_reg_value));

#ifdef CONFIG_DDR_TYPE_DDR3
	ddr3_creator_init();
#endif

#ifdef CONFIG_DDR_TYPE_DDR2
	ddr2_creator_init();
#endif

#ifdef CONFIG_DDR_TYPE_LPDDR3
	lpddr3_creator_init();
#endif

	create_supported_ddr_params(reg_values);

	printf("#ifndef __DDR_REG_VALUES_H__\n");
	printf("#define __DDR_REG_VALUES_H__\n");
	printf("#include <asm/ddr_innophy.h>\n\n");

#ifdef DDR_PARAMS_STRUCT
	printf("struct ddr_reg_value supported_ddr_reg_values[] = {\n");
#endif

	for (i = 0; i < ddr_nums; i++)
	{
		dump_generated_reg_define(&reg_values[i]);
		printf("\n\n");
	}

#ifdef DDR_PARAMS_STRUCT
	printf("};\n");
#endif

	printf("#endif\n");

	return 0;
}
