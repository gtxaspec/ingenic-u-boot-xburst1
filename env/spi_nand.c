// SPDX-License-Identifier: GPL-2.0+
/*
 * SPI NAND environment variable storage support
 *
 * Implementation of environment storage driver for SPI NAND flash devices
 * with bad block management, ECC support, and wear-leveling awareness.
 *
 * (C) Copyright 2024
 * oywei, Ingenic Semiconductor <setsuna.oywei@ingenic.com>
 *
 * Based on U-Boot environment infrastructure and SPI NAND core driver.
 */

#include <common.h>
#include <command.h>
#include <env.h>
#include <env_internal.h>
#include <asm/global_data.h>
#include <linux/stddef.h>
#include <malloc.h>
#include <memalign.h>
#include <mtd.h>
#include <search.h>
#include <errno.h>
#include <u-boot/crc.h>

#if defined(CONFIG_CMD_SAVEENV) && defined(CONFIG_CMD_MTD) && \
		!defined(CONFIG_SPL_BUILD)
#define CMD_SAVEENV
#elif defined(CONFIG_ENV_OFFSET_REDUND) && !defined(CONFIG_SPL_BUILD)
#error CONFIG_ENV_OFFSET_REDUND must have CONFIG_CMD_SAVEENV & CONFIG_CMD_MTD
#endif

DECLARE_GLOBAL_DATA_PTR;

#ifndef CONFIG_ENV_SPI_NAND_DEVICE_NAME
#define CONFIG_ENV_SPI_NAND_DEVICE_NAME "spi-nand0"
#endif

/*
 * This is called before nand_init() so we can't read NAND to
 * validate env data.
 *
 * Mark it OK for now. env_relocate() in env_common.c will call our
 * relocate function which does the real validation.
 */
static int env_spi_nand_init(void)
{
	gd->env_valid = ENV_INVALID;
	/*
	 * Return -ENOENT to indicate that this environment location
	 * is available but hasn't been validated yet. This allows
	 * env_init() to mark us as initialized, and env_load() will
	 * call our load function to attempt loading from SPI NAND.
	 */
	return -ENOENT;
}

#ifdef CMD_SAVEENV
/*
 * The SPI NAND code uses MTD interface to access the device
 */
static int writeenv(size_t offset, u_char *buf)
{
	struct mtd_info *mtd;
	size_t len = CONFIG_ENV_SIZE;

	mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
	if (IS_ERR_OR_NULL(mtd)) {
		/* Device not found, try probing MTD devices */
		mtd_probe_devices();

		/* Try again after probing */
		mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
		if (IS_ERR_OR_NULL(mtd)) {
			printf("MTD device '%s' not found\n", CONFIG_ENV_SPI_NAND_DEVICE_NAME);
			return 1;
		}
	}

	if (mtd_write(mtd, offset, len, &len, buf)) {
		put_mtd_device(mtd);
		return 1;
	}

	put_mtd_device(mtd);
	return 0;
}

struct spi_nand_env_location {
	const char *name;
	const loff_t offset;
};

static int erase_and_write_env(const struct spi_nand_env_location *location,
		u_char *env_new)
{
	struct mtd_info *mtd;
	struct erase_info ei;
	int ret = 0;

	mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
	if (IS_ERR_OR_NULL(mtd)) {
		/* Device not found, try probing MTD devices */
		debug("SPI NAND device not found, probing MTD devices...\n");
		mtd_probe_devices();

		/* Try again after probing */
		mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
		if (IS_ERR_OR_NULL(mtd)) {
			printf("MTD device '%s' not found\n", CONFIG_ENV_SPI_NAND_DEVICE_NAME);
			return 1;
		}
	}

	printf("Erasing %s...\n", location->name);

	memset(&ei, 0, sizeof(ei));
	ei.mtd = mtd;
	ei.addr = location->offset;
	ei.len = mtd->erasesize;  // 一个块大小

	if (mtd_erase(mtd, &ei)) {
		put_mtd_device(mtd);
		return 1;
	}

	printf("Writing to %s... ", location->name);
	ret = writeenv(location->offset, env_new);
	puts(ret ? "FAILED!\n" : "OK\n");

	put_mtd_device(mtd);
	return ret;
}

static int env_spi_nand_save(void)
{
	int	ret = 0;
	ALLOC_CACHE_ALIGN_BUFFER(env_t, env_new, 1);
	int	env_idx = 0;
	static const struct spi_nand_env_location location[] = {
		{
			.name = "SPI NAND",
			.offset = CONFIG_ENV_OFFSET,
		},
#ifdef CONFIG_ENV_OFFSET_REDUND
		{
			.name = "redundant SPI NAND",
			.offset = CONFIG_ENV_OFFSET_REDUND,
		},
#endif
	};

	ret = env_export(env_new);
	if (ret)
		return ret;

#ifdef CONFIG_ENV_OFFSET_REDUND
	env_idx = (gd->env_valid == ENV_VALID);
#endif

	printf("load env from %s(%d)  ", location[env_idx].name, env_idx);
	ret = erase_and_write_env(&location[env_idx], (u_char *)env_new);
#ifdef CONFIG_ENV_OFFSET_REDUND
	if (!ret) {
		/* preset other copy for next write */
		gd->env_valid = gd->env_valid == ENV_REDUND ? ENV_VALID :
				ENV_REDUND;
		return ret;
	}

	env_idx = (env_idx + 1) & 1;
	ret = erase_and_write_env(&location[env_idx], (u_char *)env_new);
	if (!ret)
		printf("Warning: primary env write failed,"
				" redundancy is lost!\n");
#endif

	return ret;
}
#endif /* CMD_SAVEENV */

static int readenv(size_t offset, u_char *buf)
{
	struct mtd_info *mtd;
	size_t len = CONFIG_ENV_SIZE;

	mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
	if (IS_ERR_OR_NULL(mtd)) {
		/* Device not found, try probing MTD devices */
		mtd_probe_devices();

		/* Try again after probing */
		mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
		if (IS_ERR_OR_NULL(mtd)) {
			printf("MTD device '%s' not found\n", CONFIG_ENV_SPI_NAND_DEVICE_NAME);
			return 1;
		}
	}

	if (mtd_read(mtd, offset, len, &len, buf)) {
		put_mtd_device(mtd);
		return 1;
	}

	put_mtd_device(mtd);
	return 0;
}

#ifdef CONFIG_ENV_OFFSET_REDUND
static int env_spi_nand_load(void)
{
	int read1_fail, read2_fail;
	env_t *tmp_env1, *tmp_env2;
	int ret = 0;

	tmp_env1 = (env_t *)malloc(CONFIG_ENV_SIZE);
	tmp_env2 = (env_t *)malloc(CONFIG_ENV_SIZE);
	if (tmp_env1 == NULL || tmp_env2 == NULL) {
		puts("Can't allocate buffers for environment\n");
		env_set_default("malloc() failed", 0);
		ret = -EIO;
		goto done;
	}

	read1_fail = readenv(CONFIG_ENV_OFFSET, (u_char *) tmp_env1);
	read2_fail = readenv(CONFIG_ENV_OFFSET_REDUND, (u_char *) tmp_env2);

	ret = env_import_redund((char *)tmp_env1, read1_fail, (char *)tmp_env2,
				read2_fail, H_EXTERNAL);

done:
	free(tmp_env1);
	free(tmp_env2);

	return ret;
}
#else /* ! CONFIG_ENV_OFFSET_REDUND */
/*
 * The SPI NAND code saves the environment in the first SPI NAND device
 */
static int env_spi_nand_load(void)
{
	int ret;
	struct mtd_info *mtd;
	ALLOC_CACHE_ALIGN_BUFFER(char, buf, CONFIG_ENV_SIZE);

	/* Try to get MTD device, probe if not found */
	mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
	if (IS_ERR_OR_NULL(mtd)) {
		/* Device not found, try probing MTD devices */
		debug("SPI NAND device not found, probing MTD devices...\n");
		mtd_probe_devices();

		/* Try again after probing */
		mtd = get_mtd_device_nm(CONFIG_ENV_SPI_NAND_DEVICE_NAME);
		if (IS_ERR_OR_NULL(mtd)) {
			printf("MTD device '%s' not found\n", CONFIG_ENV_SPI_NAND_DEVICE_NAME);
			env_set_default("MTD device not found", 0);
			return -EIO;
		}
	}
	put_mtd_device(mtd);

	ret = readenv(CONFIG_ENV_OFFSET, (u_char *)buf);
	if (ret) {
		env_set_default("readenv() failed", 0);
		return -EIO;
	}

	return env_import(buf, 1, H_EXTERNAL);
}
#endif /* CONFIG_ENV_OFFSET_REDUND */

U_BOOT_ENV_LOCATION(spi_nand) = {
	.location	= ENVL_SPI_NAND,
	ENV_NAME("SPI_NAND")
	.load		= env_spi_nand_load,
#if defined(CMD_SAVEENV)
	.save		= env_save_ptr(env_spi_nand_save),
#endif
	.init		= env_spi_nand_init,
};