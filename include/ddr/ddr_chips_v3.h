#ifndef __DDR_CHIPS_V3_H__
#define __DDR_CHIPS_V3_H__

#include  "chips-v3/ddr_chip.h"

#ifdef CONFIG_DDR2_M14D5121632A
#include "chips-v3/DDR2_M14D5121632A.h"
#endif
#ifdef CONFIG_DDR2_W9751V6NG
#include "chips-v3/DDR2_W9751V6NG.h"
#endif

#ifdef CONFIG_DDR3_W631GU6NG
#include "chips-v3/DDR3_W631GU6NG.h"
#endif
#ifdef CONFIG_DDR3_NK5CC64M16HQ3
#include "chips-v3/DDR3_NK5CC64M16HQ3.h"
#endif

#ifdef CONFIG_DDR3_W632GU6NG
#include "chips-v3/DDR3_W632GU6NG.h"
#endif
#ifdef CONFIG_DDR3_W634GU6RB
#include "chips-v3/DDR3_W634GU6RB.h"
#endif
#ifdef CONFIG_DDR3_W638GU6QB
#include "chips-v3/DDR3_W638GU6QB.h"
#endif

#ifdef CONFIG_LPDDR3_W63AH6N2B_BJ
#include "chips-v3/LPDDR3_W63AH6N2B_BJ.h"
#endif


#endif
