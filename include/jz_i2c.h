#ifndef __INCLUDE__JZ__I2C__
#define __INCLUDE__JZ__I2C__


#define SMB0 0xb0050000
#define SMB1 0xb0051000
#define SMB2 0xb0052000
#define SMB3 0xb0053000

enum I2C_CHIOCE{
    I2C0 = 0,
    I2C1,
    I2C2,
    I2C3,
    I2C_MAX,
};
/*i2c base addr*/
#define SMB 0xb0050000
#define I2C_BASE(n) (SMB+(n*0x1000))

#define I2C_CTRL        (0x00)
#define I2C_TAR         (0x04)
#define I2C_SAR         (0x08)
#define I2C_DC          (0x10)
#define I2C_SHCNT       (0x14)
#define I2C_SLCNT       (0x18)
#define I2C_FHCNT       (0x1C)
#define I2C_FLCNT       (0x20)
#define I2C_INTST       (0x2C)
#define I2C_INTM        (0x30)
#define I2C_RINTST      (0x34)
#define I2C_RXTL        (0x38)
#define I2C_TXTL        (0x3c)
#define I2C_CINTR       (0x40)
#define I2C_CRXUF       (0x44)
#define I2C_CRXOF       (0x48)
#define I2C_CTXOF       (0x4C)
#define I2C_CRXREQ      (0x50)
#define I2C_CTXABRT     (0x54)
#define I2C_CRXDONE     (0x58)
#define I2C_CACT        (0x5C)
#define I2C_CSTP        (0x60)
#define I2C_CSTT        (0x64)
#define I2C_CGC         (0x68)
#define I2C_ENB         (0x6C)
#define I2C_STA         (0x70)
#define I2C_TXFLR       (0x74)
#define I2C_RXFLR       (0x78)
#define I2C_SDAHD       (0x7C)
#define I2C_TXABRT      (0x80)
#define I2C_DMACR       (0x88)
#define I2C_DMATDLR     (0x8c)
#define I2C_DMARDLR     (0x90)
#define I2C_SDASU       (0x94)
#define I2C_ACKGC       (0x98)
#define I2C_ENSTA       (0x9C)
#define I2C_FLT         (0xA0)

#define REG_I2C_CTRL(n)	    (*(volatile unsigned int *)(I2C_BASE(n) + I2C_CTRL))
#define REG_I2C_TAR(n)		(*(volatile unsigned int *)(I2C_BASE(n) + I2C_TAR))
#define REG_I2C_SAR(n)		(*(volatile unsigned int *)(I2C_BASE(n) + I2C_SAR))
#define REG_I2C_DC(n)		(*(volatile unsigned int *)(I2C_BASE(n) + I2C_DC))
#define REG_I2C_SHCNT(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_SHCNT))
#define REG_I2C_SLCNT(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_SLCNT))
#define REG_I2C_FHCNT(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_FHCNT))
#define REG_I2C_FLCNT(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_FLCNT))
#define REG_I2C_INTST(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_INTST))
#define REG_I2C_INTM(n)	    (*(volatile unsigned int *)(I2C_BASE(n) + I2C_INTM))
#define REG_I2C_RINTST(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_RINTST))
#define REG_I2C_RXTL(n)	    (*(volatile unsigned int *)(I2C_BASE(n) + I2C_RXTL))
#define REG_I2C_TXTL(n)	    (*(volatile unsigned int *)(I2C_BASE(n) + I2C_TXTL))
#define REG_I2C_CINTR(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CINTR))
#define REG_I2C_CRXUF(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CRXUF))
#define REG_I2C_CRXOF(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CRXOF))
#define REG_I2C_CTXOF(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CTXOF))
#define REG_I2C_CRXREQ(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CRXREQ))
#define REG_I2C_CTXABRT(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CTXABRT))
#define REG_I2C_CRXDONE(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CRXDONE))
#define REG_I2C_CACT(n)	    (*(volatile unsigned int *)(I2C_BASE(n) + I2C_CACT))
#define REG_I2C_CSTP(n)	    (*(volatile unsigned int *)(I2C_BASE(n) + I2C_CSTP))
#define REG_I2C_CSTT(n)	    (*(volatile unsigned int *)(I2C_BASE(n) + I2C_CSTT))
#define REG_I2C_CGC(n)		(*(volatile unsigned int *)(I2C_BASE(n) + I2C_CGC))
#define REG_I2C_ENB(n)		(*(volatile unsigned int *)(I2C_BASE(n) + I2C_ENB))
#define REG_I2C_STA(n)		(*(volatile unsigned int *)(I2C_BASE(n) + I2C_STA))
#define REG_I2C_TXFLR(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_TXFLR))
#define REG_I2C_RXFLR(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_RXFLR))
#define REG_I2C_SDAHD(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_SDAHD))
#define REG_I2C_TXABRT(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_TXABRT))
#define REG_I2C_DMACR(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_DMACR))
#define REG_I2C_DMATDLR(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_DMATDLR))
#define REG_I2C_DMARDLR(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_DMARDLR))
#define REG_I2C_SDASU(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_SDASU))
#define REG_I2C_ACKGC(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_ACKGC))
#define REG_I2C_ENSTA(n)	(*(volatile unsigned int *)(I2C_BASE(n) + I2C_ENSTA))
#define REG_I2C_FLT(n)		(*(volatile unsigned int *)(I2C_BASE(n) + I2C_FLT))

/* I2C Control Register (I2C_CTRL) */
#define I2C_CTRL_SLVDIS		(1 << 6)	/* after reset slave is disabled */
#define I2C_CTRL_REST		(1 << 5)
#define I2C_CTRL_MATP		(1 << 4)	/* 1: 10bit address 0: 7bit addressing */
#define I2C_CTRL_SATP		(1 << 3)	/* 1: 10bit address 0: 7bit address */
#define I2C_CTRL_SPDF		(2 << 1)	/* fast mode 400kbps */
#define I2C_CTRL_SPDS		(1 << 1)	/* standard mode 100kbps */
#define I2C_CTRL_MD		    (1 << 0)	/* master enabled */

/* I2C Status Register (I2C_STA) */
#define I2C_STA_SLVACT		(1 << 6)	/* Slave FSM is not in IDLE state */
#define I2C_STA_MSTACT		(1 << 5)	/* Master FSM is not in IDLE state */
#define I2C_STA_RFF		    (1 << 4)	/* RFIFO if full */
#define I2C_STA_RFNE		(1 << 3)	/* RFIFO is not empty */
#define I2C_STA_TFE		    (1 << 2)	/* TFIFO is empty */
#define I2C_STA_TFNF		(1 << 1)	/* TFIFO is not full  */
#define I2C_STA_ACT		    (1 << 0)	/* I2C Activity Status */

/* i2c interrupt status (I2C_RINTST) */
#define I2C_RINTST_IGC           (1 << 11)
#define I2C_RINTST_ISTT          (1 << 10)
#define I2C_RINTST_ISTP          (1 << 9)
#define I2C_RINTST_IACT          (1 << 8)
#define I2C_RINTST_RXDN          (1 << 7)
#define I2C_RINTST_TXABT         (1 << 6)
#define I2C_RINTST_RDREQ         (1 << 5)
#define I2C_RINTST_TXEMP         (1 << 4)
#define I2C_RINTST_TXOF          (1 << 3)
#define I2C_RINTST_RXFL          (1 << 2)
#define I2C_RINTST_RXOF          (1 << 1)
#define I2C_RINTST_RXUF          (1 << 0)

/*i2c interrupt mask status (I2C_INTM) */
#define I2C_INTM_MIGC		(1 << 11)
#define I2C_INTM_MISTT		(1 << 10)
#define I2C_INTM_MISTP		(1 << 9)
#define I2C_INTM_MIACT		(1 << 8)
#define I2C_INTM_MRXDN		(1 << 7)
#define I2C_INTM_MTXABT		(1 << 6)
#define I2C_INTM_MRDREQ		(1 << 5)
#define I2C_INTM_MTXEMP		(1 << 4)
#define I2C_INTM_MTXOF		(1 << 3)
#define I2C_INTM_MRXFL		(1 << 2)
#define I2C_INTM_MRXOF		(1 << 1)
#define I2C_INTM_MRXUF		(1 << 0)


/* I2C DMA Register (I2C_DMACR) */
#define I2C_DMACR_TDEN		(1 << 1)
#define I2C_DMACR_RDEN		(1 << 0)

/* I2C DC Register (I2C_DC) */
#define I2C_DC_REST		    (1 << 10)
#define I2C_DC_STP		    (1 << 9)
#define I2C_DC_READ    		(1 << 8)
#define I2C_DC_WRITE   	    (0 << 8)

#define I2C_ENB_I2CENB		(1 << 0)	/* Enable the i2c */

/* #define I2C_FIFO_LEN		(CONFIG_I2C_FIFO_LEN) */
#define I2C_FIFO_LEN		64
#define TX_LEVEL		    (I2C_FIFO_LEN / 2)
#define RX_LEVEL		    (I2C_FIFO_LEN / 2 - 1)
#define TIMEOUT			    0xff
#define DEBUG_INFO		    2
#define DEBUG_WARN		    1


/* I2C standard mode high count register(I2CSHCNT) */
#define I2CSHCNT_ADJUST(n)      (((n) - 8) < 6 ? 6 : ((n) - 8))
/* I2C standard mode low count register(I2CSLCNT) */
#define I2CSLCNT_ADJUST(n)      (((n) - 1) < 8 ? 8 : ((n) - 1))
/* I2C fast mode high count register(I2CFHCNT) */
#define I2CFHCNT_ADJUST(n)      (((n) - 8) < 6 ? 6 : ((n) - 8))
/* I2C fast mode low count register(I2CFLCNT) */
#define I2CFLCNT_ADJUST(n)      (((n) - 1) < 8 ? 8 : ((n) - 1))

typedef struct i2c_msg{
	unsigned int device_addr;
	unsigned int flag;
//#define I2C_M_RD 0x0001
	unsigned int len;
	unsigned char *buf;
} i2c_msg_t;

static inline unsigned int i2c_readl(unsigned int i2c_ctl,unsigned int offset);
static inline void i2c_writel(unsigned int i2c_ctl,unsigned int offset, unsigned int value);

static void jz_i2c_dma_dump();
static int i2c_set_speed(unsigned int i2c_ctl,int speed);
int jz_i2c_init(int speed);
int jz_i2c_probe();

static int i2c_write_cpu(unsigned int i2c_ctl,struct i2c_msg *i2c);
static void i2c_read_cpu(unsigned int base_addr,struct i2c_msg *i2c);
int  jz_i2c_write(unsigned int i2c_ctl,unsigned char chip, unsigned int addr,
                  int alen, unsigned char *buffer, int len);
int  jz_i2c_read(unsigned int i2c_ctl,unsigned char chip, unsigned int addr,
                 int alen, unsigned char *buffer, int len);

#endif
