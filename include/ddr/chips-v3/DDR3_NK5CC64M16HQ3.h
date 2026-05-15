/*
 * =====================================================================================
 *
 *       Filename:  DDR3_NK5CC64M16HQ3.h
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

#ifndef __DDR3_NK5CC64M16HQ3_H__
#define __DDR3_NK5CC64M16HQ3_H__

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
#ifndef CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ
#define CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ (CONFIG_SYS_MEM_FREQ * 2)
#endif

#if ((CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ >= 300000000) && (CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ < 330000000))
#define CONFIG_DDR_CL 5
#define CONFIG_DDR_CWL 5
#elif ((CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ >= 330000000) && (CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ <= 400000000))
#define CONFIG_DDR_CL 6
#define CONFIG_DDR_CWL 5
#elif ((CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ > 400000000) && (CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ <= 533000000))
#define CONFIG_DDR_CL 7 // or 8
#define CONFIG_DDR_CWL 6
#elif ((CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ > 533000000) && (CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ <= 667000000))
#define CONFIG_DDR_CL 9 // or 10
#define CONFIG_DDR_CWL 7
#elif ((CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ > 667000000) && (CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ <= 800000000))
#define CONFIG_DDR_CL 11
#define CONFIG_DDR_CWL 8
#elif ((CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ > 800000000) && (CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ <= 933000000))
#define CONFIG_DDR_CL 13
#define CONFIG_DDR_CWL 9
#elif ((CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ > 933000000) && (CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ <= 1066000000))
#define CONFIG_DDR_CL 14
#define CONFIG_DDR_CWL 10
#else
#define CONFIG_DDR_CL 14
#define CONFIG_DDR_CWL 10
#endif



static inline void DDR3_NK5CC64M16HQ3_INIT(void *data)
{
	struct ddr_chip_info *c = (struct ddr_chip_info *)data;

	c->DDR_ROW          = 13,
	c->DDR_COL          = 10,
	c->DDR_BANK         = 8,
	c->DDR_BIT          = 16,        /* The bit width of DDR. */

	c->DDR_BL           = 8,
	c->DDR_CL           = CONFIG_DDR_CL,
	c->DDR_CWL          = CONFIG_DDR_CWL,

	c->DDR_AL           = 0,

	c->DDR_tRFC         = DDR__ns(110);
	c->DDR_tREFI        = DDR__ns(7800);

	c->DDR_tRAS_max     = 9 * c->DDR_tREFI,
	c->DDR_tRAS_min     = DDR__ns(33),

	c->DDR_tRP          = DDR__ps(13090);
	c->DDR_tRC          = c->DDR_tRAS_min + c->DDR_tRP;

	c->DDR_RL           = CONFIG_DDR_CL;
	c->DDR_WL           = CONFIG_DDR_CWL + c->DDR_AL;

	c->DDR_tRTP         = DDR_SELECT_MAX__tCK_ps(4, 7500);

	c->DDR_tRCD         = DDR__ps(13090);

	c->DDR_tRRD         = DDR_SELECT_MAX__tCK_ps(4, 6000);
	c->DDR_tWR          = DDR__ns(15);
	c->DDR_tMOD         = DDR_SELECT_MAX__tCK_ps(12, 15000);
	c->DDR_tMRD         = DDR__tck(4);
	c->DDR_tWTR         = DDR_SELECT_MAX__tCK_ps(4, 7500);
	c->DDR_tCCD         = DDR__tck(4);
	c->DDR_tFAW         = DDR__ns(35);

	c->DDR_tCKE         = DDR_SELECT_MAX__tCK_ps(3, 5000);
	c->DDR_tCKESR       = c->DDR_tCKE + DDR__tck(1);
	c->DDR_tXP          = DDR_SELECT_MAX__tCK_ps(3, 6000);
	c->DDR_tXPDLL       = DDR_SELECT_MAX__tCK_ps(10, 24000);
	c->DDR_tXPR         = DDR_SELECT_MAX__tCK_ps(5, 120000);
	c->DDR_tCKSRX       = DDR_SELECT_MAX__tCK_ps(5, 10000);
	c->DDR_tCKSRE       = DDR_SELECT_MAX__tCK_ps(5, 10000);

	c->DDR_tDLLK        = DDR__tck(512); /* tDLLLOCK  DLL locking time */
	c->DDR_tXSDLL       = c->DDR_tDLLK;
	c->DDR_tXS          = DDR_SELECT_MAX__tCK_ps(5, 120000);
}

#define DDR3_NK5CC64M16HQ3                          \
	{                                           \
		.name = "NK5CC64M16HQ3",                    \
		.id   = DDR_CHIP_ID(TYPE_DDR3),         \
		.type = DDR3,                           \
		.freq = CONFIG_DDR3_NK5CC64M16HQ3_MEM_FREQ, \
		.size = 128,                            \
		.init = DDR3_NK5CC64M16HQ3_INIT,            \
	}

#endif
