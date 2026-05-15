/*
 * DDR parameters data structure.
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

#ifndef __DDR_PARAMS_H__
#define __DDR_PARAMS_H__

enum ddr_type{
	DDR3,
	LPDDR,
	LPDDR2,
	LPDDR3,
	DDR4,
	LPDDR4,
	DDR2,
};


/* ----------------------- */
struct size {
	uint32_t chip0;
	uint32_t chip1;
};


#define ddr_common_params		\
	uint32_t tRAS_max;				\
	uint32_t tRAS_min;				\
	uint32_t tRP;				\
	uint32_t tRCD;				\
	uint32_t tRC;				\
	uint32_t tWR;				\
	uint32_t tRRD;				\
	uint32_t tWTR;				\
	uint32_t tRFC;				\
	uint32_t tXP;				\
	uint32_t tCKE;				\
	uint32_t AL;				\
	uint32_t RL;				\
	uint32_t WL;				\
	uint32_t BL;				\
	uint32_t tREFI;

struct ddr3_params {
	ddr_common_params;
	uint32_t tCKESR;
	uint32_t tCKSRE;
	uint32_t tCKSRX;
	uint32_t tXSDLL;
	uint32_t tMOD;
	uint32_t tXPDLL;
	uint32_t tXS;
	uint32_t tRTP;
	uint32_t tCCD;
	uint32_t tFAW;
	uint32_t tMRD;
	uint32_t tCL;
	uint32_t tCWL;
	uint32_t tAL;
};


typedef union ddr_mr0 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned BL_a0_a1:2;
		unsigned CL_a2:1;
		unsigned RBT_a3:1;
		unsigned CL_a4_a6:3;
		unsigned TM_a7:1;
		unsigned DLL_reset_a8:1;
		unsigned WR_a9_a11:3;
		unsigned PPD_a12:1;
		unsigned reserved_a13_a15:3;
		unsigned reserved_a16_a31:16;
	} ddr3; /* MR0 */

	struct {
		unsigned BL_a0_a2:3;
		unsigned BT_a3:1;
		unsigned CL_a4_a6:3;
		unsigned TM_a7:1;
		unsigned DLL_reset_a8:1;
		unsigned WR_a9_a11:3;
		unsigned PD_a12:1;
		unsigned BA_a13_a14:2;
		unsigned reserved_a15:1;
		unsigned reserved_a16_a31:16;
	} ddr2; /* MR */
} ddr_mr0_t;

typedef union ddr_mr1 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned DLL_enable_a0:1;
		unsigned DIC_a1:1;
		unsigned RTT_Nom_a2:1;
		unsigned AL_a3_a4:2;
		unsigned DIC_a5:1;
		unsigned RTT_Nom_a6:1;
		unsigned Level_a7:1; /* write leveling enable */
		unsigned reserved_a8:1;
		unsigned RTT_Nom_a9:1;
		unsigned reserved_a10:1;
		unsigned TDQS_a11:1;
		unsigned Qoff_a12:1; /* Qoff */
		unsigned reserved_a13_a15:3;
		unsigned reserved_a16_a31:16;
	} ddr3; /* MR1 */

	struct {
		unsigned BL_a0_a2:3;
		unsigned reserved_a3_a4:2;
		unsigned nWR_a5_a7:3;
		unsigned MA_a8_a15:8;
		unsigned reserved_a16_a31:16;
	} lpddr3; /* MR1 */

	struct {
		unsigned DLL_enable_a0:1;
		unsigned DIC_a1:1;
		unsigned RTT_Nom_a2:1;
		unsigned AL_a3_a5:3;
		unsigned RTT_Nom_a6:1;
		unsigned OCD_program_a7_a9:3;
		unsigned DQS_Enable_a10:1;
		unsigned reserved_a11:1;
		unsigned Qoff_a12:1; /* Qoff */
		unsigned BA_a13_a14:2;
		unsigned reserved_a15:1;
		unsigned reserved_a16_a31:16;
	} ddr2; /* EMR */
} ddr_mr1_t;

typedef union ddr_mr2 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned PASR_a0_a2:3;
		unsigned CWL_a3_a5:3;
		unsigned ASR_a6:1;
		unsigned SRT_a7:1;
		unsigned reserved_a8:1;
		unsigned RTT_WR_a9_a10:2;
		unsigned reserved_a11_a15:5;
		unsigned reserved_a16_a31:16;
	} ddr3; /* MR2 */

	struct {
		unsigned RL_WL_a0_a3:4;
		unsigned nWRE_a4:1;
		unsigned reserved_a5:1;
		unsigned WL_Select_a6:1;
		unsigned WR_Lev_a7:1;
		unsigned MA_a8_a15:8;
		unsigned reserved_a16_a31:16;
	} lpddr3; /* MR2 */

	struct {
		unsigned PASR_a0_a2:3;
		unsigned DCC_a3:1;
		unsigned reserved_a4_a6:3;
		unsigned SRF_a7:1;
		unsigned reserved_a8_a12:5;
		unsigned BA_a13_a14:2;
		unsigned reserved_a15:1;
		unsigned reserved_a16_a31:16;
	} ddr2; /* EMR2 */
} ddr_mr2_t;

typedef union ddr_mr3 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned MPR_Loc_a0_a1:2;
		unsigned MPR_a2:1;
		unsigned reserved_a3_a15:13;
		unsigned reserved_a16_a31:16;
	} ddr3; /* MR3 */
	struct {
		unsigned DS_a0_a3:4;
		unsigned reserved_a4_a7:4;
		unsigned MA_a8_a15:8;
		unsigned reserved_a16_a31:16;
	} lpddr3; /* MR3 */
	struct {
		unsigned reserved_a0_a12:13;
		unsigned BA_a13_a14:2;
		unsigned reserved_a15:1;
		unsigned reserved_a16_a31:16;
	} ddr2; /* EMR3 */
} ddr_mr3_t;

typedef union ddr_mr4 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned Refresh_Rate_a0_a2:3;
		unsigned reserved_a3_a6:4;
		unsigned TUF_a7:1;
		unsigned MA_a8_a15:8;
		unsigned reserved_a16_a31:16;
	} lpddr3; /* MR4 */
} ddr_mr4_t;

typedef union ddr_mr10 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned Calibration_Code_a0_a7:8;
		unsigned MA_a8_a15:8;
		unsigned reserved_a16_a31:16;
	} lpddr3; /* MR10 */
} ddr_mr10_t;

typedef union ddr_mr11 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned DQ_ODT_a0_a1:2;
		unsigned PD_CTL_a2:1;
		unsigned reserved_a3_a7:5;
		unsigned MA_a8_a15:8;
		unsigned reserved_a16_a31:16;
	} lpddr3; /* MR11 */
} ddr_mr11_t;

typedef union ddr_mr63 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned X_a0_a7:8;
		unsigned MA_a8_a15:8;
		unsigned reserved_a16_a31:16;
	} lpddr3; /* MR63 */
} ddr_mr63_t;

struct ddr_params_common
{
	ddr_common_params;
};

union private_params {
	struct ddr_params_common ddr_base_params;
	// struct lpddr_params lpddr_params;
	// struct ddr2_params  ddr2_params;
	struct ddr3_params ddr3_params;
	// struct lpddr2_params lpddr2_params;
	// struct lpddr3_params lpddr3_params;
};

struct ddr_params {
	uint32_t type;
	uint32_t freq;
	uint32_t div;
	uint32_t cs0;
	uint32_t cs1;
	uint32_t dw32;
	uint32_t cl;
	uint32_t cwl;
	uint32_t bl;
	uint32_t col;
	uint32_t row;
	uint32_t col1;
	uint32_t row1;
	uint32_t bank;
	uint32_t bit;
	struct size size;
	union private_params private_params;
#ifdef CONFIG_DDR_INNOPHY
	ddr_mr0_t mr0;
	ddr_mr1_t mr1;
	ddr_mr2_t mr2;
	ddr_mr3_t mr3;

	// ddr_mr10_t mr10;
	// ddr_mr11_t mr11;
	// ddr_mr63_t mr63;
#endif
};

#endif /* __DDR_PARAMS_H__ */
