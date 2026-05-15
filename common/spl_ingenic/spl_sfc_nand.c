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

int sfc_execution(int cmd, int addr, int addr_len, int dummy, int data_len, void *data, int flag);
void sfc_init(void);

#define SPINAND_IS_BUSY (1 << 0)

static int sfc_nand_get_feature(u8 *status)
{
	int ret = 0;

	ret = sfc_execution(0x0f, 0xc0, 1, 0, 1, status, 0);
	if(ret)
	{
		printf("SFC: sfc_nand_get_feature fail\n");
	}

	return ret;
}

int sfc_nand_read_to_cache(u32 page_addr)
{
	u32 timeout = 0xffff;
	u32 addr_len = 3;
	u8 status = 0;
	int ret = 0;

	ret = sfc_execution(0x13, page_addr, addr_len, 0, 0, NULL, 0);
	if (ret)
	{
		printf("SFC: sfc_nand_read_to_cache fail\n");
	}

retry_get_feature:
	timeout--;
	if(timeout == 0)
		goto get_feature_timeout;
	ret = sfc_nand_get_feature(&status);
	if (!(status & SPINAND_IS_BUSY))
		return 0;
	else
		goto retry_get_feature;

get_feature_timeout:
	printf("SFC: sfc_nand_get_feature timeout\n");
	sfc_execution(0x0f, 0xc0, 0, 0, 0, NULL, 0);

	return 0;
}

static int sfc_nand_page_read(u8 *data, u32 page_offset, u32 data_len)
{
	u32 addr_len = 2;
	u32 dummy_bits = 8;
	int ret = 0;

	ret = sfc_execution(0x03, page_offset, addr_len, dummy_bits, data_len, data, 0);
	if (ret)
	{
		printf("SFC: sfc_nand_page_read fail\n");
	}

	return ret;
}

static u32 sfc_nand_load(u32 addr, u32 len, u8 *data)
{
	u32 page_size = 2048;
	u32 data_len = 0, read_len = 0;
	u32 page_addr = addr >> 11;		// nand 页地址
	u32 page_offset = addr & 0x7ff; // nand 页内偏移
	int ret = 0;

	while(len - read_len)
	{
		data_len = (len > page_size) ? page_size : len;
		data_len -= page_offset;

		sfc_nand_read_to_cache(page_addr);
		ret = sfc_nand_page_read(data, page_offset, data_len);
		if (ret)
		{
			printf("SFC: sfc_nand_page_read fail\n");
			return ret;
		}
		data += page_size;
		read_len += data_len;
		page_addr++;
		page_offset = 0;
	}

	return read_len;
}

int spl_sfc_nand_load_image(struct spl_image_info *spl_image, struct spl_boot_device *bootdev)
{
	sfc_init();

#if defined(CONFIG_SPL_OS_BOOT)
	spl_sfc_nand_os_load(spl_image, bootdev); //未添加
	return 0;
#else
	{
		struct image_header *header;
		header = (struct image_header *)(CONFIG_SYS_TEXT_BASE - sizeof(struct image_header));

		sfc_nand_load(CONFIG_SPL_PAD_TO, CONFIG_SYS_MONITOR_LEN, (u8 *)CONFIG_SYS_TEXT_BASE);

		spl_parse_image_header(spl_image, bootdev, header);

	}

	return 0;
#endif
}
SPL_LOAD_IMAGE_METHOD("NAND", 0, BOOT_DEVICE_NAND, spl_sfc_nand_load_image);
