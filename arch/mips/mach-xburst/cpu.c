// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <common.h>
#include <command.h>
#include <netdev.h>
#include <asm/global_data.h>
#include <asm/mipsregs.h>
#include <asm/cacheops.h>
#include <asm/reboot.h>
#include <asm/io.h>
#include <asm/arch/wdt.h>
#include <asm/arch/jz_cache.h>
#include <linux/delay.h>

DECLARE_GLOBAL_DATA_PTR;

void lowlevel_init(void)
{

}

void _machine_restart(void)
{
	int time = RTC_FREQ / WDT_DIV * RESET_DELAY_MS / 1000;

	if(time > 65535)
		time = 65535;

	writel(TSCR_WDTSC, TCU_BASE + TCU_TSCR);

	writel(0, WDT_BASE + WDT_TCNT);
	writel(time, WDT_BASE + WDT_TDR);
	writel(TCSR_PRESCALE | TCSR_RTC_EN, WDT_BASE + WDT_TCSR);
	writel(0, WDT_BASE + WDT_TCER);

	printf("reset in %dms", RESET_DELAY_MS);
	writel(TCER_TCEN, WDT_BASE + WDT_TCER);
	mdelay(1000);
}

int print_cpuinfo(void)
{
	printf("CPU: Ingenic XBurst\n");
	return 0;
}

 void reset_cpu(void)
{
	_machine_restart();
}


void flush_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_DCACHE_LINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);
	unsigned int writebuffer;
	for (; addr <= aend; addr += lsize)
		cache_op(HIT_WRITEBACK_INV_D, addr);

	__asm__ __volatile__("sync");
	writebuffer = *(volatile unsigned int *)0xa0000000;
}

void invalid_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_DCACHE_LINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);

	for (; addr <= aend; addr += lsize)
		cache_op(HIT_INVALIDATE_D, addr);
}

void invalidate_dcache_range(ulong start_addr, ulong stop)
{
	unsigned long lsize = CONFIG_SYS_DCACHE_LINE_SIZE;
	unsigned long addr = start_addr & ~(lsize - 1);
	unsigned long aend = (stop - 1) & ~(lsize - 1);
	unsigned int writebuffer;

	for (; addr <= aend; addr += lsize)
		cache_op(HIT_INVALIDATE_D, addr);

	__asm__ __volatile__("sync");
	writebuffer = *(volatile unsigned int *)0xa0000000;

}

void flush_icache_all(void)
{
	u32 addr, t = 0;

	__asm__ __volatile__("mtc0 $0, $28"); /* Clear Taglo */
	__asm__ __volatile__("mtc0 $0, $29"); /* Clear TagHi */

	printf("%s[%d]: \n",__func__,__LINE__);
	for (addr = CKSEG0; addr < CKSEG0 + CONFIG_SYS_ICACHE_SIZE;
			addr += CONFIG_SYS_ICACHE_LINE_SIZE) {
		cache_op(INDEX_STORE_TAG_I, addr);
	}

	printf("%s[%d]: \n",__func__,__LINE__);
	/* invalidate btb */
	__asm__ __volatile__(
			".set mips32\n\t"
			"mfc0 %0, $16, 7\n\t"
			"nop\n\t"
			"ori %0,2\n\t"
			"mtc0 %0, $16, 7\n\t"
			".set mips2\n\t"
			:
			: "r" (t));

	printf("%s[%d]: \n",__func__,__LINE__);
}

void flush_dcache_all(void)
{
	u32 addr;

	for (addr = CKSEG0; addr < CKSEG0 + CONFIG_SYS_DCACHE_SIZE;
			addr += CONFIG_SYS_DCACHE_LINE_SIZE) {
		cache_op(INDEX_WRITEBACK_INV_D, addr);
	}

	printf("%s[%d]: \n",__func__,__LINE__);
	__asm__ __volatile__("sync");
}

void flush_l2cache_all(void)
{
	u32 addr;

	for (addr = CKSEG0; addr < (CKSEG0 + CONFIG_SYS_SCACHE_SIZE);
			addr += CONFIG_SYS_SCACHE_LINE_SIZE) {
		cache_op(INDEX_WRITEBACK_INV_SD, addr);
	}
}

void flush_cache_all(void)
{
	flush_dcache_all();
	printf("%s[%d]: \n",__func__,__LINE__);
	flush_icache_all();
	// printf("%s[%d]: \n",__func__,__LINE__);
	//	flush_l2cache_all();
}

void flush_cache(ulong start_addr, ulong size)
{
	flush_dcache_range(start_addr, start_addr + size);
	invalid_dcache_range(start_addr, start_addr + size);
}
