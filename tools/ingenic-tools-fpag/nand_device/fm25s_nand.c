#include <stdio.h>
#include "nand_common.h"

#define FM25S_MID			    0xA1
#define FM25S_NAND_DEVICD_COUNT	    2

static unsigned char fm25s_02a[] = {0x2, 0x3};

static struct device_struct device[] = {
	DEVICE_STRUCT(0xE4, 2048, 2, 4, 2, 2, fm25s_02a),
	DEVICE_STRUCT(0xE5, 2048, 2, 4, 2, 2, fm25s_02a),
};

static struct nand_desc fm25s_nand = {
	.id_manufactory = FM25S_MID,
	.device_counts = FM25S_NAND_DEVICD_COUNT,
	.device = device,
};

int fm25s_nand_register_func(void) {
	return nand_register(&fm25s_nand);
}
