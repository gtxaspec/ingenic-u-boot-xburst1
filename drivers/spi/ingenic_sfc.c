// SPDX-License-Identifier: GPL-2.0+
/*
 * 君正PRJ SFC Flash驱动程序
 *
 * Copyright (C) 2024 Ingenic Semiconductor Co.,Ltd.
 *
 * 本文件为SFC硬件提供SPI-NOR框架适配。
 * SFC (串行Flash控制器) 是专用的SPI Flash控制器,
 * 支持 cmd+addr+data / cmd+data / cmd 传输格式。
 *
 * 1. 传统 SPI 框架 (使用 xfer)：
 * 应用层 → SPI Flash 层 → 通用 SPI 层 → SPI 控制器驱动
 *                                    ↑
 *                               使用 xfer()
 * 2. 现代 SPI-MEM 框架 (使用 exec_op)：
 * 应用层 → SPI Flash 层 → SPI-MEM 层 → SPI 控制器驱动
 *                                    ↑
 *                              使用 exec_op()
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <spi.h>
#include <spi-mem.h>
#include <asm/io.h>
// #define SFC_NOR_DEBUG
#include "ingenic_sfc.h"


/*
 * SPI-MEM操作执行 - SPI-NOR框架的主入口点
 */
static int sfc_exec_op(struct spi_slave *slave, const struct spi_mem_op *op)
{
	struct udevice *bus = slave->dev->parent;
	struct sfc_priv *sfc = dev_get_priv(bus);
	// struct dm_spi_slave_plat *plat = dev_get_parent_plat(slave->dev);
	int ret;

	sfc_debug("SFC: cmd=0x%02x addr=0x%08llx len=%u, sfc-mode=%u\n",
			   op->cmd.opcode, op->addr.val, op->data.nbytes, sfc->mode);

	if (!op->cmd.buswidth || op->cmd.buswidth > 4)
		return -EINVAL;

	if (op->addr.nbytes && (!op->addr.buswidth || op->addr.buswidth > 4))
		return -EINVAL;

	if (op->data.nbytes && (!op->data.buswidth || op->data.buswidth > 4))
		return -EINVAL;

	/* 执行硬件操作 */
	ret = sfc_hw_exec_op(sfc, op);
	if (ret)
	{
		printf("SFC操作失败: %d\n", ret);
		return ret;
	}

	return 0;
}

/*
 * SPI-MEM框架的SPI操作结构体
 */
static const struct spi_controller_mem_ops sfc_mem_ops = {
	.exec_op = sfc_exec_op,
};

/*
 * SPI从设备配置
 */
static int sfc_set_speed(struct udevice *bus, uint speed)
{
	struct sfc_priv *sfc = dev_get_priv(bus);

	sfc_debug("SFC: 设置速度为 %u Hz\n", speed);

	if (speed > sfc->max_freq)
	{
		printf("速度 %u 过高,限制为 %u\n",
			   speed, sfc->max_freq);
		speed = sfc->max_freq;
	}

	return sfc_hw_set_speed(sfc, speed);
}

static int sfc_set_mode(struct udevice *bus, uint mode)
{
	struct sfc_priv *sfc = dev_get_priv(bus);

	/* 保存模式供硬件配置使用 */
	sfc->mode = mode;

	/* 仅支持SPI模式0 (CPOL=0, CPHA=0) */
	if ((sfc->mode & (SPI_CPOL | SPI_CPHA)) != 0)
	{
		return -EINVAL;
	}

	return 0;
}

static int sfc_claim_bus(struct udevice *slave)
{
	/* SFC是专用控制器,无需总线仲裁 */
	return 0;
}

static int sfc_release_bus(struct udevice *slave)
{
	/* SFC是专用控制器,无需释放总线 */
	return 0;
}

/*
 * 片选信息查询函数 - 告诉SPI框架我们支持哪些片选
 */
static int sfc_cs_info(struct udevice *bus, uint cs, struct spi_cs_info *info)
{
	/* 仅支持片选0 */
	if (cs != SFC_SUPPORTED_CS)
	{
		return -ENODEV;
	}

	/* 片选0有效 */
	info->dev = NULL; /* 设备由设备树创建 */

	return 0;
}

/*
 * SPI操作结构体
 */
static const struct dm_spi_ops sfc_ops = {
	.claim_bus = sfc_claim_bus,
	.release_bus = sfc_release_bus,
	.set_speed = sfc_set_speed,
	.set_mode = sfc_set_mode,
	.cs_info = sfc_cs_info,
	.mem_ops = &sfc_mem_ops,
};

/*
 * 设备树解析和初始化
 */
static int sfc_of_to_plat(struct udevice *bus)
{
	struct sfc_priv *sfc = dev_get_priv(bus);
	int ret;

	/* 获取寄存器基地址 */
	sfc->base = dev_read_addr_ptr(bus);
	if (!sfc->base)
	{
		return -EINVAL;
	}

	/* 获取最大频率 */
	ret = dev_read_u32(bus, "spi-max-frequency", &sfc->max_freq);
	if (ret)
	{
		printf("未找到spi-max-frequency,使用默认值\n");
		sfc->max_freq = SFC_DEFAULT_FREQ;
	}

	/* 获取其他可选属性 */
	sfc->mode = 0;
	if (dev_read_bool(bus, "spi-cpol"))
		sfc->mode |= SPI_CPOL;
	if (dev_read_bool(bus, "spi-cpha"))
		sfc->mode |= SPI_CPHA;

	sfc_debug("SFC: base=0x%p max_freq=%u mode=0x%x\n",
			  sfc->base, sfc->max_freq, sfc->mode);

	return 0;
}

/*
 * 探测函数 - 初始化SFC控制器
 */
static int sfc_probe(struct udevice *bus)
{
	struct sfc_priv *sfc = dev_get_priv(bus);
	int ret;

	sfc->dev = bus;

	/* 初始化硬件 */
	ret = sfc_hw_init(sfc);
	if (ret)
	{
		printf("SFC硬件初始化失败: %d\n", ret);
		return ret;
	}

	return 0;
}

/*
 * 子设备绑定函数 - 绑定 SPI NOR Flash 子设备
 */
static int sfc_child_pre_probe(struct udevice *dev)
{
	struct spi_slave *slave = dev_get_parent_priv(dev);
	struct dm_spi_slave_plat *plat = dev_get_parent_plat(dev);
	u32 cs;
	int ret;

	/* 获取片选号 子设备reg属性(片选号) */
	ret = dev_read_u32(dev, "reg", &cs);
	if (ret)
	{
		return -EINVAL;
	}

	/* 检查片选号有效性 - SFC仅支持片选0 */
	if (cs != SFC_SUPPORTED_CS)
	{
		return -EINVAL;
	}

	/* 设置片选信息 */
	slave->dev = dev;
	plat->cs = cs;

	return 0;
}

/*
 * 驱动绑定
 */
static const struct udevice_id sfc_ids[] = {
	{.compatible = "ingenic,PRJ-sfc"},
	{}
};

U_BOOT_DRIVER(ingenic_sfc) = {
	.name = "ingenic_sfc",
	.id = UCLASS_SPI,
	.of_match = sfc_ids,
	.ops = &sfc_ops,
	.of_to_plat = sfc_of_to_plat,
	.probe = sfc_probe,
	.child_pre_probe = sfc_child_pre_probe,
	.priv_auto = sizeof(struct sfc_priv),
	.per_child_auto = sizeof(struct spi_slave),
	.per_child_plat_auto = sizeof(struct dm_spi_slave_plat),
};

/*
 * ================================================================
 * 硬件相关
 * ================================================================
 */
static int sfc_hw_init(struct sfc_priv *sfc)
{
	u32 i;
	sfc_reg_t reg = {0};

	reg.b32 = SFC_REG32(SFC_GLB0);
	reg.sfc_glb0.wp_en = 1;
	reg.sfc_glb0.tran_dir = 0;
	reg.sfc_glb0.op_mode = 0;
	reg.sfc_glb0.threshold = THRESHOLD;
	SFC_REG32(SFC_GLB0) = reg.b32;

	reg.b32 = SFC_REG32(SFC_DEV_CONF);
	reg.sfc_dev_conf.cmd_type = 0;
	reg.sfc_dev_conf.cpol = 0;
	reg.sfc_dev_conf.cpha = 0;
	reg.sfc_dev_conf.smp_delay = 0;
	reg.sfc_dev_conf.thold = 0;
	reg.sfc_dev_conf.tsetup = 0;
	reg.sfc_dev_conf.tsh = 0;
	reg.sfc_dev_conf.ce_dl = 1;
	reg.sfc_dev_conf.hold_dl = 1;
	reg.sfc_dev_conf.wp_dl = 1;
	SFC_REG32(SFC_DEV_CONF) = reg.b32;

	// sta_type need init by read function
	for (i = 0; i < 6; i++)
	{
		SFC_REG32(SFC_TRAN_CONF0(i)) &= ~(TRAN_CONF0_FMAT);
		SFC_REG32(SFC_TRAN_CONF1(i)) &= ~(TRAN_CONF1_TRAN_MODE_MSK);
	}

	SFC_REG32(SFC_SCR) = (CLR_END | CLR_TREQ |
						  CLR_RREQ | CLR_OVER | CLR_UNDER);
	SFC_REG32(SFC_INTC) |= (MASK_END | MASK_TREQ |
							MASK_RREQ | MASK_OVER | MASK_UNDR);
	SFC_REG32(SFC_CGE) = 0;

	SFC_REG32(SFC_TRIG) = TRIG_STOP;
	SFC_REG32(SFC_TRIG) = TRIG_FLUSH;
	SFC_REG32(SFC_TRAN_LEN) = 0;

	sfc_debug("SFC: 硬件初始化\n");
	return 0;
}

/*
 * 使用SFC硬件执行SPI存储器操作
 * cmd+addr+data / cmd+data / cmd 传输格式
 */
static int sfc_hw_exec_op(struct sfc_priv *sfc, const struct spi_mem_op *op)
{
	u32 timeout = 0xffff;
	u32 dummy = 0;
	u32 data_dir = 1;
	sfc_reg_t reg = {0};

	/* 参数有效性检查 */
	if (!sfc || !op)
	{
		printf("SFC错误: exec_op收到无效参数\n");
		return -EINVAL;
	}

	sfc_debug("SFC: exec_op 执行中...\n");
	sfc_debug("  命令: 0x%02x (总线宽度=%u)\n", op->cmd.opcode, op->cmd.buswidth);

	if (op->addr.nbytes)
	{
		sfc_debug("  地址: 0x%08llx (%u字节, 总线宽度=%u)\n", op->addr.val, op->addr.nbytes, op->addr.buswidth);
	}

	if (op->dummy.nbytes)
	{
		sfc_debug("  虚拟周期: %u周期 (总线宽度=%u)\n", op->dummy.nbytes * 8 / op->dummy.buswidth, op->dummy.buswidth);
		dummy = op->dummy.nbytes * 8 / op->dummy.buswidth;
		if (dummy > 32)
		{
			printf("dummy bits > 32, not support\n");
			return -1;
		}
	}

	if (op->data.nbytes)
	{
		sfc_debug("  数据: %u字节 (总线宽度=%u, 方向=%s)\n", op->data.nbytes, op->data.buswidth, op->data.dir == SPI_MEM_DATA_IN ? "读" : "写");
		data_dir = (op->data.dir == SPI_MEM_DATA_IN ? 0 : 1); // read:0  write:1
	}

	SFC_REG32(SFC_TRAN_LEN) = op->data.nbytes; /* SFC_TRAN_LEN 是字节数,不是words */

	if ((SFC_REG32(SFC_SR) & BUSY_MSK))
	{
		sfc_debug("Now sfc busy,and stop it\n");
		SFC_REG32(SFC_TRIG) = TRIG_STOP;
		while (!((SFC_REG32(SFC_SR)) & SR_END))
		{
			if (timeout == 0)
				break;
			timeout--;
		}
	}
	SFC_REG32(SFC_SCR) = (CLR_END | CLR_TREQ |
						  CLR_RREQ | CLR_OVER | CLR_UNDER);

	reg.b32 = SFC_REG32(SFC_GLB0);
	reg.sfc_glb0.phase_num = 1;
	reg.sfc_glb0.tran_dir = data_dir;
	SFC_REG32(SFC_GLB0) = reg.b32;

	reg.b32 = SFC_REG32(SFC_TRAN_CONF0(0));
	reg.sfc_tran_cfg0.addr_width = op->addr.nbytes;
	reg.sfc_tran_cfg0.cmd_en = 1;
	reg.sfc_tran_cfg0.data_en = (op->data.nbytes > 0 ? 1 : 0);
	reg.sfc_tran_cfg0.cmd = op->cmd.opcode;
	reg.sfc_tran_cfg0.dmy_bits = dummy;
	SFC_REG32(SFC_TRAN_CONF0(0)) = reg.b32;

	reg.b32 = SFC_REG32(SFC_TRAN_CONF1(0));
	reg.sfc_tran_cfg1.tran_md = sfc_set_tran_mode(op);
	SFC_REG32(SFC_TRAN_CONF1(0)) = reg.b32;

	SFC_REG32(SFC_DEV_ADDR(0)) = op->addr.val;
	SFC_REG32(SFC_DEV_ADDR_PLUS(0)) = 0; // 一般都用不到,配置为0即可
	SFC_REG32(SFC_TRIG) = (TRIG_START);
	sfc_debug("cmd = 0x%02x, addr = 0x%08llx, len = 0x%08x\n", op->cmd.opcode, op->addr.val, op->data.nbytes);


	if (op->data.dir == SPI_MEM_DATA_OUT)
		sfc_write_data(op->data.buf.out, op->data.nbytes);
	else if (op->data.dir == SPI_MEM_DATA_IN)
		sfc_read_data((void *)op->data.buf.in, op->data.nbytes);
	// else // 只有cmd/cmd+addr传输
	// 	;

	// wait sfc end
	timeout = 0xffff;
	while (!((SFC_REG32(SFC_SR)) & SR_END))
	{
		if (timeout == 0)
		{
			printf("SFC: wait end timeout\n");
			break;
		}
		timeout--;
	}

	if ((SFC_REG32(SFC_SR)) & SR_END)
		SFC_REG32(SFC_SCR) = (CLR_END);

	return 0;
}

static int sfc_hw_set_speed(struct sfc_priv *sfc, u32 speed)
{
	_clk_set_rate(SFC0, speed);
	sfc_debug("SFC: set speed %u Hz\n", speed);
	return 0;
}
