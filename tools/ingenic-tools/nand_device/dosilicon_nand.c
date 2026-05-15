#include <stdio.h>
#include "nand_common.h"

#define DS_MID 0xE5
#define DS_NAND_DEVICD_COUNT 4

static unsigned char ds_x1ga[] = {0x2};

static struct device_struct device[DS_NAND_DEVICD_COUNT] = {
	DEVICE_STRUCT(0x71, 2048, 2, 4, 2, 1, ds_x1ga),
	DEVICE_STRUCT(0x75, 2048, 2, 4, 2, 1, ds_x1ga),
	DEVICE_STRUCT(0x51, 2048, 2, 4, 2, 1, ds_x1ga),
	DEVICE_STRUCT(0xb2, 2048, 2, 4, 2, 1, ds_x1ga),
};

static struct nand_desc dosilicon_nand = {

	.id_manufactory = DS_MID,
	.device_counts = DS_NAND_DEVICD_COUNT,
	.device = device,
};

int dosilicon_nand_register_func(void)
{
	return nand_register(&dosilicon_nand);
}
