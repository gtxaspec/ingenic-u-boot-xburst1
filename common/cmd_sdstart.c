#include <common.h>
#include <command.h>
#include <fdtdec.h>
#include <malloc.h>
#include <menu.h>
#include <post.h>
#include <version.h>
#include <watchdog.h>
#include <linux/ctype.h>
#include <environment.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <spi_flash.h>
#include <linux/mtd/mtd.h>
#include <fat.h>
#include <mmc.h>

#ifdef CONFIG_DDR2_128M
#define CONFIG_BOOTARGS2 "console=ttyS1,115200n8 mem=64M@0x0 rmem=64M@0x4000000 root=/dev/ram0 rw rdinit=/linuxrc"
#define CONFIG_BOOTCMD2 "bootm 0x84000000"
#define LOAD_ADDR ((unsigned char *)0x84000000)
#else
#define CONFIG_BOOTARGS2 "console=ttyS1,115200n8 mem=48M@0x0 rmem=16M@0x3000000 root=/dev/ram0 rw rdinit=/linuxrc"
#define CONFIG_BOOTCMD2 "bootm 0x83000000"
#define LOAD_ADDR ((unsigned char *)0x83000000)
#endif

#define MAX_LOADSZ 0x500000
#define DEFAULT_DELAY 1 // 1 second by default

long sz = 0;
char *aufiles = "factory_0P3N1PC_kernel";

static void prompt_and_wait_for_interrupt(int delay) {
    printf("Press Ctrl-C to interrupt kernel loading within %d second(s)...\n", delay);
    int start = get_timer(0);
    while (get_timer(start) < (delay * 1000)) {
        if (ctrlc()) {
            printf("Operation interrupted by user!\n");
            return;
        }
        udelay(10000); // Check every 10ms
    }
}

static int au_check_cksum_valid(long nbytes) {
    image_header_t *hdr;
    unsigned long checksum;

    hdr = (image_header_t *)LOAD_ADDR;
    if (nbytes != (sizeof(*hdr) + ntohl(hdr->ih_size))) {
        printf("Image %s bad total SIZE\n", aufiles);
        return -1;
    }
    checksum = ntohl(hdr->ih_dcrc);
    if (crc32(0, (unsigned char const *)(LOAD_ADDR + sizeof(*hdr)), ntohl(hdr->ih_size)) != checksum) {
        printf("Image %s bad data checksum\n", aufiles);
        return -1;
    }
    return 0;
}

static int au_check_header_valid(long nbytes) {
    image_header_t *hdr;
    unsigned long checksum;
    hdr = (image_header_t *)LOAD_ADDR;

    if (nbytes < sizeof(*hdr)) {
        printf("Image %s bad header SIZE\n", aufiles);
        return -1;
    }
    if (ntohl(hdr->ih_magic) != IH_MAGIC || hdr->ih_arch != IH_ARCH_MIPS) {
        printf("Image %s bad MAGIC or ARCH\n", aufiles);
        return -1;
    }
    checksum = ntohl(hdr->ih_hcrc);
    hdr->ih_hcrc = 0;
    if (crc32(0, (unsigned char const *)hdr, sizeof(*hdr)) != checksum) {
        printf("Image %s bad header checksum\n", aufiles);
        return -1;
    }
    if (hdr->ih_type != IH_TYPE_KERNEL) {
        printf("Image %s wrong type\n", aufiles);
        return -1;
    }
    return 0;
}

static int update_to_flash(void) {
    sz = file_fat_read(aufiles, LOAD_ADDR, sizeof(image_header_t));
    if (sz <= 0) {
        printf("%s not found\n", aufiles);
        return -1;
    }
    if (au_check_header_valid(sz) < 0) {
        printf("%s header not valid\n", aufiles);
        return -1;
    }
    sz = file_fat_read(aufiles, LOAD_ADDR, MAX_LOADSZ);
    if (sz <= 0) {
        printf("%s not found\n", aufiles);
        return -1;
    }
    if (au_check_cksum_valid(sz) < 0) {
        printf("%s checksum not valid\n", aufiles);
        return -1;
    }
    return 0;
}

static int do_check_sd(void) {
    int ret = 0;
    block_dev_desc_t *stor_dev;

    stor_dev = get_dev("mmc", 0);
    if(NULL == stor_dev) {
        printf("MMC card is not present\n");
        return -1;
    }
    ret = fat_register_device(stor_dev, 1);
    if(ret != 0) {
        printf("FAT device registration failed\n");
        return -1;
    }
    ret = file_fat_detectfs();
    if (ret != 0) {
        printf("FAT filesystem detection failed\n");
        return -1;
    }
    printf("FAT filesystem detected\n");
    return 0;
}

static void setenv_sd_start(void) {
    char bootargs[512];
    const char* osmem = getenv("osmem");
    const char* rmem = getenv("rmem");
    const char* mtdparts = getenv("mtdparts");
    const char* extras = getenv("extras");

    snprintf(bootargs, sizeof(bootargs), "mem=%s rmem=%s console=ttyS1,115200n8 panic=20 root=/dev/mtdblock3 rootfstype=squashfs init=/init mtdparts=%s %s",
             osmem ? osmem : "default_value",
             rmem ? rmem : "default_value",
             mtdparts ? mtdparts : "default_value",
             extras ? extras : "");

    setenv("bootargs", bootargs);
    setenv("bootcmd", CONFIG_BOOTCMD2);
    run_command("boot", 0);
}

int do_sd_start(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[]) {
    int old_ctrlc = 0;
    int delay = DEFAULT_DELAY;

    if (argc > 1) {
        delay = simple_strtol(argv[1], NULL, 10);
    }
    prompt_and_wait_for_interrupt(delay);
    if (ctrlc()) {
        printf("Operation was canceled during the prompt.\n");
        return 0;
    }
    if (0 == do_check_sd()) {
        old_ctrlc = disable_ctrlc(0);
        if(0 == update_to_flash()) {
            setenv_sd_start();
            return 0;
        }
        disable_ctrlc(old_ctrlc);
    }
    return 0;
}

U_BOOT_CMD(
    sdstart,    2,    0,    do_sd_start,
    "load a kernel from the mmc card with an interruptible delay",
    "[delay_in_seconds]"
);
