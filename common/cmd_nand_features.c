#include <common.h>
#include <command.h>

static int do_nand_features(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
  unsigned char cmd[2];

	if(argc < 2)return -1;

	if(strcmp(argv[1], "reset")  == 0){ // Soft Reset (FFH)
		if(argc != 2)return -1;
		sfc_do_nand_features(1, 0, 0);
		return 0;
	}

	if(strcmp(argv[1], "r")  == 0){ //Get Features (0FH)
		if(argc != 3)return -1;
		cmd[0] = simple_strtoul(argv[2], NULL, 16);

    return sfc_do_nand_features(0, 0, cmd);
	}

	if(strcmp(argv[1], "w")  == 0){ // Set Features (1FH)
		if(argc != 4)return -1;
		cmd[0] = simple_strtoul(argv[2], NULL, 16);
		cmd[1] = simple_strtoul(argv[3], NULL, 16);

		return sfc_do_nand_features(0, 1, cmd);
	}

	return -1;
}

U_BOOT_CMD(
	dnand, CONFIG_SYS_MAXARGS, 1, do_nand_features,
	"nand flash features operations - v3.0",

	"\nFeatures Gettings\n"
  "dnand r <Addr>\n"
  "        <Addr> -- refers to the address of the Get Features Register Addr.\n"

  "\nFeatures Settings\n"
  "dnand w <Addr> <Data>\n"
  "        <Addr> -- refers to the Set Features Register Addr.\n"
  "        <Data> -- refers to the data written.\n"

  "\nReset Operations\n"
  "dnand reset\n"
  "        The RESET(FFH) command stops all operations and the status.\n"
);
