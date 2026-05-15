/*
 * #endif
 * Ingenic burner function Code (Vendor Private Protocol)
 *
 * Copyright (c) 2014 jykang <jykang@ingenic.cn>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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

#include <errno.h>
#include <common.h>
#include <malloc.h>
#include <mmc.h>
#include <rtc.h>
#include <part.h>
#include <efuse.h>
#include <ingenic_soft_i2c.h>
#include <ingenic_nand_mgr/nand_param.h>
#include <linux/compiler.h>
#include <asm/arch/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <config.h>
#include <serial.h>

#include <asm/jz_uart.h>
#include <asm/arch/base.h>
#include <vsprintf.h>
#define TOTAL 24

char ptemp[16][32];
//char stateok[32]="state:ok\n";
char stateok[32]="ok\n";
char stateretry[32]="state:retry\n";
char *data_addr;
int write_back_chk;
int count=0;
#define REG32(addr)	*((volatile unsigned int *)(addr))

#define IRDA_BASE	UART0_BASE
#define UART_BASE	UART0_BASE
#define UART_OFF	0x1000

/* Register Offset */
#define OFF_RDR		(0x00)	/* R  8b H'xx */
#define OFF_TDR		(0x00)	/* W  8b H'xx */
#define OFF_DLLR	(0x00)	/* RW 8b H'00 */
#define OFF_DLHR	(0x04)	/* RW 8b H'00 */
#define OFF_IER		(0x04)	/* RW 8b H'00 */
#define OFF_ISR		(0x08)	/* R  8b H'01 */
#define OFF_FCR		(0x08)	/* W  8b H'00 */
#define OFF_LCR		(0x0C)	/* RW 8b H'00 */
#define OFF_MCR		(0x10)	/* RW 8b H'00 */
#define OFF_LSR		(0x14)	/* R  8b H'00 */
#define OFF_MSR		(0x18)	/* R  8b H'00 */
#define OFF_SPR		(0x1C)	/* RW 8b H'00 */
#define OFF_SIRCR	(0x20)	/* RW 8b H'00, UART0 */
#define OFF_UMR		(0x24)	/* RW 8b H'00, UART M Register */
#define OFF_UACR	(0x28)	/* RW 8b H'00, UART Add Cycle Register */
#define OFF_URCR	(0x40)	/* R  8b   00, UART RXFIFO Counter Register */
#define OFF_UTCR	(0x44)	/* R  8b   00, UART TXFIFO Counter Register */

#define UART1_RDR	(UART3_BASE + OFF_RDR)
#define UART1_TDR	(UART3_BASE + OFF_TDR)
#define UART1_DLLR	(UART3_BASE + OFF_DLLR)
#define UART1_DLHR	(UART3_BASE + OFF_DLHR)
#define UART1_IER	(UART3_BASE + OFF_IER)
#define UART1_ISR	(UART3_BASE + OFF_ISR)
#define UART1_FCR	(UART3_BASE + OFF_FCR)
#define UART1_LCR	(UART3_BASE + OFF_LCR)
#define UART1_MCR	(UART3_BASE + OFF_MCR)
#define UART1_LSR	(UART3_BASE + OFF_LSR)
#define UART1_MSR	(UART3_BASE + OFF_MSR)
#define UART1_SPR	(UART3_BASE + OFF_SPR)
#define UART1_SIRCR	(UART3_BASE + OFF_SIRCR)
#define UART1_UMR	(UART3_BASE + OFF_UMR)
#define UART1_UACR	(UART3_BASE + OFF_UACR)
#define UART1_URCR	(UART3_BASE + OFF_URCR)
#define UART1_UTCR	(UART3_BASE + OFF_UTCR)

#define UART3_RDR	(UART3_BASE + OFF_RDR)
#define UART3_TDR	(UART3_BASE + OFF_TDR)
#define UART3_DLLR	(UART3_BASE + OFF_DLLR)
#define UART3_DLHR	(UART3_BASE + OFF_DLHR)
#define UART3_IER	(UART3_BASE + OFF_IER)
#define UART3_ISR	(UART3_BASE + OFF_ISR)
#define UART3_FCR	(UART3_BASE + OFF_FCR)
#define UART3_LCR	(UART3_BASE + OFF_LCR)
#define UART3_MCR	(UART3_BASE + OFF_MCR)
#define UART3_LSR	(UART3_BASE + OFF_LSR)
#define UART3_MSR	(UART3_BASE + OFF_MSR)
#define UART3_SPR	(UART3_BASE + OFF_SPR)
#define UART3_SIRCR	(UART3_BASE + OFF_SIRCR)
#define UART3_UMR	(UART3_BASE + OFF_UMR)
#define UART3_UACR	(UART3_BASE + OFF_UACR)
#define UART3_URCR	(UART3_BASE + OFF_URCR)
#define UART3_UTCR	(UART3_BASE + OFF_UTCR)


#define UARTIER_RIE	(1 << 0)	/* 0: receive fifo full interrupt disable */
#define UARTIER_TIE	(1 << 1)	/* 0: transmit fifo empty interrupt disable */
#define UARTIER_RLIE	(1 << 2)	/* 0: receive line status interrupt disable */
#define UARTIER_MIE	(1 << 3)	/* 0: modem status interrupt disable */
#define UARTIER_RTIE	(1 << 4)	/* 0: receive timeout interrupt disable */

/*
 * Define macros for UARTISR
 * UART Interrupt Status Register
 */
#define UARTISR_IP	(1 << 0)	/* 0: interrupt is pending  1: no interrupt */
#define UARTISR_IID	(7 << 1)	/* Source of Interrupt */
#define UARTISR_IID_MSI		(0 << 1)  /* Modem status interrupt */
#define UARTISR_IID_THRI	(1 << 1)  /* Transmitter holding register empty */
#define UARTISR_IID_RDI		(2 << 1)  /* Receiver data interrupt */
#define UARTISR_IID_RLSI	(3 << 1)  /* Receiver line status interrupt */
#define UARTISR_IID_RTO		(6 << 1)  /* Receive timeout */
#define UARTISR_FFMS		(3 << 6)  /* FIFO mode select, set when UARTFCR.FE is set to 1 */
#define UARTISR_FFMS_NO_FIFO	(0 << 6)
#define UARTISR_FFMS_FIFO_MODE	(3 << 6)

/*
 * Define macros for UARTFCR
 * UART FIFO Control Register
 */
#define UARTFCR_FE	(1 << 0)	/* 0: non-FIFO mode  1: FIFO mode */
#define UARTFCR_RFLS	(1 << 1)	/* write 1 to flush receive FIFO */
#define UARTFCR_TFLS	(1 << 2)	/* write 1 to flush transmit FIFO */
#define UARTFCR_DMS	(1 << 3)	/* 0: disable DMA mode */
#define UARTFCR_UUE	(1 << 4)	/* 0: disable UART */
#define UARTFCR_RTRG	(3 << 6)	/* Receive FIFO Data Trigger */
#define UARTFCR_RTRG_1	(0 << 6)
#define UARTFCR_RTRG_16	(1 << 6)
#define UARTFCR_RTRG_32	(2 << 6)
#define UARTFCR_RTRG_60	(3 << 6)

/*
 * Define macros for UARTLCR
 * UART Line Control Register
 */
#define UARTLCR_WLEN	(3 << 0)	/* word length */
#define UARTLCR_WLEN_5	(0 << 0)
#define UARTLCR_WLEN_6	(1 << 0)
#define UARTLCR_WLEN_7	(2 << 0)
#define UARTLCR_WLEN_8	(3 << 0)
#define UARTLCR_STOP	(1 << 2)	/* 0: 1 stop bit when word length is 5,6,7,8
1: 1.5 stop bits when 5; 2 stop bits when 6,7,8 */
#define UARTLCR_STOP1	(0 << 2)
#define UARTLCR_STOP2	(1 << 2)
#define UARTLCR_PE	(1 << 3)	/* 0: parity disable */
#define UARTLCR_PROE	(1 << 4)	/* 0: even parity  1: odd parity */
#define UARTLCR_SPAR	(1 << 5)	/* 0: sticky parity disable */
#define UARTLCR_SBRK	(1 << 6)	/* write 0 normal, write 1 send break */
#define UARTLCR_DLAB	(1 << 7)	/* 0: access UARTRDR/TDR/IER  1: access UARTDLLR/DLHR */

/*
 * Define macros for UARTLSR
 * UART Line Status Register
 */
#define UARTLSR_DR	(1 << 0)	/* 0: receive FIFO is empty  1: receive data is ready */
#define UARTLSR_ORER	(1 << 1)	/* 0: no overrun error */
#define UARTLSR_PER	(1 << 2)	/* 0: no parity error */
#define UARTLSR_FER	(1 << 3)	/* 0; no framing error */
#define UARTLSR_BRK	(1 << 4)	/* 0: no break detected  1: receive a break signal */
#define UARTLSR_TDRQ	(1 << 5)	/* 1: transmit FIFO half "empty" */
#define UARTLSR_TEMT	(1 << 6)	/* 1: transmit FIFO and shift registers empty */
#define UARTLSR_RFER	(1 << 7)	/* 0: no receive error  1: receive error in FIFO mode */

/*
 * Define macros for UARTMCR
 * UART Modem Control Register
 */
#define UARTMCR_RTS	(1 << 1)	/* 0: RTS_ output high, 1: RTS_ output low */
#define UARTMCR_LOOP	(1 << 4)	/* 0: normal  1: loopback mode */
#define UARTMCR_FCM	(1 << 6)	/* 0: software  1: hardware */
#define UARTMCR_MCE	(1 << 7)	/* 0: modem function is disable */

/*
 * Define macros for UARTMSR
 * UART Modem Status Register
 */
#define UARTMSR_CCTS	(1 << 0)        /* 1: a change on CTS_ pin */
#define UARTMSR_CTS	(1 << 4)	/* 0: CTS_ pin is high */

/*
 * Define macros for SIRCR
 * Slow IrDA Control Register
 */
#define SIRCR_TSIRE	(1 << 0)  /* 0: transmitter is in UART mode  1: SIR mode */
#define SIRCR_RSIRE	(1 << 1)  /* 0: receiver is in UART mode  1: SIR mode */
#define SIRCR_TPWS	(1 << 2)  /* 0: transmit 0 pulse width is 3/16 of bit length
1: 0 pulse width is 1.6us for 115.2Kbps */
#define SIRCR_TDPL	(1 << 3)  /* 0: encoder generates a positive pulse for 0 */
#define SIRCR_RDPL	(1 << 4)  /* 0: decoder interprets positive pulse as 0 */

/*
 * Define macros for UART_LSR
 * UART Line Status Register
 */
#define UART_LSR_DR	(1 << 0)	/* 0: receive FIFO is empty  1: receive data is ready */
#define UART_LSR_ORER	(1 << 1)	/* 0: no overrun error */
#define UART_LSR_PER	(1 << 2)	/* 0: no parity error */
#define UART_LSR_FER	(1 << 3)	/* 0; no framing error */
#define UART_LSR_BRK	(1 << 4)	/* 0: no break detected  1: receive a break signal */
#define UART_LSR_TDRQ	(1 << 5)	/* 1: transmit FIFO half "empty" */
#define UART_LSR_TEMT	(1 << 6)	/* 1: transmit FIFO and shift registers empty */
#define UART_LSR_RFER	(1 << 7)	/* 0: no receive error  1: receive error in FIFO mode */

DECLARE_GLOBAL_DATA_PTR;

static struct jz_uart *uart __attribute__ ((section(".data")));

#define BURNNER_DEBUG 0

#define MMC_ERASE_ALL	1
#define MMC_ERASE_PART	2
#define MMC_ERASE_CNT_MAX	10
enum func_type{
	WRITE = 0,
	ARGS,
	READ,
	GET_CPU,
	INIT,
	TIME,
	RUN,
	RESET,
	BAUDRATE
};

extern enum medium_type {
	MEMORY = 0,
	       NAND,
	       MMC,
	       I2C,
	       EFUSE,
	       REGISTER
};

extern enum data_type {
	RAW = 0,
	    OOB,
	    IMAGE,
};

extern struct i2c_args {
	int clk;
	int data;
	int device;
	int value_count;
	int value[0];
};

extern struct mmc_erase_range {
	uint32_t start;
	uint32_t end;
};

struct spi_args
{
	uint32_t clk;
	uint32_t data_in;
	uint32_t data_out;
	uint32_t enable;
	uint32_t rate;
};

struct spi_erase_range{
	uint32_t blocksize;
	uint32_t blockcount;
};

struct jz_spi_support{
	uint8_t id;
	char name[32];
	int page_size;
	int sector_size;
	int size;
};


extern struct arguments {
	int efuse_gpio;
	int use_nand_mgr;
	int use_mmc;
	int use_spi;

	int nand_erase;
	int nand_erase_count;
	unsigned int offsets[32];

	int mmc_open_card;
	int mmc_erase;
	uint32_t mmc_erase_range_count;
	struct mmc_erase_range mmc_erase_range[MMC_ERASE_CNT_MAX];

	struct spi_args spi_args;
	uint32_t spi_erase;
	struct jz_spi_support jz_spi_support_table;
	struct spi_erase_range spi_erase_range;


	int transfer_data_chk;
	int write_back_chk;

	PartitionInfo PartInfo;
	int nr_nand_args;
	nand_flash_param nand_params[0];
};

extern union cmd {
	struct update {
		uint32_t length;
	}update;

	struct write {
		uint64_t partation;
		uint32_t ops;
		uint32_t offset;
		uint32_t length;
		uint32_t crc;
	}write;

	struct read {
		uint64_t partation;
		uint32_t ops;
		uint32_t offset;
		uint32_t length;
	}read;

	struct rtc_time rtc;
};

struct serial_cloner {
	union cmd *cmd;
	int cmd_type;
	uint32_t buf_size;
	int ack;
	struct arguments *args;
	int inited;
};

static uint32_t crc_table[] = {
	/* CRC polynomial 0xedb88320 */
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

static uint32_t local_crc32(uint32_t crc,unsigned char *buffer, uint32_t size)
{
	uint32_t i;
	for (i = 0; i < size; i++) {
		crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
	}
	return crc ;
}

int write = 0;
int data = 0;
const u8 crc7_table[256] = {
	0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
	0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
	0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
	0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
	0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
	0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
	0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
	0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
	0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
	0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
	0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
	0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
	0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
	0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
	0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
	0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
	0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
	0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
	0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
	0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
	0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
	0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
	0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
	0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
	0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
	0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
	0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
	0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
	0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
	0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
	0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
	0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};

static inline u8 t10_crc7_byte(u8 crc, u8 data)
{
	return crc7_table[(crc << 1) ^ data];
}

u8 t10_crc7(u8 crc, u8 *buffer, int len)
{
	while (len--)
		crc = t10_crc7_byte(crc, *buffer++);
	return crc;
}

static void local_serial_setbrg(void)
{
	u32 baud_div, tmp;
#if 0
	writel(17,0xb0033024);
	writel(0,0xb0033028);
	baud_div = 3;
	//460800
#endif

#if 0
	writel(13,0xb0033024);
	writel(0,0xb0033028);
	baud_div = 2;//baudrate 921600
#endif
	baud_div = CONFIG_SYS_EXTAL / 16 / 115200 ;
	//	baud_div = CONFIG_SYS_EXTAL / 16 / 57600 ;

	tmp = readb(&uart->lcr);
	tmp |= UART_LCR_DLAB;
	writeb(tmp, &uart->lcr);

	writeb((baud_div >> 8) & 0xff, &uart->dlhr_ier);
	writeb(baud_div & 0xff, &uart->rbr_thr_dllr);

	tmp &= ~UART_LCR_DLAB;
	writeb(tmp, &uart->lcr);
}

static int local_serial_init(void)
{
	uart = (struct jz_uart *)(UART0_BASE + 1 * 0x1000);

	/* Disable port interrupts while changing hardware */
	writeb(0, &uart->dlhr_ier);

	/* Disable UART unit function */
	writeb(~UART_FCR_UUE, &uart->iir_fcr);

	/* Set both receiver and transmitter in UART mode (not SIR) */
	writeb(~(SIRCR_RSIRE | SIRCR_TSIRE), &uart->isr);

	/*
	 * Set databits, stopbits and parity.
	 * (8-bit data, 1 stopbit, no parity)
	 */
	writeb(UART_LCR_WLEN_8 | UART_LCR_STOP_1, &uart->lcr);

	/* Set baud rate */
	local_serial_setbrg();

	/* Enable UART unit, enable and clear FIFO */
	writeb(UART_FCR_UUE | UART_FCR_FE | UART_FCR_TFLS | UART_FCR_RFLS,
			&uart->iir_fcr);

	return 0;
}

static int local_serial_init2(int baudratevalue)
{
	uart = (struct jz_uart *)(UART0_BASE + 1 * 0x1000);

	/* Disable port interrupts while changing hardware */
	writeb(0, &uart->dlhr_ier);

	/* Disable UART unit function */
	writeb(~UART_FCR_UUE, &uart->iir_fcr);

	/* Set both receiver and transmitter in UART mode (not SIR) */
	writeb(~(SIRCR_RSIRE | SIRCR_TSIRE), &uart->isr);

	/*
	 * Set databits, stopbits and parity.
	 * (8-bit data, 1 stopbit, no parity)
	 */
	writeb(UART_LCR_WLEN_8 | UART_LCR_STOP_1, &uart->lcr);

	/* Set baud rate */
	//local_serial_setbrg();
	u32 baud_div, tmp;
	switch(baudratevalue){
		case 57600:
			baud_div = CONFIG_SYS_EXTAL / 16 / 57600 ;
			break;
		case 115200:
			baud_div = CONFIG_SYS_EXTAL / 16 / 115200 ;
			break;
		case 460800:
			writel(17,0xb0033024);
			writel(0,0xb0033028);
			baud_div = 3;
			//460800
			break;
		case 921600:
			writel(13,0xb0033024);
			writel(0,0xb0033028);
			baud_div = 2;//baudrate 921600
			break;
		default:
			printf("Not support this baudrate\n");
			break;
	}


	tmp = readb(&uart->lcr);
	tmp |= UART_LCR_DLAB;
	writeb(tmp, &uart->lcr);

	writeb((baud_div >> 8) & 0xff, &uart->dlhr_ier);
	writeb(baud_div & 0xff, &uart->rbr_thr_dllr);

	tmp &= ~UART_LCR_DLAB;
	writeb(tmp, &uart->lcr);

	/* Enable UART unit, enable and clear FIFO */
	writeb(UART_FCR_UUE | UART_FCR_FE | UART_FCR_TFLS | UART_FCR_RFLS,
			&uart->iir_fcr);

	return 0;
}

static int local_serial_tstc(void)
{
	if (readb(&uart->lsr) & UART_LSR_DR)
		return 1;

	return 0;
}

static char *local_serial_gets(void)
{
	char buffer[64]={0};
	int i = 0;
	while(!local_serial_fifo_full())
		;
	while(local_serial_tstc()){
		buffer[i] = readb(&uart->rbr_thr_dllr);
		i ++;
		count ++;
	}
	return buffer;
}

int local_serial_fifo_full(void)
{
	if (readb(&uart->lsr) & UART_LSR_ORER)
		return 1;
	return 0;
}

static void local_serial_putc(const char c)
{
	/* Wait for fifo to shift out some bytes */
	while (!((readb(&uart->lsr) & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60));
	writeb((u8)c, &uart->rbr_thr_dllr);
}

static void local_serial_fifo_puts(const char *s)
{
	int i = 0;
	while(!local_serial_fifo_full()){
		writeb((u8)s[i], &uart->rbr_thr_dllr);
		i ++;
	}

	/* Wait for fifo to shift out some bytes */
	while (!((readb(&uart->lsr) & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60));
}

static void local_serial_puts(const char *s)
{
	while(*s)
		local_serial_putc(*s++);
}

static int local_serial_getc(void)
{
	while (!local_serial_tstc())
		;

	return readb(&uart->rbr_thr_dllr);
}

static void local_serial_gpio_init()
{
	gpio_set_func(GPIO_PORT_F, GPIO_FUNC_0, 3<<4);
}

static void local_serial_clk_init()
{
	unsigned int clkgr = cpm_inl(CPM_CLKGR);
	clkgr &= ~(0xfff<<12);
	cpm_outl(clkgr, CPM_CLKGR);
}

static unsigned int atoi(char *pstr)
{
	unsigned int value = 0;
	int sign = 1;
	unsigned int radix;

	if(*pstr == '-'){
		sign = -1;
		pstr++;
	}
	if(*pstr == '0' && (*(pstr+1) == 'x' || *(pstr+1) == 'X')){
		radix = 16;
		pstr += 2;
	}
	else
		radix = 10;
	while(*pstr && (*pstr != '\n')){
		if(radix == 16){
			if(*pstr >= '0' && *pstr <= '9')
				value = value * radix + *pstr - '0';
			else if(*pstr >= 'A' && *pstr <= 'F')
				value = value * radix + *pstr - 'A' + 10;
			else if(*pstr >= 'a' && *pstr <= 'f')
				value = value * radix + *pstr - 'a' + 10;
		}
		else
			value = value * radix + *pstr - '0';
		pstr++;
	}
	return value;
}

static int i2c_program_serial(struct serial_cloner *serial_cloner)
{
	int i = 0;
	struct i2c_args *i2c_arg = (struct i2c_args *)data_addr;
	struct i2c i2c;
	i2c.scl = i2c_arg->clk;
	i2c.sda = i2c_arg->data;
	i2c_init(&i2c);

	for(i=0;i<i2c_arg->value_count;i++) {
		char reg = i2c_arg->value[i] >> 16;
		unsigned char value = i2c_arg->value[i] & 0xff;
		i2c_write(&i2c,i2c_arg->device,reg,1,&value,1);
	}
	return 0;
}

#define MMC_BYTE_PER_BLOCK 512
extern ulong mmc_erase_t(struct mmc *mmc, ulong start, lbaint_t blkcnt);
static int mmc_erase(struct arguments *args)
{
	int curr_device = 0;
	struct mmc *mmc = find_mmc_device(0);
	uint32_t blk, blk_end, blk_cnt;
	uint32_t erase_cnt = 0;
	int timeout = 30000;
	int i;
	int ret;

	if (!mmc) {
		printf("no mmc device at slot %x\n", curr_device);
		return -ENODEV;
	}

	mmc_init(mmc);

	if (mmc_getwp(mmc) == 1) {
		printf("Error: card is write protected!\n");
		return -EPERM;
	}
	if (args->mmc_erase == MMC_ERASE_ALL) {
		blk = 0;
		blk_cnt = mmc->capacity / MMC_BYTE_PER_BLOCK;

		printf("MMC erase: dev # %d, start block # %d, count %u ... \n",
				curr_device, blk, blk_cnt);

		ret = mmc_erase_t(mmc, blk, blk_cnt);
		if (ret) {
			printf("mmc erase error\n");
			return ret;
		}
		ret = mmc_send_status(mmc, timeout);
		if(ret){
			printf("mmc erase error\n");
			return ret;
		}

		printf("mmc all erase ok, blocks %d\n", blk_cnt);
		return 0;
	} else if (args->mmc_erase != MMC_ERASE_PART) {
		return -EINVAL;
	}

	/*mmc part erase */
	erase_cnt = (args->mmc_erase_range_count >MMC_ERASE_CNT_MAX) ?
		MMC_ERASE_CNT_MAX : args->mmc_erase_range_count;

	for (i = 0; erase_cnt > 0; i++, erase_cnt--) {
		blk = args->mmc_erase_range[i].start / MMC_BYTE_PER_BLOCK;
		blk_end = args->mmc_erase_range[i].end / MMC_BYTE_PER_BLOCK;
		blk_cnt = blk_end - blk + 1;

		printf("MMC erase: dev # %d, start block # 0x%x, count 0x%x ... \n",
				curr_device, blk, blk_cnt);

		if ((blk % mmc->erase_grp_size) || (blk_cnt % mmc->erase_grp_size)) {
			printf("\n\nCaution! Your devices Erase group is 0x%x\n"
					"The erase block range would be change to "
					"0x" LBAF "~0x" LBAF "\n\n",
					mmc->erase_grp_size, blk & ~(mmc->erase_grp_size - 1),
					((blk + blk_cnt + mmc->erase_grp_size)
					 & ~(mmc->erase_grp_size - 1)) - 1);
		}

		ret = mmc_erase_t(mmc, blk, blk_cnt);
		if (ret) {
			printf("mmc erase error\n");
			return ret;
		}
		ret = mmc_send_status(mmc, timeout);
		if(ret){
			printf("mmc erase error\n");
			return ret;
		}

		printf("mmc part erase, part %d ok\n", i);
	}
	printf("mmc erase ok\n");
	return 0;
}

static int cloner_init_serial(struct arguments *args)
{
	if(args->use_nand_mgr) {
#ifdef CONFIG_JZ_NAND_MGR
		nand_probe_burner(&(args->PartInfo),
				&(args->nand_params[0]),
				args->nr_nand_args,
				args->nand_erase,args->offsets,args->nand_erase_count);
#endif
	}

	if (args->use_mmc) {
		if (args->mmc_erase) {
			mmc_erase(args);
		}
	}
}

static int nand_program_serial(struct serial_cloner *serial_cloner)
{
#ifdef CONFIG_JZ_NAND_MGR
	int curr_device = 0;
	u32 startaddr = atoi(ptemp[4]) + atoi(ptemp[6]);
	u32 length = atoi(ptemp[8]);
	void *databuf = (void *)data_addr;

	printf("=========++++++++++++>   NAND PROGRAM:startaddr = %d P offset = %d P length = %d \n",startaddr,length);
	do_nand_request(startaddr, databuf, length,atoi(ptemp[6]));

	return 0;
#else
	return -ENODEV;
#endif
}

static int mmc_program_serial(char *data,int mmc_index)
{
#define MMC_BYTE_PER_BLOCK 512
	int curr_device = 0;
	write_back_chk = 1;
	struct mmc *mmc = find_mmc_device(mmc_index);
	u32 blk = (atoi(ptemp[4]) + atoi(ptemp[6]))/MMC_BYTE_PER_BLOCK;
	u32 cnt = (atoi(ptemp[8]) + MMC_BYTE_PER_BLOCK - 1)/MMC_BYTE_PER_BLOCK;
	void *addr = (void *)data;
	u32 n;

	if (!mmc) {
		printf("no mmc device at slot %x\n", curr_device);
		return -ENODEV;
	}

	//debug_cond(BURNNER_DEBUG,"\nMMC write: dev # %d, block # %d, count %d ... ",
	printf("MMC write: dev # %d, block # %d, count %d ... ",
			curr_device, blk, cnt);

	mmc_init(mmc);
	if (mmc_getwp(mmc) == 1) {
		printf("Error: card is write protected!\n");
		return -EPERM;
	}
	n = mmc->block_dev.block_write(curr_device, blk,
			cnt, addr);
	//debug_cond(BURNNER_DEBUG,"%d blocks write: %s\n",n, (n == cnt) ? "OK" : "ERROR");
	printf("%d blocks write: %s\n",n, (n == cnt) ? "OK" : "ERROR");

	if (n != cnt)
		return -EIO;

	if(write_back_chk){
		mmc->block_dev.block_read(curr_device, blk,
				cnt, addr);
		debug_cond(BURNNER_DEBUG,"%d blocks read: %s\n",n, (n == cnt) ? "OK" : "ERROR");
		if (n != cnt)
			return -EIO;

		uint32_t tmp_crc = local_crc32(0xffffffff,addr,atoi(ptemp[8]));
		debug_cond(BURNNER_DEBUG,"%d blocks check: %s\n",n,atoi(ptemp[12] == tmp_crc) ? "OK" : "ERROR");
		if (atoi(ptemp[12]) != tmp_crc) {
			printf("src_crc32 = %u , dst_crc32 = %u\n",atoi(ptemp[12]),tmp_crc);
			return -EIO;
		}
	}
	return 0;
}

static int efuse_program_serial(struct serial_cloner *serial_cloner)
{
	static int enabled = 0;
	if(!enabled) {
		efuse_init(serial_cloner->args->efuse_gpio);
		enabled = 1;
	}
	u32 partation = atoi(ptemp[4]);
	u32 length = atoi(ptemp[8]);
	void *addr = (void *)data_addr;
	u32 r = 0;

	if (r = efuse_write(addr, length, partation)) {
		printf("efuse write error\n");
		return r;
	}
	return r;
}

static int serial_program(char *data,int *success)
{
	int ack;
retry:	ack = mmc_program_serial(data,0);
	if(ack == 0){
		printf(stateok);
		*success = 1;
		return;
	}else{
		printf(stateretry);
		*success = 0;
	}
	return ack;
}

static int analysis_cmd(char *strcmd,int *type)
{
	char *temp = ":,";
	char *p;
	char *cmd_temp[16];
	int index = 0,i;
	unsigned int length;
	for(i = 0; i < sizeof(ptemp); i ++)
		memset(ptemp[i],0,sizeof(ptemp[i]));
	strtok(strcmd,temp);
	while((p = strtok(NULL, temp))){
		cmd_temp[index] = p;
		strcpy(ptemp[index] , p);
		index ++;
	}
	count = index;
	length = atoi(cmd_temp[8]);
	if(!strcmp(ptemp[0],"write")){
		*type = 0;
	}else if(!strcmp(ptemp[0],"args")){
		*type = 1;
	}else if(!strncmp(ptemp[0],"init",4)){
		*type = 4;
	}else if(!strcmp(ptemp[0],"time")){
		*type = 5;
	}else if(!strcmp(ptemp[0],"reset")){
		*type = 7;
	}else if(!strcmp(ptemp[0],"baudrate")){
		*type = 8;
	}else{
		*type = 6;
	}
	return length;
}

static int analysis(char *strcmd,int *hindex,unsigned int *hcrc,int *hlength)
{
	char temp[] = ":,\n";
	char *p;
	char *cmd_temp[32];
	char digital[16][16];
	int index = 0,i;
	p = strtok(strcmd,temp);
	while((p = strtok(NULL, temp))){
		cmd_temp[index] = p;
		index ++;
	}
	*hcrc = atoi(cmd_temp[0]);
	*hindex = atoi(cmd_temp[2]);
	*hlength = atoi(cmd_temp[4]);
	return 0;
}

static int local_serial_receive_data(char *data,int length,int timeout)
{
	int index_data = 0;
	while(index_data < length){
		while(local_serial_tstc())
			data[index_data ++] = readb(&uart->rbr_thr_dllr);
		if(timeout--  <= 0)
			break;
	}
	return length - index_data;

}

static int local_serial_receive_cmd(char *cmd,int timeout)
{
	int index_cmd = 0;
	while(1){
		while(local_serial_tstc())
			cmd[index_cmd ++] = readb(&uart->rbr_thr_dllr);
		if(cmd[0] == 'g'){
			if(index_cmd == 24)
				break;
		}else{
			if(cmd[index_cmd - 1] == '\n')
				break;
		}
		if(timeout--  <= 0)
			return -1;
	}
	return 0;
}

int start_serial_cloner()
{
	printf("start_serial_cloner\n");
	int success = 0;
	int handle_type,data_count = 0;
	int data_len = 0;
	char cmd[1024];
	char data[1024*1024];
	char cmd_data[128];
	unsigned int crc,true_crc;
	struct rtc_time rtc1;
	int transfer_size;
	int htype,hlength,hindex;
	unsigned int hcrc;

	struct arguments *argument = (struct arguments *)malloc(8192);
	local_serial_gpio_init();
	local_serial_clk_init();
	local_serial_init();

	printf("uart3 is ready\n");

	volatile u8 *uart_fcr = (volatile u8 *)(UART3_FCR);

	int cmd_timout = 2000000;
	int baudvalue;
	while(1){
		printf("Running .......\n");
receive_cmd:	memset(cmd,0,sizeof(cmd));
		memset(data,0,sizeof(data));
		if(local_serial_receive_cmd(cmd,cmd_timout) == -1){
			goto receive_cmd;
		}
		printf("cmd:%s\n",cmd);
		if(cmd[0] == 'g'){
			crc = cmd[23];
			if(t10_crc7(0, (u8 *)cmd, 23) != crc){
				*uart_fcr = ~UARTFCR_UUE;
				*uart_fcr = UARTFCR_TFLS | UARTFCR_RFLS;
				*uart_fcr = UARTFCR_UUE | UARTFCR_FE;
			}else{
				printf("boot\n");
				local_serial_puts("BOOT47XX\n");printf("BOOT47XX\n");
			}
			goto receive_cmd;
		}

		data_len = analysis_cmd(cmd,&handle_type);
		printf("data_len:%d\n",data_len);

		switch(handle_type){
			case WRITE:
				transfer_size = atoi(ptemp[14]);
				printf("transfer_size:%d\n",transfer_size);
				if(data_len % transfer_size == 0){
					data_count = data_len / transfer_size;
				}else{
					data_count = (data_len / transfer_size) + 1;
				}

				local_serial_puts(stateok);
			recmd:	memset(cmd_data,0,sizeof(cmd_data));
				if(local_serial_receive_cmd(cmd_data,10000000) == -1){
					local_serial_puts(stateretry);
					goto recmd;
				}
				printf("cmd_data:%s\n",cmd_data);
				analysis(cmd_data,&hindex,&hcrc,&hlength);
				printf("hcrc:%u   hindex:%d    hlength:%d\n",hcrc,hindex,hlength);
				local_serial_puts(stateok);
			redata:	if(!local_serial_receive_data(data + hindex * transfer_size,hlength,20000000)){
					crc = local_crc32(0xffffffff,data + hindex* transfer_size ,hlength);
					if(crc == hcrc){
						local_serial_puts(stateok);
					}else{
						printf("dst crc:%u,src crc:%u\n",crc,hcrc);
						printf(stateretry);
						memset(data+hindex * transfer_size,0,hlength);
						local_serial_puts(stateretry);
					}
				}else{
					local_serial_puts(stateretry);
					goto redata;
				}
				if(hindex == data_count - 1){
				program1:serial_program(data,&success);
					if(success==1){
						local_serial_puts(stateok);
						success = 0;
					}else{
						local_serial_puts(stateretry);
					}
				}else{
					goto recmd;
				}
				break;
			case ARGS:
				local_serial_puts(stateok);
				if(!local_serial_receive_data(data,data_len,20000000)){
					crc = local_crc32(0xffffffff,data,data_len);
					if(crc == atoi(ptemp[12])){
						memcpy(argument,data,sizeof(struct arguments));
						local_serial_puts(stateok);
						printf(stateok);
					}else{
						printf("crc error\n");
						local_serial_puts(stateretry);
					}
				}else{
					printf("chaoshi\n");
					local_serial_puts(stateretry);
				}
				break;
			case RUN:
				local_serial_puts(stateok);
				break;
			case INIT:
				printf("mgr:%d mmc:%d\n",argument->use_nand_mgr,argument->use_mmc);
				if(!cloner_init_serial(argument)){
					memset(data,0,sizeof(data));
					local_serial_puts(stateok);
				}
				break;
			case TIME:
				if(!local_serial_receive_data(data,data_len,20000000)){
					crc = local_crc32(0xffffffff,data,data_len);
					if(crc == atoi(ptemp[12])){
						rtc1 = *((struct rtc_time *)data);
						if(rtc_set(&rtc1)){
							local_serial_puts(stateok);
						}else{
							local_serial_puts(stateretry);
						}

					}else{
						printf("crc error\n");
						local_serial_puts(stateretry);
					}
				}else{
					printf("chaoshi\n");
					local_serial_puts(stateretry);
				}
				break;
			case RESET:
				local_serial_puts(stateok);
				do_reset(NULL,0,0,NULL);
				break;
			case BAUDRATE:
				printf("baudrate changed\n");
				baudvalue = atoi(ptemp[2]);
				local_serial_puts(stateok);
				local_serial_gpio_init();
				local_serial_clk_init();
				local_serial_init2(baudvalue);
				break;
			default:
				printf("Not support this command\n");
				goto receive_cmd;
				break;
		}
	}
}
