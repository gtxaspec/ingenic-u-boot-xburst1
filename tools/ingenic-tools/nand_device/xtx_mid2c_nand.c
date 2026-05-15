#include <stdio.h>
#include "nand_common.h"

#define XTX_MID2C_MID			    0x2C
#define XTX_MID2C_NAND_DEVICD_COUNT	    1

static unsigned char xtx_mid2c_xaw[] = {0x2};

static struct device_struct device[1] = {
	DEVICE_STRUCT(0x24, 2048, 2, 4, 3, 1, xtx_mid2c_xaw),
};

static struct nand_desc xtx_mid2c_nand = {

	.id_manufactory = XTX_MID2C_MID,
	.device_counts = XTX_MID2C_NAND_DEVICD_COUNT,
	.device = device,
};

int xtx_mid2c_nand_register_func(void) {
	return nand_register(&xtx_mid2c_nand);
}
