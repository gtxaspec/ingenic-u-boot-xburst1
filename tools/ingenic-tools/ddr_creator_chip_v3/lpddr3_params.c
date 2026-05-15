#include "ddr_params_creator.h"

static void fill_mr_params_lpddr3(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddr_mr1_t mr1 = {0};
	ddr_mr2_t mr2 = {0};
	ddr_mr3_t mr3 = {0};
	ddr_mr10_t mr10 = {0};
	ddr_mr11_t mr11 = {0};
	ddr_mr63_t mr63 = {0};
	int tmp;

/* set MR# MA[7:0] */
	mr1.lpddr3.MA_a8_a15 = 1;
	mr2.lpddr3.MA_a8_a15 = 2;
	mr3.lpddr3.MA_a8_a15 = 3;
	mr10.lpddr3.MA_a8_a15 = 10;
	mr11.lpddr3.MA_a8_a15 = 11;
	mr63.lpddr3.MA_a8_a15 = 63;

/* set MR1 */
	mr1.lpddr3.BL_a0_a2 = 0x3;

	tmp = ps2cycle_ceil(p->DDR_tWR, 0);
	BETWEEN(tmp, 3, 16);
	switch (tmp)
	{
		case 3 ... 4:
			tmp = 3;
			break;
		case 5 ... 7:
			tmp = 6;
			break;
		case 12 ... 13:
			tmp = 12;
			break;
		case 14 ... 15:
			tmp = 14;
			break;
	}
	if(tmp < 10)
	{
		mr1.lpddr3.nWR_a5_a7 = tmp - 2;
		mr2.lpddr3.nWRE_a4 = 0;
	}
	else
	{
		mr1.lpddr3.nWR_a5_a7 = tmp - 10;
		mr2.lpddr3.nWRE_a4 = 1;
	}

/* set MR2 */
	tmp = p->DDR_RL;
	BETWEEN(tmp, 8, 16);
	mr2.lpddr3.RL_WL_a0_a3 = tmp - 2;
	mr2.lpddr3.WL_Select_a6 = 0;
	mr2.lpddr3.WR_Lev_a7 = 0;

/* set MR3 */
	mr3.lpddr3.DS_a0_a3 = 2;  //(default)
	/** winbond
	  * 0000b: Reserved
	  * 0001b: 34.3 ohm typical
	  * 0010b: 40 ohm typical (default)
	  * 0011b: 48 ohm typical
	  * 0100b: 60 ohm typical
	  * 0101b: Reserved
	  * 0110b: 80 ohm typical
	  * 1001b: 34.3 ohm typical
	  * 1010b: 40 ohm typical
	  * 1011b: 34.3 ohm typical
	  * All others: Reserved
	 */

/* set MR10 */
	mr10.lpddr3.Calibration_Code_a0_a7 = 0xab;

/* set MR11 */
	mr11.lpddr3.DQ_ODT_a0_a1 = 0x0;

/* set MR63 */
	mr63.lpddr3.X_a0_a7 = 0xfc;

/* set INIT3 */
	ddrc->INIT3.b32.mr  = (mr1.d32 & 0xff);
	ddrc->INIT3.b32.emr = (mr2.d32 & 0xff);
/* set INIT4 */
	ddrc->INIT4.b32.emr2 = (mr3.d32 & 0xff);
}

/* ddrc setting */
static void ddrc_set_MSTR(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->MSTR.b32.burst_rdwr = p->DDR_BL / 2;
	ddrc->MSTR.b32.lpddr3 = 1;
}

static void ddrc_set_SWCTLSTATIC(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->SWCTLSTAIC.b32.sw_staic_unlpck = 0x1;
}

static void ddrc_set_SWCTL(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->SWCTL.b32.sw_done = 0x0;
}

static void ddrc_set_INIT0(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->INIT0.b32.post_cke_x1024 = RoundUp(RoundUp(ps2cycle_ceil(200000000, 1), 1024), 2); //200us
	ddrc->INIT0.b32.pre_cke_x1024 = RoundUp(RoundUp(ps2cycle_ceil(100000, 1), 1024), 2); //tINIT1为100ns(最小值)
}

static void ddrc_set_INIT1(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{

}

static void ddrc_set_INIT5(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->INIT5.b32.dev_zqinit_x32 = RoundUp(RoundUp(ps2cycle_ceil(1000000, 1), 32), 2); //1us
	ddrc->INIT5.b32.max_auto_init_x1024 = RoundUp(RoundUp(ps2cycle_ceil(10000000, 1), 1024), 2); //200us
}

static void ddrc_set_timing0(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	tmp = ps2cycle_ceil(p->DDR_tRAS_min, 1);
	ddrc->TIMING0.b32.t_ras_min = RoundDown(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tRAS_max, 0);
	tmp = RoundDown(tmp, 1024) - 1;
	ddrc->TIMING0.b32.t_ras_max = RoundDown(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tFAW, 1);
	ddrc->TIMING0.b32.t_faw = RoundUp(tmp, 2);

	/* wr2pre = (tWL + (BL / 2) + tWR) / 2; */
	tmp0 = p->DDR_WL;
	tmp1 =  p->DDR_BL / 2;
	tmp2 = ps2cycle_ceil(p->DDR_tWR, 1);
	tmp = RoundDown(tmp0 + tmp1 + tmp2, 2);
	ddrc->TIMING0.b32.wr2pre = tmp;
}

static void ddrc_set_timing1(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	tmp = ps2cycle_ceil(p->DDR_tRC, 1);
	ddrc->TIMING1.b32.t_rc = RoundUp(tmp, 2);

	tmp1 = p->DDR_BL / 2;
	tmp2 = ps2cycle_ceil(p->DDR_tRTP, 1);
	tmp2 = (tmp2 > 4) ? tmp2 : 4;
	tmp = tmp1 + tmp2 - 4;
	ddrc->TIMING1.b32.rd2pre = RoundDown(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tXP, 1);
	ddrc->TIMING1.b32.t_xp = RoundUp(tmp, 2);
}

static void ddrc_set_timing2(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	/* write_latency = RoundUp(WL / 2) */
	tmp = p->DDR_WL;
	ddrc->TIMING2.b32.write_latency = RoundUp(tmp, 2);

	/* read_latency = RoundUp(RL / 2) */
	tmp = p->DDR_RL;
	ddrc->TIMING2.b32.read_latency = RoundUp(tmp, 2);

	/* rd2wr = (tRL + (BL / 2) + RU(tDQSCKmax) + 1 - tWL) / 2; */
	tmp0 = p->DDR_RL;
	tmp1 = p->DDR_BL / 2;
	tmp2 = p->DDR_WL;
	tmp = ps2cycle_ceil(p->DDR_tDQSCK_max, 1);
	tmp = RoundUp(tmp0 + tmp1 + tmp + 1 - tmp2, 2);
	ddrc->TIMING2.b32.rd2wr = tmp;

	/* wr2rd = (WL + (BL / 2) + tWTR + 1) / 2; */
	tmp0 = p->DDR_WL;
	tmp1 = p->DDR_BL / 2;
	tmp2 = ps2cycle_ceil(p->DDR_tWTR, 1);
	tmp  = RoundUp(tmp0 + tmp1 + tmp2 + 1, 2);
	ddrc->TIMING2.b32.wr2rd = tmp;
}

static void ddrc_set_timing3(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	tmp = ps2cycle_ceil(p->DDR_tMRW, 1);
	ddrc->TIMING3.b32.t_mrw = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tMRD, 1);
	ddrc->TIMING3.b32.t_mrd = RoundUp(tmp, 2);
}

static void ddrc_set_timing4(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	tmp = ps2cycle_ceil(p->DDR_tRCD, 1);
	tmp = (tmp > 2) ? tmp : 2;
	ddrc->TIMING4.b32.t_rcd = RoundUp(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tCCD, 1);
	ddrc->TIMING4.b32.t_ccd = RoundUp(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tRRD, 1);
	ddrc->TIMING4.b32.t_rrd = RoundUp(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tRP, 1);
	ddrc->TIMING4.b32.t_rp = RoundDown(tmp, 2) + 1;
}

static void ddrc_set_timing5(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1;

	ddrc->TIMING5.b32.t_cksrx = RoundUp(2, 2);

	ddrc->TIMING5.b32.t_cksre = RoundUp(2, 2);

	tmp0 = ps2cycle_ceil(p->DDR_tCKESR, 1);
	ddrc->TIMING5.b32.t_ckesr = RoundUp(tmp0, 2);

	tmp1 = ps2cycle_ceil(p->DDR_tCKE,1);
	tmp = (tmp0 > tmp1) ? tmp0 : tmp1;
	ddrc->TIMING5.b32.t_cke = RoundUp(tmp, 2);
}

static void ddrc_set_timing6(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	ddrc->TIMING6.b32.t_ckdpde = RoundUp(2, 2);
	ddrc->TIMING6.b32.t_ckdpdx = RoundUp(2, 2);

	tmp = ps2cycle_ceil(p->DDR_tXP, 1) + 2;
	ddrc->TIMING6.b32.t_ckcsx = RoundUp(tmp, 2);
}

static void ddrc_set_timing7(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	ddrc->TIMING7.b32.t_ckpde = RoundUp(2, 2);
	ddrc->TIMING7.b32.t_ckpdx = RoundUp(2, 2);
}

static void ddrc_set_timing14(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	tmp = ps2cycle_ceil(p->DDR_tXSR, 1);
	ddrc->TIMING14.b32.t_xsr = RoundUp(tmp, 2);
}


static void ddrc_set_odtcfg(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	ddrc->ODTCFG.b32.wr_odt_hold = 0x6;

	ddrc->ODTCFG.b32.wr_odt_delay = 0x0;

	ddrc->ODTCFG.b32.rd_odt_hold = 0x6;

	tmp = p->DDR_CL - p->DDR_CWL;
	ddrc->ODTCFG.b32.rd_odt_delay = tmp;
}

static void ddrc_params_creator_lpddr3(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc_set_MSTR(ddrc, p);
	ddrc_set_SWCTLSTATIC(ddrc, p);
	ddrc_set_SWCTL(ddrc, p);
	ddrc_set_INIT0(ddrc, p);
	ddrc_set_INIT1(ddrc, p);
/* set INIT3 & INIT4 */
	fill_mr_params_lpddr3(ddrc, p);
	ddrc_set_INIT5(ddrc, p);

/* set ADDRMAP */
	fill_ddr_addrmap(ddrc, p);

/* set RFSHTMG */
	fill_tref(ddrc, p);
	ddrc_set_timing0(ddrc, p);
	ddrc_set_timing1(ddrc, p);
	ddrc_set_timing2(ddrc, p);
	ddrc_set_timing3(ddrc, p);
	ddrc_set_timing4(ddrc, p);
	ddrc_set_timing5(ddrc, p);
	ddrc_set_timing6(ddrc, p);
	ddrc_set_timing7(ddrc, p);
	ddrc_set_timing14(ddrc, p);

/* set odtcfg */
	ddrc_set_odtcfg(ddrc, p);

/* set DFITMG */
	ddrc_set_d2d3_dfitmg0(ddrc, p);
	ddrc_set_d2d3_dfitmg1(ddrc, p);
	ddrc_set_d2d3_dfiupd0(ddrc, p);
}

static void ddrp_params_creator_lpddr3(struct ddrp_reg *p, struct ddr_chip_info *c)
{
	int i = 0, tmp = 0;
/* set reg 0x000 */
	tmp = 0;
	for(i = 0; i < c->DDR_BIT / 8; i++)
		tmp |= (1 << i);
	p->reg_channel_en = tmp;

	p->mem_select_t = 0x3; // lpddr3 PHY MODE

/* set reg 0x008 */
	p->AL_FRE_OP0 = 0;

/* set reg 0x00c */
	p->CL_FRE_OP0 = c->DDR_RL;

/* set reg 0x010 */
	p->CWL_FRE_OP0 = c->DDR_WL;

/* set reg 0x0b8 */
	tmp  = ps2cycle_ceil(c->DDR_tRFC,1);
	p->reg_phy_trfc = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(c->DDR_tREFI,1);
	p->reg_phy_trefi = RoundUp(tmp, 2);

	ddrp_set_pll(p, c);
}


/* lpddr3_params.c */
static struct ddr_creator_ops lpddr3_creator_ops = {
	.type = LPDDR3,
	.ddrc_params_creator = ddrc_params_creator_lpddr3,
	.ddrp_params_creator = ddrp_params_creator_lpddr3,
};

void lpddr3_creator_init(void)
{
	register_ddr_creator(&lpddr3_creator_ops);
}
