#include "jz_hash.h"
// #include <stdint.h>
#include <common.h>
typedef unsigned int uint32_t;
typedef unsigned int u32;


/* Reverse the bytes in a 32-bit value */
#define BLSWAP32(val) \
    ((val) = ((uint32_t)((((uint32_t)(val) & (uint32_t)0x000000ffU) << 24) | \
          (((uint32_t)(val) & (uint32_t)0x0000ff00U) <<  8) | \
          (((uint32_t)(val) & (uint32_t)0x00ff0000U) >>  8) | \
          (((uint32_t)(val) & (uint32_t)0xff000000U) >> 24))))


#define HASH_BASE		0xb3480000

#define HASH_REG_HSCR		0x0
#define HASH_REG_HSSR		0x4
#define HASH_REG_HSINTM		0x8
#define HASH_REG_HSSA		0xc
#define HASH_REG_HSTC		0x10
#if defined(CONFIG_PRJ008)
#define HASH_REG_HSDI		0x18
#define HASH_REG_HSDO		0x1c
#define HASH_REG_HSCG       0x20
#elif defined(CONFIG_PRJ007)
#define HASH_REG_HSDI		0x14
#define HASH_REG_HSDO		0x18
#define HASH_REG_HSCG       0x1c
#endif

#define HASH_DONE		(0x1 << 0)
#define HASH_DMADONE		(0x1 << 1)

#define HASH_SELECT_MD5		0x0
#define HASH_SELECT_SHA1	0x1
#define HASH_SELECT_SHA256  0x3

#define HSCR_EN_SFT_BIT		0
#define HSCR_SEL_SFT_BIT	1
#define HSCR_INIT_SFT_BIT	4
#define HSCR_DMAE_SFT_BIT	5
#define HSCR_DMAS_SFT_BIT	6
#define HSCR_DIRVS_SFT_BIT	7
#define HSCR_DORVS_SFT_BIT	8

#define CPM_CLKGR0_OFFSET       (0x20)  /* T31/T40/T41/T32, rw, 32, 0x1FFFFF80 */
#define hash_writel(off, val)	(*(volatile unsigned int *)(HASH_BASE + off) = val)
#define hash_readl(off)		*(volatile unsigned int *)(HASH_BASE + off)

#define HASH_NEWROUND		(0x1 << 16)
#define HASH_ENDROUND		(0x1 << 18)
#define HASH_DMAMODE		(0x1 << 17)

#define HASH_BLOCK_BYTELEN	64
#define HASH_BLOCK_WORDLEN	16
#define HASH_PENDING_BYTELEN	8
#define	CPM_BASE	0xb0000000

#define CPM_CLKGR0       	(CPM_BASE + CPM_CLKGR0_OFFSET)		/* T31/T40 */
#define REG32(addr)	*((volatile unsigned int *)(addr))
#define REG_CPM_CLKGR0          REG32(CPM_CLKGR0)

//T41#define CLKGR0_SC_HASH      (1 << 2)
//T40#define CLKGR0_SC_HASH      (1 << 19)
//T31#define CLKGR0_SC_HASH      (1 << 18)
#define CLKGR0_SC_HASH      (1 << 15)  /*  T32  */

#define __cpm_start_hash()  (REG_CPM_CLKGR0 &= ~CLKGR0_SC_HASH)
#define __cpm_stop_hash()   (REG_CPM_CLKGR0 |= CLKGR0_SC_HASH)

static int hash_init(void)
{
   __cpm_start_hash();                                                                                      
  hash_writel(HASH_REG_HSCR, 1 | 1 << 7 | 3 << 1);               
  hash_writel(HASH_REG_HSCR, hash_readl(HASH_REG_HSCR) | 1 << 4);
	
   return 0;
}

static int hash_deinit(void)
{
	__cpm_stop_hash();
	return 0;
}

static int total_size = 0;
static char rest_data[4] = {0};
static int remain_num = 0;
static int j = 0;
static int sha256_update(unsigned char *data_in, int size)
{
	int i=0, word_cnt = (remain_num+size) / 4;
    total_size = total_size + size; 
	u32 size_block = size / 64;
    unsigned int before_updata = 0 ;
    static int last_remain ;
    last_remain = remain_num;  //上次update
    remain_num = (remain_num+size) % 4 ;//本次update剩余未对齐数据长度

    if(last_remain){
        if(last_remain+size<4){
            for(i=last_remain; i< last_remain+size ; i++){
                rest_data[i] = data_in[i-last_remain] ;
            }
        }else{
            for(i=last_remain; i< 4 ; i++){
                rest_data[i] = data_in[i-last_remain] ;  
            }
        }
    }

    hash_writel(HASH_REG_HSINTM, 0x1);
	hash_writel(HASH_REG_HSSR, 1);
	hash_writel(HASH_REG_HSTC, size_block);

    if(last_remain && (last_remain+size)>=4){
        before_updata = (u32)( (rest_data[0]&0xff) + ((rest_data[1]&0xff) << 8) + ((rest_data[2]&0xff) << 16) + ((rest_data[3]&0xff) << 24 ));
        hash_writel(HASH_REG_HSDI, before_updata);
        j++;
        if (j && j % 16 == 0) {
            while(!(hash_readl(HASH_REG_HSSR) & (1<<0)));
            hash_writel(HASH_REG_HSSR, 1);
        }
        word_cnt--;
    }

    for(i = 0; i < word_cnt; i++) {
        if(last_remain){
            hash_writel(HASH_REG_HSDI, (u32)( data_in[i*4+4-last_remain] + (data_in[i*4+5-last_remain] << 8) + (data_in[i*4+6-last_remain] << 16) \
                                            + (data_in[i*4+7-last_remain] << 24)));
        }else{
            hash_writel(HASH_REG_HSDI, (u32)( data_in[i*4] + (data_in[i*4+1] << 8) + (data_in[i*4+2] << 16) + (data_in[i*4+3] << 24)));
        }
        j++;
        if (j && j % 16 == 0) {
            while(!(hash_readl(HASH_REG_HSSR) & (1<<0)));
            hash_writel(HASH_REG_HSSR, 1);
        }
    }

    if((last_remain == 0) || (last_remain && (last_remain+size)>=4)){
        if(last_remain){
            word_cnt++;
        }
        for(i=0; i< remain_num ; i++){
            rest_data[i] = data_in[4*word_cnt+i-last_remain] ;
        }
    }

    return 0;
}

static int sha256_final(u32 *data_out)
{
    int i, word_cnt = total_size / 4;
    int remainder = total_size % 4;
    u32 bit_len = (u32)total_size << 3;
    int pad_offset = 0;
    int dataout_align = 0; //四字节对齐
    BLSWAP32(bit_len);

    u32 padding = 0 ;
    if(!remainder){
        padding = 0x00000080;
    }else{
        for(i=0 ; i<remain_num ; i++){
            padding = (u32)(((rest_data[i]&0xff) << i*8) + padding );
        }
        padding = (u32)( padding + ((0x80&0xff) << remain_num*8) );
    }
    
    u32 index = total_size & 0x3f;
    u32 padding_len;
    total_size = 0;

    if (index < 56) {
        padding_len = 56 - index;
    } else {
        padding_len = 120 - index;
    }

    hash_writel(HASH_REG_HSDI, padding);

    j++;
    if (j % 16 == 0) {
        while(!(hash_readl(HASH_REG_HSSR) & (1<<0)));
        hash_writel(HASH_REG_HSSR, 1);
    }

    if(remainder) {
        pad_offset = 0;
    } else {
        pad_offset = 1;
    }

    for(i = 0; i < padding_len / 4 - pad_offset; i++) {
        hash_writel(HASH_REG_HSDI, 0);
        j++;
        if (j % 16 == 0) {
            while(!(hash_readl(HASH_REG_HSSR) & (1<<0)));
            hash_writel(HASH_REG_HSSR, 1);
        }
    }

    hash_writel(HASH_REG_HSDI, 0);
    hash_writel(HASH_REG_HSDI, bit_len);

    while(!(hash_readl(HASH_REG_HSSR) & (1<<0)));      
    hash_writel(HASH_REG_HSSR, 1);

    if(((uintptr_t)data_out % 4) != 0)
    {
        dataout_align = 1;
        printf("%s:%s: data_in is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, data_out);
    }

    if(dataout_align){
        unsigned int buf[8] __attribute__((aligned(sizeof(int))));
        for(i = 0; i < 8;i ++){
            buf[i] = hash_readl(HASH_REG_HSDO);
            BLSWAP32(buf[i]);
        }
        memmove(data_out,buf, 8 * sizeof(unsigned int));
    }else{
        for(i = 0; i < 8;i ++){
            data_out[i] = hash_readl(HASH_REG_HSDO);
            BLSWAP32(data_out[i]);
        }
    }
    
    j = 0;
    remain_num = 0;
    hash_writel(HASH_REG_HSSR,0x0);
    hash_writel(HASH_REG_HSINTM,0x0);
    hash_writel(HASH_REG_HSTC,0x0);
    hash_writel(HASH_REG_HSSA,0x0);
    hash_writel(HASH_REG_HSSR,0x01);
    hash_writel(HASH_REG_HSCG,0x0);

    hash_writel(HASH_REG_HSCR, hash_readl(HASH_REG_HSCR) | 1 << 4);/*start a new round*/    
    return 0;

}
static struct jz_hash_device _device = {
	.init_hash = hash_init,
	.deinit_hash = hash_deinit,
    .sha256_update = sha256_update,
    .sha256_final  = sha256_final,
};

struct jz_hash_device* get_hash_device()
{
	return &_device;
}

