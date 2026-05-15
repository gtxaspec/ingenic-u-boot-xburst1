#include "ddr_params_creator.h"


static void fill_mr_params_ddr2(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddr_mr0_t m0 = {0};
	ddr_mr1_t m1 = {0};
	ddr_mr2_t m2 = {0};
	ddr_mr3_t m3 = {0};
	int tmp;

/* set MR0 */
	if(p->DDR_BL == 4)
		m0.ddr2.BL_a0_a2 = 2;
	else if(p->DDR_BL == 8)
		m0.ddr2.BL_a0_a2 = 3;
	else{
		out_error("DDR_BL(%d) error,only support 4 or 8\n",p->DDR_BL);
		assert(1);
	}
	// m0.ddr2.BL_a0_a1 = (8 - p->DDR_BL) / 2;

	m0.ddr2.BT_a3 = 0;

	BETWEEN(p->DDR_CL, 3, 9);
	switch (p->DDR_CL)
	{
		case 3 ... 7:
			m0.ddr2.CL_a4_a6 = p->DDR_CL;
			break;
		case 8 ... 9:
			m0.ddr2.CL_a4_a6 = 9 - p->DDR_CL;
			break;
		default:
			out_error("DDR_CL(%d) is error, valid value is between from 3 to 9.\n",
					p->DDR_CL);
	}

	m0.ddr2.TM_a7 = 0; // 0:Normal 1:Test

	m0.ddr2.DLL_reset_a8 = 1; //dll reset

	tmp = ps2cycle_ceil(p->DDR_tWR, 1);
	BETWEEN(tmp, 2, 14);
	switch(tmp)
	{
	case 2 ... 8:
		m0.ddr2.WR_a9_a11 = tmp - 1;
		break;
	case 9 ... 14: //tWR = 15ns, ≥666M时， WR > 9tCK
		m0.ddr2.WR_a9_a11 = 0;
		break;
	default:
		out_error("tWR(%d) is error, valid value is between from 2 to 12, (WR = %d cycles).\n",
		       p->DDR_tWR, tmp);
		assert(1);
	}


	// #ifdef CONFIG_DDR_DLL_OFF
	// 	m0.ddr2.PPD_a12 = 0;
	// #else
	// 	m0.ddr2.PPD_a12 = 1;
	// #endif
	// m0.ddr2.PD_a12 = 1; // 0：Fast Exit  1:Slow Exit

/* set MR1 */
	// #ifdef CONFIG_DDR_DLL_OFF
	// 	m1.ddr2.DLL_enable_a0 = 1; /* 1: DLL disable. */
	// #else
	// 	m1.ddr2.DLL_enable_a0 = 0; /* 1: DLL enable. */
	// #endif
	m1.ddr2.DLL_enable_a0 = 0; /* 0: DLL enable. 1: DLL disable. */

	m1.ddr2.AL_a3_a5 = p->DDR_AL;
	m1.ddr2.OCD_program_a7_a9 = 0;
	m1.ddr2.DQS_Enable_a10 = 0;
	m1.ddr2.Qoff_a12 = 0;

	/*************************
     * DIC_a1                *
	 *   0    Normal(100%).  *
	 *   1    Weak(60%).     *
	 *************************/
	m1.ddr2.DIC_a1 = 0;
	BETWEEN(m1.ddr2.DIC_a1,0,1);

	/**********************
	 * 00 - ODT disable.  *
	 * 01 - 75Ω.          *
	 * 10 - 150Ω.         *
	 * 11 - 50Ω.          *
	 **********************/
	m1.ddr2.RTT_Nom_a2 = 0;
	m1.ddr2.RTT_Nom_a6 = 0;
	BETWEEN(m1.ddr2.RTT_Nom_a2,0,1);
	BETWEEN(m1.ddr2.RTT_Nom_a6,0,1);


/* set MR2 */
	m2.d32 = 0;

/* set MR3 */
	m3.d32 = 0;

/* set INIT3 */
	ddrc->INIT3.b32.mr  = (m0.d32 & 0x1fff);
	ddrc->INIT3.b32.emr = (m1.d32 & 0x1fff);
/* set INIT4 */
	ddrc->INIT4.b32.emr2 = (m2.d32 & 0x1fff);
	ddrc->INIT4.b32.emr3 = (m3.d32 & 0x1fff);

}

static void ddrc_set_MSTR(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	// ddrc->MSTR.b32.ddr2 = 0x1;
	ddrc->MSTR.b32.burst_rdwr = p->DDR_BL / 2;
}

static void ddrc_set_SWCTLSTAIC(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->SWCTLSTAIC.b32.sw_staic_unlpck = 0x1;
}

static void ddrc_set_SWCTL(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->SWCTL.b32.sw_done = 0x0;
}

static void ddrc_set_INIT0(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->INIT0.b32.pre_cke_x1024 = RoundUp(RoundUp(ps2cycle_ceil(200000000, 1), 1024), 2); // 200us
	ddrc->INIT0.b32.post_cke_x1024 = RoundUp(RoundUp(ps2cycle_ceil(400000, 1), 1024), 2); // 400ns
}

static void ddrc_set_INIT1(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	// ddrc->INIT1.b32.pre_ocd_x32 = 0xf;
	ddrc->INIT1.b32.pre_ocd_x32 = RoundUp(200, 32);
}

static void ddrc_set_timing0(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	tmp = ps2cycle_ceil(p->DDR_tRAS_min, 1);
	ddrc->TIMING0.b32.t_ras_min = RoundDown(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tRAS_max, 0);
	tmp = RoundDown(tmp, 1024) - 1;
	ddrc->TIMING0.b32.t_ras_max = RoundDown(tmp, 2);

	if(p->DDR_BANK == 4)
		ddrc->TIMING0.b32.t_faw = 1; // In a 4-bank design, set this register to 0x1 independent of the 1:1/1:2 frequency mode.
	else
		ddrc->TIMING0.b32.t_faw = RoundUp(ps2cycle_ceil(p->DDR_tFAW, 1), 2);

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

	tmp0 = p->DDR_AL;
	tmp1 = p->DDR_BL / 2;
	tmp2 = ps2cycle_ceil(p->DDR_tRTP, 1);
	tmp2 = (tmp2 > 2) ? tmp2 : 2;
	tmp = tmp0 + tmp1 + tmp2 - 2;
	ddrc->TIMING1.b32.rd2pre = RoundDown(tmp, 2);

	tmp = ps2cycle_ceil(p->DDR_tXP, 1);
	ddrc->TIMING1.b32.t_xp = RoundUp(tmp, 2);
}

static void ddrc_set_timing2(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	/* wr2rd = RoundUp((CWL + (BL / 2) + tWTR) / 2); */
	tmp0 = p->DDR_WL;
	tmp1 = p->DDR_BL / 2;
	tmp2 = ps2cycle_ceil(p->DDR_tWTR, 1);
	tmp  = RoundUp(tmp0 + tmp1 + tmp2, 2);
	ddrc->TIMING2.b32.wr2rd = tmp;

	/* rd2wr = RoundUp((tRL + (BL / 2) + 2 - tWL)) */
	tmp0 = p->DDR_RL;
	tmp1 = p->DDR_BL / 2;
	tmp2 = p->DDR_WL;
	tmp = RoundUp(tmp0 + tmp1 + 2 - tmp2, 2);
	ddrc->TIMING2.b32.rd2wr = tmp;

	// [24:16] read_latency = RL / 2 + 1
	// tmp = p->DDR_RL;
	// ddrc->TIMING2.b32.read_latency = RoundUp(tmp, 2); // This register field is not required for DDR2 and DDR3

	// [29:24] write_latency = WL / 2 + 1
	// tmp = p->DDR_WL;
	// ddrc->TIMING2.b32.write_latency = RoundUp(tmp, 2); // This register field is not required for DDR2 and DDR3
}

static void ddrc_set_timing3(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	tmp  = ps2cycle_ceil(p->DDR_tMRD,1);
	ddrc->TIMING3.b32.t_mrd = RoundUp(tmp, 2);
}

static void ddrc_set_timing4(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	tmp = ps2cycle_ceil(p->DDR_tRCD, 1) - p->DDR_AL;
	tmp = (tmp > 2) ? tmp : 2;
	ddrc->TIMING4.b32.t_rcd = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tCCD, 1);
	ddrc->TIMING4.b32.t_ccd = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tRRD, 1);
	ddrc->TIMING4.b32.t_rrd = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tRP, 1);
	ddrc->TIMING4.b32.t_rp = RoundDown(tmp, 2) + 1;
}

static void ddrc_set_timing5(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	ddrc->TIMING5.b32.t_cksrx = 1;
	ddrc->TIMING5.b32.t_cksre = 1;

	tmp  = ps2cycle_ceil(p->DDR_tCKE, 1);
	ddrc->TIMING5.b32.t_ckesr = RoundUp(tmp, 2);
	ddrc->TIMING5.b32.t_cke = RoundUp(tmp, 2);
}

static void ddrc_set_timing7(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->TIMING7.b32.t_ckpde = ddrc->TIMING5.b32.t_cksre;
	ddrc->TIMING7.b32.t_ckpdx = ddrc->TIMING5.b32.t_cksrx;
}

static void ddrc_set_timing8(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp;

	tmp = ps2cycle_ceil(p->DDR_tXSRD, 1);
	ddrc->TIMING8.b32.t_xs_dll_x32 = RoundUp(RoundUp(tmp, 32), 2);

	tmp  = ps2cycle_ceil(p->DDR_tXSNR, 1);
	ddrc->TIMING8.b32.t_xs_x32 = RoundUp(RoundUp(tmp, 32), 2);
}

static void ddrc_set_odtcfg(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	switch(p->DDR_BL)
	{
		case 0x8:
			if(p->freq >= 200000000 && p->freq <=334000000)
				tmp = 0x5;
			else if(p->freq > 334000000 && p->freq <=400000000)
				tmp = 0x6;
			else if(p->freq > 400000000)
				tmp = 0x7;
			break;
		case 0x4:
			if(p->freq >= 200000000 && p->freq <=334000000)
				tmp = 0x3;
			else if(p->freq > 334000000 && p->freq <=400000000)
				tmp = 0x4;
			else if(p->freq > 400000000)
				tmp = 0x5;
			break;
		default:
			out_error("DDR_BL(%d) is error, The valid values are 4 or 8.\n",
					p->DDR_BL);
	}
	tmp = (tmp > 2) ? tmp : 2;
	ddrc->ODTCFG.b32.wr_odt_hold = tmp;

	tmp = p->DDR_WL + p->DDR_AL;
	if (p->freq >= 200000000 && p->freq <= 334000000)
		tmp = tmp - 3;
	else if (p->freq > 334000000 && p->freq <= 400000000)
		tmp = tmp - 4;
	else if (p->freq > 400000000)
		tmp = tmp - 5;
	BETWEEN(tmp, 0, p->DDR_WL - 3);
	ddrc->ODTCFG.b32.wr_odt_delay = tmp;

	switch(p->DDR_BL)
	{
		case 0x8:
			if (p->freq >= 533000000)
				tmp = 0x7;
			else
				tmp = 0x6;
			break;
		case 0x4:
			if (p->freq >= 533000000)
				tmp = 0x5;
			else
				tmp = 0x4;
			break;
		default:
			out_error("DDR_BL(%d) is error, The valid values are 4 or 8.\n",
					p->DDR_BL);
	}
	tmp = (tmp > 2) ? tmp : 2;
	ddrc->ODTCFG.b32.rd_odt_hold = tmp;

	tmp = p->DDR_WL + p->DDR_AL;
	if (p->freq >= 533000000)
		tmp = tmp - 5;
	else
		tmp = tmp - 4;
	BETWEEN(tmp,  p->DDR_WL - 5, p->DDR_WL - 4);
	ddrc->ODTCFG.b32.rd_odt_delay = tmp;
}

/* ddr2 timing */
static void ddrc_params_creator_ddr2(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc_set_MSTR(ddrc, p);
	ddrc_set_SWCTLSTAIC(ddrc, p);
	ddrc_set_SWCTL(ddrc, p);
	ddrc_set_INIT0(ddrc, p);
	ddrc_set_INIT1(ddrc, p);

/* set INIT3 & INIT4 */
	fill_mr_params_ddr2(ddrc, p);

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
	ddrc_set_timing7(ddrc, p);
	ddrc_set_timing8(ddrc, p);

/* set odtcfg */
	ddrc_set_odtcfg(ddrc, p);

/* set DFITMG */
	ddrc_set_d2d3_dfitmg0(ddrc, p);
	ddrc_set_d2d3_dfitmg1(ddrc, p);
	ddrc_set_d2d3_dfiupd0(ddrc, p);
}


static void ddrp_params_creator_ddr2(struct ddrp_reg *p, struct ddr_chip_info *c)
{
	int i = 0, tmp = 0;
/* set reg 0x000 */
	tmp = 0;
	for(i = 0; i < c->DDR_BIT / 8; i++)
		tmp |= (1 << i);
	p->reg_channel_en = tmp;

	p->mem_select_t = 0x0; // DDR2 PHY MODE

/* set reg 0x008 */
	p->AL_FRE_OP0 = c->DDR_AL;

/* set reg 0x00c */
	p->CL_FRE_OP0 = c->DDR_CL;

/* set reg 0x010 */
	p->CWL_FRE_OP0 = c->DDR_WL;

/* set reg 0x0b8 */
	tmp  = ps2cycle_ceil(c->DDR_tRFC,1);
	p->reg_phy_trfc = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(c->DDR_tREFI,1);
	p->reg_phy_trefi = RoundUp(tmp, 2);

	ddrp_set_pll(p, c);
}


/* ddr2_params.c */
static struct ddr_creator_ops ddr2_creator_ops = {
	.type = DDR2,
	.ddrc_params_creator = ddrc_params_creator_ddr2,
	.ddrp_params_creator = ddrp_params_creator_ddr2,
};

void ddr2_creator_init(void)
{
	register_ddr_creator(&ddr2_creator_ops);
}
