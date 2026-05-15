#include <stdio.h>
#include "nand_common.h"

#define DS_MID 0xE5
#define DS_NAND_DEVICD_COUNT 1

static unsigned char ds_q1ga[] = {0x2};

static struct device_struct device[] = {
	DEVICE_STRUCT(0x71, 2048, 2, 4, 2, 1, ds_q1ga),
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
