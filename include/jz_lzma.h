#ifndef __JZ_LZMA_H__
#define __JZ_LZMA_H__

#include <common.h>
#include <stddef.h>  /* 为了 size_t */

/**
 * @brief jz_lzma_decompress: LZMA解压缩
 *
 * @param src: 源数据缓冲区指针
 * @param size: 源数据大小（字节）
 * @param dst: 目标数据缓冲区指针
 *
 * @return: 解压缩成功的数据大小（字节），返回0表示解压缩失败
 */
unsigned int jz_lzma_decompress(unsigned char *src, size_t size, unsigned char *dst);

#endif  /* __JZ_LZMA_H__ */
