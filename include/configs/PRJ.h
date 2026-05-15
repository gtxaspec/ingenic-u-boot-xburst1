#ifndef __CONFIG_PRJ_H
#define __CONFIG_PRJ_H

#define INGENIC_UBOOT_VERSIONS "H20251206a"

#define voe() printf("%s %d\n", __FILE__, __LINE__)

/**
 * Basic configuration(SOC, Cache, UART, DDR).
 */
/* #define CONFIG_SPL_ALIOS_BOOT */
#define CFG_CPU_SEL_PLL		APLL
#define CFG_DDR_SEL_PLL		MPLL
#define CFG_MACPHY_SEL_PLL	MPLL

#define  CGU_CLK_SRC {				\
		{LCD, MPLL},			\
		{MSC0, MPLL},			\
		{SFC0, MPLL},			\
		{CIM, MPLL},			\
		{SRC_EOF,SRC_EOF}		\
	}

#define CFG_SYS_SDRAM_BASE              0x80000000 /* cached (KSEG0) address */
#define CFG_SYS_INIT_SP_OFFSET          0x200000
#define CFG_SDRAM_MAX_SIZE              0x10000000
#define CFG_SPL_VERSION 1

#define CONFIG_SYS_SDRAM_BASE           CFG_SYS_SDRAM_BASE
#define CONFIG_SYS_INIT_SP_OFFSET	CFG_SYS_INIT_SP_OFFSET
#define CONFIG_SYS_UBOOT_BASE		0

#if defined(CONFIG_SPL_MMC_SUPPORT)
#define CONFIG_SYS_MONITOR_LEN		(640*1024)
#define CFG_SYS_MMCSD_RAW_MODE_U_BOOT_OFFSET	(CONFIG_SPL_PAD_TO / 1024 + 17)  /* spl_size + gpt_offset */
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR  (CFG_SYS_MMCSD_RAW_MODE_U_BOOT_OFFSET * 2)
#elif defined(CONFIG_SPL_SFC_NOR_SUPPORT)
#define CONFIG_SYS_MONITOR_LEN		(400*1024)
#elif defined(CONFIG_SPL_SFC_NAND_SUPPORT)
#define CONFIG_SYS_MONITOR_LEN		(640*1024)

#define CFG_SFC_QUAD
#define CFG_SPI_NAND_BPP                     (2048 + 64)      /*Bytes Per Page*/
#define CFG_SPI_NAND_PPB                     (64)            /*Page Per Block*/
#define CFG_SYS_NAND_BASE                    0xb3441000
#endif


/**
 * Environment variables configuration (when not using external file)
 */
#ifndef CONFIG_USE_DEFAULT_ENV_FILE

#define BOOTARGS_CONSOLE "console=ttyS1,115200n8"

#if defined(CONFIG_PRJ_N) || defined(CONFIG_PRJ_NQ) || defined(CONFIG_PRJ_ZN) || defined(CONFIG_PRJ_VN) || defined(CONFIG_PRJ_VNP)
	#define BOOTARGS_MEM "mem=64M@0x0 rmem=64M@0x4000000"
#elif defined(CONFIG_PRJ_XQ) || defined(CONFIG_PRJ_ZX)
	#define BOOTARGS_MEM "mem=128M@0x0 rmem=128M@0x8000000"
#elif defined(CONFIG_PRJ_LQ) || defined(CONFIG_PRJ_ZL) || defined(CONFIG_PRJ_VL)
	#define BOOTARGS_MEM "mem=41M@0x0 rmem=23M@0x2900000"
#else
	#define BOOTARGS_MEM "${bootargs_mem}"
#endif

#ifdef CONFIG_SPL_MMC_SUPPORT
	#define BOOTARGS_ROOT "init=/linuxrc root=/dev/mmcblk0p2 rw rootdelay=1"
	#define CONFIG_BOOTCOMMAND "mmc read ${kernel_addr_r} 0x1800 0x3000; bootm ${kernel_addr_r}"
#elif defined(CONFIG_SPL_SFC_NOR_SUPPORT)
	#define BOOTARGS_ROOT "init=/linuxrc rootfstype=squashfs root=/dev/mtdblock2 rw mtdparts=sfc0_nor:512k(boot),2560k(kernel),2048k(root),-(appfs)"
	#define CONFIG_BOOTCOMMAND "sf probe;sf read ${kernel_addr_r} 0x80000 0x280000;bootm ${kernel_addr_r}"
#elif defined(CONFIG_SPL_SFC_NAND_SUPPORT)
	#define BOOTARGS_ROOT "init=/linuxrc ubi.mtd=2 root=ubi0:rootfs rootfstype=ubifs rw mtdparts=sfc0_nand:1M(uboot),3M(kernel),20M(root),-(appfs)"
	/* #define CONFIG_BOOTCOMMAND "nand read ${kernel_addr_r} 0x100000 0x300000;bootm ${kernel_addr_r}" */
	#define CONFIG_BOOTCOMMAND  "mtd read spi-nand0 ${kernel_addr_r} 0x100000 0x300000;bootm ${kernel_addr_r}"
#endif

#ifdef CONFIG_FAST_BOOT
#define BOOTARGS_QUIET "lpj=11968512 quiet"
#else
#define BOOTARGS_QUIET "lpj=11968512"
#endif

#define CONFIG_USE_BOOTARGS
#define CONFIG_BOOTARGS BOOTARGS_CONSOLE " " BOOTARGS_MEM " " BOOTARGS_ROOT " " BOOTARGS_QUIET

/* Network configuration */
#define CONFIG_GATEWAYIP    192.168.2.1
#define CONFIG_IPADDR       192.168.2.121
#define CONFIG_NETMASK      255.255.255.0
#define CONFIG_SERVERIP     10.3.2.4

/* Boot configuration */
#define CONFIG_BOOTDELAY    1

/* Additional environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS \
    "kernel_addr_r=0x80600000\0"  \
    "bootargs_mem=mem=64M@0x0 rmem=64M@0x4000000\0"

#endif /* CONFIG_USE_DEFAULT_ENV_FILE */

#ifndef REG32
#define REG32(addr) *((volatile unsigned int *)(addr))
#endif

#endif /* __CONFIG_ISVP_PRJ_H */
