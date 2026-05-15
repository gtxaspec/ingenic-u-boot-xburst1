// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <config.h>
#include <common.h>
#include <cpu_func.h>
#include <hang.h>
#include <image.h>
#include <init.h>
#include <asm/arch/clk.h>
#include <asm/arch/cpm.h>
#include <asm/arch/wdt.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <asm/sections.h>
#include <asm/arch/uart.h>
#include <asm/arch/jz_cache.h>
#include <mmc.h>
#include <spl.h>

#ifdef CONFIG_SPL_BUILD
/* Pointer to the global data structure for SPL */
DECLARE_GLOBAL_DATA_PTR;
gd_t gdata __section(".data");

struct global_info ginfo __section(".data") = {
	.extal		= CONFIG_SYS_EXTAL,
	.cpufreq	= CONFIG_SYS_CPU_FREQ,
	.ddrfreq	= CONFIG_SYS_MEM_FREQ / 2,
	.uart_idx	= CONFIG_SYS_UART_INDEX,
	.baud_rate	= CONFIG_BAUDRATE,

};

extern struct jz_uart *uart __section(".data");
extern void gpio_init(void);
extern void pll_init(void);
extern void sdram_init(void);

static void enable_cpu_read_ddr(void)
{
	int res = 0, res1 = 0;
	__asm__ __volatile__(		\
		".set	push	\n"	\
		".set	reorder	\n"	\
		"mfc0	%0,$9,4	\n"	\
		"li	%1,-3	\n"	\
		"and	%0,%0,%1\n"	\
		"mtc0	%0,$9,4	\n"	\
		".set	pop	\n"	\
		:"=r"(res),"=r"(res1));
	return;
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
			data = (j/4)%2 ? test_data : 0;
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

void board_init_f(ulong dummy)
{
	/* Set global data pointer */
	gd = &gdata;
	/* Setup global info */
	gd->arch.gi = &ginfo;

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

	/* Set global data pointer */
	gpio_init();
	/* OST clk gate set 0 */
	cpm_outl(cpm_inl(CPM_CLKGR0) & (~CPM_CLKGR1_OST), CPM_CLKGR0);
	/* wtd disable */
	writel(0, WDT_BASE + WDT_TCER);
	cpm_outl(cpm_inl(CPM_MESTSEL) | 0x7, CPM_MESTSEL);
	/* Init uart first */
	enable_uart_clk();
	preloader_console_init();
	printf("ERROR EPC 0x%lx\n", read_c0_errorepc());

	timer_init();
	clk_prepare();
	pll_init();
	clk_init();

	/* CPU allocated访问DDR开启 */
	enable_cpu_read_ddr();

	sdram_init();

	if(ddr_mem_test12())
	{
		printf("ddr error!\n");
		hang();
	}

	/* Clear the BSS */
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);
	gd->flags |= GD_FLG_SPL_INIT;
	board_init_r(NULL, 0);

	return;
}

#endif   //CONFIG_SPL_BUILD
