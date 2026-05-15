/*
 * =====================================================================================
 *
 *       Filename:  DDR2_M14D5121632A.h
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

#ifndef __DDR2_M14D5121632A_H__
#define __DDR2_M14D5121632A_H__


#ifndef CONFIG_DDR2_M14D5121632A_MEM_FREQ
#define CONFIG_DDR2_M14D5121632A_MEM_FREQ (CONFIG_SYS_MEM_FREQ * 2)
#endif

#if ((CONFIG_DDR2_M14D5121632A_MEM_FREQ >= 133000000) && (CONFIG_DDR2_M14D5121632A_MEM_FREQ < 400000000))
#define CONFIG_DDR_CL 5 // or 6
#elif ((CONFIG_DDR2_M14D5121632A_MEM_FREQ >= 400000000) && (CONFIG_DDR2_M14D5121632A_MEM_FREQ <= 533000000))
#define CONFIG_DDR_CL 7
#elif ((CONFIG_DDR2_M14D5121632A_MEM_FREQ > 533000000) && (CONFIG_DDR2_M14D5121632A_MEM_FREQ <= 667000000))
#define CONFIG_DDR_CL 8 // or 7
#else
#define CONFIG_DDR_CL 9
#endif


static inline void DDR2_M14D5121632A_init(void *data)
{
	struct ddr_chip_info *c = (struct ddr_chip_info *)data;

	c->DDR_ROW          = 13,
	c->DDR_COL          = 10,
	c->DDR_BANK         = 4,
	c->DDR_BIT          = 16,        /* The bit width of DDR. */

	c->DDR_BL           = 8,
	c->DDR_AL           = 0,
	c->DDR_CL           = CONFIG_DDR_CL,
	c->DDR_RL           = CONFIG_DDR_CL + c->DDR_AL,
	c->DDR_WL           = c->DDR_RL - 1,
	c->DDR_CWL          = c->DDR_RL - 1,

	c->DDR_tRFC         = DDR__ns(105);
	c->DDR_tREFI        = DDR__ns(7800);

	c->DDR_tRAS_max     = DDR__ns(70000), // 70K ns
	c->DDR_tRAS_min     = DDR__ns(45),

	c->DDR_tRP          = DDR__ns(12);
	c->DDR_tRC          = DDR__ns(57);

	c->DDR_tRTP         = DDR_SELECT_MAX__tCK_ps(4, 7500);

	c->DDR_tRCD         = DDR__ns(12);

	c->DDR_tRRD         = DDR__ns(10);
	c->DDR_tWR          = DDR__ps(13500);
	c->DDR_tMRD         = DDR__tck(2);
	c->DDR_tWTR         = DDR_SELECT_MAX__tCK_ps(4, 7500);
	c->DDR_tCCD         = DDR__tck(2);
	c->DDR_tFAW         = DDR__ns(45);

	c->DDR_tCKE         = DDR__tck(3);
	c->DDR_tXP          = DDR__tck(4);

	/* Exit Self Refresh to Read command */
	c->DDR_tXSRD        = DDR__tck(200);

	/* Exit Self Refresh to non-Read command */
	c->DDR_tXSNR        = (c->DDR_tRFC + DDR__ns(10));

	/* Exit active power down to Read command */
	c->DDR_tXARD        = DDR__tck(4);
	/* Exit active power down to Read command(slow exit, lower power) */
	c->DDR_tXARDS       = DDR__tck(12) - c->DDR_AL;
}

#define DDR2_M14D5121632A                          \
	{                                              \
		.name = "M14D5121632A",                    \
		.id = DDR_CHIP_ID(TYPE_DDR2),              \
		.type = DDR2,                              \
		.freq = CONFIG_DDR2_M14D5121632A_MEM_FREQ, \
		.size = 64,                                \
		.init = DDR2_M14D5121632A_init,            \
	}

#endif
