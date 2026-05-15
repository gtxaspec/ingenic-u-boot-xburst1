#ifndef __JZ_SDHCI_REGS_H
#define __JZ_SDHCI_REGS_H

#define BIT(nr)         (1UL << (nr))

#define JZ_SDHCI_HOST_CTRL2_R		(0x3e)
#define JZ_SDHCI_HOST_VER4_ENABLE_BIT		BIT(12)

/* Clock Control Register */
#define JZ_SDHCI_PLL_ENABLE_BIT				BIT(3)

#define CPM_MSC0_CLK_R						(0xB0000068)
#define MSC_CLK_H_FREQ						(0x1 << 20)

#endif //__JZ_SDHCI_REGS_H
