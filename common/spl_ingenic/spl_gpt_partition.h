#ifndef _SPL_GPT_PARTITION_H_
#define _SPL_GPT_PARTITION_H_

int spl_get_built_in_gpt_partition(
	const char *name, unsigned int *offset, unsigned int *size);

#endif /* _SPL_GPT_PARTITION_H_ */
