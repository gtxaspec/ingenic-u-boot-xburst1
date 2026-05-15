/*
 * T21  cpm definitions
 *
 * Copyright (c) 2017 Ingenic Semiconductor Co.,Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CPM_H__
#define __CPM_H__
#include <asm/arch/base.h>
/**********CGU registers Configuration*****/
#define CPM_CPCCR       (0x00)
#define CPM_CPPCR       (0x0c)
#define CPM_CPAPCR      (0x10)
#define CPM_CPMPCR      (0x14)
#define CPM_CPVPCR      (0xe0)
#define CPM_CPAPACR     (0x18)
#define CPM_CPMPACR     (0x1c)
#define CPM_CPVPACR     (0xe4)

#define CPM_DDRCDR      (0x2c)
#define CPM_HELIXCDR    (0x30) // VPU

#define CPM_CPSPR       (0x34)
#define CPM_CPSPPR      (0x38)

#define CPM_USBPCR      (0x3c)
#define CPM_USBRDT      (0x40)
#define CPM_USBVBFIL    (0x44)
#define CPM_USBPCR1     (0x48)

#define CPM_RSACDR      (0x4c)
#define CPM_MACCDR      (0x54)
#define CPM_MACPHYC     (0xe8)
#define CPM_SFC0CDR     (0x58)
#define CPM_SFC1CDR     (0xac)
#define CPM_PWMCDR      (0x5c)
#define CPM_SSICDR      (0x74)
#define CPM_SLVCDR      (0x60)
#define CPM_LPCDR       (0x64)
#define CPM_MSC0CDR     (0x68)
#define CPM_MSC1CDR     (0xa4)
#define CPM_LDCCDR      (0x6c)
#define CPM_I2STCDR     (0x70)
#define CPM_I2STCDR1    (0x78)
#define CPM_I2SRCDR     (0x84)
#define CPM_I2SRCDR1    (0x88)
#define CPM_CIMCDR      (0x7c)
#define CPM_ISPMCDR     (0x80)
#define CPM_ISPSCDR     (0x90)
#define CPM_ISPACDR     (0x94)

#define CPM_OSC_CTRL    (0x8c)
#define CPM_INTR        (0xb0)
#define CPM_INTRE       (0xb4)
#define CPM_DRCG        (0xd0)
#define CPM_CPCSR       (0xd4)

#define CPM_FRAM_CTRL   (0xa0)


/************power/reset management*********/
#define CPM_LCR         (0x04)
#define CPM_CLKGR0      (0x20)
#define CPM_CLKGR1      (0x28)
#define CPM_TNPUCDR     (0x98)
#define CPM_SRBC0       (0xc4)
// #define CPM_SRBC1   (0xAC)
#define CPM_MESTSEL     (0xEC)
#define CPM_OPCR        (0x24)

#define MEMSCR0         (0xF0)
#define MEMSCR1         (0xF4)
#define MEMPDR0         (0xF8)
/******************************************/

/**********CLKGR0 0x20**************/
#define CPM_CLKGR_LDC           (1 << 31)
#define CPM_CLKGR_IPU           (1 << 30)
#define CPM_CLKGR_DTRNG         (1 << 29)
#define CPM_CLKGR_VPU           (1 << 28)
#define CPM_CLKGR_DDR           (1 << 27)
#define CPM_CLKGR_TCU           (1 << 26)
#define CPM_CLKGR_RTC           (1 << 25)
#define CPM_CLKGR_DES           (1 << 24)
#define CPM_CLKGR_RSA           (1 << 23)
#define CPM_CLKGR_RISCV         (1 << 22)
#define CPM_CLKGR_MIPI_CSI      (1 << 21)
#define CPM_CLKGR_LCD           (1 << 20)
#define CPM_CLKGR_ISP           (1 << 19)
#define CPM_CLKGR_PDMA          (1 << 18)
#define CPM_CLKGR_SFC           (1 << 17)
#define CPM_CLKGR_SFC0          (1 << 17)
#define CPM_CLKGR_BUS_MONITOR   (1 << 16)
#define CPM_CLKGR_SC_HASH       (1 << 15)
#define CPM_CLKGR_SMB2          (1 << 14)
#define CPM_CLKGR_UART2         (1 << 13)
#define CPM_CLKGR_UART1         (1 << 12)
#define CPM_CLKGR_UART0         (1 << 11)
#define CPM_CLKGR_SADC          (1 << 10)
#define CPM_CLKGR_DMIC          (1 << 9)
#define CPM_CLKGR_AIC           (1 << 8)
#define CPM_CLKGR_I2C1          (1 << 7)
#define CPM_CLKGR_I2C0          (1 << 6)
#define CPM_CLKGR_SSI0          (1 << 5)
#define CPM_CLKGR_MSC1          (1 << 4)
#define CPM_CLKGR_MSC0          (1 << 3)
#define CPM_CLKGR_OTG           (1 << 2)
#define CPM_CLKGR_EFUSE         (1 << 1)
#define CPM_CLKGR_NEMC          (1 << 0)


#define CPM_CLKGR1_LZMA1        (1 << 15)
#define CPM_CLKGR1_SLV          (1 << 14)
#define CPM_CLKGR1_SFC1         (1 << 13)
#define CPM_CLKGR1_CPU          (1 << 12)
#define CPM_CLKGR1_TNPU         (1 << 11)
#define CPM_CLKGR1_APB0         (1 << 10)
#define CPM_CLKGR1_PWM          (1 << 9)
#define CPM_CLKGR1_I2D          (1 << 8)
#define CPM_CLKGR1_SYS_OST      (1 << 7)
#define CPM_CLKGR1_AHB0         (1 << 6)
#define CPM_CLKGR1_ODVP         (1 << 5)
#define CPM_CLKGR1_LZMA0        (1 << 4)
#define CPM_CLKGR1_IVDC         (1 << 3)
#define CPM_CLKGR1_AHB1         (1 << 2)
#define CPM_CLKGR1_AES          (1 << 1)
#define CPM_CLKGR1_GMAC         (1 << 0)

#define CPM_RSR_HR              (1 << 3)
#define CPM_RSR_P0R             (1 << 2)
#define CPM_RSR_WR              (1 << 1)
#define CPM_RSR_PR              (1 << 0)

#define OPCR_ERCS               (1 << 2)
#define OPCR_PD                 (1 << 3)
#define OPCR_IDLE               (1 << 31)

#define cpm_readl(off)          readl(CPM_BASE + (off))
#define cpm_writel(val,off)     writel(val, CPM_BASE + (off))
#define cpm_inl(off)            readl(CPM_BASE + (off))
#define cpm_outl(val,off)       writel(val, CPM_BASE + (off))
#define cpm_test_bit(bit,off)   (cpm_inl(off) & 0x1<<(bit))
#define cpm_set_bit(bit,off)    (cpm_outl((cpm_inl(off) | 0x1<<(bit)),off))
#define cpm_clear_bit(bit,off)  (cpm_outl(cpm_inl(off) & ~(0x1 << bit), off))

/*USBCDR*/
#define USBCDR_UCS_PLL      (1 << 31)
#define USBCDR_UPCS_MPLL    (1 << 30)
#define USBCDR_CE_USB       (1 << 29)
#define USBCDR_USB_BUSY     (1 << 28)
#define USBCDR_USB_STOP     (1 << 27)
#define USBCDR_USB_DIS      (1 << 26)
#define USBCDR_MIPI_CS      (1 << 25)
#define USBCDR_USBCDR_MSK   (0xff)

/*USBPCR*/
#define USBPCR_USB_MODE_ORG     (1 << 31)
#define USBPCR_VBUSVLDEXT       (1 << 24)
#define USBPCR_VBUSVLDEXTSEL    (1 << 23)
#define USBPCR_POR              (1 << 22)
#define USBPCR_OTG_DISABLE      (1 << 20)

/*USBPCR1*/
#define USBPCR1_REFCLKSEL_BIT   (26)
#define USBPCR1_REFCLKSEL_MSK   (0x3 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKSEL_CORE  (0x2 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKSEL_EXT   (0x1 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKSEL_CSL   (0x0 << USBPCR1_REFCLKSEL_BIT)
#define USBPCR1_REFCLKDIV_BIT   (24)
#define USBPCR1_REFCLKDIV_MSK   (0X3 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_19_2M (0x3 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_48M   (0x2 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_24M   (0x1 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_REFCLKDIV_12M   (0x0 << USBPCR1_REFCLKDIV_BIT)
#define USBPCR1_WORD_IF0_16_30  (1 << 19)

/*OPCR*/
#define OPCR_SPENDN0            (1 << 7)

/* CPM scratch pad protected register(CPSPPR) */
#define CPSPPR_CPSPR_WRITABLE   (0x00005a5a)
#define RECOVERY_SIGNATURE      (0x1a1a)        /* means "RECY" */
#define RECOVERY_SIGNATURE_SEC  0x800           /* means "RECY" */
#define FASTBOOT_SIGNATURE      (0x0666)        /* means "FASTBOOT" */

#define cpm_get_scrpad()        readl(CPM_BASE + CPM_CPSPR)
#define cpm_set_scrpad(data)                    \
do {                                            \
	volatile int i = 0x3fff;                    \
	writel(0x5a5a,CPM_BASE + CPM_CPSPPR);       \
	while(i--);                                 \
	writel(data,CPM_BASE + CPM_CPSPR);          \
	writel(0xa5a5,CPM_BASE + CPM_CPSPPR);       \
} while (0)

#endif /* __CPM_H__ */
