#include <stdio.h>
#include "nand_common.h"

#define SKY_MID			    0x01
#define SKY_NAND_DEVICD_COUNT	    1

static unsigned char sky_errstat[]= {0x3};

static struct device_struct device[] = {
	DEVICE_STRUCT(0x15, 2048, 2, 4, 2, 1, sky_errstat),
};

static struct nand_desc sky_nand = {

	.id_manufactory = SKY_MID,
	.device_counts = SKY_NAND_DEVICD_COUNT,
	.device = device,
};

int sky_nand_register_func(void) {
	return nand_register(&sky_nand);
}

