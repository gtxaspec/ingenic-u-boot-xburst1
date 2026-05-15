#include <errno.h>
#include <malloc.h>
#include <linux/mtd/partitions.h>
#include "../jz_sfc_nand.h"
#include "nand_common.h"
#define XC_DEVICES_NUM          5
#define TSETUP		5
#define THOLD		5
#define	TSHSL_R		20
#define	TSHSL_W		20

static struct jz_nand_base_param xc_param[XC_DEVICES_NUM] = {

	[0] = {
		/*XC5F1GQ4UB*/
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 32,
		.flashsize = (2 * 1024 * 64 * 1024) - ( 2 * 1024 * 64 * 10),

		.tSETUP  = TSETUP,
		.tHOLD   = THOLD,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.ecc_max = 0x4,
#ifdef CONFIG_SPI_STANDARD
	.need_quad = 0,
#else
	.need_quad = 1,
#endif
	},
	[1] = {
		/*XC5F2GQ5UE*/
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 64,
		.flashsize = 2 * 1024 * 64 * 2048 - (2 * 1024 * 64 * 10),

		.tSETUP  = TSETUP,
		.tHOLD   = THOLD,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.ecc_max = 0x4,
#ifdef CONFIG_SPI_STANDARD
		.need_quad = 0,
#else
		.need_quad = 1,
#endif
	},
	[2] = {
		/*F50L1G41LC*/
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 32,
		.flashsize = (2 * 1024 * 64 * 1024) - ( 2 * 1024 * 64 * 10),

		.tSETUP  = TSETUP,
		.tHOLD   = THOLD,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.ecc_max = 0x4,
#ifdef CONFIG_SPI_STANDARD
	.need_quad = 0,
#else
	.need_quad = 1,
#endif
	},

};

static struct device_id_struct device_id[XC_DEVICES_NUM] = {
	DEVICE_ID_STRUCT(0x01, "XCSP1AAPK",&xc_param[0]),
	DEVICE_ID_STRUCT(0xA1, "XCSP2AAPK",&xc_param[1]),
	DEVICE_ID_STRUCT(0x2C, "F50L1G41LC",&xc_param[2]),
	};

static void xc_single_read(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

	struct sfc_flash *flash = op_info->flash;
	struct jz_nand_descriptor *nand_desc = flash->flash_info;
	uint8_t device_id = nand_desc->id_device;
	uint8_t addr_len = 0;
	switch(device_id) {
	    case 0x01:
		case 0xA1:
		case 0x2C:
			addr_len = 2;
			break;
	    default:
		    printf("device_id err, please check your  device id: device_id = 0x%02x\n", device_id);
		    addr_len = 2;
		    break;
	}

	nand_single_read(transfer, cmd, op_info, addr_len);
	return;
}

static void xc_quad_read(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

	struct sfc_flash *flash = op_info->flash;
	struct jz_nand_descriptor *nand_desc = flash->flash_info;
	uint8_t device_id = nand_desc->id_device;
	uint8_t addr_len = 0;
	switch(device_id) {
		case 0x01:
		case 0xA1:
		case 0x2C:
			addr_len = 2;
			break;
	    default:
		    printf("device_id err, please check your device id: device_id = 0x%02x\n", device_id);
		    addr_len = 2;
		    break;
	}
	nand_quad_read(transfer, cmd, op_info, addr_len);
	return;
}

static int32_t xc_get_f0_register_value(struct sfc_flash *flash) {

	struct sfc_transfer transfer;
	struct sfc_message message;
	struct cmd_info cmd;
	uint32_t buf = 0;

	memset(&transfer, 0, sizeof(transfer));
	memset(&cmd, 0, sizeof(cmd));
	sfc_message_init(&message);

	cmd.cmd = SPINAND_CMD_GET_FEATURE;
	transfer.sfc_mode = TM_STD_SPI;

	transfer.addr = 0xf0;
	transfer.addr_len = 1;

	cmd.dataen = ENABLE;
	transfer.len = 1;
	transfer.data = (u_char *)&buf;
	transfer.direction = GLB_TRAN_DIR_READ;

	transfer.data_dummy_bits = 0;
	transfer.cmd_info = &cmd;
	transfer.ops_mode = CPU_OPS;

	sfc_message_add_tail(&transfer, &message);
	if(sfc_sync(flash->sfc, &message)) {
	        printf("sfc_sync error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}
	return buf;
}

static int32_t xc_get_read_feature(struct flash_operation_message *op_info) {

	struct sfc_flash *flash = op_info->flash;
	struct jz_nand_descriptor *nand_desc = flash->flash_info;
	struct sfc_transfer transfer;
	struct sfc_message message;
	struct cmd_info cmd;
	uint8_t device_id = nand_desc->id_device;
	uint8_t ecc_status = 0;
	int32_t ret = 0;

	memset(&transfer, 0, sizeof(transfer));
	memset(&cmd, 0, sizeof(cmd));
	sfc_message_init(&message);

	cmd.cmd = SPINAND_CMD_GET_FEATURE;
	transfer.sfc_mode = TM_STD_SPI;

	transfer.addr = SPINAND_ADDR_STATUS;
	transfer.addr_len = 1;

	cmd.dataen = DISABLE;
	transfer.len = 0;

	transfer.data_dummy_bits = 0;
	cmd.sta_exp = (0 << 0);
	cmd.sta_msk = SPINAND_IS_BUSY;
	transfer.cmd_info = &cmd;
	transfer.ops_mode = CPU_OPS;

	sfc_message_add_tail(&transfer, &message);
	if(sfc_sync(flash->sfc, &message)) {
		printf("sfc_sync error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}
	ecc_status = sfc_get_sta_rt(flash->sfc);

	switch(device_id) {
		case 0x01:
		case 0xA1:
		case 0x2C:
			switch((ecc_status >> 4) & 0x3) {
				case 0x2:
					ret = -EBADMSG;
				default:
					ret = 0x0;
			}
			break;
		default:
			printf("device_id err,it maybe don`t support this device, please check your device id: device_id = 0x%02x\n", device_id);
			ret = -EIO;   //notice!!!
	}

	return ret;
}

int xc_nand_init(void) {
	struct jz_nand_device *xc_nand;
	xc_nand = kzalloc(sizeof(*xc_nand), GFP_KERNEL);
	if(!xc_nand) {
		pr_err("alloc xc_nand struct fail\n");
		return -ENOMEM;
	}

	xc_nand->id_manufactory = 0x8C;
	xc_nand->id_device_list = device_id;
	xc_nand->id_device_count = XC_DEVICES_NUM;

	xc_nand->ops.nand_read_ops.get_feature = xc_get_read_feature;
	xc_nand->ops.nand_read_ops.single_read = xc_single_read;
	xc_nand->ops.nand_read_ops.quad_read = xc_quad_read;

	return jz_spinand_register(xc_nand);
}
SPINAND_MOUDLE_INIT(xc_nand_init);
