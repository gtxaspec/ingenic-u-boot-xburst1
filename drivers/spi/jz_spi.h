/*
 * Ingenic JZ SPI driver
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Tiger <lbh@ingenic.cn>
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

#ifndef __JZ_SPI_H__
#define __JZ_SPI_H__
#ifdef CONFIG_JZ_SFC
#include <asm/arch/sfc.h>
#endif /* CONFIG_JZ_SFC */
#define SPL_TYPE_FLAG_LEN       6
#define SSI_BASE SSI0_BASE
#define COMMAND_MAX_LENGTH		8
#define SIZEOF_NAME			32
#define FIFI_THRESHOLD			64
#define SPI_WRITE_CHECK_TIMES		50

struct spi_nor_block_info {
	u32 blocksize;
	u8 cmd_blockerase;
	/* MAX Busytime for block erase, unit: ms */
	u32 be_maxbusy;
};

struct spi_quad_mode {
	u8 dummy_byte;
	u8 RDSR_CMD;
	u8 WRSR_CMD;
	unsigned int RDSR_DATE;//the data is write the spi status register for QE bit
	unsigned int RD_DATE_SIZE;//the data is write the spi status register for QE bit
	unsigned int WRSR_DATE;//this bit should be the flash QUAD mode enable
	unsigned int WD_DATE_SIZE;//the data is write the spi status register for QE bit
	u8 cmd_read;
	u8 sfc_mode;
	u8 otp_ops_mode;
};

struct jz_spi_support {
	unsigned int id_manufactory;
	unsigned char id_device;
	char name[SIZEOF_NAME];
	int page_size;
	int oobsize;
	int sector_size;
	int block_size;
	int addr_size;
	int size;
	int page_num;
	unsigned int *page_list;
	unsigned short column_cmdaddr_bits;/* read from cache ,the bits of cmd + addr */
	struct spi_quad_mode quad_mode;
};

#ifdef CONFIG_JZ_SFC
struct norflash_params {
	char name[SIZEOF_NAME];
	u32 pagesize;
	u32 sectorsize;
	u32 chipsize;
	u32 erasesize;
	int id;
	/* Flash Address size, unit: Bytes */
	int addrsize;

	/* MAX Busytime for page program, unit: ms */
	u32 pp_maxbusy;
	/* MAX Busytime for sector erase, unit: ms */
	u32 se_maxbusy;
	/* MAX Busytime for chip erase, unit: ms */
	u32 ce_maxbusy;

	/* Flash status register num, Max support 3 register */
	int st_regnum;
	/* Some NOR flash has different blocksize and block erase command,
	 *          * One command with One blocksize. */
	struct spi_nor_block_info block_info;
	struct spi_quad_mode quad_mode;
};

struct nor_sharing_params {
	uint32_t magic;
	uint32_t version;
	struct norflash_params norflash_params;
	struct norflash_partitions norflash_partitions;
};
#endif

struct jz_spi_slave {
	struct spi_slave slave;
	unsigned int mode;
	unsigned int max_hz;
};
struct jz_spi_support_from_burner{
	unsigned int id_manufactory;
	unsigned char id_device;
	char name[32];
	int page_size;
	int oobsize;
	int sector_size;
	int block_size;
	int size;
	int page_num;
	uint32_t tRD_maxbusy;
	uint32_t tPROG_maxbusy;
	uint32_t tBERS_maxbusy;
	unsigned short column_cmdaddr_bits;
};
struct nand_param_from_burner {
	int version;
	int flash_type;
	int para_num;
	struct jz_spi_support_from_burner *addr;
	int partition_num;
	struct jz_spinand_partition *partition;
};

#if 0
#ifdef CONFIG_SFC_NAND
static struct jz_spi_support jz_spi_nand_support_table[] = {
	{
		.id_manufactory = 0xc8,
		.id_device = 0xf1,
		.name = "GD5F1GQ4U",
		.page_size = 2 * 1024,
		.block_size = 128 * 1024,
		.size = 128 * 1024 * 1024,
	},
	{
		.id_manufactory = 0xc8,
		.id_device = 0xf2,
		.name = "GD5F2GQ4U",
		.page_size = 2 * 1024,
		.block_size = 128 * 1024,
		.size = 256 * 1024 * 1024,
	},
	{
		.id_manufactory = 0xc8,
		.id_device = 0xf4,
		.name = "GD5F4GQ4U",
		.page_size = 2 * 1024,
		.block_size = 128 * 1024,
		.size = 512 * 1024 * 1024,
	}
};
#endif
#endif
#ifdef CONFIG_SFC_NOR
static struct jz_spi_support jz_spi_support_table[] = {
	{
		.name           = "MX25L128**E",
		.id_manufactory = 0xc22018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "MX25L64**E",
		.id_manufactory = 0xc22017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "MX25L25645G/35F",
		.id_manufactory = 0xc22019,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "MX25L51245G",
		.id_manufactory = 0xc2201A,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (64 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "GD25Q64C",
		.id_manufactory = 0xc84017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "GD25LQ128E",
		.id_manufactory = 0xc86018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,// the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,// this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "GD25Q128C",
		.id_manufactory = 0xc84018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,// the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,// this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "GD25Q256D/E",
		.id_manufactory = 0xc84019,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "W25Q64",
		.id_manufactory = 0xef4017,
		.page_size       = 256,
		.addr_size = 3,
		.sector_size      = (64 * 1024),
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "W25Q128",
		.id_manufactory = 0xef4018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//his bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},

	},
	{
		.name           = "W25Q128-DTR",
		.id_manufactory = 0xef7018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//his bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},

	},
	{
		.name           = "W25Q256JV",
		.id_manufactory = 0xef4019,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//his bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		}
	},
	{
		.name           = "W25Q512JV",
		.id_manufactory = 0xef4020,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (64 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//his bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		}
	},
       {
		   .name           = "PALLADIUM_128A",
		   .id_manufactory = 0x202018,
		   .page_size       = 256,
		   .sector_size      = (64 * 1024),
		   .addr_size = 3,
		   .size           = (16 * 1024 * 1024),
		   .quad_mode = {
			   .dummy_byte = 8,
			   .RDSR_CMD = CMD_RDSR,
			   .WRSR_CMD = CMD_WRSR,
			   .RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			   .RD_DATE_SIZE = 1,
			   .WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			   .WD_DATE_SIZE = 1,
			   .cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			   .sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		   }
	   },
	{
		.name           = "XM25QH128A",
		.id_manufactory = 0x207018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
			.otp_ops_mode = 1,
		},
	},
	{
		.name           = "XM25QH64A",
		.id_manufactory = 0x207017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
			.otp_ops_mode = 1,
		},
	},
	{
		.name           = "XM25QH128B",
		.id_manufactory = 0x206018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XM25QH64B",
		.id_manufactory = 0x206017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XM25QH64C",
		.id_manufactory = 0x204017,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XM25QH128C",
		.id_manufactory = 0x204018,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XM25QH256C",
		.id_manufactory = 0x204019,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XM25QU128C",
		.id_manufactory = 0x204118,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XM25QU256C",
		.id_manufactory = 0x204119,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "EN25QH64",
		.id_manufactory = 0x1c7017,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
			.otp_ops_mode = 1,
		},
	},
	{
		.name           = "EN25QH128A",
		.id_manufactory = 0x1c7018,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
			.otp_ops_mode = 1,
		},
	},
	{
		.name           = "EN25QH256A",
		.id_manufactory = 0x1c7019,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
			.otp_ops_mode = 1,
		},
	},
		{
		.name           = "EN25QX64A",
		.id_manufactory = 0x1c7117,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "EN25QX128A",
		.id_manufactory = 0x1c7118,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
		{
		.name           = "EN25QY256A",
		.id_manufactory = 0x1c7319,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "FM25Q64",
		.id_manufactory = 0xF83217,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "FM25W128",
		.id_manufactory = 0xa12818,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "BY25Q64AS",
		.id_manufactory = 0x684017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "BY25Q128AS",
		.id_manufactory = 0x684018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
		{
		.name           = "BY25YQ256ES",
		.id_manufactory = 0x684019,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "ZB25VQ64",
		.id_manufactory = 0x5e4017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
		{
		.name           = "DS25Q64A",
		.id_manufactory = 0xe53117,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "DS25Q4AA",
		.id_manufactory = 0xe53118,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "ZB25VQ128",
		.id_manufactory = 0x5e4018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "ZB25Q256A",
		.id_manufactory = 0x5e4019,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},

	{
		.name           = "XT25F64B",
		.id_manufactory = 0x0b4017,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x200,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 2,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XT25F128B",
		.id_manufactory = 0x0b4018,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x200,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 2,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "XT25F256B",
		.id_manufactory = 0x0b4019,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x200,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 2,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},

	{
		.name           = "XT25Q128D",
		.id_manufactory = 0x0b6018,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "FS25Q064F2TFI1",
		.id_manufactory = 0xa14017,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "FS25Q128F2TFI",
		.id_manufactory = 0xa14018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "P25Q64H",
		.id_manufactory = 0x856017,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "P25Q128H",
		.id_manufactory = 0x856018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x2,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x2,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "ZD25Q64B",
		.id_manufactory = 0xba3217,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "IS25LP064D",
		.id_manufactory = 0x9d6017,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "IS25LP128F",
		.id_manufactory = 0x9d6018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "SK25P128",
		.id_manufactory = 0x256018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR,
			.WRSR_CMD = CMD_WRSR,
			.RDSR_DATE = 0x40,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x40,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "NM25Q64EVD",
		.id_manufactory = 0x522217,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x4,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x4,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},

	{
		.name           = "NM25Q128EVB",
		.id_manufactory = 0x522118,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x4,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x4,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},

	{
		.name           = "GM25Q64A",
		.id_manufactory = 0x1c4017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
	{
		.name           = "GM25Q128A",
		.id_manufactory = 0x1c4018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
{
		.name           = "PY25Q64HA",
		.id_manufactory = 0x852017,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (8 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},

	{
		.name           = "PY25Q128HA",
		.id_manufactory = 0x852018,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 3,
		.size           = (16 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
		{
		.name           = "PY25Q256HB",
		.id_manufactory = 0x852019,
		.page_size       = 256,
		.sector_size      = (32 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},
{
		.name           = "FM25Q256I3",
		.id_manufactory = 0xA14019,
		.page_size       = 256,
		.sector_size      = (64 * 1024),
		.addr_size = 4,
		.size           = (32 * 1024 * 1024),
		.quad_mode = {
			.dummy_byte = 8,
			.RDSR_CMD = CMD_RDSR_1,
			.WRSR_CMD = CMD_WRSR_1,
			.RDSR_DATE = 0x02,//the data is write the spi status register for QE bit
			.RD_DATE_SIZE = 1,
			.WRSR_DATE = 0x02,//this bit should be the flash QUAD mode enable
			.WD_DATE_SIZE = 1,
			.cmd_read = CMD_QUAD_READ,
#ifdef CONFIG_JZ_SFC
			.sfc_mode = TRAN_CONF1_SPI_QUAD,
#endif
		},
	},

};
#endif /* CONFIG_SFC_NOR */
#endif /* __JZ_SPI_H__ */
