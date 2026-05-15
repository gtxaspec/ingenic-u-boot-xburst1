/**
 * i2c dma configurations
 */

#include <common.h>
#include <config.h>
#include <spl.h>
#include <spi.h>
#include <asm/io.h>
#include <nand.h>
#include <asm/arch/sfc.h>
#include <asm/arch/spi.h>
#include <asm/arch/clk.h>
#include <asm/arch/gpio.h>
#include <jz_i2c.h>
#define DEBUG_DUMP  0
#define I2C_M_RD	1
#define I2C_SPEED 100

/*I2C used in configuration*/
#define I2C0_GPIOA
/* #define I2C1_GPIOA */
//#define I2C1_GPIOD

//#define I2C2_GPIOB
//#define I2C2_GPIOC

struct i2c_gpio_ctl{
	int grp;
	int func;
	unsigned int pins;
	unsigned int base_addr;
};
struct i2c_gpio_ctl i2c_gpio_init[]={
	/**I2C0**/
#if defined (I2C0_GPIOA)
	{.grp=GPIO_PORT_A, .func=GPIO_FUNC_0, .pins=(0x3<<12), .base_addr=I2C0},
#endif
#if defined (I2C1_GPIOA)
	{.grp=GPIO_PORT_A, .func=GPIO_FUNC_0, .pins=(0x3<<14), .base_addr=I2C1},
#endif
};



/*-----------------------------------------------------------------------
 * Local functions
 */

/*
 * Read from the register related to the current offset
 */
static inline unsigned int i2c_readl(unsigned int i2c_ctl,unsigned int offset)
{
	return readl(I2C_BASE(i2c_ctl) + offset);
}

/*
 * Write to the register related to the current offset
 */
static inline void i2c_writel(unsigned int i2c_ctl,unsigned int offset, unsigned int value)
{
	writel(value, I2C_BASE(i2c_ctl) + offset);
}

/*
 * Display the value of the registers enlisted in
 */
#ifdef DEBUG_DUMP
static void jz_i2c_dump(unsigned int i2c_ctl)
{
#if 1
	printf("dump registers infos\n");
	printf("I2C_CTRL  0x%x\n", i2c_readl(i2c_ctl,I2C_CTRL));
	printf("I2C_TAR  0x%x\n", i2c_readl(i2c_ctl,I2C_TAR));
	printf("I2C_SAR  0x%x\n", i2c_readl(i2c_ctl,I2C_SAR));
	printf("I2C_DC  0x%x\n", i2c_readl(i2c_ctl,I2C_DC));
	printf("I2C_RINTST  0x%x\n", i2c_readl(i2c_ctl,I2C_RINTST));
	printf("end of dump infos\n");
#endif
}
#endif
/*
 * Set speed of data transfer (Read and Write)
 */
static int i2c_set_speed(unsigned int i2c_ctl,int speed)
{
	/*ns*/
	long dev_clk = 100000000/1000;
	long cnt_high = 0;	/* HIGH period count of the SCL clock */
	long cnt_low = 0;	/* LOW period count of the SCL clock */
	long setup_time = 0;
	long hold_time = 0;
	unsigned short tmp;
	int rate = speed;


	if (rate <= 100) {
		tmp = 0x43 | (1<<5);      /* standard speed mode*/
		i2c_writel(i2c_ctl,I2C_CTRL,tmp);
	} else if(rate <= 400){
		tmp = 0x45 | (1<<5);      /* fast speed mode*/
		i2c_writel(i2c_ctl,I2C_CTRL,tmp);
	}else if(rate > 400){
		tmp = 0x47 | (1<<5);      /* high speed mode*/
		i2c_writel(i2c_ctl,I2C_CTRL,tmp);
	}

	/*         high
	 *         ____     ____      ____      ____
	 *  clk __|  | |___|    |____|    |____|    |___
	 *           | | |
	 *           | | |
	 *           |_|_|     _________      ____
	 * data    __/ | |\___/         \____/    \____
	 *    setup->| |<|
	 *           ->| |<-hold
	 */

	setup_time = (dev_clk / (rate * 4));
	if (setup_time > 1)
		setup_time -= 1;
	hold_time = (dev_clk / (rate * 4));

	/*         high
	 *         ____     ____
	 *  clk __|    |___|    |____
	 *              low
	 *        |<--period--->|
	 *
	 */
	cnt_high = dev_clk / (rate * 2);
	cnt_low = dev_clk / (rate * 2);

	if (setup_time > 255)
		setup_time = 255;
	if (setup_time <= 0)
		setup_time = 1;
	if (hold_time > 0xFFFF)
		hold_time = 0xFFFF;

	if (rate <= 100) {
		i2c_writel(i2c_ctl,I2C_SHCNT, I2CSHCNT_ADJUST(cnt_high));
		i2c_writel(i2c_ctl,I2C_SLCNT, I2CSLCNT_ADJUST(cnt_low));
	} else {
		i2c_writel(i2c_ctl,I2C_FHCNT, I2CFHCNT_ADJUST(cnt_high));
		i2c_writel(i2c_ctl,I2C_FLCNT, I2CFLCNT_ADJUST(cnt_low));
	}

	i2c_writel(i2c_ctl,I2C_SDASU, setup_time & 0xff);
	i2c_writel(i2c_ctl,I2C_SDAHD, hold_time);

	return 0;
}

/*
 * Write data to CPU
 *
 * Use of i2c protocol to write data to CPU
 */
static int i2c_write_cpu(unsigned int i2c_ctl,struct i2c_msg *i2c)
{
	int i2c_tar = 0;
	unsigned short reg_tmp;
	int timeout = 1000;

	/* write device address */
	i2c_tar = i2c_readl(i2c_ctl,I2C_TAR);
	i2c_tar &= ~(1<<12);
	i2c_tar &= ~(0x3ff);
	i2c_tar |= i2c->device_addr;
	i2c_writel(i2c_ctl,I2C_TAR, i2c_tar);
	/* write data */
	i2c_writel(i2c_ctl,I2C_TXTL, TX_LEVEL);
	i2c_readl(i2c_ctl,I2C_CSTP);	/* clear STP bit */
	i2c_readl(i2c_ctl,I2C_CTXOF);	/* clear TXOF bit */
	i2c_readl(i2c_ctl,I2C_CTXABRT);	/* clear TXABRT bit */

	while ((i2c_readl(i2c_ctl,I2C_STA) & I2C_STA_TFNF) && (i2c->len > 0)) {
		reg_tmp = *i2c->buf++;
		if (i2c->len == 1) {
			reg_tmp |= I2C_DC_STP;
		}
		i2c_writel(i2c_ctl,I2C_DC, reg_tmp);

		i2c->len -= 1;
	}

	if (i2c->len == 0) {
		i2c_writel(i2c_ctl,I2C_TXTL, 0);
	}

	timeout = 1000;
	while (!(i2c_readl(i2c_ctl,I2C_STA) & I2C_STA_TFE) && (--timeout > 0))
	{
		udelay(150);
	}
	if (!timeout) {
		//jz_i2c_dump(i2c_ctl);
		printf("timeout i2c%d tx fifo is full\n",i2c_ctl);
	}


	reg_tmp = I2C_RINTST_TXEMP | I2C_RINTST_TXABT | I2C_RINTST_TXOF;
	i2c_writel(i2c_ctl,I2C_RINTST, reg_tmp);

	timeout = 2000;
	while (!(i2c_readl(i2c_ctl,I2C_RINTST) & I2C_RINTST_ISTP) && --timeout);

	if (!timeout) {
		//jz_i2c_dump(i2c_ctl);
		printf("timeout issue\n");
		return -1;
	}

	reg_tmp = i2c_readl(i2c_ctl,I2C_TXABRT);
	if (reg_tmp) {
		if (reg_tmp & 0x01 == 0x01) {
			printf("i2c address mismatch\n");
		} else {
			printf("TXABRT: %x\n", reg_tmp);
		}
		i2c_readl(i2c_ctl,I2C_CTXABRT);
		return -1;
	}
	return 0;
}

/*
 * Read data from CPU
 *
 * Use of i2c protocol to read data from CPU
 */
static void i2c_read_cpu(unsigned int i2c_ctl,struct i2c_msg *i2c)
{
	unsigned int i2c_dc = 0;
	int i2c_tar = 0;
	unsigned int i = 0;

	//i2c_jz_reset();

	/* write device address */
	i2c_tar = i2c_readl(i2c_ctl,I2C_TAR);
	i2c_tar &= ~(1<<12);
	i2c_tar &= ~(0x3ff);
	i2c_tar |= i2c->device_addr;
	i2c_writel(i2c_ctl,I2C_TAR, i2c_tar);

	/* read data and stop */
	i2c_writel(i2c_ctl,I2C_RXTL, RX_LEVEL);

	i2c_readl(i2c_ctl,I2C_CSTP);	/* clear STP bit */
	i2c_readl(i2c_ctl,I2C_CTXOF);	/* clear TXOF bit */
	i2c_readl(i2c_ctl,I2C_CTXABRT);	/* clear TXABRT bit */

	for(i=0;i<(i2c->len);i++) {
		i2c_dc = 0;
		i2c_dc |= (1<<8);

		if(i==0)
		{
			i2c_dc |= I2C_DC_REST;	/*restart*/
		}
		else if(i== i2c->len - 1)
		{
			i2c_dc |= I2C_DC_STP;	/*stop*/
		}

		i2c_writel(i2c_ctl,I2C_DC, i2c_readl(i2c_ctl,I2C_DC) | i2c_dc);

		while(!(REG_I2C_STA(i2c_ctl) & I2C_STA_RFNE));

		i2c->buf[i] = (i2c_readl(i2c_ctl,I2C_DC) & 0xff);
	}
}
/*=====================================================================*/
/*                         Public Functions                            */
/*=====================================================================*/

/*
 * Initialization
 *
 * Initialize i2c settings
 */
int jz_i2c_init(int speed)
{
	int timeout = TIMEOUT;
	int reg_tmp = 0;
	int i = 0;
	/* printf("*** hardware i2c ***\n"); */
	/* gpio_set_func(GPIO_PORT_A,GPIO_FUNC_0, (0x1 << 16));  //mclk set */
	/* *(volatile unsigned int *)(0xB0010018) |= (1 << 20);   // bit20 = 0 */ 
	/* *(volatile unsigned int *)(0xB0010024) |= (1 << 20);   // bit20 = 1 */
	/* *(volatile unsigned int *)(0xB0010038) |= (1 << 20); */
	/* *(volatile unsigned int *)(0xB0010044) |= (1 << 20); */
	/* mdelay(5); */
	/* *(volatile unsigned int *)(0xB0010048) |= (1 << 20); */
	/* mdelay(10); */
	/* *(volatile unsigned int *)(0xB0010044) |= (1 << 20); */
	/* mdelay(10); */
	for(i = 0;i<sizeof(i2c_gpio_init)/sizeof(struct i2c_gpio_ctl);i++){
		gpio_set_func(i2c_gpio_init[i].grp,i2c_gpio_init[i].func,i2c_gpio_init[i].pins);
#if 0
		reg_tmp = i2c_readl(I2C_ENB);
		reg_tmp |= (1 << 1);
		reg_tmp &= ~(0x01);
		i2c_writel(I2C_ENB, reg_tmp);
		while(i2c_readl(I2C_ENSTA) & (0X01) && --timeout > 0);

		if(timeout < 0) {
			printf("Abort hardware i2c: failed\n");
			return -1;
		}
#endif

		reg_tmp = i2c_readl(i2c_gpio_init[i].base_addr,I2C_CTRL);
		reg_tmp &=~ I2C_CTRL_SATP; //7bit address
		reg_tmp &=~ I2C_CTRL_REST; //no restart
		i2c_writel(i2c_gpio_init[i].base_addr,I2C_CTRL, reg_tmp);

		i2c_set_speed(i2c_gpio_init[i].base_addr,speed);

		i2c_writel(i2c_gpio_init[i].base_addr,I2C_INTM, 0x00);

		i2c_writel(i2c_gpio_init[i].base_addr,I2C_ENB, 0x01);     //enable SMB
		while(!(i2c_readl(i2c_gpio_init[i].base_addr,I2C_ENSTA) &(0X01)) ==1 && --timeout);

		if(timeout < 0) {
			printf("hd I2C timeout!\n");
			return -1;
		}

	}
	return 0;
}

/*
 * Probe function
 *
 * initialize i2c and return the value of the initialization function
 */
int jz_i2c_probe(void)
{
	int ret = 0;
	unsigned int reg = cpm_readl(0x20);
#ifdef I2C0_GPIOA
    reg &= ~(1 << 6);
#endif
#if defined(I2C1_GPIOA) || defined(I2C1_GPIOD)
    reg &= ~(1 << 7);
#endif
#if defined(I2C2_GPIOB) || defined(I2C2_GPIOC)
    reg &= ~(1 << 14);
#endif
	cpm_writel(reg, 0x20);  //smb gate
	ret = jz_i2c_init(I2C_SPEED);
	if(ret < 0) {
		printf("i2c probe error\n");
		return -1;
	}

	return ret;
}

/*
 * Write data preparations
 *
 * Store current address and data from the list of registers
 * to a structure(i2c_msg_t) which will be used to write infos only.
 */
int  jz_i2c_write(unsigned int i2c_ctl,unsigned char chip, unsigned int addr,
		  int alen, unsigned char *buffer, int len)
{

	i2c_msg_t i2c;
	if(i2c_ctl >= I2C_MAX){
		return -1;
	}
	unsigned char buffer_dest[32];

	i2c.buf = buffer_dest;

	i2c.device_addr = chip;
	//i2c.flag = !I2C_M_RD;
	i2c.len = alen + len;
	switch(alen) {
	case 2:
		i2c.buf[0] = (addr >> 8) & 0xff;
		i2c.buf[1] = addr & 0xff;
		break;
	case 1:
		i2c.buf[0] = addr & 0xff;
		break;
	default:
		break;
	}
		memcpy(&i2c.buf[alen], buffer, len);

	int ret = i2c_write_cpu(i2c_ctl,&i2c);
	if (ret !=0 ) {
		return -1;
	}

	return 0;
}

/*
 * Read data preparations
 *
 * Store current address and data from the list of registers
 * to a structure(i2c_msg_t) which will be used to write
 * then read infos.
 */
int  jz_i2c_read(unsigned int i2c_ctl,unsigned char chip, unsigned int addr,
		 int alen, unsigned char *buffer, int len)
{
	i2c_msg_t i2c[2];
	if(i2c_ctl >= I2C_MAX){
		return -1;
	}

	unsigned char temp[1];

	i2c[0].buf = temp;
	i2c[0].device_addr = chip;
	//i2c[0].flag = !I2C_M_RD;
	i2c[0].len = alen;
	switch(alen) {
	case 2:
		i2c[0].buf[0] = (addr >> 8) & 0xff;
		i2c[0].buf[1] = addr & 0xff;
		break;
	case 1:
		i2c[0].buf[0] = addr & 0xff;
		break;
	default:
		break;
	}
	int ret = i2c_write_cpu(i2c_ctl,&i2c[0]);
	if (ret != 0) {//如果此处错误不return  后面的i2c_read_cpu阻塞
		return -1;
	}

	i2c[1].buf = buffer;
	i2c[1].device_addr = chip;
	//i2c[1].flag = I2C_M_RD;
	i2c[1].len = len;
	i2c_read_cpu(i2c_ctl,&i2c[1]);

	return 0;
}

