/* SPDX-License-Identifier: GPL-2.0+ */
/*
* Xburst u-boot global infomation structure.
*
* Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
*
*/

#ifndef __GLOBAL_INFO_H__
#define __GLOBAL_INFO_H__

#include <asm/arch/gpio.h>

#ifdef CONFIG_BURNER
struct address_line_test {
	unsigned int enable;	/* Test the case or not(enable test: 1, disable test: 0) */
	unsigned char stat;		/* test result(Err:1, Right: 0) */
	unsigned char bank_line;	/* the error bank_line(1: error line, 0: right line) */
	unsigned int addr_line;		/* the error addr_line(1: error line, 0: right line) */
};

struct data_line_test {
	unsigned int enable;
	unsigned char stat;
	unsigned int data_line;		/* the error data_line(1: error line, 0: right line) */
};

struct cache_test {
	unsigned int enable;
	unsigned char stat;
	unsigned char remap;
};

struct uncache_test {
	unsigned int enable;
	unsigned char stat;
	unsigned char remap;
};

struct uncache_all_test {
	unsigned int enable;
	unsigned char stat;
	unsigned char remap;
};

struct dma_test {
	unsigned int enable;
	unsigned char stat;
};

struct cpu_dma_test {
	unsigned int enable;
	unsigned char stat;
};

struct ddr_test_burner {
	struct address_line_test address_line_test;
	struct data_line_test data_line_test;
	struct cache_test cache_test;
	struct uncache_test uncache_test;
	struct uncache_all_test uncache_all_test;
	struct dma_test dma_test;
	struct cpu_dma_test cpu_dma_test;
};
#endif

typedef union soc {
	uint32_t d32;
	struct {
		unsigned PRJ007:1;
		unsigned PRJ008:1;
		unsigned reserved:14;
		unsigned soc_version:8;
		unsigned ddr_id:8;
	} xburst; /* soc type */
} soc_type;

struct global_info {
	uint32_t extal;
	uint32_t cpufreq;
	uint32_t ddrfreq;
	uint32_t ddr_div;
	uint32_t uart_idx;
	uint32_t baud_rate;
	soc_type soc;
#ifdef CONFIG_BURNER
	//	struct ddr_params ddr_params;
	struct jz_gpio_func_def uart_gpio[1];
	uint32_t nr_gpio_func;
	struct jz_gpio_func_def gpio[0];
	struct ddr_test_burner ddr_test;
#endif

	struct {
		uint32_t ddr_cfg;
		uint32_t ddr_mmap0;
		uint32_t ddr_mmap1;
		uint32_t ddr_timing4;
		uint32_t ddr_autosr;
		uint32_t ddr_remap_array[5];
	} ddr_change_param;

};

#endif /* __GLOBAL_INFO_H__ */
