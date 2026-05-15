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
#ifdef CONFIG_DDR3_W632GU6NG
#include "chips-v3/DDR3_W632GU6NG.h"
#endif
#ifdef CONFIG_DDR3_W638GU6QB
#include "chips-v3/DDR3_W638GU6QB.h"
#endif

#endif
