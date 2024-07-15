// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2007 Semihalf
 *
 * Written by: Rafal Jaworowski <raj@semihalf.com>
 */

#include <config.h>
#include <common.h>
#include <net.h>
#include <linux/types.h>
#include <api_public.h>

DECLARE_GLOBAL_DATA_PTR;

#define DEBUG
#undef DEBUG

#ifdef DEBUG
#define debugf(fmt, args...) do { printf("%s(): ", __func__); printf(fmt, ##args); } while (0)
#else
#define debugf(fmt, args...)
#endif

#define errf(fmt, args...) do { printf("ERROR @ %s(): ", __func__); printf(fmt, ##args); } while (0)


static int dev_valid_net(void *cookie)
{
	return ((void *)eth_get_dev() == cookie) ? 1 : 0;
}

int dev_open_net(void *cookie)
{
	if (!dev_valid_net(cookie))
		return API_ENODEV;

	if (eth_init(gd->bd) < 0)
		return API_EIO;

	return 0;
}

int dev_close_net(void *cookie)
{
	if (!dev_valid_net(cookie))
		return API_ENODEV;

	eth_halt();
	return 0;
}

/*
 * There can only be one active eth interface at a time - use what is
 * currently set to eth_current
 */
int dev_enum_net(struct device_info *di)
{
	struct eth_device *eth_current = eth_get_dev();

	di->type = DEV_TYP_NET;
	di->cookie = (void *)eth_current;
	if (di->cookie == NULL)
		return 0;

	memcpy(di->di_net.hwaddr, eth_current->enetaddr, 6);

	debugf("device found, returning cookie 0x%08x\n",
		(u_int32_t)di->cookie);

	return 1;
}

int dev_write_net(void *cookie, void *buf, int len)
{
	/* XXX verify that cookie points to a valid net device??? */

	return eth_send(buf, len);
}

int dev_read_net(void *cookie, void *buf, int len)
{
	/* XXX verify that cookie points to a valid net device??? */

	return eth_receive(buf, len);
}
