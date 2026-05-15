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

u32 addr_len = 0;

static u32 sfc_nor_load(u32 addr, u32 data_len, u8 *data)
{
	u32 dummy_bits = 0x0;
	int ret = 0;

	ret = sfc_execution(0x03, addr, addr_len, dummy_bits, data_len, data, 0);
	if (ret)
	{
		printf("SFC: sfc_nand_page_read fail\n");
	}

	return data_len;
}

#define SPI_SIG_LEN (16)
static int sfc_get_params(void)
{
	int ret = 0;
	u8 sig[SPI_SIG_LEN] = {0};

	addr_len = 6;
	/* read as spi nor as default */
	ret = sfc_nor_load(0x0, SPI_SIG_LEN, sig);
	if (SPI_SIG_LEN != ret)
	{
		printf("spl: sfc read params error!, read_len = %d, want_len = %d\n", ret, SPI_SIG_LEN);
		return -1;
	}

	addr_len = sig[0];
	if ((addr_len >= 0x2) && (addr_len <= 0x6))
	{
		if ((sig[addr_len - 1] == 0x55) &&
			(sig[addr_len] == 0xaa) &&
			(sig[addr_len + 1] == 0x55) &&
			(sig[addr_len + 2] == 0xaa))
		{
			// flash.type is nor
			ret = 0;
		}
		else
		{
			ret = -1;
		}
	}

	return ret;
}

int spl_sfc_nor_load_image(struct spl_image_info *spl_image, struct spl_boot_device *bootdev)
{
	sfc_init();
	if (sfc_get_params()) // 获取flash addr_len
	{
		printf("spl: sfc get params error!\n");
		return -1;
	}
#if defined(CONFIG_SPL_OS_BOOT)
	spl_sfc_nor_os_load(spl_image, bootdev); //未添加
	return 0;
#else
	{
		struct image_header *header;
		header = (struct image_header *)(CONFIG_SYS_TEXT_BASE - sizeof(struct image_header));

		sfc_nor_load(CONFIG_SPL_PAD_TO, CONFIG_SYS_MONITOR_LEN, (u8 *)CONFIG_SYS_TEXT_BASE);

		spl_parse_image_header(spl_image, bootdev, header);

	}

	return 0;
#endif
}
SPL_LOAD_IMAGE_METHOD("NOR", 0, BOOT_DEVICE_NOR, spl_sfc_nor_load_image);
