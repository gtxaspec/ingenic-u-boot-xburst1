#ifndef __JZ_AES_H__
#define __JZ_AES_H__

typedef unsigned int uint32_t;

#define	CPM_BASE	0xb0000000
#define AES_BASE    0xb3430000

#define CPM_CLKGR1_OFFSET       (0x28)
#define CLKGR1_AES			    (1<<1)

#define ASCR         (0x0)
#define ASSR         (0x4)
#define ASINTM       (0x8)
#define ASSA         (0xC)
#define ASSTC        (0x10)
#define ASDA         (0x14)
#define ASDTC        (0x18)
#define ASBNUM       (0x1C)
#define ASDI         (0x20)
#define ASDO         (0x24)
#define ASKY         (0x28)
#define ASIV         (0x2C)

#define ASCR_EN         (1 << 0)

#define aes_writel(val, off)	(*(volatile unsigned int *)(AES_BASE + off) = val)
#define aes_readl(off)		*(volatile unsigned int *)(AES_BASE + off)

#define REG32(addr)	*((volatile unsigned int *)(addr))

#define CPM_CLKGR1       	(CPM_BASE + CPM_CLKGR1_OFFSET)
#define REG_CPM_CLKGR1          REG32(CPM_CLKGR1)

#define __cpm_start_aes()  (REG_CPM_CLKGR1 &= ~CLKGR1_AES)
#define __cpm_stop_aes()   (REG_CPM_CLKGR1 |= CLKGR1_AES)

typedef enum {
    ENC_ECB ,
    DEC_ECB ,
    ENC_CBC ,
    DEC_CBC ,
} ModeType;

typedef enum {
	KEY_SIZE_128 ,
	KEY_SIZE_256 ,
} KEY_SIZE;

void aes_efuse_start(uint32_t data_in, uint32_t len,uint32_t data_out);
void aes_efuse_init(uint32_t *aesiv,ModeType mode,KEY_SIZE key_size);
uint32_t get_aes_header_length_and_iv(uint32_t img_phys_addr, uint32_t *iv_out);

#endif
