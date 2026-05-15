/*
 * usb_boot.h
 *
 * Internal BootROM c-code for JZ4780B.
 * Supports booting USB device.
 *
 * Copyright (C) 2005 - 2012 Ingenic Semiconductor Inc.
 *
 * Authors: cli <cli@ingenic.cn>
 */
#ifndef __USB_BOOT_H__
#define __USB_BOOT_H__

/* Standard requests, for the bRequest field of a SETUP packet. */
#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
#define USB_REQ_SET_FEATURE		0x03
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B
#define USB_REQ_SYNCH_FRAME		0x0C

/* Vendor requests. */
#define EP0_GET_CPU_INFO		0x00
#define EP0_SET_DATA_ADDRESS		0x01
#define EP0_SET_DATA_LENGTH		0x02
#define EP0_FLUSH_CACHES		0x03
#define EP0_PROG_START1			0x04
#define EP0_PROG_START2			0x05
#define EP0_BOOTROM_TEST		0x07
#define EP0_QUERY_PHASE 		0x08

/* Descriptor types ... USB 2.0 spec table 9.5 */
#define USB_DT_DEVICE			0x01
#define USB_DT_CONFIG			0x02
#define USB_DT_STRING			0x03
#define USB_DT_INTERFACE		0x04
#define USB_DT_ENDPOINT			0x05
#define USB_DT_DEVICE_QUALIFIER		0x06
#define USB_DT_OTHER_SPEED_CONFIG	0x07
#define USB_DT_INTERFACE_POWER		0x08
/* these are from a minor usb 2.0 revision (ECN) */
#define USB_DT_OTG			0x09
#define USB_DT_DEBUG			0x0a
#define USB_DT_INTERFACE_ASSOCIATION	0x0b


#define USB_DT_DEVICE_SIZE		18
#define USB_DT_CONFIG_SIZE		9

#define USBPCR_INCRM            	BIT27   /* INCR_MASK bit */
#define USBPCR_COMMONONN        	BIT25
#define USBPCR_VBUSVLDEXT       	BIT24
#define USBPCR_VBUSVLDEXTSEL    	BIT23
#define USBPCR_POR              	BIT22
#define USBPCR_SIDDQ            	BIT21
#define USBPCR_OTG_DISABLE      	BIT20

#define OPCR_SPENDN0			BIT7
#define OPCR_SPENDN1			BIT6
#define OPCR_O1SE				BIT4
#define OPCR_BUS_MODE			BIT1
/*
 * CPM registers offset address definition
 */
#define CPM_CPCCR_OFFSET        (0x00)  /* T23, rw, 32, 0x55000000 */
#define CPM_LCR_OFFSET          (0x04)  /* T23, rw, 32, 0x00000000 */
#define CPM_CLKGR0_OFFSET       (0x20)  /* T23, rw, 32, 0x1FFFFF80 */
#define CPM_OPCR_OFFSET         (0x24)  /* T23, rw, 32, 0x00001500 */
#define CPM_USBPCR_OFFSET       (0x3c)  /* T23, rw, 32, 0x409919b8 */
#define CPM_USBRDT_OFFSET       (0x40)	/* T23, rw, 32, 0x00000004 */
#define CPM_USBVBFIL_OFFSET     (0x44)	/* T23, rw, 32, 0x00ff0080 */
#define CPM_USBPCR1_OFFSET      (0x48)  /* T23, rw, 32, 0x8a800090 */
#define CPM_CPCSR_OFFSET		(0xd4)	/* T23, rw, 32, 0x00000000 */
#define CPM_SRBC0_OFFSET		(0xc4)	/* T23, rw, 32, 0x00000000 */
#define CPM_CPAPCR_OFFSET       (0x10)  /* T23, rw, 32, 0x06305040 */
#define CPM_CPMPCR_OFFSET       (0x14)  /* T23, rw, 32, 0x06305040 */
#define CPM_DDRCDR_OFFSET		(0x2c)	/* T23, rw, 32, 0x40000000 */

/*
 * CPM registers address definition
 */
#define CPM_CPCCR        	(CPM_BASE + CPM_CPCCR_OFFSET)		/* T23 */
#define CPM_LCR          	(CPM_BASE + CPM_LCR_OFFSET)			/* T23 */
#define CPM_CLKGR0       	(CPM_BASE + CPM_CLKGR0_OFFSET)		/* T23 */
#define CPM_OPCR         	(CPM_BASE + CPM_OPCR_OFFSET)		/* T23 */
#define CPM_USBPCR       	(CPM_BASE + CPM_USBPCR_OFFSET)		/* T23 */
#define CPM_USBRDT       	(CPM_BASE + CPM_USBRDT_OFFSET)		/* T23 */
#define CPM_USBPCR1      	(CPM_BASE + CPM_USBPCR1_OFFSET)		/* T23 */
#define CPM_CPCSR        	(CPM_BASE + CPM_CPCSR_OFFSET)		/* T23 */
#define CPM_SRBC0        	(CPM_BASE + CPM_SRBC0_OFFSET)		/* T23 */
#define CPM_CPAPCR       	(CPM_BASE + CPM_CPAPCR_OFFSET)		/* T23 */
#define CPM_CPMPCR       	(CPM_BASE + CPM_CPMPCR_OFFSET)		/* T23 */
#define CPM_DDRCDR			(CPM_BASE + CPM_DDRCDR_OFFSET)		/* T23 */


/*---------------------------------------------------------*/
#define NULL 0
typedef struct usb_status {
	u8 *addr_in;		// save of operate data address
	u8 *addr_out;		// save of operate data address
	u32 setup_packet[2];
	u32 length;		// save of operate data length
	u32 fw_len;		// save firmware length
} USB_STATUS;

/*---------------------------------------------------------*/
static const u8 device_desc[] = {	0x12, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
				0x08, 0xa1, 0x09, 0xc3, 0x00, 0x01, 0x01, 0x02,
				0x00, 0x01					};

static const u8 dev_qualifier[] = {	0x0a, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
				0x01, 0x00					};

static const u8 hs_desc[] = {
			0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80,0xFA,
			0x09, 0x04, 0x00, 0x00, 0x02, 0xff, 0x00,0x00, 0x00,
			0x07, 0x05, 0x01, 0x02, 0x00, 0x02,0x00,
			0x07, 0x05, 0x81, 0x02, 0x00, 0x02, 0x00	};

static const u8 fs_desc[] = {
			0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0x80,0xFA,
			0x09, 0x04, 0x00, 0x00, 0x02, 0xff, 0x00,0x00, 0x00,
			0x07, 0x05, 0x01, 0x02, 0x40, 0x00,0x00,
			0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00	};

static const u8 string_lang_ids[] = {0x04, 0x03, 0x09, 0x04};

static const u8 string_manufacture[] = {	0x10, 0x03, 0x49, 0x00, 0x6e, 0x00, 0x67, 0x00,
					0x65, 0x00, 0x6e, 0x00, 0x69, 0x00, 0x63, 0x00	};

static const u8 string_product[] = {
				0x26,//length
				0x03, //type
				0xc3, 0x00, // magic
				0x09, 0x00, // magic
				0x20, 0x00,
				0x55, 0x00, // U
				0x53, 0x00, // S
				0x42, 0x00, // B
				0x20, 0x00,
				0x42, 0x00, // B
				0x6f, 0x00, // o
				0x6f, 0x00, // o
				0x74, 0x00, // t
				0x20, 0x00,
				0x44, 0x00, // D
				0x65, 0x00, // e
				0x76, 0x00, // v
				0x69, 0x00, // i
				0x63, 0x00, // c
				0x65, 0x00};// e

static const u8 cpu_info_data[] = "T 3 1 V 1";

#endif  /* __USB_BOOT_H__ */
