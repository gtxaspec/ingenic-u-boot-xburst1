/*
 * usb_boot.c
 *
 * Internal BootROM c-code for JZ4780B.
 * Supports booting USB device.
 *
 * Copyright (C) 2005 - 2012 Ingenic Semiconductor Inc.
 *
 * Authors: cli <cli@ingenic.cn>
 *
 * Revision history:
 *  - 2013/09/02: modify jz4780B usb_boot base on jz4780 boot. <cli>
 *  - 2012/03/21: Init version from jz4780 usb_boot. <Twxie>
 *  - 2012/04/07: Cut ch9.h included and adjust usb_boot.h. <Twxie>
 *  - 2012/04/14: Adjust code style for usb_boot. <Twxie>
 */
//#define DEBUG
#include <common.h>
#include "usb/otg_dwc2.h"
#include "usb/usb_boot.h"


#ifdef DEBUG
#define DBG(x...) printf(x)
#else
#define DBG(x...) do{}while(0)
#endif

#ifdef DEBUG
#define DBG_HEX(x) printf("%x\n",x)
#else
#define DBG_HEX(x) do{}while(0)
#endif

#ifdef DEBUG
#define DBG_DEC(x) printf("%d\n",x)
#else
#define DBG_DEC(x) do{}while(0)
#endif


static u32 volatile START_NEXT_PHASE_FLAG = 0;

static u32 status_map;
static u32 ddr_is_ok = 0;

enum {
	HIGH_SPEED = 0,
	FULL_SPEED,
	LOW_SPEED
};

enum {
	DWC_DEFAULT = 0,
	DWC_ADDRESS,
	DWC_CONFIG,
};

int enum_done_speed = HIGH_SPEED;

static inline int DEP_EP_MAXPKT_SIZE(int n)
{
	if (n) {
		if (enum_done_speed == HIGH_SPEED)
			return DEP_HS_PKTSIZE;
		else if (enum_done_speed == FULL_SPEED)
			return DEP_FS_PKTSIZE;
		else
			return 0;
	}
	return 64;
}

static void read_out_packet(USB_STATUS *status, u16 count,u32 epnum)
{
	int dwords = (count+3) / 4;
	int i;
/*
	DBG("epnum:");
	DBG_DEC(epnum);
	DBG("count:");
	DBG_DEC(count);
	DBG("dst addr:");
	DBG_HEX((u32)status->addr_out);
*/
	if (epnum == 1 && status->addr_out != 0) {
		for (i = 0; i < dwords; i++)
			REG32((unsigned int *)(status->addr_out) + i) = REG_EP_FIFO(1);
		status->addr_out += (dwords*4);
	} else {
		/*discard out*/
		DBG("discard\n");
		for (i = 0; i < dwords; i++)
			REG_EP_FIFO(epnum);
	}
	return;
}

static void dwc_otg_flush_rx_fifo(void)
{
	REG_GRST_CTL = RSTCTL_RXFIFO_FLUSH;

	DBG("dwc_otg_flush_rx_fifo :\n");
	while (REG_GRST_CTL & RSTCTL_RXFIFO_FLUSH);
	return;
}

static void dwc_otg_flush_tx_fifo(unsigned char txf_num)
{
	unsigned int gintsts;
	unsigned int grstctl;
	gintsts = REG_GINT_STS;

	/*
	 * Step1: Check that GINTSTS.GinNakEff=0 if this
	 *		  bit is cleared then set Dctl.SGNPInNak = 1.
	 *        Nak effective interrupt = H indicating the core
	 *        is not reading from fifo
	 */

	DBG("dwc_otg_flush_tx_fifo :");
	DBG_HEX(txf_num);
	if ((gintsts & GINTSTS_GINNAK_EFF))
	{
		REG_OTG_DCTL |= DCTL_SET_GNPINNAK;

		/* Step2: wait for GINTSTS.GINNakEff=1,which indicates
		 *        the NAK setting has taken effect to all IN endpoints */
		while(!(REG_GINT_STS & GINTSTS_GINNAK_EFF))
			udelay(1);
	}

	/* Step3: wait for ahb master idle state */
	while (!(REG_GRST_CTL & RSTCTL_AHB_IDLE));

	/* Step4: Check that GrstCtl.TxFFlsh=0, if it is 0, then write
	 *        the TxFIFO number you want to flush to GrstCTL.TxFNum*/
	grstctl = REG_GRST_CTL;
	if (!(grstctl & RSTCTL_TXFIFO_FLUSH))
	{
		REG_GRST_CTL |= (txf_num << 6);
	}

	/* Step5: Set GRSTCTL.TxFFlsh=1 and wait for it to clear */
	REG_GRST_CTL |= RSTCTL_TXFIFO_FLUSH;

	while (REG_GRST_CTL & RSTCTL_TXFIFO_FLUSH);

	/* Step6: Set the DCTL.GCNPinNak bit */
	REG_OTG_DCTL |= DCTL_CLR_GNPINNAK;
}

static void dwc_set_in_nak(int epnum)
{
	int  timeout = 5000;

	REG_DIEP_CTL(epnum) |= DEP_SET_NAK;
	do {
		udelay(1);
		if (timeout < 2) {
			DBG("dwc set in nak timeout epnum\n");
			DBG_HEX(epnum);
		}
	} while ( (!(REG_DIEP_INT(epnum) & DEP_INEP_NAKEFF)) && (--timeout > 0));
}

void dwc_disable_in_ep(int epnum)
{
	int  timeout = 100000;

	if (!(REG_DIEP_CTL(epnum) & DEP_ENA_BIT))
		return ;

	DBG("disable in endpoint epnum:\n");
	DBG_HEX(epnum);

	/*step 1 : set nak*/
	dwc_set_in_nak(epnum);

	/*step 2: disable endpoint*/
	REG_DIEP_CTL(epnum) |= DEP_DISENA_BIT;

	do
	{
		udelay(1);
		if (timeout < 2) {
			DBG("dwc disable in ep timeout epnum :");
			DBG_HEX(epnum);
		}
	} while ( (!(REG_DIEP_INT(epnum) & DEP_EPDIS_INT)) && (--timeout > 0));
	REG_DIEP_INT(epnum) = DEP_EPDIS_INT;

	/*step 3: flush tx fifo*/
	dwc_otg_flush_tx_fifo(epnum);

	REG_DIEP_SIZE(epnum) = 0x0;

	/*step 4: clear nak*/
	if (epnum == 1)
		REG_DIEP_CTL(1) |= DEP_CLEAR_NAK;
}

static void dwc_otg_core_reset(void)
{
	u32 greset = 0;
	u32 cnt = 0;

	do {
		udelay(10);
		greset = REG_GRST_CTL;
		if (cnt++ > 100000) {
			DBG("GRESET wait IDLE timeout.\n");
			return;
		}
	} while ((greset & RSTCTL_AHB_IDLE) == 0);

	cnt = 0;

	REG_GRST_CTL = greset | RSTCTL_CORE_RST;
	do {
		greset = REG_GRST_CTL;
		if (cnt++ > 10000 ) {
			DBG("GRESET wait reset timeout.\n");
			return;
		}
		udelay(10);
	} while (greset & RSTCTL_CORE_RST);

	/* wait for 3 phy clocks */
	udelay(100);

}

static void dwc_otg_core_init(void)
{
	u32 gusbcfg;
	u32 reset = 0;

	DBG("dwc_otg_core_init :\n");

	/* AHB config Slave mode ,Unmask globle inter*/
	REG_GAHB_CFG = AHBCFG_GLOBLE_INTRMASK;

	/*Mask RxfvlMsk Intr*/
	REG_GINT_MASK &= ~(GINTSTS_RXFIFO_NEMPTY);

	/*HNP SRP not support , usb2.0 , utmi+, 16bit phy*/
	gusbcfg = REG_GUSB_CFG;
	if (!(gusbcfg | USBCFG_16BIT_PHY) ||
			(gusbcfg | USBCFG_PHY_INF_UPLI)) {
		/*When we change phy config, we must reset core*/
		reset = 1;
	}
	gusbcfg &= ~( USBCFG_HNP_EN | USBCFG_SRP_EN | USBCFG_PHY_SEL_USB1 |
			USBCFG_TRDTIME_MASK | USBCFG_PHY_INF_UPLI);
	gusbcfg |= USBCFG_16BIT_PHY | USBCFG_TRDTIME(5);
	REG_GUSB_CFG = gusbcfg;

	if (reset) {
		dwc_otg_core_reset();
		REG_GAHB_CFG = AHBCFG_GLOBLE_INTRMASK;
	}

	REG_GINT_MASK = GINTSTS_MODE_MISMATCH|GINTSTS_OTG_INTR;
	REG_GINT_STS = GINTSTS_START_FRAM;
}

static void dwc_fifo_allocate(void)
{
	u16 start_addr = 0;
	u16 gdfifocfg;

	/*rx fifo size*/
	REG_GRXFIFO_SIZE = DEP_RXFIFO_SIZE;
	DBG("rx fifo size: \n");
	DBG_HEX(DEP_RXFIFO_SIZE);
	DBG("REG_GRXFIFO_SIZE :\n");
	DBG_HEX(REG_GRXFIFO_SIZE);

	/* txfifo0 size */
	start_addr += DEP_RXFIFO_SIZE;
	REG_GNPTXFIFO_SIZE = (DEP_NPTXFIFO_SIZE << 16) | start_addr;
	DBG("tx fifo 0  size:\n");
	DBG_HEX((DEP_NPTXFIFO_SIZE << 16) | start_addr);
	DBG("REG_GNPTXFIFO_SIZE :\n");
	DBG_HEX(REG_GNPTXFIFO_SIZE);

	/* txfifo1 size */
	start_addr += DEP_NPTXFIFO_SIZE;
	REG_DIEPTXF(1)	= (DEP_DTXFIFO1_SIZE << 16) | start_addr;
	DBG("tx fifo 1 size:");
	DBG_HEX((DEP_DTXFIFO1_SIZE << 16) | start_addr);
	DBG("REG_DIEPTXF(1) :\n");
	DBG_HEX(REG_DIEPTXF(1));

	/* txfifo2 size */
	start_addr += DEP_DTXFIFO1_SIZE;
	REG_DIEPTXF(2)	= (DEP_DTXFIFO2_SIZE << 16) | start_addr;
	DBG("tx fifo 2 size:");
	DBG_HEX((DEP_DTXFIFO2_SIZE << 16) | start_addr);
	DBG("REG_DIEPTXF(2):\n");
	DBG_HEX(REG_DIEPTXF(2));

	/*ep info size*/
	start_addr += DEP_DTXFIFO2_SIZE;
	gdfifocfg = (start_addr << 16) | REG_GHW_CFG3 >> 16;
	REG_GDFIFO_CFG = (start_addr << 16) | gdfifocfg;

	dwc_otg_flush_tx_fifo(0x10);
	dwc_otg_flush_rx_fifo();
}

static void dwc_otg_device_init(void)
{
	DBG("bootrom device init!!!!\n");

	/* dma disable ,High speed , stall no zero handshack*/
	REG_OTG_DCFG = DCFG_HANDSHAKE_STALL_ERR_STATUS;

	/* Soft  connect*/
	REG_OTG_DCTL &= ~DCTL_SOFT_DISCONN;
	REG_OTG_DCTL |= DCTL_NAK_ON_BBLE;

	/* Unmask suspend earlysuspend reset enumdone sof intr*/
	REG_GINT_MASK |= GINTSTS_USB_SUSPEND | GINTSTS_USB_RESET | GINTSTS_ENUM_DONE |
		GINTSTS_USB_EARLYSUSPEND;

	return;
}

void handle_in_transfer(USB_STATUS *status, int epnum)
{
	int mps = DEP_EP_MAXPKT_SIZE(epnum);
	int xfer_size = status->length;

	/*only handle one packet one time*/
	DBG("handle_in_transfer epnum :");
	DBG_HEX(epnum);

	if (REG_DIEP_CTL(epnum) & DEP_ENA_BIT)
		return;

	if (status->addr_in == NULL)
		xfer_size = 0;

	if (mps < xfer_size)
		xfer_size = mps;

	DBG("handle in length left: ");
	DBG_HEX(status->length);
	DBG("transfer size: ");
	DBG_HEX(xfer_size);

	REG_DIEP_SIZE(epnum) = (1 << DXEPSIZE_PKTCNT_BIT) | xfer_size;
	DBG("epnum :");
	DBG_HEX(epnum);
	DBG("REG_DIEP_SIZE :");
	DBG_HEX(REG_DIEP_SIZE(epnum));
	REG_DIEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	if (xfer_size)
		REG_DIEP_EMPMSK |= (1 << epnum);
	return;
}

static void write_tranfer_fifo(USB_STATUS *status, int epnum)
{
	u32 dwords = 0;
	u32 xfer_size = 0;
	u32 txstatus = 0;
	int i;
	int mps = DEP_EP_MAXPKT_SIZE(epnum);

	if (status->length > mps)
		xfer_size = mps;
	else
		xfer_size = status->length;

	dwords = (xfer_size + 3) / 4;
	if (dwords) {
		txstatus = (REG_DIEP_TXFSTS(epnum) & 0xffff);
		if (txstatus >= dwords) {
			DBG("xfer size one time = :");
			DBG_DEC(xfer_size);
			for (i = 0; i < dwords; i++) {
				REG_EP_FIFO(epnum) = REG32((unsigned int *)(status->addr_in)+i);
			}
			status->addr_in += xfer_size;
			status->length -= xfer_size;
			REG_DIEP_EMPMSK &= ~(1 << epnum);
			REG_DIEP_INT(epnum) = DEP_INTOKEN_RECV_TXFIFO_EMPTY|DEP_TXFIFO_EMPTY;
		}
	}
	return;
}

void dwc_udc_init(void)
{
	/*dwc core reset*/
	dwc_otg_core_reset();

	/*dwc core init*/
	dwc_otg_core_init();

	/*dwc fifo allocate*/
	dwc_fifo_allocate();

	/*dwc device init*/
	dwc_otg_device_init();

	DBG("dwc_udc_init ok\n");
	return;
}


static void handle_early_suspend_intr(USB_STATUS *status)
{
	u32 dsts = REG_OTG_DSTS;
	DBG("Handle early suspend intr.\n");
	REG_GINT_STS = GINTSTS_USB_EARLYSUSPEND;
	if (dsts & DSTS_ERRATIC_ERROR) {
		/*handle babble conditional,software reset*/
		memset((int *)status, 0,2*sizeof(USB_STATUS));
		REG_OTG_DCTL |= DCTL_SOFT_DISCONN;
		mdelay(100);
		dwc_udc_init();
	}
	return;
}

void handle_reset_intr(int *config)
{
	DBG("Handle reset intr called \n");

	/* step1 :NAK OUT ep */
	REG_DOEP_CTL(0) |= DEP_SET_NAK;
	REG_DOEP_CTL(1) |= DEP_SET_NAK;

	/* step2: unmask inter */
	REG_DAINT_MASK = DAINT_IEP_MASK(0) | DAINT_OEP_MASK(0);
	REG_DOEP_MASK = DEP_SETUP_PHASE_DONE | DEP_XFER_COMP;
	REG_DIEP_MASK = DEP_XFER_COMP;

	/* step3: device init nothing to do */

	/* step4: dfifo dynamic allocated */
	//dwc_fifo_allocate();

	/* step5: Reset Device Address */
	REG_OTG_DCFG &= ~DCFG_DEV_ADDR_MASK;

	/* step6: setup EP0 to receive SETUP packets */
	REG_DOEP_SIZE(0) = DOEPSIZE0_SUPCNT_3;
	REG_DOEP_CTL(0) |= DEP_ENA_BIT | DEP_CLEAR_NAK;

	REG_GINT_STS = GINTSTS_USB_RESET;

	/* step7: software*/
	*config = DWC_DEFAULT;
	REG_DIEP_EMPMSK = 0;

	dwc_disable_in_ep(0);
	dwc_disable_in_ep(1);
	return;
}	/* handle_reset_intr */

void handle_enum_done_intr(int *flag)
{
	u32 dsts = REG_OTG_DSTS;
	*flag = 1;

	DBG("Handle enum done intr.\n");
	switch(dsts & DSTS_ENUM_SPEED_MASK) {
	case DSTS_ENUM_SPEED_HIGH:
		DBG("High speed.\n");
		enum_done_speed = HIGH_SPEED;
		break;
	case DSTS_ENUM_SPEED_FULL_30OR60:
	case DSTS_ENUM_SPEED_FULL_48:
		DBG("Full speed.\n");
		enum_done_speed = FULL_SPEED;
		break;
	case DSTS_ENUM_SPEED_LOW:
		enum_done_speed = LOW_SPEED;
	default:
		DBG("Fault speed enumration\n");
		return;
	}

	REG_OTG_DCTL |= DCTL_CLR_GNPINNAK;
	REG_DIEP_CTL(0) |= DEP_EP0_MPS_64 | DEP_IN_FIFO_SEL(0);
	REG_DOEP_CTL(0) |= DEP_EP0_MPS_64 | DEP_CLEAR_NAK;
	REG_GINT_MASK |= GINTSTS_RXFIFO_NEMPTY | GINTSTS_IEP_INTR |
		GINTSTS_OEP_INTR | GINTSTS_START_FRAM;
	REG_DOEP_MASK |=  0x601B;
	REG_DIEP_MASK |= 0x2053;
	REG_GINT_STS = GINTSTS_ENUM_DONE;
	return;
}

static void handle_rxfifo_nempty(USB_STATUS *status)
{
	u32 rxsts_pop = REG_GRXSTS_POP;
	u16 count = (rxsts_pop & GRXSTSP_BYTE_CNT_MASK) >> GRXSTSP_BYTE_CNT_BIT;
	u32 epnum = (rxsts_pop & 0xf);

	REG_GINT_MASK &= ~GINTSTS_RXFIFO_NEMPTY;
	switch(rxsts_pop & GRXSTSP_PKSTS_MASK) {
	case GRXSTSP_PKSTS_GOUT_NAK:
		DBG("GRXSTSP_PKSTS_GOUT_NAK.\n");
		break;
	case GRXSTSP_PKSTS_TX_COMP:
		//DBG("GRXSTSP_PKSTS_RX_COMP.\n");
		break;
	case GRXSTSP_PKSTS_GOUT_RECV:
		//DBG("GRXSTSP_PKSTS_GOUT_RECV.\n");
		read_out_packet(&status[epnum], count, epnum);
		break;
	case GRXSTSP_PKSTS_SETUP_COMP:
		DBG("GRXSTSP_PKSTS_SETUP_COMP.\n");
		break;
	case GRXSTSP_PKSTS_SETUP_RECV:
		DBG("GRXSTSP_PKSTS_SETUP_RECV.\n");
		status[epnum].setup_packet[0] = REG_EP_FIFO(epnum);
		status[epnum].setup_packet[1] = REG_EP_FIFO(epnum);
		break;
	default:
		DBG("rxsts_pop :");
		DBG_HEX(rxsts_pop);
		break;
	}

	REG_GINT_STS = GINTSTS_RXFIFO_NEMPTY;
	REG_GINT_MASK |= GINTSTS_RXFIFO_NEMPTY;
	return;
}

static void ep0_in_stall(void)
{
	DBG("ep0 stall\n");
	/*stall ep0*/
	REG_DIEP_CTL(0) |= DEP_SET_STALL;

	/*restart ep0 out*/
	REG_DOEP_SIZE(0) = DOEPSIZE0_SUPCNT_3;
	REG_DOEP_CTL(0) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
}

static void dwc_set_config(USB_STATUS *status)
{
	int max_pkt_size = DEP_EP_MAXPKT_SIZE(1);

	dwc_disable_in_ep(1);
	REG_DOEP_INT(1) = 0xffff;
	REG_DIEP_INT(1) = 0xffff;

	REG_DIEP_CTL(1) = USB_ACTIVE_EP | DEP_TYPE_BULK | max_pkt_size | DEP_RESET_DATA0 | DEP_IN_FIFO_SEL(1);
	REG_DOEP_CTL(1) = USB_ACTIVE_EP | DEP_TYPE_BULK | max_pkt_size | DEP_RESET_DATA0;
	REG_DIEP_SIZE(1) = 0;
	REG_DOEP_SIZE(1) = (1 << 19) | max_pkt_size;
	REG_DOEP_CTL(1) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
	REG_DAINT_MASK  |=  (1 << 1) | (1 << 17);    //umask ep1 intr;
}

static int handle_setup_packet(USB_STATUS *status,int *config)
{
	u32 i;
	u32 addr;
	u32 word1 = status[0].setup_packet[0];
	u32 word2 = status[0].setup_packet[1];
	u32 usb_stall = 0;
	status[0].length = 0;

	if ((word1 & 0x60) == 0x40 && (*config >= DWC_CONFIG)) {
		/* vendor_request handle */
		DBG("Vendor_request case :");
		switch((word1 >> 8) & 0xff) {
		case EP0_GET_CPU_INFO:
			DBG("EP0_GET_CPU_INFO \n");
			dwc_disable_in_ep(1);
			status[0].addr_in = (u8* )cpu_info_data;
			status[0].length = 8;
			break;

		case EP0_SET_DATA_ADDRESS:
			status[1].addr_out = (u8 *)((word1 & 0xffff0000) | (word2 & 0xffff));
			status[1].addr_in = status[1].addr_out;
			DBG("EP0_SET_DATA_ADDRESS. Data Addr = 0x");
			DBG_HEX((u32)status[1].addr_out);
			break;

		case EP0_SET_DATA_LENGTH:
			status[1].length = (word1 & 0xffff0000) | (word2 & 0xffff);
			status[1].fw_len = status[1].length;
			DBG("EP0_SET_DATA_LENGTH. Data length = ");
			DBG_DEC(status[1].length);
			break;
		case EP0_FLUSH_CACHES:
			DBG("flush cache\n");
			flush_cache_all();
			break;
		case EP0_PROG_START1:
			addr = (word1 & 0xffff0000) | (word2 & 0xffff);
			DBG("EP0_PROG_START1. Start addr = 0x");
			DBG_HEX(addr);
			handle_in_transfer(&status[0],0);
			{
				int timeout = 0xffff;
				while ((~(REG_DIEP_INT(0) & DEP_XFER_COMP)) && (timeout--) );
				REG_DIEP_INT(0) = DEP_XFER_COMP;
			}
			if (!ddr_is_ok) {
				int cpu_freq = 0;
				DBG("statge1 length :");
				DBG_HEX(status[1].fw_len);

                /*edwin to do*/                
                /*
				cpu_freq = xfer_d2i(addr, status[1].fw_len);
				DBG_HEX(cpu_freq);
				if (cpu_freq > 0)
					notify_cpufreq_change(cpu_freq);*/
				ddr_is_ok = 1;
			}
			return 0;

		case EP0_PROG_START2:
            DBG("#EP0_PROG_START2.break;START_NEXT_PHASE_FLAG \n");
            
            START_NEXT_PHASE_FLAG = 5;
			break;
#if 0
            addr = ((word1 & 0xffff0000)|(word2 & 0xffff));
			DBG("EP0_PROG_START2. Start addr = 0x");
			DBG_HEX(addr);
			handle_in_transfer(&status[0],0);
			{
				int timeout = 0xffff;
				void (*func_usb)(unsigned int);

				while ((~(REG_DIEP_INT(0) & DEP_XFER_COMP)) && (timeout--) );
				REG_DIEP_INT(0) = DEP_XFER_COMP;

				func_usb = (void (*)(unsigned int)) addr;
				DBG("bootrom start stage 2\n");
				(*func_usb) (addr);
			}
			break;
		default:
			usb_stall = 1;
			break;
#endif
        }
	} else if (((word1 & 0x60) == 0x0)) {
		/* standard device request handle */
		DBG("Standard_request : ");
		switch((word1 >> 8) & 0xff) {
		case USB_REQ_SET_ADDRESS:
			addr = word1 >> 16;
			REG_OTG_DCFG &= ~DCFG_DEV_ADDR_MASK;
			REG_OTG_DCFG |= addr << DCFG_DEV_ADDR_BIT;
			if (addr != 0)
				*config = DWC_ADDRESS;
			else
				*config = DWC_DEFAULT;
			DBG("SET_ADDRESS. Address = ");
			DBG_HEX(addr);
			break;

		case USB_REQ_SET_CONFIGURATION:
			DBG("SET_CONFIGURATION. Configuration = ");
			DBG_DEC(word1 >> 16);
			*config = DWC_CONFIG;
			dwc_set_config(&status[1]);
			break;

		case USB_REQ_GET_STATUS:
			DBG("GET_STATUS\n");
			status[0].length = 2;
			switch (word1 & 0x1f) {
			case 0x2:	/*endpoint*/
			case 0x1:	/*interface*/
				if (!(REG_DOEP_CTL(1) & USB_ACTIVE_EP)) {
					status[0].length = 0;
					usb_stall = 1;
					break;
				}
			case 0x0:	/*device*/
				status[0].addr_in = (u8*)&status_map;	//default map
				break;
			default:
				status[0].length = 0;
				usb_stall = 1;
				break;
			}
			break;
		case USB_REQ_GET_DESCRIPTOR:
			DBG("GET_DESCRIPTOR - ");
			status[0].length = word2 >> 16;
			switch(word1 >> 24) {
			case USB_DT_DEVICE:
				DBG("DEVICE. \n");
				status[0].addr_in = (u8*)device_desc;
				if (status[0].length > USB_DT_DEVICE_SIZE)
					status[0].length = USB_DT_DEVICE_SIZE;// max length of device_desc
				break;

			case USB_DT_CONFIG:
				DBG("CONFIG. \n");
				if ((REG_OTG_DSTS & DSTS_ENUM_SPEED_MASK) == DSTS_ENUM_SPEED_HIGH)
					status[0].addr_in = (u8 *)hs_desc;
				else
					status[0].addr_in = (u8 *)fs_desc;
				if (status[0].length > 32)
					status[0].length = 32;// max length of device_desc
				break;

			case USB_DT_STRING:
				DBG("STRING. \n");
				i = (word1 >>16) & 0xff;
				if (i == 1) {
					status[0].addr_in = (u8 *)string_manufacture;
					if (status[0].length > 16)
						status[0].length = 16;
				} else if (i == 2) {
					status[0].addr_in = (u8 *)string_product;
					if (status[0].length > 46)
						status[0].length = 46;
				} else {
					status[0].addr_in = (u8 *)string_lang_ids;
					if (status[0].length > 4)
						status[0].length = 4;
				}
				break;

			case USB_DT_DEVICE_QUALIFIER:
				DBG("DEVICE_QUALIFIER. \n");
				status[0].addr_in = (u8 *)dev_qualifier;
				if (status[0].length > 10)
					status[0].length = 10;// max length of device_desc
				break;
			default:
				DBG("Not contain.\n");
				status[0].length = 0;
				usb_stall = 1;
				break;
			}
			break;

		default:
			DBG("Not contain.\n");
			usb_stall = 1;
			break;
		}
	} else {
		DBG("class request,reserve request will be stall\n");
		usb_stall = 1;
	}

	if (!usb_stall) {
		handle_in_transfer(&status[0],0);
	} else {
		ep0_in_stall();
	}
	return 0;
}

static void handle_outep_intr(USB_STATUS *status,int *config)
{
	u32 ep_intr, intr;
	int epnum = 0;

	ep_intr = (REG_OTG_DAINT & 0x30000) >> 16;

	while (ep_intr) {
		if (ep_intr & 0x1) {
			intr = REG_DOEP_INT(epnum);

			if (intr & DEP_XFER_COMP) {
				REG_DOEP_INT(epnum) = DEP_XFER_COMP;
				if (epnum) {
					//DBG("out xfer complex:");
					REG_DOEP_SIZE(epnum) = (1 << 19) | DEP_EP_MAXPKT_SIZE(1);
				} else {
					//DBG("Handle epnum 0 xfer complex:");
					REG_DOEP_SIZE(0) = DOEPSIZE0_SUPCNT_3;
				}
				//DBG("epnum=%d",epnum);
				REG_DOEP_CTL(epnum) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
			}

			if (intr & DEP_SETUP_PHASE_DONE) {
				DBG("Handle setup done inter:");
				DBG_HEX(epnum);
				if (!epnum) {
					REG_DOEP_INT(0) = DEP_SETUP_PHASE_DONE;
					if (!(REG_DOEP_CTL(0) & DEP_ENA_BIT)) {
						REG_DOEP_SIZE(0) = DOEPSIZE0_SUPCNT_3;
						REG_DOEP_CTL(0) |= DEP_ENA_BIT | DEP_CLEAR_NAK;
					}
					handle_setup_packet(status,config);
				}
			}

			if (intr & DEP_EPDIS_INT) {
				DBG("out disabled.\n");
				REG_DOEP_INT(epnum) = DEP_EPDIS_INT;
			}

			if (intr & DEP_BABBLE_ERR_INT) {
				DBG("out DEP_BABBLE_ERR_INT.\n");
				REG_DOEP_INT(epnum) = DEP_BABBLE_ERR_INT;
			}

			if (intr & DEP_NAK_INT) {
				//DBG("out DEP_NAK_INT.\n");
				REG_DOEP_INT(epnum) = DEP_NAK_INT;
			}

			if (intr & DEP_NYET_INT) {
				DBG("out DEP_NYET_INT.\n");
				REG_DOEP_INT(epnum) = DEP_NYET_INT;
			}
		}
		epnum++;
		ep_intr >>= 1;
	}
	return;
}

static void handle_inep_intr(USB_STATUS *status)
{
	u32 ep_intr;
	int epnum = 0;

	ep_intr = (REG_OTG_DAINT & 0x3);
	while (ep_intr) {
		if (ep_intr & 0x1) {
			u32 intr = REG_DIEP_INT(epnum);
			if (intr & DEP_XFER_COMP) {
				DBG("in xfer complex:");
				DBG_DEC(epnum);
				REG_DIEP_INT(epnum) = DEP_XFER_COMP;
			}

			if (intr & DEP_EPDIS_INT)
				REG_DIEP_INT(epnum) = DEP_EPDIS_INT;

			if (intr & DEP_NAK_INT)
				REG_DIEP_INT(epnum) = DEP_NAK_INT;

			if (intr & DEP_NYET_INT)
				REG_DIEP_INT(epnum) = DEP_NYET_INT;

			if (intr & DEP_INTOKEN_RECV_TXFIFO_EMPTY) {
				DBG("DEP_INTOKEN_RECV_TXFIFO_EMPTY: ");
				DBG_HEX(epnum);
				REG_DIEP_INT(epnum) = DEP_INTOKEN_RECV_TXFIFO_EMPTY;
				if (epnum == 1 && !(REG_DIEP_EMPMSK & (1 << epnum)))
					handle_in_transfer(&status[1],1);
			}

			if ((intr & DEP_TXFIFO_EMPTY) && (REG_DIEP_EMPMSK & (1 << epnum))) {
				DBG("in fifo empty:");
				DBG_DEC(epnum);
				write_tranfer_fifo(&status[epnum], epnum);
			}
		}
		epnum++;
		ep_intr >>= 1;
	}
	return ;
}

#if 0
static void dwc_phy_init(unsigned cpu_freq,enum pll_type pll)
{
	DBG("bootrom cpm init!!!!\n");

	/*This bus selects the reference clock frequency
	 *101: 24MHz 010: 12MHz*/
	REG_CPM_USBPCR1 &= ~(0x1f << 23);
	REG_CPM_USBPCR1 |= (0x2 << 26);			//The PLL uses CLKCORE as reference,must be 10b for T10
	if (jz_extal == 12000000) {
		//12M EXTAL
		REG_CPM_USBPCR1 |= (2 << 23);
	} else { //default 24M EXTAL
		REG_CPM_USBPCR1 |= (5 << 23);
	}

	REG_CPM_USBPCR &= ~(1 << 31);					//usb otg phy force device mode
	REG_CPM_USBPCR |= USBPCR_VBUSVLDEXTSEL | USBPCR_VBUSVLDEXT;	//set vbus sign to 1
	/*select 16bit 30M phy*/
	REG_CPM_USBPCR1 |= (1 << 19);

	/*enable otg phy*/
	/*REG_CPM_OPCR |= OPCR_OTGPHY_ENABLE;*/

	REG_CPM_SRBC0 |= CPMSRBC0_OTG1_SR;
	REG_CPM_USBPCR |= USBPCR_POR;
	REG_CPM_USBRDT &= ~USBPCR_INCRM;
	udelay(5);
	REG_CPM_USBPCR &= ~USBPCR_POR;

	udelay(10);
	REG_CPM_OPCR |= OPCR_SPENDN0;

	udelay(600);
	REG_CPM_USBRDT |= USBPCR_INCRM;

	udelay(10);
	REG_CPM_SRBC0 &= ~CPMSRBC0_OTG1_SR;

	REG_CPM_USBPCR1 |= ((1 << 8) | (1 << 9) |  (1 << 28) | (1 << 29) | (1 << 30));

#ifndef FPGA_TEST
	__cpm_start_otg();		//open otg clock gate
#endif	/*nFPGA_TEST*/
}

int udc_init(void)
{
	enum pll_type pll = APLL;

	/*stop wdt when usb or cdc boot*/
	__tcu_stop_wdt_clock();

	/*set cpu_freq = 576000000
	 *nf = 72
	 *nr = 1
	 *od1 = 3
	 *od0 = 1
	 */
	set_cpufreq(72, 1, 3, 1, pll);

	/*cpm phy init*/
	dwc_phy_init(cpufreq,pll);

	/*dump hardware cfg*/
	DBG_HEX(REG_GHW_CFG1);
	DBG_HEX(REG_GHW_CFG2);
	DBG_HEX(REG_GHW_CFG3);
	DBG_HEX(REG_GHW_CFG4);

	/*dwc init*/
	dwc_udc_init();

	/*Start judge if an inadvertently touch*/
	xtimeout(0, 1);

	return 0;
}
#endif

#define REG_CPM_CPCCR           REG32(CPM_CPCCR)
#define REG_CPM_USBPCR          REG32(CPM_USBPCR)
#define REG_CPM_USBRDT          REG32(CPM_USBRDT)
#define REG_CPM_USBPCR1         REG32(CPM_USBPCR1)
#define REG_CPM_CPCSR           REG32(CPM_CPCSR)
#define REG_CPM_SRBC0           REG32(CPM_SRBC0)
#define REG_CPM_LCR             REG32(CPM_LCR)
#define REG_CPM_CLKGR0          REG32(CPM_CLKGR0)
#define REG_CPM_OPCR            REG32(CPM_OPCR)
#define REG_CPM_CLKGR           REG32(CPM_CLKGR0)
#define REG_CPM_CPAPCR          REG32(CPM_CPAPCR)
#define REG_CPM_CPMPCR          REG32(CPM_CPMPCR)

enum pll_type {
	APLL = 0,
	MPLL,
};
unsigned int volatile jz_extal = 24000000;
unsigned int volatile cpufreq = 24000000 / 1000000;

    void notify_cpufreq_change(unsigned int speed)
    {
        cpufreq = speed;
    }
    void set_cpufreq(unsigned int cpu_freq_nf, unsigned int cpu_freq_nr, unsigned int cpu_freq_od1, unsigned int cpu_freq_od0, enum pll_type pll_sel)
    {
#ifndef FPGA_TEST
        unsigned int volatile fout = 0;
        unsigned int tmp = 0;
        fout = jz_extal * cpu_freq_nf / cpu_freq_nr / cpu_freq_od0 / cpu_freq_od1;
        /*pll init*/
        if (!pll_init(cpu_freq_nf/*m*/, cpu_freq_nr/*n*/, cpu_freq_od1, cpu_freq_od0, pll_sel)) {
            /* cpu freq init
             *  PCLK : [19:16] H2CLK: [15:12] H0CLK: [11:8] L2CCLK:[7:4] CCLK [3:0]
             *  8:4:4:2:1
             */
            tmp = REG_CPM_CPCCR & 0xff800000;
            tmp |= (1 << 22) | (1 << 21) | (1 << 20) |
                (7 << 16) | (3 << 12) | (3 << 8) | (1 << 4);
            REG_CPM_CPCCR = tmp;    //add delay
            while(REG_CPM_CPCSR & 0x7);
    
            tmp = REG_CPM_CPCCR;
            tmp &= ~(0xff8 << 20);
            if (pll_sel == APLL)        // select apll
                tmp |= (1 << 24) | (1 << 26) | (1 << 28) | (2 << 30);
            else                // select mpll
                tmp |= (2 << 24) | (2 << 26) | (2 << 28) | (1 << 30);
            REG_CPM_CPCCR = tmp;
            notify_cpufreq_change((fout/1000000));
        }
#endif
        return;
    }

#define CLKGR0_OTG			BIT3
#define CPMSRBC0_OTG1_SR		BIT12
#define __cpm_start_otg()	(REG_CPM_CLKGR0 &= ~CLKGR0_OTG)

    
static void dwc_phy_init(unsigned cpu_freq,enum pll_type pll)
{
    DBG("bootrom cpm init!!!!\n");

    /*This bus selects the reference clock frequency
     *101: 24MHz 010: 12MHz*/
    REG_CPM_USBPCR1 &= ~(0x1f << 23);
    REG_CPM_USBPCR1 |= (0x2 << 26);         //The PLL uses CLKCORE as reference,must be 10b for T10
    if (jz_extal == 12000000) {
        //12M EXTAL
        REG_CPM_USBPCR1 |= (2 << 23);
    } else { //default 24M EXTAL
        REG_CPM_USBPCR1 |= (5 << 23);
    }

    REG_CPM_USBPCR &= ~(1 << 31);                   //usb otg phy force device mode
    REG_CPM_USBPCR |= USBPCR_VBUSVLDEXTSEL | USBPCR_VBUSVLDEXT; //set vbus sign to 1
    /*select 16bit 30M phy*/
    REG_CPM_USBPCR1 |= (1 << 19);

    /*enable otg phy*/
    /*REG_CPM_OPCR |= OPCR_OTGPHY_ENABLE;*/

    REG_CPM_SRBC0 |= CPMSRBC0_OTG1_SR;
    REG_CPM_USBPCR |= USBPCR_POR;
    REG_CPM_USBRDT &= ~USBPCR_INCRM;
    udelay(5);
    REG_CPM_USBPCR &= ~USBPCR_POR;

    udelay(10);
    REG_CPM_OPCR |= OPCR_SPENDN0;

    udelay(600);
    REG_CPM_USBRDT |= USBPCR_INCRM;

    udelay(10);
    REG_CPM_SRBC0 &= ~CPMSRBC0_OTG1_SR;

    REG_CPM_USBPCR1 |= ((1 << 8) | (1 << 9) |  (1 << 28) | (1 << 29) | (1 << 30));

#ifndef FPGA_TEST
    __cpm_start_otg();      //open otg clock gate
#endif	/*nFPGA_TEST*/
}



int udc_init(void)
{
	enum pll_type pll = APLL;

	/*stop wdt when usb or cdc boot*/
	//__tcu_stop_wdt_clock();

	/*set cpu_freq = 576000000
	 *nf = 72
	 *nr = 1
	 *od1 = 3
	 *od0 = 1
	 */
	set_cpufreq(72, 1, 3, 1, pll);

	/*cpm phy init*/
	dwc_phy_init(cpufreq,pll);

	/*dump hardware cfg*/
	DBG_HEX(REG_GHW_CFG1);
	DBG_HEX(REG_GHW_CFG2);
	DBG_HEX(REG_GHW_CFG3);
	DBG_HEX(REG_GHW_CFG4);

	/*dwc init*/
	dwc_udc_init();

	/*Start judge if an inadvertently touch*/
	//xtimeout(0, 1);

	return 0;
}

/*
 * Macros to access the system control coprocessor
 */

#define __read_32bit_c0_register(source, sel)				\
({ volatile int __res;							\
	if (sel == 0)							\
		__asm__ __volatile__(					\
			"mfc0\t%0, " #source "\n\t"			\
			: "=r" (__res));				\
	else								\
		__asm__ __volatile__(					\
			".set\tmips32\n\t"				\
			"mfc0\t%0, " #source ", " #sel "\n\t"		\
			".set\tmips0\n\t"				\
			: "=r" (__res));				\
	__res;								\
})
        
#define __write_32bit_c0_register(register, sel, value)			\
        do {                                    \
            if (sel == 0)                           \
                __asm__ __volatile__(                   \
                    "mtc0\t%z0, " #register "\n\t"          \
                    : : "Jr" ((unsigned int)(value)));      \
            else                                \
                __asm__ __volatile__(                   \
                    ".set\tmips32\n\t"              \
                    "mtc0\t%z0, " #register ", " #sel "\n\t"    \
                    ".set\tmips0"                   \
                    : : "Jr" ((unsigned int)(value)));      \
        } while (0)

#define get_perfcnt_cfg()	__read_32bit_c0_register($25, 0)
#define set_perfcnt_cfg(val)	__write_32bit_c0_register($25, 0, val)
#define get_perfcnt_cnt()	__read_32bit_c0_register($25, 1)
#define set_perfcnt_cnt(val)	__write_32bit_c0_register($25, 1, val)
#define	PERFCNT_M_BIT		(31)
#define PERFCNT_IMPL_BIT	(25)
#define PERFCNT_PCTD_BIT	(15)
#define PERFCNT_EVENTEXT_BIT	(11)
#define PERFCNT_EVENTEXT_MSK	(0xf << PERFCNT_EVENTEXT_BIT)
#define PERFCNT_EVENT_BIT	(5)
#define PERFCNT_EVENT_MSK	(0x3f << PERFCNT_EVENT_BIT)
#define PERFCNT_IE_BIT		(4)
#define PERFCNT_U_BIT		(3)
#define PERFCNT_S_BIT		(2)
#define PERFCNT_K_BIT		(1)
#define PERFCNT_EXL_BIT		(0)

#define perfcnt_clear_cnt() do {		\
		set_perfcnt_cnt(0);		\
	} while(0)

#define perfcnt_kernel_start() do {			\
		unsigned int reg_tmp;			\
		reg_tmp = get_perfcnt_cfg();		\
		reg_tmp |= (1 << PERFCNT_K_BIT);	\
		set_perfcnt_cfg(reg_tmp);		\
	} while(0)

#define perfcnt_kernel_stop() do {			\
		unsigned int reg_tmp;			\
		reg_tmp = get_perfcnt_cfg();		\
		reg_tmp &= ~(1 << PERFCNT_K_BIT);	\
		set_perfcnt_cfg(reg_tmp);		\
	} while(0)

#define perfcnt_init_cfg(event)	do {			\
		unsigned int reg_tmp;			\
		reg_tmp = 0x0;				\
		reg_tmp |= (1 << PERFCNT_PCTD_BIT);	\
		reg_tmp |= (event) << PERFCNT_EVENT_BIT;\
		set_perfcnt_cfg(reg_tmp);		\
		perfcnt_clear_cnt();			\
	} while (0)

#define perfcnt_prepare(event) do {			\
		perfcnt_init_cfg(event);		\
	} while(0)


#define pmon_stop()	perfcnt_kernel_stop()
#define pmon_start()	perfcnt_kernel_start()
#define pmon_clear_cnt()	perfcnt_clear_cnt()
#define pmon_prepare(event)	perfcnt_prepare(event)
#define get_pmon_rc()	get_perfcnt_cnt()

int xtimeout(unsigned int usec, int ret)
{
	volatile unsigned int rc;

	if (ret) {
		pmon_stop();
		pmon_clear_cnt();
		pmon_start();
		return 0;
	} else {
		rc = get_pmon_rc();
		if (rc>= (cpufreq*usec)) {
			pmon_stop();
			pmon_clear_cnt();
			return 0;
		} else
			return 1;
	}
}

int burn_boot(void)
{
	int usb_reset_flag = 0;
	int config = DWC_DEFAULT;
	u32 intsts;
	USB_STATUS status[2];

	ddr_is_ok = 0;
	status_map = 0;
	enum_done_speed = HIGH_SPEED;

	memset((int *)status, 0,2*sizeof(USB_STATUS));

	dwc_udc_init();

	/* Main loop of polling the usb commands */
	while (1) {
        if(5 == START_NEXT_PHASE_FLAG){
            printf("exit usb burner\n");
            break;
        }
#if 1
		if (!(usb_reset_flag)) {
			/* Reset doesn't receive from host
			   May be an inadvertently operation*/
			if (!xtimeout(10 * 1000 * 1000, 0)) {
				DBG("*! USB idle more than 10s !*\n\n");
				return -1;
			}
		} else {
			pmon_clear_cnt();
			pmon_stop();
		}
#endif
		intsts = REG_GINT_STS;

		if ((intsts & GINTSTS_USB_EARLYSUSPEND & REG_GINT_MASK))
			handle_early_suspend_intr(status);

		if ((intsts & GINTSTS_USB_RESET) & REG_GINT_MASK)
			handle_reset_intr(&config);

		if ((intsts & GINTSTS_ENUM_DONE) & REG_GINT_MASK)
			handle_enum_done_intr(&usb_reset_flag);

		if ((intsts & GINTSTS_IEP_INTR) & REG_GINT_MASK)
			handle_inep_intr(status);

		if ((intsts & GINTSTS_OEP_INTR) & REG_GINT_MASK)
			handle_outep_intr(status,&config);

		if ((intsts & GINTSTS_RXFIFO_NEMPTY) & REG_GINT_MASK)
			handle_rxfifo_nempty(status);

	}
	return 0;
}	/* usb_boot end */

int usb_burn_firmware(void)
{
	int ret = 0;

    printf("start usb burner\n");
    ret = burn_boot();

	return ret;
}

