#include <common.h>
#include <asm/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/arch/base.h>
#include <jz_efuse_PRJ008.h>

//在只读应用场景中，应该弱化寄存器的控制，不参与读,在debug时，使用寄存器
#define EFUCTRL      (0x0)
#define EFUCFG       (0x4)
#define EFUSTATE     (0x8)
#define EFUDATA      (0xc)
#define EFUSPEEN     (0x10)
#define EFUSPESEG    (0x14)

#define MIPS_KSEG1_OFFSET (0xA0000000)
#define EFUSE_BASE (0x13540000)
#define EFUSEDATA_OFFSET (0x200)
#define READ_EFUSE_ADDR (MIPS_KSEG1_OFFSET+EFUSE_BASE+EFUSEDATA_OFFSET)

#define efuse_read_reg(addr) *(volatile unsigned int *)(MIPS_KSEG1_OFFSET + EFUSE_BASE + addr)
// #define writel(addr, off, val) (*(volatile unsigned int *)(addr + off) = val)



// #define OTP_DEBUG

uint32_t seg_addr[] = {
	CHIP_ID_ADDR,
	PROT_ON_ADDR,
	SPEC_ADDR,
	PROT_BAK_ADDR,
	TSENS_ADDR,
	RETEN_ADDR,
	SCB_DATA_ADDR,
	UKEY_ADDR,
	SCB_BAK_ADDR,
};

// #ifdef OTP_DEBUG
#define MAX_BUF_SIZE (128)
struct ReadCfg
{
	const char *msg;
	enum segment_id seg_id;
	enum segment_size length;
};

struct ReadCfg readcfgs[] = {
	{"chip id", CHIP_ID, CHIP_ID_SIZE},
	{"prot on", PROT_ON, PROT_ON_SIZE},
	{"spec", SPEC, SPEC_SIZE},
	{"prot bak", PROT_BAK, PROT_BAK_SIZE},
	{"tsens", TSENS, TSENS_SIZE},
	{"reten", RETEN, RETEN_SIZE},
	{"scb data", SCB_DATA, SCB_DATA_SIZE},
	{"ukey", UKEY, UKEY_SIZE},
	{"scb bak", SCB_BAK, SCB_BAK_SIZE},
};

// unsigned int efuse_readl(unsigned int addr)
// {
// 	unsigned int val = *(volatile unsigned int *)(READ_EFUSE_ADDR + addr);
// 	//大小端转换
// 	val = (val << 24) | ((val << 8) & 0xff0000) | ((val >> 8) & 0xff00) | (val >> 24);
// 	// printf("efuse_readl: addr 0x%08x val 0x%08x\n", addr, val);
// 	return val;
// }


unsigned char efuse_readb(unsigned int addr)
{
    unsigned int aligned_addr = addr & ~0x3;  // 4字节对齐地址
    unsigned int offset = addr & 0x3;         // 计算字节偏移
    unsigned int val;

    // 如果地址已经4字节对齐且后续3字节也在同一word内，直接读取
    if (offset == 0) {
        val = *(volatile unsigned int *)(READ_EFUSE_ADDR + aligned_addr);
        // 大小端转换 - 只提取需要的字节
        val = (val << 24) | ((val << 8) & 0xff0000) | ((val >> 8) & 0xff00) | (val >> 24);
        return val & 0xFF;
    }

    // 如果地址不对齐，读取4字节然后提取对应字节
    val = *(volatile unsigned int *)(READ_EFUSE_ADDR + aligned_addr);
    // 大小端转换
    val = (val << 24) | ((val << 8) & 0xff0000) | ((val >> 8) & 0xff00) | (val >> 24);

    // 根据偏移提取对应字节
    return (val >> (offset * 8)) & 0xFF;
}

static void dump_reg()
{
	printf("EFUCTRL: 0x%x\n", efuse_read_reg(EFUCTRL));
	printf("EFUCFG: 0x%x\n", efuse_read_reg(EFUCFG));
	printf("EFUSTATE: 0x%x\n", efuse_read_reg(EFUSTATE));
	printf("EFUDATA: 0x%x\n", efuse_read_reg(EFUDATA));
	printf("EFUSPEEN: 0x%x\n", efuse_read_reg(EFUSPEEN));
	printf("EFUSPESEG: 0x%x\n", efuse_read_reg(EFUSPESEG));
}

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

void otp_info_show(void)
{
	dump_reg();
	printf("=======================================================\n");
	int i = 0;
	for (; i < sizeof(readcfgs) / sizeof(struct ReadCfg); i++) {
		unsigned char buf[MAX_BUF_SIZE];
		read_efuse(readcfgs[i].seg_id, buf, readcfgs[i].length);
		show_hex(buf, readcfgs[i].length, readcfgs[i].msg);
	}
	printf("=======================================================\n");
}
// #endif

void read_efuse(enum segment_id seg_id, unsigned char *buf, int len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		unsigned char val_b = efuse_readb(seg_addr[seg_id]+i);
		// printf("addr 0x%x read byte 0x%02x\n", seg_addr[seg_id]+i, val_b);
		buf[i] = val_b;
	}

}
