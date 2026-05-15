#include "jz_rsa_PRJ007.h"
#include <stdint.h>


typedef unsigned int uint32_t;

/* Reverse the bytes in a 32-bit value */
#define BLSWAP32(val) \
    ((val) = ((uint32_t)((((uint32_t)(val) & (uint32_t)0x000000ffU) << 24) | \
          (((uint32_t)(val) & (uint32_t)0x0000ff00U) <<  8) | \
          (((uint32_t)(val) & (uint32_t)0x00ff0000U) >>  8) | \
          (((uint32_t)(val) & (uint32_t)0xff000000U) >> 24))))

#define	CPM_BASE	0xb0000000

#define RSA_BASE        0xb34C0000 

#define CPM_CLKGR0_OFFSET       (0x20)  /* T40 T41 T32, rw, 32*/
#define RSACDDR_OFFSET			(0x4c)
#define CLKGR0_RSA			    (1<<23)

#define RSAC            (0x0)
#define RSAE            (0x4)
#define RSAN            (0x8)
#define RSAM            (0xc)
#define RSAP            (0x10)

#define RSAC_RSA_INT_M  (1 << 17)
#define RSAC_PER_INT_M  (1 << 16)
#define RSAC_RSA_2048   (1 << 7)
#define RSAC_RSAC       (1 << 6)
#define RSAC_RSAD       (1 << 5)
#define RSAC_RSAS       (1 << 4)
#define RSAC_PERC       (1 << 3)
#define RSAC_PERD       (1 << 2)
#define RSAC_PERS       (1 << 1)
#define RSAC_EN         (1 << 0)

#define rsa_writel(val, off)	(*(volatile unsigned int *)(RSA_BASE + off) = val)
#define rsa_readl(off)		*(volatile unsigned int *)(RSA_BASE + off)

#define REG32(addr)	*((volatile unsigned int *)(addr))

#define CPM_CLKGR0       	(CPM_BASE + CPM_CLKGR0_OFFSET)		/* T40 T41*/
#define RSACDDR				(CPM_BASE + RSACDDR_OFFSET)

#define REG_CPM_CLKGR0          REG32(CPM_CLKGR0)

#define __rsa_clk()		   (REG32(RSACDDR) = 0x60000003)
#define __cpm_start_rsa()  (REG_CPM_CLKGR0 &= ~CLKGR0_RSA)
#define __cpm_stop_rsa()   (REG_CPM_CLKGR0 |= CLKGR0_RSA)

static void rsa_perpare_key(unsigned int *n, unsigned int e, unsigned int klen)
{
	unsigned int i;
	unsigned int tmp;

	/* sel rsa bit len */
	tmp = rsa_readl(RSAC);

	if(klen == 32)
		tmp &= ~RSAC_RSA_2048;
	else
		tmp |= RSAC_RSA_2048;
	rsa_writel(tmp, RSAC);

	/* set rsa E-key，先写全0，最后一个word写入e值 */
	for(i = 0; i < 63; i++)
		rsa_writel( 0, RSAE);
	rsa_writel(e, RSAE);

	/* set rsa N-key */
	for(i = 0; i < klen; i++)
		rsa_writel(n[i], RSAN);

	/* set RSAC.PRES */
	rsa_writel(rsa_readl( RSAC) | RSAC_PERS, RSAC);

	/* polling perd, clear pers */
	while(!(rsa_readl( RSAC) & RSAC_PERD));
	rsa_writel(rsa_readl( RSAC) & ~RSAC_PERS, RSAC);

	/* per clear */
	rsa_writel(rsa_readl( RSAC) | RSAC_PERC, RSAC);
	rsa_writel(rsa_readl( RSAC) & ~RSAC_PERC, RSAC);
}

static unsigned int total_dlen = 0;
static void rsa_do_crypt(unsigned int *id,unsigned int dlen)
{
    unsigned int i;

	/* set rsa MESSAGE */
	for(i = 0; i < dlen; i++)
		rsa_writel(id[i], RSAM);
    
    total_dlen = total_dlen + dlen;
}

static int jz_rsa_init()
{
	__cpm_start_rsa();
	__rsa_clk();
	return 0;
}

static int jz_rsa_deinit()
{
	__cpm_stop_rsa();
	return 0;
}

static void jz_rsa_enable()
{
	rsa_writel(rsa_readl(RSAC) | RSAC_EN, RSAC);
}

static void jz_rsa_disable()
{
	rsa_writel(rsa_readl(RSAC) & ~RSAC_EN, RSAC);
}

static int jz_rsa_2048_init(unsigned int *n, unsigned int e,unsigned int klen)
{
    int data_n = 0;
    if(klen  * sizeof(int) * 8 != 2048) {
        return -1;
    }
    if(((uintptr_t)n % 4) != 0){
        data_n = 1;
        printf("%s:%s: data_n is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, n);
    }
    if(data_n){
        unsigned int buf[64] __attribute__((aligned(sizeof(int))));
        if (klen > 64){
            printf("%s:%s: Invalid length: too long\n",__FILE__, __FUNCTION__);
            return -1;
        }
        memmove(buf, n, klen * sizeof(unsigned int));

        jz_rsa_enable();
        rsa_perpare_key(buf, e, klen);
    }else{
        jz_rsa_enable();
        rsa_perpare_key(n, e, klen);
    }
    
    return 0;
}

static int jz_rsa_2048_update(unsigned int *id, unsigned int len)
{
    int data_id = 0;
    if(((uintptr_t)id % 4) != 0){
        data_id = 1;
        printf("%s:%s: data_id is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, id);
    }

    if(data_id){
         unsigned int buf[64] __attribute__((aligned(sizeof(int))));
         if((len << 2) > sizeof(buf)){
             printf("Invalid length: too long\n");
             return -1;
         }
         memmove(buf, id, len << 2);
         rsa_do_crypt(buf, len);
    }else{
        rsa_do_crypt(id, len);
    }

    return 0;
}

static int jz_rsa_2048_final(unsigned int *od)
{
    int i;
    int data_od = 0;
    if(((uintptr_t)od % 4) != 0){
        data_od = 1;
        printf("%s:%s: data_od is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, od); 
    }
   /* set RSAC.RSAS */
   rsa_writel(rsa_readl(RSAC) | RSAC_RSAS, RSAC);

   /* polling rsad, clear rsas */
   while(!(rsa_readl(RSAC) & RSAC_RSAD));
   rsa_writel(rsa_readl(RSAC) & ~RSAC_RSAS, RSAC);

   /* read output data, 输出数据 word 大小端为反 */
    if(data_od){
        unsigned int buf[64] __attribute__((aligned(sizeof(int))));
        if (total_dlen > 64){
            printf("%s:%s: Invalid length: too long\n",__FILE__, __FUNCTION__);
            return -1;
        }
        for(i = 0; i < total_dlen; i++) {
            buf[total_dlen-1-i] = rsa_readl(RSAP);
        }
        memmove(od,buf, total_dlen * sizeof(unsigned int));
    }else{
        for(i = 0; i < total_dlen; i++) {
            od[total_dlen - 1 - i] = rsa_readl( RSAP); 
        }
    }

   /* rsa clear */
   rsa_writel(rsa_readl(RSAC) | RSAC_RSAC, RSAC);
   rsa_writel(rsa_readl(RSAC) & ~RSAC_RSAC, RSAC);

   total_dlen = 0;
    jz_rsa_disable();
    return 0;
}
static struct jz_rsa_device _rsa_device = {
	.init_rsa = jz_rsa_init,
	.deinit_rsa = jz_rsa_deinit,
    .rsa_2048_init = jz_rsa_2048_init,
    .rsa_2048_update = jz_rsa_2048_update,
    .rsa_2048_final = jz_rsa_2048_final,
};

struct jz_rsa_device* get_jz_rsa(void)
{
	return &_rsa_device;
}
