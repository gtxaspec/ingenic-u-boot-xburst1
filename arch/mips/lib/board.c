/*
 * (C) Copyright 2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <serial.h>
#include <stdio_dev.h>
#include <version.h>
#include <net.h>
#include <environment.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <spi.h>
#include <mmc.h>

#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

#define BOOT_SCRIPT "fatload mmc 0 ${baseaddr} boot.scr"
#define ENV_FILE "fatload mmc 0 ${baseaddr} uEnv.txt"

extern int debug_socinfo;
extern int do_socinfo(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

DECLARE_GLOBAL_DATA_PTR;
ulong monitor_flash_len;
uchar enetaddr[6];

extern int jz_net_initialize(bd_t *bis);
extern int autoupdate_status;

void handle_gpio_settings(const char *env_var_name);
static char *failed = "*** failed ***\n";

/*
 * mips_io_port_base is the begin of the address space to which x86 style
 * I/O ports are mapped.
 */
const unsigned long mips_io_port_base = -1;

int __board_early_init_f(void)
{
	/*
	 * Nothing to do in this dummy implementation
	 */
	return 0;
}
int board_early_init_f(void)
	__attribute__((weak, alias("__board_early_init_f")));
int board_early_init_r(void)
	__attribute__((weak, alias("__board_early_init_f")));

static int init_func_ram(void)
{
#ifdef	CONFIG_BOARD_TYPES
	int board_type = gd->board_type;
#else
	int board_type = 0;	/* use dummy arg */
#endif
	puts("DRAM:  ");

	gd->ram_size = initdram(board_type);
	if (gd->ram_size > 0) {
		print_size(gd->ram_size, "\n\n");
		return 0;
	}
	puts(failed);
	return 1;
}

static int display_banner(void)
{

	printf("\n\n%s\n\n", version_string);
	return 0;
}

#ifndef CONFIG_SYS_NO_FLASH
static void display_flash_config(ulong size)
{
	puts("Flash: ");
	print_size(size, "\n");
}
#endif

static int init_baudrate(void)
{
	gd->baudrate = getenv_ulong("baudrate", 10, CONFIG_BAUDRATE);
	return 0;
}


/*
 * Breath some life into the board...
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependend #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t)(void);

init_fnc_t *init_sequence[] = {
	board_early_init_f,
	timer_init,
	env_init,		/* initialize environment */
#ifdef CONFIG_INCA_IP
	incaip_set_cpuclk,	/* set cpu clock according to env. variable */
#endif
	init_baudrate,		/* initialize baudrate settings */
#ifndef CONFIG_BURNER
	serial_init,		/* serial communications setup */
#endif
	console_init_f,
	display_banner,		/* say that we are here */
	checkboard,
	init_func_ram,
	NULL,
};


void board_init_f(ulong bootflag)
{
	gd_t gd_data, *id;
	bd_t *bd;
	init_fnc_t **init_fnc_ptr;
	ulong addr, addr_sp, len;
	ulong *s;

	/* Pointer is writable since we allocated a register for it.
	 */
	gd = &gd_data;
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("" : : : "memory");

	memset((void *)gd, 0, sizeof(gd_t));

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr)() != 0)
			hang();
	}

	/*
	 * Now that we have DRAM mapped and working, we can
	 * relocate the code and continue running from DRAM.
	 */
	addr = CONFIG_SYS_SDRAM_BASE + gd->ram_size;
#ifdef CONFIG_SYS_SDRAM_MAX_TOP
	addr = MIN(addr, CONFIG_SYS_SDRAM_MAX_TOP);
#endif

	/* We can reserve some RAM "on top" here.
	 */

	/* round down to next 4 kB limit.
	 */
	addr &= ~(4096 - 1);
	printf("Top of RAM usable for U-Boot at: %08lx\n", addr);
#ifdef CONFIG_LCD
#ifdef CONFIG_FB_ADDR
	gd->fb_base = CONFIG_FB_ADDR;
#else
	/* reserve memory for LCD display (always full pages) */
	addr = lcd_setmem(addr);
	printf("Reserving %ldk for U-Boot at: %08lx\n", len >> 10, addr);
	gd->fb_base = addr;
#endif /* CONFIG_FB_ADDR */
#endif /* CONFIG_LCD */

	/* Reserve memory for U-Boot code, data & bss
	 * round down to next 16 kB limit
	 */
	len = bss_end() - CONFIG_SYS_MONITOR_BASE;
	addr -= len;
	addr &= ~(16 * 1024 - 1);

	printf("Reserving %ldk for U-Boot at: %08lx\n", len >> 10, addr);

	 /* Reserve memory for malloc() arena.
	 */
	addr_sp = addr - TOTAL_MALLOC_LEN;
	printf("Reserving %dk for malloc() at: %08lx\n",
			TOTAL_MALLOC_LEN >> 10, addr_sp);

	/*
	 * (permanently) allocate a Board Info struct
	 * and a permanent copy of the "global" data
	 */
	addr_sp -= sizeof(bd_t);
	bd = (bd_t *)addr_sp;
	gd->bd = bd;
	printf("Reserving %zu Bytes for Board Info at: %08lx\n",
			sizeof(bd_t), addr_sp);

	addr_sp -= sizeof(gd_t);
	id = (gd_t *)addr_sp;
	printf("Reserving %zu Bytes for Global Data at: %08lx\n",
			sizeof(gd_t), addr_sp);

	/* Reserve memory for boot params.
	 */
	addr_sp -= CONFIG_SYS_BOOTPARAMS_LEN;
	bd->bi_boot_params = addr_sp;
	printf("Reserving %dk for boot params() at: %08lx\n",
			CONFIG_SYS_BOOTPARAMS_LEN >> 10, addr_sp);

	/*
	 * Finally, we set up a new (bigger) stack.
	 *
	 * Leave some safety gap for SP, force alignment on 16 byte boundary
	 * Clear initial stack frame
	 */
	addr_sp -= 16;
	addr_sp &= ~0xF;
	s = (ulong *)addr_sp;
	*s-- = 0;
	*s-- = 0;
	addr_sp = (ulong)s;
	printf("Stack Pointer at: %08lx\n", addr_sp);

	/*
	 * Save local variables to board info struct
	 */
	bd->bi_memstart	= CONFIG_SYS_SDRAM_BASE;	/* start of DRAM */
	bd->bi_memsize	= gd->ram_size;		/* size of DRAM in bytes */
	bd->bi_baudrate	= gd->baudrate;		/* Console Baudrate */

	memcpy(id, (void *)gd, sizeof(gd_t));

	relocate_code(addr_sp, id, addr);

	/* NOTREACHED - relocate_code() does not return */
}

/*
 * This is the next part if the initialization sequence: we are now
 * running from RAM and have a "normal" C environment, i. e. global
 * data can be written, BSS has been cleared, the stack size in not
 * that critical any more, etc.
 */
void board_init_r(gd_t *id, ulong dest_addr)
{
#ifndef CONFIG_SYS_NO_FLASH
	ulong size;
#endif
	bd_t *bd;

	gd = id;
	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */

	printf("Now running in RAM - U-Boot at: %08lx\n\n", dest_addr);

#ifdef CONFIG_XBURST_TRAPS
	traps_init();
#endif
	gd->relocaddr = dest_addr;
	gd->reloc_off = dest_addr - CONFIG_SYS_MONITOR_BASE;

	monitor_flash_len = image_copy_end() - dest_addr;

	board_early_init_r();

	serial_initialize();

	bd = gd->bd;

	/* The Malloc area is immediately below the monitor copy in DRAM */
	mem_malloc_init(CONFIG_SYS_MONITOR_BASE + gd->reloc_off -
			TOTAL_MALLOC_LEN, TOTAL_MALLOC_LEN);

#ifndef CONFIG_SYS_NO_FLASH
	/* configure available FLASH banks */
	size = flash_init();
	display_flash_config(size);
	bd->bi_flashstart = CONFIG_SYS_FLASH_BASE;
	bd->bi_flashsize = size;

#if CONFIG_SYS_MONITOR_BASE == CONFIG_SYS_FLASH_BASE
	bd->bi_flashoffset = monitor_flash_len;	/* reserved area for U-Boot */
#else
	bd->bi_flashoffset = 0;
#endif
#else
	bd->bi_flashstart = 0;
	bd->bi_flashsize = 0;
	bd->bi_flashoffset = 0;
#endif

#ifdef CONFIG_CMD_NAND
	puts("NAND:  ");
	nand_init();		/* go init the NAND */
#endif
#ifdef CONFIG_CMD_SFCNAND
	puts("SFC_NAND:  ");
	sfc_nand_init();
#endif
#ifdef CONFIG_CMD_ZM_NAND
	puts("NAND_ZM:	");
	nand_zm_init();
#endif

#if defined(CONFIG_CMD_ONENAND)
	onenand_init();
#endif

#ifdef CONFIG_GENERIC_MMC
	puts("MMC:   ");
	mmc_initialize(bd);
#endif

	/* relocate environment function pointers etc. */
	env_relocate();

	/* At this point, Environment has been setup, now we can use it */

	/* Generate a random mac address, and save it as soon as the environment is up*/
	#ifdef CONFIG_RANDOM_MACADDR
	// Check if a valid ethaddr is already set
	if (!eth_getenv_enetaddr("ethaddr", enetaddr)) {
		// No valid ethaddr set, generate and set a new one
		eth_random_enetaddr(enetaddr);
		if (eth_setenv_enetaddr("ethaddr", enetaddr)) {
			printf("Net:   Failed to set ethernet address\n");
		} else {
			printf("Net:   Random MAC address set successfully\n");
			saveenv();
		}
	} else {
		// A valid ethaddr is already set, so no need to set it again
		printf("Net:   HW Ethernet address: %02X:%02X:%02X:%02X:%02X:%02X\n",
				enetaddr[0], enetaddr[1], enetaddr[2], enetaddr[3], enetaddr[4], enetaddr[5]);
	}
	#endif

#if defined(CONFIG_PCI)
	/*
	 * Do pci configuration
	 */
	pci_init();
#endif

/** leave this here (after malloc(), environment and PCI are working) **/
	/* Initialize stdio devices */
	stdio_init();

	jumptable_init();

	/* Initialize the console (after the relocation and devices init) */
	console_init_r();
/** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** **/

	/* Initialize from environment */
	load_addr = getenv_ulong("loadaddr", 16, load_addr);

#ifdef CONFIG_CMD_SPI
	puts("SPI:   ");
	spi_init();		/* go init the SPI */
	puts("ready\n");
#endif

#ifdef CONFIG_USB_GADGET
extern void board_usb_init(void);
	board_usb_init();
#endif

#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	misc_init_r();

	/* Reset */
	#define ERASE_BLOCK_SIZE 0x8000 // 64KB

	char *overlay_str, *flashsize_str;
	unsigned long overlay, flashsize, length_to_erase, erase_block_size = 0x8000;
	char cmd[64]; // Buffer for command

	char* gpio_button_str = getenv("gpio_button");
	if (gpio_button_str) {
		unsigned gpio_number = (unsigned)simple_strtoul(gpio_button_str, NULL, 10);
		handle_gpio_settings("gpio_button");
		int value = gpio_get_value(gpio_number); // Get the GPIO value
		debug("GPIO %u value: %d\n", gpio_number, value); // Print the value

		if (value == 0) {
			printf("KEY:   Reset button pressed during boot, erasing ENV and Overlay...\n");
			run_command("env default -f -a", 0);
			/* Carry over the gpio between resets if desired */
			/* setenv("gpio_button", gpio_number); */
			saveenv();
			run_command("sf probe; sq probe", 0); // Initialize SPI flash and probe to set ENV variables

			overlay_str = getenv("overlay");
			flashsize_str = getenv("flash_len");
			if (overlay_str && flashsize_str) {
				overlay = simple_strtoul(overlay_str, NULL, 16);
				flashsize = simple_strtoul(flashsize_str, NULL, 16);

				// Calculate the initial length to erase before alignment
				length_to_erase = flashsize - overlay;

				// Align length to the next block size without exceeding flash size
				unsigned long aligned_length = (length_to_erase + erase_block_size - 1) & ~(erase_block_size - 1);
				if (overlay + aligned_length > flashsize) {
					// If alignment exceeds flash size, adjust length to not exceed flash
					aligned_length = flashsize - overlay;
					// Ensure adjusted length is also aligned to block size
					aligned_length = aligned_length & ~(erase_block_size - 1);
				}

				if (overlay + aligned_length <= flashsize) {
					debug("RST:   Overlay: 0x%lX, Flash size: 0x%lX, Length to erase: 0x%lX\n", overlay, flashsize, flashsize - overlay);
					sprintf(cmd, "sf erase 0x%lX 0x%lX", overlay, aligned_length);
					debug("Executing command: %s\n", cmd);
					if (run_command(cmd, 0) != 0) {
						printf("RST:   Error: Failed to execute erase command.\n");
					} else {
						debug("RST:   Successfully executed: %s\n", cmd);
					}
				} else {
					printf("RST:   Error: Erase range still exceeds flash size after adjustment.\n");
				}
			} else {
				printf("RST:   Error: overlay or flash_len environment variable is not set.\n");
			}
		}
	} else {
		printf("KEY:   Reset button undefined\n");
	}

	/* Platform Default GPIO Set */
	handle_gpio_settings("gpio_default");
#endif

#ifdef CONFIG_BITBANGMII
	bb_miiphy_init();
#endif

/* Try to get the value of the 'disable_sd' environment variable */
char* disable_sd = getenv("disable_sd");

#if defined(CONFIG_CMD_NET)
	int ret = 0;
	char* disable_eth = getenv("disable_eth");

#ifdef CONFIG_USB_ETHER_ASIX
	char* ethact = getenv("ethact");
	if (ethact && strncmp(ethact, "asx", 3) == 0) {
		if (run_command("usb start", 0) != 0) {
			printf("USB:   USB start failed\n");
		}
	}
#endif
	int networkInitializationAttempted = 0;

	/* Check if disable_eth is set to "true" */
	if (disable_eth && strcmp(disable_eth, "true") == 0) {
		/* disable_eth is true, so skip network initialization */
		printf("Net:   Network disabled\n");
		/* Handle GPIO settings since network init is skipped */
		handle_gpio_settings("gpio_default_net");
	} else {
		/* Attempt network initialization */
		networkInitializationAttempted = 1;
		ret = jz_net_initialize(gd->bd);
		if (ret < 0) {
			debug("Net:   Network initialization failed.\n");
			// Network initialization failed, handle GPIO settings here
			handle_gpio_settings("gpio_default_net");
		}
	}

	/* Check if disable_sd is "false" AND either network initialization was not attempted
	due to disable_eth being "true" OR it failed. */
	if (disable_sd != NULL && strcmp(disable_sd, "false") == 0 &&
		(!networkInitializationAttempted || ret < 0)) {
		/* MMC specific user GPIO set */
		handle_gpio_settings("gpio_mmc_power");
	}
#endif

/* IRCUT GPIO set */
handle_gpio_settings("gpio_ircut");
/* User defined GPIO set */
handle_gpio_settings("gpio_user");
/* User defined MOTOR GPIO set */
handle_gpio_settings("gpio_motor_v");
handle_gpio_settings("gpio_motor_h");

/* Check if 'disable_sd' was found and compare its value */
if (disable_sd != NULL && strcmp(disable_sd, "false") == 0) {
/* The environment variable 'disable_sd' exists and its value is "false" */
#ifdef CONFIG_AUTO_UPDATE
	run_command("sdupdate",0);
#endif
#ifdef CONFIG_CMD_SDSTART
	run_command("sdstart",0);
#endif

	printf("MMC:   Checking for boot/env files...\n");
	if (!run_command("fatload mmc 0 ${baseaddr} boot.scr", 0)) {
		printf("MMC:   Loading boot.scr\n");
		run_command(BOOT_SCRIPT, 0);
		run_command("source", 0);
	}

	if (!run_command("fatload mmc 0 ${baseaddr} uEnv.txt", 0)) {
		printf("MMC:   Loading uEnv.txt\n");
		run_command(ENV_FILE, 0);
		run_command("env import -t -r ${baseaddr} ${filesize};setenv filesize;saveenv;", 0);
	}

	if (autoupdate_status == 3) {
		printf("MMC:   Auto-update is set to 'full'. Resetting the device...\n");
		do_reset(NULL, 0, 0, NULL);
	}

} else {
	/* 'disable_sd' does not exist or is not "true" */
	printf("MMC:   SD card disabled\n");
}

	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;)
		main_loop();
	/* NOTREACHED - no way out of command loop except booting */
}

int checkboard(void)
{
	char output[100];
	puts("Platform: ISVP (Ingenic XBurst1)\n");
	sprintf(output, "Built profile: %s\n", SOC_VAR);
	puts(output);

	debug_socinfo = 0;
	do_socinfo(NULL, 0, 0, NULL);

	return 0;
}
