/*
 * Usefuls routines based on the LzmaTest.c file from LZMA SDK 4.65
 *
 * Copyright (C) 2007-2009 Industrie Dial Face S.p.A.
 * Luigi 'Comio' Mantellini (luigi.mantellini@idf-hit.com)
 *
 * Copyright (C) 1999-2005 Igor Pavlov
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * LZMA_Alone stream format:
 *
 * uchar   Properties[5]
 * uint64  Uncompressed size
 * uchar   data[*]
 *
 */

#include <config.h>
#include <common.h>
#include <watchdog.h>


#define LZMA_PROPERTIES_OFFSET 0
#define LZMA_SIZE_OFFSET       LZMA_PROPS_SIZE
#define LZMA_DATA_OFFSET       LZMA_SIZE_OFFSET+sizeof(uint64_t)

#include <linux/string.h>
#include <malloc.h>
#include <asm/arch/clk.h>
#include <spi.h>
#include <asm/io.h>
#include <linux/lzo.h>
#include <asm/arch-PRJ/cpm.h>

#define VPUCDR                  0xb0000030
#define LZMA_BASE_ADDR          0xb31f0000
#define LZMA_CTRL               0x00
#define LZMA_BS_BASE            0x04
#define LZMA_BS_SIZE            0x08
#define LZMA_DST_BASE           0x0C
#define LZMA_TIMEOUT            0x10
#define LZMA_FINAL_SIZE         0x14
#define LZMA_VERSION            0x58

#define lzma_readl(x)       readl((void *)(LZMA_BASE_ADDR + (x)))
#define lzma_writel(x, y)   writel((x), (void *)(LZMA_BASE_ADDR + (y)))

unsigned int tmpva = 0;

static void lzma_clock_on(void *cpm_ioaddr)
{
	unsigned int clk_gate = 0;
	/* tnpu clk on, low is valid */
	clk_gate = readl(cpm_ioaddr + CPM_CLKGR1);
	clk_gate &= ~(1 << 11); /* tnpu enable */
	writel(clk_gate, cpm_ioaddr + CPM_CLKGR1);

	/* set hoclk */
	tmpva = readl(cpm_ioaddr + CPM_CPCCR);
	writel(readl(cpm_ioaddr + CPM_CPCCR) & (~(1 << 11)), cpm_ioaddr + CPM_CPCCR);

	/* setting tnpu_clk */
	writel(0x20000011, cpm_ioaddr + CPM_TNPUCDR); /* tnpu enable */

	/* lzma0_clk */
	writel(readl(cpm_ioaddr + CPM_CLKGR1) & (~(0x1 << 4)), cpm_ioaddr + CPM_CLKGR1);
	/* lzma1_clk */
	writel(readl(cpm_ioaddr + CPM_CLKGR1) & (~(0x1 << 15)), cpm_ioaddr + CPM_CLKGR1);

	writel(readl(cpm_ioaddr + CPM_SRBC0) & (~(0x1 << 25)), cpm_ioaddr + CPM_SRBC0);
}

static void lzma_clock_off(void *cpm_ioaddr)
{
	unsigned int clk_gate = 0;
	/* open clk gate */
	clk_gate = readl(cpm_ioaddr + CPM_CLKGR1);
	clk_gate |= 1 << 11; /* tnpu disable */
	writel(clk_gate, cpm_ioaddr + CPM_CLKGR1);

	/* set ahb0clk */
	writel(tmpva, cpm_ioaddr + CPM_CPCCR);

	/* setting tnpu_clk */
	writel(readl(cpm_ioaddr + CPM_TNPUCDR) & (~(1 << 29)), cpm_ioaddr + CPM_TNPUCDR); /* tnpu disable */

	/* lzma0_clk */
	writel(readl(cpm_ioaddr + CPM_CLKGR1) | (0x1 << 4), cpm_ioaddr + CPM_CLKGR1);
	/* lzma1_clk */
	writel(readl(cpm_ioaddr + CPM_CLKGR1) | (0x1 << 15), cpm_ioaddr + CPM_CLKGR1);

	writel(readl(cpm_ioaddr + CPM_SRBC0) | (0x1 << 25), cpm_ioaddr + CPM_SRBC0);
}

unsigned int jz_lzma_decompress(unsigned char *src, size_t size, unsigned char *dst)
{
	unsigned int outlen;

	lzma_clock_on(CPM_BASE);

	/* switch to LZMA decompress */
	printf("----- LZMA_VERSION : 0x%x\n", lzma_readl(LZMA_VERSION));
	while (!((lzma_readl(LZMA_CTRL) >> 31) & 0x1))
	{
		lzma_writel(0x1 << 31, LZMA_CTRL);
	}

	/* ram config */
	while (((lzma_readl(LZMA_CTRL) >> 7) & 0x1) != 1)
	{
		lzma_writel(0x1 << 7, LZMA_CTRL);
	}

	/* reset */
	lzma_writel(0x1 << 1, LZMA_CTRL);
	while (((lzma_readl(LZMA_CTRL) >> 1) & 0x1))
		;

	/* additional reset */
	lzma_writel(0, 0x20);
	lzma_writel(0, 0x30);

	/* config */
	lzma_writel((unsigned long)src - 0x80000000, LZMA_BS_BASE);
	lzma_writel(size, LZMA_BS_SIZE);
	lzma_writel((unsigned long)dst - 0x80000000, LZMA_DST_BASE);

	/* close intr and start lzma */
	lzma_writel((1 << 0), LZMA_CTRL);

	/* 等待解压缩结束 */
	while (lzma_readl(LZMA_CTRL) & 0x01)
	{
		outlen = lzma_readl(LZMA_FINAL_SIZE);
	}

	outlen = lzma_readl(LZMA_FINAL_SIZE);

	/* chn0 set to bscaler */
	if (((lzma_readl(LZMA_CTRL) >> 31) & 0x1) == 1)
	{
		lzma_writel(0x1 << 31, LZMA_CTRL);
	}

	lzma_clock_off(CPM_BASE);
	return outlen;
}
