#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/arch/jz_cache.h>
#include <common.h>
#include <config.h>
#include <generated/autoconf.h>
#include <image.h>
#include <linux/delay.h>
#include <spl.h>

// #define SFC_NOR_DEBUG
#include <asm/arch/spl_sfc.h>

void sfc_init(void)
{
	u32 i;
	sfc_reg_t reg = {0};
#ifdef CONFIG_SFC_NOR_INIT_RATE
	_clk_set_rate(SFC0, CONFIG_SFC_NOR_INIT_RATE);
#elif CONFIG_SFC_NAND_INIT_RATE
	_clk_set_rate(SFC0, CONFIG_SFC_NAND_INIT_RATE);
#else
	/* default: sfc rate 25MHz */
	_clk_set_rate(SFC0, 25000000);
#endif

	reg.b32 = SFC_REG32(SFC_GLB0);
	reg.sfc_glb0.tran_dir = 0;
	reg.sfc_glb0.op_mode = 0;
	reg.sfc_glb0.wp_en = 1;
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

	/* bootrom not stop, here stop it to avoid FIFO residual error */
	SFC_REG32(SFC_TRIG) = TRIG_STOP;
	SFC_REG32(SFC_TRIG) = TRIG_FLUSH;
	SFC_REG32(SFC_TRAN_LEN) = 0;

	sfc_dump_reg();
}

static int sfc_wait_end(void)
{
	u32 timeout = 0xffff;
	while (!((SFC_REG32(SFC_SR)) & SR_END))
	{
		if (timeout == 0)
		{
			printf("SFC: wait end timeout\n");
			return -1;
		}
		timeout--;
	}
	return 0;
}

static void sfc_set_reg(u32 cmd, u32 addr,
						u32 addr_len, u32 dummy_bits, u32 data_en, u32 data_len, u32 dir)
{
	sfc_reg_t reg = {0};
	u32 addr_plus = 0;

	SFC_REG32(SFC_TRAN_LEN) = data_len;

	if ((SFC_REG32(SFC_SR) & BUSY_MSK))
	{
		printf("Now sfc busy,and stop it\n");
		SFC_REG32(SFC_TRIG) = TRIG_STOP;
		sfc_wait_end();
	}
	SFC_REG32(SFC_SCR) = (CLR_END | CLR_TREQ |
						  CLR_RREQ | CLR_OVER | CLR_UNDER);

	reg.b32 = SFC_REG32(SFC_GLB0);
	reg.sfc_glb0.phase_num = 0x1;
	reg.sfc_glb0.tran_dir = dir; // SPL 仅读方向
	SFC_REG32(SFC_GLB0) = reg.b32;

	reg.b32 = SFC_REG32(SFC_TRAN_CONF0(0));
	reg.sfc_tran_cfg0.addr_width = addr_len;
	reg.sfc_tran_cfg0.dmy_bits = dummy_bits;
	reg.sfc_tran_cfg0.cmd = cmd;
	reg.sfc_tran_cfg0.cmd_en = 1;
	reg.sfc_tran_cfg0.phase_format = 0;
	reg.sfc_tran_cfg0.data_en = (data_en) ? 1 : 0;
	SFC_REG32(SFC_TRAN_CONF0(0)) = reg.b32;

	SFC_REG32(SFC_DEV_ADDR(0)) = addr;
	SFC_REG32(SFC_DEV_ADDR_PLUS(0)) = addr_plus; // 一般是0
	SFC_REG32(SFC_TRIG) = (TRIG_START);
}

static int sfc_read_data(u8 *data, u32 len)
{
	u32 i = 0, timeout = 0xffff;
	u32 tmp_buf[THRESHOLD] = {0};
	u32 fifo_num = 0, data_len = 0, fifo_len = THRESHOLD * 4;
	u32 read_len = len;

	while (len > 0)
	{
		// 等待传输请求信号 RECE_REQ
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
		data = data + data_len;
		len -= data_len;
	}

	return read_len - len; //返回已读字节数
}

int sfc_execution(u32 cmd, u32 addr, u32 addr_len, u32 dummy_bits, u32 data_len, u8 *data, u32 dir)
{
	int ret = 0;
	u32 data_en = (data_len) ? 1 : 0;
	sfc_set_reg(cmd, addr, addr_len, dummy_bits, data_en, data_len, 0);

	if (data_en)
	{
		if (0 == dir)
		{
			ret = sfc_read_data(data, data_len);
			if (ret != data_len)
			{
				printf("SFC: sfc_execution data len error, expect %d, actual %d\n", data_len, ret);
				return -1;
			}
		}
	}
	ret = sfc_wait_end();
	if (ret)
	{
		printf("SFC: sfc_execution wait end timeout\n");
	}
	return ret;
}