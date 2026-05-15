// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/cpm.h>
#include <asm/arch/clk.h>
#include <generated/pll_reg_values.h>
#include <asm/global_data.h>

DECLARE_GLOBAL_DATA_PTR;

#define CONFIG_PLL_STABILIZE   0x20

static void pll_set(unsigned int reg)
{
	unsigned int val;
	unsigned int timeout = 0;

	val = cpm_inl(reg);
	val &= ~(1 << 0);
	cpm_outl(val,reg);

	switch(reg) {
	case CPM_CPAPCR:
		val = (APLL_EN_VALUE << 0)  | (APLL_M_VALUE << 20) | (APLL_N_VALUE << 14) |
			(APLL_OD1_VALUE << 11) | (APLL_OD0_VALUE << 8);
		cpm_outl(val, CPM_CPAPCR);
		break;
	case CPM_CPMPCR:
		val = (MPLL_EN_VALUE << 0)  | (MPLL_M_VALUE << 20) | (MPLL_N_VALUE << 14) |
			(MPLL_OD1_VALUE << 11) | (MPLL_OD0_VALUE << 8);
		cpm_outl(val, CPM_CPMPCR);
		break;
	case CPM_CPVPCR:
		if (gd->arch.gi->soc.xburst.PRJ008)
			break;
		val = (VPLL_EN_VALUE << 0)  | (VPLL_M_VALUE << 20) | (VPLL_N_VALUE << 14) |
			(VPLL_OD1_VALUE << 11) | (VPLL_OD0_VALUE << 8);
		cpm_outl(val, CPM_CPVPCR);
		break;
	default:
		printf("pll reg[0x%x] 0x%x not recognise!\n", reg, val);
	}

	timeout = 0x10000;
	while((!(cpm_inl(reg) & (1 << 3))));
	if(timeout == 0) {
		printf("pll reg[0x%x] val[0x%x] setting timeout!\n",reg, val);
	}
}
static void pll_sets(void)
{
	if(APLL_EN_VALUE) {
		pll_set(CPM_CPAPCR);
#ifndef CONFIG_DDR_DRVODT_DEBUG
		printf("CPA_CPAPCR:0x%08x\n",cpm_inl(CPM_CPAPCR));
#endif
	}

	if(MPLL_EN_VALUE){
		pll_set(CPM_CPMPCR);
#ifndef CONFIG_DDR_DRVODT_DEBUG
		printf("CPM_CPMPCR:0x%08x\n",cpm_inl(CPM_CPMPCR));
#endif
	}

	if (gd->arch.gi->soc.xburst.PRJ008)
		return ;

	if(VPLL_EN_VALUE){
		pll_set(CPM_CPVPCR);
#ifndef CONFIG_DDR_DRVODT_DEBUG
		printf("CPM_CPVPCR:0x%08x\n",cpm_inl(CPM_CPVPCR));
#endif
	}
}
static void cpccr_default(void)
{
	cpm_outl(0x55700000,CPM_CPCCR);
	while((cpm_inl(CPM_CPCSR) & 0xf0000007) != 0xf0000000);
}
static void cpccr_sets(void)
{
	unsigned int val;
	val = 0x55700000 |
		(CDIV_REG_VALUE  <<  0) |
		(L2DIV_REG_VALUE <<  4) |
		(H0DIV_REG_VALUE <<  8) |
		(H2DIV_REG_VALUE << 12) |
		(PDIV_REG_VALUE <<  16);
	cpm_outl(val,CPM_CPCCR);
	while(cpm_inl(CPM_CPCSR) & 7);
	val = (SEL_SRC_REG_VALUE   << 30) |
		(SEL_CPLL_REG_VALUE  << 28) |
		(SEL_H0PLL_REG_VALUE << 26) |
		(SEL_H2PLL_REG_VALUE << 24) |
		(CDIV_REG_VALUE  <<  0) |
		(L2DIV_REG_VALUE <<  4) |
		(H0DIV_REG_VALUE <<  8) |
		(H2DIV_REG_VALUE << 12) |
		(PDIV_REG_VALUE <<  16);
	cpm_outl(val,CPM_CPCCR);
	while((cpm_inl(CPM_CPCSR) & 0xf0000000) != 0xf0000000);

#ifndef CONFIG_DDR_DRVODT_DEBUG
	printf("CPM_CPCCR:0x%08x\n",cpm_inl(CPM_CPCCR));
#endif
}


static void riscvhcdr_init(void)
{
	// while((cpm_inl(CPM_RISCVHCDR) & (0x1 << 28))); // 正常情况下这个时间点，riscv cpm busy应该为 0，所以不用等待 riscv busy
	if((cpm_inl(CPM_RISCVHCDR) & (0x1 << 28)))
		printf("riscv cpm busy!!!!\n"); //以防万一，加个打印；如果busy的话可能需要加上wait riscv busy动作
	cpm_outl(0x1 | (0x1 << 29), CPM_RISCVHCDR); // 配置riscv cpm默认2分频
}

/* 
CONFIG_SYS_APLL_FREQ=1200000000
CONFIG_SYS_MPLL_FREQ=900000000
CONFIG_SYS_VPLL_FREQ=1188000000
CONFIG_SYS_CPU_FREQ=1200000000
CONFIG_SYS_MEM_FREQ=450000000
*/

void pll_init(void)
{
	cpccr_default();
	riscvhcdr_init();
	pll_sets();
	cpccr_sets();
}
