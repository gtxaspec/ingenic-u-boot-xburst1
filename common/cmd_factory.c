#include <common.h>
#include <command.h>
#include <configs/isvp_common.h>

#define ALIGNMENT_BLOCK_SIZE	CONFIG_SFC_MIN_ALIGN
#define DEFAULT_DELAY		CONFIG_BOOTDELAY

static bool prompt_and_wait_for_interrupt(int delay) {
	printf("RST:   Factory reset requested.\n");
	printf("RST:   To cancel factory reset, press Ctrl-C within the next %d second(s)...\n", delay);
	unsigned long start = get_timer(0);

	while (get_timer(0) - start < delay * 1000) {
		if (ctrlc()) {
			printf("RST:   Factory reset canceled by user.\n");
			return true;
		}
		udelay(10000);
	}
	return false;
}

static int erase_config_partition_from_mtdparts(void)
{
	const char *mtdparts = getenv("mtdparts");
	if (!mtdparts) {
		printf("RST:   Error: mtdparts not set in environment.\n");
		return CMD_RET_FAILURE;
	}

	/* reading mtdparts value will give something like:
	* "jz_sfc:256k(boot),64k(env),192k(config),${kern_size_dec}(kernel),${rootfs_size}(rootfs),-(rootfs_data)${update}"
	*
	* Skip the device name: find the colon.
	*/
	const char *p = strchr(mtdparts, ':');
	if (!p) {
		printf("RST:   Error: mtdparts missing colon separator.\n");
		return CMD_RET_FAILURE;
	}
	p++; // p now points to the partition list

	/*
	* Tokenize the partition list by commas.
	* We expect tokens in order:
	*    Token 1: "256k(boot)"
	*    Token 2: "64k(env)"
	*    Token 3: "<size>k(config)"
	*/

	char partition[64];
	int token = 0;
	const char *cursor = p;
	const char *token_start = cursor;
	const char *token_end;
	unsigned long part_sizes[3] = {0, 0, 0};

	while ((token_end = strchr(cursor, ',')) != NULL && token < 3) {
		int len = token_end - token_start;
		if (len >= sizeof(partition))
			len = sizeof(partition) - 1;
		memcpy(partition, token_start, len);
		partition[len] = '\0';

		/* Extract the numeric part (in kilobytes) from the token.
		*/
		char num_buf[16];
		int i = 0;
		while (partition[i] && partition[i] >= '0' && partition[i] <= '9' && i < sizeof(num_buf)-1) {
			num_buf[i] = partition[i];
			i++;
		}
		num_buf[i] = '\0';
		if (i == 0) {
			printf("RST:   Error: Unable to parse partition size from token: %s\n", partition);
			return CMD_RET_FAILURE;
		}
		part_sizes[token] = simple_strtoul(num_buf, NULL, 10) * 1024; /* convert kilobytes to bytes */
		token++;
		cursor = token_end + 1;
		token_start = cursor;
	}

	/* We must have at least three tokens for boot, env, and config. */
	if (token < 3) {
		printf("RST:   Error: mtdparts does not contain at least three partitions.\n");
		return CMD_RET_FAILURE;
	}

	/* Compute config partition start address:
	* config_start = boot_size + env_size.
	*/
	unsigned long config_start = part_sizes[0] + part_sizes[1];
	unsigned long config_size = part_sizes[2];  /* Third token is config size */

	/* For debugging, print parsed values: */
	printf("RST:   Boot size: 0x%lX, Env size: 0x%lX, Config size: 0x%lX\n",
		part_sizes[0], part_sizes[1], config_size);
	printf("RST:   Computed config partition start: 0x%lX\n", config_start);

	/*
	* The command should be: "sf erase <start> <length>"
	*/
	char cmd[64];
	sprintf(cmd, "sf erase 0x%lX 0x%lX", config_start, config_size);
	printf("RST:   Executing config partition erase command: %s\n", cmd);
	if (run_command(cmd, 0) != 0) {
		printf("RST:   Error: Failed to execute config partition erase command.\n");
		return CMD_RET_FAILURE;
	} else {
		printf("RST:   Successfully erased config partition.\n");
	}

	return CMD_RET_SUCCESS;
}


/*
* factory_reset_internal() - Perform factory reset operations.
* @force: if nonzero, bypass the GPIO button check.
*
* Returns CMD_RET_SUCCESS if reset operations succeed,
* or CMD_RET_FAILURE otherwise.
*/
static int factory_reset_internal(int force)
{
	int ret_status = CMD_RET_SUCCESS;  /* Assume success */
	char *overlay_str, *flashsize_str;
	unsigned long overlay, flashsize, length_to_erase, erase_block_size;
	char cmd[64]; /* Buffer for command */

	if (!force) {
		char *gpio_button_str = getenv("gpio_button");
		if (gpio_button_str) {
			unsigned gpio_number = (unsigned)simple_strtoul(gpio_button_str, NULL, 10);
			udelay(250000); /* Wait for input GPIO to stabilize */
			int value = gpio_get_value(gpio_number); /* Get the GPIO value */
			debug("RST:   GPIO %u value: %d\n", gpio_number, value);
			if (value != 0) {
				printf("RST:   Reset button not pressed; skipping factory reset.\n");
				return CMD_RET_FAILURE;
			}
		} else {
			printf("RST:   Reset button undefined; skipping factory reset.\n");
			return CMD_RET_FAILURE;
		}
	}

	/* allow user to cancel with delay */
	if (prompt_and_wait_for_interrupt(DEFAULT_DELAY)) {
		return CMD_RET_FAILURE; /* Return early if user interrupted */
	}

	/* Proceed with reset operations (either forced or button checked) */
	printf("RST:   Factory reset initiated%s.\n", force ? " (forced)" : "");

	if (run_command("sf probe-alt", 0) != 0) {
		printf("RST:   Error: SPI flash probe failed.\n");
		ret_status = CMD_RET_FAILURE;
	}

	/* Use the defined block size */
	erase_block_size = ALIGNMENT_BLOCK_SIZE;

	if (run_command("sq probe-alt", 0) != 0) {
		printf("RST:   Error: Flash probing (sq probe-alt) failed.\n");
		ret_status = CMD_RET_FAILURE;
	}

	/* Wipe the config partition */
	erase_config_partition_from_mtdparts();

	overlay_str = getenv("overlay");
	flashsize_str = getenv("flash_len");
	if (overlay_str && flashsize_str) {
		overlay = simple_strtoul(overlay_str, NULL, 16);
		flashsize = simple_strtoul(flashsize_str, NULL, 16);

		/* Align overlay address to the erase block size */
		unsigned long aligned_overlay = (overlay + erase_block_size - 1) & ~(erase_block_size - 1);

		/* Calculate the length to erase */
		length_to_erase = flashsize - aligned_overlay;
		unsigned long aligned_length = (length_to_erase + erase_block_size - 1) & ~(erase_block_size - 1);
		if (aligned_overlay + aligned_length > flashsize) {
			aligned_length = flashsize - aligned_overlay;
			aligned_length = aligned_length & ~(erase_block_size - 1);
		}

		if (aligned_overlay + aligned_length <= flashsize) {
			debug("RST:   Aligned Overlay: 0x%lX, Flash size: 0x%lX, Length to erase: 0x%lX\n",
				aligned_overlay, flashsize, aligned_length);
			sprintf(cmd, "sf erase 0x%lX 0x%lX", aligned_overlay, aligned_length);
			debug("RST:   Executing command: %s\n", cmd);
			printf("RST:   Erasing overlay...\n");
			if (run_command(cmd, 0) != 0) {
				printf("RST:   Error: Failed to execute erase command.\n");
				ret_status = CMD_RET_FAILURE;
			} else {
				debug("RST:   Successfully executed: %s\n", cmd);
			}
		} else {
			printf("RST:   Error: Erase range still exceeds flash size after adjustment.\n");
			ret_status = CMD_RET_FAILURE;
		}
	} else {
		printf("RST:   Error: overlay or flash_len environment variable is not set.\n");
		ret_status = CMD_RET_FAILURE;
	}

	debug("RST:   Reset Environment...");
	if (run_command("env default -f -a", 0) != 0) {
		printf("RST:   Error: Failed to reset ENV defaults.\n");
		ret_status = CMD_RET_FAILURE;
	}
	saveenv();

	return ret_status;
}

/*
* do_factory_reset() - Command handler for factory reset.
*
* Accepts two forms:
*   factory reset      -> Force reset (ignores button)
*   factory reset-boot -> Reset only if reset button is pressed
*/
static int do_factory_reset(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2 ||
		(strcmp(argv[1], "reset") != 0 && strcmp(argv[1], "reset-boot") != 0)) {
		puts("Usage: factory reset\n");
		return CMD_RET_USAGE;
	}

	int ret;
	if (strcmp(argv[1], "reset") == 0) {
		/* "reset" mode: do not check button, always perform reset */
		ret = factory_reset_internal(1);
	} else { /* "reset-boot" mode: check for button */
		ret = factory_reset_internal(0);
	}

	if (ret == CMD_RET_SUCCESS) {
		printf("RST:   Factory reset successful, resetting system...\n");
		run_command("reset", 0);
		return CMD_RET_SUCCESS;
	} else {
		debug("RST:   Factory reset not performed (or failed).\n");
		return CMD_RET_FAILURE;
	}
}

U_BOOT_CMD(
	factory, 2, 1, do_factory_reset,
	"Erase overlay partition and reset ENV",
	"reset - Erase overlay partition and reset ENV"
);
