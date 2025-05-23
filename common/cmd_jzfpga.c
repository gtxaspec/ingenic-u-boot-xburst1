#include <common.h>

#define BOOT_ROM_ADDR (0xba032000)
#if defined(CONFIG_JZ4775)
#define BOOT_RAM_LOADADDR (0xb3414000)
#define BOOT_ROM_SIZE (16 * 1024)
#elif defined(CONFIG_JZ4780)
#define BOOT_RAM_LOADADDR (0xb3414000)	//??
#define BOOT_ROM_SIZE (32 * 1024)
#elif defined(CONFIG_M200)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (32 * 1024)
#elif defined(CONFIG_T10)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (32 * 1024)
#elif defined(CONFIG_T5)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (16 * 1024)
#elif defined(CONFIG_T15G)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (16 * 1024)
#elif defined(CONFIG_T30)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (16 * 1024)
#elif defined(CONFIG_T21)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (32 * 1024)
#elif defined(CONFIG_T23)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (16 * 1024)
#elif defined(CONFIG_T31)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (16 * 1024)
#elif defined(CONFIG_C100)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (16 * 1024)
#elif defined(CONFIG_T40)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (22 * 1024)
#elif defined(CONFIG_T41)
#define BOOT_RAM_LOADADDR (0xb3418000)
#define BOOT_ROM_SIZE (22 * 1024)
#endif

static int do_jbootrom_part(unsigned long bootrom_addr)
{
	unsigned int ram_addr, store_addr, rom_addr;
	unsigned int *src,*dst;
	volatile unsigned int *aa;
	unsigned int count = 0;
	unsigned int i = 0;
	void (*bootrom)(void);

	ram_addr = (unsigned int)BOOT_RAM_LOADADDR;
	store_addr = (unsigned int)bootrom_addr;
	rom_addr = 0xbfc00000;

	/* Dump 8 byte info */
	aa = (unsigned int volatile *)rom_addr;
	printf("dst ori(%p):\n", aa);
	for (i = 0; i < 8; i++)
		printf("0x%08x\n", aa[i]);
	aa = (unsigned int volatile *)ram_addr;
	for (i = 0; i < 8; i++)
		aa[i] = i;
	aa = (unsigned int volatile *)rom_addr;
	printf("dst before(%p):\n", aa);
	for (i = 0; i < 8; i++)
		printf("0x%08x\n", aa[i]);

	/* Copy bootrom.bin to bootram*/
	src = (unsigned int *)store_addr;
	dst = (unsigned int *)ram_addr;
	printf("copy %d byte data fromt %p to %p\n", BOOT_ROM_SIZE, src, dst);
	while (count < (BOOT_ROM_SIZE/sizeof(int))) {
		*dst++  = src[count++];
	}

	/* Dump first 8 byte*/
	aa = (volatile unsigned int *)store_addr;
	printf("src(%p):\n", aa);
	for (i = 0; i < 8; i++)
		printf("0x%08x\n",aa[i]);

	aa = (volatile unsigned int *)rom_addr;
	printf("dst(%p):\n", aa);
	for (i = 0; i < 8; i++)
		printf("0x%08x\n",aa[i]);

	/* Jump bootrom */
	printf("jump to :0x%x\n", rom_addr);
	bootrom = (void (*)(void))rom_addr;
	flush_cache_all();
	(*bootrom)();
	return 0;
}


static int do_jbootrom(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int bootrom_addr;

#ifndef BOOT_RAM_LOADADDR
#error  "BOOT_RAM_LOADADDR is not defined"
#endif
#ifndef BOOT_ROM_SIZE
#error  "BOOT_ROM_SIZE is not defined"
#endif
	if (argc != 2)
		bootrom_addr = BOOT_ROM_ADDR;
	else
		bootrom_addr = simple_strtoul(argv[1], NULL, 16);
	do_jbootrom_part(bootrom_addr);
	return 1;
}

U_BOOT_CMD(
	jbootrom, 2, 1, do_jbootrom,
	"do jbootrom",
	"jbootrom store_addr\n"
	"store_addr : bootrom store addr\n"
	);
