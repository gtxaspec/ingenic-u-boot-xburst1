/*
 * spl_spi.c
 *
 * Internal code for T5. Copy from BootRom
 * Supports load data from SPI NOR/NAND flash.
 *
 * Copyright (C) 2005 - 2014 Ingenic Semiconductor Inc.
 */

#include <common.h>
#include <spl.h>
#include <asm/arch-t5/ssi.h>

#define SPI_NAND_PAGE_SIZE	2048	// assume 2KB
#define SPI_NAND_PPB		128	// page per block
#define SPI_NAND_MAX_RETRIES	8	// retry times

#define CMD_PAGE_READ	0x13	/* Page Read to cache */

#define CMD_WREN 	0x06	/* Write Enable */
#define CMD_WRDI 	0x04	/* Write Disable */
#define CMD_RDSR 	0x05	/* Read Status Register */
#define CMD_WRSR 	0x01	/* Write Status Register */
#define CMD_READ 	0x03	/* Read Data */
#define CMD_FASH_READ 	0x0B	/* Read Data at high speed */
#define CMD_PP 		0x02	/* Page Program(write data) */
#define CMD_SE 		0xD8	/* Sector Erase */
#define CMD_BE 		0xC7	/* Bulk or Chip Erase */
#define CMD_DP 		0xB9	/* Deep Power-Down */
#define CMD_RES 	0xAB	/* Release from Power-Down and Read Electronic Signature */
#define CMD_RDID	0x9F	/* Read Identification */

#define SPI_TXRX_BYTE_WAIT_TIME (20 * 1)

#define SPI_SIG_LEN	(4)
#define SPI_NOR		(1)
#define SPI_NAND	(2)

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

#define __disable_ssi()					\
do{							\
	__ssi_disable(0);				\
}while (0)

#define __enable_ssi()					\
do{							\
	__ssi_enable(0);				\
}while (0)

#define __ssi_flush_txrxfifo()		\
do{					\
	__ssi_flush_fifo(0);		\
	while(!__ssi_rxfifo_empty(0));	\
}while (0)

#define __ssi_reg_init()			\
do{						\
	REG_SSI_GR(0) = 11;			\
						\
	REG_SSI_CR1(0) = SSI_CR1_TFVCK_3 |	\
			SSI_CR1_TCKFI_3 |	\
			SSI_CR1_FMAT_SPI |	\
			SSI_CR1_GPCMD |		\
			SSI_CR1_GPCHL |		\
			SSI_CR1_FLEN_8BIT;	\
	REG_SSI_CR0(0) = SSI_CR0_EACLRUN |	\
			SSI_CR0_TFLUSH |	\
			SSI_CR0_RFLUSH;		\
} while(0)

static unsigned char ssi_get_byte(void)
{
	REG_SSI_DR(0) = 0;
	while (__ssi_rxfifo_empty(0));

	return REG_SSI_DR(0);
}

static unsigned int ssi_get_word(void)
{
	volatile int i;
	unsigned int word = 0;
	unsigned char *p = (unsigned char *)&word;

	for (i = 3; i >= 0 ; i--) {
		*(p++) = ssi_get_byte();
	}

	return word;
}

static int jz_spi_write_dummy_and_read( unsigned char * read_buf, int count)
{
	/* flush Rx-FIFO and Tx-FIFO */
	__ssi_flush_txrxfifo();

	/* send one byte, read one byte, */
	while ( count-- ) {
		*read_buf++ = ssi_get_byte();
	}

	return 0;
}

static int jz_spi_write_only(const unsigned char *buf, int count)
{
	int count0 = count;

	/* Tx-FIFO and RX-FIFO need flush? */
	/* flush Rx-FIFO and Tx-FIFO */
	__ssi_flush_txrxfifo();

	while ( count-- ) {
		REG_SSI_DR(0) = *buf++;
		while ( __ssi_txfifo_full(0) ) {	/* if TX-FIFO full, wait.. */
			udelay(10); /* dleay 10 us */
		}
	}

	/* wait until all data drived out */
	while (__ssi_get_rxfifo_count(0) != count0);

	return 0;
}

static int spi_nand_read_to_cache(int page)
{
	unsigned int addr = 0;
	unsigned char cmd_13h = 0x13;

	/* address: 33 22 11. will be sent out as: 11 22 33
	 * As spi flash wish MSB first: 33 22 11,
	 * translate address to: 11 22 33, will be sent out as: 33 22 11
	 */
	/*
	 * 8 dummy bits + 16-bit page/block address
	 */
	addr = ((page&0xFF)<<16) | (page&0x0000FF00) | ((page>>16)&0xFF);

	__ssi_flush_fifo(0);
	__ssi_wait_transmit(0); /* busy wait mode, assert CS before send data*/

	jz_spi_write_only( &cmd_13h, 1);			/* send read page command */
	jz_spi_write_only( (unsigned char *)&addr, 3);		/* send 3 bytes address */

	__ssi_finish_transmit(0); /* terminate transfer and deassert CS */

	while ( __ssi_is_busy(0) ) { /* wait trans end */
		DBG("ssi is busy, wait...\n");
		udelay(10);
	}

	return 0;
}

int spi_nand_read_from_cache(unsigned char *buf, int count)
{
	unsigned char cmd_03h = 0x03;
	unsigned int col = 0;

	__ssi_flush_fifo(0);
	__ssi_wait_transmit(0); /* busy wait mode, assert CS before send data*/

	jz_spi_write_only( &cmd_03h, 1);		/* send read from cache command */
	jz_spi_write_only( (unsigned char *)&col, 3);	/* send 3 bytes address */

	jz_spi_write_dummy_and_read( buf, count);

	__ssi_finish_transmit(0); /* terminate transfer and deassert CS */

	while ( __ssi_is_busy(0) ) { /* wait trans end */
		DBG("ssi is busy, wait...\n");
		udelay(10);
	}

	return count;
}

/*
 * read data start at any address
 * needn't wait WIP(wirte in progress),
 * while wait WIP end in write operations(PP,SE BE).
 */
static int spi_nand_page_read(unsigned char * buf, unsigned int page, int count)
{
	spi_nand_read_to_cache(page);

	udelay(500);	/* delay 500 us */

	spi_nand_read_from_cache(buf, count);

	return count;
}

static void spi_init(void)
{
	/* gpio already inited by gpio_init() */
	//__gpio_as_spi_boot();

	/* Configure SSI controller */
	__disable_ssi();
	__ssi_reg_init();
	__enable_ssi();

	/* Manually set SSI Control Register1 UNFIN bit */
	REG_SSI_CR1(0) |= SSI_CR1_UNFIN ;
}

//--------------------------------------------------------------
// Read the first 4 words in spi nor to get the information of it,
// and change SSI clock according to the value of SSI_GR in the 7th
// byte. If there is no boot codes in SPI NOR, jump to sd boot(MSC1).
// Spl code length in the third word. Check sum in the 4th word.
//
// The content of the 4 words:
//	  | 0x4 | 0x3 | 0x2 | 0x55 | 0xaa | 0x55 | 0xaa | SSI_GR |
// index     0     1     2      3      4      5      6      7
//	  | len(LSB) | len | len | len(MSB) | check(LSB) | check | check | check(MSB) |
// index       8        9     a        b          c          d       e         f
//
// The first byte read from spi nor is the number of address
// bytes, which will be 4, 3 or 2, and it will be stored to num_addr.
//--------------------------------------------------------------
static int spi_get_params(void)
{
	int retrycnt = 1;
	unsigned int addr_len, spi_sig[SPI_SIG_LEN];
	unsigned char *sig = (unsigned char *)spi_sig;
	unsigned int timeout;

retry:
	timeout = 0xffff;

	/* firstly, read it assuming it's a 4-bytes-address spi nor */
	REG_SSI_DR(0) = CMD_READ;			//read command
	REG_SSI_DR(0) = 0x00;				//4 bytes address
	REG_SSI_DR(0) = 0x00;
	REG_SSI_DR(0) = 0x00;
	REG_SSI_DR(0) = 0x00;

	udelay(500); /* delay 500 us */

	/* timeout handle : wait receive 5byte */
	while (__ssi_get_rxfifo_count(0) < 5) {
		if (timeout == 0) {
			DBG("spl error: spi read params error!\n");
			return -1;
		}
		timeout--;
	}

	jz_spi_write_dummy_and_read(sig, SPI_SIG_LEN * sizeof(int));

	addr_len = sig[0];
	if ((addr_len >= 0x2) && (addr_len <= 0x4)) {
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
			//flash.bpp = sig[addr_len + 5] * 1024;
			//flash.ppb = sig[addr_len + 4] * 32;
			flash.bpp = SPI_NAND_PAGE_SIZE;
			flash.ppb = SPI_NAND_PPB;
			DBG("spl: detect spi nand device, bpp = [%d], ppb = [%d]\n", flash.bpp, flash.ppb);
		}
	} else {
		/* wrong data, may be spi nand */
		__ssi_finish_transmit(0);
		while (__ssi_is_busy(0));

		spi_nand_read_to_cache(0x0);
		__ssi_flush_fifo(0);
		__ssi_wait_transmit(0);

		udelay(500); /* delay 500 us */

		if (retrycnt--)
			goto retry;
	}
	flash.addr_len = addr_len;

	return 0;
}

static int spi_load(unsigned int offset, unsigned int len_words, unsigned int *buf)
{
	int i;
	unsigned char *cbuf = (unsigned char *)buf;

	if (flash.type == SPI_NAND) {
		unsigned int start_page;
		unsigned int wpp = flash.bpp >> 2; /* word per page */
		unsigned int pages = len_words / (flash.bpp / sizeof(int));
		unsigned int leftw = len_words % wpp;

		if (offset % flash.bpp) {
			DBG("spl error: offset is not align to spi nand page! offset = [%d]\n", offset);
			return -1;
		}

		start_page = offset / flash.bpp;

		__ssi_finish_transmit(0); /* terminate transfer and deassert CS */
		while (__ssi_is_busy(0));

		for (i = 0; i < pages; i++) {
			spi_nand_page_read(cbuf, start_page + i, flash.bpp);
			cbuf += flash.bpp;
		}

		if (leftw) {
			spi_nand_page_read(cbuf, start_page + i, leftw * sizeof(int));
		}
	} else {
		unsigned int len_bytes = len_words * sizeof(int);

		// load spl: spi nor
		REG_SSI_DR(0) = CMD_READ;
		REG_SSI_DR(0) = 0x00;
		REG_SSI_DR(0) = 0x00;
		if (flash.addr_len == 0x4) {
			REG_SSI_DR(0) = 0x00;
			REG_SSI_DR(0) = 0x00;
		} else if (flash.addr_len == 0x3) {
			REG_SSI_DR(0) = 0x00;
		}

		while (__ssi_get_rxfifo_count(0) != (flash.addr_len + 1));

		__ssi_flush_txrxfifo();				// clean rx fifo

		for (i = 0; i < 128; i++)			// prepare get data
			REG_SSI_DR(0) = 0x0;

		for (i = 0; i < len_bytes; i++) {
			while(__ssi_rxfifo_empty(0));

			*(cbuf++) = REG_SSI_DR(0);
			REG_SSI_DR(0) = 0x00;
		}
	}

	return 0;
}

void spl_spi_load_image(void)
{
	struct image_header *header = (struct image_header *)(CONFIG_SYS_TEXT_BASE);

	/* spi init */
	spi_init();

	/* get type ppb bpp */
	/* NOTE & TODO: here need retry the backups it read the first error */
	spi_get_params();

	/* read the image header */
	spi_load(CONFIG_SPL_PAD_TO, (sizeof(struct image_header) + 3) >> 2, (void *)CONFIG_SYS_TEXT_BASE);

	/* parse image header */
	spl_parse_image_header(header);

	/* load the rest */
	spi_load(CONFIG_SPL_PAD_TO, (spl_image.size + 3) >> 2, (void *)CONFIG_SYS_TEXT_BASE);
}
