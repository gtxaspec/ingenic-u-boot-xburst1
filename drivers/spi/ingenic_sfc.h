/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * SFC NOR Flash驱动头文件
 * 
 * Copyright (C) 2024 Ingenic Semiconductor Co.,Ltd.
 */

#ifndef _SPI_NOR_SFC_H
#define _SPI_NOR_SFC_H

#include <linux/types.h>
#include <asm/arch/spl_sfc.h>
#include <asm/arch/clk.h>

#define SFC_DEFAULT_FREQ		24000000	/* 24MHz */
#define SFC_MAX_FREQ			50000000	/* 50MHz */

#define SFC_MAX_CS_NUM			1		/* 仅支持1个片选 预留是否可以兼容做SFC1 */
#define SFC_SUPPORTED_CS		0		/* 仅支持CS0 */

/* SFC控制器私有数据结构 */
struct sfc_priv
{
	struct udevice *dev;
	void __iomem *base;
	u32 max_freq;
	u32 mode;
};

/* 硬件抽象的SFC操作结构体 */
struct sfc_op {
	/* 命令阶段 */
	u8 cmd;
	u8 cmd_buswidth;

	/* 地址阶段 */
	u64 addr;
	u8 addr_nbytes;
	u8 addr_buswidth;

	/* 虚拟周期阶段 */
	u8 dummy_cycles;
	u8 dummy_buswidth;

	/* 数据阶段 */
	void *data_buf;
	u32 data_nbytes;
	u8 data_buswidth;
	bool data_write;		/* true = 写, false = 读 */
};


static int sfc_exec_op(struct spi_slave *slave, const struct spi_mem_op *op);
static int sfc_set_speed(struct udevice *bus, uint speed);
static int sfc_set_mode(struct udevice *bus, uint mode);
static int sfc_claim_bus(struct udevice *slave);
static int sfc_release_bus(struct udevice *slave);
static int sfc_cs_info(struct udevice *bus, uint cs, struct spi_cs_info *info);
static int sfc_of_to_plat(struct udevice *bus);
static int sfc_probe(struct udevice *bus);
static int sfc_child_pre_probe(struct udevice *dev);
/* 硬件操作函数前向声明 */
static int sfc_hw_init(struct sfc_priv *sfc);
static int sfc_hw_exec_op(struct sfc_priv *sfc, const struct spi_mem_op *op);
static int sfc_hw_set_speed(struct sfc_priv *sfc, u32 speed);

static u8 sfc_set_tran_mode(const struct spi_mem_op *op);
static int sfc_write_data(const void *data, u32 len);
static int sfc_read_data(void *data, u32 len);


static u8 sfc_set_tran_mode(const struct spi_mem_op *op)
{
	u8 tran_md = TM_STD_SPI; /* 默认标准模式 */
	u8 cmd_width, addr_width, data_width;

	if (!op)
	{
		sfc_debug("SFC: tran_mode参数无效,使用标准模式\n");
		return TM_STD_SPI;
	}

	cmd_width = op->cmd.buswidth;
	addr_width = op->addr.buswidth;
	data_width = op->data.buswidth;

	if (cmd_width == 1 && addr_width == 1 && data_width == 1)
		tran_md = TM_STD_SPI; /* 0000: 标准 SPI（默认） - 1-1-1 */
	else if (cmd_width == 1 && addr_width == 1 && data_width == 2)
		tran_md = TM_DI_DO_SPI; /* 0001: Dual Input/Dual Output SPI - 1-1-2 */
	else if (cmd_width == 1 && addr_width == 2 && data_width == 2)
		tran_md = TM_DIO_SPI; /* 0010: Dual I/O SPI - 1-2-2 */
	else if (cmd_width == 2 && addr_width == 2 && data_width == 2)
		tran_md = TM_FULL_DIO_SPI; /* 0011: Full Dual I/O SPI - 2-2-2 */
	else if (cmd_width == 1 && addr_width == 1 && data_width == 4)
		tran_md = TM_QI_QO_SPI; /* 0101: Quad Input/Quad Output SPI - 1-1-4 */
	else if (cmd_width == 1 && addr_width == 4 && data_width == 4)
		tran_md = TM_QIO_SPI; /* 0110: Quad I/O SPI - 1-4-4 */
	else if (cmd_width == 4 && addr_width == 4 && data_width == 4)
		tran_md = TM_FULL_QIO_SPI; /* 0111: Full Quad I/O SPI - 4-4-4 */

	sfc_debug("SFC_TRAN_CONF1[7:4] 传输模式: %u\n", tran_md);

	return tran_md;
}

static int sfc_write_data(const void *data, u32 len)
{
	u32 i = 0, timeout = 0xffff;
	u32 tmp_buf[THRESHOLD] = {0};
	u32 fifo_num = 0, data_len = 0, fifo_len = THRESHOLD * 4;

	while (len > 0)
	{
		if (!(SFC_REG32(SFC_SR) & TRAN_REQ))
		{
			if (timeout == 0)
			{
				printf("SFC: wait TRAN_REQ timeout\n");
				break;
			}
			timeout--;
			continue;
		}
		timeout = 0xffff;

		SFC_REG32(SFC_SCR) = CLR_TREQ;
		data_len = (len >= fifo_len) ? fifo_len : len;
		fifo_num = (data_len + 3) / 4;

		memcpy(tmp_buf, data, data_len);
		for (i = 0; i < fifo_num; i++)
		{
			SFC_REG32(SFC_RM_DR) = tmp_buf[i];
		}
		data = (u8 *)data + data_len;
		len -= data_len;
	}

	return len;
}

static int sfc_read_data(void *data, u32 len)
{
	u32 i = 0, timeout = 0xffff;
	u32 tmp_buf[THRESHOLD] = {0};
	u32 fifo_num = 0, data_len = 0, fifo_len = THRESHOLD * 4;

	while (len > 0)
	{
		if (!(SFC_REG32(SFC_SR) & RECE_REQ))
		{
			if (timeout == 0)
			{
				printf("SFC: wait RECE_REQ timeout\n");
				break;
			}
			timeout--;
			continue;
		}
		timeout = 0xffff;

		SFC_REG32(SFC_SCR) = (CLR_RREQ);
		data_len = (len >= fifo_len) ? fifo_len : len;
		fifo_num = (data_len + 3) / 4;

		for (i = 0; i < fifo_num; i++)
		{
			tmp_buf[i] = SFC_REG32(SFC_RM_DR);
		}
		memcpy(data, tmp_buf, data_len);
		data = (u8 *)data + data_len;
		len -= data_len;
	}

	return len;
}

#endif /* _SPI_NOR_SFC_H */

