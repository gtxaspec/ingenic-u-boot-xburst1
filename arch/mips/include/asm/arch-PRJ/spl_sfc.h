#ifndef __SPL_SFC_H__
#define __SPL_SFC_H__

#define	SFC_BASE  (0xB3440000)

#ifndef REG32(addr)
#define REG32(addr)	*((volatile unsigned int *)(addr))
#endif

#define	SFC_GLB                  (SFC_BASE + 0x0000)
#define	SFC_GLB1                 (SFC_BASE + 0x0094)
#define	SFC_DEV_CONF             (SFC_BASE + 0x0004)
#define	SFC_DEV_STA_EXP          (SFC_BASE + 0x0008)
#define	SFC_DEV_STA_RT           (SFC_BASE + 0x000c)
#define	SFC_DEV_STA_MSK          (SFC_BASE + 0x0010)
#define	SFC_TRAN_CONF(n)         (SFC_BASE + 0x0014 + (n * 4))
#define	SFC_TRAN_CONF1(n)        (SFC_BASE + 0x009c + (n * 4))
#define	SFC_TRAN_LEN             (SFC_BASE + 0x002c)
#define	SFC_DEV_ADDR(n)          (SFC_BASE + 0x0030 + (n * 4))
#define	SFC_DEV_ADDR_PLUS(n)     (SFC_BASE + 0x0048 + (n * 4))
#define	SFC_MEM_ADDR             (SFC_BASE + 0x0060)
#define	SFC_TRIG                 (SFC_BASE + 0x0064)
#define	SFC_SR                   (SFC_BASE + 0x0068)
#define	SFC_SCR                  (SFC_BASE + 0x006c)
#define	SFC_INTC                 (SFC_BASE + 0x0070)
#define	SFC_FSM                  (SFC_BASE + 0x0074)
#define	SFC_CGE                  (SFC_BASE + 0x0078)
#define	SFC_RM_DR                (SFC_BASE + 0x1000)

#define TRAN_CONF1_SPI_STANDARD  (0x0)
#define TRAN_CONF1_SPI_DUAL      (0x1)
#define TRAN_CONF1_SPI_QUAD      (0x5)
#define TRAN_CONF1_SPI_IO_QUAD   (0x6)

//For SFC_GLB
#define	TRAN_DIR                 (1 << 13)
#define	THRESHOLD_OFFSET         (7)
#define THRESHOLD_MSK            (0x3f << THRESHOLD_OFFSET)
#define OP_MODE                  (1 << 6)
#define PHASE_NUM_OFFSET         (3)
#define PHASE_NUM_MSK            (0x7 << PHASE_NUM_OFFSET)
#define WP_EN                    (1 << 2)
#define BURST_MD_OFFSET          (0)
#define BURST_MD_MSK             (0x3 << BURST_MD_OFFSET)

//For SFC_GLB1
#define	GLB1_SFC_MODE2           (1 << 3)
#define	GLB1_DQS_EN              (1 << 2)
#define GLB1_CHIP_SEL            (3 << 0)

//For SFC_DEV_CONF
#define	SMP_DELAY_OFFSET         (16)
#define	SMP_DELAY_MSK            (0x3 << SMP_DELAY_OFFSET)
#define CMD_TYPE                 (1 << 15)
#define STA_TYPE_OFFSET          (13)
#define STA_TYPE_MSK             (1 << STA_TYPE_OFFSET)
#define THOLD_OFFSET             (11)
#define	THOLD_MSK                (0x3 << THOLD_OFFSET)
#define TSETUP_OFFSET            (9)
#define TSETUP_MSK               (0x3 << TSETUP_OFFSET)
#define TSH_OFFSET               (5)
#define TSH_MSK                  (0xf << TSH_OFFSET)
#define CPHA                     (1 << 4)
#define CPOL                     (1 << 3)
#define CEDL                     (1 << 2)
#define HOLDDL                   (1 << 1)
#define WPDL                     (1 << 0)

//For SFC_TRAN_CONFx
#define CLK_MODE                 (7 << 29)
#define TRAN_MODE_OFFSET         (29)
#define TRAN_MODE_MSK            (0x7 << TRAN_MODE_OFFSET)
#define ADDR_WIDTH_OFFSET        (26)
#define ADDR_WIDTH_MSK           (0x7 << ADDR_WIDTH_OFFSET)
#define POLLEN                   (1 << 25)
#define CMDEN                    (1 << 24)
#define FMAT                     (1 << 23)
#define DMYBITS_OFFSET           (17)
#define DMYBITS_MSK              (0x3f << DMYBITS_OFFSET)
#define DATEEN                   (1 << 16)
#define CMD_OFFSET               (0)
#define CMD_MSK                  (0xffff << CMD_OFFSET)

// For SFC_TRIG
#define FLUSH                    (1 << 2)
#define STOP                     (1 << 1)
#define START                    (1 << 0)

// For SFC_SR
#define FIFONUM_OFFSET           (16)
#define FIFONUM_MSK              (0x7f << FIFONUM_OFFSET)
#define BUSY_OFFSET              (5)
#define BUSY_MSK                 (0x3 << BUSY_OFFSET)
#define END                      (1 << 4)
#define TRAN_REQ                 (1 << 3)
#define RECE_REQ                 (1 << 2)
#define OVER                     (1 << 1)
#define UNDER                    (1 << 0)

// For SFC_SCR
#define CLR_END                  (1 << 4)
#define CLR_TREQ                 (1 << 3)
#define CLR_RREQ                 (1 << 2)
#define CLR_OVER                 (1 << 1)
#define CLR_UNDER                (1 << 0)

// For SFC_INTC
#define MASK_END                 (1 << 4)
#define MASK_TREQ                (1 << 3)
#define MASK_RREQ                (1 << 2)
#define MASK_OVER                (1 << 1)
#define MASK_UNDR                (1 << 0)

// For SFC_FSM
#define FSM_AHB_OFFSET           (16)
#define FSM_AHB_MSK              (0xf << FSM_AHB_OFFSET)
#define FSM_SPI_OFFSET           (11)
#define FSM_SPI_MSK              (0x1f << FSM_SPI_OFFSET)
#define FSM_CLK_OFFSET           (6)
#define FSM_CLK_MSK              (0xf << FSM_CLK_OFFSET)
#define FSM_DMAC_OFFSET          (3)
#define FSM_DMAC_MSK             (0x7 << FSM_DMAC_OFFSET)
#define FSM_RMC_OFFSET           (0)
#define FSM_RMC_MSK              (0x7 << FSM_RMC_OFFSET)

// For SFC_CGE
#define CG_EN                    (1 << 0)

#define SFC_FIFO_LEN             (63)
#define THRESHOLD                (31)
#endif /* __JZ_SFC_H__ */
