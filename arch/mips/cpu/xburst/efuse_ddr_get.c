/*
 * Copyright (C) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: knight <tyu@ingenic.cn>
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
#include <asm/ddr_innophy.h>
#include <generated/ddr_reg_values.h>
#include <config.h>
#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

// #define HAM_DEBUG
#ifdef HAM_DEBUG
#define ham_debug(fmt, args...) \
    do                          \
    {                           \
        printf(fmt, ##args);    \
    } while (0)
#else
#define ham_debug(fmt, args...) \
    do                          \
    {                           \
    } while (0)
#endif

int check_ParityBits(int dataBits)
{
    int m = 1;
    while ((1 << m) < (dataBits + m + 1))
    {
        m++;
    }
    return m;
}

static void get_index_ddr(int index, int data, unsigned int *efuse_ddr_datas)
{
    switch (index)
    {
        case 0x1:
            if(data & 0x40)
            {
                efuse_ddr_datas[ODT_PD] = efuse_ddr_datas[ODT_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_ODT_PD] = efuse_ddr_datas[INDEX_ODT_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[ODT_PU]    is %x  get efuse_ddr_datas[ODT_PD]    is %x\n",
                           index, efuse_ddr_datas[ODT_PU], efuse_ddr_datas[ODT_PD]);
            }
            else
            {
                efuse_ddr_datas[ODT_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_ODT_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[ODT_PU]    is %x\n", index, efuse_ddr_datas[ODT_PU]);
            }
            break;
        case 0x2:
            if(data & 0x40)
            {
                efuse_ddr_datas[ODT_PD] = efuse_ddr_datas[ODT_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_ODT_PD] = efuse_ddr_datas[INDEX_ODT_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[ODT_PU]    is %x  get efuse_ddr_datas[ODT_PD]    is %x\n",
                           index, efuse_ddr_datas[ODT_PU], efuse_ddr_datas[ODT_PD]);
            }
            else
            {
                efuse_ddr_datas[ODT_PD] = data & 0x1f;
                efuse_ddr_datas[INDEX_ODT_PD] = 1;
                ham_debug("index %x: get efuse_ddr_datas[ODT_PD]    is %x\n", index, efuse_ddr_datas[ODT_PD]);
            }
            break;
        case 0x3:
            if(data & 0x40)
            {
                efuse_ddr_datas[CMD_RC_PD] = efuse_ddr_datas[CMD_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_CMD_PD] = efuse_ddr_datas[INDEX_CMD_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CMD_RC_PU] is %x   get efuse_ddr_datas[CMD_RC_PD] is %x\n",
                           index, efuse_ddr_datas[CMD_RC_PU], efuse_ddr_datas[CMD_RC_PD]);
            }
            else
            {
                efuse_ddr_datas[CMD_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_CMD_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CMD_RC_PU] is %x\n", index, efuse_ddr_datas[CMD_RC_PU]);
            }
            break;
        case 0x4:
            if(data & 0x40)
            {
                efuse_ddr_datas[CMD_RC_PD] = efuse_ddr_datas[CMD_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_CMD_PD] = efuse_ddr_datas[INDEX_CMD_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CMD_RC_PU] is %x   get efuse_ddr_datas[CMD_RC_PD] is %x\n",
                        index, efuse_ddr_datas[CMD_RC_PU], efuse_ddr_datas[CMD_RC_PD]);
            }
            else
            {
                efuse_ddr_datas[CMD_RC_PD] = data & 0x1f;
                efuse_ddr_datas[INDEX_CMD_PD] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CMD_RC_PD] is %x\n", index, efuse_ddr_datas[CMD_RC_PD]);
            }
            break;
        case 0x5:
            if(data & 0x40)
            {
                efuse_ddr_datas[CLK_RC_PD] = efuse_ddr_datas[CLK_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_CLK_PD] = efuse_ddr_datas[INDEX_CLK_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CLK_RC_PU] is %x   get efuse_ddr_datas[CLK_RC_PD] is %x\n",
                           index, efuse_ddr_datas[CLK_RC_PU], efuse_ddr_datas[CLK_RC_PD]);
            }
            else
            {
                efuse_ddr_datas[CLK_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_CLK_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CLK_RC_PU] is %x\n", index, efuse_ddr_datas[CLK_RC_PU]);
            }
            break;
        case 0x6:
            if(data & 0x40)
            {
                efuse_ddr_datas[CLK_RC_PD] = efuse_ddr_datas[CLK_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_CLK_PD] = efuse_ddr_datas[INDEX_CLK_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CLK_RC_PU] is %x   get efuse_ddr_datas[CLK_RC_PD] is %x\n",
                           index, efuse_ddr_datas[CLK_RC_PU], efuse_ddr_datas[CLK_RC_PD]);
            }
            else
            {
                efuse_ddr_datas[CLK_RC_PD] = data & 0x1f;
                efuse_ddr_datas[INDEX_CLK_PD] = 1;
                ham_debug("index %x: get efuse_ddr_datas[CLK_RC_PD] is %x\n", index, efuse_ddr_datas[CLK_RC_PD]);
            }
            break;
        case 0x7:
            if(data & 0x40)
            {
                efuse_ddr_datas[DQX_RC_PD] = efuse_ddr_datas[DQX_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_DQ_PD] = efuse_ddr_datas[INDEX_DQ_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[DQX_RC_PD] is %x   get efuse_ddr_datas[DQX_RC_PU] is %x\n",
                           index, efuse_ddr_datas[DQX_RC_PD], efuse_ddr_datas[DQX_RC_PU]);
            }
            else
            {
                efuse_ddr_datas[DQX_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_DQ_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[DQX_RC_PU] is %x\n", index, efuse_ddr_datas[DQX_RC_PU]);
            }
            break;
        case 0x8:
            if(data & 0x40)
            {
                efuse_ddr_datas[DQX_RC_PD] = efuse_ddr_datas[DQX_RC_PU] = data & 0x1f;
                efuse_ddr_datas[INDEX_DQ_PD] = efuse_ddr_datas[INDEX_DQ_PU] = 1;
                ham_debug("index %x: get efuse_ddr_datas[DQX_RC_PD] is %x   get efuse_ddr_datas[DQX_RC_PU] is %x\n",
                           index, efuse_ddr_datas[DQX_RC_PD], efuse_ddr_datas[DQX_RC_PU]);
            }
            else
            {
                efuse_ddr_datas[DQX_RC_PD] = data & 0x1f;
                efuse_ddr_datas[INDEX_DQ_PD] = 1;
                ham_debug("index %x: get efuse_ddr_datas[DQX_RC_PD] is %x\n", index, efuse_ddr_datas[DQX_RC_PD]);
            }
            break;
        case 0x9:
            efuse_ddr_datas[KGD_ODT] = data & 0x7;
            efuse_ddr_datas[KGD_DS] = (data >> 3) & 0x3;
            efuse_ddr_datas[INDEX_KGD] = 1;
            ham_debug("index %x: get efuse_ddr_datas[KGD_ODT]   is %x, efuse_ddr_datas[KGD_DS]   is %x\n",
                        index, efuse_ddr_datas[KGD_ODT], efuse_ddr_datas[KGD_DS]);
            break;
        case 0xa:
            efuse_ddr_datas[VREF] = data << 1;
            efuse_ddr_datas[INDEX_VREF] = 1;
            ham_debug("index %x: get efuse_ddr_datas[VREF]      is %x\n", index, efuse_ddr_datas[VREF]);
            break;
        case 0xb:
            efuse_ddr_datas[SKEW_DQS1R] = efuse_ddr_datas[SKEW_DQS0R] = data;
            efuse_ddr_datas[INDEX_DQSR] = 1;
            efuse_ddr_datas[SKEW_TRX] |= 0x1;
            ham_debug("index %x: get efuse_ddr_datas[SKEW_DQSR] is %x\n", index, efuse_ddr_datas[SKEW_DQS0R]);
            break;
        case 0xc:
            efuse_ddr_datas[SKEW_DQS1T] = efuse_ddr_datas[SKEW_DQS0T] = data;
            efuse_ddr_datas[INDEX_DQST] = 1;
            efuse_ddr_datas[SKEW_TRX] |= 0x2;
            ham_debug("index %x: get efuse_ddr_datas[SKEW_DQST] is %x\n", index, efuse_ddr_datas[SKEW_DQS0T]);
            break;
        case 0xd:
            efuse_ddr_datas[SKEW_DQRX] = data;
            efuse_ddr_datas[INDEX_DQRX] = 1;
            efuse_ddr_datas[SKEW_TRX] |= 0x1;
            ham_debug("index %x: get efuse_ddr_datas[SKEW_DQRX] is %x\n", index, efuse_ddr_datas[SKEW_DQRX]);
            break;
        case 0xe:
            efuse_ddr_datas[SKEW_DQTX] = data;
            efuse_ddr_datas[INDEX_DQRX] = 1;
            efuse_ddr_datas[SKEW_TRX] |= 0x2;
            ham_debug("index %x: get efuse_ddr_datas[SKEW_DQTX] is %x\n", index, efuse_ddr_datas[SKEW_DQTX]);
            break;
        case 0xf:
            efuse_ddr_datas[SKEW_DQTX] = efuse_ddr_datas[SKEW_DQRX] = data;
            efuse_ddr_datas[INDEX_DQTX] = efuse_ddr_datas[INDEX_DQRX] = 1;
            efuse_ddr_datas[SKEW_TRX] |= 0x3;
            ham_debug("index %x: get efuse_ddr_datas[SKEW_DQRX] is %x   get efuse_ddr_datas[SKEW_DQTX] is %x\n",
                        index, efuse_ddr_datas[SKEW_DQRX], efuse_ddr_datas[SKEW_DQTX]);
            break;
        default:
            ham_debug("index not support!\n");
            break;
    }
}

static void get_efuse_ddr_data(int data[], int numBits, unsigned int *efuse_ddr_datas)
{
    int i = 0, k = 0;
    int index_0 = 0, data_0 = 0;
    int index_1 = 0, data_1 = 0;
    int index_2 = 0, data_2 = 0;
    unsigned int j = 0;
    unsigned int get_ddr_datas[KGD_RTT_DIC] = {0};

    ham_debug("data is: ");
    for (i = numBits - 1; i >= 0; i--)
    {
        ham_debug("%d ", data[i]);
        if((i == 1 || i == 6 || i == 11 || i == 16 || i == 21 || i == 24 || i == 26) && 0 == data[0])
            ham_debug("| ");
        else if((i == 1 || i == 8 || i == 12 || i == 19 || i == 23 || i == 30 || i == 34 || i == 38 ) && 1 == data[0])
            ham_debug("| ");
    }
    ham_debug("\n");

    if (data[0])
    {
        ham_debug("          Index  |       DATA    |  Index  |     DATA      |  Index  |     DATA      | Index\n");
        efuse_ddr_datas[INDEX_EN] = 1;
        for (i = 1, j = 0; j < 7; i++, j++)
            data_0 |= (data[i] & 0x1) << j;
        for (i, j = 0; j < 4; i++, j++)
            index_0 |= (data[i] & 0x1) << j;
        for (i, j = 0; j < 7; i++, j++)
            data_1 |= (data[i] & 0x1) << j;
        for (i, j = 0; j < 4; i++, j++)
            index_1 |= (data[i] & 0x1) << j;
        for (i, j = 0; j < 7; i++, j++)
            data_2 |= (data[i] & 0x1) << j;
        for (i, j = 0; j < 4; i++, j++)
            index_2 |= (data[i] & 0x1) << j;
        get_index_ddr(index_0, data_0, efuse_ddr_datas);
        get_index_ddr(index_1, data_1, efuse_ddr_datas);
        get_index_ddr(index_2, data_2, efuse_ddr_datas);
    }
    else
    {
        ham_debug("                VREF     |  DS |  RTT  |  DATA_RC  |   CK_RC   |   CMD_RC  |    ODT    | Index\n");
        efuse_ddr_datas[INDEX_EN] = 0;
        for (i = 1, j = 0; j < (sizeof(((hanming_ddr_data *)0)->odt) / 4); j++, i++)
            get_ddr_datas[ODT_PD] = get_ddr_datas[ODT_PU] |= (data[i] & 0x1) << j;

        for (i, j = 0; j < (sizeof(((hanming_ddr_data *)0)->cmd_rc) / 4); j++, i++)
            get_ddr_datas[CMD_RC_PD] = get_ddr_datas[CMD_RC_PU] |= (data[i] & 0x1) << j;

        for (i, j = 0; j < (sizeof(((hanming_ddr_data *)0)->ck_rc) / 4); j++, i++)
            get_ddr_datas[CLK_RC_PD] = get_ddr_datas[CLK_RC_PU] |= (data[i] & 0x1) << j;

        for (i, j = 0; j < (sizeof(((hanming_ddr_data *)0)->dq_rc) / 4); j++, i++)
            get_ddr_datas[DQX_RC_PD] = get_ddr_datas[DQX_RC_PU] |= (data[i] & 0x1) << j;

        for (i, j = 0; j < (sizeof(((hanming_ddr_data *)0)->kgd_odt) / 4); j++, i++)
            get_ddr_datas[KGD_ODT] |= (data[i] & 0x1) << j;

        for (i, j = 0; j < (sizeof(((hanming_ddr_data *)0)->kgd_ds) / 4); j++, i++)
            get_ddr_datas[KGD_DS] |= (data[i] & 0x1) << j;

        for (i, j = 0; j < (sizeof(((hanming_ddr_data *)0)->vref) / 4); j++, i++)
            get_ddr_datas[VREF] |= (data[i] & 0x1) << j;

        memcpy(efuse_ddr_datas, get_ddr_datas, sizeof(get_ddr_datas));
    }
}

static int check_HammingCode(int hammingCode[], int numBits, unsigned int *efuse_ddr_datas)
{
    int numParityBits = check_ParityBits(numBits);
    unsigned int zero_parity = 0;
    int position = 0;
    int bitMask = 0;
    int parity = 0;
    int i, j;
    int decodedData = 0;
    int dataBits[numBits];
    int dataIndex = 0;

    for (i = 0; i < numParityBits; i++)
    {
        position = (1 << i);
        bitMask = position;
        parity = 0;

        for (j = position; j <= numBits + numParityBits; j++)
        {
            if (j & bitMask)
            {
                /* 0 ^ 0 = 0; 1 ^ 0 = 1*/
                parity ^= hammingCode[j - 1];
            }
        }
        zero_parity |= parity << i;
    }

    if (zero_parity != 0)
    {
        ham_debug("efuse data Err!!!\n");
        return 0;
    }
    else
    {
        ham_debug("data is right\n");
    }

    for (i = 0, j = 0; i < numBits + numParityBits; i++)
    {
        if ((i + 1) == (1 << j))
        {
            j++;
        }
        else
        {
            dataBits[dataIndex++] = hammingCode[i];
        }
    }

    for (i = 0; i < numBits; i++)
    {
        decodedData |= dataBits[i] << i;
    }
    get_efuse_ddr_data(dataBits, numBits, efuse_ddr_datas);
    return decodedData;
}

static void ddr_par_init(unsigned int *ddr_drv_config)
{
#if defined(CONFIG_DDR_TYPE_DDR3)      //  ODT_D ODT_U CMD_D CMD_U CLK_D CLK_U DQX_D DQX_U VREF K_RTT KGD_DS RTT&DIC
    unsigned int init_ddr_par[INDEX_EN] = {0x00, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x70, 0x00, 0x00, 0x00,
                                           0x20, 0x20, 0x3a, 0x84, 0x84, 0x80, 0x00};
                                       // DQS0R  DQS1R DQRX  DQS0T DQS1T DQTX  S_TRX

#elif defined(CONFIG_DDR_TYPE_DDR2)    //  ODT_D ODT_U CMD_D CMD_U CLK_D CLK_U DQX_D DQX_U VREF K_RTT KGD_DS RTT&DIC
    unsigned int init_ddr_par[INDEX_EN] = {0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x70, 0x00, 0x00, 0x00,
                                           0x55, 0x55, 0x40, 0x7a, 0x7a, 0x80, 0x00};
                                       // DQS0R  DQS1R DQRX  DQS0T DQS1T DQTX  S_TRX

    if (ETRON_25NM_512_D2 == (gd->arch.gi->soc.xburst.ddr_id))
	    init_ddr_par[ODT_PU] = init_ddr_par[ODT_PD] = 0x5;

#elif defined(CONFIG_DDR_TYPE_LPDDR3)  //  ODT_D ODT_U CMD_D CMD_U CLK_D CLK_U DQX_D DQX_U VREF K_RTT KGD_DS RTT&DIC
    unsigned int init_ddr_par[INDEX_EN] = {0x00, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x70, 0x00, 0x02, 0x02,
                                           0x33, 0x33, 0x20, 0x86, 0x86, 0x7d, 0x00};
                                       // DQS0R  DQS1R DQRX  DQS0T DQS1T DQTX  S_TRX

#else                                  //  ODT_D ODT_U CMD_D CMD_U CLK_D CLK_U DQX_D DQX_U VREF K_RTT KGD_DS RTT&DIC
    unsigned int init_ddr_par[INDEX_EN] = {0x00, 0x00, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x70, 0x00, 0x00, 0x00,
                                           0x0f, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x00}; /* old version */
                                       // DQS0R  DQS1R DQRX  DQS0T DQS1T DQTX  S_TRX

#endif

    memcpy(ddr_drv_config, init_ddr_par, sizeof(init_ddr_par));
}

static void set_ddr_par(unsigned int *efuse_ddr_data, unsigned int *ddr_drv_config, int par_size)
{
    unsigned int kgd_rtt = efuse_ddr_data[KGD_ODT];
    unsigned int kgd_dic = efuse_ddr_data[KGD_DS];
#ifdef CONFIG_DDR_TYPE_DDR3 // D.I.C: bit 1 5        RTT:2 6 9
    efuse_ddr_data[KGD_RTT_DIC]  = ((kgd_rtt & 0x1) << 2) | ((kgd_rtt & 0x2) << 5) | ((kgd_rtt & 0x4) << 7);
    efuse_ddr_data[KGD_RTT_DIC] |= ((kgd_dic & 0x1) << 1) | ((kgd_dic & 0x2) << 4);

#elif defined(CONFIG_DDR_TYPE_DDR2) // D.I.C: bit 1      RTT: 2 6
    efuse_ddr_data[KGD_RTT_DIC]  = ((kgd_rtt & 0x1) << 2) | ((kgd_rtt & 0x2) << 5);
    efuse_ddr_data[KGD_RTT_DIC] |= ((kgd_dic & 0x1) << 1);

#else // LPDDR2/3
    efuse_ddr_data[KGD_RTT_DIC]  = (kgd_dic & 0x3);
#endif

    memcpy(ddr_drv_config, efuse_ddr_data, par_size);
}

void dump_ddr_par(unsigned int *ddr_drv_config)
{
    ham_debug("---------------------------------------------------\n");
    ham_debug("efuse_test ODT_PU       %x\n", ddr_drv_config[ODT_PU]);
    ham_debug("efuse_test ODT_PD       %x\n", ddr_drv_config[ODT_PD]);
    ham_debug("efuse_test CMD_RC_PU    %x\n", ddr_drv_config[CMD_RC_PU]);
    ham_debug("efuse_test CMD_RC_PD    %x\n", ddr_drv_config[CMD_RC_PD]);
    ham_debug("efuse_test CLK_RC_PU    %x\n", ddr_drv_config[CLK_RC_PU]);
    ham_debug("efuse_test CLK_RC_PD    %x\n", ddr_drv_config[CLK_RC_PD]);
    ham_debug("efuse_test DQX_RC_PU    %x\n", ddr_drv_config[DQX_RC_PU]);
    ham_debug("efuse_test DQX_RC_PD    %x\n", ddr_drv_config[DQX_RC_PD]);
    ham_debug("efuse_test KGD_ODT      %x\n", ddr_drv_config[KGD_ODT]);
    ham_debug("efuse_test KGD_DS       %x\n", ddr_drv_config[KGD_DS]);
    ham_debug("efuse_test KGD_RTT_DIC  %x\n", ddr_drv_config[KGD_RTT_DIC]);
    ham_debug("efuse_test SKEW_DQS0R   %x\n", ddr_drv_config[SKEW_DQS0R]);
    ham_debug("efuse_test SKEW_DQS1R   %x\n", ddr_drv_config[SKEW_DQS1R]);
    ham_debug("efuse_test SKEW_DQRX    %x\n", ddr_drv_config[SKEW_DQRX]);
    ham_debug("efuse_test SKEW_DQS0T   %x\n", ddr_drv_config[SKEW_DQS0T]);
    ham_debug("efuse_test SKEW_DQS1T   %x\n", ddr_drv_config[SKEW_DQS1T]);
    ham_debug("efuse_test SKEW_DQTX    %x\n", ddr_drv_config[SKEW_DQTX]);
    ham_debug("efuse_test VREF         %x\n", ddr_drv_config[VREF]);
    ham_debug("efuse_test SKEW_TRX     %x\n", ddr_drv_config[SKEW_TRX]);
    ham_debug("efuse_test INDEX_EN     %x\n", ddr_drv_config[INDEX_EN]);
    ham_debug("efuse_test INDEX_ODT_PU %x\n", ddr_drv_config[INDEX_ODT_PU]);
    ham_debug("efuse_test INDEX_ODT_PD %x\n", ddr_drv_config[INDEX_ODT_PD]);
    ham_debug("efuse_test INDEX_CMD_PU %x\n", ddr_drv_config[INDEX_CMD_PU]);
    ham_debug("efuse_test INDEX_CMD_PD %x\n", ddr_drv_config[INDEX_CMD_PD]);
    ham_debug("efuse_test INDEX_CLK_PU %x\n", ddr_drv_config[INDEX_CLK_PU]);
    ham_debug("efuse_test INDEX_CLK_PD %x\n", ddr_drv_config[INDEX_CLK_PD]);
    ham_debug("efuse_test INDEX_DQ_PU  %x\n", ddr_drv_config[INDEX_DQ_PU]);
    ham_debug("efuse_test INDEX_DQ_PD  %x\n", ddr_drv_config[INDEX_DQ_PD]);
    ham_debug("efuse_test INDEX_KGD    %x\n", ddr_drv_config[INDEX_KGD]);
    ham_debug("---------------------------------------------------\n");

}

void get_ddr_par(unsigned int *ddr_drv_config, int par_size)
{
    int i = 0;
    unsigned int efuse_ddr_data[DDR_PAR_NUM] = {0};
    int decodedData = 0, hamming_data0 = 0, hamming_data1 = 0;
    int dataBits = sizeof(hanming_ddr_data) / 4 + 1;
    int numParityBits = check_ParityBits(dataBits);
    int hammingCode[dataBits + numParityBits + 1];

    ddr_par_init(efuse_ddr_data);

    if (gd->arch.gi->soc.xburst.PRJ007){
        hamming_data0 = *((volatile unsigned int *)(0xb3540220)) & 0xffffff00;
        hamming_data1 = *((volatile unsigned int *)(0xb3540224)) & 0xffff;
        hamming_data0 = ((hamming_data0 >> 8) & 0xffffff) | (((hamming_data1 & 0xff) << 24) & 0xff000000);
        hamming_data1 = (hamming_data1 >> 8) & 0xff;
    } else if (gd->arch.gi->soc.xburst.PRJ008) {
        hamming_data0 = *((volatile unsigned int *)(0xb3540214));
        hamming_data1 = *((volatile unsigned int *)(0xb3540218)) & 0xff;
    }
    else {
        printf("Unknown soc version(eFuse)\n");
    }

    printf("%x %p %x %x\n", gd->arch.gi->soc.d32, &hamming_data0, hamming_data0, hamming_data1);

    if(0 == hamming_data0 && 0 == hamming_data1)
    {
        ham_debug("The data inside the eFuse is empty.\n");
        goto set_ddrp_par;
    }


    ham_debug("hamming_data is %x %x, data is: \n", hamming_data0, hamming_data1);
    for (i = 0; i < (dataBits + numParityBits + 1); i++)
    {
        if(i < 32)
        {
            hammingCode[i] = (hamming_data0 >> i) & 0x1;
            ham_debug("%d ", hammingCode[i]);
        }
        else{
            hammingCode[i] = (hamming_data1 >> i) & 0x1;
            ham_debug("%d ", hammingCode[i]);
        }

    }
    ham_debug("\ndataBits is %d\n", dataBits);
    decodedData = check_HammingCode(hammingCode, dataBits, efuse_ddr_data);
    ham_debug("Decoded Data: %x\n", decodedData);

set_ddrp_par:

    set_ddr_par(efuse_ddr_data, ddr_drv_config, par_size);

}
