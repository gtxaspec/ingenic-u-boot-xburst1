#ifndef __JZ_RSA_PRJ008_H__
#define __JZ_RSA_PRJ008_H__

#define RSA_BASE        0xb34C0000
#define	CPM_BASE		0xb0000000
#define CPM_CLKGR0_OFFSET       (0x20)
#define RSACDDR_OFFSET          (0x004C)

#define RSAC            (0x0)
#define RSAE            (0x4)
#define RSAN            (0x8)
#define RSAM            (0xc)
#define RSAP            (0x10)
#define RSACLR          (0x14)
#define RSAINFO         (0X18)
#define RSAEN           (0X1c)
#define RSADBG0         (0X20)
#define RSADBG1         (0X24)

#define CLKGR0_RSA      (1 << 23)

#define RSAC_RSA_1024   (32 << 4)
#define RSAC_RSA_2048   (64 << 4)
#define RSAC_EN         (1 << 0)
#define RSA_N_LEN       (0x7f << 4)
#define RSA_N_CFG_DONE  (1 << 4)
#define RSA_N_CFG_CLR   (1 << 4)
#define RSA_INT_DONE    (1 << 0)
#define RSA_INT_CLR     (1 << 0)

#define REG32(addr)				*((volatile unsigned int *)(addr))
#define rsa_readl(off)			*(volatile unsigned int *)(RSA_BASE + off)
#define rsa_writel(val, off)	(*(volatile unsigned int *)(RSA_BASE + off) = val)

#define CPM_CLKGR0             (CPM_BASE + CPM_CLKGR0_OFFSET)
#define RSACDDR                (CPM_BASE + RSACDDR_OFFSET)
#define __cpm_start_rsa()      (REG32(CPM_CLKGR0) &= ~CLKGR0_RSA)
#define __cpm_stop_rsa()       (REG32(CPM_CLKGR0) |= CLKGR0_RSA)
#define __rsa_clk()            (REG32(RSACDDR) = 0x60000002)

struct jz_rsa_device {

	/**
	 * @brief init_rsa: rsa初始化，包括开cpm，配频
	 *
	 * @retval: 0; success;
	 * @retval: <0 error;
	 *
	 * @attention:
	 */
	int (*init_rsa)(void);

	/**
	 * @brief deinit_rsa:
	 *
	 * @retval: 0; success;
	 * @retval: <0 error;
	 *
	 * @attention:
	 */
	int (*deinit_rsa)(void);

    /**
    ¦* @brief do_rsa_2048: rsa2048加(原)密初始化
    ¦*
    ¦* @param[in] n: rsa mod n的值
    ¦* @param[in] e: rsa mod e的值
    ¦* @param[in] id: rsa需要加(解)密的密(原)文
    ¦* @param[out] od: 加(解)密输出结果
    ¦* @param[in] klen: klen为密文长度，rsa-2048这里默认为64(须为32的倍数)
    ¦*
    ¦* @retval: >0; success;
    ¦* @retval: <0 error;
    ¦*
    ¦* @attention: n和e即为公钥
    ¦*/
    int (*do_rsa_2048)(unsigned int *n, unsigned int e,unsigned int *id, unsigned int *od,unsigned int klen)
};

/**
 * @brief get_jz_rsa: 获取rsa设备
 *
 * @retval: NULL error
 * @retval: NOT NULL success
 *
 * @attention
 */
struct jz_rsa_device* get_jz_rsa(void);


#endif /* _SCROM_RSA_H_ */

