/*
 * Ingenic JZ SFC driver
 *
 * Copyright (c) 2013 Ingenic Semiconductor Co.,Ltd
 * Author: Tiger <xyfu@ingenic.cn>
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

#include <config.h>
#include <common.h>
#include <spi.h>
#include <spi_flash.h>
#include <malloc.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <asm/arch/cpm.h>
#include <asm/arch/spi.h>
#include <asm/arch/sfc.h>
#include <asm/arch/clk.h>
#include <asm/arch/base.h>
#include <malloc.h>

#include "../../../spi/jz_spi.h"

static struct jz_spi_support gparams;

struct spi_quad_mode *quad_mode = NULL;
/* wait time before read status (us) for spi nand */
//static int t_reset = 500;
int mode = 0;
int flag = 0;
int sfc_is_init = 0;
unsigned int sfc_rate = 0;
unsigned int sfc_quad_mode = 0;
unsigned int quad_mode_is_set = 0;
unsigned char sfc_index;


struct jz_sfc_slave {
	struct spi_slave slave;
	unsigned int mode;
	unsigned int max_hz;
};

#define CMD_LEN 5
static int spi_xfer_cmd_len;
static unsigned char spi_xfer_cmd[CMD_LEN];

struct jz_sfc {
	unsigned int  addr;
	unsigned int  len;
	unsigned int  cmd;
	unsigned int  addr_plus;
	unsigned int  sfc_mode;
	unsigned char daten;
	unsigned char addr_len;
	unsigned char pollen;
	unsigned char phase;
	unsigned char dummy_byte;
};

static uint32_t jz_sfc_readl(unsigned char sfc_index,unsigned int offset)
{
	if(sfc_index == SFC_0)
		return readl(SFC0_BASE + offset);
	else if (sfc_index == SFC_1)
		return readl(SFC1_BASE + offset);
	else
		return 0;
}

static void jz_sfc_writel(unsigned char sfc_index,unsigned int value, unsigned int offset)
{
	if(sfc_index == SFC_0)
		writel(value, SFC0_BASE + offset);
	else if (sfc_index == SFC_1)
		writel(value, SFC1_BASE + offset);
	else;
}

void dump_sfc_reg(unsigned char sfc_index)
{
	int i = 0;
	printf("SFC%d_GLB					:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_GLB ));
	printf("SFC%d_GLB1					:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_GLB1 ));
	printf("SFC%d_DEV_CONF				:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_DEV_CONF ));
	printf("SFC%d_DEV_STA_RT			:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_DEV_STA_RT ));
	printf("SFC%d_DEV1_STA_RT			:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_DEV1_STA_RT ));
	printf("SFC%d_DEV_STA_MSK			:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_DEV_STA_MSK ));
	printf("SFC%d_TRAN_LEN				:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_TRAN_LEN ));
	for(i = 0; i < 6; i++)
		printf("SFC%d_TRAN_CONF(%d)		:%x\n", sfc_index,i,jz_sfc_readl(sfc_index,SFC_TRAN_CONF(i)));
	for(i = 0; i < 6; i++)
		printf("SFC%d_TRAN_CONF1(%d)	:%x\n", sfc_index,i,jz_sfc_readl(sfc_index,SFC_TRAN_CONF1(i)));
	for(i = 0; i < 6; i++)
		printf("SFC%d_DEV_ADDR(%d)		:%x\n", sfc_index,i,jz_sfc_readl(sfc_index,SFC_DEV_ADDR(i)));
	printf("SFC%d_MEM_ADDR				:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_MEM_ADDR));
	printf("SFC%d_TRIG					:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_TRIG));
	printf("SFC%d_SR		 			:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_SR));
	printf("SFC%d_SCR		 			:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_SCR));
	printf("SFC%d_INTC					:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_INTC));
	printf("SFC%d_FSM		 			:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_FSM ));
	printf("SFC%d_CGE		 			:%x\n", sfc_index,jz_sfc_readl(sfc_index,SFC_CGE ));
}

unsigned int sfc_fifo_num(unsigned char sfc_index)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(sfc_index,SFC_SR);
	tmp &= (0x7f << 16);
	tmp = tmp >> 16;
	return tmp;
}


void sfc_set_mode(unsigned char sfc_index,int channel, int value)
{
	unsigned int tmp;
	/*T40 modify SFC_TRAN_CONF->SFC_TRAN_CONF1*/
	tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF1(channel));
	tmp &= ~(TRAN_CONF1_TRAN_MODE_MSK);
	tmp |= (value << TRAN_CONF1_TRAN_MODE_OFFSET);
	jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF1(channel));
}


void sfc_dev_addr_dummy_bytes(unsigned char sfc_index,int channel, unsigned int value)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF(channel));
	tmp &= ~TRAN_CONF0_DMYBITS_MSK;
	tmp |= (value << TRAN_CONF0_DMYBITS_OFFSET);
	jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF(channel));
}

void sfc_transfer_direction(unsigned char sfc_index,int value)
{
	if(value == 0) {
		unsigned int tmp;
		tmp = jz_sfc_readl(sfc_index,SFC_GLB);
		tmp &= ~GLB_TRAN_DIR;
		jz_sfc_writel(sfc_index,tmp,SFC_GLB);
	} else {
		unsigned int tmp;
		tmp = jz_sfc_readl(sfc_index,SFC_GLB);
		tmp |= GLB_TRAN_DIR;
		jz_sfc_writel(sfc_index,tmp,SFC_GLB);
	}
}

void sfc_set_length(unsigned char sfc_index,int value)
{
	jz_sfc_writel(sfc_index,value,SFC_TRAN_LEN);
}

void sfc_set_addr_length(unsigned char sfc_index,int channel, unsigned int value)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF(channel));
	tmp &= ~(TRAN_CONF0_ADDR_WIDTH_MSK);
	tmp |= (value << TRAN_CONF0_ADDR_WIDTH_OFFSET);
	jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF(channel));
}

void sfc_cmd_en(unsigned char sfc_index,int channel, unsigned int value)
{
	if(value == 1) {
		unsigned int tmp;
		tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF(channel));
		tmp |= TRAN_CONF0_CMD_EN;
		jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF(channel));
	} else {
		unsigned int tmp;
		tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF(channel));
		tmp &= ~TRAN_CONF0_CMD_EN;
		jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF(channel));
	}
}

void sfc_data_en(unsigned char sfc_index,int channel, unsigned int value)
{
	if(value == 1) {
		unsigned int tmp;
		tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF(channel));
		tmp |= TRAN_CONF0_DATEEN;
		jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF(channel));
	} else {
		unsigned int tmp;
		tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF(channel));
		tmp &= ~TRAN_CONF0_DATEEN;
		jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF(channel));
	}
}

void sfc_write_cmd(unsigned char sfc_index,int channel, unsigned int value)
{
	unsigned int tmp;
	tmp = jz_sfc_readl(sfc_index,SFC_TRAN_CONF(channel));
	tmp &= ~TRAN_CONF0_CMD_MSK;
	tmp |= value;
	jz_sfc_writel(sfc_index,tmp,SFC_TRAN_CONF(channel));
}

void sfc_dev_addr(unsigned char sfc_index,int channel, unsigned int value)
{
	jz_sfc_writel(sfc_index,value, SFC_DEV_ADDR(channel));
}

void sfc_dev_addr_plus(unsigned char sfc_index,int channel, unsigned int value)
{
	jz_sfc_writel(sfc_index,value,SFC_DEV_ADDR_PLUS(channel));
}

void sfc_set_transfer(unsigned char sfc_index,struct jz_sfc *hw,int dir)
{
	if(dir == 1)
		sfc_transfer_direction(sfc_index,GLB_TRAN_DIR_WRITE);
	else
		sfc_transfer_direction(sfc_index,GLB_TRAN_DIR_READ);
	sfc_set_length(sfc_index,hw->len);
	sfc_set_addr_length(sfc_index,0, hw->addr_len);
	sfc_cmd_en(sfc_index,0, 0x1);
	sfc_data_en(sfc_index,0, hw->daten);
	sfc_write_cmd(sfc_index,0, hw->cmd);
	sfc_dev_addr(sfc_index,0, hw->addr);
	sfc_dev_addr_plus(sfc_index,0, hw->addr_plus);
	sfc_dev_addr_dummy_bytes(sfc_index,0,hw->dummy_byte);
	sfc_set_mode(sfc_index,0,hw->sfc_mode);
}

static int sfc_read_data(unsigned char sfc_index,unsigned int *data, unsigned int length)
{
	unsigned int tmp_len = 0;
	unsigned int fifo_num = 0;
	unsigned int i;
	unsigned int reg_tmp = 0;
	unsigned int  len = (length + 3) / 4 ;

	while(1){
		reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
		if (reg_tmp & RECE_REQ) {
			jz_sfc_writel(sfc_index,CLR_RREQ,SFC_SCR);
			if ((len - tmp_len) > THRESHOLD)
				fifo_num = THRESHOLD;
			else {
				fifo_num = len - tmp_len;
			}

			for (i = 0; i < fifo_num; i++) {
				*data = jz_sfc_readl(sfc_index,SFC_RM_DR);
				data++;
				tmp_len++;
			}
		}
		if (tmp_len == len)
			break;
	}

	reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
	while (!(reg_tmp & END)){
		reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
	}

	if ((jz_sfc_readl(sfc_index,SFC_SR)) & END)
		jz_sfc_writel(sfc_index,CLR_END,SFC_SCR);


	return 0;
}


static int sfc_write_data(unsigned char sfc_index,unsigned int *data, unsigned int length)
{
	unsigned int tmp_len = 0;
	unsigned int fifo_num = 0;
	unsigned int i;
	unsigned int reg_tmp = 0;
	unsigned int  len = (length + 3) / 4 ;

	while(1){
		reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
		if (reg_tmp & TRAN_REQ) {
			jz_sfc_writel(sfc_index,CLR_TREQ,SFC_SCR);
			if ((len - tmp_len) > THRESHOLD)
				fifo_num = THRESHOLD;
			else {
				fifo_num = len - tmp_len;
			}

			for (i = 0; i < fifo_num; i++) {
				jz_sfc_writel(sfc_index,*data,SFC_RM_DR);
				data++;
				tmp_len++;
			}
		}
		if (tmp_len == len)
			break;
	}

	reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
	while (!(reg_tmp & END)){
		reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
	}

	if ((jz_sfc_readl(sfc_index,SFC_SR)) & END)
		jz_sfc_writel(sfc_index,CLR_END,SFC_SCR);


	return 0;
}

static int sfc_init(unsigned char sfc_index)
{
	unsigned int i;
	volatile unsigned int tmp;
#ifndef CONFIG_BURNER
	#ifndef CONFIG_SPI_QUAD
		sfc_rate = 50000000;
	#else
		sfc_rate = 70000000;
	#endif /* CONFIG_SPI_QUAD */
	#ifndef CONFIG_FPGA
		#ifdef CONFIG_SFC0_NOR
			clk_set_rate(SFC0, sfc_rate);
		#endif
		#ifdef CONFIG_SFC1_NOR
			clk_set_rate(SFC1, sfc_rate);
		#endif
	#endif /* CONFIG_FPGA */
#else
	if(sfc_rate !=0 )
	#ifndef CONFIG_FPGA
		clk_set_rate(SFC0, sfc_rate);
	#endif /* CONFIG_FPGA */
	else{
		printf("this will be an error that the sfc rate is 0\n");
		sfc_rate = 70000000;
	#ifndef CONFIG_FPGA
		clk_set_rate(SFC0, sfc_rate);
	#endif /* CONFIG_FPGA */
	}
#endif /* CONFIG_BURNER */

#ifdef CONFIG_SPI_QUAD
	sfc_quad_mode = 1;
#endif

	tmp = jz_sfc_readl(sfc_index,SFC_GLB);
	tmp &= ~(GLB_TRAN_DIR | GLB_OP_MODE );

	tmp &= ~GLB_WP_EN; //T40 modify
	//tmp |= GLB_WP_EN;
	jz_sfc_writel(sfc_index,tmp,SFC_GLB);

	tmp = jz_sfc_readl(sfc_index,SFC_DEV_CONF);
	tmp &= ~(DEV_CONF_CMD_TYPE | DEV_CONF_CPHA | DEV_CONF_CPOL | DEV_CONF_SMP_DELAY_MSK |
			DEV_CONF_THOLD_MSK | DEV_CONF_TSETUP_MSK | DEV_CONF_TSH_MSK);
	/*T40 ADD STA_ENDIAN_MSB*/
	//tmp |= (DEV_CONF_CEDL | DEV_CONF_HOLDDL | DEV_CONF_WPDL | 1 << DEV_CONF_SMP_DELAY_OFFSET | STA_ENDIAN_MSB);
	tmp |= (DEV_CONF_CEDL | DEV_CONF_HOLDDL | DEV_CONF_WPDL);
	//T40 modify delay
#ifdef CONFIG_SPI_QUAD
	tmp |= DEV_CONF_SMP_DELAY_180 << 16;
#endif
	jz_sfc_writel(sfc_index,tmp,SFC_DEV_CONF);

	/*T40 modify;CONF1_TRAN_MODE*/
	for (i = 0; i < 6; i++) {
		jz_sfc_writel(sfc_index,jz_sfc_readl(sfc_index,SFC_TRAN_CONF(i))& (~(TRAN_CONF0_PHASE_FORMAT)),SFC_TRAN_CONF(i));
		jz_sfc_writel(sfc_index,jz_sfc_readl(sfc_index,SFC_TRAN_CONF1(i))& (~(TRAN_CONF1_TRAN_MODE_MSK)),SFC_TRAN_CONF1(i));
	}

	jz_sfc_writel(sfc_index,(CLR_END | CLR_TREQ | CLR_RREQ | CLR_OVER | CLR_UNDR),SFC_INTC);

	jz_sfc_writel(sfc_index,0,SFC_CGE);

	tmp = jz_sfc_readl(sfc_index,SFC_GLB);
	tmp &= ~(GLB_THRESHOLD_MSK);
	tmp &= ~(GLB_BURST_MD_MSK); //T40 ADD
	tmp |= (THRESHOLD << GLB_THRESHOLD_OFFSET);
	jz_sfc_writel(sfc_index,tmp,SFC_GLB);

	return 0;
}

void sfc_send_cmd(unsigned char sfc_index,unsigned char *cmd,unsigned int len,unsigned int addr ,unsigned addr_len,unsigned dummy_byte,unsigned int daten,unsigned char dir)
{
	struct jz_sfc sfc;
	unsigned int reg_tmp = 0;
	sfc.cmd = *cmd;
	sfc.addr_len = addr_len;
	//sfc.addr = ((*addr << 16) &0x00ff0000) | ((*(addr + 1) << 8)&0x0000ff00) |((*(addr + 2))&0xff);
	sfc.addr = addr;
	sfc.addr_plus = 0;
	sfc.dummy_byte = dummy_byte;
	sfc.daten = daten;
	sfc.len = len;

	if((daten == 1)&&(addr_len != 0)){
		sfc.sfc_mode = mode;
	}else{
		sfc.sfc_mode = 0;
	}
	sfc_set_transfer(sfc_index,&sfc,dir);
	jz_sfc_writel(sfc_index,TRIG_FLUSH,SFC_TRIG);
	jz_sfc_writel(sfc_index,TRIG_START,SFC_TRIG);

	/*this must judge the end status*/
	if((daten == 0)){
		reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
		while (!(reg_tmp & END)){
			reg_tmp = jz_sfc_readl(sfc_index,SFC_SR);
		}
		if ((jz_sfc_readl(sfc_index,SFC_SR)) & END)
			jz_sfc_writel(sfc_index,CLR_END,SFC_SCR);
	}

}

#ifdef CONFIG_SFC_NOR
int jz_sfc_set_address_mode(unsigned char sfc_index,struct spi_flash *flash, int on)
{
	unsigned char cmd[3];
	unsigned int  buf = 0;
	if(flash->addr_size == 4){
		cmd[0] = CMD_WREN;
		if(on == 1){
			cmd[1] = CMD_EN4B;
		}else{
			cmd[1] = CMD_EX4B;
		}
		cmd[2] = CMD_RDSR;

		sfc_send_cmd(sfc_index,&cmd[0],0,0,0,0,0,1);

		sfc_send_cmd(sfc_index,&cmd[1],0,0,0,0,0,1);

		sfc_send_cmd(sfc_index,&cmd[2], 1,0,0,0,1,0);

		sfc_read_data(sfc_index,&buf, 1);
		while(buf & CMD_SR_WIP) {
			sfc_send_cmd(sfc_index,&cmd[2], 1,0,0,0,1,0);
			sfc_read_data(sfc_index,&buf, 1);
		}
	}
	return 0;
}

void sfc_set_quad_mode(unsigned char sfc_index)
{
	/* the paraterms is
	 * cmd , len, addr,addr_len
	 * dummy_byte, daten
	 * dir
	 *
	 * */
	unsigned char cmd[6];
	unsigned int buf = 0;
	unsigned int tmp = 0;
	int i = 10;

	if(quad_mode != NULL){
		cmd[0] = CMD_WREN;
		cmd[1] = quad_mode->WRSR_CMD;
		cmd[2] = quad_mode->RDSR_CMD;
		cmd[3] = CMD_RDSR;
		cmd[4] = CMD_ENOTP;
		cmd[5] = CMD_EXOTP;

		if(quad_mode->otp_ops_mode)
			sfc_send_cmd(sfc_index,&cmd[4],0,0,0,0,0,1);

		sfc_send_cmd(sfc_index,&cmd[0],0,0,0,0,0,1);

		sfc_send_cmd(sfc_index,&cmd[1],quad_mode->WD_DATE_SIZE,0,0,0,1,1);
		sfc_write_data(sfc_index,&quad_mode->WRSR_DATE,1);

		sfc_send_cmd(sfc_index,&cmd[3],1,0,0,0,1,0);
		sfc_read_data(sfc_index,&tmp, 1);
		while(tmp & CMD_SR_WIP) {
			sfc_send_cmd(sfc_index,&cmd[3],1,0,0,0,1,0);
			sfc_read_data(sfc_index,&tmp, 1);
		}

		sfc_send_cmd(sfc_index,&cmd[2], quad_mode->RD_DATE_SIZE,0,0,0,1,0);
		sfc_read_data(sfc_index,&buf, 1);
		while(!(buf & quad_mode->RDSR_DATE)&&((i--) > 0)) {
			sfc_send_cmd(sfc_index,&cmd[2], quad_mode->RD_DATE_SIZE,0,0,0,1,0);
			sfc_read_data(sfc_index,&buf, 1);
		}
		if(quad_mode->otp_ops_mode)
			sfc_send_cmd(sfc_index,&cmd[5],0,0,0,0,0,1);
		quad_mode_is_set = 1;
		/*printf("set quad mode is enable.the buf = %x\n",buf);*/
	}else{

		printf("the quad_mode is NULL,the nor flash id we not support\n");
	}
}

int jz_sfc_read(unsigned char sfc_index,struct spi_flash *flash, u32 offset, size_t len, void *data)
{
	unsigned char cmd[5];
	unsigned long read_len;
	unsigned int i;

	jz_sfc_set_address_mode(sfc_index,flash,1);

	if(sfc_quad_mode == 1){
		cmd[0]  = quad_mode->cmd_read;
		mode = quad_mode->sfc_mode;
	}else{
		cmd[0]  = CMD_READ;
		mode = TRAN_CONF1_SPI_STANDARD;
	}

	for(i = 0; i < flash->addr_size; i++){
		cmd[i + 1] = offset >> (flash->addr_size - i - 1) * 8;
	}

	read_len = flash->size - offset;

	if(len < read_len)
		read_len = len;
	/* the paraterms is
		 * cmd , len, addr,addr_len
		 * dummy_byte, daten
		 * dir
		 *
		 * */

	if(sfc_quad_mode == 1){
		sfc_send_cmd(sfc_index,&cmd[0],read_len,offset,flash->addr_size,quad_mode->dummy_byte,1,0);
	}else{
		sfc_send_cmd(sfc_index,&cmd[0],read_len,offset,flash->addr_size,0,1,0);
	}
	//	dump_sfc_reg(sfc_index);
	sfc_read_data(sfc_index,data, len);

	jz_sfc_set_address_mode(sfc_index,flash,0);

	return 0;
}

#if 0
static int jz_sfc_read_norflash_params(unsigned char sfc_index,struct spi_flash *flash, u32 offset, size_t len, void *data)
{
	unsigned char cmd[5];
	unsigned long read_len;
	unsigned int i;

	cmd[0]  = CMD_READ;
	mode = TRAN_CONF1_SPI_STANDARD;

	for(i = 0; i < flash->addr_size; i++){
		cmd[i + 1] = offset >> (flash->addr_size - i - 1) * 8;
	}

	read_len = flash->size - offset;

	if(len < read_len)
		read_len = len;

	sfc_send_cmd(sfc_index,&cmd[0],read_len,offset,flash->addr_size,0,1,0);
	sfc_read_data(sfc_index,data, len);

	return 0;
}
#endif
int jz_sfc_write(unsigned char sfc_index,struct spi_flash *flash, u32 offset, size_t length, const void *buf)
{
	unsigned char cmd[7];
	unsigned tmp = 0;
	int i;
	unsigned char *send_buf = (unsigned char *)buf;
	unsigned int pagelen = 0,len = 0,retlen = 0;

	jz_sfc_set_address_mode(sfc_index,flash,1);

	if (offset + length > flash->size) {
		printf("Data write overflow this chip\n");
		return -1;
	}

	cmd[0] = CMD_WREN;

	if(sfc_quad_mode == 1){
		cmd[1]  = CMD_QPP;
		mode = TRAN_CONF1_SPI_QUAD;
	}else{
		cmd[1]  = CMD_PP;
		mode = TRAN_CONF1_SPI_STANDARD;
	}

	cmd[flash->addr_size + 2] = CMD_RDSR;

	while (length) {
		if (length >= flash->page_size)
			pagelen = 0;
		else
			pagelen = length % flash->page_size;

		/* the paraterms is
		 * cmd , len, addr,addr_len
		 * dummy_byte, daten
		 * dir
		 *
		 * */
		sfc_send_cmd(sfc_index,&cmd[0],0,0,0,0,0,1);

		if (!pagelen || pagelen > flash->page_size)
			len = flash->page_size;
		else
			len = pagelen;

		if (offset % flash->page_size + len > flash->page_size)
			len -= offset % flash->page_size + len - flash->page_size;

		for(i = 0; i < flash->addr_size; i++){
			cmd[i+2] = offset >> (flash->addr_size - i - 1) * 8;
		}

		sfc_send_cmd(sfc_index,&cmd[1], len,offset,flash->addr_size,0,1,1);

		/*dump_sfc_reg(sfc_index);*/

		sfc_write_data(sfc_index,(unsigned int *)send_buf ,len);

		retlen = len;

		/*polling*/
		sfc_send_cmd(sfc_index,&cmd[flash->addr_size + 2],1,0,0,0,1,0);
		sfc_read_data(sfc_index,&tmp, 1);
		while(tmp & CMD_SR_WIP) {
			sfc_send_cmd(sfc_index,&cmd[flash->addr_size + 2],1,0,0,0,1,0);
			sfc_read_data(sfc_index,&tmp, 1);
		}

		if (!retlen) {
			printf("spi nor write failed\n");
			return -1;
		}

		offset += retlen;
		send_buf += retlen;
		length -= retlen;
	}

	jz_sfc_set_address_mode(sfc_index,flash,0);
	return 0;
}

int jz_sfc_chip_erase(unsigned char sfc_index)
{
	/* the paraterms is
	 * cmd , len, addr,addr_len
	 * dummy_byte, daten
	 * dir
	 *
	 * */

	unsigned char cmd[6];
	cmd[0] = CMD_WREN;
	cmd[1] = CMD_ERASE_CE;
	cmd[5] = CMD_RDSR;
	unsigned int  buf = 0;

	if(sfc_quad_mode == 1){
		if(quad_mode_is_set == 0){
			sfc_set_quad_mode(sfc_index);
		}
	}

	jz_sfc_writel(sfc_index,1 << 2,SFC_TRIG);
	sfc_send_cmd(sfc_index,&cmd[0],0,0,0,0,0,1);

	sfc_send_cmd(sfc_index,&cmd[1],0,0,0,0,0,1);

	sfc_send_cmd(sfc_index,&cmd[5], 1,0,0,0,1,0);
	sfc_read_data(sfc_index,&buf, 1);
	/*printf("sfc start chip erase\n");*/
	while(buf & CMD_SR_WIP) {
		sfc_send_cmd(sfc_index,&cmd[5], 1,0,0,0,1,0);
		sfc_read_data(sfc_index,&buf, 1);
	}
	/*printf("########## chip erase ok ######### \n");*/
	return 0;
}

int jz_sfc_erase(unsigned char sfc_index,struct spi_flash *flash, u32 offset, size_t len)
{
	unsigned long erase_size = flash->sector_size;
	unsigned char cmd[7];
	unsigned int  buf = 0, i;

	jz_sfc_set_address_mode(sfc_index,flash,1);

	if(len < 0x8000){
		erase_size = 0x1000;
	}

	if(offset % CONFIG_SFC_MIN_ALIGN) {
		printf("addr align as %x !\n", flash->sector_size);
		return -1;
	}

	if(len % CONFIG_SFC_MIN_ALIGN) {
		printf("len align as %x !\n", flash->sector_size);
		return -1;
	}

	if((offset + len) > flash->size) {
		printf("len plus offset more than flash size!\n");
		return -1;
	}

	if(len == flash->size) {
		jz_sfc_chip_erase(sfc_index);
		return 0;
	}

	cmd[0] = CMD_WREN;
	cmd[flash->addr_size + 2] = CMD_RDSR;

	while(len > 0) {
		for(i = 0; i < flash->addr_size; i++){
			cmd[i+2] = offset >> (flash->addr_size - i - 1) * 8;
		}

		//	printf("erase %x %x %x %x %x %x %x \n", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], offset);

		/* the paraterms is
		 * cmd , len, addr,addr_len
		 * dummy_byte, daten
		 * dir
		 *
		 * */

		switch(erase_size) {
		case 0x1000 :
			cmd[1] = CMD_ERASE_4K;
			break;
		case 0x8000 :
			cmd[1] = CMD_ERASE_32K;
			break;
		case 0x10000 :
			cmd[1] = CMD_ERASE_64K;
			break;
		default:
			printf("unknown erase size !\n");
			return -1;
		}

		sfc_send_cmd(sfc_index,&cmd[0],0,0,0,0,0,1);

		sfc_send_cmd(sfc_index,&cmd[1],0,offset,flash->addr_size,0,0,1);

		sfc_send_cmd(sfc_index,&cmd[flash->addr_size + 2], 1,0,0,0,1,0);
		sfc_read_data(sfc_index,&buf, 1);
		while(buf & CMD_SR_WIP) {
			sfc_send_cmd(sfc_index,&cmd[flash->addr_size + 2], 1,0,0,0,1,0);
			sfc_read_data(sfc_index,&buf, 1);
		}

		offset += erase_size;
		len -= erase_size;

		if((erase_size != 0x1000 ) && (len  < erase_size)) {
			erase_size = 0x1000;
		}
	}

	jz_sfc_set_address_mode(sfc_index,flash,0);
	return 0;
}

#if 0
struct spi_flash *sfc_flash_probe_ingenic(struct spi_slave *spi, u8 *idcode)
{
	int i;
	struct spi_flash *flash;
	//struct jz_spi_support *params;

	for (i = 0; i < ARRAY_SIZE(jz_spi_support_table); i++) {
		gparams = &jz_spi_support_table[i];
		if (gparams->id_manufactory == idcode[0])
			break;
	}

	if (i == ARRAY_SIZE(jz_spi_support_table)) {
#ifdef CONFIG_BURNER
		if (idcode[0] != 0){
			printf("unsupport ID is %04x if the id not be 0x00,the flash is ok for burner\n",idcode[0]);
			gparams = &jz_spi_support_table[1];
		}else{
			printf("ingenic: Unsupported ID %04x\n", idcode[0]);
			return NULL;

		}
#else
			printf("ingenic: Unsupported ID %04x\n", idcode[0]);
			return NULL;
#endif
	}

	flash = spi_flash_alloc_base(spi, gparams->name);
	if (!flash) {
		printf("ingenic: Failed to allocate memory\n");
		return NULL;
	}

	flash->erase = jz_sfc_erase;
	flash->write = jz_sfc_write;
	flash->read  = jz_sfc_read;

	flash->page_size = gparams->page_size;
	flash->sector_size = gparams->sector_size;
	flash->size = gparams->size;
	flash->addr_size = gparams->addr_size;
	return flash;
}
#endif


void sfc_nor_RDID(unsigned char sfc_index,unsigned int *idcode)
{
	/* the paraterms is
	 * cmd , len, addr,addr_len
	 * dummy_byte, daten
	 * dir
	 *
	 * */
	unsigned char cmd[1];
//	unsigned char chip_id[4];
	unsigned int chip_id = 0;

	cmd[0] = CMD_RDID;
	sfc_send_cmd(sfc_index,&cmd[0],3,0,0,0,1,0);
	sfc_read_data(sfc_index,&chip_id, 1);
//	*idcode = chip_id[0];
	*idcode = chip_id & 0x00ffffff;
}

#if 0
static void dump_norflash_params(void)
{
	printf(" =================================================\n");
	printf(" ======   gparams->name = %s\n",gparams.name);
	printf(" ======   gparams->id_manufactory = %x\n",gparams.id_manufactory);
	printf(" ======   gparams->page_size = %d\n",gparams.page_size);
	printf(" ======   gparams->sector_size = %d\n",gparams.sector_size);
	printf(" ======   gparams->addr_size = %d\n",gparams.addr_size);
	printf(" ======   gparams->size = %d\n",gparams.size);

	printf(" ======   gparams->quad.dummy_byte = %d\n",gparams.quad_mode.dummy_byte);
	printf(" ======   gparams->quad.RDSR_CMD = %x\n",gparams.quad_mode.RDSR_CMD);
	printf(" ======   gparams->quad.WRSR_CMD = %x\n",gparams.quad_mode.WRSR_CMD);
	printf(" ======   gparams->quad.RDSR_DATE = %x\n",gparams.quad_mode.RDSR_DATE);
	printf(" ======   gparams->quad.WRSR_DATE = %x\n",gparams.quad_mode.WRSR_DATE);
	printf(" ======   gparams->quad.RD_DATE_SIZE = %x\n",gparams.quad_mode.RD_DATE_SIZE);
	printf(" ======   gparams->quad.WD_DATE_SIZE = %x\n",gparams.quad_mode.WD_DATE_SIZE);
	printf(" ======   gparams->quad.cmd_read = %x\n",gparams.quad_mode.cmd_read);
	printf(" ======   gparams->quad.sfc_mode = %x\n",gparams.quad_mode.sfc_mode);

	printf(" =================================================\n");
}
#endif
#ifdef CONFIG_BURNER
int get_norflash_params_from_burner(unsigned char *addr)
{
	unsigned int idcode,chipnum,i;
	struct norflash_params *tmp;

	unsigned int flash_type = *(unsigned int *)(addr + 4);	//0:nor 1:nand
	if(flash_type == 0){

		sfc_nor_RDID(&idcode);

		printf("the norflash chip_id is %x\n",idcode);

		pdata.magic = NOR_MAGIC;
		pdata.version = CONFIG_NOR_VERSION;

		chipnum = *(unsigned int *)(addr + 8);
		for(i = 0; i < chipnum; i++){
			tmp = (struct norflash_params *)(addr + 12 +sizeof(struct norflash_params) * i);
			if(tmp->id == idcode) {
				memcpy(&pdata.norflash_params,tmp,sizeof(struct norflash_params));
				printf("-----break,break,break,break %x\n",idcode);
				break;
			}
		}

		if(i == chipnum){
			printf("none norflash support for the table ,please check the burner norflash supprot table\n");
			return -1;
		}

		pdata.norflash_partitions.num_partition_info = *(unsigned int *)(addr + 12 + sizeof(struct norflash_params) * chipnum);
		memcpy(&pdata.norflash_partitions.nor_partition[0] ,addr + 12 +sizeof(struct norflash_params) * chipnum + 4, \
				sizeof(struct nor_partition) * pdata.norflash_partitions.num_partition_info);
	}else{
		printf("the params recive from cloner not't the norflash\n");
		return -1;
	}
	return 0;
}

static void write_norflash_params_to_spl(unsigned int addr)
{
	memcpy(addr+CONFIG_SPIFLASH_PART_OFFSET,&pdata,sizeof(struct nor_sharing_params));
}

unsigned int get_partition_index(u32 offset, int *pt_offset, int *pt_size)
{
	int i;
	for(i = 0; i < pdata.norflash_partitions.num_partition_info; i++){
		if(offset >= pdata.norflash_partitions.nor_partition[i].offset && \
				offset < (pdata.norflash_partitions.nor_partition[i].offset + \
				pdata.norflash_partitions.nor_partition[i].size)){
			*pt_offset = pdata.norflash_partitions.nor_partition[i].offset;
			*pt_size = pdata.norflash_partitions.nor_partition[i].size;
			break;
		}
	}
	return i;
}
#endif

int sfc_nor_init(unsigned int idcode)
{

		int i = 0;

		for (i = 0; i < ARRAY_SIZE(jz_spi_support_table); i++) {
			gparams = jz_spi_support_table[i];
			if (gparams.id_manufactory == idcode){
				if(sfc_quad_mode == 1){
					quad_mode = &jz_spi_support_table[i].quad_mode;
				}
				break;
			}
		}

		if (i == ARRAY_SIZE(jz_spi_support_table)) {
			if ((idcode != 0)&&(idcode != 0xff)&&(sfc_quad_mode == 0)){
				printf("the id code = %x\n",idcode);
				printf("unsupport ID is if the id not be 0x00,the flash is ok for burner\n");
			}else{
				printf("ingenic: sfc quad mode Unsupported ID %x\n", idcode);
				return -1;
			}
		}
	return 0;
}

int sfc_nor_read(unsigned char sfc_index,struct spi_flash *flash, unsigned int src_addr, unsigned int count, void *dst_addr)
{
	int ret = 0;

	flag = 0;

	if(sfc_quad_mode == 1){
		if(quad_mode_is_set == 0){
			sfc_set_quad_mode(sfc_index);
		}
	}

	jz_sfc_writel(sfc_index,1 << 2,SFC_TRIG);

	ret = jz_sfc_read(sfc_index,flash,src_addr,count,(void *)dst_addr);
	if (ret) {
		printf("sfc read error\n");
		return -1;
	}

	return 0;
}

int sfc_nor_write(unsigned char sfc_index,struct spi_flash *flash, unsigned int src_addr, unsigned int count, const void *dst_addr)
{
	int ret = 0;

#ifdef CONFIG_SPI_QUAD
	sfc_quad_mode = 1;
#endif

	if(sfc_quad_mode == 1){
		if(quad_mode_is_set == 0){
			sfc_set_quad_mode(sfc_index);
		}
	}

#ifdef CONFIG_BURNER
	if(src_addr == 0){
		write_norflash_params_to_spl(dst_addr);
	}
#endif

	jz_sfc_writel(sfc_index,1 << 2,SFC_TRIG);

	ret = jz_sfc_write(sfc_index,flash,src_addr,count,(const void *)dst_addr);
	if (ret) {
		printf("sfc write error\n");
		return -1;
	}

	return 0;
}

int sfc_nor_erase(unsigned char sfc_index,struct spi_flash *flash, unsigned int src_addr, unsigned int count)
{
	int ret = 0;

#ifdef CONFIG_SPI_QUAD
	sfc_quad_mode = 1;
#endif

	if(sfc_quad_mode == 1){
		if(quad_mode_is_set == 0){
			sfc_set_quad_mode(sfc_index);
		}
	}

	jz_sfc_writel(sfc_index,1 << 2,SFC_TRIG);
	ret = jz_sfc_erase(sfc_index,flash,src_addr,count);
	if (ret) {
		printf("sfc erase error\n");
		return -1;
	}

	return 0;
}
#endif

static inline struct jz_sfc_slave *to_jz_sfc(struct spi_slave *slave)
{
	return container_of(slave, struct jz_sfc_slave, slave);
}

struct spi_slave *spi_setup_slave(unsigned char sfc_index,unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct jz_sfc_slave *ss;

	ss = spi_alloc_slave(struct jz_sfc_slave, bus, cs);
	if (!ss)
		return NULL;

	ss->mode = mode;
	ss->max_hz = max_hz;

	sfc_init(sfc_index);
	return &ss->slave;
}

int spi_claim_bus(struct spi_slave *slave)
{
	/* nothing doing */

	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	/* nothing doing */

	return ;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct jz_sfc_slave *ss = to_jz_sfc(slave);

	free(ss);
}

/* To be continued */
int spi_xfer(unsigned char sfc_index,struct spi_slave *slave, unsigned int bitlen, const void *dout, void *din, unsigned long flags)
{
		/*unsigned int idcode;*/
		/*sfc_nor_RDID(&idcode);*/
	unsigned int ret;
	unsigned int addr;
	unsigned int addr_len;
	unsigned int bytelen = bitlen / 8;
	unsigned int wordlen = (bytelen + 3) / 4;
	unsigned char *cmd = (unsigned char *)dout;

	if(flags == SPI_XFER_BEGIN) {
		spi_xfer_cmd_len = bytelen;
		if(spi_xfer_cmd_len > CMD_LEN) {
			printf("cmd(%x) len more %d\n", *(unsigned char *)dout, CMD_LEN);
			return -1;
		}

		memcpy(spi_xfer_cmd, (unsigned char *)dout, bytelen);

		jz_sfc_writel(sfc_index,TRIG_STOP, SFC_TRIG);
		jz_sfc_writel(sfc_index,TRIG_FLUSH, SFC_TRIG);
	}

	if(flags == 0) {
		sfc_transfer_direction(sfc_index,0);
		switch (spi_xfer_cmd_len) {
			case 1:
				addr = 0;
				addr_len = 0;
				break;
			default:
				printf("spi_xfer_cmd_len is %d\n", spi_xfer_cmd_len);
		}
		sfc_send_cmd(sfc_index,&spi_xfer_cmd[0],bytelen,addr,addr_len,0,1,0);
		unsigned int tmp[1];
		ret = sfc_read_data(sfc_index,tmp, bytelen);
		*(unsigned char *)din = (tmp[0] & 0xff);
		if (ret)
			return ret;
	}

	if(flags == (SPI_XFER_BEGIN | SPI_XFER_END)) {
		switch (bytelen) {
			case 1:
				addr = 0;
				addr_len = 0;
				break;
			case 4:
				addr = (cmd[1] << 16) | (cmd[2] << 8) | cmd[3];
				addr_len = 3;
				break;
			default:
				printf("len is %d\n", bytelen);
		}

		sfc_send_cmd(sfc_index,&spi_xfer_cmd[0],bytelen,addr,addr_len,0,1,0);
	}

	if(flags == SPI_XFER_END) {
		jz_sfc_writel(sfc_index,(wordlen * 4), SFC_TRAN_LEN);
		if(dout != NULL) { /* write */
			printf("????");
		}

		if(din != NULL) { /* read */
			switch (spi_xfer_cmd_len) {
				case 1:
					addr = 0;
					addr_len = 0;
					break;
				case 4:
					addr = (spi_xfer_cmd[1] << 16) | (spi_xfer_cmd[2] << 8) | spi_xfer_cmd[3];
					addr_len = 3;
					break;
				case 5:
					addr = (spi_xfer_cmd[1] << 16) | (spi_xfer_cmd[2] << 8) | spi_xfer_cmd[3];
					addr_len = 3;
					sfc_dev_addr_dummy_bytes(sfc_index,0, 8);
					sfc_set_mode(sfc_index,0, 0);
					break;
				default:
					printf("spi_xfer_cmd_len is %d\n", spi_xfer_cmd_len);
			}
			sfc_send_cmd(sfc_index,&spi_xfer_cmd[0],wordlen*4,addr,addr_len,0,1,0);
			ret = sfc_read_data(sfc_index,(unsigned int *)din, wordlen);
			if (ret)
				return ret;
		}
		jz_sfc_writel(sfc_index,TRIG_STOP, SFC_TRIG);
		jz_sfc_writel(sfc_index,TRIG_FLUSH, SFC_TRIG);
	}
	return 0;
}

struct spi_flash *spi_flash_probe_ingenic(struct spi_slave *spi, u8 *idcode)
{
	unsigned int id;
	struct spi_flash *flash;

	id = (idcode[0] << 16) | (idcode[1] << 8) | (idcode[2]);
	if (sfc_nor_init(id) < 0)
		return NULL;

	flash = spi_flash_alloc_base(spi, gparams.name);
	if (!flash) {
		printf("ingenic: Failed to allocate memory\n");
		return NULL;
	}
	flash->read = sfc_nor_read;
	flash->erase = sfc_nor_erase;
	flash->write = sfc_nor_write;

	flash->page_size = gparams.page_size;
	flash->sector_size = gparams.sector_size;
	flash->size = gparams.size;
	flash->addr_size = gparams.addr_size;

	return flash;
}

#ifdef CONFIG_CMD_SFC_NOR_FEATURES
int sfc_do_nor_features(int argc, char *const argv[])
{
    unsigned char cmd[4];
    unsigned int WR_DATE_SIZE = 1;
    unsigned int tmp = 0;
    unsigned int sfc_index = 0;
    unsigned char data[128] = {0};
    unsigned int len, addr, addr_len, dummy_bit, daten, dir;
    if (argc < 2)
        return -1;
    cmd[0] = CMD_WREN;

    if (strcmp(argv[1], "r") == 0)
    {
        if (argc != 3)
            return -1;
        cmd[1] = simple_strtoul(argv[2], NULL, 16);
        sfc_send_cmd(0, &cmd[1], 1, 0, 0, 0, 1, 0);
        udelay(25000);
        sfc_read_data(0, &tmp, 1);
        tmp &= 0xff;
        printf("0x%02x\n", tmp);
        return 0;
    }
    else if (strcmp(argv[1], "w") == 0)
    {
        if (argc != 4)
            return -1;
        cmd[1] = simple_strtoul(argv[2], NULL, 16);
        tmp = simple_strtoul(argv[3], NULL, 16);
        sfc_send_cmd(0, &cmd[0], 0, 0, 0, 0, 0, 1);
        sfc_send_cmd(0, &cmd[1], 1, 0, 0, 0, 1, 1);
        sfc_write_data(0, &tmp, WR_DATE_SIZE);
        return 0;
    }
    else if (strcmp(argv[1], "sfc") == 0)
    {
        if (argc != 11)
        {
            printf("dnor sfc read <sfc_index> <cmd> <len_byte> <addr> <addr_len> <dummy_bit> <daten> <dir>\n\n");
            return -1;
        }
        else if (strcmp(argv[2], "read") == 0)
        {
            sfc_index = simple_strtoul(argv[3], NULL, 16);
            cmd[1]    = simple_strtoul(argv[4], NULL, 16);
            len       = simple_strtoul(argv[5], NULL, 10);
            addr      = simple_strtoul(argv[6], NULL, 16);
            addr_len  = simple_strtoul(argv[7], NULL, 10);
            dummy_bit = simple_strtoul(argv[8], NULL, 10);
            daten     = simple_strtoul(argv[9], NULL, 10);
            dir       = simple_strtoul(argv[10], NULL, 10);
            dir       = 0;
            sfc_send_cmd(sfc_index, &cmd[1], len, addr, addr_len, dummy_bit, daten, dir);
            sfc_read_data(sfc_index, &data, len);
            printf("get data: \n");
            for (tmp = 0; tmp < len; tmp++)
            {
                printf("%x  ", data[tmp]);
                if (((tmp + 1) % 8 == 0) || (tmp == len - 1))
                    printf("\n");
            }

            return 0;
        }
    }

    return -1;
}
#endif
