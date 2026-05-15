/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 * Licensed under the GPL-2 or later.
 */

#include <common.h>
#include <div64.h>
#include <malloc.h>
#include <spi_flash.h>
#include <asm/arch/sfc.h>

#include <asm/io.h>

#ifndef CONFIG_SF_DEFAULT_SPEED
# define CONFIG_SF_DEFAULT_SPEED	1000000
#endif
#ifndef CONFIG_SF_DEFAULT_MODE
# define CONFIG_SF_DEFAULT_MODE		SPI_MODE_3
#endif
#ifndef CONFIG_SF_DEFAULT_CS
# define CONFIG_SF_DEFAULT_CS		0
#endif
#ifndef CONFIG_SF_DEFAULT_BUS
# define CONFIG_SF_DEFAULT_BUS		0
#endif

static struct spi_flash *flash0;
static struct spi_flash *flash1;

#define PRINT_TIME

/*
 * This function computes the length argument for the erase command.
 * The length on which the command is to operate can be given in two forms:
 * 1. <cmd> offset len  - operate on <'offset',  'len')
 * 2. <cmd> offset +len - operate on <'offset',  'round_up(len)')
 * If the second form is used and the length doesn't fall on the
 * sector boundary, than it will be adjusted to the next sector boundary.
 * If it isn't in the flash, the function will fail (return -1).
 * Input:
 *    arg: length specification (i.e. both command arguments)
 * Output:
 *    len: computed length for operation
 * Return:
 *    1: success
 *   -1: failure (bad format, bad address).
 */
static int sf_parse_len_arg(unsigned char sfc_index,char *arg, ulong *len)
{
	char *ep;
	char round_up_len; /* indicates if the "+length" form used */
	size_t sector_size;
	ulong len_arg;

	if(sfc_index == SFC_0) {
		sector_size = flash0->sector_size;
	} else if(sfc_index == SFC_1) {
		sector_size = flash1->sector_size;
	}
	round_up_len = 0;
	if (*arg == '+') {
		round_up_len = 1;
		++arg;
	}

	len_arg = simple_strtoul(arg, &ep, 16);
	if (ep == arg || *ep != '\0')
		return -1;

	if (round_up_len && sector_size > 0)
		*len = ROUND(len_arg, sector_size);
	else
		*len = len_arg;

	return 1;
}

/**
 * This function takes a byte length and a delta unit of time to compute the
 * approximate bytes per second
 *
 * @param len		amount of bytes currently processed
 * @param start_ms	start time of processing in ms
 * @return bytes per second if OK, 0 on error
 */
static ulong bytes_per_second(unsigned int len, ulong start_ms)
{
	/* less accurate but avoids overflow */
	if (len >= ((unsigned int) -1) / 1024)
		return len / (max(get_timer(start_ms) / 1024, 1));
	else
		return 1024 * len / max(get_timer(start_ms), 1);
}

static int do_spi_flash_probe(int argc, char * const argv[],unsigned char sfc_index)
{
	unsigned int bus = CONFIG_SF_DEFAULT_BUS;
	unsigned int cs = CONFIG_SF_DEFAULT_CS;
	unsigned int speed = CONFIG_SF_DEFAULT_SPEED;
	unsigned int mode = CONFIG_SF_DEFAULT_MODE;
	char *endp;
	struct spi_flash *new;

	if (argc >= 2) {
		cs = simple_strtoul(argv[1], &endp, 0);
		if (*argv[1] == 0 || (*endp != 0 && *endp != ':'))
			return -1;
		if (*endp == ':') {
			if (endp[1] == 0)
				return -1;

			bus = cs;
			cs = simple_strtoul(endp + 1, &endp, 0);
			if (*endp != 0)
				return -1;
		}
	}

	if (argc >= 3) {
		speed = simple_strtoul(argv[2], &endp, 0);
		if (*argv[2] == 0 || *endp != 0)
			return -1;
	}
	if (argc >= 4) {
		mode = simple_strtoul(argv[3], &endp, 16);
		if (*argv[3] == 0 || *endp != 0)
			return -1;
	}

	new = spi_flash_probe(sfc_index,bus, cs, speed, mode);

	if (!new) {
		printf("Failed to initialize SPI flash at %u:%u\n", bus, cs);
		return 1;
	}

	if (flash0 && sfc_index == SFC_0)
		spi_flash_free(flash0);
	if (flash1 && sfc_index == SFC_1)
		spi_flash_free(flash1);

	if (sfc_index == SFC_0)
		flash0 = new;
	else if (sfc_index == SFC_1)
		flash1 = new;
	else;

	return 0;
}

/**
 * Write a block of data to SPI flash, first checking if it is different from
 * what is already there.
 *
 * If the data being written is the same, then *skipped is incremented by len.
 *
 * @param flash		flash context pointer
 * @param offset	flash offset to write
 * @param len		number of bytes to write
 * @param buf		buffer to write from
 * @param cmp_buf	read buffer to use to compare data
 * @param skipped	Count of skipped data (incremented by this function)
 * @return NULL if OK, else a string containing the stage which failed
 */
static const char *spi_flash_update_block(unsigned char sfc_index,struct spi_flash *flash, u32 offset,
		size_t len, const char *buf, char *cmp_buf, size_t *skipped)
{
	debug("offset=%#x, sector_size=%#x, len=%#zx\n",
		offset, flash->sector_size, len);
	if (spi_flash_read(sfc_index,flash, offset, len, cmp_buf))
		return "read";
	if (memcmp(cmp_buf, buf, len) == 0) {
		debug("Skip region %x size %zx: no change\n",
			offset, len);
		*skipped += len;
		return NULL;
	}
	if (spi_flash_erase(sfc_index,flash, offset, len))
		return "erase";
	if (spi_flash_write(sfc_index,flash, offset, len, buf))
		return "write";
	return NULL;
}

/**
 * Update an area of SPI flash by erasing and writing any blocks which need
 * to change. Existing blocks with the correct data are left unchanged.
 *
 * @param flash		flash context pointer
 * @param offset	flash offset to write
 * @param len		number of bytes to write
 * @param buf		buffer to write from
 * @return 0 if ok, 1 on error
 */
static int spi_flash_update(unsigned char sfc_index,struct spi_flash *flash, u32 offset,
		size_t len, const char *buf)
{
	const char *err_oper = NULL;
	char *cmp_buf;
	const char *end = buf + len;
	size_t todo;		/* number of bytes to do in this pass */
	size_t skipped = 0;	/* statistics */
	const ulong start_time = get_timer(0);
	size_t scale = 1;
	const char *start_buf = buf;
	ulong delta;

	if (end - buf >= 200)
		scale = (end - buf) / 100;
	cmp_buf = malloc(flash->sector_size);
	if (cmp_buf) {
		ulong last_update = get_timer(0);

		for (; buf < end && !err_oper; buf += todo, offset += todo) {
			todo = min(end - buf, flash->sector_size);
			if (get_timer(last_update) > 100) {
				printf("   \rUpdating, %zu%% %lu B/s",
					100 - (end - buf) / scale,
					bytes_per_second(buf - start_buf,
							 start_time));
				last_update = get_timer(0);
			}
			err_oper = spi_flash_update_block(sfc_index,flash, offset, todo,
					buf, cmp_buf, &skipped);
		}
	} else {
		err_oper = "malloc";
	}
	free(cmp_buf);
	putc('\r');
	if (err_oper) {
		printf("SPI flash failed in %s step\n", err_oper);
		return 1;
	}

	delta = get_timer(start_time);
	printf("%zu bytes written, %zu bytes skipped", len - skipped,
		skipped);
	printf(" in %ld.%lds, speed %ld B/s\n",
		delta / 1000, delta % 1000, bytes_per_second(len, start_time));

	return 0;
}

static int do_spi_flash_read_write(int argc, char * const argv[],unsigned char sfc_index)
{
	unsigned long addr;
	unsigned long offset;
	unsigned long len;
	void *buf;
	char *endp;
	int ret = 1;

	if (argc < 4)
		return -1;

	addr = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	offset = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		return -1;
	len = simple_strtoul(argv[3], &endp, 16);
	if (*argv[3] == 0 || *endp != 0)
		return -1;

	/* Consistency checking */
	if(sfc_index == SFC_0) {
		if (offset + len > flash0->size) {
			printf("ERROR: attempting %s past flash size (%#x)\n",
				argv[0], flash0->size);
			return 1;
		}
	} else if(sfc_index == SFC_1) {
		if (offset + len > flash1->size) {
			printf("ERROR: attempting %s past flash size (%#x)\n",
				argv[0], flash1->size);
			return 1;
		}
	}

	buf = map_physmem(addr, len, MAP_WRBACK);
	if (!buf) {
		puts("Failed to map physical memory\n");
		return 1;
	}

	if (strcmp(argv[0], "update") == 0) {
		if(sfc_index == SFC_0) {
			ret = spi_flash_update(sfc_index,flash0, offset, len, buf);
		} else if(sfc_index == SFC_1) {
			ret = spi_flash_update(sfc_index,flash1, offset, len, buf);
		}
	} else if (strncmp(argv[0], "read", 4) == 0 ||
			strncmp(argv[0], "write", 5) == 0) {
		int read;

		read = strncmp(argv[0], "read", 4) == 0;
		if (read) {
			if(sfc_index == SFC_0) {
				ret = spi_flash_read(sfc_index,flash0, offset, len, buf);
			} else if(sfc_index == SFC_1) {
				ret = spi_flash_read(sfc_index,flash1, offset, len, buf);
			}
		} else {
			if(sfc_index == SFC_0) {
				ret = spi_flash_write(sfc_index,flash0, offset, len, buf);
			} else if(sfc_index == SFC_1) {
				ret = spi_flash_write(sfc_index,flash1, offset, len, buf);
			}
		}

		printf("SF: %zu bytes @ %#x %s: %s\n", (size_t)len, (u32)offset,
			read ? "Read" : "Written", ret ? "ERROR" : "OK");
	}

	unmap_physmem(buf, len);

	return ret == 0 ? 0 : 1;
}

static int do_spi_flash_erase(int argc, char * const argv[],unsigned char sfc_index)
{
	unsigned long offset;
	unsigned long len;
	char *endp;
	int ret;

	if (argc < 3)
		return -1;

	offset = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;

	ret = sf_parse_len_arg(sfc_index,argv[2], &len);
	if (ret != 1)
		return -1;

	/* Consistency checking */
	if(sfc_index == SFC_0) {
		if (offset + len > flash0->size) {
			printf("ERROR: attempting %s past flash size (%#x)\n",
				argv[0], flash0->size);
			return 1;
		}
	} else if(sfc_index == SFC_1) {
		if (offset + len > flash1->size) {
			printf("ERROR: attempting %s past flash size (%#x)\n",
				argv[0], flash1->size);
			return 1;
		}
	}

	if(sfc_index == SFC_0) {
		ret = spi_flash_erase(sfc_index,flash0, offset, len);
	} else if(sfc_index == SFC_1) {
		ret = spi_flash_erase(sfc_index,flash1, offset, len);
	}
	printf("SF: %zu bytes @ %#x Erased: %s\n", (size_t)len, (u32)offset,
			ret ? "ERROR" : "OK");

	return ret == 0 ? 0 : 1;
}

#ifdef CONFIG_CMD_SF_TEST
enum {
	STAGE_ERASE,
	STAGE_CHECK,
	STAGE_WRITE,
	STAGE_READ,

	STAGE_COUNT,
};

static char *stage_name[STAGE_COUNT] = {
	"erase",
	"check",
	"write",
	"read",
};

struct test_info {
	int stage;
	int bytes;
	unsigned base_ms;
	unsigned time_ms[STAGE_COUNT];
};

static void show_time(struct test_info *test, int stage)
{
	uint64_t speed;	/* KiB/s */
	int bps;	/* Bits per second */

	speed = (long long)test->bytes * 1000;
	do_div(speed, test->time_ms[stage] * 1024);
	bps = speed * 8;

	printf("%d %s: %d ticks, %d KiB/s %d.%03d Mbps\n", stage,
	       stage_name[stage], test->time_ms[stage],
	       (int)speed, bps / 1000, bps % 1000);
}

static void spi_test_next_stage(struct test_info *test)
{
	test->time_ms[test->stage] = get_timer(test->base_ms);
	show_time(test, test->stage);
	test->base_ms = get_timer(0);
	test->stage++;
}

/**
 * Run a test on the SPI flash
 *
 * @param flash		SPI flash to use
 * @param buf		Source buffer for data to write
 * @param len		Size of data to read/write
 * @param offset	Offset within flash to check
 * @param vbuf		Verification buffer
 * @return 0 if ok, -1 on error
 */
static int spi_flash_test(struct spi_flash *flash, uint8_t *buf, ulong len,
			   ulong offset, uint8_t *vbuf)
{
	struct test_info test;
	int i;

	printf("SPI flash test:\n");
	memset(&test, '\0', sizeof(test));
	test.base_ms = get_timer(0);
	test.bytes = len;
	if (spi_flash_erase(sfc_index,flash, offset, len)) {
		printf("Erase failed\n");
		return -1;
	}
	spi_test_next_stage(&test);

	if (spi_flash_read(sfc_index,flash, offset, len, vbuf)) {
		printf("Check read failed\n");
		return -1;
	}
	for (i = 0; i < len; i++) {
		if (vbuf[i] != 0xff) {
			printf("Check failed at %d\n", i);
			print_buffer(i, vbuf + i, 1, min(len - i, 0x40), 0);
			return -1;
		}
	}
	spi_test_next_stage(&test);

	if (spi_flash_write(sfc_index,flash, offset, len, buf)) {
		printf("Write failed\n");
		return -1;
	}
	memset(vbuf, '\0', len);
	spi_test_next_stage(&test);

	if (spi_flash_read(sfc_index,flash, offset, len, vbuf)) {
		printf("Read failed\n");
		return -1;
	}
	spi_test_next_stage(&test);

	for (i = 0; i < len; i++) {
		if (buf[i] != vbuf[i]) {
			printf("Verify failed at %d, good data:\n", i);
			print_buffer(i, buf + i, 1, min(len - i, 0x40), 0);
			printf("Bad data:\n");
			print_buffer(i, vbuf + i, 1, min(len - i, 0x40), 0);
			return -1;
		}
	}
	printf("Test passed\n");
	for (i = 0; i < STAGE_COUNT; i++)
		show_time(&test, i);

	return 0;
}

static int do_spi_flash_test(int argc, char * const argv[],unsigned char sfc_index)
{
	unsigned long offset;
	unsigned long len;
	uint8_t *buf = (uint8_t *)CONFIG_SYS_TEXT_BASE;
	char *endp;
	uint8_t *vbuf;
	int ret;

	offset = simple_strtoul(argv[1], &endp, 16);
	if (*argv[1] == 0 || *endp != 0)
		return -1;
	len = simple_strtoul(argv[2], &endp, 16);
	if (*argv[2] == 0 || *endp != 0)
		return -1;

	vbuf = malloc(len);
	if (!vbuf) {
		printf("Cannot allocate memory\n");
		return 1;
	}
	buf = malloc(len);
	if (!buf) {
		free(vbuf);
		printf("Cannot allocate memory\n");
		return 1;
	}

	memcpy(buf, (char *)CONFIG_SYS_TEXT_BASE, len);
	if(sfc_index == SFC_0) {
		ret = spi_flash_test(flash0, buf, len, offset, vbuf);
	} else if(sfc_index == SFC_1) {
		ret = spi_flash_test(flash1, buf, len, offset, vbuf);
	}
	free(vbuf);
	free(buf);
	if (ret) {
		printf("Test failed\n");
		return 1;
	}

	return 0;
}
#endif /* CONFIG_CMD_SF_TEST */

static int do_spi_flash0(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	int ret;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];
	--argc;
	++argv;

#ifdef PRINT_TIME
	unsigned int start, end;
	start = get_timer(0);
#endif

	if (strcmp(cmd, "probe") == 0) {
		ret = do_spi_flash_probe(argc, argv,SFC_0);
		goto done;
	}

	/* The remaining commands require a selected device */
	if (!flash0) {
		puts("No SPI flash selected. Please run 'sf probe' or 'sf0 probe'\n");
		return 1;
	}

	if (strcmp(cmd, "read") == 0 || strcmp(cmd, "write") == 0 ||
	    strcmp(cmd, "update") == 0)
		ret = do_spi_flash_read_write(argc, argv,SFC_0);
	else if (strcmp(cmd, "erase") == 0)
		ret = do_spi_flash_erase(argc, argv,SFC_0);
#ifdef CONFIG_CMD_SF_TEST
	else if (!strcmp(cmd, "test"))
		ret = do_spi_flash_test(argc, argv,SFC_0);
#endif
	else
		ret = -1;

done:
#ifdef PRINT_TIME
	end = get_timer(0);
	printf("--->%s spend %d ms\n",cmd,end - start);
#endif

	if (ret != -1)
		return ret;
usage:
	return CMD_RET_USAGE;
}

#ifdef CONFIG_SFC1_NOR
static int do_spi_flash1(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	const char *cmd;
	int ret;

	/* need at least two arguments */
	if (argc < 2)
		goto usage;

	cmd = argv[1];
	--argc;
	++argv;

#ifdef PRINT_TIME
	unsigned int start, end;
	start = get_timer(0);
#endif

	if (strcmp(cmd, "probe") == 0) {
		ret = do_spi_flash_probe(argc, argv,SFC_1);
		goto done;
	}

	/* The remaining commands require a selected device */
	if (!flash1) {
		puts("No SPI flash selected. Please run `sf1 probe'\n");
		return 1;
	}

	if (strcmp(cmd, "read") == 0 || strcmp(cmd, "write") == 0 ||
	    strcmp(cmd, "update") == 0)
		ret = do_spi_flash_read_write(argc, argv,SFC_1);
	else if (strcmp(cmd, "erase") == 0)
		ret = do_spi_flash_erase(argc, argv,SFC_1);
#ifdef CONFIG_CMD_SF_TEST
	else if (!strcmp(cmd, "test"))
		ret = do_spi_flash_test(argc, argv,SFC_1);
#endif
	else
		ret = -1;

done:
#ifdef PRINT_TIME
	end = get_timer(0);
	printf("--->%s spend %d ms\n",cmd,end - start);
#endif

	if (ret != -1)
		return ret;
usage:
	return CMD_RET_USAGE;
}
#endif
#ifdef CONFIG_CMD_SF_TEST
#define SF_TEST_HELP "\nsf test offset len		" \
		"- run a very basic destructive test"
#else
#define SF_TEST_HELP
#endif

#ifdef CONFIG_SFC0_NOR
/* Compatible with the SF command used previously, sf == sf0 */
U_BOOT_CMD(
	sf,	5,	1,	do_spi_flash0,
	"SPI flash sub-system",
	"sf probe [[bus:]cs] [hz] [mode]	- init flash device on given SPI bus\n"
	"				  and chip select\n"
	"sf read addr offset len 	- read `len' bytes starting at\n"
	"				  `offset' to memory at `addr'\n"
	"sf write addr offset len	- write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'\n"
	"sf erase offset [+]len		- erase `len' bytes from `offset'\n"
	"				  `+len' round up `len' to block size\n"
	"sf update addr offset len	- erase and write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'"
	SF_TEST_HELP
);

U_BOOT_CMD(
	sf0,	5,	1,	do_spi_flash0,
	"SPI flash sub-system",
	"sf0 probe [[bus:]cs] [hz] [mode]	- init flash device on given SPI bus\n"
	"				  and chip select\n"
	"sf0 read addr offset len 	- read `len' bytes starting at\n"
	"				  `offset' to memory at `addr'\n"
	"sf0 write addr offset len	- write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'\n"
	"sf0 erase offset [+]len		- erase `len' bytes from `offset'\n"
	"				  `+len' round up `len' to block size\n"
	"sf0 update addr offset len	- erase and write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'"
	SF_TEST_HELP
);
#endif
#ifdef CONFIG_SFC1_NOR
U_BOOT_CMD(
	sf1,	5,	1,	do_spi_flash1,
	"SPI flash sub-system",
	"sf1 probe [[bus:]cs] [hz] [mode]	- init flash device on given SPI bus\n"
	"				  and chip select\n"
	"sf1 read addr offset len 	- read `len' bytes starting at\n"
	"				  `offset' to memory at `addr'\n"
	"sf1 write addr offset len	- write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'\n"
	"sf1 erase offset [+]len		- erase `len' bytes from `offset'\n"
	"				  `+len' round up `len' to block size\n"
	"sf1 update addr offset len	- erase and write `len' bytes from memory\n"
	"				  at `addr' to flash at `offset'"
	SF_TEST_HELP
);
#endif
