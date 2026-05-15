#include "ddr_params_creator.h"

#ifdef DDR_PARAMS_STRUCT
#define dump_ddr(fmt, args...) printf("		." fmt, ##args)
#else
#define dump_ddr(fmt, args...) printf("#define " fmt, ##args)
#endif

#define dump_ddr_test(fmt, args...) printf("// " fmt, ##args)

static void dump_ddr_info(struct ddr_reg_value *reg)
{
#ifndef DDR_PARAMS_STRUCT
	printf("#define DDR_NAME                \"%s\"\n", reg->h.name);
	printf("#define DDR_FREQ                %10d //%dMHz  tCK %dps\n", reg->h.freq, reg->h.freq / 1000000, __ps_per_tck);
	printf("#define DDR_TYPE                0x%08x\n", reg->h.id);
	printf("#define DDR_SIZE                0x%08x //%dMB\n", reg->h.size, reg->h.size / 1024 / 1024);
#else
	printf("	{\n");
	printf("		.h = { \n");
	printf("			.name        =   \"%s\",\n", reg->h.name);
	printf("			.id          =   0x%08x,\n", reg->h.id);
	printf("			.freq        =   %d,  // %dMHz\n", reg->h.freq, reg->h.freq / 1000000);
	printf("		},\n");
#endif
}

static void dump_ddr_timing(struct ddr_reg_value *reg)
{
//	dump_ddr("DDRC_SWCTLSTATIC        0x%08x\n", reg->DDRC_SWCTLSTATIC);
//	dump_ddr("DDRC_SWCTL              0x%08x\n", reg->DDRC_SWCTL);
	dump_ddr("DDRC_MSTR               0x%08x\n", reg->DDRC_MSTR);
	dump_ddr("DDRC_INIT0              0x%08x\n", reg->DDRC_INIT0);
	dump_ddr("DDRC_INIT1              0x%08x\n", reg->DDRC_INIT1);
	dump_ddr("DDRC_INIT2              0x%08x\n", reg->DDRC_INIT2);
	dump_ddr("DDRC_INIT3              0x%08x //MR0 0x%08x MR1 0x%08x\n", reg->DDRC_INIT3, (reg->DDRC_INIT3 >> 16) & 0xffff, reg->DDRC_INIT3 & 0xffff);
	dump_ddr("DDRC_INIT4              0x%08x //MR2 0x%08x MR3 0x%08x\n", reg->DDRC_INIT4, (reg->DDRC_INIT4 >> 16) & 0xffff, reg->DDRC_INIT4 & 0xffff);
	dump_ddr("DDRC_TIMING0            0x%08x\n", reg->DDRC_TIMING0);
	dump_ddr("DDRC_TIMING1            0x%08x\n", reg->DDRC_TIMING1);
	dump_ddr("DDRC_TIMING2            0x%08x\n", reg->DDRC_TIMING2);
	dump_ddr("DDRC_TIMING3            0x%08x\n", reg->DDRC_TIMING3);
	dump_ddr("DDRC_TIMING4            0x%08x\n", reg->DDRC_TIMING4);
	dump_ddr("DDRC_TIMING5            0x%08x\n", reg->DDRC_TIMING5);
	dump_ddr("DDRC_TIMING8            0x%08x\n", reg->DDRC_TIMING8);
	dump_ddr("DDRC_ADDRMAP0           0x%08x\n", reg->DDRC_ADDRMAP0);
	dump_ddr("DDRC_ADDRMAP1           0x%08x\n", reg->DDRC_ADDRMAP1);
	dump_ddr("DDRC_ADDRMAP2           0x%08x\n", reg->DDRC_ADDRMAP2);
	dump_ddr("DDRC_ADDRMAP3           0x%08x\n", reg->DDRC_ADDRMAP3);
	dump_ddr("DDRC_ADDRMAP4           0x%08x\n", reg->DDRC_ADDRMAP4);
	dump_ddr("DDRC_ADDRMAP5           0x%08x\n", reg->DDRC_ADDRMAP5);
	dump_ddr("DDRC_ADDRMAP6           0x%08x\n", reg->DDRC_ADDRMAP6);
	dump_ddr("DDRC_RFSHCTL3           0x%08x\n", reg->DDRC_RFSHCTL3);
	dump_ddr("DDRC_RFSHTMG            0x%08x\n", reg->DDRC_RFSHTMG);
	dump_ddr("DDRC_ODTCFG             0x%08x\n", reg->DDRC_ODTCFG);
	dump_ddr("DDRC_DFITMG0            0x%08x\n", reg->DDRC_DFITMG0);
	dump_ddr("DDRC_DFITMG1            0x%08x\n", reg->DDRC_DFITMG1);
	dump_ddr("DDRC_DFIUPD0            0x%08x\n", reg->DDRC_DFIUPD0);

	dump_ddr("DDRP_REG_CHANNEL_EN     0x%08x\n", reg->DDRP_REG_CHANNEL_EN);
	dump_ddr("DDRP_CWL_FRE_OP0        0x%08x\n", reg->DDRP_CWL_FRE_OP0);
	dump_ddr("DDRP_CL_FRE_OP0         0x%08x\n", reg->DDRP_CL_FRE_OP0);
	dump_ddr("DDRP_AL_FRE_OP0         0x%08x\n", reg->DDRP_AL_FRE_OP0);
	dump_ddr("DDRP_REG_PHY_TRFC       0x%08x\n", reg->DDRP_REG_PHY_TRFC);
	dump_ddr("DDRP_REG_PHY_TREFI      0x%08x\n", reg->DDRP_REG_PHY_TREFI);
	dump_ddr("DDRP_MEM_SELECT_T       0x%08x\n", reg->DDRP_MEM_SELECT_T);

	dump_ddr("DDRP_REG_PLLPOSTDIVEN   0x%08x\n", reg->DDRP_REG_PLLPOSTDIVEN);
	dump_ddr("DDRP_REG_PLLPOSTDIV     0x%08x\n", reg->DDRP_REG_PLLPOSTDIV);
}

static void dump_ddr_timing_test(struct ddr_reg_value *reg)
{
#ifdef CONFIG_DDR_REMAP
	dump_ddr_test("open ddr Remap func\n");
#endif
#ifdef CONFIG_DDR_SOFT_TRAIN
	dump_ddr_test("open ddr SoftTraining func\n");
#endif
	dump_ddr_test("tCK %dps\n", __ps_per_tck);
}

void dump_generated_reg_define(struct ddr_reg_value *reg)
{
	dump_ddr_info(reg);
	dump_ddr_timing(reg);

#ifdef DDR_PARAMS_STRUCT
	printf("	},\n");
#endif

	dump_ddr_timing_test(reg);

}