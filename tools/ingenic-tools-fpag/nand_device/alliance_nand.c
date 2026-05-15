#include <stdio.h>
#include "nand_common.h"

#define AS_MID			    0x52
#define AS_NAND_DEVICD_COUNT	    1

static unsigned char as5f_errstat[] = {0x2};

static struct device_struct device[AS_NAND_DEVICD_COUNT] = {
	DEVICE_STRUCT(0x2d, 4096, 2, 4, 2, 1, as5f_errstat),
};

static struct nand_desc as_nand = {
	.id_manufactory = AS_MID,
	.device_counts = AS_NAND_DEVICD_COUNT,
	.device = device,
};

int alliance_nand_register_func(void) {
	return nand_register(&as_nand);
}
