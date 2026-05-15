/*
 * Inno DDR PHY common data structure.
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

#ifndef __DDRP_INNO_H__
#define __DDRP_INNO_H__

typedef union ddrp_0x000 {
	uint32_t d32;
	struct {
		unsigned int soft_reset:1;
		unsigned int soft_reset0:1;
		unsigned int soft_reset1:1;
		unsigned int reserved3:1;
		unsigned int mem_select_t:3;
		unsigned int reg_burst_sel:1;
		unsigned int reg_channel_en:9;
		unsigned int reg_dq_invalid_value:1;
		unsigned int reserved18_31:14;
	}b32;
}ddrp_0x000_t;

typedef union ddrp_0x008 {
	uint32_t d32;
	struct {
		unsigned int AL_FRE_OP3:6;
		unsigned int reserved6_7:2;
		unsigned int AL_FRE_OP2:6;
		unsigned int reserved14_15:2;
		unsigned int AL_FRE_OP1:6;
		unsigned int reserved22_23:2;
		unsigned int AL_FRE_OP0:6;
		unsigned int reserved30_31:2;
	}b32;
}ddrp_0x008_t;

typedef union ddrp_0x00c {
	uint32_t d32;
	struct {
		unsigned int CL_FRE_OP3:6;
		unsigned int reserved6_7:2;
		unsigned int CL_FRE_OP2:6;
		unsigned int reserved14_15:2;
		unsigned int CL_FRE_OP1:6;
		unsigned int reserved22_23:2;
		unsigned int CL_FRE_OP0:6;
		unsigned int reserved30_31:2;
	}b32;
}ddrp_0x00c_t;

typedef union ddrp_0x010 {
	uint32_t d32;
	struct {
		unsigned int CWL_FRE_OP3:6;
		unsigned int reserved6_7:2;
		unsigned int CWL_FRE_OP2:6;
		unsigned int reserved14_15:2;
		unsigned int CWL_FRE_OP1:6;
		unsigned int reserved22_23:2;
		unsigned int CWL_FRE_OP0:6;
		unsigned int reserved30_31:2;
	}b32;
}ddrp_0x010_t;

typedef union ddrp_0x0b8 {
	uint32_t d32;
	struct {
		unsigned int reg_phy_refresh_en:1;
		unsigned int reg_phy_max_refi_enable:1;
		unsigned int reserved2_3:2;
		unsigned int reg_max_refi_cnt:4;
		unsigned int reg_phy_trfc:10;
		unsigned int reg_phy_trefi:24;
	}b32;
}ddrp_0x0b8_t;


// struct ddrp_reg {
// 	ddrp_rst_t            rst;
// 	ddrp_mem_cfg_t        memcfg;
// 	ddrp_dq_width_t       dq_width;
// 	uint32_t              cl;
// 	uint32_t              cwl;
// 	ddrp_pll_fbdiv_t      pll_fbdiv;
// 	ddrp_ctrl_t           ctrl;
// 	ddrp_pdiv_t           pdiv;
// 	ddrp_lock_t           lock;
// 	ddrp_trainning_ctrl_t trainning_ctrl;
// 	ddrp_done_t           done;
// 	ddrp_delay_al_t       delay_al;
// 	ddrp_delay_ah_t       delay_ah;
// 	ddrp_bypass_al_t      bypass_al;
// 	ddrp_bypass_ah_t      bypass_ah;
// 	ddrp_wl_mode1_t       wl_mode1;
// 	ddrp_wl_mode2_t       wl_mode2;
// 	ddrp_wl_done_t        wl_done;
// 	ddrp_init_comp_t      init_comp;
// };

#endif /* __DDRP_INNO_H__ */
