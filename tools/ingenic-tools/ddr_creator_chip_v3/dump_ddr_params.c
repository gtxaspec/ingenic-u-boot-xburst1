#include "ddr_params_creator.h"

// #ifdef CONFIG_DDR_PARAMS_STRUCT
// #define dump_ddr(fmt, args...) printf("		." fmt, ##args)
// #else
// #define dump_ddr(fmt, args...) printf("#define " fmt, ##args)
// #endif

#define dump_ddr_test(fmt, args...) printf("// " fmt, ##args)

#ifdef CONFIG_DDR_PARAMS_STRUCT
#define DDR_DUMP_REG(reg_name)  printf("        .%-21s = 0x%08x,\n", #reg_name, reg->reg_name);
#else
#define DDR_DUMP_REG(reg_name)   printf("#define %-21s 0x%08x\n", #reg_name, reg->reg_name);
#endif



static void dump_ddr_info(struct ddr_reg_value *reg)
{
#ifndef CONFIG_DDR_PARAMS_STRUCT
	printf("#define DDR_NAME              \"%s\"\n", reg->h.name);
	printf("#define DDR_FREQ              %10d //%dMHz  tCK %dps\n", reg->h.freq, reg->h.freq / 1000000, __ps_per_tck);
	printf("#define DDR_ID                0x%08x\n", reg->h.id);
	printf("#define DDR_TYPE              0x%08x\n", reg->h.type);
	printf("#define DDR_SIZE              0x%08x //%dMB\n", reg->h.size, reg->h.size / 1024 / 1024);
#else
	printf("    {\n");
	printf("        .h = { \n");
	printf("            .name = \"%s\",\n", reg->h.name);
	printf("            .id   = 0x%08x,\n", reg->h.id);
	printf("            .type = 0x%08x,\n", reg->h.type);
	printf("            .freq = %d,  // %dMHz\n", reg->h.freq, reg->h.freq / 1000000);
	printf("            .size = 0x%08x, //%dMB\n", reg->h.size, reg->h.size / 1024 / 1024);
	printf("        },\n");
#endif
}

static void dump_ddr_timing(struct ddr_reg_value *reg)
{

//	printf("DDRC_SWCTLSTATIC        0x%08x\n", reg->DDRC_SWCTLSTATIC);
//	printf("DDRC_SWCTL              0x%08x\n", reg->DDRC_SWCTL);
	DDR_DUMP_REG(DDRC_MSTR);
	DDR_DUMP_REG(DDRC_INIT0);
	DDR_DUMP_REG(DDRC_INIT1);
	DDR_DUMP_REG(DDRC_INIT2);
	DDR_DUMP_REG(DDRC_INIT3);
	DDR_DUMP_REG(DDRC_INIT4);
	dump_ddr_test("#define DDRC_INIT3              0x%08x //MR0 0x%08x MR1 0x%08x\n", reg->DDRC_INIT3, (reg->DDRC_INIT3 >> 16) & 0xffff, reg->DDRC_INIT3 & 0xffff);
	dump_ddr_test("#define DDRC_INIT4              0x%08x //MR2 0x%08x MR3 0x%08x\n", reg->DDRC_INIT4, (reg->DDRC_INIT4 >> 16) & 0xffff, reg->DDRC_INIT4 & 0xffff);
	DDR_DUMP_REG(DDRC_INIT5);
	DDR_DUMP_REG(DDRC_TIMING0);
	DDR_DUMP_REG(DDRC_TIMING1);
	DDR_DUMP_REG(DDRC_TIMING2);
	DDR_DUMP_REG(DDRC_TIMING3);
	DDR_DUMP_REG(DDRC_TIMING4);
	DDR_DUMP_REG(DDRC_TIMING5);
	DDR_DUMP_REG(DDRC_TIMING6);
	DDR_DUMP_REG(DDRC_TIMING7);
	DDR_DUMP_REG(DDRC_TIMING8);
	DDR_DUMP_REG(DDRC_TIMING14);
	DDR_DUMP_REG(DDRC_ADDRMAP0);
	DDR_DUMP_REG(DDRC_ADDRMAP1);
	DDR_DUMP_REG(DDRC_ADDRMAP2);
	DDR_DUMP_REG(DDRC_ADDRMAP3);
	DDR_DUMP_REG(DDRC_ADDRMAP4);
	DDR_DUMP_REG(DDRC_ADDRMAP5);
	DDR_DUMP_REG(DDRC_ADDRMAP6);
	DDR_DUMP_REG(DDRC_RFSHCTL3);
	DDR_DUMP_REG(DDRC_RFSHTMG);
	DDR_DUMP_REG(DDRC_ODTCFG);
	DDR_DUMP_REG(DDRC_DFITMG0);
	DDR_DUMP_REG(DDRC_DFITMG1);
	DDR_DUMP_REG(DDRC_DFIUPD0);
	DDR_DUMP_REG(DDRP_REG_CHANNEL_EN);
	DDR_DUMP_REG(DDRP_CWL_FRE_OP0);
	DDR_DUMP_REG(DDRP_CL_FRE_OP0);
	DDR_DUMP_REG(DDRP_AL_FRE_OP0);
	DDR_DUMP_REG(DDRP_REG_PHY_TRFC);
	DDR_DUMP_REG(DDRP_REG_PHY_TREFI);
	DDR_DUMP_REG(DDRP_MEM_SELECT_T);
	DDR_DUMP_REG(DDRP_REG_PLLPOSTDIVEN);
	DDR_DUMP_REG(DDRP_REG_PLLPOSTDIV);

#if 0 	/* DDRP_MAP 使用单独数组，此处注释 */
	DDR_DUMP_REG(DDRP_CMD0_WRAP);
	DDR_DUMP_REG(DDRP_CMD1_WRAP);
	DDR_DUMP_REG(DDRP_CMD2_WRAP);
	DDR_DUMP_REG(DDRP_CMD3_WRAP);
	DDR_DUMP_REG(DDRP_CMD4_WRAP);
	DDR_DUMP_REG(DDRP_CMD5_WRAP);
	DDR_DUMP_REG(DDRP_CMD6_WRAP);
	DDR_DUMP_REG(DDRP_CMD7_WRAP);
	DDR_DUMP_REG(DDRP_CMD8_WRAP);
	DDR_DUMP_REG(DDRP_CMD9_WRAP);
	DDR_DUMP_REG(DDRP_CMD10_WRAP);
	DDR_DUMP_REG(DDRP_CMD11_WRAP);
	DDR_DUMP_REG(DDRP_CMD12_WRAP);
	DDR_DUMP_REG(DDRP_CMD13_WRAP);
	DDR_DUMP_REG(DDRP_CMD16_WRAP);
	DDR_DUMP_REG(DDRP_CMD17_WRAP);
	DDR_DUMP_REG(DDRP_CMD18_WRAP);
	DDR_DUMP_REG(DDRP_CMD19_WRAP);
	DDR_DUMP_REG(DDRP_CMD20_WRAP);
	DDR_DUMP_REG(DDRP_CMD21_WRAP);
	DDR_DUMP_REG(DDRP_L_DM_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ0_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ1_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ2_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ3_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ4_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ5_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ6_WRAP);
	DDR_DUMP_REG(DDRP_L_DQ7_WRAP);
	DDR_DUMP_REG(DDRP_H_DM_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ0_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ1_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ2_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ3_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ4_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ5_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ6_WRAP);
	DDR_DUMP_REG(DDRP_H_DQ7_WRAP);
#endif
}

static void dump_ddr_timing_test(struct ddr_reg_value *reg)
{
#ifdef CONFIG_DDR_REMAP
	dump_ddr_test("open ddr Remap func\n");
#endif
#ifdef CONFIG_DDR_SOFT_TRAIN
	dump_ddr_test("open ddr SoftTraining func\n");
#endif
	dump_ddr_test("tCK %dps\n", reg->ps_per_tck);
}

void dump_generated_reg_define(struct ddr_reg_value *reg)
{
	dump_ddr_info(reg);
	dump_ddr_timing(reg);

#ifdef CONFIG_DDR_PARAMS_STRUCT
	printf("    },\n");
#endif

	dump_ddr_timing_test(reg);
}

#ifdef CONFIG_DDR_PARAMS_STRUCT
void dump_generated_struct(void)
{
	printf("struct ddr_reg_value\n");
	printf("{\n");
	printf("	struct ddr_reg_header h;\n");
	printf("\n");
	printf("	/* DDRC */\n");
	printf("	unsigned int DDRC_SWCTLSTATIC;\n");
	printf("	unsigned int DDRC_SWCTL;\n");
	printf("	unsigned int DDRC_MSTR;\n");
	printf("	unsigned int DDRC_INIT0;\n");
	printf("	unsigned int DDRC_INIT1;\n");
	printf("	unsigned int DDRC_INIT2;\n");
	printf("	unsigned int DDRC_INIT3;\n");
	printf("	unsigned int DDRC_INIT4;\n");
	printf("	unsigned int DDRC_INIT5;\n");
	printf("	unsigned int DDRC_TIMING0;\n");
	printf("	unsigned int DDRC_TIMING1;\n");
	printf("	unsigned int DDRC_TIMING2;\n");
	printf("	unsigned int DDRC_TIMING3;\n");
	printf("	unsigned int DDRC_TIMING4;\n");
	printf("	unsigned int DDRC_TIMING5;\n");
	printf("	unsigned int DDRC_TIMING6;\n");
	printf("	unsigned int DDRC_TIMING7;\n");
	printf("	unsigned int DDRC_TIMING8;\n");
	printf("	unsigned int DDRC_TIMING14;\n");
	printf("	unsigned int DDRC_ADDRMAP0;\n");
	printf("	unsigned int DDRC_ADDRMAP1;\n");
	printf("	unsigned int DDRC_ADDRMAP2;\n");
	printf("	unsigned int DDRC_ADDRMAP3;\n");
	printf("	unsigned int DDRC_ADDRMAP4;\n");
	printf("	unsigned int DDRC_ADDRMAP5;\n");
	printf("	unsigned int DDRC_ADDRMAP6;\n");
	printf("	unsigned int DDRC_ADDRMAP7;\n");
	printf("	unsigned int DDRC_RFSHTMG;\n");
	printf("	unsigned int DDRC_RFSHCTL3;\n");
	printf("	unsigned int DDRC_ODTCFG;\n");
	printf("\n");
	printf("	unsigned int DDRC_DFITMG0;\n");
	printf("	unsigned int DDRC_DFITMG1;\n");
	printf("	unsigned int DDRC_DFIUPD0;\n");
	printf("\n");
	printf("	/* DDR PHY */\n");
	printf("	unsigned int DDRP_REG_CHANNEL_EN;\n");
	printf("	unsigned int DDRP_CWL_FRE_OP0;\n");
	printf("	unsigned int DDRP_CL_FRE_OP0;\n");
	printf("	unsigned int DDRP_AL_FRE_OP0;\n");
	printf("	unsigned int DDRP_REG_PHY_TRFC;\n");
	printf("	unsigned int DDRP_REG_PHY_TREFI;\n");
	printf("	unsigned int DDRP_MEM_SELECT_T;\n");
	printf("	unsigned int DDRP_REG_PLLPOSTDIVEN;\n");
	printf("	unsigned int DDRP_REG_PLLPOSTDIV;\n");

#if 0 	/* DDRP_MAP 使用单独数组，此处注释 */
	printf("	/* DDRP WRAP */\n");
	printf("	unsigned int DDRP_CMD0_WRAP;\n");
	printf("	unsigned int DDRP_CMD1_WRAP;\n");
	printf("	unsigned int DDRP_CMD2_WRAP;\n");
	printf("	unsigned int DDRP_CMD3_WRAP;\n");
	printf("	unsigned int DDRP_CMD4_WRAP;\n");
	printf("	unsigned int DDRP_CMD5_WRAP;\n");
	printf("	unsigned int DDRP_CMD6_WRAP;\n");
	printf("	unsigned int DDRP_CMD7_WRAP;\n");
	printf("	unsigned int DDRP_CMD8_WRAP;\n");
	printf("	unsigned int DDRP_CMD9_WRAP;\n");
	printf("	unsigned int DDRP_CMD10_WRAP;\n");
	printf("	unsigned int DDRP_CMD11_WRAP;\n");
	printf("	unsigned int DDRP_CMD12_WRAP;\n");
	printf("	unsigned int DDRP_CMD13_WRAP;\n");
	printf("	unsigned int DDRP_CMD16_WRAP;\n");
	printf("	unsigned int DDRP_CMD17_WRAP;\n");
	printf("	unsigned int DDRP_CMD18_WRAP;\n");
	printf("	unsigned int DDRP_CMD19_WRAP;\n");
	printf("	unsigned int DDRP_CMD20_WRAP;\n");
	printf("	unsigned int DDRP_CMD21_WRAP;\n");
	printf("	unsigned int DDRP_L_DM_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ0_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ1_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ2_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ3_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ4_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ5_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ6_WRAP;\n");
	printf("	unsigned int DDRP_L_DQ7_WRAP;\n");
	printf("	unsigned int DDRP_H_DM_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ0_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ1_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ2_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ3_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ4_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ5_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ6_WRAP;\n");
	printf("	unsigned int DDRP_H_DQ7_WRAP;\n");
	printf("\n");
#endif
	printf("	/* LPDDR */\n");
	printf("	unsigned int DDRC_MRCTRL1_MR1;\n");
	printf("	unsigned int DDRC_MRCTRL1_MR2;\n");
	printf("	unsigned int DDRC_MRCTRL1_MR3;\n");
	printf("	unsigned int DDRC_MRCTRL1_MR10;\n");
	printf("	unsigned int DDRC_MRCTRL1_MR11;\n");
	printf("	unsigned int DDRC_MRCTRL1_MR63;\n");
	printf("	/* LPDDR */\n");
	printf("};\n\n");
}


void dump_generated_define(void)
{
	printf("#define DDR_NAME                (global_reg_value->h.name)\n");
	printf("#define DDR_FREQ                (global_reg_value->h.freq)\n");
	printf("#define DDR_ID                  (global_reg_value->h.id)\n");
	printf("#define DDR_TYPE                (global_reg_value->h.type)\n");
	printf("#define DDR_SIZE                (global_reg_value->h.size)\n");
	printf("#define DDRC_MSTR               (global_reg_value->DDRC_MSTR)\n");
	printf("#define DDRC_INIT0              (global_reg_value->DDRC_INIT0)\n");
	printf("#define DDRC_INIT1              (global_reg_value->DDRC_INIT1)\n");
	printf("#define DDRC_INIT2              (global_reg_value->DDRC_INIT2)\n");
	printf("#define DDRC_INIT3              (global_reg_value->DDRC_INIT3)\n");
	printf("#define DDRC_INIT4              (global_reg_value->DDRC_INIT4)\n");
	printf("#define DDRC_INIT5              (global_reg_value->DDRC_INIT5)\n");
	printf("#define DDRC_TIMING0            (global_reg_value->DDRC_TIMING0)\n");
	printf("#define DDRC_TIMING1            (global_reg_value->DDRC_TIMING1)\n");
	printf("#define DDRC_TIMING2            (global_reg_value->DDRC_TIMING2)\n");
	printf("#define DDRC_TIMING3            (global_reg_value->DDRC_TIMING3)\n");
	printf("#define DDRC_TIMING4            (global_reg_value->DDRC_TIMING4)\n");
	printf("#define DDRC_TIMING5            (global_reg_value->DDRC_TIMING5)\n");
	printf("#define DDRC_TIMING6            (global_reg_value->DDRC_TIMING6)\n");
	printf("#define DDRC_TIMING7            (global_reg_value->DDRC_TIMING7)\n");
	printf("#define DDRC_TIMING8            (global_reg_value->DDRC_TIMING8)\n");
	printf("#define DDRC_TIMING14           (global_reg_value->DDRC_TIMING14)\n");
	printf("#define DDRC_ADDRMAP0           (global_reg_value->DDRC_ADDRMAP0)\n");
	printf("#define DDRC_ADDRMAP1           (global_reg_value->DDRC_ADDRMAP1)\n");
	printf("#define DDRC_ADDRMAP2           (global_reg_value->DDRC_ADDRMAP2)\n");
	printf("#define DDRC_ADDRMAP3           (global_reg_value->DDRC_ADDRMAP3)\n");
	printf("#define DDRC_ADDRMAP4           (global_reg_value->DDRC_ADDRMAP4)\n");
	printf("#define DDRC_ADDRMAP5           (global_reg_value->DDRC_ADDRMAP5)\n");
	printf("#define DDRC_ADDRMAP6           (global_reg_value->DDRC_ADDRMAP6)\n");
	printf("#define DDRC_RFSHCTL3           (global_reg_value->DDRC_RFSHCTL3)\n");
	printf("#define DDRC_RFSHTMG            (global_reg_value->DDRC_RFSHTMG)\n");
	printf("#define DDRC_ODTCFG             (global_reg_value->DDRC_ODTCFG)\n");
	printf("#define DDRC_DFITMG0            (global_reg_value->DDRC_DFITMG0)\n");
	printf("#define DDRC_DFITMG1            (global_reg_value->DDRC_DFITMG1)\n");
	printf("#define DDRC_DFIUPD0            (global_reg_value->DDRC_DFIUPD0)\n");
	printf("#define DDRP_REG_CHANNEL_EN     (global_reg_value->DDRP_REG_CHANNEL_EN)\n");
	printf("#define DDRP_CWL_FRE_OP0        (global_reg_value->DDRP_CWL_FRE_OP0)\n");
	printf("#define DDRP_CL_FRE_OP0         (global_reg_value->DDRP_CL_FRE_OP0)\n");
	printf("#define DDRP_AL_FRE_OP0         (global_reg_value->DDRP_AL_FRE_OP0)\n");
	printf("#define DDRP_REG_PHY_TRFC       (global_reg_value->DDRP_REG_PHY_TRFC)\n");
	printf("#define DDRP_REG_PHY_TREFI      (global_reg_value->DDRP_REG_PHY_TREFI)\n");
	printf("#define DDRP_MEM_SELECT_T       (global_reg_value->DDRP_MEM_SELECT_T)\n");
	printf("#define DDRP_REG_PLLPOSTDIVEN   (global_reg_value->DDRP_REG_PLLPOSTDIVEN)\n");
	printf("#define DDRP_REG_PLLPOSTDIV     (global_reg_value->DDRP_REG_PLLPOSTDIV)\n");

#if 0 	/* DDRP_MAP 使用单独数组，此处注释 */
	printf("#define DDRP_CMD0_WRAP          (global_reg_value->DDRP_CMD0_WRAP)\n");
	printf("#define DDRP_CMD1_WRAP          (global_reg_value->DDRP_CMD1_WRAP)\n");
	printf("#define DDRP_CMD2_WRAP          (global_reg_value->DDRP_CMD2_WRAP)\n");
	printf("#define DDRP_CMD3_WRAP          (global_reg_value->DDRP_CMD3_WRAP)\n");
	printf("#define DDRP_CMD4_WRAP          (global_reg_value->DDRP_CMD4_WRAP)\n");
	printf("#define DDRP_CMD5_WRAP          (global_reg_value->DDRP_CMD5_WRAP)\n");
	printf("#define DDRP_CMD6_WRAP          (global_reg_value->DDRP_CMD6_WRAP)\n");
	printf("#define DDRP_CMD7_WRAP          (global_reg_value->DDRP_CMD7_WRAP)\n");
	printf("#define DDRP_CMD8_WRAP          (global_reg_value->DDRP_CMD8_WRAP)\n");
	printf("#define DDRP_CMD9_WRAP          (global_reg_value->DDRP_CMD9_WRAP)\n");
	printf("#define DDRP_CMD10_WRAP         (global_reg_value->DDRP_CMD10_WRAP)\n");
	printf("#define DDRP_CMD11_WRAP         (global_reg_value->DDRP_CMD11_WRAP)\n");
	printf("#define DDRP_CMD12_WRAP         (global_reg_value->DDRP_CMD12_WRAP)\n");
	printf("#define DDRP_CMD13_WRAP         (global_reg_value->DDRP_CMD13_WRAP)\n");
	printf("#define DDRP_CMD16_WRAP         (global_reg_value->DDRP_CMD16_WRAP)\n");
	printf("#define DDRP_CMD17_WRAP         (global_reg_value->DDRP_CMD17_WRAP)\n");
	printf("#define DDRP_CMD18_WRAP         (global_reg_value->DDRP_CMD18_WRAP)\n");
	printf("#define DDRP_CMD19_WRAP         (global_reg_value->DDRP_CMD19_WRAP)\n");
	printf("#define DDRP_CMD20_WRAP         (global_reg_value->DDRP_CMD20_WRAP)\n");
	printf("#define DDRP_CMD21_WRAP         (global_reg_value->DDRP_CMD21_WRAP)\n");
	printf("#define DDRP_L_DM_WRAP          (global_reg_value->DDRP_L_DM_WRAP)\n");
	printf("#define DDRP_L_DQ0_WRAP         (global_reg_value->DDRP_L_DQ0_WRAP)\n");
	printf("#define DDRP_L_DQ1_WRAP         (global_reg_value->DDRP_L_DQ1_WRAP)\n");
	printf("#define DDRP_L_DQ2_WRAP         (global_reg_value->DDRP_L_DQ2_WRAP)\n");
	printf("#define DDRP_L_DQ3_WRAP         (global_reg_value->DDRP_L_DQ3_WRAP)\n");
	printf("#define DDRP_L_DQ4_WRAP         (global_reg_value->DDRP_L_DQ4_WRAP)\n");
	printf("#define DDRP_L_DQ5_WRAP         (global_reg_value->DDRP_L_DQ5_WRAP)\n");
	printf("#define DDRP_L_DQ6_WRAP         (global_reg_value->DDRP_L_DQ6_WRAP)\n");
	printf("#define DDRP_L_DQ7_WRAP         (global_reg_value->DDRP_L_DQ7_WRAP)\n");
	printf("#define DDRP_H_DM_WRAP          (global_reg_value->DDRP_H_DM_WRAP)\n");
	printf("#define DDRP_H_DQ0_WRAP         (global_reg_value->DDRP_H_DQ0_WRAP)\n");
	printf("#define DDRP_H_DQ1_WRAP         (global_reg_value->DDRP_H_DQ1_WRAP)\n");
	printf("#define DDRP_H_DQ2_WRAP         (global_reg_value->DDRP_H_DQ2_WRAP)\n");
	printf("#define DDRP_H_DQ3_WRAP         (global_reg_value->DDRP_H_DQ3_WRAP)\n");
	printf("#define DDRP_H_DQ4_WRAP         (global_reg_value->DDRP_H_DQ4_WRAP)\n");
	printf("#define DDRP_H_DQ5_WRAP         (global_reg_value->DDRP_H_DQ5_WRAP)\n");
	printf("#define DDRP_H_DQ6_WRAP         (global_reg_value->DDRP_H_DQ6_WRAP)\n");
	printf("#define DDRP_H_DQ7_WRAP         (global_reg_value->DDRP_H_DQ7_WRAP)\n");
#endif

}
#endif
