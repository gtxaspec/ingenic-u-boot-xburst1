#ifndef _JZ_HASH_H_
#define _JZ_HASH_H_

/**
 * @brief jz_hash_device 
 */
struct jz_hash_device
{
	/**
	 * @brief init_hash: 初始化
	 *
	 * @retval: 0; success; 
	 * @retval: <0 error;
	 *
	 * @attention: 
	 */
	void (*init_hash)(void);

	/**
	 * @brief deinit_hash: 
	 *
	 * @retval: 0; success; 
	 * @retval: <0 error;
	 *
	 * @attention: 
	 */
	void (*deinit_hash)(void);

	/**
	 * @brief hash_sha256: hash sha256加密 
	 *
	 * @param[in] data_in:	被加密的明文
	 * @param[in] size:  明文长度， 单位Byte
	 * @param[out] data_out:  输出密文
	 *
	 * @retval: < 0;  error
	 * @retval: > 0;  success
	 *
	 * @attention 
	 */
	//void (*hash_sha256)(unsigned int* data_in, int size, unsigned int* data_out);
    void (*sha256_update)(unsigned char* data_in, int size);

    void (*sha256_final)(unsigned int* data_out);
};


/**
 * @brief get_hash_device: 获取hash设备
 *
 * @retval: NULL error;
 * @retval: NOT NULL success; 
 *
 * @attention 
 */
struct jz_hash_device* get_hash_device();

#endif
