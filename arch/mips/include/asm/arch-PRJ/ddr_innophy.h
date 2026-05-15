// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __DDR_H__
#define __DDR_H__

#include <asm/arch/base.h>

extern unsigned int __ps_per_tck;

#define DDR_SELECT_MAX__tCK_ps(tck, ps)						\
({										\
	unsigned int value;							\
	value = (tck * __ps_per_tck > ps) ? (tck * __ps_per_tck) : ps;   	\
	value;									\
})

#define DDR_SELECT_MIN__tCK_ps(tck, ps)						\
({										\
	unsigned int value;							\
	value = (tck * __ps_per_tck < ps) ? (tck * __ps_per_tck) : ps;   	\
	value;									\
})

#define DDR__ns(ns)   (ns * 1000)
#define DDR__ps(ps)   (ps)
#define DDR__tck(tck) (tck * __ps_per_tck)


struct ddr_reg_header {
	unsigned char name[32];
	unsigned int id;	/*DDR ID: */
	unsigned int type;	/*DDR type*/
	unsigned int freq;	/*ddr freqency.*/
	unsigned int size;	/*ddr size.*/
};

enum DDRP_CMD_MAP{
	DDRP_A0,
	DDRP_A1,
	DDRP_A2,
	DDRP_A3,
	DDRP_A4,
	DDRP_A5,
	DDRP_A6,
	DDRP_A7,
	DDRP_A8,
	DDRP_A9,
	DDRP_A10,
	DDRP_A11,
	DDRP_A12,
	DDRP_A13,
	DDRP_A14,
	DDRP_A15,
	DDRP_WEB,
	DDRP_CASB,
	DDRP_RASB,
	DDRP_BA0,
	DDRP_BA1,
	DDRP_BA2
};

enum DDRP_L_DQ_MAP{
	DDRP_DQ0,
	DDRP_DQ1,
	DDRP_DQ2,
	DDRP_DQ3,
	DDRP_DQ4,
	DDRP_DQ5,
	DDRP_DQ6,
	DDRP_DQ7,
	DDRP_DM0,
};

enum DDRP_H_DQ_MAP{
	DDRP_DQ8,
	DDRP_DQ9,
	DDRP_DQ10,
	DDRP_DQ11,
	DDRP_DQ12,
	DDRP_DQ13,
	DDRP_DQ14,
	DDRP_DQ15,
	DDRP_DM1,
};

enum {
	WINBOND_25NM_1G_D3       = 0xAA,
	ESMT_25NM_1G_D3          = 0x11,
	ETRON_25NM_1G_D3         = 0x22,
	NTC_20NM_1G_D3           = 0x33,
	ISSI_25NM_1G_D3          = 0x44,
	NTC_30NM_1G_D3           = 0x55,
	ETRON_38NM_1G_D3         = 0x66,
	WINBOND_20NM_1G_D3_933   = 0x77,
	NTC_20NM_2G_D3           = 0x99,
	WINBOND_20NM_1G_D3_1066  = 0xff,

	ESMT_25NM_512_D2         = 0xBB,
	ETRON_25NM_512_D2        = 0xCC,
	WINBOND_25NM_512_D2      = 0x88,
};

enum {
	// PRJ_L   = 0x99,
	PRJ_N   = 0xAA,
	// PRJ_XQ  = 0x22,
	// PRJ_ZL  = 0x55,
	PRJ_ZN  = 0x77,
	// PRJ_ZX  = 0x66,
	// PRJ_VL  = 0xCC,
	PRJ_VN  = 0xDD,
	// PRJ_NQH = 0x33,
	// PRJ_DL  = 0x11,
	// PRJ_VNP = 0xBB,
	// PRJ_A   = 0x44,
};


enum
{
	/* par */
	ODT_PD, // 0
	ODT_PU,
	CMD_RC_PD,
	CMD_RC_PU,
	CLK_RC_PD,
	CLK_RC_PU,
	DQX_RC_PD,
	DQX_RC_PU,
	VREF,
	KGD_ODT,
	KGD_DS,
	KGD_RTT_DIC,
	SKEW_DQS0R,
	SKEW_DQS1R,
	SKEW_DQRX,
	SKEW_DQS0T,
	SKEW_DQS1T,
	SKEW_DQTX,
	SKEW_TRX,
	/* flag */
	INDEX_EN,
	INDEX_ODT_PU,
	INDEX_ODT_PD,
	INDEX_CMD_PU,
	INDEX_CMD_PD,
	INDEX_CLK_PU,
	INDEX_CLK_PD,
	INDEX_DQ_PU,
	INDEX_DQ_PD,
	INDEX_VREF,
	INDEX_KGD,
	INDEX_DQSR,
	INDEX_DQST,
	INDEX_DQRX,
	INDEX_DQTX,
	DDR_PAR_NUM,
};

typedef struct
{
	int odt[5];
	int cmd_rc[5];
	int ck_rc[5];
	int dq_rc[5];
	int kgd_odt[3];
	int kgd_ds[2];
	int vref[8];
} hanming_ddr_data;

extern struct ddr_reg_value *g_ddr_param;
#endif /* __DDR_H__ */
