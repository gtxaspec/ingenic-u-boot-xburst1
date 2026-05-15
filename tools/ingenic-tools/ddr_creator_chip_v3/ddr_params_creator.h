#ifndef __DDR_PARAMS_CREATOR_H__
#define __DDR_PARAMS_CREATOR_H__

#ifndef CONFIG_SPL_BUILD
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define debug(fmt, ...)
#else
#include <common.h>
#include <generated/ddr_reg_values.h>
#endif	/* define CONFIG_SPL_BUILD */

#include <config.h>
#include <ddr/ddr_common.h>

#define out_error(fmt,y...) printf("#error "fmt,##y)
#define out_warn(fmt,y...) printf("/* "fmt "*/\n\n",##y)


struct ddr_creator_ops{
	int type;
	void (*ddrc_params_creator)(struct ddrc_reg *ddrc_reg, struct ddr_chip_info *p);
	void (*ddrp_params_creator)(struct ddrp_reg *ddrc, struct ddr_params *p);
	void (*fill_reg_value)(struct ddr_reg_value *reg, struct ddrc_reg *ddrc, struct ddr_chip_info *p);
};

#define ROUND_DOWN 0
#define ROUND_UP 1

int RoundUp(int a, int b);
int RoundDown(int a, int b);
int ps2cycle_ceil(int ps, int div_tck);
void register_ddr_creator(struct ddr_creator_ops *ops);
void ddr3_creator_init(void);
int create_one_ddr_params(struct ddr_chip_info *p, struct ddr_reg_value *reg);
int init_supported_ddr(void);
void create_supported_ddr_params(struct ddr_reg_value *reg_values);
void dump_generated_reg_define(struct ddr_reg_value *reg);
void fill_addrmap(struct ddrc_reg *ddrc, struct ddr_chip_info *p);
void ddrc_set_d2d3_dfitmg0(struct ddrc_reg *ddrc, struct ddr_chip_info *p);
void ddrc_set_d2d3_dfitmg1(struct ddrc_reg *ddrc, struct ddr_chip_info *p);
void ddrc_set_d2d3_dfiupd0(struct ddrc_reg *ddrc, struct ddr_chip_info *p);

/* ddr_params_creator.h */
#define BETWEEN(T, MIN, MAX)				\
	do{						\
		if (T < MIN) {				\
			out_error("%d is too small and check %s %d!\n",T,__FILE__,__LINE__); \
			assert(1);			\
		} else if (T > MAX) {			\
			out_error("%d is too big and check %s %d!\n",T,__FILE__,__LINE__);	\
			assert(1);			\
		}					\
	}while(0)

struct ddrp_reg {
	uint32_t cl;
	uint32_t cwl;

	unsigned int soft_reset;
	unsigned int soft_reset0;
	unsigned int soft_reset1;
	unsigned int mem_select_t;
	unsigned int reg_burst_sel;
	unsigned int reg_channel_en;
	unsigned int reg_dq_invalid_value;

	unsigned int AL_FRE_OP3;
	unsigned int AL_FRE_OP2;
	unsigned int AL_FRE_OP1;
	unsigned int AL_FRE_OP0;

	unsigned int CL_FRE_OP3;
	unsigned int CL_FRE_OP2;
	unsigned int CL_FRE_OP1;
	unsigned int CL_FRE_OP0;

	unsigned int CWL_FRE_OP3;
	unsigned int CWL_FRE_OP2;
	unsigned int CWL_FRE_OP1;
	unsigned int CWL_FRE_OP0;

	unsigned int reg_phy_refresh_en;
	unsigned int reg_phy_max_refi_enable;
	unsigned int reg_max_refi_cnt;
	unsigned int reg_phy_trfc;
	unsigned int reg_phy_trefi;

	unsigned int reg_pllpostdiven;
	unsigned int reg_pllpostdiv;
};

struct ddr_reg_value
{
	struct ddr_reg_header h;

	/* DDRC */
	unsigned int DDRC_SWCTLSTATIC;
	unsigned int DDRC_SWCTL;
	unsigned int DDRC_MSTR;
	unsigned int DDRC_INIT0;
	unsigned int DDRC_INIT1;
	unsigned int DDRC_INIT2;
	unsigned int DDRC_INIT3;
	unsigned int DDRC_INIT4;
	unsigned int DDRC_INIT5;
	unsigned int DDRC_TIMING0;
	unsigned int DDRC_TIMING1;
	unsigned int DDRC_TIMING2;
	unsigned int DDRC_TIMING3;
	unsigned int DDRC_TIMING4;
	unsigned int DDRC_TIMING5;
	unsigned int DDRC_TIMING6;
	unsigned int DDRC_TIMING7;
	unsigned int DDRC_TIMING8;
	unsigned int DDRC_TIMING14;
	unsigned int DDRC_ADDRMAP0;
	unsigned int DDRC_ADDRMAP1;
	unsigned int DDRC_ADDRMAP2;
	unsigned int DDRC_ADDRMAP3;
	unsigned int DDRC_ADDRMAP4;
	unsigned int DDRC_ADDRMAP5;
	unsigned int DDRC_ADDRMAP6;
	unsigned int DDRC_ADDRMAP7;
	unsigned int DDRC_RFSHTMG;
	unsigned int DDRC_RFSHCTL3;
	unsigned int DDRC_ODTCFG;

	unsigned int DDRC_DFITMG0;
	unsigned int DDRC_DFITMG1;
	unsigned int DDRC_DFIUPD0;

	/* DDR PHY */
	unsigned int DDRP_REG_CHANNEL_EN;
	unsigned int DDRP_CWL_FRE_OP0;
	unsigned int DDRP_CL_FRE_OP0;
	unsigned int DDRP_AL_FRE_OP0;
	unsigned int DDRP_REG_PHY_TRFC;
	unsigned int DDRP_REG_PHY_TREFI;
	unsigned int DDRP_MEM_SELECT_T;
	unsigned int DDRP_REG_PLLPOSTDIVEN;
	unsigned int DDRP_REG_PLLPOSTDIV;

	/* LPDDR */
	unsigned int DDRC_MRCTRL1_MR1;
	unsigned int DDRC_MRCTRL1_MR2;
	unsigned int DDRC_MRCTRL1_MR3;
	unsigned int DDRC_MRCTRL1_MR10;
	unsigned int DDRC_MRCTRL1_MR11;
	unsigned int DDRC_MRCTRL1_MR63;
	/* LPDDR */
};

#endif
