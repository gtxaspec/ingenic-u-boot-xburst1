#ifndef __JZ_RSA_H__
#define __JZ_RSA_H__

/**
 * @brief: rsa device
 */
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
    ¦* @brief rsa_2048_init: rsa2048加(原)密初始化
    ¦*
    ¦* @param[in] n: rsa mod n的值
    ¦* @param[in] e: rsa mod e的值
    ¦* @param[in] klen: klen为密文长度/sizeof(int)，rsa-2048这里默认为64
    ¦*
    ¦* @retval: >0; success;
    ¦* @retval: <0 error;
    ¦*
    ¦* @attention: n和e即为公钥
    ¦*/
    int (*rsa_2048_init)(unsigned int *n, unsigned int e,unsigned int klen);

    /**
    ¦* @brief rsa_2048_update: rsa2048加(原)密数据传输
    ¦*
    ¦* @param[in] id: rsa需要加(解)密的密(原)文
    ¦* @param[in] klen: 此次传输的数据长度/sizeof(int)
    ¦*
    ¦* @retval: >0; success;
    ¦* @retval: <0 error;
    ¦*/
    int (*rsa_2048_update)(unsigned int *id, unsigned int len);

    /**
    ¦* @brief rsa_2048_final: rsa2048加(原)密数据结束
    ¦*
    ¦* @param[out] od: 加(解)密输出结果
    ¦*
    ¦* @retval: >0; success;
    ¦* @retval: <0 error;
    ¦*/
    int (*rsa_2048_final)(unsigned int *od);
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

#endif
