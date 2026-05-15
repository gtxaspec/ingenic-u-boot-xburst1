#include "ddr_params_creator.h"


static void fill_mr_params_ddr3(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddr_mr0_t m0 = {0};
	ddr_mr1_t m1 = {0};
	ddr_mr2_t m2 = {0};
	ddr_mr3_t m3 = {0};
	int tmp;

/* set MR0 */
	if(p->DDR_BL == 4)
		m0.ddr3.BL_a0_a1 = 2;
	else if(p->DDR_BL == 8)
		m0.ddr3.BL_a0_a1 = 0;
	else{
		out_error("DDR_BL(%d) error,only support 4 or 8\n",p->DDR_BL);
		assert(1);
	}
	// m0.ddr3.BL_a0_a1 = (8 - p->DDR_BL) / 2;

	BETWEEN(p->DDR_CL,5,16);
	switch (p->DDR_CL)
	{
		case 5 ... 11:
			m0.ddr3.CL_a2 = 0;
			m0.ddr3.CL_a4_a6 = p->DDR_CL - 4;
			break;
		case 12 ... 16:
			m0.ddr3.CL_a2 = 1;
			m0.ddr3.CL_a4_a6 = p->DDR_CL - 12;
			break;
		default:
			out_error("DDR_CL(%d) is error, valid value is between from 5 to 16.\n",
					p->DDR_CL);
	}

	m0.ddr3.DLL_reset_a8 = 1; //dll reset

	tmp = ps2cycle_ceil(p->DDR_tWR, 1);
	switch(tmp)
	{
	case 5 ... 8:
		m0.ddr3.WR_a9_a11 = tmp - 4;
		break;
	case 9 ... 14:
		m0.ddr3.WR_a9_a11 = (tmp + 1) / 2;
		break;
	case 15 ... 20:
		m0.ddr3.WR_a9_a11 = 0;
		break;
	default:
		out_error("tWR(%d) is error, valid value is between from 5 to 14 & 16, (WR = %d cycles).\n",
		       p->DDR_tWR, tmp);
		assert(1);
	}


	// #ifdef CONFIG_DDR_DLL_OFF
	// 	m0.ddr3.PPD_a12 = 0;
	// #else
	// 	m0.ddr3.PPD_a12 = 1;
	// #endif
	m0.ddr3.PPD_a12 = 1;

/* set MR1 */
	// #ifdef CONFIG_DDR_DLL_OFF
	// 	m1.ddr3.DLL_enable_a0 = 1; /* 1: DLL disable. */
	// #else
	// 	m1.ddr3.DLL_enable_a0 = 0; /* 1: DLL enable. */
	// #endif
	m1.ddr3.DLL_enable_a0 = 0;

	m1.ddr3.Level_a7 = 0;
	m1.ddr3.Qoff_a12 = 0;

	/*************************
     * DIC5   DIC1           *
	 * 0     0 - RZQ/6.      *
	 * 0     1 - RZQ/7.      *
	 * 1     0 - Reserved.   *  ntc:Reserved  winbond:RZQ/3.
	 * 1     1 - Reserved.   *  ntc:Reserved  winbond:RZQ/4.
	 *************************/
	m1.ddr3.DIC_a1 = 0;
	m1.ddr3.DIC_a5 = 0;
	BETWEEN(m1.ddr3.DIC_a1,0,1);
	BETWEEN(m1.ddr3.DIC_a5,0,1);

	/**********************
	 * 000 - ODT disable. *
	 * 001 - RZQ/4.       *
	 * 010 - RZQ/2.       *
	 * 011 - RZQ/6.       *
	 * 100 - RZQ/12.      *
	 * 101 - RZQ/8.       *
	 * 110 - Reserved.    *
	 * 111 - Reserved.    *
	 **********************/
	m1.ddr3.RTT_Nom_a2 = 0;
	m1.ddr3.RTT_Nom_a6 = 0;
	m1.ddr3.RTT_Nom_a9 = 0;
	BETWEEN(m1.ddr3.RTT_Nom_a2,0,1);
	BETWEEN(m1.ddr3.RTT_Nom_a6,0,1);
	BETWEEN(m1.ddr3.RTT_Nom_a9,0,1);

/* set MR2 */
	m2.ddr3.RTT_WR_a9_a10 = 0;
	BETWEEN(m2.ddr3.RTT_WR_a9_a10,0,3);

	tmp = p->DDR_WL;
	if(tmp < 5 || tmp > 10)
	{
		out_error("ddr frequancy too fast. %d\n",tmp);
		// out_error(". %d\n",__ps_per_tck);
		assert(1);
	}
	m2.ddr3.CWL_a3_a5 = tmp - 5;

/* set MR3 */
	m3.d32 = 0;

/* set INIT3 */
	ddrc->INIT3.b32.mr  = (m0.d32 & 0xffff);
	ddrc->INIT3.b32.emr = (m1.d32 & 0xffff);
/* set INIT4 */
	ddrc->INIT4.b32.emr2 = (m2.d32 & 0xffff);
	ddrc->INIT4.b32.emr3 = (m3.d32 & 0xffff);

}

/* ddrc setting */
static void ddrc_set_MSTR(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->MSTR.b32.burst_rdwr = p->DDR_BL / 2;
	ddrc->MSTR.b32.ddr3 = 1;
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
	ddrc->INIT0.b32.pre_cke_x1024 = RoundUp(RoundUp(ps2cycle_ceil(500000000, 1), 1024), 2); //500us
	ddrc->INIT0.b32.post_cke_x1024 = RoundUp(RoundUp(ps2cycle_ceil(p->DDR_tXPR, 1), 1024), 2); //120ns
}

static void ddrc_set_INIT1(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->INIT1.b32.dram_rstn_x1024 = RoundUp(RoundUp(ps2cycle_ceil(200000000, 1), 1024), 2);
}

static void ddrc_set_INIT5(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc->INIT5.b32.dev_zqinit_x32 = RoundUp(RoundUp(512, 32), 2); //512 cycles; Simulation testing requires +1
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

	tmp0 = ps2cycle_ceil(p->DDR_tRTP, 1);
	tmp0 = (tmp0 > 4) ? tmp0 : 4;

	tmp1 = ps2cycle_ceil(p->DDR_AL,1);
	tmp = tmp0 + tmp1;
	ddrc->TIMING1.b32.rd2pre = RoundDown(tmp, 2);

	if(ddrc->INIT3.b32.mr & (1 << 12))
		tmp = ps2cycle_ceil(p->DDR_tXP, 1);
	else
		tmp = ps2cycle_ceil(p->DDR_tXPDLL, 1);
	ddrc->TIMING1.b32.t_xp = RoundUp(tmp, 2); // For DDR3, this must be programmed to tXPDLL if slow powerdown exit is selected in MR0[12].
}

static void ddrc_set_timing2(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	/* wr2rd = (CWL + (BL / 2) + tWTR) / 2 + 1; */
	tmp0 = p->DDR_CWL;
	tmp1 = p->DDR_BL / 2;
	tmp2 = ps2cycle_ceil(p->DDR_tWTR, 1);
	tmp  = RoundUp(tmp0 + tmp1 + tmp2, 2);
	ddrc->TIMING2.b32.wr2rd = tmp;

	/* rd2wr = (tRL + (BL / 2) + 2 - tWL) / 2 + 1; */
	tmp0 = p->DDR_RL;
	tmp1 = p->DDR_BL / 2;
	tmp2 = p->DDR_WL;
	tmp = RoundUp(tmp0 + tmp1 + 2 - tmp2, 2);
	ddrc->TIMING2.b32.rd2wr = tmp;
}

static void ddrc_set_timing3(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

	tmp  = ps2cycle_ceil(p->DDR_tMOD, 1);
	ddrc->TIMING3.b32.t_mod = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tMRD, 1);
	ddrc->TIMING3.b32.t_mrd = RoundUp(tmp, 2);
}

static void ddrc_set_timing4(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	int tmp, tmp0, tmp1, tmp2;

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
	int tmp, tmp0, tmp1, tmp2;

	tmp  = ps2cycle_ceil(p->DDR_tCKSRX, 1);
	ddrc->TIMING5.b32.t_cksrx = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tCKSRE, 1);
	ddrc->TIMING5.b32.t_cksre = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tCKESR, 1);
	ddrc->TIMING5.b32.t_ckesr = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(p->DDR_tCKE,1);
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

	tmp = ps2cycle_ceil(p->DDR_tXSDLL,1);
	ddrc->TIMING8.b32.t_xs_dll_x32 = RoundUp(RoundUp(tmp, 2), 32);

	tmp  = ps2cycle_ceil(p->DDR_tXS,1);
	ddrc->TIMING8.b32.t_xs_x32 = RoundUp(RoundUp(tmp, 2), 32);
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

static void ddrc_params_creator_ddr3(struct ddrc_reg *ddrc, struct ddr_chip_info *p)
{
	ddrc_set_MSTR(ddrc, p);
	ddrc_set_SWCTLSTATIC(ddrc, p);
	ddrc_set_SWCTL(ddrc, p);
	ddrc_set_INIT0(ddrc, p);
	ddrc_set_INIT1(ddrc, p);
/* set INIT3 & INIT4 */
	fill_mr_params_ddr3(ddrc, p);
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
	ddrc_set_timing7(ddrc, p);
	ddrc_set_timing8(ddrc, p);

/* set odtcfg */
	ddrc_set_odtcfg(ddrc, p);

/* set DFITMG */
	ddrc_set_d2d3_dfitmg0(ddrc, p);
	ddrc_set_d2d3_dfitmg1(ddrc, p);
	ddrc_set_d2d3_dfiupd0(ddrc, p);
}

static void ddrp_params_creator_ddr3(struct ddrp_reg *p, struct ddr_chip_info *c)
{
	int i = 0, tmp = 0;
/* set reg 0x000 */
	tmp = 0;
	for(i = 0; i < c->DDR_BIT / 8; i++)
		tmp |= (1 << i);
	p->reg_channel_en = tmp;

	p->mem_select_t = 0x2; // DDR3 PHY MODE

/* set reg 0x008 */
	p->AL_FRE_OP0 = c->DDR_AL;

/* set reg 0x00c */
	p->CL_FRE_OP0 = c->DDR_CL;

/* set reg 0x010 */
	p->CWL_FRE_OP0 = c->DDR_CWL;

/* set reg 0x0b8 */
	tmp  = ps2cycle_ceil(c->DDR_tRFC,1);
	p->reg_phy_trfc = RoundUp(tmp, 2);

	tmp  = ps2cycle_ceil(c->DDR_tREFI,1);
	p->reg_phy_trefi = RoundUp(tmp, 2);

	ddrp_set_pll(p, c);
}


/* ddr3_params.c */
static struct ddr_creator_ops ddr3_creator_ops = {
	.type = DDR3,
	.ddrc_params_creator = ddrc_params_creator_ddr3,
	.ddrp_params_creator = ddrp_params_creator_ddr3,
};

void ddr3_creator_init(void)
{
	register_ddr_creator(&ddr3_creator_ops);
}
