#include "jz_rsa_PRJ008.h"

void jz_rsa_init()
{
	__cpm_start_rsa();
    __rsa_clk();
}
static void jz_rsa_deinit(void)
{
    __cpm_stop_rsa();
}
static void rsa_enable(void)
{
	rsa_writel(rsa_readl(RSAC) | RSAC_EN, RSAC);
}

static void rsa_disable(void)
{
	rsa_writel(rsa_readl(RSAC) & ~RSAC_EN, RSAC);
}

static void rsa_perpare_key(unsigned int *id, unsigned int *n, unsigned int e, unsigned int klen)
{
	unsigned int i;
	unsigned int tmp = 0;

	/* sel rsa bit len */
	tmp = rsa_readl(RSAC);
	tmp &= ~(RSA_N_LEN);
	rsa_writel(tmp, RSAC);

	if(klen == 32){
		tmp |= RSAC_RSA_1024;
    }else if(klen == 64){
		tmp |= RSAC_RSA_2048;
    }else{
        printf("error: klen must be 32 or 64\n");
        return;
    }

	rsa_writel(tmp, RSAC);

	/* set rsa MESSAGE */
    for(i = 0; i < (64 - klen); i++){
        rsa_writel(0, RSAM);
    }
	for(i = 0; i < klen; i++)
		rsa_writel(id[i], RSAM);

	/* set rsa E-key，先写全0，最后一个word写入e值 */
	for(i = 0; i < 63; i++)
		rsa_writel(0, RSAE);
	rsa_writel(e, RSAE);

	/* set rsa N-key */
	for(i = 0; i < klen; i++)
		rsa_writel(n[i], RSAN);

	/* wait N done */
	while(1) {
		if((rsa_readl(RSAINFO) & (RSA_N_CFG_DONE))) {
			break;
		}
	}

	/* rsa clear interrupt bit 4 */
	rsa_writel(rsa_readl(RSACLR) | (RSA_N_CFG_CLR),RSACLR);
}

static void rsa_do_crypt(unsigned int *od, unsigned int dlen)
{
	unsigned int i;
	int data_od = 0;
    if (((unsigned long)od % 4) != 0) {
        data_od = 1;
        printf("%s:%s: data_od is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, od);
	}
	/* wait process done */
	while(1) {
		if((rsa_readl(RSAINFO) & (RSA_INT_DONE))) {
			break;
		}
	}

	/* read output data */
    if(data_od){
        unsigned int buf[64] __attribute__((aligned(4)));
        if (dlen > 64){
            printf("%s:%s: Invalid length: too long\n",__FILE__, __FUNCTION__);
            return -1;
        }
        for(i = 0; i < dlen; i++) {
            buf[dlen-1-i] = rsa_readl(RSAP);
        }
        memmove(od,buf, dlen * sizeof(unsigned int));
    }else{
        for(i = 0; i < dlen; i++) {
            od[dlen - 1 - i] = rsa_readl(RSAP);
        }
    }

	/* rsa clear interrupt bit 0 */
	rsa_writel(rsa_readl(RSACLR) | (RSA_INT_CLR),RSACLR);
}

static int jz_do_rsa_2048(unsigned int *n, unsigned int e,
				unsigned int *id, unsigned int *od,
				unsigned int klen)
{

	printf("do_rsa_2048 enter\n");
	printf("klen: %d\n", klen);

    if (klen * 32 != 2048) {
        return -1;
    }
	if (klen > 64) {
		printf("%s:%s: Invalid length: too long\n",__FILE__, __FUNCTION__);
		return -1; 
	}
    if (((unsigned long)n % 4) != 0) {
		printf("%s:%s: data_n is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, n);
        unsigned int buf[64] __attribute__((aligned(4)));
        memmove(buf, n, klen * sizeof(unsigned int));
        n = buf;
    }

    if (((unsigned long)id % 4) != 0) {
		printf("%s:%s: data_id is not 4-byte aligned. Address: %p\n", __FILE__, __FUNCTION__, id);
        unsigned int buf[64] __attribute__((aligned(4)));
        memmove(buf, id, klen * sizeof(unsigned int));
        id = buf;
    }
	rsa_perpare_key(id, n, e, klen);
	rsa_enable();
	rsa_do_crypt(od, klen);
	rsa_disable();

	return klen;
}

static struct jz_rsa_device _rsa_device = {
    .init_rsa = jz_rsa_init,
    .deinit_rsa = jz_rsa_deinit,
    .do_rsa_2048 = jz_do_rsa_2048,
};

struct jz_rsa_device* get_jz_rsa(void)
{
    return &_rsa_device;
}