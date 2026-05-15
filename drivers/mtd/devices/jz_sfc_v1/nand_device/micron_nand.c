#include <errno.h>
#include <malloc.h>
#include <linux/mtd/partitions.h>
#include "../jz_sfc_nand.h"
#include "nand_common.h"

#define MICRON_DEVICES_NUM         1
#define TSETUP		5
#define THOLD		5
#define	TSHSL_R		20
#define	TSHSL_W		20

static struct jz_nand_base_param micron_param[MICRON_DEVICES_NUM] = {

	[0] = {
		/* MT29F2G01A */
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 128,
		.flashsize = 2 * 1024 * 64 * 2048,

		.tSETUP  = TSETUP,
		.tHOLD   = THOLD,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.ecc_max = 0x8,
#ifdef CONFIG_SPI_STANDARD
		.need_quad = 0,
#else
		.need_quad = 1,
#endif
	}
};

static struct device_id_struct device_id[MICRON_DEVICES_NUM] = {
	DEVICE_ID_STRUCT(0x24, "MT29F2G01A", &micron_param[0]),
};

static void micron_single_read(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

	uint8_t plane_flag = 0;

	plane_flag = (op_info->pageaddr >> 6) & 1;
	op_info->columnaddr |= (plane_flag << 12);

	cmd->cmd = SPINAND_CMD_FRCH;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	cmd->dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_READ;

	transfer->data_dummy_bits = 8;
	transfer->cmd_info = cmd;
	transfer->ops_mode = CPU_OPS;
	return;
}


static void micron_quad_read(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

	uint8_t plane_flag = 0;

	plane_flag = (op_info->pageaddr >> 6) & 1;
	op_info->columnaddr |= (plane_flag << 12);

	cmd->cmd = SPINAND_CMD_RDCH_X4;
	transfer->sfc_mode = TM_QI_QO_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	cmd->dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_READ;

	transfer->data_dummy_bits = 8;
	transfer->cmd_info = cmd;
	transfer->ops_mode = CPU_OPS;
	return;


}

static void micron_single_load(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

	uint8_t plane_flag = 0;

	plane_flag = (op_info->pageaddr >> 6) & 1;
	op_info->columnaddr |= (plane_flag << 12);

	cmd->cmd = SPINAND_CMD_PRO_LOAD;
	transfer->sfc_mode = TM_STD_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	cmd->dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_WRITE;

	transfer->data_dummy_bits = 0;
	transfer->cmd_info = cmd;
	transfer->ops_mode = CPU_OPS;
	return;
}

static void micron_quad_load(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

	uint8_t plane_flag = 0;

	plane_flag = (op_info->pageaddr >> 6) & 1;
	op_info->columnaddr |= (plane_flag << 12);

	cmd->cmd = SPINAND_CMD_PRO_LOAD_X4;
	transfer->sfc_mode = TM_QI_QO_SPI;

	transfer->addr = op_info->columnaddr;
	transfer->addr_len = 2;

	cmd->dataen = ENABLE;
	transfer->data = op_info->buffer;
	transfer->len = op_info->len;
	transfer->direction = GLB_TRAN_DIR_WRITE;

	transfer->data_dummy_bits = 0;
	transfer->cmd_info = cmd;
	transfer->ops_mode = CPU_OPS;
	return;
}


static int32_t micron_get_read_feature(struct flash_operation_message *op_info) {

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
		case 0x24:
			switch((ecc_status >> 4) & 0x7) {
				case 0x02:
					ret = -EBADMSG;
					break;
				default:
					ret = 0;
			}
			break;
		default:
			printf("device_id err, it maybe don`t support this device, check your device id: device_id = 0x%02x\n", device_id);
			ret = -EIO;   //notice!!!

	}
	return ret;
}

int micron_nand_init(void) {
	struct jz_nand_device *micron_nand;
	micron_nand = kzalloc(sizeof(*micron_nand), GFP_KERNEL);
	if(!micron_nand) {
		pr_err("alloc micron_nand struct fail\n");
		return -ENOMEM;
	}

	micron_nand->id_manufactory = 0x2C;
	micron_nand->id_device_list = device_id;
	micron_nand->id_device_count = MICRON_DEVICES_NUM;

	micron_nand->ops.nand_read_ops.single_read = micron_single_read;
	micron_nand->ops.nand_read_ops.quad_read = micron_quad_read;

	micron_nand->ops.nand_write_ops.single_load = micron_single_load;
	micron_nand->ops.nand_write_ops.quad_load = micron_quad_load;

	micron_nand->ops.nand_read_ops.get_feature = micron_get_read_feature;
	return jz_spinand_register(micron_nand);
}
SPINAND_MOUDLE_INIT(micron_nand_init);
