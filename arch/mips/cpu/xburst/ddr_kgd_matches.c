#include <asm/ddr_innophy.h>
#include <generated/ddr_reg_values.h>
#include <config.h>
#include <common.h>
#include <ddr/ddrc.h>

DECLARE_GLOBAL_DATA_PTR;

/* #define CONFIG_DWC_DEBUG 1 */
#include "ddr_debug.h"

/* efuse_ddrpmap_get() returns const unsigned char *, declare here to avoid implicit declaration warning */
extern const unsigned char *efuse_ddrpmap_get(void);

void ddrp_map_set()
{
	unsigned char w631gu6ng_map[] = W631GU6NG_MAP;
	unsigned char nk5cc64m16hq3_map[] = NK5CC64M16HQ3_MAP;
	unsigned char m14d5121632a_map[] = M14D5121632A_MAP;
	const unsigned char *ddrp_map = efuse_ddrpmap_get();

	if(NULL == ddrp_map) // EFUSE中没有map参数或者参数错误
	{
		switch (gd->arch.gi->soc.xburst.ddr_id)
		{
			case WINBOND_20NM_1G_D3_1066:
				ddrp_map = w631gu6ng_map;
				break;
			case NTC_20NM_1G_D3:
			case ESMT_21NM_1G_D3:
				ddrp_map = nk5cc64m16hq3_map;
				break;
			case ETRON_25NM_1G_D3:
				return;
			case ESMT_25NM_512_D2:
			case ESMT_25NM_512_D2_2S:
			case ESMT_25NM_512_D2_2G:
			case WINBOND_25NM_512_D2:
			case ETRON_25NM_512_D2:
			case UNIIC_25NM_512_D2:
				ddrp_map = m14d5121632a_map;
				break;
			default:
#if defined(CONFIG_DDR3_W631GU6NG)
				ddrp_map = w631gu6ng_map;
#elif defined(CONFIG_DDR_TYPE_DDR3)
				ddrp_map = nk5cc64m16hq3_map;
#elif defined(CONFIG_DDR_TYPE_DDR2)
				ddrp_map = m14d5121632a_map;
#else
				ddrp_map = w631gu6ng_map;
#endif
		}
	}

	SET_INNOPHY_REG(reg_cmd0_wrap_sel, (u32)ddrp_map[0]);
	SET_INNOPHY_REG(reg_cmd1_wrap_sel, (u32)ddrp_map[1]);
	SET_INNOPHY_REG(reg_cmd2_wrap_sel, (u32)ddrp_map[2]);
	SET_INNOPHY_REG(reg_cmd3_wrap_sel, (u32)ddrp_map[3]);
	SET_INNOPHY_REG(reg_cmd4_wrap_sel, (u32)ddrp_map[4]);
	SET_INNOPHY_REG(reg_cmd5_wrap_sel, (u32)ddrp_map[5]);
	SET_INNOPHY_REG(reg_cmd6_wrap_sel, (u32)ddrp_map[6]);
	SET_INNOPHY_REG(reg_cmd7_wrap_sel, (u32)ddrp_map[7]);
	SET_INNOPHY_REG(reg_cmd8_wrap_sel, (u32)ddrp_map[8]);
	SET_INNOPHY_REG(reg_cmd9_wrap_sel, (u32)ddrp_map[9]);
	SET_INNOPHY_REG(reg_cmd10_wrap_sel, (u32)ddrp_map[10]);
	SET_INNOPHY_REG(reg_cmd11_wrap_sel, (u32)ddrp_map[11]);
	SET_INNOPHY_REG(reg_cmd12_wrap_sel, (u32)ddrp_map[12]);
	SET_INNOPHY_REG(reg_cmd13_wrap_sel, (u32)ddrp_map[13]);
	SET_INNOPHY_REG(reg_cmd16_wrap_sel, (u32)ddrp_map[14]); // WEB
	SET_INNOPHY_REG(reg_cmd17_wrap_sel, (u32)ddrp_map[15]); // CASB
	SET_INNOPHY_REG(reg_cmd18_wrap_sel, (u32)ddrp_map[16]); // RASB
	SET_INNOPHY_REG(reg_cmd19_wrap_sel, (u32)ddrp_map[17]); // BA0
	SET_INNOPHY_REG(reg_cmd20_wrap_sel, (u32)ddrp_map[18]); // BA1
	SET_INNOPHY_REG(reg_cmd21_wrap_sel, (u32)ddrp_map[19]); // BA2
	SET_INNOPHY_REG(reg_a_l_dm_bit_wrap_sel, (u32)ddrp_map[20]);
	SET_INNOPHY_REG(reg_a_l_dq0_bit_wrap_sel, (u32)ddrp_map[21]);
	SET_INNOPHY_REG(reg_a_l_dq1_bit_wrap_sel, (u32)ddrp_map[22]);
	SET_INNOPHY_REG(reg_a_l_dq2_bit_wrap_sel, (u32)ddrp_map[23]);
	SET_INNOPHY_REG(reg_a_l_dq3_bit_wrap_sel, (u32)ddrp_map[24]);
	SET_INNOPHY_REG(reg_a_l_dq4_bit_wrap_sel, (u32)ddrp_map[25]);
	SET_INNOPHY_REG(reg_a_l_dq5_bit_wrap_sel, (u32)ddrp_map[26]);
	SET_INNOPHY_REG(reg_a_l_dq6_bit_wrap_sel, (u32)ddrp_map[27]);
	SET_INNOPHY_REG(reg_a_l_dq7_bit_wrap_sel, (u32)ddrp_map[28]);
	SET_INNOPHY_REG(reg_a_h_dm_bit_wrap_sel, (u32)ddrp_map[29]);
	SET_INNOPHY_REG(reg_a_h_dq0_bit_wrap_sel, (u32)ddrp_map[30]);
	SET_INNOPHY_REG(reg_a_h_dq1_bit_wrap_sel, (u32)ddrp_map[31]);
	SET_INNOPHY_REG(reg_a_h_dq2_bit_wrap_sel, (u32)ddrp_map[32]);
	SET_INNOPHY_REG(reg_a_h_dq3_bit_wrap_sel, (u32)ddrp_map[33]);
	SET_INNOPHY_REG(reg_a_h_dq4_bit_wrap_sel, (u32)ddrp_map[34]);
	SET_INNOPHY_REG(reg_a_h_dq5_bit_wrap_sel, (u32)ddrp_map[35]);
	SET_INNOPHY_REG(reg_a_h_dq6_bit_wrap_sel, (u32)ddrp_map[36]);
	SET_INNOPHY_REG(reg_a_h_dq7_bit_wrap_sel, (u32)ddrp_map[37]);

	dwc_debug("=============== DDR PHY WRAP =================\n");
	PRINT_DDRP(reg_cmd0_wrap_sel);
	PRINT_DDRP(reg_cmd1_wrap_sel);
	PRINT_DDRP(reg_cmd2_wrap_sel);
	PRINT_DDRP(reg_cmd3_wrap_sel);
	PRINT_DDRP(reg_cmd4_wrap_sel);
	PRINT_DDRP(reg_cmd5_wrap_sel);
	PRINT_DDRP(reg_cmd6_wrap_sel);
	PRINT_DDRP(reg_cmd7_wrap_sel);
	PRINT_DDRP(reg_cmd8_wrap_sel);
	PRINT_DDRP(reg_cmd9_wrap_sel);
	PRINT_DDRP(reg_cmd10_wrap_sel);
	PRINT_DDRP(reg_cmd11_wrap_sel);
	PRINT_DDRP(reg_cmd12_wrap_sel);
	PRINT_DDRP(reg_cmd13_wrap_sel);
	PRINT_DDRP(reg_cmd16_wrap_sel);
	PRINT_DDRP(reg_cmd17_wrap_sel);
	PRINT_DDRP(reg_cmd18_wrap_sel);
	PRINT_DDRP(reg_cmd19_wrap_sel);
	PRINT_DDRP(reg_cmd20_wrap_sel);
	PRINT_DDRP(reg_cmd21_wrap_sel);
	PRINT_DDRP(reg_a_l_dm_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq0_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq1_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq2_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq3_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq4_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq5_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq6_bit_wrap_sel);
	PRINT_DDRP(reg_a_l_dq7_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dm_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq0_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq1_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq2_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq3_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq4_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq5_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq6_bit_wrap_sel);
	PRINT_DDRP(reg_a_h_dq7_bit_wrap_sel);
}

#if defined(CONFIG_DDR_PARAMS_STRUCT)
void get_ddr_params_normal()
{
	unsigned int size = 0;
	unsigned int found = 0;
	unsigned int i;

	size = ARRAY_SIZE(supported_ddr_reg_values);

	if (size == 1)
	{
		found = 1;
		global_reg_value = &supported_ddr_reg_values[0];
	}
	else
	{
		for (i = 0; i < size; i++)
		{
			global_reg_value = &supported_ddr_reg_values[i];
			if (gd->arch.gi->soc.xburst.ddr_id == global_reg_value->h.id)
			{
				found = 1;
				break;
			}
		}
		if (!found)
			printf("unknown kgd info %x\n", ddr_id);
	}
	// printf("global_reg_value %s\n", global_reg_value->h.name);
	// printf("use ddr freq %d\n", DDR_FREQ);
}
#endif
