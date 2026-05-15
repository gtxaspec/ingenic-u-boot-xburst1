// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <config.h>
#include <common.h>
#include <serial.h>
#include <linux/compiler.h>

#include <asm/io.h>
#include <asm/arch/uart.h>
#include <asm/arch/base.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * serial_init - initialize a channel
 *
 * This routine initializes the number of data bits, parity
 * and set the selected baud rate. Interrupts are disabled.
 * Set the modem control signals if the option is selected.
 *
 * RETURNS: N/A
 */

struct jz_uart *uart __attribute__ ((section(".data")));

static struct baudtoregs_t
{
	unsigned int baud;
	unsigned short div;
	unsigned int umr:5;
	unsigned int uacr:12;
} baudtoregs[] = {
#ifdef CONFIG_SMALL_BAUDRATE_TABLE
	{9600,0x9c,0x10,0x0},
	{115200,0xd,0x10,0x0},
	{2000000,0x1,0xc,0x0},
	{3000000,0x1,0x8,0x0},
#else
	{50,0x7530,0x10,0x0},
	{75,0x4e20,0x10,0x0},
	{110,0x3521,0x10,0x0},
	{134,0x2b9d,0x10,0x0},
	{150,0x2710,0x10,0x0},
	{200,0x1d4c,0x10,0x0},
	{300,0x1388,0x10,0x0},
	{600,0x9c4,0x10,0x0},
	{1200,0x4e2,0x10,0x0},
	{1800,0x340,0x10,0x0},
	{2400,0x271,0x10,0x0},
	{4800,0x138,0x10,0x0},
	{9600,0x9c,0x10,0x0},
	{19200,0x4e,0x10,0x0},
	{38400,0x27,0x10,0x0},
	{57600,0x1a,0x10,0x0},
	{115200,0xd,0x10,0x0},
	{230400,0x6,0x11,0x252},
	{460800,0x3,0x11,0x252},
	{500000,0x3,0x10,0x0},
	{576000,0x3,0xd,0xfef},
	{921600,0x2,0xd,0x0},
	{1000000,0x2,0xc,0x0},
	{1152000,0x1,0x14,0xefb},
	{1500000,0x1,0x10,0x0},
	{2000000,0x1,0xc,0x0},
	{2500000,0x1,0x9,0x6b5},
	{3000000,0x1,0x8,0x0},
	{3500000,0x1,0x6,0xbf7},
	{4000000,0x1,0x6,0x0},
#endif
};

static int jz_serial_init(void)
{
#ifdef CONFIG_BURNER
	uart = (struct jz_uart *)(UART0_BASE + gd->arch.gi->uart_idx * 0x1000);
#else
	uart = (struct jz_uart *)(UART0_BASE + CONFIG_SYS_UART_INDEX * 0x1000);
#endif

	/* Disable port interrupts while changing hardware */
	writeb(0, &uart->dlhr_ier);

	/* Disable UART unit function */
	writeb(~UART_FCR_UUE, &uart->iir_fcr);

	/* Set both receiver and transmitter in UART mode (not SIR) */
	writeb(~(SIRCR_RSIRE | SIRCR_TSIRE), &uart->isr);

	/*
	 * Set databits, stopbits and parity.
	 * (8-bit data, 1 stopbit, no parity)
	 */
	writeb(UART_LCR_WLEN_8 | UART_LCR_STOP_1, &uart->lcr);

	/* Set baud rate */
	serial_setbrg();

	/* Enable UART unit, enable and clear FIFO */
	writeb(UART_FCR_UUE | UART_FCR_FE | UART_FCR_TFLS | UART_FCR_RFLS,
	       &uart->iir_fcr);

	/*
	 * write tag for kernel uart detect
	 */
	writeb(0xa9, &uart->spr);

	return 0;
}

static void jz_serial_setbrg(void)
{
	u32 tmp, i;
	u32 umr = 0x10;
	u32 uacr = 0x0;
	u32 baud_div = 0xd;

#ifdef CONFIG_BURNER
	tmp = gd->arch.gi->baud_rate;
#else
	tmp = CONFIG_BAUDRATE;
#endif

	for(i = 0; i < ARRAY_SIZE(baudtoregs); i++) {
		if(tmp == baudtoregs[i].baud) {
			umr = baudtoregs[i].umr;
			uacr = baudtoregs[i].uacr;
			baud_div = baudtoregs[i].div;
		}
	}

#ifdef CONFIG_PALLADIUM
	writel(32,0xb0030024);
	writel(0,0xb0030028);
	baud_div = 1;
#endif
	tmp = readb(&uart->lcr);
	tmp |= UART_LCR_DLAB;
	writeb(tmp, &uart->lcr);

	writeb((baud_div >> 8) & 0xff, &uart->dlhr_ier);
	writeb(baud_div & 0xff, &uart->rbr_thr_dllr);

	tmp &= ~UART_LCR_DLAB;
	writeb(tmp, &uart->lcr);

	writeb(umr, &uart->umr);
	writew(uacr, &uart->uacr);
}

static int jz_serial_tstc(void)
{
	if (readb(&uart->lsr) & UART_LSR_DR)
		return 1;

	return 0;
}

static void jz_serial_putc(const char c)
{
	if (c == '\n')
		serial_putc('\r');

	writeb((u8)c, &uart->rbr_thr_dllr);

	/* Wait for fifo to shift out some bytes */
	while (!((readb(&uart->lsr) & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60))
		;
}


void jz_serial_puts (const char *s)
{
        while (*s) {
                jz_serial_putc(*s++);
        }
}


void serial_put_hex(unsigned int  d)
{
        char c[12];
        unsigned char i;
        for(i = 0; i < 8; i++)
        {
                c[i] = (d >> ((7 - i) * 4)) & 0xf;
                if(c[i] < 10)
                        c[i] += 0x30;
                else
                        c[i] += (0x41 - 10);
        }

	c[8] = 0;
        jz_serial_puts(c);
}



static int jz_serial_getc(void)
{
	while (!serial_tstc())
		;

	return readb(&uart->rbr_thr_dllr);
}

static struct serial_device jz_serial_drv = {
	.name	= "jz_serial",
	.start	= jz_serial_init,
	.stop	= NULL,
	.setbrg	= jz_serial_setbrg,
	.putc	= jz_serial_putc,
	.puts	= default_serial_puts,
	.getc	= jz_serial_getc,
	.tstc	= jz_serial_tstc,
};

void jz_serial_initialize(void)
{
	serial_register(&jz_serial_drv);
}

__weak struct serial_device *default_serial_console(void)
{
	return &jz_serial_drv;
}
