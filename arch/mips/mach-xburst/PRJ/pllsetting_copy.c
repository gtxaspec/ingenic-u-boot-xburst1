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

static void pll_set(u32 reg, u32 reg_val)
{
	u32 val = 0;
	u32 timeout = 0;

	val = cpm_inl(reg);
	val &= ~(1 << 0);
	cpm_outl(val, reg);

	cpm_outl(val, reg_val);

	timeout = 0x10000;
	while ((!(cpm_inl(reg) & (1 << 3))))
		;
	if (timeout == 0)
	{
		printf("pll reg[0x%x] val[0x%x] setting timeout!\n", reg, val);
	}
}
static void pll_sets(void)
{
	if (APLL_EN_VALUE)
	{
		pll_set(CPM_CPAPCR);
		printf("CPA_CPAPCR:0x%08x\n", cpm_inl(CPM_CPAPCR));
	}

	if (MPLL_EN_VALUE)
	{
		pll_set(CPM_CPMPCR);
		printf("CPM_CPMPCR:0x%08x\n", cpm_inl(CPM_CPMPCR));
	}

	if (VPLL_EN_VALUE)
	{
		if (gd->arch.gi->soc.xburst.PRJ008)
			return;
		pll_set(CPM_CPVPCR);
		printf("CPM_CPVPCR:0x%08x\n", cpm_inl(CPM_CPVPCR));
	}
}

static void cpccr_default(void)
{
	cpm_outl(0x55700000,CPM_CPCCR);
	while((cpm_inl(CPM_CPCSR) & 0xf0000007) != 0xf0000000);
}

static void cpccr_sets(void)
{
	u32 val;
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


#if 0

static u32 get_pllreg_value(u32 pll)
{
#define PLL_MIN_M 16
#define PLL_MAX_M 4095
#define PLL_MIN_N 1
#define PLL_MAX_N 63
#define PLL_MIN_OD1 2
#define PLL_MAX_OD1 7
#define PLL_MIN_FVCO 1250
#define PLL_MAX_FVCO 5000
#define PLL_DIVISOR 24
	u32 count = 0;
	const u32 od0 = 1;
	u32 m, n, od1;
	u32 numerator;
	u32 fvco, actual_pll;

	pll /= 1000000; // 单位转换成MHz

	for (od1 = PLL_MIN_OD1; od1 <= PLL_MAX_OD1; od1++)
	{
		for (n = PLL_MIN_N; n <= PLL_MAX_N; n++)
		{
			numerator = pll * n * od1 * od0;
			if (numerator % PLL_DIVISOR != 0)
			{
				continue;
			}

			m = (u32)(numerator / PLL_DIVISOR);

			if (m < PLL_MIN_M || m > PLL_MAX_M)
			{
				continue;
			}

			fvco = (24 * m) / n;
			if ((24 * m) % n != 0 || fvco < PLL_MIN_FVCO || fvco > PLL_MAX_FVCO)
			{
				continue;
			}

			actual_pll = fvco / (od1 * od0);
			if (actual_pll != pll)
			{
				continue;
			}

			count++;

			printf("count %d  | m %d  | n %d  | od1 %d  | od0 %d  | fvco %d  | actual_pll %d\n",
				  count, m, n, od1, od0, fvco, actual_pll);
			return ((m << 20) | (n << 14) | (od1 << 11) | (od0 << 8));
		}
	}

	if (count == 0)
	{
		printf("Pll(m n od1 od0) No valid combinations found that meet all constraints!\n");
		return -EINVAL;
	}
}

static void pll_set(u32 pll, u32 freq)
{
	struct pll_reg_config {
		u32 cpm_cppcr;
		u32 cpm_cppacr;
		cpm_cpxpcr_t cppcr;
	} pll_reg_config;

	switch (pll)
	{
		case APLL:
			pll_reg_config.cpm_cppcr = CPM_CPAPCR;
			pll_reg_config.cpm_cppacr = CPM_CPAPACR;
			pll_reg_config.cppcr.d32 = CONFIG_SYS_APLL_MNOD;
			break;
		case MPLL:
			pll_reg_config.cpm_cppcr = CPM_CPMPCR;
			pll_reg_config.cpm_cppacr = CPM_CPMPACR;
			pll_reg_config.cppcr.d32 = CONFIG_SYS_MPLL_MNOD;
			break;
		case VPLL:
			pll_reg_config.cpm_cppcr = CPM_CPVPCR;
			pll_reg_config.cpm_cppacr = CPM_CPVPACR;
			pll_reg_config.cppcr.d32 = CONFIG_SYS_VPLL_MNOD;
			break;
		default:
			printf("error!!! pll %d not support\n", pll);
			return -EINVAL;
	}

#if defined(CONFIG_DDR_PARAMS_STRUCT)
		pll_reg_config.cppcr.d32 = get_pllreg_value(freq);
#endif

		pll_reg_config.cppcr.b.PLLEN = 1;
		cpm_outl(pll_reg_config.cppcr.d32, pll_reg_config.cpm_cppcr);
		while(!(cpm_inl(pll_reg_config.cpm_cppcr) & (0x1 << 3)));
		printf("CPM_%sCR 0x%x\n", pll_reg_config.cpm_cppcr == CPM_CPAPCR ? "AP" : (pll_reg_config.cpm_cppcr == CPM_CPMPCR ? "MP" : "VP"), cpm_inl(pll_reg_config.cpm_cppcr));

}

#endif

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

	// pll_set(APLL, CONFIG_SYS_APLL_FREQ);
	// pll_set(MPLL, CONFIG_SYS_MPLL_FREQ);
	// if(gd->arch.soc.xburst.PRJ007)
	// 	pll_set(VPLL, CONFIG_SYS_VPLL_FREQ);

	pll_sets();
	cpccr_sets();
}
