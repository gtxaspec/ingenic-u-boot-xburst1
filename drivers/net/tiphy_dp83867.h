#ifndef __TIPHY_DP8367_H__
#define __TIPHY_DP8367_H__

/* TI PHY flags */
#define TI_PHY_IDENTIFIER       0x2000
#define TI_PHY_MODEL            0x230
#define TI_PHY_CR               0xD
#define TI_PHY_PHYCTRL          0x10
#define TI_PHY_CR_SGMII_EN      0x0800
#define TI_PHY_ADDDR            0xE
#define TI_PHY_CFGR2            0x14
#define TI_PHY_SGMIITYPE        0xD3
#define TI_PHY_CFGR2_SGMII_AUTONEG_EN   0x0080
#define TI_PHY_SGMIICLK_EN      0x4000
#define TI_PHY_CR_DEVAD_EN      0x001F
#define TI_PHY_CR_DEVAD_DATAEN      0x4000

/* IEEE PHY Specific definitions */
#define PHY_R0_CTRL_REG     0
#define PHY_R3_PHY_IDENT_REG    3

#define PHY_R0_RESET         0x8000
#define PHY_R0_LOOPBACK      0x4000
#define PHY_R0_ANEG_ENABLE   0x1000
#define PHY_R0_DFT_SPD_MASK  0x2040
#define PHY_R0_DFT_SPD_10    0x0000
#define PHY_R0_DFT_SPD_100   0x2000
#define PHY_R0_DFT_SPD_1000  0x0040
#define PHY_R0_DFT_SPD_2500  0x0040
#define PHY_R0_ISOLATE       0x0400

/* Marvel PHY 88E1111 Specific definitions */
#define PHY_R20_EXTND_CTRL_REG  20
#define PHY_R27_EXTND_STS_REG   27

#define PHY_R20_DFT_SPD_10      0x20
#define PHY_R20_DFT_SPD_100     0x50
#define PHY_R20_DFT_SPD_1000    0x60
#define PHY_R20_RX_DLY      0x80

#define PHY_R27_MAC_CONFIG_GMII      0x000F
#define PHY_R27_MAC_CONFIG_MII       0x000F
#define PHY_R27_MAC_CONFIG_RGMII     0x000B
#define PHY_R27_MAC_CONFIG_SGMII     0x0004

/* Marvel PHY 88E1116R Specific definitions */
#define PHY_R22_PAGE_ADDR_REG   22
#define PHY_PG2_R21_CTRL_REG    21

#define PHY_REG21_10      0x0030
#define PHY_REG21_100     0x2030
#define PHY_REG21_1000    0x0070

#endif // __TIPHY_DP8367_H__
