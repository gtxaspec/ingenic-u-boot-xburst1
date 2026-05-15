/*
 * Timer for JZ4775, JZ4780
 *
 * Copyright (c) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
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

#include <config.h>
#include <common.h>
#include <div64.h>
#include <asm/io.h>
#include <asm/mipsregs.h>
#include <asm/arch/ost.h>

DECLARE_GLOBAL_DATA_PTR;

unsigned int multiple __attribute__((section(".data")));

static void ost_writel(uint32_t val, uint32_t off)
{
	writel(val, (void __iomem *)OST_BASE + off);
}

static uint32_t ost_readl(uint32_t off)
{
	return readl((void __iomem *)OST_BASE + off);
}

#define USEC_IN_1SEC 1000000
int timer_init(void)
{
	// 24000000 /4 =6000000/1000/1000=6000 =1us
	multiple = CONFIG_SYS_EXTAL / OST_DIV / USEC_IN_1SEC;
	ost_writel(1 << 2, OST_OSTCCR);
	ost_writel(0, OST_OSTCR);
	ost_writel(2, OST_OSTESR);

	return 0;
}

static uint64_t get_timer64(void)
{
	uint32_t low = ost_readl(OST_OST2CNTL);
	uint32_t high = ost_readl(OST_OST2CNTH);
	return ((uint64_t)high << 32) | low;
}

ulong get_timer(ulong base)
{
	return lldiv(get_timer64(), (USEC_IN_1SEC / CONFIG_SYS_HZ) * multiple) - base;
}

void __udelay(unsigned long usec)
{
	/* OST count increments at 3MHz */
	uint64_t end = get_timer64() + ((uint64_t)usec * multiple);
	while (get_timer64() < end)
		;
}

unsigned long long get_ticks(void)
{
	return get_timer64();
}

ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}
