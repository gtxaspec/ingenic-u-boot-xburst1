#ifndef __JZ_AES_H__
#define __JZ_AES_H__

#include <common.h>

typedef enum {
    ENC_ECB ,
    DEC_ECB ,
    ENC_CBC ,
    DEC_CBC ,
} ModeType;

typedef enum {
    AES_128 ,
    AES_192 ,
    AES_256 ,
} BitType;

typedef unsigned int uint32_t;

/**
 * @brief: aes device
 */
struct jz_aes_device {

	/**
	 * @brief init_aes: aes初始化，包括开cpm
     *
	¦* @param[in] aesiv: aes iv值
    ¦* @param[in] aeskey: aes key值
    ¦* @param[in] mode: aes编解码方式
	¦* @param[in] len: aes密钥长度
    ¦*
     * @retval: 0; success;
	 * @retval: <0 error;
	 *
	 */
	int (*init_aes)(uint32_t *aesiv,uint32_t *aeskey,ModeType mode,BitType len);

	/**
	 * @brief deinit_aes:
	 *
	 * @retval: 0; success;
	 * @retval: <0 error;
	 *
	 * @attention:
	 */
	int (*deinit_aes)(void);

    /**
    ¦* @brief aes_update: aes加(原)密数据传输
    ¦*
    ¦* @param[in] data_in: aes需要加(解)密的密(原)文
    ¦* @param[in] len: 此次传输的数据长度/sizeof(int)
    ¦* @param[out] data_out: 加(解)密输出结果
    ¦*
    ¦* @retval: >0; success;
    ¦* @retval: <0 error;
    ¦*/
    int (*aes_update)(uint32_t *data_in, uint32_t len, uint32_t *data_out);

    /**
    ¦* @brief aes_final: aes加(原)密数据结束
    ¦*
    ¦* @retval: >0; success;
    ¦* @retval: <0 error;
    ¦*/
    int (*aes_final)(void);
};

/**
 * @brief get_jz_aes: 获取aes设备
 *
 * @retval: NULL error
 * @retval: NOT NULL success
 *
 * @attention
 */
struct jz_aes_device* get_jz_aes(void);

#endif
