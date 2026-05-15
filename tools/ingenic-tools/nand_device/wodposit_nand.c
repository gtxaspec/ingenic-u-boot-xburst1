#include <stdio.h>
#include "nand_common.h"

#define WODPOSIT_MID			    0xA5
#define WODPOSIT_NAND_DEVICD_COUNT	    1

static unsigned char wodposit_xaw[] = {0x2};

static struct device_struct device[1] = {
	DEVICE_STRUCT(0xA0, 2048, 2, 4, 2, 1, wodposit_xaw),
};

static struct nand_desc wodposit_nand = {

	.id_manufactory = WODPOSIT_MID,
	.device_counts = WODPOSIT_NAND_DEVICD_COUNT,
	.device = device,
};

int wodposit_nand_register_func(void) {
	return nand_register(&wodposit_nand);
}
