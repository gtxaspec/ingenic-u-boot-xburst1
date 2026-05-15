/*
 * =====================================================================================
 *
 *       Filename:  LPDDR3_W63AH6N2B_BJ.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  2020年09月21日 17时55分13秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef __LPDDR3_W63AH6N2B_BJ_H
#define __LPDDR3_W63AH6N2B_BJ_H

/*
 * CL:5, CWL:5  300M ~ 330M
 * CL:6, CWL:5	300M ~ 400M
 * CL:7, CWL:6
 * CL:8, CWL:6	400M ~ 533M
 * CL:9, CWL:7
 * CL:10, CWL:7 533M ~ 666M
 * CL:11, CWL:8
 * CL:13, CWL:9 800M ~ 933M
 * CL:14, CWL:10
 *
 * */
#ifndef CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ
#define CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ (CONFIG_SYS_MEM_FREQ * 2)
#endif

#if (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 400000000)
#define CONFIG_DDR_RL 6
#define CONFIG_DDR_WL 3
#elif ((CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ > 400000000) && (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 533000000))
#define CONFIG_DDR_RL 8
#define CONFIG_DDR_WL 4
#elif ((CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ > 533000000) && (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 600000000))
#define CONFIG_DDR_RL 9
#define CONFIG_DDR_WL 5
#elif ((CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ > 600000000) && (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 667000000))
#define CONFIG_DDR_RL 10
#define CONFIG_DDR_WL 6
#elif ((CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ > 667000000) && (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 733000000))
#define CONFIG_DDR_RL 11
#define CONFIG_DDR_WL 6
#elif ((CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ > 733000000) && (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 800000000))
#define CONFIG_DDR_RL 12
#define CONFIG_DDR_WL 6
#elif ((CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ > 800000000) && (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 933000000))
#define CONFIG_DDR_RL 14
#define CONFIG_DDR_WL 8
#elif ((CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ > 933000000) && (CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ <= 1066000000))
#define CONFIG_DDR_RL 16
#define CONFIG_DDR_WL 8
#else
#define CONFIG_DDR_RL 16
#define CONFIG_DDR_WL 8
#endif

static inline void LPDDR3_W63AH6N2B_BJ_INIT(void *data)
{
	struct ddr_chip_info *c = (struct ddr_chip_info *)data;

	c->DDR_ROW          = 13,
	c->DDR_COL          = 10,
	c->DDR_BANK         = 8,
	c->DDR_BIT          = 16,        /* The bit width of DDR. */

	c->DDR_BL           = 8,

	c->DDR_AL           = 0,

	c->DDR_tRFC         = DDR__ns(130);
	c->DDR_tREFI        = DDR__ns(7800);

	c->DDR_tRAS_min     = DDR_SELECT_MAX__tCK_ps(3, 42000),
	c->DDR_tRAS_max     = DDR_SELECT_MIN__tCK_ps(9 * c->DDR_tREFI, 70200000),

	c->DDR_tRP          = DDR_SELECT_MAX__tCK_ps(3, 21000),
	c->DDR_tRC          = c->DDR_tRAS_min + c->DDR_tRP,

	c->DDR_RL           = CONFIG_DDR_RL,
	c->DDR_WL           = CONFIG_DDR_WL,

	c->DDR_tRTP         = DDR_SELECT_MAX__tCK_ps(4, 7500);

	c->DDR_tRCD         = DDR_SELECT_MAX__tCK_ps(3, 18000);

	c->DDR_tRRD         = DDR_SELECT_MAX__tCK_ps(2, 10000);
	c->DDR_tWR          = DDR_SELECT_MAX__tCK_ps(4, 15000);
	c->DDR_tMRD         = DDR_SELECT_MAX__tCK_ps(10, 14000);
	c->DDR_tWTR         = DDR_SELECT_MAX__tCK_ps(4, 7500);
	c->DDR_tCCD         = DDR__tck(4);
	c->DDR_tFAW         = DDR_SELECT_MAX__tCK_ps(8, 50000),
	c->DDR_tMRW         = DDR__tck(10);

	c->DDR_tDQSCK_min   = DDR__ps(2500);
	c->DDR_tDQSCK_max   = DDR__ps(5500);

	c->DDR_tCKE         = DDR_SELECT_MAX__tCK_ps(3, 7500);
	c->DDR_tCKESR       = DDR_SELECT_MAX__tCK_ps(3, 15000);
	c->DDR_tXP          = DDR_SELECT_MAX__tCK_ps(3, 7500);
	c->DDR_tXSR         = DDR_SELECT_MAX__tCK_ps(2, (c->DDR_tRFC + 10 * 1000));
}

#define LPDDR3_W63AH6N2B_BJ                          \
	{                                           \
		.name = "W63AH6N2B_BJ",                    \
		.id   = DDR_CHIP_ID(TYPE_LPDDR3),         \
		.type = LPDDR3,                           \
		.freq = CONFIG_LPDDR3_W63AH6N2B_BJ_MEM_FREQ, \
		.size = 128,                            \
		.init = LPDDR3_W63AH6N2B_BJ_INIT,            \
	}

#endif
