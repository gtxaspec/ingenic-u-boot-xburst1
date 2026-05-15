#include <stdio.h>
#include <autoconf.h>
#include <asm/arch/cpm.h>
#include <asm/arch/clk.h>

struct cgu {
	unsigned en: 8;
	unsigned off: 8;
	unsigned sel_bit: 8;
	unsigned sel_src: 8;
	unsigned char sel[4];
	unsigned ce: 8;
	unsigned busy: 8;
	unsigned stop: 8;
	unsigned id: 8;
};
struct cgu_clk_src {
	unsigned int cgu_clk;
	unsigned int src;
};

#if 0
struct cgu cgu_clk_sel[CGU_CNT] = {
	[DDR] = {1, CPM_DDRCDR, 30, CFG_DDR_SEL_PLL, {0, APLL, MPLL, -1}, 29, 28, 27},
	[HELIX] = {1, CPM_HELIXCDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[MACPHY] = {1, CPM_MACCDR, 30, CFG_MACPHY_SEL_PLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[LCD] = {1, CPM_LPCDR, 30, VPLL, {APLL, MPLL, VPLL}, 28, 27, 26},
	[MSC0] = {1, CPM_MSC0CDR, 30, CFG_CPU_SEL_PLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[MSC1] = {0, CPM_MSC1CDR, 0, 0, {-1, -1, -1, -1}, 29, 28, 27},
	[I2S] = {1, CPM_I2STCDR, 30, VPLL, {APLL, MPLL, VPLL}, 29, 28, 27}, //需要再配置; i2s 使用VPLL
	[SFC0] = {1, CPM_SFC0CDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[SFC1] = {1, CPM_SFC1CDR, 30, VPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[SSI] = {1, CPM_SSICDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[PWM] = {1, CPM_PWMCDR, 30, APLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[CIM] = {1, CPM_CIMCDR, 30, VPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
	[ISP] = {1, CPM_ISPMCDR, 30, MPLL, {APLL, MPLL, VPLL}, 29, 28, 27},
};
#endif

#ifdef CONFIG_PRJ008
struct cgu cgu_clk_sel[CGU_CNT] = {
	//          en cdr_off     cs_bit sel_src               sel[2]                   ce busy stop */
	[DDR]    = {1, CPM_DDRCDR,   30, CFG_DDR_SEL_PLL,       {0,    APLL, MPLL,  -1}, 29, 28, 27},
	[HELIX]  = {1, CPM_HELIXCDR, 30, MPLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[MACPHY] = {1, CPM_MACCDR,   30, CFG_MACPHY_SEL_PLL,    {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[LCD]    = {1, CPM_LPCDR,    30, APLL,                  {APLL, MPLL, -1,    -1}, 28, 27, 26},
	[MSC]    = {1, CPM_MSC0CDR,  30, CFG_CPU_SEL_PLL,       {APLL, MPLL, -1, EXCLK}, 29, 28, 27},
	[MSC1]   = {0, CPM_MSC1CDR,  30, CFG_CPU_SEL_PLL,       {APLL, MPLL, -1, EXCLK}, 29, 28, 27},
	[I2S]    = {1, CPM_I2STCDR,  30, APLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[SFC0]   = {1, CPM_SFC0CDR,  30, MPLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[SFC1]   = {1, CPM_SFC1CDR,  30, APLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[SSI]    = {1, CPM_SSICDR,   30, MPLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[PWM]    = {1, CPM_PWMCDR,   30, APLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[CIM]    = {1, CPM_CIMCDR,   30, APLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
	[ISP]    = {1, CPM_ISPMCDR,  30, MPLL,                  {APLL, MPLL, -1,    -1}, 29, 28, 27},
};
#else
struct cgu cgu_clk_sel[CGU_CNT] = {
	//          en cdr_off     cs_bit sel_src               sel[2]               ce busy stop */
	[DDR]    = {1, CPM_DDRCDR,   30, CFG_DDR_SEL_PLL,       {0, APLL, MPLL, -1}, 29, 28, 27},
	[HELIX]  = {1, CPM_HELIXCDR, 30, MPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[MACPHY] = {1, CPM_MACCDR,   30, CFG_MACPHY_SEL_PLL,    {APLL, MPLL, VPLL},  29, 28, 27},
	[LCD]    = {1, CPM_LPCDR,    30, VPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[MSC]    = {1, CPM_MSC0CDR,  30, CFG_CPU_SEL_PLL,       {APLL, MPLL, VPLL},  29, 28, 27},
	[MSC1]   = {1, CPM_MSC1CDR,  30, MPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[I2S]    = {1, CPM_I2STCDR,  30, VPLL,                  {APLL, MPLL, VPLL},  29, 28, 27}, //需要再配置; i2s 使用VPLL
	[SFC0]   = {1, CPM_SFC0CDR,  30, MPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[SFC1]   = {1, CPM_SFC1CDR,  30, VPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[SSI]    = {1, CPM_SSICDR,   30, MPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[PWM]    = {1, CPM_PWMCDR,   30, APLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[CIM]    = {1, CPM_CIMCDR,   30, VPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
	[ISP]    = {1, CPM_ISPMCDR,  30, MPLL,                  {APLL, MPLL, VPLL},  29, 28, 27},
};
#endif


static void gen_cgu_params(struct clk_cgu_setting *cgusetting)
{
	/*stop clk and set div max*/
	int i, id;
	struct cgu *cgu = NULL;
	struct cgu_clk_src cgu_clk_src[] = CGU_CLK_SRC;
	unsigned regval = 0, reg = 0;
	for (i = 0; cgu_clk_src[i].cgu_clk != SRC_EOF; i++) {
		id = cgu_clk_src[i].cgu_clk;
		cgu_clk_sel[id].sel_src = cgu_clk_src[i].src;
	}
	for (id = 0; id < CGU_CNT; id++) {
		cgu = &(cgu_clk_sel[id]);
		reg = 0xb0000000 + cgu->off;

		regval = 0;
		cgusetting[id].sel_val = 0;
		for (i = 0; i < 4; i++) {
			if (cgu->sel_src == cgu->sel[i]) {
				cgusetting[id].sel_val = i << cgu->sel_bit;
			}
		}
		if(cgu->busy) {
			/*set div max*/
			regval |= (0xf | (1 << cgu->ce) | (1 << cgu->stop));
		} else {
			/*clear ce*/
			regval &= ~(1 << cgu->ce);
		}
		cgusetting[id].addr = reg;
		cgusetting[id].val = regval;
		cgusetting[id].sel_src = cgu_clk_sel[id].sel_src;
		cgusetting[id].ce = cgu->ce;
		cgusetting[id].busy = cgu->busy;
		cgusetting[id].stop = cgu->stop;
	}
}

static void file_head_print(void)
{
	printf("/*\n");
	printf(" * DO NOT MODIFY.\n");
	printf(" *\n");
	printf(" * This file was generated by clk_params_creator\n");
	printf(" *\n");
	printf(" */\n");
	printf("\n");
	printf("#ifndef __CLK_REG_VALUES_H__\n");
	printf("#define __CLK_REG_VALUES_H__\n\n");
}

static void file_end_print(void)
{
	printf("\n#endif /* __CLK_REG_VALUES_H__ */\n");
}

int main(int argc, char *argv[])
{
	struct clk_cgu_setting cgusetting[CGU_CNT];
	int i;
	gen_cgu_params(cgusetting);
	file_head_print();
	printf("#define CGU_REG_VALUE  {\\\n");
	for (i = 0; i < CGU_CNT; i++) {
		printf("\t{0x%08x, 0x%08x, %d, %d, %d, %d, 0x%08x}, \\\n",
		       cgusetting[i].addr, cgusetting[i].val,		\
		       cgusetting[i].ce, cgusetting[i].busy, cgusetting[i].stop, \
		       cgusetting[i].sel_src, cgusetting[i].sel_val);
	}
	printf("\t}\n");
	file_end_print();
	return 0;
}
