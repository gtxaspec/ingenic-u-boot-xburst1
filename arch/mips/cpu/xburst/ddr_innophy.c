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


static void sys_ddrc_dump()
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

static void ddrc_init(unsigned int kgd_rtt_dic)
{
	unsigned int data = 0;

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
		ddrp_map_set();

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

static void ddrp_set_drv_odt(unsigned int *p)
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

static void ddrc_set_port_priority()
{
	unsigned int port = 0, val = 0;

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
	writel(0xee, QOS_BASE + 0x30); //ISP 14
	writel(0xffdd, QOS_BASE + 0x70); //CPU 15, VPU 13
}

static void ddrc_dfi_init()
{
	unsigned int ddrc_reset = 0;
	unsigned int p[INDEX_EN] = {0}; //ddr parameter

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
	ddrp_trining(p);

	SET_INNOPHY_REG2((IO_BASE + 0x20), 19, 19, 1); // ISP
	SET_INNOPHY_REG2((IO_BASE + 0x20), 28, 28, 1); // VPU
	SET_INNOPHY_REG2((IO_BASE + 0x28), 03, 03, 1); // IVDC
	SET_INNOPHY_REG2((IO_BASE + 0x28), 11, 11, 1); // TNPU

	ddrc_set_port_priority();
	ddrc_writel(SWCTLSTATIC, 0x0);
	ddrc_writel(SWCTL, 0x1);
	/* DDR controller initialization complete */
}

/* DDR sdram init */
void sdram_init(void)
{
	unsigned int rate = gd->arch.gi->ddrfreq;
	int type;

	printf("sdram init start\n");
	get_ddr_type();
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
	return DDR_SIZE;
}
