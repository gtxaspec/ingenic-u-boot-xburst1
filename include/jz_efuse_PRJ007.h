#ifndef _JZ_EFUSE_PRJ007_H__
#define _JZ_EFUSE_PRJ007_H__

#define CHIP_ID_ADDR         (0x00)
#define USER_ID_ADDR         (0x0C)
#define SARADC_CAL           (0x10)
#define TRIM_ADDR            (0x12)
#define PROGRAM_PROTECT_ADDR (0x13)
#define CPU_ID_ADDR          (0x14)
#define SPECIAL_ADDR         (0x16)
#define CUSTOMER_RESV_ADDR   (0x18)
#define SCB_DATA_ADDR        (0x40)
#define RESERVE_DATA_ADDR    (0x80)

enum segment_id {
	CHIP_ID,
	USER_ID,
	ADC_CALIB,
	TRIM_DATA,
	PROTECT_ID,
	CPU_ID,
	SPECIAL_USE,
	USER_DATA,
	SCB_DATA,
	RESERVE_DATA,
};

enum segment_size {
	CHIP_ID_SIZE = 12,    /* 12 Bytes,  96 bits */
	USER_ID_SIZE = 4,     /*  4 Bytes,  32 bits */
	ADC_CALIB_SIZE = 2,   /*  2 Bytes,  16 bits */
	TRIM_DATA_SIZE = 1,   /*  1 Bytes,  8 bits */
	PROTECT_ID_SIZE = 1,  /*  1 Bytes,   8 bits */
	CPU_ID_SIZE = 2,      /*  2 Bytes,  16 bits */
	SPECIAL_USE_SIZE = 2, /*  2 Bytes,  16 bits */
	USER_DATA_SIZE = 40,  /* 40 Bytes, 320 bits */
	SCB_DATA_SIZE = 64,   /* 64 Bytes, 512 bits */
	RESERVE_DATA_SIZE = 64 /* 64 Bytes, 512 bits */
};

/**
 * @brief read_efuse:	读efuse，按段读写
 *
 * @param[in] seg_id:	段名
 * @param[in] buf:		读取数据的buf
 * @param[in] len:		要读取的长度
 *
 * @retval: == 0; success
 * @retval: <0; error
 *
 * @attention
 */
int read_efuse(enum segment_id seg_id, unsigned char *buf, int len);

#undef SCB_DEBUG
#ifdef SCB_DEBUG
void scb_info_show(void);
#endif

#endif
