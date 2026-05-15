// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __INGENIC_EFUSE_H__
#define __INGENIC_EFUSE_H__

#define EFUSE_CTRL                   0x0
#define EFUSE_CFG                    0x4
#define EFUSE_STATE                  0x8
#define EFUSE_DATA(n)                (0xC + (n) * 4)


// efuse ctrl bits
#define EFUSE_CTRL_ADDR              (21)
#define EFUSE_CTRL_ADDR_MASK         (0x3f)
#define EFUSE_CTRL_LEN               (16)
#define EFUSE_CTRL_LEN_MASK          (0x1f)
#define EFUSE_CTRL_PGEN              (1 << 15)
#define EFUSE_CTRL_WREN              (1 << 1)
#define EFUSE_CTRL_RDEN              (1 << 0)


// efuse cfg bits
#define EFUSE_CFG_INT_EN             (1 << 31)
#define EFUSE_CFG_RD_ADJ             (20)
#define EFUSE_CFG_RD_ADJ_MASK        (0xf)
#define EFUSE_CFG_RD_STROBE          (16)
#define EFUSE_CFG_RD_STROBE_MASK     (0xf)
#define EFUSE_CFG_WR_ADJ             (12)
#define EFUSE_CFG_WR_ADJ_MASK        (0xf)
#define EFUSE_CFG_WR_STROBE          (0)
#define EFUSE_CFG_WR_STROBE_MASK     (0x7ff)


// efuse state bits
#define EFUSE_STA_NKU_PRT            (1 << 21)
#define EFUSE_STA_USERKEY_PRT        (1 << 20)
#define EFUSE_STA_CHIPKEY_PRT        (1 << 19)
#define EFUSE_STA_HIDEBLK_PRT        (1 << 18)
#define EFUSE_STA_SOCINFO_PRT        (1 << 17)
#define EFUSE_STA_TRIM3_PRT          (1 << 16)
#define EFUSE_STA_TRIM2_PRT          (1 << 15)
#define EFUSE_STA_TRIM1_PRT          (1 << 14)
#define EFUSE_STA_TRIM0_PRT          (1 << 13)
#define EFUSE_STA_CUT_PRT            (1 << 12)
#define EFUSE_STA_CHIPID_PRT         (1 << 11)
#define EFUSE_STA_SCBT_PRT           (1 << 10)
#define EFUSE_STA_DIS_JTAG           (1 << 9)
#define EFUSE_STA_SCBT_EN            (1 << 8)
#define EFUSE_STA_WR_DONE            (1 << 1)
#define EFUSE_STA_RD_DONE            (1 << 0)


#define CHIPID_OFFSET_ADDR            (0x0)
#define CUT_OFFSET_ADDR               (0x10)
#define TRIM0_OFFSET_ADDR             (0x1b)
#define TRIM1_OFFSET_ADDR             (0x1f)
#define TRIM2_OFFSET_ADDR             (0x23)
#define TRIM3_OFFSET_ADDR             (0x27)
#define SOCINFO_OFFSET_ADDR           (0x2b)
#define HIDEBLK_OFFSET_ADDR           (0x2d)
#define PRT_OFFSET_ADDR               (0x2e)
#define CHIPKEY_OFFSET_ADDR           (0x30)
#define USERKEY_OFFSET_ADDR           (0x50)
#define NKU_OFFSET_ADDR               (0x70)


#define CHIPID_BIT_NUM              (128)
#define CUT_BIT_NUM                 (88)
#define TRIM0_BIT_NUM               (32)
#define TRIM1_BIT_NUM               (32)
#define TRIM2_BIT_NUM               (32)
#define TRIM3_BIT_NUM               (32)
#define SOCINFO_BIT_NUM             (16)
#define HIDEBLK_BIT_NUM             (8)
#define PRT_BIT_NUM                 (16)
#define CHIPKEY_BIT_NUM             (256)
#define USERKEY_BIT_NUM             (256)
#define NKU_BIT_NUM                 (128)


enum segment_id {
	CHIPID = 0,
	CUTID,
	TRIM0,
	TRIM1,
	TRIM2,
	TRIM3,
	SOCINFO,
	HIDEBLK,
	PRT,
	CHIPKEY,
	USERKEY,
	NKU
};


struct seg_info {
	char seg_name[32];
	uint32_t seg_id;
	uint32_t offset_address;
	uint32_t bit_num;
	uint32_t prt_bit;
};

static struct seg_info seg_info_array[] = {
	[0] = {
		.seg_name = "CHIPID",
		.seg_id = CHIPID,
		.offset_address = CHIPID_OFFSET_ADDR,
		.bit_num = CHIPID_BIT_NUM,
		.prt_bit = EFUSE_STA_CHIPID_PRT,
	},
	[1] = {
		.seg_name = "CUTID",
		.seg_id = CUTID,
		.offset_address = CUT_OFFSET_ADDR,
		.bit_num = CUT_BIT_NUM,
		.prt_bit = EFUSE_STA_CUT_PRT,
	},
	[2] = {
		.seg_name = "TRIM0",
		.seg_id = TRIM0,
		.offset_address = TRIM0_OFFSET_ADDR,
		.bit_num = TRIM0_BIT_NUM,
		.prt_bit = EFUSE_STA_TRIM0_PRT,
	},
	[3] = {
		.seg_name = "TRIM1",
		.seg_id = TRIM1,
		.offset_address = TRIM1_OFFSET_ADDR,
		.bit_num = TRIM1_BIT_NUM,
		.prt_bit = EFUSE_STA_TRIM1_PRT,
	},
	[4] = {
		.seg_name = "TRIM2",
		.seg_id = TRIM2,
		.offset_address = TRIM2_OFFSET_ADDR,
		.bit_num = TRIM2_BIT_NUM,
		.prt_bit = EFUSE_STA_TRIM2_PRT,
	},
	[5] = {
		.seg_name = "TRIM3",
		.seg_id = TRIM3,
		.offset_address = TRIM3_OFFSET_ADDR,
		.bit_num = TRIM3_BIT_NUM,
		.prt_bit = EFUSE_STA_TRIM3_PRT,
	},
	[6] = {
		.seg_name = "SOCINFO",
		.seg_id = SOCINFO,
		.offset_address = SOCINFO_OFFSET_ADDR,
		.bit_num = SOCINFO_BIT_NUM,
		.prt_bit = EFUSE_STA_SOCINFO_PRT,
	},
	[7] = {
		.seg_name = "HIDEBLK",
		.seg_id = HIDEBLK,
		.offset_address = HIDEBLK_OFFSET_ADDR,
		.bit_num = HIDEBLK_BIT_NUM,
		.prt_bit = EFUSE_STA_HIDEBLK_PRT,
	},
	[8] = {
		.seg_name = "PRT",
		.seg_id = PRT,
		.offset_address = PRT_OFFSET_ADDR,
		.bit_num = PRT_BIT_NUM,
		.prt_bit = 0xffffffff,
	},
	[9] = {
		.seg_name = "CHIPKEY",
		.seg_id = CHIPKEY,
		.offset_address = CHIPKEY_OFFSET_ADDR,
		.bit_num = CHIPKEY_BIT_NUM,
		.prt_bit = EFUSE_STA_CHIPKEY_PRT,
	},
	[10] = {
		.seg_name = "USERKEY",
		.seg_id = USERKEY,
		.offset_address = USERKEY_OFFSET_ADDR,
		.bit_num = USERKEY_BIT_NUM,
		.prt_bit = EFUSE_STA_USERKEY_PRT,
	},
	[11] = {
		.seg_name = "NKU",
		.seg_id = NKU,
		.offset_address = NKU_OFFSET_ADDR,
		.bit_num = NKU_BIT_NUM,
		.prt_bit = EFUSE_STA_NKU_PRT,
	},
};

void jz_efuse_id_read(int is_chip_id, uint8_t *buf);

#endif
