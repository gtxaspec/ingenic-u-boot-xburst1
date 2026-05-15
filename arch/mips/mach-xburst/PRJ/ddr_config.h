#ifndef __DDR_CONFIG_H
#define __DDR_CONFIG_H

#include <config.h>
#include <common.h>
#include <linux/delay.h>
#include <ddr/ddr_common.h>
#include <ddr/ddrc.h>
#include <generated/ddr_reg_values.h>
#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/arch/cpm.h>

DECLARE_GLOBAL_DATA_PTR;

static void sys_ddrc_dump(void);
static void get_ddr_type(void);

#ifdef CONFIG_DDR_SOFT_TRAIN
static int ddr_mem_pattern(void);
static void ddrp_training_soft_read_train(u32 dq);
static void ddrp_training_soft_write_train(void);
static void ddrp_training_read_calib_bypass(void);
static void ddrp_training_read_train_bypass(u32 dqs0, u32 dqs1, u32 dq);
static void ddrp_training_write_train_bypass_dq(u32 dq);
// static void ddrp_training_write_train_bypass_dqs(u32 dqs0, u32 dqs1);
#else
#define ddr_mem_pattern()
#define ddrp_training_soft_read_train(dq)
#define ddrp_training_soft_write_train()
#define ddrp_training_read_calib_bypass()
#define ddrp_training_read_train_bypass(dqs0, dqs1, dq)
#define ddrp_training_write_train_bypass_dq(dq)
// #define ddrp_training_write_train_bypass_dqs(dqs0, dqs1)
#endif

#ifdef CONFIG_DDR_HARDWARE_TRAIN
static void ddrp_training_write_leveling(void);
static void ddrp_training_read_training(void);
static void ddrp_training_write_training(void);
#else
#define ddrp_training_write_leveling()
#define ddrp_training_read_training()
#define ddrp_training_write_training()
#endif

static void ddrp_training(u32 *p);
static void ddrp_dqs_calibration(void);
static void ddrp_map_set(u32 ddr_id);
static void ddrc_init(u32 kgd_rtt_dic);
static void ddrp_init(u32 ddrc_reset);
static void ddrp_set_drv_odt(u32 *p);
static void ddrc_set_port_priority(void);
static void ddrc_dfi_init(void);

void dump_ddr_par(u32 *ddr_drv_config);
void get_ddr_par(u32 *ddr_drv_config, int par_size);


#ifdef CONFIG_DDR_PARAMS_STRUCT
void matches_ddr_params(u32 ddr_id);
#else
#define matches_ddr_params(ddr_id)
#endif


#if CONFIG_DWC_DEBUG
#define dwc_debug(fmt, args...)  printf(fmt, ##args)

#define sys_ddrc(fmt, addr) dwc_debug("%s addr(0x%x) 0x%x\n", fmt, addr, ddrc_readl(addr))
#define PRINT_DDRP(var) PRINTF_DDRP(__LINE__, #var, READ_INNOPHY_REG2(var))

static void PRINTF_DDRP(u32 line, const char* name, u32 value) {
	dwc_debug("%d\t%s\r\t\t\t\t\t0x%x\n", line, name, value);
}
#else
#define dwc_debug(fmt, args...)
#define sys_ddrc(fmt, addr)
#define PRINT_DDRP(var)
#endif

#endif

