#include <stdio.h>
#include "nand_common.h"

#define XTX_MID0B_MID			    0x0B
#define XTX_MID0B_NAND_DEVICD_COUNT	    5

static unsigned char xtx_mid0b_xaw[] = {0xf};
static unsigned char xtx_mid0b_x01dw[] = {0x2};
static unsigned char xtx_mid0b_x02dw[] = {0x2};
static unsigned char xtx_mid0b_x11dw[] = {0x2};
static unsigned char xtx_mid0b_x12dw[] = {0x2};

static struct device_struct device[XTX_MID0B_NAND_DEVICD_COUNT] = {
	DEVICE_STRUCT(0x11, 2048, 2, 4, 4, 1, xtx_mid0b_xaw),
	DEVICE_STRUCT(0x31, 2048, 2, 4, 4, 1, xtx_mid0b_x01dw),
	DEVICE_STRUCT(0x32, 2048, 2, 4, 4, 1, xtx_mid0b_x02dw),
	DEVICE_STRUCT(0x34, 2048, 2, 4, 4, 1, xtx_mid0b_x11dw),
	DEVICE_STRUCT(0x35, 2048, 2, 4, 4, 1, xtx_mid0b_x12dw),
};

static struct nand_desc xtx_mid0b_nand = {

	.id_manufactory = XTX_MID0B_MID,
	.device_counts = XTX_MID0B_NAND_DEVICD_COUNT,
	.device = device,
};

int xtx_mid0b_nand_register_func(void) {
	return nand_register(&xtx_mid0b_nand);
}
