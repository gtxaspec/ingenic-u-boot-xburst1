#include <common.h>
#include <jz_aes.h>


typedef unsigned int uint32_t;

/* Reverse the bytes in a 32-bit value */
#define BLSWAP32(val) \
    ((val) = ((uint32_t)((((uint32_t)(val) & (uint32_t)0x000000ffU) << 24) | \
          (((uint32_t)(val) & (uint32_t)0x0000ff00U) <<  8) | \
          (((uint32_t)(val) & (uint32_t)0x00ff0000U) >>  8) | \
          (((uint32_t)(val) & (uint32_t)0xff000000U) >> 24))))

#define	CPM_BASE	0xb0000000
#define AES_BASE    0xb3430000

#define CPM_CLKGR1_OFFSET       (0x28)  /* T40 T41 T32, rw, 32*/
#define CLKGR1_AES			    (1<<1)

#define ASCR         (0x0)
#define ASSR         (0x4)
#define ASINTM       (0x8)
#define ASSA         (0xc)
#if defined(CONFIG_PRJ007)
    #define ASDA         (0x10)
    #define ASTC         (0x14)
    #define ASDI         (0x18)
    #define ASDO         (0x1c)
    #define ASKY         (0x20)
    #define ASIV         (0x24)
#elif defined(CONFIG_PRJ008)
    #define ASDA         (0x14)
    #define ASDTC        (0x18)
    #define ASBNUM       (0x1c)
    #define ASDI         (0x20)
    #define ASDO         (0x24)
    #define ASKY         (0x28)
    #define ASIV         (0x2c)
#endif


#define ASCR_EN         (1 << 0)
#define AES_ASSR_KEYD	(1 << 0)

#define aes_writel(val, off)	(*(volatile unsigned int *)(AES_BASE + off) = val)
#define aes_readl(off)		*(volatile unsigned int *)(AES_BASE + off)

#define REG32(addr)	*((volatile unsigned int *)(addr))

#define CPM_CLKGR1       	(CPM_BASE + CPM_CLKGR1_OFFSET)		/* T40 T41 T32*/

#define REG_CPM_CLKGR1          REG32(CPM_CLKGR1)

#define __cpm_start_aes()  (REG_CPM_CLKGR1 &= ~CLKGR1_AES)
#define __cpm_stop_aes()   (REG_CPM_CLKGR1 |= CLKGR1_AES)


static int jz_aes_init(uint32_t *aesiv,uint32_t *aeskey,ModeType mode,BitType len)
{
	__cpm_start_aes();

    int i,key_words;
	aes_writel(aes_readl(ASSR),ASSR);
    #if defined(CONFIG_PRJ007)
    aes_writel(0x3,ASINTM);
	#elif defined(CONFIG_PRJ008)
    aes_writel(0x1,ASINTM);
    #endif
	if(len == AES_128){
		aes_writel((aes_readl(ASCR) & ~(0x3 << 6)) | 3 << 28 | 1<<10 | 1 << 0, ASCR);
		key_words = 4;
	}
	else if(len == AES_192){
		aes_writel((aes_readl(ASCR) & ~(0x3 << 6)) | (0x1 << 6) | 3 << 28 | 1<<10 | 1 << 0, ASCR);
		key_words = 6;
	}
	else{
		aes_writel((aes_readl(ASCR) & ~(0x3 << 6)) | (0x2 << 6) | 3 << 28 | 1<<10 | 1 << 0, ASCR);
		key_words = 8;
	}

    if((mode == ENC_ECB ) || (mode == DEC_ECB)){
        if(mode == DEC_ECB){
            /* Enable Decrypts, ECB mode */
            aes_writel(1<<4 |aes_readl(ASCR),ASCR);
        }
    }else{
        if(mode == ENC_CBC){
            /* Enable Encrypts, CBC mode */
            aes_writel(1<<5 | aes_readl(ASCR),ASCR);
        }else{
            /* Enable Decrypts, CBC mode */
            aes_writel(1<<5 | 1<<4 | aes_readl(ASCR),ASCR);
        }
        for(i = 0; i < 4; i++)
            aes_writel(aesiv[i],ASIV);

        /* write IV init */
        aes_writel(aes_readl(ASCR) | 1<<1,ASCR);
    }


    for(i = 0; i < key_words; i++)
	{
        aes_writel(aeskey[i],ASKY);
    }
    /* write KEYS start */
    aes_writel(aes_readl(ASCR) | 1<<2,ASCR);

    #if defined(CONFIG_PRJ007)
    while(!(aes_readl(ASSR) & (1<<0)));
    aes_writel(1,ASSR);
    #endif

	return 0;
}

static int jz_aes_deinit(void)
{
	__cpm_stop_aes();
	return 0;
}

static int jz_aes_update(uint32_t *data_in, uint32_t len,uint32_t *data_out)
{
    uint32_t i;
    uint32_t temp;
    int datain_align = 0 ,dataout_align = 0;   //四字节对齐

    if(((uintptr_t)data_in % 4) != 0){
        datain_align = 1;
        printf("%s:%s: data_in is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, data_in);
    }
    if(((uintptr_t)data_out % 4) != 0){
        dataout_align = 1;
        printf("%s:%s: data_out is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, data_out);
    }

    for (i = 0; i < len; i++) {
        if(datain_align){
		    temp = 0;
            temp |= ((uint8_t *)data_in)[i * 4 + 0] << 24;
            temp |= ((uint8_t *)data_in)[i * 4 + 1] << 16;
            temp |= ((uint8_t *)data_in)[i * 4 + 2] << 8;
            temp |= ((uint8_t *)data_in)[i * 4 + 3];
		    aes_writel(temp, ASDI);
	    }else{
		    aes_writel(BLSWAP32(((uint32_t*)data_in)[i]),ASDI);
	    }

        if ((i + 1) % 4 == 0) {
            aes_writel(aes_readl(ASCR) | (1 << 3), ASCR);

			#if defined(CONFIG_PRJ007)
				while (!(aes_readl(ASSR) & (1 << 1)));
				aes_writel(aes_readl(ASSR) | (1 << 1), ASSR);
			#elif defined(CONFIG_PRJ008)
				while (!(aes_readl(ASSR) & (1 << 0)));
				aes_writel(aes_readl(ASSR) | (1 << 0), ASSR);
			#endif

            if(dataout_align){
                temp = aes_readl(ASDO);
                ((uint8_t *)data_out)[(i - 3) * 4 + 0] = (temp >> 24) & 0xFF;
                ((uint8_t *)data_out)[(i - 3) * 4 + 1] = (temp >> 16) & 0xFF;
                ((uint8_t *)data_out)[(i - 3) * 4 + 2] = (temp >> 8) & 0xFF;
                ((uint8_t *)data_out)[(i - 3) * 4 + 3] = temp & 0xFF;

                temp = aes_readl(ASDO);
                ((uint8_t *)data_out)[(i - 2) * 4 + 0] = (temp >> 24) & 0xFF;
                ((uint8_t *)data_out)[(i - 2) * 4 + 1] = (temp >> 16) & 0xFF;
                ((uint8_t *)data_out)[(i - 2) * 4 + 2] = (temp >> 8) & 0xFF;
                ((uint8_t *)data_out)[(i - 2) * 4 + 3] = temp & 0xFF;

                temp = aes_readl(ASDO);
                ((uint8_t *)data_out)[(i - 1) * 4 + 0] = (temp >> 24) & 0xFF;
                ((uint8_t *)data_out)[(i - 1) * 4 + 1] = (temp >> 16) & 0xFF;
                ((uint8_t *)data_out)[(i - 1) * 4 + 2] = (temp >> 8) & 0xFF;
                ((uint8_t *)data_out)[(i - 1) * 4 + 3] = temp & 0xFF;

                temp = aes_readl(ASDO);
                ((uint8_t *)data_out)[i * 4 + 0] = (temp >> 24) & 0xFF;
                ((uint8_t *)data_out)[i * 4 + 1] = (temp >> 16) & 0xFF;
                ((uint8_t *)data_out)[i * 4 + 2] = (temp >> 8) & 0xFF;
                ((uint8_t *)data_out)[i * 4 + 3] = temp & 0xFF;
            }else{
                data_out[i-3] = aes_readl(ASDO);
                data_out[i-2] = aes_readl(ASDO);
                data_out[i-1] = aes_readl(ASDO);
                data_out[i]   = aes_readl(ASDO);
            }
        }
    }
    return 0;
}

static int jz_aes_final(void)
{
    aes_writel(aes_readl(ASCR) & 0<<10,ASCR);

    return 0;
}
static struct jz_aes_device _aes_device = {
	.init_aes = jz_aes_init,
	.deinit_aes = jz_aes_deinit,
    .aes_update = jz_aes_update,
    .aes_final = jz_aes_final,
};

struct jz_aes_device* get_jz_aes(void)
{
	return &_aes_device;
}
