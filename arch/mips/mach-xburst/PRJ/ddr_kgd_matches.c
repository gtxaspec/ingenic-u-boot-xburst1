#include <generated/ddr_reg_values.h>
#include <common.h>
// #include <ddr/ddrc.h>
// #include "ddr_config.h"

extern struct ddr_reg_value supported_ddr_reg_values[];
extern struct ddr_reg_value *global_reg_value;

void matches_ddr_params(unsigned int ddr_id)
{
	unsigned int size = 0;
	unsigned int found = 0;
	unsigned int i;
voe();
	size = ARRAY_SIZE(supported_ddr_reg_values);
voe();
	if (size == 1)
	{
		voe();
		found = 1;
		global_reg_value = &supported_ddr_reg_values[0];
		voe();
		return ;
	}
	else
	{
		for (i = 0; i < size; i++)
		{
			if (ddr_id == supported_ddr_reg_values[i].h.id)
			{
				global_reg_value = &supported_ddr_reg_values[i];
				found = 1;
				break;
			}
		}
		if (!found)
			printf("unknown kgd info %x\n", ddr_id);
	}

}
