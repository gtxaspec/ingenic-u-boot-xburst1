/*
 * efuse DDR参数映射读取 v3
 *
 * efuse寄存器位分布:
 *   reg0 [31:0]:  CMD0-CMD5(bit0-29), CMD6[1:0](bit30-31)
 *   reg1 [31:0]:  CMD6[4:2](bit0-2), CMD7-CMD11(bit3-27), CMD12[3:0](bit28-31)
 *   reg2 [31:0]:  CMD12[4](bit0), CMD13,WEB,CASB,RASB,BA0,BA1[0](bit1-30,31)
 *   reg3 [31:0]:  BA1[4:1],BA2,L_DM,L_DQ0-3(bit0-28), L_DQ4[2:0](bit29-31)
 *   reg4 [31:0]:  L_DQ4[4:3],L_DQ5-7,H_DM,H_DQ0-3(bit0-29), H_DQ4[1:0](bit30-31)
 *   reg5 [31:0]:  H_DQ4[4:2],H_DQ5-7(bit0-22), 保留(bit23-29), VERSION(bit30-31)
 *   reg6 [31:0]:  CRC32
 */

#include <asm/ddr_innophy.h>
#include <config.h>
#include <common.h>
#include <ddr/ddrc.h>

// #define CONFIG_DWC_DEBUG 1
#include "ddr_debug.h"

#define EFUSE_DDRPMAP_DBG(fmt, args...) dwc_debug(fmt, ##args)

#define EFUSE_REG_BASE           (0xb3540260)
#define EFUSE_REG_COUNT          (7)
#define CRC_DATA_LEN             (28)
#define DDRP_PARAM_COUNT         (38)
#define DDRP_CMD_COUNT           (20)
#define DDRP_DQ_COUNT            (18)
#define JZ_MAGIC                 (0x4A5A4E42)

#define REG32(addr)             (*(volatile u32 *)(addr))
#define DDRP_DQ_BYTE_WRAP       ((efuse_regs[5] >> 30) & 0x1)
#define EFUSE_DDRPMAP_EN        ((efuse_regs[5] >> 31) & 0x1)

#define EFUSE_REG_SCBOOT_BU     ((REG32(0xb354020C) >> 16) & (1 << 5))

/* 从efuse寄存器中提取5位参数，支持跨寄存器边界 */
static unsigned char get_param(u32 *efuse_regs, int bit_start)
{
	int reg_idx;
	int bit_offset;
	u32 low;
	u32 high;
	u32 mask;

	reg_idx = bit_start / 32;
	bit_offset = bit_start % 32;

	if (bit_offset + 5 > 32)
	{
		low = efuse_regs[reg_idx] >> bit_offset; // 获取低位
		mask = (1 << (5 + bit_offset - 32)) - 1;
		high = efuse_regs[reg_idx + 1] & mask;   // 获取高位
		return (high << (32 - bit_offset)) | low;
	}
	return (efuse_regs[reg_idx] >> bit_offset) & 0x1f;
}

/* 读取efuse DDR参数映射 */
/* 返回38字节: CMD0-13,WEB,CASB,RASB,BA0-2(20个) + L_DM,L_DQ0-7,H_DM,H_DQ0-7(18个) */
const unsigned char *efuse_ddrpmap_get(void)
{
	static unsigned char result[DDRP_PARAM_COUNT];
	u32 efuse_regs[EFUSE_REG_COUNT];
	u32 calculated_crc, stored_crc;
	int i;

	/* 判断是否打开SCBOOT 备份段，如果打开直接退出 */
	if(EFUSE_REG_SCBOOT_BU)
		return NULL;

	/* 先读取CRC，如果CRC为空，则直接退出 */
	stored_crc = REG32(EFUSE_REG_BASE + 6 * 4);
	EFUSE_DDRPMAP_DBG("reg CRC: 0x%x\n", stored_crc);
	if (!stored_crc)
		return NULL;

	for (i = 0; i < EFUSE_REG_COUNT - 1; i++)
	{
		efuse_regs[i] = REG32(EFUSE_REG_BASE + i * 4);
		EFUSE_DDRPMAP_DBG("reg[%d]: 0x%x\n", i, efuse_regs[i]);
	}

	/**
	 * 注意，使能位 = 1bit
	 * 0：使用eFuse map方案
	 * 1：不使用eFuse map方案 
	 */
	if (EFUSE_DDRPMAP_EN)
		return NULL;

	efuse_regs[6] = JZ_MAGIC;

	calculated_crc = crc32(0, (u8 *)efuse_regs, CRC_DATA_LEN);

	EFUSE_DDRPMAP_DBG("stored_crc: 0x%x, calculated_crc: 0x%x, dq_byte_wrap: %x\n",
					  stored_crc, calculated_crc, DDRP_DQ_BYTE_WRAP);

	/* CRC校验不一致，则退出 */
	if (calculated_crc != stored_crc)
		return NULL;

	/** 
	 * efuse_regs[5] bit[30]
	 *   0: L/H DQ BYTE不交换
	 *   1: L/H DQ BYTE交换
	 */
	if(DDRP_DQ_BYTE_WRAP)
	{
		/* 配置DQ Byte Wrap */
		SET_INNOPHY_REG(reg_byte0_wrap_sel, 1);
		SET_INNOPHY_REG(reg_byte1_wrap_sel, 0);
		PRINT_DDRP(reg_byte0_wrap_sel);
		PRINT_DDRP(reg_byte1_wrap_sel);
	}

	for (i = 0; i < DDRP_CMD_COUNT; i++)
		result[i] = get_param(efuse_regs, i * 5);

	for (i = 0; i < DDRP_DQ_COUNT; i++)
		result[DDRP_CMD_COUNT + i] = get_param(efuse_regs, 100 + i * 5);

	return result;
}
