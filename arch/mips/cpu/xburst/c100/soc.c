/*
 * C100 common routines
 *
 * Copyright (c) 2017 Ingenic Semiconductor Co.,Ltd
 * Author: Zoro <ykli@ingenic.cn>
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

#define DEBUG
#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/clk.h>
#include <asm/arch/cpm.h>
#include <spl.h>
#include <asm/ddr_innophy.h>

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
extern void validate_cache(void);

void board_init_f(ulong dummy)
{
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

	gpio_init();

#ifndef CONFIG_FPGA
	/* Init uart first */
	enable_uart_clk();
#endif

#ifdef CONFIG_SPL_SERIAL_SUPPORT
	preloader_console_init();
#endif

#ifndef CONFIG_FPGA
	debug("Timer init\n");
	timer_init();

#ifdef CONFIG_SPL_REGULATOR_SUPPORT
	debug("regulator set\n");
	spl_regulator_set();
#endif
	debug("CLK stop\n");
	clk_prepare();

	debug("PLL init\n");
	pll_init();

	debug("CLK init\n");
	clk_init();
#endif
	debug("SDRAM init\n");
	sdram_init();
	debug("SDRAM init ok\n");


#if 0 /* a simple ddr training */
	/*MUST access 0xa3fffffc address */
	//*(volatile unsigned int *)0xa3fffffc = 0x12345678;
	*(volatile unsigned int *)0xa0000000 = 0x02345670;
	u8 a = 0;
	u32 d = 0x12345678;
	u32 addr = 0xa0000000;
	for (a = 0; a < 128; a++) {
		//writel(a, C100_REG46);
		//writel(a, C100_REG56);

		*(volatile unsigned int *)addr = d;
		printf("4:0x%x 0x%x\n", addr, *(u32*)addr);
		d = d + 0x10001;
		addr = addr + 4;
	}
	//writel(30, C100_REG46);
	//writel(30, C100_REG56);
#endif

#if 0
	{
		volatile u32 tmp = 0;
		u32 data = 0;
		for (tmp = 0xa0000000; tmp < 0xa0000000 + 0x8000000/4; tmp+=4) {
			u32 i = 0;
			u32 td = 0x12345678;
			for (i = 0; i < 16; i++) {
				*(u32*)tmp = td;
				data = *(u32*)tmp;
				if (data != td) {
					printf("\n##### add(%d) = %p, want = %x, get = %x\n", i, tmp, td, data);
					mdelay(3000);
				}
				td = td << 1;
			}
			//printf("#");
		}
	}
#endif

#ifdef CONFIG_DDR_TEST
	ddr_basic_tests();
#endif
#ifndef CONFIG_BURNER
	/* Clear the BSS */
	memset(__bss_start, 0, (char *)&__bss_end - __bss_start);
	debug("board_init_r\n");
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

unsigned long do_go_exec(ulong (*entry)(int, char * const []), int argc,
				 char * const argv[])
{
	printf("Flush cache all before jump. \n");
	flush_cache_all();

	return entry (argc, argv);
}
