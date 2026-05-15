#include "ddr_params_creator.h"

void ddrp_wrap(struct ddr_reg_value *reg, struct ddr_chip_info *p)
{
	if (strcmp(p->name, "W631GU6NG") == 0)
	{	                                   // PHY   KGD
		reg->DDRP_CMD0_WRAP = DDRP_BA0;    // A0 -> BA0
		reg->DDRP_CMD1_WRAP = DDRP_A4;     // A1 -> A4
		reg->DDRP_CMD2_WRAP = DDRP_A2;     // A2 -> A2
		reg->DDRP_CMD3_WRAP = DDRP_BA2;    // A3 -> BA2
		reg->DDRP_CMD4_WRAP = DDRP_A1;     // A4 -> A1
		reg->DDRP_CMD5_WRAP = DDRP_A5;     // A5 -> A5
		reg->DDRP_CMD6_WRAP = DDRP_A9;     // A6 -> A9
		reg->DDRP_CMD7_WRAP = DDRP_A11;    // A7 -> A11
		reg->DDRP_CMD8_WRAP = DDRP_A8;     // A8 -> A8
		reg->DDRP_CMD9_WRAP = DDRP_A6;     // A9 -> A6
		reg->DDRP_CMD10_WRAP = DDRP_CASB;  // A10 -> CASB
		reg->DDRP_CMD11_WRAP = DDRP_A7;    // A11 -> A7
		reg->DDRP_CMD12_WRAP = DDRP_A0;    // A12 -> A0
		reg->DDRP_CMD13_WRAP = DDRP_A13;   // A13 -> A13
		reg->DDRP_CMD16_WRAP = DDRP_WEB;   // WEB -> WEB
		reg->DDRP_CMD17_WRAP = DDRP_A10;   // CASB -> A10
		reg->DDRP_CMD18_WRAP = DDRP_RASB;  // RASB -> RASB
		reg->DDRP_CMD19_WRAP = DDRP_BA1;   // BA0 -> BA1
		reg->DDRP_CMD20_WRAP = DDRP_A12;   // BA1 -> A12
		reg->DDRP_CMD21_WRAP = DDRP_A3;    // BA2 -> A3
		reg->DDRP_L_DM_WRAP = DDRP_DQ0;    // DM0 -> DQ0
		reg->DDRP_L_DQ0_WRAP = DDRP_DQ2;   // DQ0 -> DQ2
		reg->DDRP_L_DQ1_WRAP = DDRP_DQ1;   // DQ1 -> DQ1
		reg->DDRP_L_DQ2_WRAP = DDRP_DM0;   // DQ2 -> DM
		reg->DDRP_L_DQ3_WRAP = DDRP_DQ3;   // DQ3 -> DQ3
		reg->DDRP_L_DQ4_WRAP = DDRP_DQ4;   // DQ4 -> DQ4
		reg->DDRP_L_DQ5_WRAP = DDRP_DQ5;   // DQ5 -> DQ5
		reg->DDRP_L_DQ6_WRAP = DDRP_DQ6;   // DQ6 -> DQ6
		reg->DDRP_L_DQ7_WRAP = DDRP_DQ7;   // DQ7 -> DQ7
		reg->DDRP_H_DM_WRAP = DDRP_DQ8;    // DM1 -> DQ8
		reg->DDRP_H_DQ0_WRAP = DDRP_DM1;   // DQ8 -> DM1
		reg->DDRP_H_DQ1_WRAP = DDRP_DQ9;   // DQ9 -> DQ9
		reg->DDRP_H_DQ2_WRAP = DDRP_DQ10;  // DQ10 -> DQ10
		reg->DDRP_H_DQ3_WRAP = DDRP_DQ11;  // DQ11 -> DQ11
		reg->DDRP_H_DQ4_WRAP = DDRP_DQ12;  // DQ12 -> DQ12
		reg->DDRP_H_DQ5_WRAP = DDRP_DQ13;  // DQ13 -> DQ13
		reg->DDRP_H_DQ6_WRAP = DDRP_DQ15;  // DQ14 -> DQ15
		reg->DDRP_H_DQ7_WRAP = DDRP_DQ14;  // DQ15 -> DQ14
	}
	else if (strcmp(p->name, "NK5CC64M16HQ3") == 0)
	{	                                   // PHY   KGD
		reg->DDRP_CMD0_WRAP = DDRP_A3;     // A0 -> A3
		reg->DDRP_CMD1_WRAP = DDRP_A4;     // A1 -> A4
		reg->DDRP_CMD2_WRAP = DDRP_A2;     // A2 -> A2
		reg->DDRP_CMD3_WRAP = DDRP_A0;     // A3 -> A0
		reg->DDRP_CMD4_WRAP = DDRP_A1;     // A4 -> A1
		reg->DDRP_CMD5_WRAP = DDRP_A5;     // A5 -> A5
		reg->DDRP_CMD6_WRAP = DDRP_A11;    // A6 -> A11
		reg->DDRP_CMD7_WRAP = DDRP_A9;     // A7 -> A9
		reg->DDRP_CMD8_WRAP = DDRP_A8;     // A8 -> A8
		reg->DDRP_CMD9_WRAP = DDRP_A7;     // A9 -> A7
		reg->DDRP_CMD10_WRAP = DDRP_A10;   // A10 -> A10
		reg->DDRP_CMD11_WRAP = DDRP_A6;    // A11 -> A6
		reg->DDRP_CMD12_WRAP = DDRP_A12;   // A12 -> A12
		reg->DDRP_CMD13_WRAP = DDRP_A13;   // A13 -> A13
		reg->DDRP_CMD16_WRAP = DDRP_WEB;   // WEB -> WEB / WE#
		reg->DDRP_CMD17_WRAP = DDRP_CASB;  // CASB -> CASB
		reg->DDRP_CMD18_WRAP = DDRP_RASB;  // RASB -> RASB
		reg->DDRP_CMD19_WRAP = DDRP_BA0;   // BA0 -> BA0
		reg->DDRP_CMD20_WRAP = DDRP_BA1;   // BA1 -> BA1
		reg->DDRP_CMD21_WRAP = DDRP_BA2;   // BA2 -> BA2
		reg->DDRP_L_DM_WRAP = DDRP_DM0;    // DM0 -> DM0
		reg->DDRP_L_DQ0_WRAP = DDRP_DQ0;   // DQ0 -> DQ0
		reg->DDRP_L_DQ1_WRAP = DDRP_DQ1;   // DQ1 -> DQ1
		reg->DDRP_L_DQ2_WRAP = DDRP_DQ2;   // DQ2 -> DQ2
		reg->DDRP_L_DQ3_WRAP = DDRP_DQ3;   // DQ3 -> DQ3
		reg->DDRP_L_DQ4_WRAP = DDRP_DQ4;   // DQ4 -> DQ4
		reg->DDRP_L_DQ5_WRAP = DDRP_DQ5;   // DQ5 -> DQ5
		reg->DDRP_L_DQ6_WRAP = DDRP_DQ6;   // DQ6 -> DQ6
		reg->DDRP_L_DQ7_WRAP = DDRP_DQ7;   // DQ7 -> DQ7
		reg->DDRP_H_DM_WRAP = DDRP_DM1;    // DM1 -> DM1
		reg->DDRP_H_DQ0_WRAP = DDRP_DQ8;   // DQ8 -> DQ8
		reg->DDRP_H_DQ1_WRAP = DDRP_DQ9;   // DQ9 -> DQ9
		reg->DDRP_H_DQ2_WRAP = DDRP_DQ10;  // DQ10 -> DQ10
		reg->DDRP_H_DQ3_WRAP = DDRP_DQ11;  // DQ11 -> DQ11
		reg->DDRP_H_DQ4_WRAP = DDRP_DQ12;  // DQ12 -> DQ12
		reg->DDRP_H_DQ5_WRAP = DDRP_DQ13;  // DQ13 -> DQ13
		reg->DDRP_H_DQ6_WRAP = DDRP_DQ14;  // DQ14 -> DQ14
		reg->DDRP_H_DQ7_WRAP = DDRP_DQ15;  // DQ15 -> DQ15
	}
	else if (strcmp(p->name, "M14D5121632A") == 0)
	{	                                   // PHY   KGD
		reg->DDRP_CMD0_WRAP = DDRP_A10;    // A0 -> A10
		reg->DDRP_CMD1_WRAP = DDRP_A3;     // A1 -> A3
		reg->DDRP_CMD2_WRAP = DDRP_A6;     // A2 -> A6
		reg->DDRP_CMD3_WRAP = DDRP_A1;     // A3 -> A1
		reg->DDRP_CMD4_WRAP = DDRP_A5;     // A4 -> A5
		reg->DDRP_CMD5_WRAP = DDRP_A4;     // A5 -> A4
		reg->DDRP_CMD6_WRAP = DDRP_A11;    // A6 -> A11
		reg->DDRP_CMD7_WRAP = DDRP_A9;     // A7 -> A9
		reg->DDRP_CMD8_WRAP = DDRP_A12;    // A8 -> A12
		reg->DDRP_CMD9_WRAP = DDRP_A7;     // A9 -> A7
		reg->DDRP_CMD10_WRAP = DDRP_BA0;   // A10 -> BA0
		reg->DDRP_CMD11_WRAP = DDRP_A8;    // A11 -> A8
		reg->DDRP_CMD12_WRAP = DDRP_A2;    // A12 -> A2
		reg->DDRP_CMD13_WRAP = DDRP_A13;   // A13 -> A13
		reg->DDRP_CMD16_WRAP = DDRP_BA1;   // WEB -> BA1
		reg->DDRP_CMD17_WRAP = DDRP_RASB;  // CASB -> RAS
		reg->DDRP_CMD18_WRAP = DDRP_WEB;   // RASB -> WE
		reg->DDRP_CMD19_WRAP = DDRP_CASB;  // BA0 -> CAS
		reg->DDRP_CMD20_WRAP = DDRP_A0;    // BA1 -> A0
		reg->DDRP_CMD21_WRAP = DDRP_BA2;   // BA2 -> BA2
		reg->DDRP_L_DM_WRAP = DDRP_DM0;    // DM0 -> DM0
		reg->DDRP_L_DQ0_WRAP = DDRP_DQ6;   // DQ0 -> DQ6
		reg->DDRP_L_DQ1_WRAP = DDRP_DQ3;   // DQ1 -> DQ3
		reg->DDRP_L_DQ2_WRAP = DDRP_DQ1;   // DQ2 -> DQ1
		reg->DDRP_L_DQ3_WRAP = DDRP_DQ4;   // DQ3 -> DQ4
		reg->DDRP_L_DQ4_WRAP = DDRP_DQ2;   // DQ4 -> DQ2
		reg->DDRP_L_DQ5_WRAP = DDRP_DQ7;   // DQ5 -> DQ7
		reg->DDRP_L_DQ6_WRAP = DDRP_DQ5;   // DQ6 -> DQ5
		reg->DDRP_L_DQ7_WRAP = DDRP_DQ0;   // DQ7 -> DQ0
		reg->DDRP_H_DM_WRAP = DDRP_DQ13;   // DM1 -> DQ13
		reg->DDRP_H_DQ0_WRAP = DDRP_DQ10;  // DQ8 -> DQ10
		reg->DDRP_H_DQ1_WRAP = DDRP_DQ12;  // DQ9 -> DQ12
		reg->DDRP_H_DQ2_WRAP = DDRP_DQ8;   // DQ10 -> DQ8
		reg->DDRP_H_DQ3_WRAP = DDRP_DQ15;  // DQ11 -> DQ15
		reg->DDRP_H_DQ4_WRAP = DDRP_DQ14;  // DQ12 -> DQ14
		reg->DDRP_H_DQ5_WRAP = DDRP_DQ11;  // DQ13 -> DQ11
		reg->DDRP_H_DQ6_WRAP = DDRP_DQ9;   // DQ14 -> DQ9
		reg->DDRP_H_DQ7_WRAP = DDRP_DM1;   // DQ15 -> DM1
	}
}
