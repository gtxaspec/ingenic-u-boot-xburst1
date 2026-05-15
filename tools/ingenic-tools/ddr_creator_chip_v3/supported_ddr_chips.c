// #include <config.h>
#include <config.h>
#include <stdio.h>
#include "ddr_params_creator.h"

/* supported_ddr_chips.c */
struct ddr_chip_info supported_ddr_chips[] = {

#ifdef CONFIG_DDR3_W631GU6NG
	DDR3_W631GU6NG,
#endif

#ifdef CONFIG_DDR3_NK5CC64M16HQ3
	DDR3_NK5CC64M16HQ3,
#endif

#ifdef CONFIG_DDR3_W632GU6NG
	DDR3_W632GU6NG,
#endif

#ifdef CONFIG_DDR3_W634GU6RB
	DDR3_W634GU6RB,
#endif

#ifdef CONFIG_DDR3_W638GU6QB
	DDR3_W638GU6QB,
#endif

#ifdef CONFIG_LPDDR3_W63AH6N2B_BJ
	LPDDR3_W63AH6N2B_BJ,
#endif

#ifdef CONFIG_DDR2_M14D5121632A
	DDR2_M14D5121632A,
#endif

#ifdef CONFIG_DDR2_W9751V6NG
	DDR2_W9751V6NG,
#endif
};

int init_supported_ddr(void)
{

	return sizeof(supported_ddr_chips)/ sizeof(struct ddr_chip_info);
}

void create_supported_ddr_params(struct ddr_reg_value *reg_values)
{
	struct ddr_chip_info *p;
	struct ddr_reg_value *reg;

	int i;

	for(i = 0; i < sizeof(supported_ddr_chips) / sizeof(struct ddr_chip_info); i++) {
		p = &supported_ddr_chips[i];
		reg = &reg_values[i];

		__ps_per_tck = (1000000000 / (p->freq / 1000));

		p->init(p);

		create_one_ddr_params(p, reg);
	}
}
