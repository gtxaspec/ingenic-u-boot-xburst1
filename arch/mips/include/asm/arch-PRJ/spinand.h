// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __SPINAND_H
#define __SPINAND_H
#include <asm/arch/sfc.h>
#include <asm/arch/spinand_cmd.h>
#include <linux/types.h>
#include <linker_lists.h>

#define MTD_MODE                0x0     //use mtd mode, erase partition when write
#define MTD_D_MODE              0x2     //use mtd dynamic mode, erase block_size when write
#define UBI_MANAGER             0x1

#define SPINAND_MAGIC_NUM	0x646e616e   //ascii "nand"

struct jz_sfcnand_partition {
	char name[32];		/* identifier string */
	uint32_t size;          /* partition size */
	uint32_t offset;        /* offset within the master MTD space */
	uint32_t mask_flags;    /* master MTD flags to mask out for this partition */
	uint32_t manager_mode;  /* manager_mode mtd or ubi */
};

struct jz_sfcnand_burner_param {
	uint32_t magic_num;
	int32_t partition_num;
	struct jz_sfcnand_partition *partition;
};
struct jz_sfcnand_partition_param {
	uint8_t num_partition;
/*	struct mtd_partition *partition;*/
	struct jz_sfcnand_partition *partition;
};


/*
 * u-boot private
 */
#ifndef CONFIG_SPL_BUILD
struct jz_sfcnand_base_param {
	uint32_t pagesize;
	uint32_t blocksize;
	uint32_t oobsize;
	uint32_t flashsize;

	uint16_t tHOLD;
	uint16_t tSETUP;
	uint16_t tSHSL_R;
	uint16_t tSHSL_W;

	uint16_t tRD;
	uint16_t tPP;
	uint16_t tBE;

	/*
	 * Indicates that NAND flash has a serial plane structure,
	 * needs to convert the plane by changing the column address
	 */
	uint8_t plane_select;

	uint8_t ecc_max;
	uint8_t need_quad;
};

struct device_id_struct {
	uint16_t id_device;
	char *name;
	struct jz_sfcnand_base_param *param;
};

struct spi_nand_cmd_info {
	unsigned short cmd;
	unsigned char dummy_byte;
	unsigned char addr_nbyte;
	unsigned char transfer_mode;

};

struct spi_nand_st_info {
	unsigned short cmd;
	unsigned char bit_shift;
	unsigned char mask;
	unsigned char val;
	unsigned char len;	/* length of byte to operate from register */
	unsigned char dummy;
};

struct jz_sfcnand_cdt_params {
	/* general cmd info */
	struct spi_nand_cmd_info r_to_cache;
	struct spi_nand_cmd_info standard_r;
	struct spi_nand_cmd_info quad_r;
	struct spi_nand_cmd_info standard_w_cache;
	struct spi_nand_cmd_info quad_w_cache;
	struct spi_nand_cmd_info w_exec;
	struct spi_nand_cmd_info b_erase;
	struct spi_nand_cmd_info w_en;
	struct spi_nand_cmd_info ecc_r;

	/* status polling cmd info */
	struct spi_nand_st_info oip;
};
typedef struct jz_sfcnand_cdt_params cdt_params_t;

struct jz_sfcnand_ops {
	cdt_params_t *(*get_cdt_params)(struct sfc_flash *, uint16_t);
	int (*deal_ecc_status)(struct sfc_flash *, uint16_t, uint8_t);
	int32_t (*get_feature)(struct sfc_flash *, uint8_t);
};

struct jz_sfcnand_device {
	uint8_t id_manufactory;
	struct device_id_struct *id_device_list;
	uint8_t id_device_count;

	struct jz_sfcnand_ops ops;
	cdt_params_t cdt_params;

	struct list_head list;
};

struct jz_sfcnand_flashinfo {
	uint8_t id_manufactory;
	uint16_t id_device;

	struct jz_sfcnand_base_param param;
	struct jz_sfcnand_partition_param partition;
	struct jz_sfcnand_ops *ops;
	cdt_params_t *cdt_params;
};
#define X_ENV_LENGTH		1024
#define X_COMMAND_LENGTH	128

#define MTD_MODE	0x0
#define UBI_MANAGER     0x1


int jz_sfcnand_register(struct jz_sfcnand_device *flash);
typedef int32_t (*spinand_regcall_t)(void);

#define ingenic_entry_declare(_type, _name, _list)				\
	static _type ingenic_##_list##_2_##_name __aligned(4)			\
			__attribute__((used,					\
			section("__u_boot_list_2_"#_list"_2_"#_name)))

#define SPINAND_MOUDLE_INIT(fn)      \
	ingenic_entry_declare(spinand_regcall_t, _1##fn, flash) = fn

/* SFC CDT Maximum INDEX number */
#define INDEX_MAX_NUM 32

/* SFC CDT INDEX */
enum {
	/* 1. reset */
	NAND_RESET,

	/* 2. try id */
	NAND_TRY_ID,

	/* 3. try id with dummy */
	NAND_TRY_ID_DMY,

	/* 4. set feature */
	NAND_SET_FEATURE,

	/* 5. get feature */
	NAND_GET_FEATURE,

	/* 6. nand standard read */
	NAND_STANDARD_READ_TO_CACHE,
	NAND_STANDARD_READ_GET_FEATURE,
	NAND_STANDARD_READ_FROM_CACHE,

	/* 7. nand quad read */
	NAND_QUAD_READ_TO_CACHE,
	NAND_QUAD_READ_GET_FEATURE,
	NAND_QUAD_READ_FROM_CACHE,

	/* 8. nand standard write */
	NAND_STANDARD_WRITE_ENABLE,
	NAND_STANDARD_WRITE_TO_CACHE,
	NAND_STANDARD_WRITE_EXEC,
	NAND_STANDARD_WRITE_GET_FEATURE,

	/* 9. nand quad write */
	NAND_QUAD_WRITE_ENABLE,
	NAND_QUAD_WRITE_TO_CACHE,
	NAND_QUAD_WRITE_EXEC,
	NAND_QUAD_WRITE_GET_FEATURE,

	/* 10. block erase */
	NAND_ERASE_WRITE_ENABLE,
	NAND_BLOCK_ERASE,
	NAND_ERASE_GET_FEATURE,

	/* 11. ecc status read */
	NAND_ECC_STATUS_READ,

	/* index count */
	NAND_MAX_INDEX,
};
#endif


/*
 * spl private
 */
#ifdef CONFIG_SPL_BUILD
/* some manufacture with unusual method */
#define MANUFACTURE_PID1_IGNORE 0x00

#define WINBOND_VID             0xef
#define WINBOND_PID0            0xaa
#define WINBOND_PID1            0x21

#define GIGADEVICE_VID          0xc8

#define GD5F1GQ4UC_PID          0xb1
#define GD5F2GQ4UC_PID          0xb2
#define GD5F1GQ4RC_PID          0xa1
#define GD5F2GQ4RC_PID          0xa2
//#define GD5FxGQ4xC_PID1         0x48

#define BITS_BUF_EN             (1 << 3)

/* Buswidth is 16 bit */
#define NAND_BUSWIDTH_16	0x00000002
#define NAND_BUSWIDTH_8		0x00000001

enum {
    VALUE_SET,
    VALUE_CLR,
};

#define OPERAND_CONTROL(action, val, ret) do{                                   \
    if (action == VALUE_SET)                                                    \
        ret |= val;                                                             \
    else                                                                        \
        ret &= ~val;                                                            \
} while(0)

struct spiflash_register {
    uint32_t addr;
    uint32_t val;
    uint8_t action;
};

struct special_spiflash_id {
    uint8_t vid;
    uint8_t pid;
};

struct special_spiflash_desc {
    uint8_t vid;
    struct spiflash_register regs;
};

int spinand_init(void);
int spinand_read(uint32_t src_addr, uint32_t count, uint32_t dst_addr);

///////////////////upper layer logic depends on flash /////////////////
#ifdef CONFIG_BEIJING_OTA
int ota_load(uint32_t *argv, uint32_t dst_addr);
#endif

#ifdef CONFIG_RECOVERY
int is_recovery_update_failed(void);
#endif
#endif

#endif
