#include <stdio.h>
#include "nand_common.h"

#define XTX_MID0B_MID			    0x0B
#define XTX_MID0B_NAND_DEVICD_COUNT	    1

static unsigned char xtx_mid0b_xaw[] = {0xf};

static struct device_struct device[1] = {
	DEVICE_STRUCT(0x11, 2048, 2, 4, 4, 1, xtx_mid0b_xaw),
};

static struct nand_desc xtx_mid0b_nand = {

	.id_manufactory = XTX_MID0B_MID,
	.device_counts = XTX_MID0B_NAND_DEVICD_COUNT,
	.device = device,
};

int xtx_mid0b_nand_register_func(void) {
	return nand_register(&xtx_mid0b_nand);
}
