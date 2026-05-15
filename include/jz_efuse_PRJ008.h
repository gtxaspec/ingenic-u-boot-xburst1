#ifndef _JZ_EFUSE_PRJ008_H__
#define _JZ_EFUSE_PRJ008_H__

// #define CHIP_ID_ADDR         (0x00)
// #define USER_ID_ADDR         (0x0C)
// #define SARADC_CAL           (0x10)
// #define TRIM_ADDR            (0x12)
// #define PROGRAM_PROTECT_ADDR (0x13)
// #define CPU_ID_ADDR          (0x14)
// #define SPECIAL_ADDR         (0x16)
// #define CUSTOMER_RESV_ADDR   (0x18)
// #define SCB_DATA_ADDR        (0x40)
// #define RESERVE_DATA_ADDR    (0x80)

#define CHIP_ID_ADDR         	(0x00)
#define PROT_ON_ADDR           	(0x0C)
#define SPEC_ADDR            	(0x0E)
#define PROT_BAK_ADDR         	(0x0F)
#define TSENS_ADDR            	(0x11)//非对齐访问，要特殊处理
#define RETEN_ADDR             	(0x12)
#define SCB_DATA_ADDR         	(0x20)
#define UKEY_ADDR            	(0x40)
#define SCB_BAK_ADDR         	(0x60)

enum segment_id {
	CHIP_ID,
	PROT_ON,
	SPEC,
	PROT_BAK,
	TSENS,
	RETEN,
	SCB_DATA,
	UKEY,
	SCB_BAK,
};

enum segment_size {
	// CHIP_ID_SIZE = 12,    /* 12 Bytes,  96 bits */
	// USER_ID_SIZE = 4,     /*  4 Bytes,  32 bits */
	// ADC_CALIB_SIZE = 2,   /*  2 Bytes,  16 bits */
	// TRIM_DATA_SIZE = 1,   /*  1 Bytes,  8 bits */
	// PROTECT_ID_SIZE = 1,  /*  1 Bytes,   8 bits */
	// CPU_ID_SIZE = 2,      /*  2 Bytes,  16 bits */
	// SPECIAL_USE_SIZE = 2, /*  2 Bytes,  16 bits */
	// USER_DATA_SIZE = 40,  /* 40 Bytes, 320 bits */
	// SCB_DATA_SIZE = 64,   /* 64 Bytes, 512 bits */
	// RESERVE_DATA_SIZE = 64 /* 64 Bytes, 512 bits */
	CHIP_ID_SIZE = 12,    /* 12 Bytes,  96 bits */
	PROT_ON_SIZE = 2,     /*  2 Bytes,   16 bits */
	SPEC_SIZE = 1,        /*  1 Bytes,   8 bits */
	PROT_BAK_SIZE = 2,    /*  2 Bytes,   16 bits */
	TSENS_SIZE = 1,       /*  1 Bytes,   8 bits */
	RETEN_SIZE = 12,       /*  12 Bytes,  96 bits */
	SCB_DATA_SIZE = 32,   /* 32 Bytes, 256 bits */
	UKEY_SIZE = 32,       /* 32 Bytes, 256 bits */
	SCB_BAK_SIZE = 32,    /* 32 Bytes, 256 bits */
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
void read_efuse(enum segment_id seg_id, unsigned char *buf, int len);

#undef OTP_DEBUG
#define OTP_DEBUG
#ifdef OTP_DEBUG
void otp_info_show(void);
#endif

#endif
