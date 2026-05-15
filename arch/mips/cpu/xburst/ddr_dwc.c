/*
 * DDR driver for Synopsys DWC DDR PHY.
 * Used by Jz4775, JZ4780...
 *
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
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
#include <ddr/ddr_common.h>
#include <generated/ddr_reg_values.h>

#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/ddr_dwc.h>
//#define CONFIG_DWC_DEBUG 0
#include "ddr_debug.h"

#if (CONFIG_DDR_CS1 == 1)
#ifndef DDR_ROW1
#error "please define DDR_ROW1"
#endif /* DDR_ROW1 */
#ifndef DDR_COL1
#error "please define DDR_COL1"
#endif /* DDR_COL1 */
#endif /* CONFIG_DDR_CS1 */

DECLARE_GLOBAL_DATA_PTR;
extern unsigned int sdram_size(int cs, struct ddr_params *p);
struct ddr_params *ddr_params_p = NULL;
#ifndef CONFIG_FPGA
extern void reset_dll(void);
#endif

#if defined(CONFIG_SPL_DDR_SOFT_TRAINING) || defined(CONFIG_DDR_FORCE_SOFT_TRAINING)
extern bool dqs_gate_train(int rank_cnt, int byte_cnt);
extern void send_MR0(int a);
#else /* CONFIG_SPL_DDR_SOFT_TRAINING */
#define send_MR0(a)	do {} while(0);
#define dqs_gate_train(rank_cnt, byte_cnt) true
#endif /* CONFIG_SPL_DDR_SOFT_TRAINING */

#define BYPASS_ENABLE       1
#define BYPASS_DISABLE      0
#define IS_BYPASS_MODE(x)     (((x) & 1) == BYPASS_ENABLE)
	/* DDR3, */
	/* LPDDR, */
	/* LPDDR2, */
	/* DDR2,  */
	/* VARIABLE, */

#define DDR_TYPE_MODE(x)     (((x) >> 1) & 0xf)

static void dump_ddrc_register(void)
{
#ifdef CONFIG_DWC_DEBUG
	printf("DDRC_STATUS		0x%x\n", ddr_readl(DDRC_STATUS));
	printf("DDRC_CFG		0x%x\n", ddr_readl(DDRC_CFG));
	printf("DDRC_CTRL		0x%x\n", ddr_readl(DDRC_CTRL));
	printf("DDRC_LMR		0x%x\n", ddr_readl(DDRC_LMR));
	printf("DDRC_TIMING1		0x%x\n", ddr_readl(DDRC_TIMING(1)));
	printf("DDRC_TIMING2		0x%x\n", ddr_readl(DDRC_TIMING(2)));
	printf("DDRC_TIMING3		0x%x\n", ddr_readl(DDRC_TIMING(3)));
	printf("DDRC_TIMING4		0x%x\n", ddr_readl(DDRC_TIMING(4)));
	printf("DDRC_TIMING5		0x%x\n", ddr_readl(DDRC_TIMING(5)));
	printf("DDRC_TIMING6		0x%x\n", ddr_readl(DDRC_TIMING(6)));
	printf("DDRC_REFCNT		0x%x\n", ddr_readl(DDRC_REFCNT));
	printf("DDRC_MMAP0		0x%x\n", ddr_readl(DDRC_MMAP0));
	printf("DDRC_MMAP1		0x%x\n", ddr_readl(DDRC_MMAP1));
	printf("DDRC_REMAP1		0x%x\n", ddr_readl(DDRC_REMAP(1)));
	printf("DDRC_REMAP2		0x%x\n", ddr_readl(DDRC_REMAP(2)));
	printf("DDRC_REMAP3		0x%x\n", ddr_readl(DDRC_REMAP(3)));
	printf("DDRC_REMAP4		0x%x\n", ddr_readl(DDRC_REMAP(4)));
	printf("DDRC_REMAP5		0x%x\n", ddr_readl(DDRC_REMAP(5)));
	printf("DDRC_AUTOSR_EN		0x%x\n", ddr_readl(DDRC_AUTOSR_EN));
#endif
}

static void dump_ddrp_register(void)
{
#ifdef CONFIG_DWC_DEBUG
	printf("DDRP_PIR		0x%x\n", ddr_readl(DDRP_PIR));
	printf("DDRP_PGCR		0x%x\n", ddr_readl(DDRP_PGCR));
	printf("DDRP_PGSR		0x%x\n", ddr_readl(DDRP_PGSR));
	printf("DDRP_PTR0		0x%x\n", ddr_readl(DDRP_PTR0));
	printf("DDRP_PTR1		0x%x\n", ddr_readl(DDRP_PTR1));
	printf("DDRP_PTR2		0x%x\n", ddr_readl(DDRP_PTR2));
	printf("DDRP_DCR		0x%x\n", ddr_readl(DDRP_DCR));
	printf("DDRP_DTPR0		0x%x\n", ddr_readl(DDRP_DTPR0));
	printf("DDRP_DTPR1		0x%x\n", ddr_readl(DDRP_DTPR1));
	printf("DDRP_DTPR2		0x%x\n", ddr_readl(DDRP_DTPR2));
	printf("DDRP_MR0		0x%x\n", ddr_readl(DDRP_MR0));
	printf("DDRP_MR1		0x%x\n", ddr_readl(DDRP_MR1));
	printf("DDRP_MR2		0x%x\n", ddr_readl(DDRP_MR2));
	printf("DDRP_MR3		0x%x\n", ddr_readl(DDRP_MR3));
	printf("DDRP_ODTCR		0x%x\n", ddr_readl(DDRP_ODTCR));
	printf("DDRP_DSGCR		0x%x\n", ddr_readl(DDRP_DSGCR));
	int i=0;
	for(i=0;i<4;i++) {
		printf("DX%dGSR0: %x\n", i, ddr_readl(DDRP_DXGSR0(i)));
		printf("@pas:DXDQSTR(%d)= 0x%x\n", i,ddr_readl(DDRP_DXDQSTR(i)));
	}
#endif
}
static void reset_controller(void)
{
#ifndef CONFIG_FPGA
	ddr_writel(0xf << 20, DDRC_CTRL);
#else
	ddr_writel(0xc0 << 16, DDRC_CTRL);
#endif
	mdelay(5);
	ddr_writel(0, DDRC_CTRL);
	mdelay(5);
}

static void remap_swap(int a, int b)
{
	uint32_t remmap[2], tmp[2];

	remmap[0] = ddr_readl(DDRC_REMAP(a / 4 + 1));
	remmap[1] = ddr_readl(DDRC_REMAP(b / 4 + 1));

#define BIT(bit) ((bit % 4) * 8)
#define MASK(bit) (0x1f << BIT(bit))
	tmp[0] = (remmap[0] & MASK(a)) >> BIT(a);
	tmp[1] = (remmap[1] & MASK(b)) >> BIT(b);

	remmap[0] &= ~MASK(a);
	remmap[1] &= ~MASK(b);

	ddr_writel(remmap[0] | (tmp[1] << BIT(a)), DDRC_REMAP(a / 4 + 1));
	ddr_writel(remmap[1] | (tmp[0] << BIT(b)), DDRC_REMAP(b / 4 + 1));
#undef BIT
#undef MASK
}

static void mem_remap(void)
{
	uint32_t start = 0, num = 0;
	int row, col, dw32, bank8, cs0, cs1;
	uint32_t size0 = 0, size1 = 0;

#ifdef CONFIG_DDR_HOST_CC
#if (CONFIG_DDR_CS0 == 1)
			row = DDR_ROW;
			col = DDR_COL;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
#endif

	size0 = (unsigned int)(DDR_CHIP_0_SIZE);
	size1 = (unsigned int)(DDR_CHIP_1_SIZE);

	/* For two different size ddr chips, just don't remmap */

#if (CONFIG_DDR_CS1 == 1)
	if (size0 != size1)
		return;
#endif

#if (CONFIG_DDR_CS0 == 1)
#if (CONFIG_DDR_CS1 == 1)
	if (size1 && size0) {
		if (size1 <= size0) {
			row = DDR_ROW1;
			col = DDR_COL1;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
		} else {
			row = DDR_ROW;
			col = DDR_COL;
			dw32 = CONFIG_DDR_DW32;
			bank8 = DDR_BANK8;
		}
	} else {
		printf("Error: The DDR size is 0\n");
		hang();
	}
#else /*CONFIG_DDR_CS1 == 1*/
	if (size0) {
		row = DDR_ROW;
		col = DDR_COL;
		dw32 = CONFIG_DDR_DW32;
		bank8 = DDR_BANK8;
	} else {
		printf("Error: The DDR size is 0\n");
		hang();
	}

#endif /* CONFIG_DDR_CS1 == 1 */
#else /* CONFIG_DDR_CS0 == 1 */
	if (size1) {
		row = DDR_ROW1;
		col = DDR_COL1;
		dw32 = CONFIG_DDR_DW32;
		bank8 = DDR_BANK8;
	} else {
		printf("Error: The DDR size is 0\n");
		hang();
	}

#endif /* CONFIG_DDR_CS0 == 1 */
	cs0 = CONFIG_DDR_CS0;
	cs1 = CONFIG_DDR_CS1;
#else /* CONFIG_DDR_HOST_CC */
	size0 = ddr_params_p->size.chip0;
	size1 = ddr_params_p->size.chip1;
	if (size0 && size1) {
		if (size1 <= size0) {
			row = ddr_params_p->row1;
			col = ddr_params_p->col1;
			dw32 = ddr_params_p->dw32;
			bank8 = ddr_params_p->bank8;
		} else {
			row = ddr_params_p->row;
			col = ddr_params_p->col;
			dw32 = ddr_params_p->dw32;
			bank8 = ddr_params_p->bank8;
		}
	} else if (size0) {
		row = ddr_params_p->row;
		col = ddr_params_p->col;
		dw32 = ddr_params_p->dw32;
		bank8 = ddr_params_p->bank8;
	} else if (size1) {
		row = ddr_params_p->row1;
		col = ddr_params_p->col1;
		dw32 = ddr_params_p->dw32;
		bank8 = ddr_params_p->bank8;
	} else {
		printf("Error: The DDR size is 0\n");
		hang();
	}

	cs0 = ddr_params_p->cs0;
	cs1 = ddr_params_p->cs1;
#endif /* CONFIG_DDR_HOST_CC */
	start += row + col + (dw32 ? 4 : 2) / 2;
	start -= 12;

	if (bank8)
		num += 3;
	else
		num += 2;

	if (cs0 && cs1)
		num++;

	for (; num > 0; num--)
		remap_swap(0 + num - 1, start + num - 1);
}

void ddr_controller_init(void)
{
	dwc_debug("DDR Controller init\n");
// dsqiu
//	mdelay(1);
	ddr_writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	ddr_writel(0, DDRC_CTRL);
	/* DDRC CFG init*/
	ddr_writel(DDRC_CFG_VALUE, DDRC_CFG);
	/* DDRC timing init*/
	ddr_writel(DDRC_TIMING1_VALUE, DDRC_TIMING(1));
	ddr_writel(DDRC_TIMING2_VALUE, DDRC_TIMING(2));
	ddr_writel(DDRC_TIMING3_VALUE, DDRC_TIMING(3));
	ddr_writel(DDRC_TIMING4_VALUE, DDRC_TIMING(4));
	ddr_writel(DDRC_TIMING5_VALUE, DDRC_TIMING(5));
	ddr_writel(DDRC_TIMING6_VALUE, DDRC_TIMING(6));

	/* DDRC memory map configure*/
	ddr_writel(DDRC_MMAP0_VALUE, DDRC_MMAP0);
	ddr_writel(DDRC_MMAP1_VALUE, DDRC_MMAP1);
	ddr_writel(DDRC_CTRL_CKE | DDRC_CTRL_ALH, DDRC_CTRL);
	ddr_writel(DDRC_REFCNT_VALUE, DDRC_REFCNT);
	ddr_writel(DDRC_CTRL_VALUE, DDRC_CTRL);
}
static void ddr_phy_param_init(unsigned int mode)
{
	int i;
	unsigned int timeout = 10000;
	ddr_writel(DDRP_DCR_VALUE, DDRP_DCR);
	ddr_writel(DDRP_MR0_VALUE, DDRP_MR0);
	switch(DDR_TYPE_MODE(mode)){
	case LPDDR:
		break;
	//note: lpddr2 should set mr1 mr2 mr3.
	case LPDDR2:
		ddr_writel(DDRP_MR3_VALUE, DDRP_MR3);
	//note: ddr3 should set mr1 mr2 only.
	case DDR3:
		ddr_writel(DDRP_MR1_VALUE, DDRP_MR1);
		ddr_writel(DDRP_MR2_VALUE, DDRP_MR2);

		break;
	case DDR2:
		ddr_writel(DDRP_MR1_VALUE, DDRP_MR1);
		break;
	}

#ifdef CONFIG_SYS_DDR_CHIP_ODT
	ddr_writel(0, DDRP_ODTCR);
#endif

	ddr_writel(DDRP_PTR0_VALUE, DDRP_PTR0);
	ddr_writel(DDRP_PTR1_VALUE, DDRP_PTR1);
	ddr_writel(DDRP_PTR2_VALUE, DDRP_PTR2);
	ddr_writel(DDRP_DTPR0_VALUE, DDRP_DTPR0);
	ddr_writel(DDRP_DTPR1_VALUE, DDRP_DTPR1);
	ddr_writel(DDRP_DTPR2_VALUE, DDRP_DTPR2);

	for (i = 0; i < 4; i++) {
		unsigned int tmp = ddr_readl(DDRP_DXGCR(i));

		tmp &= ~(3 << 9);
#if 0
#ifdef CONFIG_DDR_PHY_ODT
#ifdef CONFIG_DDR_PHY_DQ_ODT
		tmp |= 1 << 10;
#endif /* CONFIG_DDR_PHY_DQ_ODT */
#ifdef CONFIG_DDR_PHY_DQS_ODT
		tmp |= 1 << 9;
#endif /* CONFIG_DDR_PHY_DQS_ODT */
#endif /* CONFIG_DDR_PHY_ODT */
#ifndef CONFIG_DDR_HOST_CC
		if ((i > 1) && (ddr_params_p->dw32 == 0))
			tmp &= ~DDRP_DXGCR_DXEN;
#elif (CONFIG_DDR_DW32 == 0)
		if (i > 1)
			tmp &= ~DDRP_DXGCR_DXEN;
#endif /* CONFIG_DDR_HOST_CC */
#endif
		ddr_writel(tmp, DDRP_DXGCR(i));
	}
	ddr_writel(DDRP_PGCR_VALUE, DDRP_PGCR);

	/***************************************************************
	 *  DXCCR:
	 *       DQSRES:  4...7bit  is DQSRES[].
	 *       DQSNRES: 8...11bit is DQSRES[] too.
	 *
	 *      Selects the on-die pull-down/pull-up resistor for DQS pins.
	 *      DQSRES[3]: selects pull-down (when set to 0) or pull-up (when set to 1).
	 *      DQSRES[2:0] selects the resistor value as follows:
	 *      000 = Open: On-die resistor disconnected
	 *      001 = 688 ohms
	 *      010 = 611 ohms
	 *      011 = 550 ohms
	 *      100 = 500 ohms
	 *      101 = 458 ohms
	 *      110 = 393 ohms
	 *      111 = 344 ohms
	 *****************************************************************
	 *      Note: DQS resistor must be connected for LPDDR/LPDDR2    *
	 *****************************************************************
	 *     the config will affect power and stablity
	 */
	switch(DDR_TYPE_MODE(mode)){
	case LPDDR:
		ddr_writel(0x30c00813, DDRP_ACIOCR);
		ddr_writel(0x4802, DDRP_DXCCR);
		break;
	case DDR3:
		break;
	case LPDDR2:
		ddr_writel(0x910, DDRP_DXCCR);
		break;
	case DDR2:
		break;
	}
	while (!(ddr_readl(DDRP_PGSR) == (DDRP_PGSR_IDONE
					| DDRP_PGSR_DLDONE
					| DDRP_PGSR_ZCDONE))
			&& (ddr_readl(DDRP_PGSR) != 0x1f)
			&& --timeout);
	if (timeout == 0) {
		printf("DDR PHY init timeout: PGSR=%X\n", ddr_readl(DDRP_PGSR));
		hang();
	}
}

static void ddr_chip_init(unsigned int mode)
{
	int timeout = 10000;
	unsigned int pir_val = DDRP_PIR_INIT;
	unsigned int val;
	dwc_debug("DDR chip init\n");

	// DDRP_PIR_DRAMRST for ddr3 only
#ifndef CONFIG_FPGA
	switch(DDR_TYPE_MODE(mode)){
	case DDR3:
		pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DRAMRST | DDRP_PIR_DLLSRST;
		break;
	case LPDDR2:
		pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DLLSRST;
		break;
	case LPDDR:
		pir_val |= DDRP_PIR_DRAMINT;
		break;
	case DDR2:
		pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DRAMINT;
		break;
	}
#else
	pir_val |= DDRP_PIR_DRAMINT | DDRP_PIR_DRAMRST | DDRP_PIR_DLLBYP;
#endif
	if(IS_BYPASS_MODE(mode)) {
		pir_val |= DDRP_PIR_DLLBYP | (1 << 29);
		pir_val &= ~DDRP_PIR_DLLSRST;
		// DLL Disable: only bypassmode
		ddr_writel(0x1 << 31, DDRP_ACDLLCR);
		val = ddr_readl(DDRP_DSGCR);
		/*  LPDLLPD:  only for ddr bypass mode
		 * Low Power DLL Power Down: Specifies if set that the PHY should respond to the *
		 * DFI low power opportunity request and power down the DLL of the PHY if the *
		 * wakeup time request satisfies the DLL lock time */
		val &= ~(1 << 4);
		ddr_writel(val,DDRP_DSGCR);

	}
	ddr_writel(pir_val, DDRP_PIR);
	while (!(ddr_readl(DDRP_PGSR) == (DDRP_PGSR_IDONE
					  | DDRP_PGSR_DLDONE
					  | DDRP_PGSR_ZCDONE
					  | DDRP_PGSR_DIDONE))
	       && (ddr_readl(DDRP_PGSR) != 0x1f)
	       && --timeout);
	if (timeout == 0) {
		printf("DDR init timeout: PGSR=%X\n", ddr_readl(DDRP_PGSR));
		hang();
	}
}

static int ddr_training_hardware(unsigned int mode)
{
	int result = 0;
	int timeout = 500000;
	unsigned int pir_val = DDRP_PIR_INIT;
	switch(DDR_TYPE_MODE(mode)){
	case DDR3:
	case LPDDR2:
		pir_val |= DDRP_PIR_QSTRN;
		break;
	case LPDDR:
		pir_val |= DDRP_PIR_QSTRN | DDRP_PIR_DLLLOCK;
		break;
	case DDR2:
		pir_val |= DDRP_PIR_QSTRN;
		break;
	}
	if(IS_BYPASS_MODE(mode))
		pir_val |= DDRP_PIR_DLLBYP | (1 << 29);

	ddr_writel(pir_val, DDRP_PIR);
	while ((ddr_readl(DDRP_PGSR) != (DDRP_PGSR_IDONE
					 | DDRP_PGSR_DLDONE
					 | DDRP_PGSR_ZCDONE
					 | DDRP_PGSR_DIDONE
					 | DDRP_PGSR_DTDONE))
	       && !(ddr_readl(DDRP_PGSR)
		    & (DDRP_PGSR_DTDONE | DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR))
	       && --timeout);

	if (timeout == 0) {
		dwc_debug("DDR training timeout\n");
		result = -1;
	} else if (ddr_readl(DDRP_PGSR)
		   & (DDRP_PGSR_DTERR | DDRP_PGSR_DTIERR)) {
		dwc_debug("DDR hardware training error\n");
		result = ddr_readl(DDRP_PGSR);
	}
	return result;
}
static int ddr_training_software(unsigned int mode)
{
	unsigned int result = 0;
	unsigned int ddr_bl, ddr_cl;
	unsigned int mr0_tmp = 1;
	unsigned int cs0;
	unsigned int cs1;
	unsigned int tmp = 0;
	dwc_debug("Now try soft training\n");
#ifdef CONFIG_DDR_HOST_CC
	cs0 = CONFIG_DDR_CS0;
	cs1 = CONFIG_DDR_CS1;
#else /* CONFIG_DDR_HOST_CC */
	cs0 = ddr_params_p->cs0;
	cs1 = ddr_params_p->cs1;
#endif /* CONFIG_DDR_HOST_CC */
	if (dqs_gate_train(cs0 + cs1, 4)) {
		dwc_debug("DDR soft train fail too!!!\n");
		dump_ddrp_register();
		result = 1;
	}
	if((result) && (!cs1)){
		printf("try again to soft train may be the problem for cs\n");
		tmp = ddr_readl(DDRC_CFG);
		tmp |= 3 << 6;
		ddr_writel(tmp, DDRC_CFG);
		if (dqs_gate_train(cs0, 4)) {
			dwc_debug("try again DDR soft train fail too please check the hardware!!!\n");
			dump_ddrp_register();
			result = -1;
		}
	}

#ifdef CONFIG_DDR_HOST_CC
	ddr_bl = DDR_BL;
	ddr_cl = DDR_CL;
#else /* CONFIG_DDR_HOST_CC */
	ddr_cl = ddr_params_p->cl;
	ddr_bl = ddr_params_p->bl;
#endif /* CONFIG_DDR_HOST_CC */

	if(DDR_TYPE_MODE(mode) == LPDDR){
		while (ddr_bl >> mr0_tmp)
			mr0_tmp++;
		ddr_writel((ddr_cl << 4) | (mr0_tmp - 1), DDRP_MR0);
		send_MR0(ddr_readl(DDRP_MR0));
	}
	return result;
}
static int lpddr_retrain_bypass(void)
{
	unsigned int result = 0;
	int timeout = 10000;
	unsigned int ddr_bl, ddr_cl;
	unsigned int mr0_tmp = 1;

#ifdef CONFIG_DDR_HOST_CC
	ddr_bl = DDR_BL;
	ddr_cl = DDR_CL;
#else /* CONFIG_DDR_HOST_CC */
	ddr_cl = ddr_params_p->cl;
	ddr_bl = ddr_params_p->bl;
#endif /* CONFIG_DDR_HOST_CC */

	while (ddr_bl >> mr0_tmp)
		mr0_tmp++;
	ddr_writel((ddr_cl << 4) | (mr0_tmp - 1), DDRP_MR0);


#ifndef CONFIG_DDR_PHY_ODT
	ddr_writel(DDRP_PIR_INIT | DDRP_PIR_DRAMINT, DDRP_PIR);
#else /* CONFIG_DDR_PHY_ODT */
	ddr_writel(DDRP_PIR_INIT | DDRP_PIR_DRAMINT | DDRP_PIR_DLLLOCK | DDRP_PIR_DLLBYP | (1 << 29),
		   DDRP_PIR);
	ddr_writel(0x1, DDRP_ACDLLCR);
#endif /* CONFIG_DDR_PHY_ODT */

	while ((ddr_readl(DDRP_PGSR) != (DDRP_PGSR_IDONE
					 | DDRP_PGSR_DLDONE
					 | DDRP_PGSR_ZCDONE
					 | DDRP_PGSR_DIDONE
					 | DDRP_PGSR_DTDONE))
	       && --timeout);
	if (timeout == 0) {
		printf("DDR PHY init timeout: PGSR=%X\n", ddr_readl(DDRP_PGSR));
		result = -1;
	}
	return result;
}

static void ddr_training(unsigned int mode)
{
	unsigned int training_state = -1;
	dwc_debug("DDR training\n");
#ifndef CONFIG_DDR_FORCE_SOFT_TRAINING
	training_state = ddr_training_hardware(mode);
#endif
	if(training_state)
	{
		int i = 0;
		for (i = 0; i < 4; i++) {
			dwc_debug("DX%dGSR0: %x\n", i, ddr_readl(DDRP_DXGSR0(i)));
		}
		dump_ddrp_register();
#ifdef CONFIG_SPL_DDR_SOFT_TRAINING
		training_state = ddr_training_software(mode);
#endif // CONFIG_SPL_DDR_SOFT_TRAINING
	}
	if(DDR_TYPE_MODE(mode) == LPDDR)
		training_state = lpddr_retrain_bypass();
	if(training_state)
		hang();
}
static void ddr_impedance_matching(void)
{
#if defined(CONFIG_DDR_PHY_IMPED_PULLUP) && defined(CONFIG_DDR_PHY_IMPED_PULLDOWN)
	/**
	 * DDR3 240ohm RZQ output impedance:
	 * 	55.1ohm		0xc
	 * 	49.2ohm		0xd
	 * 	44.5ohm		0xe
	 * 	40.6ohm		0xf
	 * 	37.4ohm		0xa
	 * 	34.7ohm		0xb
	 * 	32.4ohm		0x8
	 * 	30.4ohm		0x9
	 * 	28.6ohm		0x18
	 */
	unsigned int i;
	i = ddr_readl(DDRP_ZQXCR0(0)) & ~0x3ff;
	i |= DDRP_ZQXCR_ZDEN
		| ((CONFIG_DDR_PHY_IMPED_PULLUP & 0x1f) << DDRP_ZQXCR_PULLUP_IMPED_BIT)
		| ((CONFIG_DDR_PHY_IMPED_PULLDOWN & 0x1f) << DDRP_ZQXCR_PULLDOWN_IMPED_BIT);
	ddr_writel(i, DDRP_ZQXCR0(0));
#endif
}
void ddr_phy_init(unsigned int mode)
{
	unsigned int timeout = 10000, i;
	unsigned int mr0_tmp = 1;
	bool	soft_training = false;
	unsigned int ddr_bl, ddr_cl;

	dwc_debug("DDR PHY init\n");
#ifdef CONFIG_PALLADIUM
	ddr_writel(0xfc00, DDRP_DTAR);  // 0x80010000 - 1K
#else
	ddr_writel(0x150000, DDRP_DTAR);
#endif
	/* DDR training address set*/
	ddr_phy_param_init(mode);
	ddr_chip_init(mode);
	ddr_training(mode);
	ddr_impedance_matching();
	dwc_debug("DDR PHY init OK\n");
}
/* DDR sdram init */
void sdram_init(void)
{

	int type = VARIABLE;
	unsigned int mode;
	unsigned int bypass = 0;
	unsigned int rate;
#ifdef CONFIG_DDR_TYPE_DDR3
	type = DDR3;
#endif
#ifdef CONFIG_DDR_TYPE_LPDDR
	type = LPDDR;
#endif
#ifdef CONFIG_DDR_TYPE_LPDDR2
	type = LPDDR2;
#endif

#ifdef CONFIG_DDR_TYPE_DDR2
	type = DDR2;
#endif

#ifndef CONFIG_DDR_HOST_CC
	struct ddrc_reg ddrc;
	struct ddrp_reg ddrp;
#ifndef CONFIG_DDR_TYPE_VARIABLE
	struct ddr_params ddr_params;
	ddr_params_p = &ddr_params;
#else
	ddr_params_p = &gd->arch.gi->ddr_params;
	ddr_params_p->freq = gd->arch.gi->cpufreq / gd->arch.gi->ddr_div;
#endif
	fill_in_params(ddr_params_p, type);
	ddr_params_creator(&ddrc, &ddrp, ddr_params_p);
	ddr_params_assign(&ddrc, &ddrp, ddr_params_p);
#endif /* CONFIG_DDR_HOST_CC */

	dwc_debug("sdram init start\n");
#ifndef CONFIG_FPGA
	clk_set_rate(DDR, gd->arch.gi->ddrfreq);
	reset_dll();
	rate = clk_get_rate(DDR);
#else
	rate = gd->arch.gi->ddrfreq;
#endif
#ifdef CONFIG_M200
	if(rate <= 150000000)
		bypass = 1;
#endif
	reset_controller();

#ifdef CONFIG_DDR_AUTO_SELF_REFRESH
	ddr_writel(0x0 ,DDRC_AUTOSR_EN);
#endif
	/*force CKE1 HIGH*/
	ddr_writel(DDRC_CFG_VALUE, DDRC_CFG);
	ddr_writel((1 << 1), DDRC_CTRL);
	mode = (type << 1) | (bypass & 1);
	/* DDR PHY init*/
	ddr_phy_init(mode);
	dump_ddrp_register();
	ddr_writel(0, DDRC_CTRL);

	/* DDR Controller init*/
	ddr_controller_init();
	/* DDRC address remap configure*/
	mem_remap();

	ddr_writel(ddr_readl(DDRC_STATUS) & ~DDRC_DSTATUS_MISS, DDRC_STATUS);
#ifdef CONFIG_DDR_AUTO_SELF_REFRESH
	if(!bypass)
		ddr_writel(0 , DDRC_DLP);
#ifdef CONFIG_FPGA
	ddr_writel(ddr_readl(DDRP_DSGCR)&~(1<<4),DDRP_DSGCR);
#endif
	ddr_writel(0x1 ,DDRC_AUTOSR_EN);
#endif
	dump_ddrc_register();
	dwc_debug("sdram init finished\n");
#undef DDRTYPE
}

phys_size_t initdram(int board_type)
{
#ifdef CONFIG_DDR_HOST_CC
	/* SDRAM size was calculated when compiling. */
#ifndef EMC_LOW_SDRAM_SPACE_SIZE
#define EMC_LOW_SDRAM_SPACE_SIZE 0x10000000 /* 256M */
#endif /* EMC_LOW_SDRAM_SPACE_SIZE */
	unsigned int ram_size;
	ram_size = (unsigned int)(DDR_CHIP_0_SIZE) + (unsigned int)(DDR_CHIP_1_SIZE);

	if (ram_size > EMC_LOW_SDRAM_SPACE_SIZE)
		ram_size = EMC_LOW_SDRAM_SPACE_SIZE;

	return ram_size;
#elif defined (CONFIG_BURNER)
	/* SDRAM size was defined in global info. */
	ddr_params_p = &gd->arch.gi->ddr_params;
	return ddr_params_p->size.chip0 + ddr_params_p->size.chip1;
#else
	ddr_params_p->dw32 = CONFIG_DDR_DW32;
	ddr_params_p->bank8 = DDR_BANK8;
	ddr_params_p->cs0 = CONFIG_DDR_CS0;
	ddr_params_p->cs1 = CONFIG_DDR_CS1;
	ddr_params_p->row = DDR_ROW;
	ddr_params_p->col = DDR_COL;
#ifdef DDR_ROW1
	ddr_params_p->row1 = DDR_ROW1;
#endif
#ifdef DDR_COL1
	ddr_params_p->col1 = DDR_COL1;
#endif
	return sdram_size(0, ddr_params_p) + sdram_size(1, ddr_params_p);
#endif
}
