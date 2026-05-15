#ifndef __PM_H__
#define __PM_H__

#define RTC_MEMORY_START	        0xb0004000
#define RTC_MEMORY_END              0xb0005000

#define FASTBOOT_RESUME_SP		    (FASTBOOT_DATA_ADDR - 4)
#define FASTBOOT_RESUME_CODE1_LEN	64
//RESUME code start addr
#define FASTBOOT_RESUME_CODE1_ADDR	RTC_MEMORY_START
//RESUME code end addr
#define FASTBOOT_RESUME_CODE2_ADDR	(FASTBOOT_RESUME_CODE1_ADDR + FASTBOOT_RESUME_CODE1_LEN)
//RESUME code length
#define FASTBOOT_RESUME_CODE_LEN	0xe00//4*1024 - 512

/*512 bytes*/
#define FASTBOOT_DATA_ADDR		    (FASTBOOT_RESUME_CODE1_ADDR + FASTBOOT_RESUME_CODE_LEN)
#define FASTBOOT_DATA_LEN		    (RTC_MEMORY_END - FASTBOOT_DATA_ADDR)
/*
 *Because RTC ram is not enough to put resume and suspend code, put the suspend code in the 128KB position behind ORAM.
 * */
#define FASTBOOT_SLEEP_CODE_ADDR	(0xb2608000) //riscv:0xb2210000; oram:0xb2620000; tnpu:0xb2608000
#define FASTBOOT_SLEEP_CODE_LEN		(4 * 1024)
//#define FASTBOOT_SLEEP_CODE_LEN		(1 * 1024)
void load_func_to_tcsm(unsigned int tcsm_addr,unsigned int *f_addr,unsigned int size);
long long save_goto(unsigned int func);
int restore_goto(unsigned int func);

/************************************************
 *		debug interface
 ***********************************************/
/*#define DEBUG_PM*/
#define PRINT_DEBUG


#define U_IOBASE	    0xb0031000
#define UART_OFF        (0x1000)


#define OFF_TDR         (0x00)
#define OFF_LCR         (0x0C)
#define OFF_LSR         (0x14)
#define LSR_TDRQ        (1 << 5)
#define LSR_TEMT        (1 << 6)


#ifdef PRINT_DEBUG
#define TCSM_PCHAR(x)                                                   \
	*((volatile unsigned int*)(U_IOBASE+OFF_TDR)) = x;              \
while ((*((volatile unsigned int*)(U_IOBASE + OFF_LSR)) & (LSR_TDRQ | LSR_TEMT)) != (LSR_TDRQ | LSR_TEMT));
#else
#define TCSM_PCHAR(x)
#endif

#define TCSM_DELAY(x)						\
	do{							\
	register unsigned int i = x;				\
	while(i--)						\
		__asm__ volatile("nop\n\t");			\
	}while(0)

static inline void serial_put_hex(unsigned int x) {
	int i;
	unsigned int d;
	for(i = 7;i >= 0;i--) {
		d = (x  >> (i * 4)) & 0xf;
		if(d < 10) d += '0';
		else d += 'A' - 10;
		TCSM_PCHAR(d);
	}
	TCSM_PCHAR('\r');
	TCSM_PCHAR('\n');
}


#define WAIT_RTC_DONE(r,c,s)\
    do{ \
        while(r){ \
            c++; \
            if(c>5000){ \
                TCSM_PCHAR(s); \
                TCSM_DELAY(10); \
            } \
        } \
        c = 0; \
    }while(0);

static inline unsigned int  rtc_read_reg(unsigned int reg)
{
    int count = 0;
    WAIT_RTC_DONE((!((*(volatile unsigned int *)0xb0003000 >> 7) & 0x1)),count,'!');
    return *(volatile unsigned int *)reg;
}


static inline void rtc_write_reg(unsigned int reg, unsigned int val)
{
    int count = 0;
    //do{
        WAIT_RTC_DONE((!((*(volatile unsigned int *)0xb0003000 >> 7) & 0x1)),count,'o');
        *(volatile unsigned int *)0xb000303c = 0xa55a;
        WAIT_RTC_DONE((!((*(volatile unsigned int *)0xb000303c >>31) & 0x1)),count,'v');
        WAIT_RTC_DONE((!((*(volatile unsigned int *)0xb0003000 >> 7) & 0x1)),count,'e');
        *(volatile unsigned int *)reg = val;
        WAIT_RTC_DONE((!((*(volatile unsigned int *)0xb0003000 >> 7) & 0x1)),count,'r');
    //}while(val != rtc_read_reg(reg));
}

/* pm_fastboot.h */
struct pll_resume_reg {
	unsigned int cpccr;
	unsigned int cppcr;
	unsigned int cpapcr;
	unsigned int cpmpcr;
	unsigned int cpvpcr;
	unsigned int ddrcdr;
};

struct ddrc_resume_reg {
	unsigned int swctlstatic;	//0x13012328
	unsigned int swctl;			//0x13012320
	unsigned int mstr;			//0x13012000
	unsigned int init1;			//0x130120d4
	unsigned int init0;			//0x130120d0
	unsigned int init3; 		//0x130120dc
	unsigned int init4; 		//0x130120e0
	unsigned int addrmap1; 		//0x13012204
	unsigned int addrmap2; 		//0x13012208
	unsigned int addrmap3; 		//0x1301220c
	unsigned int addrmap4; 		//0x13012210
	unsigned int addrmap5; 		//0x13012214
	unsigned int addrmap6; 		//0x13012218
	unsigned int dramtmg0; 		//0x13012100
	unsigned int dramtmg1; 		//0x13012104
	unsigned int dramtmg2; 		//0x13012108
	unsigned int dramtmg3; 		//0x1301210c
	unsigned int dramtmg4; 		//0x13012110
	unsigned int dramtmg5; 		//0x13012114
	unsigned int dramtmg8; 		//0x13012120
	unsigned int dfitmg0; 		//0x13012190
	unsigned int dfitmg1; 		//0x13012194
	unsigned int dfiupd0; 		//0x130121a0
	unsigned int rfshtmg; 		//0x13012064
	unsigned int rfshctl3;      //0x13012060
	unsigned int sched; 		//0x13012250
	unsigned int pctrl0; 		//0x13012490
	unsigned int pctrl1; 		//0x13012540
	unsigned int pctrl2; 		//0x130125f0
	unsigned int pctrl3; 		//0x130126a0
	unsigned int pctrl4; 		//0x13012750
	unsigned int pctrl5; 		//0x13012800
	unsigned int pctrl6; 		//0x130128b0
	unsigned int cpm_drcg;		//0xb00000d0
};

struct ddr_phy_resume_reg {
	/* 0x8c reg_train_reg_update_en
	 * 0x0	reg_channel_en, mem_select_t
	 * 0x10	CWL_FRE_OP0
	 * 0x0c	CL_FRE_OP0
	 * 0x08	AL_FRE_OP0
	 * 0xb8	reg_phy_trfc, reg_phy_trefi
	 * 0xb0 reg_dq_wr_train_auto
	 * 0x284 training_l
	 * 0x384 traning_r
	 * */
	unsigned int mem_cfg;
	unsigned int cl;
	unsigned int al;
	unsigned int cwl;
	unsigned int rfc;
	unsigned int training_status;
	unsigned int calib_result_al;
	unsigned int calib_result_ah;

	unsigned int read_training_dm;
	unsigned int read_training_dm_h;
	unsigned int read_training_dq0_dq3;
	unsigned int read_training_dq0_dq3_h;
	unsigned int read_training_dq4_dq7;
	unsigned int read_training_dq4_dq7_h;
	unsigned int read_training_dqs;
	unsigned int read_training_dqs_h;

	unsigned int write_training_dm_dq1;
	unsigned int write_training_dm_dq1_h;
	unsigned int write_training_dq2_dq4;
	unsigned int write_training_dq2_dq4_h;
	unsigned int write_training_dq5_dq7;
	unsigned int write_training_dq5_dq7_h;
	unsigned int write_training_dqs;
	unsigned int write_training_dqs_h;
};


struct ccu_regs {
	unsigned int cfcr;
	unsigned int dmir;
	unsigned int mscr;
	unsigned int pimr;
	unsigned int mimr;
	unsigned int oimr;
	unsigned int dipr;
	unsigned int gdimr;
	unsigned int ldimr0;
	unsigned int ldimr1;
	unsigned int rer;
	unsigned int mbr0;
	unsigned int mbr1;
};
struct debug_def{
    unsigned int open_debug;
    unsigned int mem_test_addr;
};


struct store_regs {
	unsigned int resume_pc;
	unsigned int uart_index;
	unsigned int version;
	struct pll_resume_reg pll_resume_reg;
	struct ddrc_resume_reg ddrc_resume_reg;
	struct ddr_phy_resume_reg ddr_phy_resume_reg;
    struct ccu_regs cpu_resume_reg;
    struct debug_def debug;
};



struct uart_regs {
	unsigned int udllr;
	unsigned int udlhr;
	unsigned int uthr;
	unsigned int uier;
	unsigned int ufcr;
	unsigned int ulcr;
	unsigned int umcr;
	unsigned int uspr;
	unsigned int isr;
	unsigned int umr;
	unsigned int uacr;
	unsigned int urcr;
	unsigned int utcr;

};


struct ost_regs {
	unsigned int ostccr;
	unsigned int oster;
	unsigned int ostcr;
	unsigned int ostfr;
	unsigned int ostmr;
	unsigned int ostdfr;
	unsigned int ostcnt;

	unsigned int g_ostccr;
	unsigned int g_oster;
	unsigned int g_ostcr;
	unsigned int g_ostcnth;
	unsigned int g_ostcntl;
	unsigned int g_ostcntb;
};

struct cpm_regs {
	unsigned int cpm_sftint;
	unsigned int cpsppr;
	unsigned int cpspr;
	unsigned int lcr;
	unsigned int pswc0st;
	unsigned int pswc1st;
	unsigned int pswc2st;
	unsigned int pswc3st;
	unsigned int clkgr0;
	unsigned int clkgr1;
	unsigned int mestsel;
	unsigned int srbc;
	unsigned int exclk_ds;
	unsigned int memory_pd0;
	unsigned int memory_pd1;
	unsigned int slbc;
	unsigned int slpc;
	unsigned int opcr;
	unsigned int rsr;
};

/*************************************************************************
 * DDR Controller Registers
 *************************************************************************/

#define DDR_MEM_PHY_BASE		0x20000000

#define CPM_DRCG			0xb00000d0

#define DDRC_STATUS			0x0
#define DDRC_CFG			0x8
#define DDRC_CTRL			0x10
#define DDRC_LMR			0x18
#define DDRC_DLP			0x20
#define DDRC_AUTOSR_EN		0x28
#define DDRC_AUTOSR_CNT		0x30
#define DDRC_REFCNT			0x38
#define DDRC_TIMING(n)		(0x40 + 8 * (n - 1))
#define DDRC_MMAP0			0x78
#define DDRC_MMAP1			0x80
#define DDRC_BWCFG			0x88
#define DDRC_BWSTP			0x90
#define DDRC_BWP0WR			0x98
#define DDRC_BWP1WR			0xa8
#define DDRC_BWP2WR			0xb8
#define DDRC_BWP3WR			0xc8
#define DDRC_HREGPRO		0xd8
#define DDRC_DBGEN	    	0xE0
#define DDRC_DBGINFO	   	0xE8
#define DDRC_DWCFG			(DDRC_APB_OFFSET + 0x00)
#define DDRC_DWSTATUS		(DDRC_APB_OFFSET + 0x04)
#define DDRC_REMAP(n)		(DDRC_APB_OFFSET + 0x08 + 4 * (n - 1))
#define DDRC_CPAC			(DDRC_APB_OFFSET + 0x1c)
#define DDRC_CCHC(n)		(DDRC_APB_OFFSET + 0x20 + 4 * (n))
#define DDRC_CSCHC(n)		(DDRC_APB_OFFSET + 0x40 + 4 * (n))
#define DDRC_CMONC(n)		(DDRC_APB_OFFSET + 0x50 + 4 * (n))
#define DDRC_CGUC0			(DDRC_APB_OFFSET + 0x64)
#define DDRC_CGUC1			(DDRC_APB_OFFSET + 0x68)
#define DDRC_PREGPRO		(DDRC_APB_OFFSET + 0x6c)
#define DDRC_BUFCFG	    	(DDRC_APB_OFFSET + 0x70)

/*************************************************************************
 * DDRP (DDR Innophy registers)
 *************************************************************************/

/* phy reg offset by index sequence from phy doc */
#define DDRP_REG_OFFSET_RAINING_CTRL        (0x2)
#define DDRP_REG_OFFSET_WR_LEVEL1           (0x3)//not find in fpga config
#define DDRP_REG_OFFSET_WR_LEVEL2           (0x4)//not find in fpga config
#define DDRP_REG_OFFSET_WL_DONE             (0x62)//not find in fpga config

#define DDRP_REG_OFFSET_WRLEVEL_RESULT_LOW_A     (0x70+0x26)
#define DDRP_REG_OFFSET_WRLEVEL_RESULT_LOW_B     (0xa0+0x26)
#define DDRP_REG_OFFSET_WRLEVEL_RESULT_HIGH_A    (0x70+0x27)
#define DDRP_REG_OFFSET_WRLEVEL_RESULT_HIGH_B    (0xa0+0x27)

/* phy reg offset by ddrc, used by ddr_write and ddr_read function */
#define DDRP_INNOPHY_INNO_PHY_RST	(DDR_PHY_OFFSET + 0x000)
#define DDRP_INNOPHY_MEM_CFG		(DDR_PHY_OFFSET + 0x004)
#define DDRP_INNOPHY_TRAINING_CTRL  (DDR_PHY_OFFSET + DDRP_REG_OFFSET_RAINING_CTRL*4)
#define DDRP_INNOPHY_INNO_WR_LEVEL1 (DDR_PHY_OFFSET + DDRP_REG_OFFSET_WR_LEVEL1*4)//not find in fpga config
#define DDRP_INNOPHY_INNO_WR_LEVEL2 (DDR_PHY_OFFSET + DDRP_REG_OFFSET_WR_LEVEL2*4)//not find in fpga config
#define DDRP_INNOPHY_CL				(DDR_PHY_OFFSET + 0x014)
#define DDRP_INNOPHY_AL				(DDR_PHY_OFFSET + 0x018)
#define DDRP_INNOPHY_CWL			(DDR_PHY_OFFSET + 0x01c)
#define DDRP_INNOPHY_DQ_WIDTH		(DDR_PHY_OFFSET + 0x034)
#define DDRP_INNOPHY_PLL_FBDIVL		(DDR_PHY_OFFSET + 0x140)
#define DDRP_INNOPHY_PLL_FBDIVH		(DDR_PHY_OFFSET + 0x144)
#define DDRP_INNOPHY_PLL_PDIV		(DDR_PHY_OFFSET + 0x148)
#define DDRP_INNOPHY_PLL_CTRL		(DDR_PHY_OFFSET + 0x14c)
//#define DDRP_INNOPHY_DLL_LOCK		(DDR_PHY_OFFSET + 0x104)
#define DDRP_INNOPHY_PLL_LOCK		(DDR_PHY_OFFSET + 0x180)
#define DDRP_INNOPHY_CALIB_DONE		(DDR_PHY_OFFSET + 0x184)
#define DDRP_INNOPHY_WL_DONE		(DDR_PHY_OFFSET + DDRP_REG_OFFSET_WL_DONE*4)
#define DDRP_INNOPHY_INIT_COMP		(DDR_PHY_OFFSET + 0x110)
//#define DDRP_INNOPHY_BIST_RES		(DDR_PHY_OFFSET + 0x118)

#define DDRP_INNOPHY_CALIB_DELAY_AL_RESULT1 (DDR_PHY_OFFSET + 0x26c)//0x9b left for cyc_dll result
#define DDRP_INNOPHY_CALIB_DELAY_AL_RESULT2 (DDR_PHY_OFFSET + 0x270)//0x9c left for dll and oph result
#define DDRP_INNOPHY_CALIB_DELAY_AH_RESULT1 (DDR_PHY_OFFSET + 0x274)//0x9d right for cyc_dll result
#define DDRP_INNOPHY_CALIB_DELAY_AH_RESULT2 (DDR_PHY_OFFSET + 0x278)//0x9e right for dll and oph result

#define DDRP_INNOPHY_CALIB_DELAY_AL1    (DDR_PHY_OFFSET + 0x1c0)//0x70 in ddr calibration
#define DDRP_INNOPHY_CALIB_DELAY_AL2    (DDR_PHY_OFFSET + 0x1c4)//0x71 in ddr calibration
#define DDRP_INNOPHY_CALIB_DELAY_AH1    (DDR_PHY_OFFSET + 0x200)//0x80 in ddr calibration
#define DDRP_INNOPHY_CALIB_DELAY_AH2    (DDR_PHY_OFFSET + 0x204)//0x81 in ddr calibration

#define DDRP_INNOPHY_WL_MODE1		(DDR_PHY_OFFSET + 0x00c)
#define DDRP_INNOPHY_WL_MODE2		(DDR_PHY_OFFSET + 0x010)
//#define DDRP_INNOPHY_WL_DONE		(DDR_PHY_OFFSET + 0x100)

#define DDRP_INNOPHY_TRAINING_2c	(DDR_PHY_OFFSET + 0x0b0)
#define DDRP_INNOPHY_TRAINING_3c	(DDR_PHY_OFFSET + 0x0f0)
#define DDRP_INNOPHY_TRAINING_4c	(DDR_PHY_OFFSET + 0x130)
#define DDRP_INNOPHY_TRAINING_5c	(DDR_PHY_OFFSET + 0x170)
#define REG32(addr) *(volatile unsigned int *)(addr)


#define DDRP_TRAINING_CTRL_DSCSE_BP (1 << 1)
#define DDRP_TRAINING_CTRL_DSACE_START  (1 << 0)
#define ddr_writel(reg, value)	(REG32(DDRC_BASE + reg) = (value))
#define ddr_readl(reg)		REG32(DDRC_BASE + reg)

#endif/*PM*/
