#include <common.h>
#include <asm/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/arch/base.h>
#include <jz_efuse_PRJ007.h>

#define EFUCTRL      (0x0)
#define EFUCFG       (0x4)
#define EFUSTATE     (0x8)
#define EFUDATA      (0xc)
#define EFUSPEEN     (0x10)
#define EFUSPESEG    (0x14)

#define readl(addr, off) *(volatile unsigned int *)(addr + off)
#define writel(addr, off, val) (*(volatile unsigned int *)(addr + off) = val)

uint32_t seg_addr[] = {
	CHIP_ID_ADDR,
	USER_ID_ADDR,
	SARADC_CAL,
	TRIM_ADDR,
	PROGRAM_PROTECT_ADDR,
	CPU_ID_ADDR,
	SPECIAL_ADDR,
	CUSTOMER_RESV_ADDR,
	SCB_DATA_ADDR,
	RESERVE_DATA_ADDR,
};

#ifdef SCB_DEBUG
#define MAX_BUF_SIZE (128)
struct ReadCfg
{
	const char *msg;
	enum segment_id seg_id;
	enum segment_size length;
};

struct ReadCfg readcfgs[] = {
	{"chip id", CHIP_ID, CHIP_ID_SIZE},
	{"user id", USER_ID, USER_ID_SIZE},
	{"adc calib", ADC_CALIB, ADC_CALIB_SIZE},
	{"trim data", TRIM_DATA, TRIM_DATA_SIZE},
	{"protect id", PROTECT_ID, PROTECT_ID_SIZE},
	{"cpu id", CPU_ID, CPU_ID_SIZE},
	{"special use", SPECIAL_USE, SPECIAL_USE_SIZE},
	{"user data", USER_DATA, USER_DATA_SIZE},
	{"scb data", SCB_DATA, SCB_DATA_SIZE},
	{"reserve data", RESERVE_DATA, RESERVE_DATA_SIZE},
};

static void show_hex(unsigned char *buf, int len, const char *msg)
{
	printf("==> msg: %s, seg size: %d\n", msg, len);
	int i;
	for (i = 0; i < len; i++) {
		printf("%x ", buf[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n=======================================================\n");
}

void scb_info_show(void)
{
	printf("=======================================================\n");
	int i = 0;
	for (; i < sizeof(readcfgs) / sizeof(struct ReadCfg); i++) {
		unsigned char buf[MAX_BUF_SIZE];
		read_efuse(readcfgs[i].seg_id, buf, readcfgs[i].length);
		show_hex(buf, readcfgs[i].length, readcfgs[i].msg);
	}
	printf("=======================================================\n");
}
#endif

int read_efuse(enum segment_id seg_id, unsigned char *buf, int len)
{
	int offset = 0;
	void *cfg_addr = (void*)EFUSE_BASE;

	unsigned int i;
	unsigned char *save_buf = (unsigned char *)buf;
	uint32_t val, addr = 0, remainder, data = 0, count;
	count = len;
	addr = (seg_addr[seg_id] + offset) / 4;
	remainder = (seg_addr[seg_id] + offset) % 4;

	writel(cfg_addr, EFUSTATE, 0);
	val = addr << 21;
	writel(cfg_addr, EFUCTRL, val);
	val |= 1;
	writel(cfg_addr, EFUCTRL, val);
	while (!(readl(cfg_addr, EFUSTATE) & 1));
	data = readl(cfg_addr, EFUDATA);

	if ((count + remainder) <= 4) {
		data = data >> (8 * remainder);
		while (count) {
			*(save_buf) = data & 0xff;
			data = data >> 8;
			count--;
			save_buf++;
		}
		goto end;
	} else {
		data = data >> (8 * remainder);
		for (i = 0; i < (4 - remainder); i++) {
			*(save_buf) = data & 0xff;
			data = data >> 8;
			count--;
			save_buf++;
		}
	}

	/* Middle word reading */
again:
	if (count > 4) {
		addr++;
		writel(cfg_addr, EFUSTATE, 0);

		val = addr << 21;
		writel(cfg_addr, EFUCTRL, val);
		val |= 1;
		writel(cfg_addr, EFUCTRL, val);

		while (!(readl(cfg_addr, EFUSTATE) & 1));
		data = readl(cfg_addr, EFUDATA);

		for (i = 0; i < 4; i++) {
			*(save_buf) = data & 0xff;
			data = data >> 8;
			count--;
			save_buf++;
		}

		goto again;
	}

	/* Final word reading */
	addr++;
	writel(cfg_addr, EFUSTATE, 0);

	val = addr << 21;
	writel(cfg_addr, EFUCTRL, val);
	val |= 1;
	writel(cfg_addr, EFUCTRL, val);

	while (!(readl(cfg_addr, EFUSTATE) & 1));
	data = readl(cfg_addr, EFUDATA);

	while (count) {
		*(save_buf) = data & 0xff;
		data = data >> 8;
		count--;
		save_buf++;
	}

	writel(cfg_addr, EFUSTATE, 0);
	return 0;
end:
	return 0;
}
