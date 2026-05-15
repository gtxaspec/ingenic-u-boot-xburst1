#include <common.h>
#include <environment.h>
#include <command.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/ctype.h>
#include <asm/cacheops.h>
#include <config.h>
#include <asm/arch/cpm.h>
#include <asm/arch/clk.h>
#include <pm.h>

#define RISCV_CODE
//#define ZBOOST_DEBUG
#ifdef ZBOOST_DEBUG
#define zboost_debug(fmt, args...)    \
	do{ \
		zboost_debug(fmt, ##args);    \
	}while(0)
#else
#define zboost_debug(fmt, args...) \
	do{ \
	}while(0)
#endif


static char *wakup_source[4]={"NULL","Alarm clock wake up","Input Key wake up","Alarm&Key wake up"};
static unsigned int zboost_version=230711020;
static int zboost_first_flag = 0;
static struct store_regs *store_regs;
static int save_ddr_auto_value = 0;
static int bc_idx = 1; //uart index
#ifdef CONFIG_DDR_TYPE_DDR3
static int fastboot_resume_code[] = {
#include "./fastboot_resume_code.hex"
};
#else
static int fastboot_resume_code[] = {
#include "./fastboot_resume_code_ddr2.hex"
};
#endif
#ifdef RISCV_CODE
static int fastboot_tnpu_code[] = {
#include "./fastboot_tnpu_code.hex"
};
#endif

static noinline void fastboot_cpu_resume(void);
static noinline void fastboot_cpu_sleep(void);
static noinline unsigned int ddr_read_reg(unsigned int reg);

static noinline void rtc_ram_write_enable(void)
{
    unsigned int tmp;


	*(volatile unsigned int*)0xb00000a0 = 0x1;
	/* write RTC RAM enable */
    tmp = rtc_read_reg(0xb0003048);
    tmp &= ~(1 << 23);
    tmp &= ~(1 << 22);
    //tmp |= (1 << 24);
    tmp |= (1 << 21);
    tmp |= (1 << 20);
    rtc_write_reg(0xb0003048, tmp);

}

static noinline void rtc_ram_write_disable(void)
{
    unsigned int tmp;
    /*exit write RTC RAM enable */
    tmp = rtc_read_reg(0xb0003048);
    tmp |= (1 << 22);
    rtc_write_reg(0xb0003048, tmp);

}

static noinline void pll_store(void)
{
    struct pll_resume_reg *pll_resume_reg;

    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    pll_resume_reg = &store_regs->pll_resume_reg;

    pll_resume_reg->cpccr = *(volatile unsigned int *)0xb0000000;
    pll_resume_reg->cppcr = *(volatile unsigned int *)0xb000000c;
    pll_resume_reg->cpapcr = *(volatile unsigned int *)0xb0000010;
    pll_resume_reg->cpmpcr = *(volatile unsigned int *)0xb0000014;
    pll_resume_reg->cpvpcr = *(volatile unsigned int *)0xb00000e0;
    pll_resume_reg->ddrcdr = *(volatile unsigned int *)0xb000002c;
}

static noinline void ddrc_store(void)
{
    struct ddrc_resume_reg *ddrc_resume_reg;

    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    ddrc_resume_reg = &store_regs->ddrc_resume_reg;

    ddrc_resume_reg->swctlstatic = *(volatile unsigned int *)0xb3012328;
    ddrc_resume_reg->swctl = *(volatile unsigned int *)0xb3012320;
    ddrc_resume_reg->mstr = *(volatile unsigned int *)0xb3012000;
    ddrc_resume_reg->init1 = *(volatile unsigned int *)0xb30120d4;
    ddrc_resume_reg->init0 = *(volatile unsigned int *)0xb30120d0;
    ddrc_resume_reg->init3 = *(volatile unsigned int *)0xb30120dc;
    ddrc_resume_reg->init4 = *(volatile unsigned int *)0xb30120e0;
    ddrc_resume_reg->addrmap1 = *(volatile unsigned int *)0xb3012204;
    ddrc_resume_reg->addrmap2 = *(volatile unsigned int *)0xb3012208;
    ddrc_resume_reg->addrmap3 = *(volatile unsigned int *)0xb301220c;
	ddrc_resume_reg->addrmap4 = *(volatile unsigned int *)0xb3012210;
	ddrc_resume_reg->addrmap5 = *(volatile unsigned int *)0xb3012214;
	ddrc_resume_reg->addrmap6 = *(volatile unsigned int *)0xb3012218;
	ddrc_resume_reg->dramtmg0 = *(volatile unsigned int *)0xb3012100;
	ddrc_resume_reg->dramtmg1 = *(volatile unsigned int *)0xb3012104;
	ddrc_resume_reg->dramtmg2 = *(volatile unsigned int *)0xb3012108;
	ddrc_resume_reg->dramtmg3 = *(volatile unsigned int *)0xb301210c;
	ddrc_resume_reg->dramtmg4 = *(volatile unsigned int *)0xb3012110;
	ddrc_resume_reg->dramtmg5 = *(volatile unsigned int *)0xb3012114;
	ddrc_resume_reg->dramtmg8 = *(volatile unsigned int *)0xb3012120;
	ddrc_resume_reg->dfitmg0 = *(volatile unsigned int *)0xb3012190;
	ddrc_resume_reg->dfitmg1 = *(volatile unsigned int *)0xb3012194;
	ddrc_resume_reg->dfiupd0 = *(volatile unsigned int *)0xb30121a0;
	ddrc_resume_reg->rfshtmg = *(volatile unsigned int *)0xb3012064;
	ddrc_resume_reg->rfshctl3 = *(volatile unsigned int *)0xb3012060;
	ddrc_resume_reg->sched = *(volatile unsigned int *)0xb3012250;
	ddrc_resume_reg->pctrl0 = *(volatile unsigned int *)0xb3012490;
	ddrc_resume_reg->pctrl1 = *(volatile unsigned int *)0xb3012540;
	ddrc_resume_reg->pctrl2 = *(volatile unsigned int *)0xb30125f0;
	ddrc_resume_reg->pctrl3 = *(volatile unsigned int *)0xb30126a0;
	ddrc_resume_reg->pctrl4 = *(volatile unsigned int *)0xb3012750;
	ddrc_resume_reg->pctrl5 = *(volatile unsigned int *)0xb3012800;
	ddrc_resume_reg->pctrl6 = *(volatile unsigned int *)0xb30128b0;
}

static noinline void ddr_phy_store(void)
{
    unsigned int dq0 = 0, dq1 = 0, dq2 = 0, dq3 = 0, dq4 = 0, dq5 = 0, dq6 = 0, dq7 = 0;
	unsigned int dq0_h = 0, dq1_h = 0, dq2_h = 0, dq3_h = 0, dq4_h = 0, dq5_h = 0, dq6_h = 0, dq7_h = 0;
	unsigned int dqs = 0, dm = 0, dqs_h = 0, dm_h = 0;
	unsigned int dq0_min = 0, dq1_min = 0, dq2_min = 0, dq3_min = 0, dq4_min = 0, dq5_min = 0, dq6_min = 0, dq7_min = 0;
	unsigned int dq0_max = 0, dq1_max = 0, dq2_max = 0, dq3_max = 0, dq4_max = 0, dq5_max = 0, dq6_max = 0, dq7_max = 0;
	unsigned int dq0_min_h = 0, dq1_min_h = 0, dq2_min_h = 0, dq3_min_h = 0, dq4_min_h = 0, dq5_min_h = 0, dq6_min_h = 0, dq7_min_h = 0;
	unsigned int dq0_max_h = 0, dq1_max_h = 0, dq2_max_h = 0, dq3_max_h = 0, dq4_max_h = 0, dq5_max_h = 0, dq6_max_h = 0, dq7_max_h = 0;
	unsigned int dqs_min = 0, dqs_max = 0, dqs_min_h = 0, dqs_max_h = 0;
	unsigned int dm_min = 0, dm_max = 0, dm_min_h = 0, dm_max_h = 0;

	struct ddr_phy_resume_reg *ddr_phy_resume_reg;

    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    ddr_phy_resume_reg = &store_regs->ddr_phy_resume_reg;

	ddr_phy_resume_reg->mem_cfg         = *(volatile unsigned int *)0xb3011000;
	ddr_phy_resume_reg->cl              = *(volatile unsigned int *)0xb301100c;
	ddr_phy_resume_reg->al              = *(volatile unsigned int *)0xb3011008;
	ddr_phy_resume_reg->cwl             = *(volatile unsigned int *)0xb3011010;
	ddr_phy_resume_reg->rfc        = *(volatile unsigned int *)0xb30110b8;
	ddr_phy_resume_reg->training_status        = *(volatile unsigned int *)0xb30110b0;
	/* dqs_gating */
	ddr_phy_resume_reg->calib_result_al = *(volatile unsigned int *)0xb3011284;
	ddr_phy_resume_reg->calib_result_ah = *(volatile unsigned int *)0xb3011384;
#if defined(CONFIG_DDR_TYPE_DDR3) || defined(CONFIG_DDR_TYPE_LPDDR3)
	/* read_training */
	dq0_min = (*(volatile unsigned int *)0xb3011294 >> 24) & 0x7f;
	dq1_min = (*(volatile unsigned int *)0xb3011294 >> 16) & 0x7f;
	dq2_min = (*(volatile unsigned int *)0xb3011294 >> 8) & 0x7f;
	dq3_min = (*(volatile unsigned int *)0xb3011294) & 0x7f;
	dq4_min = (*(volatile unsigned int *)0xb3011298 >> 24) & 0x7f;
	dq5_min = (*(volatile unsigned int *)0xb3011298 >> 16) & 0x7f;
	dq6_min = (*(volatile unsigned int *)0xb3011298 >> 8) & 0x7f;
	dq7_min = (*(volatile unsigned int *)0xb3011298) & 0x7f;

	dq0_max = (*(volatile unsigned int *)0xb30112a0 >> 24) & 0x7f;
	dq1_max = (*(volatile unsigned int *)0xb30112a0 >> 16) & 0x7f;
	dq2_max = (*(volatile unsigned int *)0xb30112a0 >> 8) & 0x7f;
	dq3_max = (*(volatile unsigned int *)0xb30112a0) & 0x7f;
	dq4_max = (*(volatile unsigned int *)0xb30112a4 >> 24) & 0x7f;
	dq5_max = (*(volatile unsigned int *)0xb30112a4 >> 16) & 0x7f;
	dq6_max = (*(volatile unsigned int *)0xb30112a4 >> 8) & 0x7f;
	dq7_max = (*(volatile unsigned int *)0xb30112a4) & 0x7f;

	//dqs_min = (*(volatile unsigned int *)0xb301129c >> 8) & 0x7f;
	//dqs_max = (*(volatile unsigned int *)0xb30112a8 >> 8) & 0x7f;

	dm_min = (*(volatile unsigned int *)0xb30112ec >> 7) & 0x7f;
	dm_max = (*(volatile unsigned int *)0xb30112e8) & 0x7f;

	dq0 = (dq0_min + dq0_max) / 2;
	dq1 = (dq1_min + dq1_max) / 2;
	dq2 = (dq2_min + dq2_max) / 2;
	dq3 = (dq3_min + dq3_max) / 2;
	dq4 = (dq4_min + dq4_max) / 2;
	dq5 = (dq5_min + dq5_max) / 2;
	dq6 = (dq6_min + dq6_max) / 2;
	dq7 = (dq7_min + dq7_max) / 2;
	//dqs = (dqs_min + dqs_max) / 2;
	dqs = (*(volatile unsigned int *)0xb30112ac >> 24) & 0x7f;
	dm = (dm_min + dm_max) / 2;

	dq0_min_h = (*(volatile unsigned int *)0xb3011394 >> 24) & 0x7f;
	dq1_min_h = (*(volatile unsigned int *)0xb3011394 >> 16) & 0x7f;
	dq2_min_h = (*(volatile unsigned int *)0xb3011394 >> 8) & 0x7f;
	dq3_min_h = (*(volatile unsigned int *)0xb3011394) & 0x7f;
	dq4_min_h = (*(volatile unsigned int *)0xb3011398 >> 24) & 0x7f;
	dq5_min_h = (*(volatile unsigned int *)0xb3011398 >> 16) & 0x7f;
	dq6_min_h = (*(volatile unsigned int *)0xb3011398 >> 8) & 0x7f;
	dq7_min_h = (*(volatile unsigned int *)0xb3011398) & 0x7f;

	dq0_max_h = (*(volatile unsigned int *)0xb30113a0 >> 24) & 0x7f;
	dq1_max_h = (*(volatile unsigned int *)0xb30113a0 >> 16) & 0x7f;
	dq2_max_h = (*(volatile unsigned int *)0xb30113a0 >> 8) & 0x7f;
	dq3_max_h = (*(volatile unsigned int *)0xb30113a0) & 0x7f;
	dq4_max_h = (*(volatile unsigned int *)0xb30113a4 >> 24) & 0x7f;
	dq5_max_h = (*(volatile unsigned int *)0xb30113a4 >> 16) & 0x7f;
	dq6_max_h = (*(volatile unsigned int *)0xb30113a4 >> 8) & 0x7f;
	dq7_max_h = (*(volatile unsigned int *)0xb30113a4) & 0x7f;

	dqs_min_h = (*(volatile unsigned int *)0xb301139c >> 8) & 0x7f;
	dqs_max_h = (*(volatile unsigned int *)0xb30113a8 >> 8) & 0x7f;

	dm_min_h = (*(volatile unsigned int *)0xb30113ec >> 7) & 0x7f;
	dm_max_h = (*(volatile unsigned int *)0xb30113e8) & 0x7f;

	dq0_h = (dq0_min_h + dq0_max_h) / 2;
	dq1_h = (dq1_min_h + dq1_max_h) / 2;
	dq2_h = (dq2_min_h + dq2_max_h) / 2;
	dq3_h = (dq3_min_h + dq3_max_h) / 2;
	dq4_h = (dq4_min_h + dq4_max_h) / 2;
	dq5_h = (dq5_min_h + dq5_max_h) / 2;
	dq6_h = (dq6_min_h + dq6_max_h) / 2;
	dq7_h = (dq7_min_h + dq7_max_h) / 2;
	//dqs_h = (dqs_min_h + dqs_max_h) / 2;
	dqs_h = (*(volatile unsigned int *)0xb30113ac >> 24) & 0x7f;;
	dm_h = (dm_min_h + dm_max_h) / 2;

	/* dm */
	ddr_phy_resume_reg->read_training_dm = *(volatile unsigned int *)0xb3011228 & 0xffff80ff;
	ddr_phy_resume_reg->read_training_dm |= (dq0 << 8) & 0xffff80ff;
	ddr_phy_resume_reg->read_training_dm_h = *(volatile unsigned int *)0xb3011328 & 0xffff80ff;
	ddr_phy_resume_reg->read_training_dm_h |= (dq0_h << 8) & 0xffff80ff;
	/* dqs */
	ddr_phy_resume_reg->read_training_dqs = dqs << 24 | dqs << 8;
	ddr_phy_resume_reg->read_training_dqs_h = dqs_h << 24 | dqs_h << 8;
	/* dq0-dq7 */
	ddr_phy_resume_reg->read_training_dq0_dq3 = dq0 << 24 | dq1 << 16 | dq2 << 8 | dq3;
	ddr_phy_resume_reg->read_training_dq4_dq7 = dq4 << 24 | dq5 << 16 | dq6 << 8 | dq7;
	ddr_phy_resume_reg->read_training_dq0_dq3_h = dq0_h << 24 | dq1_h << 16 | dq2_h << 8 | dq3_h;
	ddr_phy_resume_reg->read_training_dq4_dq7_h = dq4_h << 24 | dq5_h << 16 | dq6_h << 8 | dq7_h;

	/* write training */
	/* dqs */
	/* write leveling */
	dqs = (*(volatile unsigned int *)0xb3011280 >> 16) & 0xff;
	dqs_h = (*(volatile unsigned int *)0xb3011380 >> 16) & 0xff;
	/* dm */
	dm_min = (*(volatile unsigned int *)0xb30112ec >> 23) & 0x1ff;
	dm_max = (*(volatile unsigned int *)0xb30112ec >> 14) & 0x1ff;
	dm = (dm_min + dm_max) / 2;
	dm_min_h = (*(volatile unsigned int *)0xb30113ec >> 23) & 0x1ff;
	dm_max_h = (*(volatile unsigned int *)0xb30113ec >> 14) & 0x1ff;
	dm_h = (dm_min_h + dm_max_h) / 2;
	/* dq */
	dq0_min = (*(volatile unsigned int *)0xb30112c0 >> 9) & 0x1ff;
	dq1_min = (*(volatile unsigned int *)0xb30112c0) & 0x1ff;
	dq2_min = (*(volatile unsigned int *)0xb30112c4 >> 18) & 0x1ff;
	dq3_min = (*(volatile unsigned int *)0xb30112c4 >> 9) & 0x1ff;
	dq4_min = (*(volatile unsigned int *)0xb30112c4) & 0x1ff;
	dq5_min = (*(volatile unsigned int *)0xb30112c8 >> 18) & 0x1ff;
	dq6_min = (*(volatile unsigned int *)0xb30112c8 >> 9) & 0x1ff;
	dq7_min = (*(volatile unsigned int *)0xb30112c8) & 0x1ff;

	dq0_max = (*(volatile unsigned int *)0xb30112cc >> 9) & 0x1ff;
	dq1_max = (*(volatile unsigned int *)0xb30112cc) & 0x1ff;
	dq2_max = (*(volatile unsigned int *)0xb30112e0 >> 18) & 0x1ff;
	dq3_max = (*(volatile unsigned int *)0xb30112e0 >> 9) & 0x1ff;
	dq4_max = (*(volatile unsigned int *)0xb30112e0) & 0x1ff;
	dq5_max = (*(volatile unsigned int *)0xb30112e4 >> 18) & 0x1ff;
	dq6_max = (*(volatile unsigned int *)0xb30112e4 >> 9) & 0x1ff;
	dq7_max = (*(volatile unsigned int *)0xb30112e4) & 0x1ff;
	dq0 = (dq0_min + dq0_max) / 2;
	dq1 = (dq1_min + dq1_max) / 2;
	dq2 = (dq2_min + dq2_max) / 2;
	dq3 = (dq3_min + dq3_max) / 2;
	dq4 = (dq4_min + dq4_max) / 2;
	dq5 = (dq5_min + dq5_max) / 2;
	dq6 = (dq6_min + dq6_max) / 2;
	dq7 = (dq7_min + dq7_max) / 2;

	dq0_min_h = (*(volatile unsigned int *)0xb30113c0 >> 9) & 0x1ff;
	dq1_min_h = (*(volatile unsigned int *)0xb30113c0) & 0x1ff;
	dq2_min_h = (*(volatile unsigned int *)0xb30113c4 >> 18) & 0x1ff;
	dq3_min_h = (*(volatile unsigned int *)0xb30113c4 >> 9) & 0x1ff;
	dq4_min_h = (*(volatile unsigned int *)0xb30113c4) & 0x1ff;
	dq5_min_h = (*(volatile unsigned int *)0xb30113c8 >> 18) & 0x1ff;
	dq6_min_h = (*(volatile unsigned int *)0xb30113c8 >> 9) & 0x1ff;
	dq7_min_h = (*(volatile unsigned int *)0xb30113c8) & 0x1ff;

	dq0_max_h = (*(volatile unsigned int *)0xb30113cc >> 9) & 0x1ff;
	dq1_max_h = (*(volatile unsigned int *)0xb30113cc) & 0x1ff;
	dq2_max_h = (*(volatile unsigned int *)0xb30113e0 >> 18) & 0x1ff;
	dq3_max_h = (*(volatile unsigned int *)0xb30113e0 >> 9) & 0x1ff;
	dq4_max_h = (*(volatile unsigned int *)0xb30113e0) & 0x1ff;
	dq5_max_h = (*(volatile unsigned int *)0xb30113e4 >> 18) & 0x1ff;
	dq6_max_h = (*(volatile unsigned int *)0xb30113e4 >> 9) & 0x1ff;
	dq7_max_h = (*(volatile unsigned int *)0xb30113e4) & 0x1ff;

	dq0_h = (dq0_min_h + dq0_max_h) / 2;
	dq1_h = (dq1_min_h + dq1_max_h) / 2;
	dq2_h = (dq2_min_h + dq2_max_h) / 2;
	dq3_h = (dq3_min_h + dq3_max_h) / 2;
	dq4_h = (dq4_min_h + dq4_max_h) / 2;
	dq5_h = (dq5_min_h + dq5_max_h) / 2;
	dq6_h = (dq6_min_h + dq6_max_h) / 2;
	dq7_h = (dq7_min_h + dq7_max_h) / 2;

	ddr_phy_resume_reg->write_training_dm_dq1 = dq0 << 18 | dq0 << 9 | dq1;
	ddr_phy_resume_reg->write_training_dq2_dq4 = dq2 << 18 | dq3 << 9 | dq4;
	ddr_phy_resume_reg->write_training_dq5_dq7 = dq5 << 18 | dq6 << 9 | dq7;
	ddr_phy_resume_reg->write_training_dqs = dqs << 24 | dqs << 8;

	ddr_phy_resume_reg->write_training_dm_dq1_h = dq0_h << 18 | dq0_h << 9 | dq1_h;
	ddr_phy_resume_reg->write_training_dq2_dq4_h = dq2_h << 18 | dq3_h << 9 | dq4_h;
	ddr_phy_resume_reg->write_training_dq5_dq7_h = dq5_h << 18 | dq6_h << 9 | dq7_h;
	ddr_phy_resume_reg->write_training_dqs_h = dqs_h << 24 | dqs_h << 8;
#endif
}

#if 0
static noinline void ccu_save(void)
{
    struct ccu_regs *ccu_regs = NULL;
    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    ccu_regs = &store_regs->cpu_resume_reg;

    ccu_regs->cfcr      = *(volatile unsigned int *)0xb2200fe0;//must write bit31
    ccu_regs->dmir      = *(volatile unsigned int *)0xb2200fc0;
    ccu_regs->mscr      = *(volatile unsigned int *)0xb2200060;
    ccu_regs->pimr      = *(volatile unsigned int *)0xb2200120;
    ccu_regs->mimr      = *(volatile unsigned int *)0xb2200160;
    ccu_regs->oimr      = *(volatile unsigned int *)0xb22001a0;
    //ccu_regs->dipr    = *(volatile unsigned int *)0xb22001c0;//only read reg
    ccu_regs->gdimr     = *(volatile unsigned int *)0xb22001e0;
    ccu_regs->ldimr0    = *(volatile unsigned int *)0xb2200300;
    ccu_regs->ldimr1    = *(volatile unsigned int *)0xb2200320;
    ccu_regs->rer       = *(volatile unsigned int *)0xb2200f00;
    ccu_regs->mbr0      = *(volatile unsigned int *)0xb2201000;
    ccu_regs->mbr1      = *(volatile unsigned int *)0xb2201004;

}
#endif
static noinline void debug_zboost(void)
{
    struct debug_def *debug = NULL;
    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    debug = &store_regs->debug;
    debug->open_debug = 0;
    debug->mem_test_addr = (unsigned int )0xa0600000;
}

static noinline void fastboot_cpu_resume(void)
{

#if 0
	/* PRJ delete */
	*(volatile unsigned int *)0xb2200f00 = 0xbfc00000; /* RESET entry = 0xbfc00000 ,reset value */
#endif
#if 1
	uint32_t data_pc;
    data_pc = (unsigned int)restore_goto;
	zboost_debug("data_pc is %p\n", data_pc);
#endif
	__asm__ volatile(
                     ".set push	\n\t"
                     ".set mips32r2	\n\t"
#if 0
					 "move $8, $0\n\t"
                     "lui $8, 0xb221\n\t"
					 "addiu $8, 0x1004\n\t"
					 "sw $28, 4($8)\n\t"
					 "lw $28, 0($8)\n\t"
					 "sw $28, 8($8)\n\t"
#endif
					 "jr.hb %0	\n\t"
                     "sync		\n\t"
                     "nop		\n\t"
                     ".set pop 	\n\t"
                     :
                     :"r" (restore_goto)
                     :
                    );
	//0x87f8d1d4
}

static noinline void save_resume_pc(void)
{
    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    store_regs->resume_pc = (unsigned int)fastboot_cpu_resume;
	zboost_debug("resume_pc is %p\n", store_regs->resume_pc);
}

static noinline void save_uart_index(void)
{
    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    store_regs->uart_index = bc_idx;
    store_regs->version = zboost_version;
}

static noinline void rtc_ram_store(void)
{
    struct ddr_phy_resume_reg *ddr_phy_resume_reg;

    store_regs = (struct store_regs *)FASTBOOT_DATA_ADDR;
    ddr_phy_resume_reg = &store_regs->ddr_phy_resume_reg;

    pll_store();
    ddrc_store();
    ddr_phy_store();
#if 0
	/* PRJ delete */
	ccu_save();
#endif
	debug_zboost();
    save_resume_pc();
    save_uart_index();
}

static noinline void load_func_to_rtc_ram(void)
{
	rtc_ram_write_enable();
	zboost_debug("sleep(%#x--%#x),data(%#x--%#x),actual:%d bytes.\nresume addr:(%#x--%#x),actual:%d bytes.\n",
			FASTBOOT_SLEEP_CODE_ADDR, FASTBOOT_SLEEP_CODE_ADDR + FASTBOOT_SLEEP_CODE_LEN,
			FASTBOOT_DATA_ADDR, FASTBOOT_DATA_ADDR + FASTBOOT_DATA_LEN,
			sizeof(struct store_regs),
			FASTBOOT_RESUME_CODE1_ADDR, FASTBOOT_RESUME_CODE1_ADDR + FASTBOOT_RESUME_CODE_LEN ,
			sizeof(fastboot_resume_code)
		  );

	if(save_ddr_auto_value == 0){
		memset((unsigned int *)RTC_MEMORY_START, 0xff, 4096);
	}
	memcpy((unsigned int *)FASTBOOT_RESUME_CODE1_ADDR, (unsigned int *)fastboot_resume_code, sizeof(fastboot_resume_code));

    rtc_ram_store();
    rtc_ram_write_disable();
    save_ddr_auto_value = 1;
}



static noinline int soc_pm_fastboot_config(void)
{
    volatile unsigned int tmp;
    volatile unsigned int tmp2;
    unsigned int intc1_msk;
#ifndef CONFIG_FPGA
#if 0
	unsigned int clk_gate0;
	clk_gate0 = cpm_inl(CPM_CLKGR0);
    clk_gate0 &= ~(1 << 25); //enable rtc gate
    cpm_outl(clk_gate0, CPM_CLKGR0);
#endif
#endif
    if(!zboost_first_flag)
		zboost_first_flag = 1;

	intc1_msk = *(volatile unsigned int *)0xb000102c;
    *(volatile unsigned int *)0xb000102c = intc1_msk & ~(1<<0); // RTC INT MSK

#if 0
    /*disabled RTC Alarm wakeup*/
    tmp = rtc_read_reg(0xb000302c);
    tmp &= ~(1);
    rtc_write_reg(0xb000302c, tmp);

    /*clean Alarm enable and Alarm flag*/
    tmp = rtc_read_reg(0xb0003000);
    tmp &=~(0x7F);
    rtc_write_reg(0xb0003000,tmp);
#endif
    /*clear RTC HWRSR. clean all flag*/
    rtc_write_reg(0xb0003030, 0);

    /*clear RSR status*/
    *(volatile u32*)(0xb0000008) = 0;

    /*clean WKUPPINCR.P_RST_EN*/
    tmp = rtc_read_reg(0xb0003048);
    tmp &=~(0xf);
    rtc_write_reg(0xb0003048,tmp);

	zboost_debug("start zboost test!\n");
    /*clean HWFCR*/
    rtc_write_reg(0xb0003028, 0x0);
#if 0
	tmp  = rtc_read_reg(0xb0003004);//now time
    tmp2 = rtc_read_reg(0xb0003008);//alarm time
    if((tmp + 1) <= tmp2){//alarm time
        rtc_write_reg(0xb0003004, tmp);
        rtc_write_reg(0xb0003008, tmp2 - 1);
    }else{
        zboost_debug("[%s]%d: alarm(%d) < now time(%d)\n",__func__,__LINE__,tmp2,tmp);
        rtc_write_reg(0xb0003008, rtc_read_reg(0xb0003004));//now time write alarm time
    }
#else
	//rtc_write_reg(0xb0003004, 0x100);
	//rtc_write_reg(0xb0003008, 0x102);
#endif
	zboost_debug("start zboost test!\n");
    /*clean HRCR*/
    rtc_write_reg(0xb0003024, 0);

    /* 32k rtc clk */
    tmp = rtc_read_reg(0xb0003000);
    tmp &= ~(1 << 1);
    rtc_write_reg(0xb0003000, tmp);

    return 0;
}

static noinline void ost_save(struct ost_regs *ost_regs)
{
    ost_regs->ostccr = *(volatile unsigned int *)0xb2100000;
    ost_regs->oster = *(volatile unsigned int *)0xb2100004;
    ost_regs->ostcr = *(volatile unsigned int *)0xb2100008;
    ost_regs->ostfr = *(volatile unsigned int *)0xb210000c;
    ost_regs->ostmr = *(volatile unsigned int *)0xb2100010;
    ost_regs->ostdfr = *(volatile unsigned int *)0xb2100014;
    ost_regs->ostcnt = *(volatile unsigned int *)0xb2100018;

    ost_regs->g_ostccr = *(volatile unsigned int *)0xb2000000;
    ost_regs->g_oster = *(volatile unsigned int *)0xb2000004;
    ost_regs->g_ostcr = *(volatile unsigned int *)0xb2000008;
    ost_regs->g_ostcnth = *(volatile unsigned int *)0xb200000c;
    ost_regs->g_ostcntl = *(volatile unsigned int *)0xb2000010;
    ost_regs->g_ostcntb = *(volatile unsigned int *)0xb2000014;

}
static noinline void cpm_save(struct cpm_regs *cpm_regs)
{
    cpm_regs->cpsppr = *(volatile unsigned int *)0xb0000038;
    cpm_regs->cpspr = *(volatile unsigned int *)0xb0000034;
    cpm_regs->lcr = *(volatile unsigned int *)0xb0000004;
    cpm_regs->clkgr0 = *(volatile unsigned int*)0xb0000020;
    cpm_regs->clkgr1 = *(volatile unsigned int*)0xb0000028;
    cpm_regs->mestsel = *(volatile unsigned int*)0xb00000ec;
    cpm_regs->srbc = *(volatile unsigned int*)0xb00000c4;
    cpm_regs->opcr = *(volatile unsigned int*)0xb0000024;
}
struct ost_regs ost_regs;
struct cpm_regs cpm_regs;
void sys_save(void)
{
    ost_save(&ost_regs);
    cpm_save(&cpm_regs);
}

void load_func_to_tcsm(unsigned int tcsm_addr,unsigned int *f_addr,unsigned int size)
{
    unsigned int instr;
    int offset;
    int i;
    zboost_debug("tcsm addr = %#x %p size = %d\n",tcsm_addr,f_addr,size);
    for(i = 0;i < size / 4;i++) {
        instr = f_addr[i];
#if 0
		if((instr >> 26) == 2){
            offset = instr & 0x3ffffff;
            offset = (offset << 2) - ((unsigned int)f_addr & 0xfffffff);
            if(offset > 0) {
                offset = ((unsigned int)tcsm_addr & 0xfffffff) + offset;
                instr = (2 << 26) | (offset >> 2);
            }
        }
#endif
        (*(volatile unsigned int *)(tcsm_addr + i*4)) = instr;
    }

    for(i = 0;i < size / 4;i++) {
        instr = f_addr[i];
#if 0
		if((instr >> 26) == 2){
            offset = instr & 0x3ffffff;
            offset = (offset << 2) - ((unsigned int)f_addr & 0xfffffff);
            if(offset > 0) {
                offset = ((unsigned int)tcsm_addr & 0xfffffff) + offset;
                instr = (2 << 26) | (offset >> 2);
            }
        }
#endif
        if((*(volatile unsigned int *)(tcsm_addr + i*4)) != instr){
            zboost_debug("copy sleep code error\n");
            while(1);
        }
    }
}

static noinline unsigned int ddr_read_reg(unsigned int reg)
{
    return ddr_readl(reg);
}
static noinline void ddr_write_reg(unsigned int reg,unsigned int val)
{
    do{
        ddr_writel(reg,val);
    }while(val != ddr_read_reg(reg));
}

#ifndef RISCV_CODE
static noinline void fastboot_cpu_sleep(void)
{
    unsigned int ddrc_ctrl = 0,tmp;
#if 0
    blast_dcache32();
    blast_scache64();
    __sync();
    __fast_iob();
#endif
    /*enter self refresh status,disabled auto self refresh*/
    zboost_debug("fastboot_cpu_sleep in!\n");
#if 0
	ddr_write_reg(DDRC_HREGPRO,0);
    ddr_write_reg(DDRC_AUTOSR_EN,0);//disenable auto self-refresh

    tmp = *(volatile unsigned int*)(0xa0000000);//touch

    ddrc_ctrl = ddr_read_reg(DDRC_CTRL);
    ddrc_ctrl |= 0x1<<5;//If set HIGH, hardware drives DRAM device entering self-refresh mode;
    ddr_write_reg(DDRC_CTRL,ddrc_ctrl);
    while(!(ddr_read_reg(DDRC_STATUS) & (1<<2)));//If set, DDR memory is in self refresh status.


    zboost_debug("fastboot_cpu_sleep in!\n");
#endif
	for (tmp = 0; tmp < 2; tmp++)
    {
        __asm__ volatile("nop\t\n");
        __asm__ volatile("nop\t\n");
    }
    /* bufferen_core = 0 */
    tmp = rtc_read_reg(0xb0003048);
	tmp &= ~(1 << 21);
	rtc_write_reg(0xb0003048, tmp);

    /*delay nop*/
    for (tmp = 0; tmp < 10; tmp++)
    {
        __asm__ volatile("nop\t\n");
        __asm__ volatile("nop\t\n");
    }
#if 0
	/* ddr phy pll power down */
    tmp = ddr_read_reg(DDRP_INNOPHY_PLL_CTRL);
    tmp |= (1<<3);//CMD and DQ PLL power down. Set to “0” to enable the PLL.
    ddr_write_reg(DDRP_INNOPHY_PLL_CTRL,tmp);
#endif
	/*zboost_debug notice*/
    TCSM_PCHAR('*');//only test
    /* RTC PD */
	zboost_debug("reset:\n");
    rtc_write_reg(0xb0003020, 1);
	udelay(800);
#ifndef CONFIG_FPGA
	while(1);
#else
	zboost_debug("reset:\n");
	do_reset(NULL, 0, 0, NULL);
#endif
}
#endif

void load_func_to_tnpu_ram(void)
{
    if(zboost_first_flag){
#ifdef RISCV_CODE
		memcpy((unsigned int *)FASTBOOT_SLEEP_CODE_ADDR, (unsigned int *)fastboot_tnpu_code, sizeof(fastboot_tnpu_code));
#else
		load_func_to_tcsm(FASTBOOT_SLEEP_CODE_ADDR, (unsigned int *)fastboot_cpu_sleep, FASTBOOT_SLEEP_CODE_LEN);
#endif
	}
}

static noinline void cpm_restore(struct cpm_regs *cpm_regs)
{
    *(volatile unsigned int *)0xb0000038 = cpm_regs->cpsppr;
    *(volatile unsigned int *)0xb0000034 = cpm_regs->cpspr;
    *(volatile unsigned int *)0xb0000004 = cpm_regs->lcr;
    *(volatile unsigned int *)0xb00000ec = cpm_regs->mestsel;
    *(volatile unsigned int *)0xb00000c4 = cpm_regs->srbc;
    *(volatile unsigned int *)0xb0000024 = cpm_regs->opcr;
    *(volatile unsigned int *)0xb0000020 = cpm_regs->clkgr0;
    *(volatile unsigned int *)0xb0000028 = cpm_regs->clkgr1;
}

static noinline void ost_restore(struct ost_regs *ost_regs)
{
    *(volatile unsigned int *)0xb2100000 = ost_regs->ostccr;
    *(volatile unsigned int *)0xb2100008 = ost_regs->ostcr;
    *(volatile unsigned int *)0xb210000c = ost_regs->ostfr;
    *(volatile unsigned int *)0xb2100010 = ost_regs->ostmr;
    *(volatile unsigned int *)0xb2100014 = ost_regs->ostdfr;
    *(volatile unsigned int *)0xb2100018 = ost_regs->ostcnt;
    *(volatile unsigned int *)0xb2100004 = ost_regs->oster;

    *(volatile unsigned int *)0xb2000000 = ost_regs->g_ostccr;
    *(volatile unsigned int *)0xb2000008 = ost_regs->g_ostcr;
    *(volatile unsigned int *)0xb2000004 = ost_regs->g_oster;

}

void sys_restore(void)
{
    cpm_restore(&cpm_regs);
    ost_restore(&ost_regs);
}

static inline void cpu_sync(void)
{
    __asm__ volatile(
                     ".set push     \n\t"
                     ".set mips32r2 \n\t"
                     "sync          \n\t"
                     "lw $0,0(%0)   \n\t"
                     "nop           \n\t"
                     ".set pop      \n\t"
                     ::"r" (0xb0003000));

}

static noinline int soc_pm_wakeup_source(void)
{
    int tmp  = rtc_read_reg(0xb0003030);
    tmp &= 0x3;
    zboost_debug("Zboost:%s\n",wakup_source[tmp]);

    return 0;
}

void soc_pm_wakeup_fastboot(void)
{
    cpu_sync();
    soc_pm_wakeup_source();
    sys_restore();
    cpu_sync();
}

void test_goto(unsigned int sleep_addr)
{
	zboost_debug("test_goto in\n");
	__asm__ volatile(
			".set push	\n\t"
			".set mips32r2	\n\t"
			"jr.hb %0	\n\t"
			"sync		\n\t"
			"nop		\n\t"
			".set pop 	\n\t"
			:
			: "r" (sleep_addr)
			:
			);
}

void rtc_alarm_set(int time)
{
	int default_time = 0x100;
	unsigned int tmp;
#ifndef CONFIG_FPGA
	unsigned int clk_gate0;
	unsigned int clk_gate1;
	clk_gate0 = cpm_inl(CPM_CLKGR0);
	clk_gate0 &= ~(1 << 25); //enable rtc gate
	cpm_outl(clk_gate0, CPM_CLKGR0);

	clk_gate1 = cpm_inl(CPM_CLKGR1);
	clk_gate1 &= ~(1 << 11); //enable tnpu gate
	cpm_outl(clk_gate1, CPM_CLKGR1);
#endif
#if 0
	tmp = rtc_read_reg(0xb0003048);
    tmp &= ~(1 << 24);
    rtc_write_reg(0xb0003048, tmp);
#endif
	*(volatile unsigned int*)0xb00000a0 = 0x0;

	/*set 32K */
	tmp = rtc_read_reg(0xb0003000);
	tmp &= ~(1 << 1);
	rtc_write_reg(0xb0003000, tmp);
	/* set 1ms */
	tmp = rtc_read_reg(0xb000300c);
	tmp &= 0xffff0000;
	tmp |= 0x1e;
	rtc_write_reg(0xb000300c, tmp);
	/* set time */
    rtc_write_reg(0xb0003004, default_time);
    rtc_write_reg(0xb0003008, (default_time + time));
	/* wakeup alarm */
	rtc_write_reg(0xb000302c, ((0x1a55a5a5 << 3) | 1));
	/* enable alarm */
	tmp = rtc_read_reg(0xb0003000);
	tmp &= ~(1 << 4);
	tmp |= (3 << 2);
	rtc_write_reg(0xb0003000, tmp);
}

void test_zboost (void)
{
	int ret = 0, time = 1, tmp = 0;
	unsigned int sleep_addr = 0;

	/* disable MS_EN */
	tmp = rtc_read_reg(0xb0003000);
	tmp &= ~(1 << 10);
	rtc_write_reg(0xb0003000, tmp);

	zboost_debug("start zboost test!\n");
	rtc_alarm_set(time);

	ret = soc_pm_fastboot_config();
	if(ret < 0)
	{
		zboost_debug("%s config fastboot error\n",__func__);
		return ret;
	}
	zboost_debug("load_func_to_rtc_ram, sys_save\n");
	load_func_to_rtc_ram();
	sys_save();
	zboost_debug("load_func_to_tnpu_ram\n");
	load_func_to_tnpu_ram();
	sleep_addr = FASTBOOT_SLEEP_CODE_ADDR;
#ifndef RISCV_CODE
	test_goto((unsigned int)sleep_addr);
#else
	save_goto((unsigned int)sleep_addr);
	zboost_debug("soc_pm_wakeup_fastboot\n");
	soc_pm_wakeup_fastboot();
#endif
	return;
}
