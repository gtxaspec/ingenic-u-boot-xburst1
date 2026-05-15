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

#include "LzmaTools.h"
#include "LzmaDec.h"

#include <linux/string.h>
#include <malloc.h>
#include <asm/arch/clk.h>
#include <spi.h>
#include <asm/io.h>
#include <linux/lzo.h>
#include <asm/arch/sfc.h>
#include <asm/arch/spi.h>
#include <asm/arch/clk.h>

#ifdef CONFIG_LZMA

static void *SzAlloc(void *p, size_t size) { p = p; return malloc(size); }
static void SzFree(void *p, void *address) { p = p; free(address); }

int lzmaBuffToBuffDecompress (unsigned char *outStream, SizeT *uncompressedSize,
                  unsigned char *inStream,  SizeT  length)
{
    int res = SZ_ERROR_DATA;
    int i;
    ISzAlloc g_Alloc;

    SizeT outSizeFull = 0xFFFFFFFF; /* 4GBytes limit */
    SizeT outProcessed;
    SizeT outSize;
    SizeT outSizeHigh;
    ELzmaStatus state;
    SizeT compressedSize = (SizeT)(length - LZMA_PROPS_SIZE);

    debug ("LZMA: Image address............... 0x%p\n", inStream);
    debug ("LZMA: Properties address.......... 0x%p\n", inStream + LZMA_PROPERTIES_OFFSET);
    debug ("LZMA: Uncompressed size address... 0x%p\n", inStream + LZMA_SIZE_OFFSET);
    debug ("LZMA: Compressed data address..... 0x%p\n", inStream + LZMA_DATA_OFFSET);
    debug ("LZMA: Destination address......... 0x%p\n", outStream);

    memset(&state, 0, sizeof(state));

    outSize = 0;
    outSizeHigh = 0;
    /* Read the uncompressed size */
    for (i = 0; i < 8; i++) {
        unsigned char b = inStream[LZMA_SIZE_OFFSET + i];
            if (i < 4) {
                outSize     += (UInt32)(b) << (i * 8);
        } else {
                outSizeHigh += (UInt32)(b) << ((i - 4) * 8);
        }
    }

    outSizeFull = (SizeT)outSize;
    if (sizeof(SizeT) >= 8) {
        /*
         * SizeT is a 64 bit uint => We can manage files larger than 4GB!
         *
         */
            outSizeFull |= (((SizeT)outSizeHigh << 16) << 16);
    } else if (outSizeHigh != 0 || (UInt32)(SizeT)outSize != outSize) {
        /*
         * SizeT is a 32 bit uint => We cannot manage files larger than
         * 4GB!  Assume however that all 0xf values is "unknown size" and
         * not actually a file of 2^64 bits.
         *
         */
        if (outSizeHigh != (SizeT)-1 || outSize != (SizeT)-1) {
            debug ("LZMA: 64bit support not enabled.\n");
            return SZ_ERROR_DATA;
        }
    }

    debug("LZMA: Uncompresed size............ 0x%zx\n", outSizeFull);
    debug("LZMA: Compresed size.............. 0x%zx\n", compressedSize);

    g_Alloc.Alloc = SzAlloc;
    g_Alloc.Free = SzFree;

    /* Decompress */
    outProcessed = outSizeFull;

    WATCHDOG_RESET();

    res = LzmaDecode(
        outStream, &outProcessed,
        inStream + LZMA_DATA_OFFSET, &compressedSize,
        inStream, LZMA_PROPS_SIZE, LZMA_FINISH_ANY, &state, &g_Alloc);
    *uncompressedSize = outProcessed;
    if (res != SZ_OK)  {
        return res;
    }

    return res;
}
#endif

#ifdef CONFIG_HARD_LZMA

#define VPUCDR                  0xb0000030
#define LZMA_BASE_ADDR_INDEX(x) 0xb31f0000
#define LZMA_CTRL               0x00
#define LZMA_BS_BASE            0x04
#define LZMA_BS_SIZE            0x08
#define LZMA_DST_BASE           0x0C
#define LZMA_TIMEOUT            0x10
#define LZMA_FINAL_SIZE         0x14
#define LZMA_VERISON            0x58

#define lzma_readl(x)       readl(LZMA_BASE_ADDR_INDEX(0) + x)
#define lzma_writel(x, y)   writel(x, LZMA_BASE_ADDR_INDEX(0) + y)



unsigned int tmpva = 0;
static void lzma_clock_on(unsigned long cpm_ioaddr)
{
	unsigned int clk_gate = 0;
	/* tnpu clk on,low is valid*/
	clk_gate = readl(cpm_ioaddr + CPM_CLKGR1);
	clk_gate &= ~(1 << 11);//tnpu disable
	writel(clk_gate, cpm_ioaddr + CPM_CLKGR1);

	//set hoclk
    tmpva = readl(cpm_ioaddr + CPM_CPCCR);
	writel(readl(cpm_ioaddr + CPM_CPCCR) & (~(1<<11)), cpm_ioaddr + CPM_CPCCR);

	//setting tnpu_clk
	writel(0x20000011, cpm_ioaddr + CPM_TNPUCDR); //tnpu enalbe

	//lzma0_clk
	writel(readl(cpm_ioaddr + CPM_CLKGR1) & (~(0x1 << 4)), cpm_ioaddr + CPM_CLKGR1);
	//lzma1_clk
	writel(readl(cpm_ioaddr + CPM_CLKGR1) & (~(0x1 << 15)), cpm_ioaddr + CPM_CLKGR1);

	writel(readl(cpm_ioaddr + CPM_SRBC0) & (~(0x1 << 25)), cpm_ioaddr + CPM_SRBC0);

	/*printk("tnpu_clk_en:%#x,ahbo_clk_freq:%#x,tnpu_clk_freq:%#x,lzma_clock_en:%#x\n",\
		readl(cpm_ioaddr + CPM_CLKGR1) >> 11 & 0x1,\
		printk("ahbo_clk_freq:%#x\n",readl(cpm_ioaddr + CPM_CPCCR)),\
		printk("tnpu_clk_freq:%#x\n",readl(cpm_ioaddr + CPM_TNPUCDR)),\
		readl(cpm_ioaddr + CPM_SRBC0)>>25 & 0x1);*/
}

static void lzma_clock_off(unsigned long cpm_ioaddr)
{
	unsigned int clk_gate = 0;
	/* open clk gate */
	clk_gate = readl(cpm_ioaddr + CPM_CLKGR1);
	clk_gate |= 1 << 11;//tnpu disable
	writel(clk_gate, cpm_ioaddr + CPM_CLKGR1);

	//set ahb0clk
	//writel(readl(cpm_ioaddr + CPM_CPCCR) | (1<<11), cpm_ioaddr + CPM_CPCCR);
    writel(tmpva, cpm_ioaddr + CPM_CPCCR);

	//setting tnpu_clk
	writel(readl(cpm_ioaddr + CPM_TNPUCDR) & (~(1<<29)), cpm_ioaddr + CPM_TNPUCDR); //tnpu enalbe

	//lzma0_clk
	writel(readl(cpm_ioaddr + CPM_CLKGR1) | (0x1 << 4), cpm_ioaddr + CPM_CLKGR1);
	//lzma1_clk
	writel(readl(cpm_ioaddr + CPM_CLKGR1) | (0x1 << 15), cpm_ioaddr + CPM_CLKGR1);

	writel(readl(cpm_ioaddr + CPM_SRBC0) | (0x1 << 25), cpm_ioaddr + CPM_SRBC0);

	/*printk("tnpu_clk_en:%#x,ahbo_clk_freq:%#x,tnpu_clk_freq:%#x,lzma_clock_en:%#x\n",\
		readl(cpm_ioaddr + CPM_CLKGR1) >> 11 & 0x1,\
		printk("ahbo_clk_freq:%#x\n",readl(cpm_ioaddr + CPM_CPCCR)),\
		printk("tnpu_clk_freq:%#x\n",readl(cpm_ioaddr + CPM_TNPUCDR)),\
		readl(cpm_ioaddr + CPM_SRBC0)>>25 & 0x1);*/
}

unsigned int jz_lzma_decompress(unsigned char *src, size_t size, unsigned char *dst)
{
    unsigned int outlen;
    /*int version = 0;*/
	lzma_clock_on(CPM_BASE);

    /* switch to LZMA decompress */
    printf("----- LZMA_VERISON : 0x%x\n", lzma_readl(LZMA_VERISON));
    while (!((lzma_readl(LZMA_CTRL) >> 31) & 0x1))
    {
        lzma_writel(0x1 << 31, LZMA_CTRL);
        // printf("readl(LZMA_BASE_ADDR_INDEX(0)+LZMA_CTRL) >> 31 == 0\n");
    }

    //ram config
	while (((lzma_readl(LZMA_CTRL) >> 7) & 0x1) != 1)
	{
		// printk("lzma-----Write LZMA_CTRL[7]\n");
		lzma_writel(0x1 << 7, LZMA_CTRL);
	}

    // printf("----- lzma reset begin...\n");
    // writel(0x1 << 1, (LZMA_BASE_ADDR_INDEX(0)+LZMA_CTRL));
    lzma_writel(0x1 << 1, LZMA_CTRL);
    while (((lzma_readl(LZMA_CTRL) >> 1) & 0x1))
        ;
    //reset
    lzma_writel(0, 0x20);
	lzma_writel(0, 0x30);
    /* config */
    // printf("[%s][%d]src %x  dst %x   size %d\n",__FUNCTION__,__LINE__, src, dst, size);
    lzma_writel((unsigned long)src - 0x80000000, LZMA_BS_BASE);
    lzma_writel(size, LZMA_BS_SIZE);
    lzma_writel((unsigned long)dst - 0x80000000, LZMA_DST_BASE);

    /* close intr and start lzma*/
    lzma_writel((1 << 0), LZMA_CTRL);

     //等待解压缩结束
    while (lzma_readl(LZMA_CTRL) & 0x01)
    {
        outlen = lzma_readl(LZMA_FINAL_SIZE);
        // printf("lzma dec size=%d\n", outlen);
    }

    outlen = lzma_readl(LZMA_FINAL_SIZE);
    // printf("[%s][%d] outlen %d\n",__FUNCTION__,__LINE__, outlen);

    // chn0 set to bscaler
    if (((lzma_readl(LZMA_CTRL) >> 31) & 0x1) == 1)
    {
        lzma_writel(0x1 << 31, LZMA_CTRL);
    }

	lzma_clock_off(CPM_BASE);
    return outlen;
#if 0
quit:
    outlen = lzma_readl(LZMA_FINAL_SIZE);
    printf("lzma dec size=%d\n", outlen);
    return 0;
#endif
}

#endif
