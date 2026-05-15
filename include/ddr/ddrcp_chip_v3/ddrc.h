/*
 * DDR Controller common data structure.
 * Used for X2000
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

#ifndef __DDRC_H__
#define __DDRC_H__


typedef union ddrc_TIMING0 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_ras_min:6;
		unsigned reserved6_7:2;
		unsigned t_ras_max:7;
		unsigned reserved15:1;
		unsigned t_faw:6;
		unsigned reserved22_23:2;
		unsigned wr2pre:7;
		unsigned reserved31:1;
	} b32;
} ddrc_TIMING0_t;


typedef union ddrc_TIMING1 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_rc:7;
		unsigned reserved7:1;
		unsigned rd2pre:6;
		unsigned reserved14_15:2;
		unsigned t_xp:5;
		unsigned reserved21_31:11;
	} b32;
} ddrc_TIMING1_t;


typedef union ddrc_TIMING2 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned wr2rd:6;
		unsigned reserved6_7:2;
		unsigned rd2wr:6;
		unsigned reserved14_15:2;
		unsigned read_latency:6;
		unsigned reserved22_23:2;
		unsigned write_latency:6;
		unsigned reserved30_31:2;
	} b32;
} ddrc_TIMING2_t;


typedef union ddrc_TIMING3 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_mod:10;
		unsigned reserved10_11:2;
		unsigned t_mrd:6;
		unsigned reserved18_19:2;
		unsigned t_mrw:10;		/* LPDDR2 */
		unsigned reserved30_31:2;
	} b32;
} ddrc_TIMING3_t;


typedef union ddrc_TIMING4 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_rp:5;
		unsigned reserved5_7:3;
		unsigned t_rrd:4;
		unsigned reserved12_15:4;
		unsigned t_ccd:4;
		unsigned reserved20_23:4;
		unsigned t_rcd:5;
		unsigned reserved29_31:3;
	} b32;
} ddrc_TIMING4_t;


typedef union ddrc_TIMING5 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_cke:5;
		unsigned reserved5_7:3;
		unsigned t_ckesr:8;
		unsigned t_cksre:8;
		unsigned t_cksrx:4;
		unsigned reserved28_31:4;
	} b32;
} ddrc_TIMING5_t;


typedef union ddrc_TIMING6 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_ckcsx:4;
		unsigned reserved4_15:12;
		unsigned t_ckdpdx:4;
		unsigned reserved20_23:4;
		unsigned t_ckdpde:4;
		unsigned reserved28_31:4;
	} b32;
} ddrc_TIMING6_t;


typedef union ddrc_TIMING7 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_ckpdx:4;
		unsigned reserved4_7:4;
		unsigned t_ckpde:4;
		unsigned reserved12_31:20;
	} b32;
} ddrc_TIMING7_t;


typedef union ddrc_TIMING8 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_xs_x32:7;
		unsigned reserved7:1;
		unsigned t_xs_dll_x32:7;
		unsigned reserved15:1;
		unsigned t_xs_abort_x32:7;
		unsigned reserved23:1;
		unsigned t_xs_fast_x32:7;
		unsigned reserved31:1;
	} b32;
} ddrc_TIMING8_t;


typedef union ddrc_TIMING9 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned wr2rd_s:6;
		unsigned reserved6_7:2;
		unsigned t_rrd_s:4;
		unsigned reserved12_15:4;
		unsigned t_ccd_s:3;
		unsigned reserved19_29:11;
		unsigned ddr4_wr_preamble:1;
		unsigned reserved31:1;
	} b32;
} ddrc_TIMING9_t;


typedef union ddrc_TIMING10 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_gear_hold:2;
		unsigned t_gear_setup:2;
		unsigned reserved4_7:4;
		unsigned t_cmd_gear:5;
		unsigned reserved13_15:3;
		unsigned t_sync_gear:5;
		unsigned reserved21_31:11;
	} b32;
} ddrc_TIMING10_t;


typedef union ddrc_TIMING14 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_xsr:12;
		unsigned reserved12_31:20;
	} b32;
} ddrc_TIMING14_t;


typedef union ddrc_TIMING15 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_stab_x32:8;
		unsigned reserved8_23:16;
		unsigned en_hwffc_t_stab:1;
		unsigned reserved25_30:6;
		unsigned en_dfi_lp_t_stab:1;
	} b32;
} ddrc_TIMING15_t;


typedef union ddrc_TIMING16 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_ccd_dlr:3;
		unsigned reserved3_7:5;
		unsigned t_rrd_dlr:3;
		unsigned reserved11_15:5;
		unsigned t_faw_dlr:5;
		unsigned reserved21_23:3;
		unsigned t_rp_ca_parity:8;
	} b32;
} ddrc_TIMING16_t;

typedef union ddrc_SWCTL {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned sw_done:1;
		unsigned reserved1_31:31;
	} b32;
} ddrc_SWCTL_t;

typedef union ddrc_SWCTLSTAIC {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned sw_staic_unlpck:1;
		unsigned reserved1_31:31;
	} b32;
} ddrc_SWCTLSTAIC_t;


typedef union ddrc_MSTR {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned ddr3:1;
		unsigned mobile:1;
		unsigned lpddr2:1;
		unsigned lpddr3:1;
		unsigned ddr4:1;
		unsigned lpddr4:1;
		unsigned reserved6_7:2;
		unsigned burst_mode:1;
		unsigned burstchop:1;
		unsigned en_2t_timing_mode:1;
		unsigned geardown_mode:1;
		unsigned data_bus_width:2;
		unsigned reserved14:1;
		unsigned dll_off_mode:1;
		unsigned burst_rdwr:4;
		unsigned active_logical_ranks:2;
		unsigned frequency_ratio:1;
		unsigned reserved_23:1;
		unsigned active_ranks:1;
		unsigned frequency_mode:1;
		unsigned device_config:1;
	} b32;
} ddrc_MSTR_t;

typedef union ddrc_INIT0 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned pre_cke_x1024:12;
		unsigned reserved12_15:4;
		unsigned post_cke_x1024:10;
		unsigned reserved26_29:4;
		unsigned skip_dram_init:2;
	} b32;
} ddrc_INIT0_t;

typedef union ddrc_INIT1 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned pre_ocd_x32:4;
		unsigned reserved4_15:12;
		unsigned dram_rstn_x1024:9;
		unsigned reserved25_31:7;
	} b32;
} ddrc_INIT1_t;

typedef union ddrc_INIT2 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned reserved0_7:8;
		unsigned idle_after_reset_x32:8;
		unsigned reserved16_31:16;
	} b32;
} ddrc_INIT2_t;

typedef union ddrc_INIT3 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned emr:16;
		unsigned mr:16;
	} b32;
} ddrc_INIT3_t;

typedef union ddrc_INIT4 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned emr3:16;
		unsigned emr2:16;
	} b32;
} ddrc_INIT4_t;

typedef union ddrc_INIT5 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned max_auto_init_x1024:10;
		unsigned reserved10_15:6;
		unsigned dev_zqinit_x32:8;
		unsigned reserved24_31:8;
	} b32;
} ddrc_INIT5_t;

typedef union ddrc_ADDRMAP0 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned addrmap_cs_bit0:5;
		unsigned reserved5_7:3;
		unsigned addrmap_cs_bit1:5;
		unsigned reserved13_15:3;
		unsigned addrmap_dch_bit0:5;
		unsigned reserved21_31:11;
	} b32;
} ddrc_ADDRMAP0_t;

typedef union ddrc_ADDRMAP1 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned addrmap_bank_b0:6;
		unsigned reserved6_7:2;
		unsigned addrmap_bank_b1:6;
		unsigned reserved14_15:2;
		unsigned addrmap_bank_b2:6;
		unsigned reserved22_31:10;
	} b32;
} ddrc_ADDRMAP1_t;


typedef union ddrc_ADDRMAP2 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned addrmap_col_b2:4;
		unsigned reserved4_7:4;
		unsigned addrmap_col_b3:5;
		unsigned reserved13_15:3;
		unsigned addrmap_col_b4:4;
		unsigned reserved20_23:4;
		unsigned addrmap_col_b5:4;
		unsigned reserved28_31:4;
	} b32;
} ddrc_ADDRMAP2_t;


typedef union ddrc_ADDRMAP3 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned addrmap_col_b6:5;
		unsigned reserved5_7:3;
		unsigned addrmap_col_b7:5;
		unsigned reserved13_15:3;
		unsigned addrmap_col_b8:5;
		unsigned reserved21_23:3;
		unsigned addrmap_col_b9:5;
		unsigned reserved29_31:3;
	} b32;
} ddrc_ADDRMAP3_t;


typedef union ddrc_ADDRMAP4 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned addrmap_col_b10:5;
		unsigned reserved5_7:3;
		unsigned addrmap_col_b11:5;
		unsigned reserved13_30:18;
		unsigned col_addr_shift:1;
	} b32;
} ddrc_ADDRMAP4_t;

typedef union ddrc_ADDRMAP5 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned addrmap_row_b0:4;
		unsigned reserved4_7:4;
		unsigned addrmap_row_b1:4;
		unsigned reserved12_15:4;
		unsigned addrmap_row_b2_b10:4;
		unsigned reserved20_23:4;
		unsigned addrmap_row_b11:4;
		unsigned reserved28_31:4;
	} b32;
} ddrc_ADDRMAP5_t;

typedef union ddrc_ADDRMAP6 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned addrmap_row_b12:4;
		unsigned reserved4_7:4;
		unsigned addrmap_row_b13:4;
		unsigned reserved12_15:4;
		unsigned addrmap_row_b14:4;
		unsigned reserved20_23:4;
		unsigned addrmap_row_b15:4;
		unsigned reserved28:4;
		unsigned lpddr32_3gb_6gb_12gb:3;
	} b32;
} ddrc_ADDRMAP6_t;

typedef union ddrc_RFSHCTL3 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned dis_auto_refresh:1;
		unsigned refresh_update_level:1;
		unsigned reserved2_3:2;
		unsigned refresh_mode:3;
		unsigned reserved7_15:9;
		unsigned rank_dis_refresh:16;
	} b32;
} ddrc_RFSHCTL3_t;

typedef union ddrc_RFSHTMG {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned t_rfc_min:10;
		unsigned reserved10_14:5;
		unsigned lpddr3_trefbw_en:1;
		unsigned t_rfc_nom_x1_x32:12;
		unsigned reserved28_30:3;
		unsigned t_rfc_nom_x1_sel:1;
	} b32;
} ddrc_RFSHTMG_t;

typedef union ddrc_ODTCFG {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned reserved0_1:2;
		unsigned rd_odt_delay:5;
		unsigned reserved7:1;
		unsigned rd_odt_hold:4;
		unsigned reserved12_15:4;
		unsigned wr_odt_delay:5;
		unsigned reserved21_23:3;
		unsigned wr_odt_hold:4;
		unsigned reserved28_31:4;
	} b32;
} ddrc_ODTCFG_t;

typedef union ddrc_DFITMG0 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned dfi_tphy_wrlat:6;
		unsigned reserved6_7:2;
		unsigned dfi_tphy_wrdata:6;
		unsigned reserved14:1;
		unsigned dfi_wrdata_use_dfi_phy_clk:1;
		unsigned dfi_t_rddata_en:7;
		unsigned dfi_rddata_use_dfi_phy_clk:1;
		unsigned dfi_t_ctrl_delay:5;
		unsigned reserved29_31:3;
	} b32;
} ddrc_DFITMG0_t;

typedef union ddrc_DFITMG1 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned dfi_t_dram_clk_enable:5;
		unsigned reserved5_7:3;
		unsigned dfi_t_dram_clk_disable:5;
		unsigned reserved13_15:3;
		unsigned dfi_t_wrdata_delay:5;
		unsigned reserved21_23:3;
		unsigned dfi_t_parin_lat:2;
		unsigned reserved26_27:2;
		unsigned dfi_t_cmd_lat:4;
	} b32;
} ddrc_DFITMG1_t;

typedef union ddrc_DFIUPD0 {
	/** raw register data */
	uint32_t d32;
	/** register bits */
	struct {
		unsigned dfi_t_ctrlup_min:10;
		unsigned reserved10_15:6;
		unsigned dfi_t_ctrlup_max:10;
		unsigned reserved26_28:3;
		unsigned ctrlupd_pre_srx:1;
		unsigned dis_auto_ctrlupd_srx:1;
		unsigned dis_auto_ctrlupd:1;
	} b32;
} ddrc_DFIUPD0_t;

struct ddrc_reg
{
	ddrc_MSTR_t MSTR;             /* 0x000 */

	ddrc_RFSHCTL3_t RFSHCTL3;     /* 0x060 */
	ddrc_RFSHTMG_t RFSHTMG;       /* 0x064 */

	ddrc_INIT0_t INIT0;           /* 0x0d0 */
	ddrc_INIT1_t INIT1;           /* 0x0d4 */
	ddrc_INIT2_t INIT2;           /* 0x0d8 */
	ddrc_INIT3_t INIT3;           /* 0x0dc */
	ddrc_INIT4_t INIT4;           /* 0x0e0 */
	ddrc_INIT5_t INIT5;           /* 0x0e4 */

	ddrc_TIMING0_t TIMING0;       /* 0x100 */
	ddrc_TIMING1_t TIMING1;       /* 0x104 */
	ddrc_TIMING2_t TIMING2;       /* 0x108 */
	ddrc_TIMING3_t TIMING3;       /* 0x10c */
	ddrc_TIMING4_t TIMING4;       /* 0x110 */
	ddrc_TIMING6_t TIMING6;       /* 0x118 */
	ddrc_TIMING7_t TIMING7;       /* 0x11C */
	ddrc_TIMING5_t TIMING5;       /* 0x114 */
	ddrc_TIMING8_t TIMING8;       /* 0x120 */
	ddrc_TIMING14_t TIMING14;     /* 0x138 */

	ddrc_DFITMG0_t DFITMG0;       /* 0x190 */
	ddrc_DFITMG1_t DFITMG1;       /* 0x194 */

	ddrc_DFIUPD0_t DFIUPD0;       /* 0x1a0 */

	ddrc_ADDRMAP0_t ADDRMAP0;     /* 0x200 */
	ddrc_ADDRMAP1_t ADDRMAP1;     /* 0x204 */
	ddrc_ADDRMAP2_t ADDRMAP2;     /* 0x208 */
	ddrc_ADDRMAP3_t ADDRMAP3;     /* 0x20c */
	ddrc_ADDRMAP4_t ADDRMAP4;     /* 0x210 */
	ddrc_ADDRMAP5_t ADDRMAP5;     /* 0x214 */
	ddrc_ADDRMAP6_t ADDRMAP6;     /* 0x218 */

	ddrc_ODTCFG_t ODTCFG;         /* 0x240 */

	ddrc_SWCTL_t SWCTL;           /* 0x320 */
	ddrc_SWCTLSTAIC_t SWCTLSTAIC; /* 0x328 */
};

#endif /* __DDRC_H__ */
