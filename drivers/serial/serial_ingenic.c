// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#include <common.h>
#include <clk.h>
#include <debug_uart.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <log.h>
#include <watchdog.h>
#include <asm/global_data.h>
#include <asm/io.h>
#include <linux/compiler.h>
#include <serial.h>
#include <linux/err.h>

#include <asm/io.h>
#include <asm/arch/uart.h>
#include <asm/arch/base.h>

DECLARE_GLOBAL_DATA_PTR;

struct ingenic_serial_plat {
	unsigned long clock;
	struct jz_uart *uart;
};

#ifdef CONFIG_DM_SERIAL
int _ingenic_serial_putc(struct jz_uart *uart, const char ch)
{
	if (ch == '\n')
		_ingenic_serial_putc(uart, '\r');

	writeb((u8)ch, &uart->rbr_thr_dllr);

	/* Wait for fifo to shift out some bytes */
	while (!((readb(&uart->lsr) & (UART_LSR_TDRQ | UART_LSR_TEMT)) == 0x60))
		;

	return 0;
}

int ingenic_serial_setbrg(struct udevice *dev, int baudrate)
{
	printascii("ingenic_serial_setbrg\n");
	return 0;
}

int ingenic_serial_probe(struct udevice *dev)
{
	return 0;
}

int ingenic_serial_getc(struct udevice *dev)
{
	return 0;
}

int ingenic_serial_putc(struct udevice *dev, const char ch)
{
	struct ingenic_serial_plat *plat = dev_get_plat(dev);

	_ingenic_serial_putc(plat->uart, ch);

	return 0;
}

int ingenic_serial_pending(struct udevice *dev, bool input)
{
	return 0;
}

static const struct dm_serial_ops ingenic_serial_ops = {
	.putc = ingenic_serial_putc,
	.getc = ingenic_serial_getc,
	.pending = ingenic_serial_pending,
	.setbrg = ingenic_serial_setbrg,
};

#if CONFIG_IS_ENABLED(OF_CONTROL)
static const struct udevice_id ingenic_serial_id[] = {
	{.compatible = "ingenic,uart"},
	{}
};


int ingenic_serial_of_to_plat(struct udevice *dev)
{
	return 0;
}
#endif

U_BOOT_DRIVER(serial_ingenic) = {
	.name	= "serial_ingenic",
	.id	= UCLASS_SERIAL,
	.of_match = of_match_ptr(ingenic_serial_id),
	.of_to_plat = of_match_ptr(ingenic_serial_of_to_plat),
	.plat_auto	= sizeof(struct ingenic_serial_plat),
	.probe = ingenic_serial_probe,
	.ops	= &ingenic_serial_ops,
	.priv_auto	= sizeof(struct jz_uart),
};

#endif
