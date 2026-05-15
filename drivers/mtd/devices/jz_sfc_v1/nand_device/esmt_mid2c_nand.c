#include <errno.h>
#include <malloc.h>
#include <linux/mtd/partitions.h>
#include "../jz_sfc_nand.h"
#include "nand_common.h"

#define ESMT_MID2C_DEVICES_NUM         1
#define TSETUP		5
#define THOLD		5
#define	TSHSL_R		20
#define	TSHSL_W		20

static struct jz_nand_base_param esmt_mid2c_param[ESMT_MID2C_DEVICES_NUM] = {

	[0] = {
		/* F50L2G41XA */
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

static struct device_id_struct device_id[ESMT_MID2C_DEVICES_NUM] = {
	DEVICE_ID_STRUCT(0x24, "F50L2G41XA", &esmt_mid2c_param[0]),
};

static void esmt_mid2c_single_read(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

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


static void esmt_mid2c_quad_read(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

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

static void esmt_mid2c_single_load(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

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

static void esmt_mid2c_quad_load(struct sfc_transfer *transfer, struct cmd_info *cmd, struct flash_operation_message *op_info) {

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


static int32_t esmt_mid2c_get_read_feature(struct flash_operation_message *op_info) {

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

int esmt_mid2c_nand_init(void) {
	struct jz_nand_device *esmt_mid2c_nand;
	esmt_mid2c_nand = kzalloc(sizeof(*esmt_mid2c_nand), GFP_KERNEL);
	if(!esmt_mid2c_nand) {
		pr_err("alloc esmt_mid2c_nand struct fail\n");
		return -ENOMEM;
	}

	esmt_mid2c_nand->id_manufactory = 0x2C;
	esmt_mid2c_nand->id_device_list = device_id;
	esmt_mid2c_nand->id_device_count = ESMT_MID2C_DEVICES_NUM;

	esmt_mid2c_nand->ops.nand_read_ops.single_read = esmt_mid2c_single_read;
	esmt_mid2c_nand->ops.nand_read_ops.quad_read = esmt_mid2c_quad_read;

	esmt_mid2c_nand->ops.nand_write_ops.single_load = esmt_mid2c_single_load;
	esmt_mid2c_nand->ops.nand_write_ops.quad_load = esmt_mid2c_quad_load;

	esmt_mid2c_nand->ops.nand_read_ops.get_feature = esmt_mid2c_get_read_feature;
	return jz_spinand_register(esmt_mid2c_nand);
}
SPINAND_MOUDLE_INIT(esmt_mid2c_nand_init);
