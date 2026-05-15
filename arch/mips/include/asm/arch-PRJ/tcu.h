// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2024, Ingenic Semiconductor Co.,Ltd.
 * Author: Keven <keven.ywhan@ingenic.com>
 */

#ifndef __TCU_H__
#define __TCU_H__

#include <asm/arch/base.h>

/*************************************************************************
 * TCU (Timer Counter Unit)
 *************************************************************************/
#define TCU_TER         (TCU_BASE + 0x10)
#define TCU_TESR        (TCU_BASE + 0x14)
#define TCU_TECR        (TCU_BASE + 0x18)
#define TCU_TSR         (TCU_BASE + 0x1C)
#define TCU_TSSR        (TCU_BASE + 0x2C)
#define TCU_TSCR        (TCU_BASE + 0x3C)
#define TCU_TFR         (TCU_BASE + 0x20)
#define TCU_TFSR        (TCU_BASE + 0x24)
#define TCU_TFCR        (TCU_BASE + 0x28)
#define TCU_TMR         (TCU_BASE + 0x30)
#define TCU_TMSR        (TCU_BASE + 0x34)
#define TCU_TMCR        (TCU_BASE + 0x38)
#define TCU_TSFR        (TCU_BASE + 0x200)
#define TCU_TSFSR       (TCU_BASE + 0x204)
#define TCU_TSFCR       (TCU_BASE + 0x208)
#define TCU_TSMR        (TCU_BASE + 0x210)
#define TCU_TSMSR       (TCU_BASE + 0x214)
#define TCU_TSMCR       (TCU_BASE + 0x218)

#define TCU_TSVR(n)     (TCU_BASE + (0x220 + (n)*0x04))
#define TCU_TSFVR(n)    (TCU_BASE + (0x240 + (n)*0x04))
#define CHN_CAP(n)      (TCU_BASE + (0xc0 + (n)*0x04))
#define CHN_CAP_VAL(n)  (TCU_BASE + (0xe0 + (n)*0x04))
#define CHN_FIL_VAL(n)  (TCU_BASE + (0x1a0 + (n)*0x04))

#define TCU_FULL0(n)    (TCU_BASE + (0x40 + (n)*0x10))
#define TCU_TCSR(n)     (TCU_BASE + (0x4c + (n)*0x10))

#endif /* __TCU_H__ */
