// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __JZ_SFC_H__
#define __JZ_SFC_H__

#ifndef CONFIG_SPL_BUILD
#include <linux/list.h>
#endif

#define SFC_REG32(addr)               REG32(SFC_BASE + (addr))

/* SFC register */
#define SFC_GLB0                      (0x0000)
#define SFC_DEV_CONF                  (0x0004)
#define SFC_DEV_STA_EXP               (0x0008)
#define SFC_DEV_STA_RT                (0x000c)
#define SFC_DEV_STA_MSK               (0x0010)
#define SFC_TRAN_CONF0(n)             (0x0014 + (n * 4))
#define SFC_TRAN_LEN                  (0x002c)
#define SFC_DEV_ADDR(n)               (0x0030 + (n * 4))
#define SFC_DEV_ADDR_PLUS(n)          (0x0048 + (n * 4))
#define SFC_MEM_ADDR                  (0x0060)
#define SFC_TRIG                      (0x0064)
#define SFC_SR                        (0x0068)
#define SFC_SCR                       (0x006c)
#define SFC_INTC                      (0x0070)
#define SFC_FSM                       (0x0074)
#define SFC_CGE                       (0x0078)
#define SFC_CMD_IDX                   (0x007c)
#define SFC_COL_ADDR                  (0x0080)
#define SFC_ROW_ADDR                  (0x0084)
#define SFC_STA_ADDR0                 (0x0088)
#define SFC_STA_ADDR1                 (0x008c)
#define SFC_DES_ADDR                  (0x0090)
#define SFC_GLB1                      (0x0094)
#define SFC_DEV1_STA_RT               (0x0098)
#define SFC_TRAN_CONF1(n)             (0x009c + (n * 4))
#define SFC_CDT                       (0x0800)
#define SFC_RM_DR                     (0x1000)

/* For SFC_GLB0 */
#define GLB_POLL_TIME_OFFSET          (16)
#define GLB_POLL_TIME_MSK             (0xffff << GLB_POLL_TIME_OFFSET)
#define GLB_DES_EN                    (1 << 15)
#define GLB_CDT_EN                    (1 << 14)
#define GLB_TRAN_DIR_OFFSET           (13)
#define GLB_TRAN_DIR                  (1 << GLB_TRAN_DIR_OFFSET)
#define GLB_TRAN_DIR_WRITE            (1)
#define GLB_TRAN_DIR_READ             (0)
#define GLB_THRESHOLD_OFFSET          (7)
#define GLB_THRESHOLD_MSK             (0x3f << GLB_THRESHOLD_OFFSET)
#define GLB_OP_MODE                   (1 << 6)
#define GLB_OP_OFF                    (6)
#define SLAVE_MODE                    (0x0)
#define DMA_MODE                      (0x1)
#define GLB_PHASE_NUM_OFFSET          (3)
#define GLB_PHASE_NUM_MSK             (0x7 << GLB_PHASE_NUM_OFFSET)
#define GLB_WP_EN                     (1 << 2)
#define GLB_BURST_MD_OFFSET           (0)
#define GLB_BURST_MD_MSK              (0x3 << GLB_BURST_MD_OFFSET)

/* For SFC_DEV_CONF */
#define DEV_CONF_STA_ENDIAN           (31)
#define STA_ENDIAN_LSB                (0)
#define STA_ENDIAN_MSB                (1)
#define DEV_CONF_SMP_DELAY_OFFSET     (16)
#define DEV_CONF_SMP_DELAY_MSK        (0x1f << DEV_CONF_SMP_DELAY_OFFSET)
#define DEV_CONF_SMP_DELAY_0          (0)
#define DEV_CONF_SMP_DELAY_45         (1)
#define DEV_CONF_SMP_DELAY_90         (2)
#define DEV_CONF_SMP_DELAY_135        (3)
#define DEV_CONF_SMP_DELAY_180        (4)
#define DEV_CONF_SMP_DELAY_225        (5)
#define DEV_CONF_SMP_DELAY_270        (6)
#define DEV_CONF_SMP_DELAY_315        (7)
#define DEV_CONF_SMP_DELAY_1          (8)
#define DEV_CONF_SMP_DELAY_2          (16)
#define DEV_CONF_CMD_TYPE             (0x1 << 15)
#define DEV_CONF_STA_TYPE_OFFSET      (13)
#define DEV_CONF_STA_TYPE_MSK         (0x3 << DEV_CONF_STA_TYPE_OFFSET)
#define DEV_CONF_THOLD_OFFSET         (11)
#define DEV_CONF_THOLD_MSK            (0x3 << DEV_CONF_THOLD_OFFSET)
#define DEV_CONF_TSETUP_OFFSET        (9)
#define DEV_CONF_TSETUP_MSK           (0x3 << DEV_CONF_TSETUP_OFFSET)
#define DEV_CONF_TSH_OFFSET           (5)
#define DEV_CONF_TSH_MSK              (0xf << DEV_CONF_TSH_OFFSET)
#define DEV_CONF_CPHA                 (0x1 << 4)
#define DEV_CONF_CPOL                 (0x1 << 3)
#define DEV_CONF_CEDL                 (0x1 << 2)
#define DEV_CONF_HOLDDL               (0x1 << 1)
#define DEV_CONF_WPDL                 (0x1 << 0)

/* For SFC_TRAN_CONF0 */
#define TRAN_CONF0_CLK_MODE           (29)
#define TRAN_CONF0_CLK_MODE_MSK       (0x7 << TRAN_CONF0_CLK_MODE)
#define TRAN_CONF0_CLK_MODE_SSS       (0)
#define TRAN_CONF0_CLK_MODE_SSD       (1)
#define TRAN_CONF0_CLK_MODE_SDS       (2)
#define TRAN_CONF0_CLK_MODE_SDD       (3)
#define TRAN_CONF0_CLK_MODE_DSS       (4)
#define TRAN_CONF0_CLK_MODE_DSD       (5)
#define TRAN_CONF0_CLK_MODE_DDS       (6)
#define TRAN_CONF0_CLK_MODE_DDD       (7)
#define TRAN_CONF0_ADDR_WIDTH_OFFSET  (26)
#define TRAN_CONF0_ADDR_WIDTH_MSK     (0x7 << TRAN_CONF0_ADDR_WIDTH_OFFSET)
#define TRAN_CONF0_POLLEN             (1 << 25)
#define TRAN_CONF0_POLL_OFFSET        (25)
#define TRAN_CONF0_CMDEN              (1 << 24)
#define TRAN_CONF0_FMAT               (1 << 23)
#define TRAN_CONF0_FMAT_OFFSET        (23)
#define TRAN_CONF0_DMYBITS_OFFSET     (17)
#define TRAN_CONF0_DMYBITS_MSK        (0x3f << TRAN_CONF0_DMYBITS_OFFSET)
#define TRAN_CONF0_DATEEN             (1 << 16)
#define TRAN_CONF0_DATEEN_OFFSET      (16)
#define TRAN_CONF0_CMD_OFFSET         (0)
#define TRAN_CONF0_CMD_MSK            (0xffff << TRAN_CONF0_CMD_OFFSET)
#define TRAN_CONF0_CMD_LEN            (1 << 15)

/* For SFC_TRAN_CONF1 */
#define TRAN_CONF1_DATA_ENDIAN        (1 << 18)
#define TRAN_CONF1_DATA_ENDIAN_OFFSET (18)
#define TRAN_CONF1_DATA_ENDIAN_LSB    (0)
#define TRAN_CONF1_DATA_ENDIAN_MSB    (1)
#define TRAN_CONF1_WORD_UNIT_OFFSET   (16)
#define TRAN_CONF1_WORD_UNIT_MSK      (3 << 16)
#define TRAN_CONF1_TRAN_MODE_OFFSET   (4)
#define TRAN_CONF1_TRAN_MODE_MSK      (0xf << TRAN_CONF1_TRAN_MODE_OFFSET)
#define TRAN_CONF1_SPI_STANDARD       (0x0)
#define TRAN_CONF1_SPI_DUAL           (0x1)
#define TRAN_CONF1_SPI_QUAD           (0x5)
#define TRAN_CONF1_SPI_IO_QUAD        (0x6)
#define TRAN_CONF1_SPI_OCTAL          (0x9)
#define TRAN_CONF1_SPI_IO_OCTAL       (0xa)
#define TRAN_CONF1_SPI_FULL_OCTAL     (0xb)

/* For SFC_TRIG */
#define TRIG_FLUSH                    (1 << 2)
#define TRIG_STOP                     (1 << 1)
#define TRIG_START                    (1 << 0)

/* For SFC_SR */
#define FIFONUM_OFFSET                (16)
#define FIFONUM_MSK                   (0x7f << FIFONUM_OFFSET)
#define BUSY_OFFSET                   (5)
#define BUSY_MSK                      (0x3 << BUSY_OFFSET)
#define SR_END                        (1 << 4)
#define TRAN_REQ                      (1 << 3)
#define RECE_REQ                      (1 << 2)
#define OVER                          (1 << 1)
#define UNDER                         (1 << 0)

/* For SFC_SCR */
#define CLR_END                       (1 << 4)
#define CLR_TREQ                      (1 << 3)
#define CLR_RREQ                      (1 << 2)
#define CLR_OVER                      (1 << 1)
#define CLR_UNDER                     (1 << 0)

/* For SFC_INTC */
#define MASK_END                      (1 << 4)
#define MASK_TREQ                     (1 << 3)
#define MASK_RREQ                     (1 << 2)
#define MASK_OVER                     (1 << 1)
#define MASK_UNDR                     (1 << 0)

// SFC_CMD_IDX
#define CMD_IDX_MSK                   (0x3f)
#define CDT_DATAEN_MSK                (0x1 << 31)
#define CDT_DATAEN_OFF                (31)
#define CDT_DIR_MSK                   (0x1 << 30)
#define CDT_DIR_OFF                   (30)

/* For SFC_GLB0 */
#define GLB1_DQS_EN                   (1 << 2)
#define GLB1_CHIP_SEL_OFFSET          (0)
#define GLB1_CHIP_SEL_MSK             (0x3)
#define GLB1_CHIP_SEL_0               (0)
#define GLB1_CHIP_SEL_1               (1)
#define GLB1_CHIP_SEL_01              (2)

#define SFC_DMA_OPS                   (1)
#define SFC_CPU_OPS                   (0)

#define TM_STD_SPI                    (0)
#define TM_DI_DO_SPI                  (1)
#define TM_DIO_SPI                    (2)
#define TM_FULL_DIO_SPI               (3)
#define TM_QI_QO_SPI                  (5)
#define TM_QIO_SPI                    (6)
#define TM_FULL_QIO_SPI               (7)
#define TM_OCTAL_SPT                  (9)
#define TM_OCTAL_IO_SPI               (10)
#define TM_OCTAL_FULL_SPI             (11)

#define DEFAULT_CDT                   (1)
#define UPDATE_CDT                    (2)
#define DEFAULT_ADDRSIZE              (3)
#define DEFAULT_ADDRMODE              (0)

#define THRESHOLD                     (32)

#define DEF_ADDR_LEN                  (3)
#define DEF_TCHSH                     (5)
#define DEF_TSLCH                     (5)
#define DEF_TSHSL_R                   (20)
#define DEF_TSHSL_W                   (50)


#ifdef SFC_NOR_DEBUG
#define sfc_debug(fmt, args...) printf(fmt, ##args)
static void __maybe_unused sfc_dump_reg(void)
{
	printf("[0X%08x]SFC_GLB0            0x%08x\n", SFC_GLB0, SFC_REG32(SFC_GLB0));
	printf("[0X%08x]SFC_DEV_CONF        0x%08x\n", SFC_DEV_CONF, SFC_REG32(SFC_DEV_CONF));
	printf("[0X%08x]SFC_DEV_STA_EXP     0x%08x\n", SFC_DEV_STA_EXP, SFC_REG32(SFC_DEV_STA_EXP));
	printf("[0X%08x]SFC_DEV_STA_RT      0x%08x\n", SFC_DEV_STA_RT, SFC_REG32(SFC_DEV_STA_RT));
	printf("[0X%08x]SFC_DEV_STA_MSK     0x%08x\n", SFC_DEV_STA_MSK, SFC_REG32(SFC_DEV_STA_MSK));
	printf("[0X%08x]SFC_TRAN_CONF0      0x%08x\n", SFC_TRAN_CONF0(0), SFC_REG32(SFC_TRAN_CONF0(0)));
	printf("[0X%08x]SFC_TRAN_LEN        0x%08x\n", SFC_TRAN_LEN, SFC_REG32(SFC_TRAN_LEN));
	printf("[0X%08x]SFC_DEV_ADDR        0x%08x\n", SFC_DEV_ADDR(0), SFC_REG32(SFC_DEV_ADDR(0)));
	printf("[0X%08x]SFC_DEV_ADDR_PLUS   0x%08x\n", SFC_DEV_ADDR_PLUS(0), SFC_REG32(SFC_DEV_ADDR_PLUS(0)));
	printf("[0X%08x]SFC_MEM_ADDR        0x%08x\n", SFC_MEM_ADDR, SFC_REG32(SFC_MEM_ADDR));
	printf("[0X%08x]SFC_TRIG            0x%08x\n", SFC_TRIG, SFC_REG32(SFC_TRIG));
	printf("[0X%08x]SFC_SR              0x%08x\n", SFC_SR, SFC_REG32(SFC_SR));
	printf("[0X%08x]SFC_SCR             0x%08x\n", SFC_SCR, SFC_REG32(SFC_SCR));
	printf("[0X%08x]SFC_INTC            0x%08x\n", SFC_INTC, SFC_REG32(SFC_INTC));
	printf("[0X%08x]SFC_FSM             0x%08x\n", SFC_FSM, SFC_REG32(SFC_FSM));
	printf("[0X%08x]SFC_CGE             0x%08x\n", SFC_CGE, SFC_REG32(SFC_CGE));
	printf("[0X%08x]SFC_CMD_IDX         0x%08x\n", SFC_CMD_IDX, SFC_REG32(SFC_CMD_IDX));
	printf("[0X%08x]SFC_COL_ADDR        0x%08x\n", SFC_COL_ADDR, SFC_REG32(SFC_COL_ADDR));
	printf("[0X%08x]SFC_ROW_ADDR        0x%08x\n", SFC_ROW_ADDR, SFC_REG32(SFC_ROW_ADDR));
	printf("[0X%08x]SFC_STA_ADDR0       0x%08x\n", SFC_STA_ADDR0, SFC_REG32(SFC_STA_ADDR0));
	printf("[0X%08x]SFC_STA_ADDR1       0x%08x\n", SFC_STA_ADDR1, SFC_REG32(SFC_STA_ADDR1));
	printf("[0X%08x]SFC_DES_ADDR        0x%08x\n", SFC_DES_ADDR, SFC_REG32(SFC_DES_ADDR));
	printf("[0X%08x]SFC_GLB1            0x%08x\n", SFC_GLB1, SFC_REG32(SFC_GLB1));
	printf("[0X%08x]SFC_DEV1_STA_RT     0x%08x\n", SFC_DEV1_STA_RT, SFC_REG32(SFC_DEV1_STA_RT));
	printf("[0X%08x]SFC_TRAN_CONF1      0x%08x\n", SFC_TRAN_CONF1(0), SFC_REG32(SFC_TRAN_CONF1(0)));
}
#else
#define sfc_dump_reg()
#define sfc_debug(fmt, args...)
#endif

typedef union sfc_reg
{
	/** raw register data */
	uint32_t b32;

	/** register bits */
	struct
	{
		unsigned burst_md : 2;
		unsigned wp_en : 1;
		unsigned phase_num : 3;
		unsigned op_mode : 1;
		unsigned threshold : 6;
		unsigned tran_dir : 1;
		unsigned cdt_en : 1;
		unsigned des_en : 1;
		unsigned poll_time : 16;
	} sfc_glb0; // 0x00
	struct
	{
		unsigned chip_sel : 2;
		unsigned dqs_en : 1;
		unsigned reserved2 : 29;
	} sfc_glb1; // 0x94
	struct
	{
		unsigned wp_dl : 1;
		unsigned hold_dl : 1;
		unsigned ce_dl : 1;
		unsigned cpol : 1;
		unsigned cpha : 1;
		unsigned tsh : 4;
		unsigned tsetup : 2;
		unsigned thold : 2;
		unsigned sta_type : 2;
		unsigned cmd_type : 1;
		unsigned smp_delay : 5;
		unsigned reserved : 10;
		unsigned sta_endian : 1;
	} sfc_dev_conf; // 0x04

	struct
	{
		unsigned dev_sta_exp : 32;
	} sfc_dev_sta_exp; // 0x08
	struct
	{
		unsigned dev0_sta_rt : 32;
	} sfc_dev0_sta_rt; // 0x0C
	struct
	{
		unsigned dev1_sta_rt : 32;
	} sfc_dev1_sta_rt; // 0x98
	struct
	{
		unsigned dev_sta_msk : 32;
	} sfc_dev_sta_msk; // 0x10

	/* 传输配置寄存器组 */
	struct
	{
		unsigned cmd : 16;
		unsigned data_en : 1;
		unsigned dmy_bits : 6;
		unsigned phase_format : 1;
		unsigned cmd_en : 1;
		unsigned poll_en : 1;
		unsigned addr_width : 3;
		unsigned clk_mode : 3; //PRJ keep 0
	} sfc_tran_cfg0; // 0x14-0x28

	struct
	{
		unsigned reserved1 : 4;
		unsigned tran_md : 4;
		unsigned reserved2 : 8;
		unsigned word_unit : 2;
		unsigned data_endian : 1; // 0:LSB 1:MSB
		unsigned reserved3 : 13;
	} sfc_tran_cfg1; // 0x9C-0xB0

	struct
	{
		unsigned tran_len : 32;
	} sfc_tran_len; // 0x2C

	/* 地址寄存器组 */
	struct
	{
		unsigned dev_addr : 32;
	} sfc_dev_addr; // 0x30-0x44
	struct
	{
		unsigned dev_addr_plus : 32;
	} sfc_dev_addr_plus; // 0x48-0x5C
	struct
	{
		unsigned mem_addr : 32;
	} sfc_mem_addr; // 0x60
	struct
	{
		unsigned start : 1;
		unsigned stop : 1;
		unsigned flush : 1;
		unsigned reserved : 29;
	} sfc_trig; // 0x64
	struct
	{
		unsigned under : 1;
		unsigned over : 1;
		unsigned rece_req : 1;
		unsigned tran_req : 1;
		unsigned end : 1;
		unsigned busy : 2;
		unsigned working : 1;
		unsigned reserved1 : 8;
		unsigned fifo_num : 7;
		unsigned reserved2 : 9;
	} sfc_sr; // 0x68
	struct
	{
		unsigned clr_under : 1;
		unsigned clr_over : 1;
		unsigned clr_rreq : 1;
		unsigned clr_treq : 1;
		unsigned clr_end : 1;
		unsigned reserved : 27;
	} sfc_scr; // 0x6C
	struct
	{
		unsigned mask_under : 1;
		unsigned mask_over : 1;
		unsigned mask_rreq : 1;
		unsigned mask_treq : 1;
		unsigned mask_end : 1;
		unsigned reserved : 27;
	} sfc_intc; // 0x70

	struct
	{
		unsigned fsm_rmc : 3;
		unsigned reserved : 1;
		unsigned fsm_dmac : 4;
		unsigned fsm_spi : 8;
		unsigned fsm_ahb : 4;
		unsigned reserved2 : 12;
	} sfc_fsm; // 0x74
	struct
	{
		unsigned cge_reg_en : 1;
		unsigned cge_spi_en : 1;
		unsigned cge_rmc_en : 1;
		unsigned cge_dma_en : 1;
		unsigned reserved1 : 1;
		unsigned cge_flow_en : 1;
		unsigned reserved2 : 26;
	} sfc_cge; // 0x78
	struct
	{
		unsigned cmd_idx : 6;
		unsigned reserved1 : 24;
		unsigned cdt_dir : 1;
		unsigned cdt_data_en : 1;
	} sfc_cmd_idx; // 0x7C
	struct
	{
		unsigned col_addr : 32;
	} sfc_col_addr; // 0x80
	struct
	{
		unsigned row_addr : 32;
	} sfc_row_addr; // 0x84
	struct
	{
		unsigned sta_addr0 : 32;
	} sfc_sta_addr0; // 0x88
	struct
	{
		unsigned sta_addr1 : 32;
	} sfc_sta_addr1; // 0x8C
	struct
	{
		unsigned des_addr : 32;
	} sfc_des_addr; // 0x90
} sfc_reg_t;

#endif


