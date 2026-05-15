/*
 * T40 ddr definitions
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
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
