#ifndef __T5_OTG_DWC2_H__
#define __T5_OTG_DWC2_H__

#define OTG_BASE	0xb3500000					/* T5 */

/* Globle Regs define */
#define GOTG_CTL		(OTG_BASE + 0x00)			/* T5 */
#define GOTG_INTR		(OTG_BASE + 0x04)			/* T5 */
#define GAHB_CFG		(OTG_BASE + 0x08)			/* T5 */
#define GUSB_CFG		(OTG_BASE + 0x0c)			/* T5 */
#define GRST_CTL		(OTG_BASE + 0x10)			/* T5 */
#define GINT_STS		(OTG_BASE + 0x14)			/* T5 */
#define GINT_MASK		(OTG_BASE + 0x18)			/* T5 */
#define GRXSTS_READ		(OTG_BASE + 0x1c)			/* T5 */
#define GRXSTS_POP		(OTG_BASE + 0x20)			/* T5 */
#define GRXFIFO_SIZE		(OTG_BASE + 0x24)			/* T5 */
#define GNPTXFIFO_SIZE		(OTG_BASE + 0x28)			/* T5 */
//#define GDTXFIFO_SIZE		(OTG_BASE + 0x104)			/* T5, DPTXFSIZ(0) */
#define GHW_CFG1		(OTG_BASE + 0x44)			/* T5 */
#define GHW_CFG2		(OTG_BASE + 0x48)			/* T5 */
#define GHW_CFG3		(OTG_BASE + 0x4c)			/* T5 */
#define GHW_CFG4		(OTG_BASE + 0x50)			/* T5 */
#define GDFIFO_CFG		(OTG_BASE + 0x5c)			/* T5 */
#define DPTXFSIZ(n)		(OTG_BASE + 0x104 + ((n) - 1) * 0x04)	/* T5, n = 1,2,3...15 */
#define DIEPTXF(n)		(OTG_BASE + 0x104 + ((n) - 1) * 0x04)	/* T5, n = 1,2,3...15 */

#define REG_GOTG_CTL		REG32(GOTG_CTL)
#define REG_GOTG_INTR		REG32(GOTG_INTR)
#define REG_GAHB_CFG		REG32(GAHB_CFG)
#define REG_GUSB_CFG		REG32(GUSB_CFG)
#define REG_GRST_CTL		REG32(GRST_CTL)
#define REG_GINT_STS		REG32(GINT_STS)
#define REG_GINT_MASK		REG32(GINT_MASK)
#define REG_GRXSTS_READ		REG32(GRXSTS_READ)
#define REG_GRXSTS_POP		REG32(GRXSTS_POP)
#define REG_GRXFIFO_SIZE	REG32(GRXFIFO_SIZE)
#define REG_GNPTXFIFO_SIZE	REG32(GNPTXFIFO_SIZE)
#define REG_GDTXFIFO_SIZE	REG32(GDTXFIFO_SIZE)
#define REG_GHW_CFG1		REG32(GHW_CFG1)
#define REG_GHW_CFG2		REG32(GHW_CFG2)
#define REG_GHW_CFG3		REG32(GHW_CFG3)
#define REG_GHW_CFG4		REG32(GHW_CFG4)
#define REG_GDFIFO_CFG		REG32(GDFIFO_CFG)
#define REG_DIEPTXF(n)		REG32(DIEPTXF(n))

/* Device Regs define */
#define EP_FIFO(n)      (OTG_BASE + (n+1)*0x1000)
#define REG_EP_FIFO(n)  REG32(EP_FIFO(n))

#define OTG_DCFG		(OTG_BASE + 0x800)			/* T5 */
#define OTG_DCTL		(OTG_BASE + 0x804)			/* T5 */
#define OTG_DSTS		(OTG_BASE + 0x808)			/* T5 */
#define DIEP_MASK		(OTG_BASE + 0x810)			/* T5 */
#define DOEP_MASK		(OTG_BASE + 0x814)			/* T5 */
#define OTG_DAINT		(OTG_BASE + 0x818)			/* T5 */
#define DAINT_MASK		(OTG_BASE + 0x81c)			/* T5 */
#define DIEP_EMPMSK		(OTG_BASE + 0x834)			/* T5 */

#define DIEP_CTL(n)		(OTG_BASE + (0x900 + (n) * 0x20))	/* T5, n = 1,2,3...15 */
#define DOEP_CTL(n)		(OTG_BASE + (0xb00 + (n) * 0x20))	/* T5, n = 1,2,3...15 */

#define DIEP_INT(n)		(OTG_BASE + (0x908 + (n) * 0x20))	/* T5, n = 0,1,2,3...15 */
#define DOEP_INT(n)		(OTG_BASE + (0xb08 + (n) * 0x20))	/* T5, n = 0,1,2,3...15 */

#define DIEP_SIZE(n)		(OTG_BASE + (0x910 + (n) * 0x20))	/* T5, n = 1,2,3...15 */
#define DOEP_SIZE(n)		(OTG_BASE + (0xb10 + (n) * 0x20))	/* T5, n = 1,2,3...15 */

#define DIEP_TXFSTS(n)		(OTG_BASE + (0x918 + (n) * 0x20))	/* T5, n = 0,1,2,3...15 */

#define REG_OTG_DCFG		REG32(OTG_DCFG)
#define REG_OTG_DCTL		REG32(OTG_DCTL)
#define REG_OTG_DSTS		REG32(OTG_DSTS)
#define REG_DIEP_MASK		REG32(DIEP_MASK)
#define REG_DOEP_MASK		REG32(DOEP_MASK)
#define REG_OTG_DAINT		REG32(OTG_DAINT)
#define REG_DAINT_MASK		REG32(DAINT_MASK)
#define REG_DIEP_EMPMSK		REG32(DIEP_EMPMSK)

#define REG_DIEP_CTL(n)		REG32(DIEP_CTL(n))
#define REG_DOEP_CTL(n)		REG32(DOEP_CTL(n))

#define REG_DIEP_INT(n)		REG32(DIEP_INT(n))
#define REG_DOEP_INT(n)		REG32(DOEP_INT(n))

#define REG_DIEP_SIZE(n)	REG32(DIEP_SIZE(n))
#define REG_DOEP_SIZE(n)	REG32(DOEP_SIZE(n))

#define REG_DIEP_TXFSTS(n)	REG32(DIEP_TXFSTS(n))

/* Regs macro define */
/*************************************************/
/* AHB Configuration Register (GAHBCFG) */
#define AHBCFG_TX_FIFO_EMT_LEVEL	BIT7				/* T5 */
#define AHBCFG_DMA_ENA			BIT5				/* T5 */
#define AHBCFG_GLOBLE_INTRMASK		BIT0				/* T5 */

/* USB Configuration Register (GUSBCFG) */
#define USBCFG_TRDTIME_MASK	(0xf << 10)				/* T5 */
#define USBCFG_TRDTIME(n)	(((n) << 10) & USBCFG_TRDTIME_MASK)	/* T5 */
#define USBCFG_HNP_EN		BIT9					/* T5 */
#define USBCFG_SRP_EN		BIT8					/* T5 */
#define USBCFG_PHY_SEL_USB1	BIT6					/* T5 */
#define USBCFG_PHY_INF_UPLI	BIT4					/* T5 */
#define USBCFG_16BIT_PHY	BIT3
#define USBCFG_TOUT_CAL_MASK	(0x7)					/* T5 */

/* Reset Register (GRSTCTL) */
#define RSTCTL_AHB_IDLE		BIT31					/* T5 */
#define RSTCTL_TXFIFO_FLUSH	BIT5					/* T5 */
#define RSTCTL_RXFIFO_FLUSH	BIT4					/* T5 */
#define RSTCTL_INTK_FLUSH	BIT3					/* T5 */
#define RSTCTL_FRMCNT_RST	BIT2					/* T5 */
#define RSTCTL_CORE_RST		BIT0					/* T5 */

/* Interrupt Register (GINTSTS) */
#define GINTSTS_RSUME_DETE	BIT31					/* T5 */
#define GINTSTS_CONID_STSCHG	BIT28					/* T5 */
#define GINTSTS_RESET_DETE	BIT23					/* T5 */
#define GINTSTS_FETCH_SUSPEND	BIT22					/* T5 */
#define GINTSTS_OEP_INTR	BIT19					/* T5 */
#define GINTSTS_IEP_INTR	BIT18					/* T5 */
#define GINTSTS_EP_MISMATCH	BIT17					/* T5 */
#define GINTSTS_ENUM_DONE	BIT13					/* T5 */
#define GINTSTS_USB_RESET	BIT12					/* T5 */
#define GINTSTS_USB_SUSPEND	BIT11					/* T5 */
#define GINTSTS_USB_EARLYSUSPEND	BIT10				/* T5 */
#define GINTSTS_I2C_INT		BIT9					/* T5 */
#define GINTSTS_GOUTNAK_EFF	BIT7					/* T5 */
#define GINTSTS_GINNAK_EFF	BIT6					/* T5 */
#define GINTSTS_NPTXFIFO_EMPTY	BIT5					/* T5 */
#define GINTSTS_RXFIFO_NEMPTY	BIT4					/* T5 */
#define GINTSTS_START_FRAM	BIT3					/* T5 */
#define GINTSTS_OTG_INTR	BIT2					/* T5 */
#define GINTSTS_MODE_MISMATCH	BIT1					/* T5 */

/* Device Control Register (DCTL) */
#define DCTL_NAK_ON_BBLE    BIT16					/* T5 */
#define DCTL_CLR_GOUTNAK    BIT10					/* T5 */
#define DCTL_SET_GOUTNAK    BIT9					/* T5 */
#define DCTL_CLR_GNPINNAK   BIT8					/* T5 */
#define DCTL_SET_GNPINNAK   BIT7					/* T5 */
#define DCTL_SOFT_DISCONN   BIT1					/* T5 */

/* Device Configuration Register (DCFG) */
#define DCFG_DEV_ADDR_MASK	(0x7f << 4)				/* T5 */
#define DCFG_DEV_ADDR_BIT	4					/* T5, ADDR_LSB */
#define DCFG_HANDSHAKE_STALL_ERR_STATUS	BIT2

/* Device All Endpoints Interrupt Mask Register (DAINTMSK)*/
#define DAINT_IEP_MASK(n)		(0x1 << ((n) & 0xffff))		/* T5, n = 0,1,2,3...15 */
#define DAINT_OEP_MASK(n)		(0x10000 << ((n) & 0xffff))	/* T5, n = 0,1,2,3...15 */

/* Device Status Register (DSTS) */
#define DSTS_ERRATIC_ERROR		BIT3				/* T5 */
#define DSTS_ENUM_SPEED_MASK		(0x3 << 1)			/* T5 */
#define DSTS_ENUM_SPEED_HIGH		(0x0 << 1)			/* T5 */
#define DSTS_ENUM_SPEED_FULL_30OR60	(0x1 << 1)			/* T5 */
#define DSTS_ENUM_SPEED_LOW		(0x2 << 1)			/* T5 */
#define DSTS_ENUM_SPEED_FULL_48		(0x3 << 1)			/* T5 */

/* Receive Status Debug Read/Status Read and Pop Registers (GRXSTSR/GRXSTSP) */
#define GRXSTSP_PKSTS_MASK		(0xf << 17)			/* T5 */
#define GRXSTSP_PKSTS_GOUT_NAK		(0x1 << 17)
#define GRXSTSP_PKSTS_GOUT_RECV		(0x2 << 17)
#define GRXSTSP_PKSTS_TX_COMP		(0x3 << 17)
#define GRXSTSP_PKSTS_SETUP_COMP	(0x4 << 17)
#define GRXSTSP_PKSTS_SETUP_RECV	(0x6 << 17)
#define GRXSTSP_BYTE_CNT_MASK		(0x7ff << 4)			/* T5 */
#define GRXSTSP_BYTE_CNT_BIT		4				/* T5 */

/* DIOEPCTL */
// ep0
#define DEP_EP0_MAXPKET_SIZE	64
#define DEP_EP0_MPS_64		(0x0)
#define DEP_EP0_MPS_32		(0x1)
#define DEP_EP0_MPS_16		(0x2)
#define DEP_EP0_MPS_8		(0x3)

#define DEP_ENA_BIT		BIT31
#define DEP_DISENA_BIT		BIT30
#define DEP_RESET_DATA0		BIT28
#define DEP_SET_NAK		BIT27
#define DEP_CLEAR_NAK		BIT26
#define DEP_IN_FIFO_SEL(n)	(((n) & 0xf) << 22)
#define DEP_SET_STALL		BIT21
#define DEP_TYPE_MASK		(0x3 << 18)
#define DEP_TYPE_CNTL		(0x0 << 18)
#define DEP_TYPE_ISO		(0x1 << 18
#define DEP_TYPE_BULK		(0x2 << 18)
#define DEP_TYPE_INTR		(0x3 << 18)
#define DEP_TYPE(x)		(((x)&0x3) << 18)
#define USB_ACTIVE_EP		BIT15
#define DEP_PKTSIZE_MASK	0x7ff
#define DEP_FS_PKTSIZE		64
#define DEP_HS_PKTSIZE		512

/* DIOEPINT */
#define DEP_SETUP_RECV		BIT15
#define DEP_NYET_INT		BIT14
#define DEP_NAK_INT		BIT13
#define DEP_BABBLE_ERR_INT	BIT12
#define DEP_PKT_DROP_STATUS	BIT11
#define DEP_BNA_INT		BIT9
#define DEP_TXFIFO_UNDRN	BIT8		// Only for INEP
#define DEP_OUTPKT_ERR		BIT8		// Only for OUTEP
#define DEP_TXFIFO_EMPTY	BIT7
#define DEP_INEP_NAKEFF		BIT6		// Only for INEP
#define DEP_B2B_SETUP_RECV	BIT6		// Only for OUTEP0
#define DEP_INTOKEN_EPMISATCH	BIT5		// Only for INEP
#define DEP_SETUP_OUT_DATA_DONE BIT5
#define DEP_INTOKEN_RECV_TXFIFO_EMPTY	BIT4	// Only for INEP
#define DEP_OUTTOKEN_RECV_EPDIS	BIT4		// Only for OUTEP
#define DEP_TIME_OUT		BIT3		// Only for INEP
#define DEP_SETUP_PHASE_DONE	BIT3		// Only for OUTEP0
#define DEP_AHB_ERR		BIT2
#define DEP_EPDIS_INT		BIT1
#define DEP_XFER_COMP		BIT0		// Used by INEP and OUTEP

/* DOEPSIZ0 */
#define DOEPSIZE0_SUPCNT_1	(0x1 << 29)
#define DOEPSIZE0_SUPCNT_2	(0x2 << 29)
#define DOEPSIZE0_SUPCNT_3	(0x3 << 29)
#define DOEPSIZE0_PKTCNT_BIT	BIT19

/* DI/OEPSIZn */
#define DXEPSIZE_PKTCNT_BIT	19

/*RX for all rx endpoint*/
#define DEP_RXFIFO_SIZE		1047	/* (4*1 +6) + (7 + 1)*(512/4 + 1) + 2*2 + 1 fix*/
/*tx 0 for control endpoint*/
#define DEP_NPTXFIFO_SIZE	128	/* (7 + 1)*(64/4) fix*/
/*tx 1 for bulk	endpoint */
#define DEP_DTXFIFO1_SIZE	1024	/* (7 + 1)*(512/4) fix*/
/*tx 3 for interrupt endpoint*/
#define DEP_DTXFIFO2_SIZE	128	/* (7 + 1)*(64/4) fix*/

#endif /* __T5_OTG_DWC2_H__ */
