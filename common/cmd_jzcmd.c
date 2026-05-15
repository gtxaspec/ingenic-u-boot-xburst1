#include <common.h>
#include <asm/arch-t20/soctype.h>

static int do_jzsoc(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int type;
	type = get_t20_type();
	if ((type>=0)&&(type<T20_TYPE_END)) {
		printf("SOC: %s\n", t20_soc_type_str[type]);
	} else {
		printf("SOC: %s\n", "NULL");
	}
	return 0;
}

U_BOOT_CMD(
		jzsoc, 1, 0, do_jzsoc,
		"jz soc info",
		"show jz soc info\n"
		);
