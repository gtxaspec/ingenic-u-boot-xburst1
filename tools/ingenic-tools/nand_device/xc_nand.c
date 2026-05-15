#include <stdio.h>
#include "nand_common.h"

#define XC_MID			    0x8C
#define XC_NAND_DEVICD_COUNT	    3

static unsigned char xcxb_errstat[]= {0x2};
static unsigned char xcxc_errstat[]= {0x7};
static unsigned char xcxe_errstat[]= {0x2};

static struct device_struct device[XC_NAND_DEVICD_COUNT] = {
	DEVICE_STRUCT(0x01, 2048, 2, 4, 2, 1, xcxe_errstat),
	DEVICE_STRUCT(0xA1, 2048, 2, 4, 2, 1, xcxe_errstat),
	DEVICE_STRUCT(0x2C, 2048, 2, 4, 2, 1, xcxe_errstat),
};

static struct nand_desc xc_nand = {

	.id_manufactory = XC_MID,
	.device_counts = XC_NAND_DEVICD_COUNT,
	.device = device,
};

int xc_nand_register_func(void) {
	return nand_register(&xc_nand);
}
