#include <common.h>
#include <command.h>


static int do_nor_features(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
  return(sfc_do_nor_features(argc, argv));
}

U_BOOT_CMD(
  dnor,  CONFIG_SYS_MAXARGS, 1,  do_nor_features,
  "nor flash features operations - v20230808",
  "\nFeatures Settings\n"
  "dnor w <WRSR> <DATA>\n"
  "       <WRSR> -- refers to the Write Status Register.\n"
  "       <DATA> -- refers to the data written.\n"

  "\nFeatures Gettings\n"
  "dnor r <RDSR>\n"
  "       <RDSR> -- refers to the address of the Read Status Register.\n\n"
);