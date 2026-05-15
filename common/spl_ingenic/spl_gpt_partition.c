#include <common.h>
#include <generated/spl_gpt_tab_data.h>
#include "spl_gpt_partition.h"

int spl_get_built_in_gpt_partition(
	const char *name, unsigned int *offset, unsigned int *size)
{
	int i;
	for (i = 0; i < ARRAY_SIZE(partition_tab); i++) {
		if (!strcmp(partition_tab[i].name, name)) {
			if (offset)
				*offset = partition_tab[i].offset_sector;
			if (size)
				*size = partition_tab[i].size_sector;
			return 0;
		}
	}
	return -1;
}
