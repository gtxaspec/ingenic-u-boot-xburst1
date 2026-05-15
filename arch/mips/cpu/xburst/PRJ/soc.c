/*
 * PRJ common routines
 *
 * Copyright (c) 2015 Ingenic Semiconductor Co.,Ltd
 * Author: Elvis <huan.wang@ingenic.com>
 * Based on: arch/mips/cpu/xburst/jz4775/jz4775.c
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/* #define DEBUG */
#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/arch/cpm.h>
#include <spl.h>
#include <asm/cacheops.h>
#include <asm/dma-default.h>

//#define CPU_DEBUG
#ifdef CONFIG_BIG_SPL
#include "testcode.c"
#endif

#ifdef CONFIG_SPL_BUILD

/* Pointer to as well as the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;
gd_t gdata __attribute__ ((section(".data")));


#ifndef CONFIG_BURNER
struct global_info ginfo __attribute__ ((section(".data"))) = {
	.extal		= CONFIG_SYS_EXTAL,
	.cpufreq	= CONFIG_SYS_CPU_FREQ,
	.ddrfreq	= CONFIG_SYS_MEM_FREQ,
	.uart_idx	= CONFIG_SYS_UART_INDEX,
	.baud_rate	= CONFIG_BAUDRATE,
};

#endif

extern void pll_init(void);
extern void sdram_init(void);
extern void test_zboost(void);
extern void validate_cache(void);

static void enable_cpu_read_ddr(void)
{
	int res = 0, res1 = 0;
	__asm__ __volatile__(		\
			".set	push	\n"	\
			".set	reorder	\n"	\
			"mfc0	%0,$9,4	\n"	\
			"li		%1,-3	\n"	\
			"and	%0,%0,%1\n"	\
			"mtc0	%0,$9,4	\n"	\
			".set	pop		\n"	\
			:"=r"(res),"=r"(res1));
	return;
}

#define cache_op(op, addr)		\
	__asm__ __volatile__(		\
			".set	push\n"		\
			".set	noreorder\n"	\
			".set	mips3\n"	\
			"cache	%0, %1\n"	\
			".set	pop\n"		\
			:			\
			: "i" (op), "R" (*(unsigned char *)(addr)))


static void flush_dcache_range_1(u32 addr, u32 size)
{
	u32 start = addr;
	for (; start < addr+size; start += CONFIG_SYS_CACHELINE_SIZE) {
		cache_op(INDEX_WRITEBACK_INV_D, start);
	}
	__asm__ __volatile__("sync");
}
static int ddr_mem_test13(void)
{
	int i = 0;
	int j = 0;
	unsigned int addr = 0;
	unsigned int value_get;
	unsigned int data;
	unsigned int test_size =  64;
	int ret = test_size / 8;
	unsigned int test_data =  0xffffffff;
	// printf("mem test13 start!\n");

	for(i = 0; i < 1; i++) {
		addr = 0xa0163ffc;
		for (j = 0; j < test_size; j+=4) {
			data = (j/4)%2?test_data:0;
			*(volatile unsigned int *)(addr+j) = data;
		}

		addr = 0xa0163ffc;
		printf("mem data:\n");
		for (j = 0; j < test_size; j+=4) {
			value_get = *(volatile unsigned int *)(addr+j);
			if(j/4%4%2)
				if(test_data == value_get)
					ret -= 1;
			if (0 == j/4%4)
				printf("%x: ", addr+j);
			printf("  %x", value_get);
			if (3 == j/4%4)
				printf("\n");
		}
	}
	// printf("mem %d end!\n", ret);

	return ret;
}

static int ddr_mem_test12(void)
{
	int i = 0;
	int j = 0;
	unsigned int addr = 0;
	unsigned int value_get;
	unsigned int data;
	unsigned int test_size =  64;
	int ret = test_size / 8;
	unsigned int test_data =  0xffffffff;
	//printf("mem test12 start!\n");

	for(i = 0; i < 1; i++) {
		addr = 0x80163ffc;
		for (j = 0; j < test_size; j+=4) {
			data = (j/4)%2?test_data:0;
			*(volatile unsigned int *)(addr+j) = data;
		}

		//flush cache
		flush_dcache_range(addr, addr+test_size);
		flush_l2cache_range(addr, addr+test_size);

		//read cached
		addr = 0x80163ffc;
		for (j = 0; j < test_size; j+=4) {
			value_get = *(volatile unsigned int *)(addr+j);
		}

		//invalid cache
		addr = 0x80163ffc;
		invalid_dcache_range(addr, addr+test_size);
		invalid_l2cache_range(addr, addr+test_size);
		//read cached
		addr = 0x80163ffc;
		debug("mem data:\n");
		for (j = 0; j < test_size; j+=4) {
			value_get = *(volatile unsigned int *)(addr+j);
			if(j/4%4%2)
				if(test_data == value_get)
					ret -= 1;
			if (0 == j/4%4)
				debug("%x: ", addr+j);
			debug("  %x", value_get);
			if (3 == j/4%4)
				debug("\n");
		}
	}
#if 0
	for(addr = 0xa0000000, i = 0; addr < 0xa1000000; addr+=4, i++)
	{
		*(u32*)addr = 0xa5a5a5a5 + i;
		value_get = *(u32*)addr;
		if(value_get =! (0xa5a5a5a5 + i))
			printf("#### err! value_get is %x, data is %x\n", value_get, (0xa5a5a5a5 + i));
	}
#endif
	//printf("mem test10 end!\n");

	return ret;
}

#ifdef CONFIG_SIMU
volatile noinline void hello_world(void)
{
#ifdef CONFIG_SIMU_DDR
	sdram_init();
	{
		volatile u32 tmp = 0;
		u32 data = 0, i = 0;
		u32 td[8] = {0x11111111, 0x22222222, 0x33333333, 0x44444444, \
			0x55555555, 0x66666666, 0x77777777, 0x88888888, 0x99999999};
		printf("%x\n", *(u32*)0xa0000000);//SVDP环境问题，环境初始化一定是需要检测第一次CPU读DDR的操作
		for (i = 0, tmp = 0xa0000000; tmp < 0xa0000020; tmp+=4, i++) {
			*(u32*)tmp = td[i];
			udelay(1);
			data = *(u32*)tmp;
			if (data != td[i]) {
				printf("\n##### add(%d) = %p, want = %x, get = %x\n", i, tmp, td[i], data);
				hang();
			}
		}
	}
#endif
#ifdef CONFIG_ZBOOST
	test_zboost();
#if 0
	{
		unsigned int tmp = 0;
		/* set time */
		writel(0x100, 0xb0003004);
		writel(0x101, 0xb0003008);
		writel(((0x1a55a5a5 << 3) | 1), 0xb000302c);
		tmp = readl(0xb0003000);
		tmp &= ~(1 << 4);
		tmp |= (3 << 2);
		writel(tmp, 0xb0003000);

		//RTC PD
		printf("reset:\n");
		writel(1, 0xb0003020);
		udelay(800);
		printf("do reset\n");
		do_reset(NULL, 0, 0, NULL);
		//test_zboost();
	}
#endif
#endif
	*(volatile unsigned int *)(0xb0011018) = 0x100;
	*(volatile unsigned int *)(0xb0011024) = 0x100;
	*(volatile unsigned int *)(0xb0011038) = 0x100;
	*(volatile unsigned int *)(0xb0011044) = 0x100;
	while(1)
	{
		*(volatile unsigned int *)(0xb0011048) = 0x100;
		*(volatile unsigned int *)(0xb0011044) = 0x100;
	}
}
#endif


void board_init_f(ulong dummy)
{
	unsigned int clk_value = 0;
	/* Set global data pointer */
	gd = &gdata;

	/* Setup global info */
#ifndef CONFIG_CMD_BURN
	gd->arch.gi = &ginfo;
#else
	gd->arch.gi = (struct global_info *)CONFIG_SPL_GINFO_BASE;
#endif

#ifdef CONFIG_BURNER
	gd->arch.gi->ddr_div = ((gd->arch.gi->cpufreq % gd->arch.gi->ddrfreq) == 0)
		? (gd->arch.gi->cpufreq / gd->arch.gi->ddrfreq)
		: (gd->arch.gi->cpufreq / gd->arch.gi->ddrfreq + 1);
#endif
#ifndef CONFIG_FPGA
	gd->arch.gi->soc.d32 = 0;
	switch (*(volatile unsigned int *)(CONFIG_SOCID_ADDRESS))
	{
		case CONFIG_SOCID_PRJ007:
			gd->arch.gi->soc.xburst.PRJ007 = 1;
			break;
		case CONFIG_SOCID_PRJ008:
			gd->arch.gi->soc.xburst.PRJ008 = 1;
			gd->arch.gi->soc.xburst.ddr_id = (*((volatile unsigned int *)(0xb354021c)) >> 16) & 0xff;
			gd->arch.gi->soc.xburst.soc_version = (*((volatile unsigned int *)(0xb354021c)) >> 24) & 0xff;
			break;
		default:
			break;
	}
#else
	gd->arch.gi->soc.xburst.PRJ008 = 1;
#endif
	gpio_init();

#ifndef CONFIG_FPGA
	/* Init uart first */
	enable_uart_clk();
#endif

#ifdef CONFIG_SPL_SERIAL_SUPPORT
	preloader_console_init();
#endif
#ifdef CPU_DEBUG
	get_deadlock_info();
#endif
#ifndef CONFIG_FPGA
	// debug("Timer init\n");
	timer_init();

#ifdef CONFIG_SPL_REGULATOR_SUPPORT
	debug("regulator set\n");
	spl_regulator_set();
#endif
#ifdef CONFIG_SIMULATION
	debug("CLK stop\n");
#endif
	clk_prepare();
#ifdef CONFIG_SIMULATION
	debug("PLL init\n");
#endif

	pll_init();

#ifdef CONFIG_SIMULATION
	debug("CLK init\n");
#endif
	clk_init();
#endif
#ifdef CONFIG_BIG_SPL
	printf("before test codes\n");
	TEST_CODES
	printf("after test codes\n");
#endif
	/* CPU allocated访问DDR开启 */
	enable_cpu_read_ddr();

#ifdef CONFIG_SIMULATION
	debug("SDRAM init\n");
#endif
#ifdef CONFIG_SIMU
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);
	hello_world();
#endif

	sdram_init();
#if 1
    // ddr_mem_test13();
	if(ddr_mem_test12())
	{
		printf("ddr error!\n");
		hang();
	}
#endif
#ifdef CONFIG_ZBOOST
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);
	test_zboost();
#endif
#if 0
	{
		unsigned int addr = 0, start = 0, end = 0, data = 0;
		start = get_timer(0);
		for (addr = 0xa0000000; addr < 0xa1000000; addr+=4)
		{
			*(volatile unsigned int *)(addr);
		}
		end = get_timer(0);
		printf("read time is %d ms, speed is %dMB/s:\n", (end - start), (100000 / (end - start)));
		start = get_timer(0);
		for (addr = 0xa0000000; addr < 0xa1000000; addr+=4)
		{
			*(volatile unsigned int *)(addr) = 0x5a5a5a5a;
		}
		end = get_timer(0);
		printf("write time is %d ms, speed is %dMB/s:\n", (end - start), (100000 / (end - start)));
		start = get_timer(0);
		for (addr = 0xa0000000; addr < 0xa1000000; addr+=4)
		{
			*(volatile unsigned int *)(addr);
		}
		end = get_timer(0);
		printf("read time is %d ms, speed is %dMB/s:\n", (end - start), (100000 / (end - start)));
	}
#endif
#ifndef CONFIG_BURNER
	/* Clear the BSS */
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);
	board_init_r(NULL, 0);
#endif
}

extern void flush_cache_all(void);

void __noreturn jump_to_image_no_args(struct spl_image_info *spl_image)
{
	typedef void __noreturn (*image_entry_noargs_t)(void);

	image_entry_noargs_t image_entry =
		(image_entry_noargs_t) spl_image->entry_point;
	flush_cache_all();
	printf("image entry point: 0x%X\n", spl_image->entry_point);
	image_entry();
}

#endif /* CONFIG_SPL_BUILD */

/*
 * U-Boot common functions
 */

void enable_interrupts(void)
{
}

int disable_interrupts(void)
{
	return 0;
}
