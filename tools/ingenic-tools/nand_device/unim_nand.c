#include <stdio.h>
#include "nand_common.h"

#define UNIM_MID			    0xB0
#define UNIM_NAND_DEVICD_COUNT	    1

static unsigned char unim_agd[] = {0x2};

static struct device_struct device[UNIM_NAND_DEVICD_COUNT] = {
	DEVICE_STRUCT(0x24, 2048, 2, 4, 3, 1, unim_agd),
};

static struct nand_desc unim_nand = {

	.id_manufactory = UNIM_MID,
	.device_counts = UNIM_NAND_DEVICD_COUNT,
	.device = device,
};

int unim_nand_register_func(void) {
	return nand_register(&unim_nand);
}
