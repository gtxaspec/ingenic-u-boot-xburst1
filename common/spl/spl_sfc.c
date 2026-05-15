/*
 * spl_sfc.c
 *
 * Internal code for T5. Copy from BootRom
 * Supports load data from SPI NOR/NAND flash.
 *
 * Copyright (C) 2005 - 2014 Ingenic Semiconductor Inc.
 */

#include <common.h>
#include <spl.h>
#include <asm/arch/spl_sfc.h>
#include <config.h>
#include <asm/io.h>
#include <asm/arch/clk.h>
#ifdef CONFIG_USE_AES_SECURE_BOOT
#include <aes_efuse.h>
#endif

#define SPI_SIG_LEN	(4)
#define SPI_NOR		(1)
#define SPI_NAND	(2)

unsigned int sfc_quad_mode = 0;

struct flash_params {
	int type; /* SPI_NAND or SPI_NOR */
	unsigned int bpp; /* for spi nand */
	unsigned int ppb; /* for spi nand */
	unsigned int addr_len; /* for spi_nor */
} flash = { 0, 0, 0, 0 };

#ifdef CONFIG_SPL_LIBCOMMON_SUPPORT
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif

static void sfc_init(void)
{
	unsigned int i;
	volatile unsigned int tmp;

	tmp = REG32(SFC_GLB);
	tmp &= ~(TRAN_DIR | OP_MODE);
	tmp |= WP_EN;
	//tmp &= ~WP_EN;
	REG32(SFC_GLB) = tmp;

	//THRESHOLD need init by read function
	//PHASE_NUM need init by read function
	tmp = REG32(SFC_DEV_CONF);
	tmp &= ~(CMD_TYPE | CPHA | CPOL | SMP_DELAY_MSK |
			THOLD_MSK | TSETUP_MSK | TSH_MSK);
	tmp |= (CEDL | HOLDDL | WPDL);
	REG32(SFC_DEV_CONF) = tmp;

	//sta_type need init by read function
	for (i = 0; i < 6; i++) {
		REG32(SFC_TRAN_CONF(i)) &= ~(TRAN_MODE_MSK | FMAT);
	}
	REG32(SFC_SCR) = (CLR_END | CLR_TREQ |
			CLR_RREQ | CLR_OVER | CLR_UNDER);
	REG32(SFC_INTC) |= (MASK_END | MASK_TREQ |
			MASK_RREQ | MASK_OVER | MASK_UNDR);
	REG32(SFC_CGE) = 0;

	tmp = REG32(SFC_GLB);
	tmp &= ~(THRESHOLD_MSK);
	tmp |= (THRESHOLD << THRESHOLD_OFFSET);
	REG32(SFC_GLB) = tmp;

	/* bootrom not stop, here stop it to avoid FIFO residual error */
	REG32(SFC_TRIG) = STOP;
	REG32(SFC_TRIG) = FLUSH;
}

static int sfc_read_data(unsigned int *data, unsigned int len)
{
	unsigned int tmp_len = 0;
	unsigned int fifo_num = 0;
	unsigned int i;
	unsigned int reg_tmp;
	unsigned int timeout = 0xffff;

	while (1) {
		reg_tmp = REG32(SFC_SR);
		if (reg_tmp & RECE_REQ) {
			REG32(SFC_SCR) = (CLR_RREQ);
			if ((len - tmp_len) > THRESHOLD)
				fifo_num = THRESHOLD;
			else {
				fifo_num = len - tmp_len;
			}

			for (i = 0; i < fifo_num; i++) {
				*data = REG32(SFC_RM_DR);
				data++;
				tmp_len++;
			}
		}

		if (tmp_len == len)
			break;
	}

	while (!((REG32(SFC_SR)) & END)) {
		if (timeout == 0)
			break;
		timeout--;
	}

	if ((REG32(SFC_SR)) & END)
		REG32(SFC_SCR) = (CLR_END);

	return 0;
}

static void sfc_set_mode(unsigned int mode)
{
	volatile unsigned int tmp;

	tmp = REG32(SFC_TRAN_CONF1(0));
	tmp &= ~(0xf << 4);
	tmp |= mode << 4;
	REG32(SFC_TRAN_CONF1(0)) = tmp;
}

static void sfc_set_read_reg(unsigned int cmd, unsigned int addr,
		unsigned int addr_plus, unsigned int addr_len, unsigned int data_en)
{
	volatile unsigned int tmp;
	unsigned int timeout = 0xffff;

	if ((REG32(SFC_SR) & BUSY_MSK)) {
		DBG("Now sfc busy,and stop it\n");
		REG32(SFC_TRIG) = STOP;
		while (!((REG32(SFC_SR)) & END)) {
			if (timeout == 0)
				break;
			timeout--;
		}
	}
	REG32(SFC_SCR) = (CLR_END | CLR_TREQ |
			CLR_RREQ | CLR_OVER | CLR_UNDER);

	REG32(SFC_GLB1) &= ~(GLB1_SFC_MODE2);
	tmp = REG32(SFC_GLB);
	tmp &= ~PHASE_NUM_MSK;
	tmp |= (0x1 << PHASE_NUM_OFFSET);
	REG32(SFC_GLB) = tmp;

	tmp = REG32(SFC_TRAN_CONF(0));
	tmp &= ~(CLK_MODE | ADDR_WIDTH_MSK | DMYBITS_MSK | CMD_MSK | FMAT | DATEEN);
	if (data_en) {
		tmp |= (addr_len << ADDR_WIDTH_OFFSET) | CMDEN |
			DATEEN | (cmd << CMD_OFFSET);
	} else {
		tmp |= (addr_len << ADDR_WIDTH_OFFSET) | CMDEN |
			(cmd << CMD_OFFSET);
	}

	if (sfc_quad_mode)
	{
		sfc_set_mode(TRAN_CONF1_SPI_IO_QUAD);
		tmp |= 4 << DMYBITS_OFFSET;
	}
	else
	{
		sfc_set_mode(TRAN_CONF1_SPI_STANDARD);
		tmp &= ~(DMYBITS_MSK);
	}
	REG32(SFC_TRAN_CONF(0)) = tmp;

	REG32(SFC_DEV_ADDR(0)) = addr;
	REG32(SFC_DEV_ADDR_PLUS(0)) = addr_plus;
	REG32(SFC_TRIG) = (START);
}

static int sfc_read(unsigned int addr, unsigned int addr_plus,
		unsigned int addr_len, unsigned int *data, unsigned int len)
{
	int ret;

	REG32(SFC_TRAN_LEN) = len * 4;

	if (sfc_quad_mode)
		sfc_set_read_reg(0xeb, addr, addr_plus, addr_len, 1);
	else
		sfc_set_read_reg(0x03, addr, addr_plus, addr_len, 1);

	ret = sfc_read_data(data, len);
	if (ret)
		return ret;
	else
		return 0;
}

static int sfc_nand_read_to_cache(unsigned int page)
{
	unsigned int addr = 0;
	unsigned int timeout = 0xffff;

	addr = page;
	REG32(SFC_TRAN_LEN) = 0;
	sfc_set_read_reg(0x13, addr, 0x0, 3, 0);

	while (!((REG32(SFC_SR)) & END)) {
		if (timeout == 0)
			break;
		timeout--;
	}

	if ((REG32(SFC_SR)) & END)
		REG32(SFC_SCR) = (CLR_END);

	udelay(500);   /* delay 500 us */

	return 0;
}

/*
 *  read data start at any address
 *  needn't wait WIP(wirte in progress),
 *  while wait WIP end in write operations(PP,SE BE).
 */
static int sfc_nand_page_read(unsigned int * buf, unsigned int page, unsigned int count)
{
	int ret = 0;

	ret = sfc_nand_read_to_cache(page);
	if (ret) {
		DBG("spl: sfc nand read to cache error!, ret = %d\n", ret);
		return ret;
	}

	udelay(500); /* delay 500 us */

	ret = sfc_read(0x0, 0x0, 3, buf, count);
	if (ret)
		return ret;
	else
		return 0;

	return count;
}

/*
 * Read the first 4 words in spi nor to get the information of it.
 * If there is no boot codes in SPI NOR, jump to sd boot(MSC1).
 * Spl code length in the third word. The crc7 check value in the 10th byte.
 * The content of the 16 bytes:
 *	  | 0x6	| 0x5 | 0x4 | 0x3 |
 * index     0     1     2     3
 * 	  | 0x2 | 0x55 | 0xaa | 0x55 |
 * index     4      5      6      7
 * 	  |0xaa | crc7 |  ppb |  bpp |
 * index     8	    9	   a      b
 *        | len(LSB) | len | len | len(MSB) |
 * index       c        d     e      f
 * The first byte read from spi nor is the number of address bytes,
 * which will be 6, 5, 4, 3 or 2, and it will be stored to num_addr.
 * pbb:page per block
 * bpp:bytes per page
 */
static int sfc_get_params(void)
{
	int ret, retrycnt = 1;
	unsigned int addr_len, spi_sig[SPI_SIG_LEN];
	unsigned char *sig = (unsigned char *)spi_sig;

retry:
	addr_len = 6;
	/* read as spi nor as default */
	ret = sfc_read(0x0, 0x0, addr_len, spi_sig, SPI_SIG_LEN);
	if (ret) {
		DBG("spl: sfc read params error!, ret = %d\n", ret);
		return -1;
	}

	addr_len = sig[0];
	if ((addr_len >= 0x2) && (addr_len <= 0x6)) {
		if ((sig[addr_len - 1] == 0x55) &&
		    (sig[addr_len] == 0xaa) &&
		    (sig[addr_len + 1] == 0x55) &&
		    (sig[addr_len + 2] == 0xaa)) {
			flash.type = SPI_NOR;
		} else if ((sig[addr_len - 1] == 0x55) &&
			   (sig[addr_len] == 0xaa) &&
			   (sig[addr_len + 1] == 0x55) &&
			   (sig[addr_len + 2] == 0x00)) {
			flash.type = SPI_NAND;
			flash.bpp = sig[addr_len + 5] * 1024;
			flash.ppb = sig[addr_len + 4] * 32;
			DBG("spl: detect sfc nand device, bpp = [%d], ppb = [%d]\n", flash.bpp, flash.ppb);
		} else {
			/* wrong data, may be spi nand */
			ret = sfc_nand_read_to_cache(0x0);
			if (ret) {
				DBG("spl: sfc nand read to cache error! ret = %d\n", ret);
				return -1;
			}
			udelay(500); /* delay 500 us */
			if (retrycnt--)
				goto retry;

			DBG("spl: unknown sfc device type! %x %x %x %x\n", sig[0], sig[1], sig[2], sig[3]);
			return -1;
		}
	} else {
		/* wrong data, may be spi nand */
		ret = sfc_nand_read_to_cache(0x0);
		if (ret) {
			DBG("spl: sfc nand read to cache error! ret = %d\n", ret);
			return -1;
		}
		udelay(500); /* delay 500 us */

		if (retrycnt--)
			goto retry;

		DBG("spl: wrong sfc device addr_len! %x %x %x %x\n", sig[0], sig[1], sig[2], sig[3]);
		return -1;
	}
	flash.addr_len = addr_len;

	return 0;
}

static int sfc_load(unsigned int offset, unsigned int len_words, unsigned int *buf)
{
	int ret = 0;

	if (flash.type == SPI_NAND) {
		int i = 0;
		unsigned int start_page;
		unsigned int wpp = flash.bpp >> 2; /* word per page */
		unsigned int pages = len_words / wpp;
		unsigned int leftw = len_words % wpp;

		if (offset % flash.bpp) {
			DBG("spl error: offset is not align to sfc nand page! offset = [%d]\n", offset);
			return -1;
		}

		start_page = offset / flash.bpp;

		/* NOTE & TODO: need bad block mamager */
		for (i = 0; i < pages; i++) {
			ret = sfc_nand_page_read(buf, start_page + i, wpp);
			if (ret) {
				DBG("spl error: read page from sfc nand error! page = [%d], ret = [%d]\n", start_page + i, ret);
				return -1;
			}
			buf += wpp;
		}

		if (leftw) {
			ret = sfc_nand_page_read(buf, start_page + i, leftw);
			if (ret) {
				DBG("spl error: read page from sfc nand error! page = [%d], ret = [%d]\n", start_page + i, ret);
				return -1;
			}
		}
	} else {
		if (sfc_quad_mode)
			offset = offset << 8;// addr[ 8: 0] Quad I/O Fast Read with “Continuous Read Mode”
		ret = sfc_read(offset, 0, flash.addr_len, (unsigned int *)buf, len_words);
		if (ret) {
			DBG("spl error: read page from sfc nor error! addr_len = [%d], ret = [%d]\n", flash.addr_len, ret);
			return -1;
		}
	}

	return 0;
}

void spl_sfc_load_image(void)
{
#ifdef CONFIG_USE_AES_SECURE_BOOT
	uint32_t aes_iv[4];
	uint32_t aes_data_len = 0;
#endif

#ifdef CONFIG_SPL_LZOP
	size_t dst_len;
#endif
	struct image_header *header = (struct image_header *)(CONFIG_SYS_TEXT_BASE);
#ifndef CONFIG_FPGA
	clk_set_rate(SFC0, 50000000);
#endif

#ifdef CONFIG_SPI_QUAD
	sfc_quad_mode = 1;
#endif
	/* sfc init */
	sfc_init();
	/* get type ppb bpp */
	/* NOTE & TODO: here need retry the backups it read the first error */
	sfc_get_params();
	/* read the image header */
	//sfc_load(CONFIG_UBOOT_OFFSET, (sizeof(struct image_header) + 3) >> 2, (void *)CONFIG_SYS_TEXT_BASE);
	memset((void *)CONFIG_SYS_TEXT_BASE, 0, sizeof(struct image_header));

	/* parse image header */
	spl_parse_image_header(header);
	/* load the rest */
#ifdef CONFIG_SPL_LZOP
	sfc_load(CONFIG_UBOOT_OFFSET, (spl_image.size + 3) >> 2, (void *)CONFIG_DECMP_BUFFER_ADRS);
	lzop_decompress((unsigned char *)(CONFIG_DECMP_BUFFER_ADRS+0x40),spl_image.size-0x40,
			(unsigned char *)CONFIG_SYS_TEXT_BASE,&dst_len);
#else
	sfc_load(CONFIG_UBOOT_OFFSET, (spl_image.size + 3) >> 2, (void *)CONFIG_SYS_TEXT_BASE);
#endif

#ifdef CONFIG_USE_AES_SECURE_BOOT
	aes_data_len = get_aes_header_length_and_iv(CONFIG_SYS_TEXT_BASE, aes_iv);
	flush_cache_all();
	aes_efuse_init(aes_iv, CONFIG_AES_DEC_MODE, CONFIG_AES_EFUSE_KEY_BIT);
	aes_efuse_start(CONFIG_SYS_TEXT_BASE + 20, CONFIG_SYS_MONITOR_LEN, CONFIG_SYS_TEXT_BASE);
	flush_cache_all();
#endif

	return;
}
