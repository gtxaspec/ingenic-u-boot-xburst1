/*
 * C100 sadc definitions
 *
 * Copyright (c) 2019 Ingenic Semiconductor Co.,Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __SADC_H__
#define __SADC_H__

#include <asm/arch/base.h>

/*************************************************************************
 * SADC (SAR A/D Controller)
 *************************************************************************/
#define SADC_ADENA	    0x00
#define SADC_ADCFG	    0x04
#define SADC_ADCTRL	    0x08
#define SADC_ADSTATE	0x0C
/*
#define SADC_ADSAME	    0x10
#define SADC_ADWAIT	    0x14
#define SADC_ADTCH	    0x18
#define SADC_ADVDAT	    0x1C
#define SADC_ADADAT	    0x20
#define SADC_ADCMD	    0x24
#define SADC_ADCLK	    0x28
#define SADC_ADSTB	    0x34
*/
#define SADC_ADATA0	    0x10
#define SADC_ADATA1	    0x14
#define SADC_ADATA2	    0x18
#define SADC_ADCLK	    0x20
#define SADC_ADSTB	    0x24
#define SADC_ADRETM	    0x28


/* ADC Enable Register */
/*
#define SADC_ADENA_POWER	(1 << 7)
#define SADC_ADENA_SLP_MD	(1 << 6)
#define SADC_ADENA_PENDEN	(1 << 3)
#define SADC_ADENA_TCHEN	(1 << 2)
#define SADC_ADENA_VBATEN	(1 << 1)
#define SADC_ADENA_AUXEN	(1 << 0)
*/

#define SADC_ADENA_POWER	    (1 << 7) //reserverd
#define SADC_ADENA_POWER_DOWN	(1 << 15)
#define SADC_ADENA_POWER_ON	    (0 << 15)
#define SADC_ADENA_POWER_OPT    (1 << 14)
#define SADC_ADENA_AUX4EN	    (1 << 4)
#define SADC_ADENA_AUX3EN	    (1 << 3)
#define SADC_ADENA_AUX2EN	    (1 << 2)
#define SADC_ADENA_AUX1EN	    (1 << 1)
#define SADC_ADENA_AUX0EN	    (1 << 0)


/* ADC Status Register */
#define SADC_ADSTATE_SLP_RDY	(1 << 7) //C100 delete
#define SADC_ADSTATE_SLPEND	    (1 << 5) //C100 delete
#define SADC_ADSTATE_ARDY4	    (1 << 4)
#define SADC_ADSTATE_ARDY3	    (1 << 3)
#define SADC_ADSTATE_ARDY2	    (1 << 2)
#define SADC_ADSTATE_ARDY1	    (1 << 1)
#define SADC_ADSTATE_ARDY0	    (1 << 0)

#endif /* __SADC_H__ */
