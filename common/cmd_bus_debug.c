/*
 *bus err debug support
 */

#include <common.h>
#include <environment.h>
#include <command.h>
#include <image.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/ctype.h>

#ifdef CONFIG_CMD_BUS_DEBUG
#define READ_CP0(ID,SEL)		({uint32_t data =0;	\
   __asm__ __volatile__	(					\
			".set push 		\n"		\
			".set noat		\n"		\
			"mfc0 %0,$"#ID","#SEL"  \n"             \
		       ".set pop		\n"             \
			"ehb                \n"             \
	               :"=r"(data));			        \
	               data;})

#define WRITE_CP0(ID,SEL,VALUE)	                                \
   __asm__ __volatile__	(					\
			".set push 		\n"		\
			".set noat		\n"		\
			"mtc0 %0,$"#ID","#SEL"  \n"             \
		       ".set pop		\n"             \
			"ehb                \n"             \
	               :            			        \
	               :"r"(VALUE)          		        \
	               :);

static void get_deadlock_info()
{
	u32 becrctl,becrcnti,becrcntd,becradri,becradrd,riscv_errorepc;
	char status;
#if 0
	unsigned int tnpu_gate = 0;
	/* open tnpu_gate */
	tnpu_gate = *(volatile unsigned int*)0xb0000028;
	tnpu_gate &= ~(0x1 << 11);
	*(volatile unsigned int*)0xb0000028 = tnpu_gate;
#endif
	becrctl  =READ_CP0(9,4);
	becrcnti =READ_CP0(22,4);
	becrcntd =READ_CP0(22,5);
	becradri =READ_CP0(22,6);
	becradrd =READ_CP0(22,7);

	printf("get deadllock info : becrctl data is %x\n",becrctl);
	status=(becrctl>> 6)&0x1;
	printf("get deadllock info : fse rdy is %d\n",status);
	status=(becrctl>> 7)&0x1;
	printf("get deadllock info : cp0 rdy is %d\n",status);
	status=(becrctl>> 8)&0x1;
	printf("get deadllock info : mxu rdy is %d\n",status);
	status=(becrctl>> 9)&0x1;
	printf("get deadllock info : dbg rdy is %d\n",status);
	status=(becrctl>>10)&0x1;
	printf("get deadllock info : mmu rdy is %d\n",status);
	status=(becrctl>>11)&0x1;
	printf("get deadllock info : btb rdy is %d\n",status);
	status=(becrctl>>12)&0x1;
	printf("get deadllock info : div rdy is %d\n",status);
	status=(becrctl>>13)&0x1;
	printf("get deadllock info : dcc rdy is %d\n",status);
	status=(becrctl>>14)&0x1;
	printf("get deadllock info : icc rdy is %d\n",status);
	status=(becrctl>>15)&0x1;
	printf("get deadllock info : pipe rdy is %d\n",status);
	status=(becrctl>>16)&0x1;
	printf("get deadllock info : core sleep is %d\n",status);
	status=(becrctl>>17)&0x1;
	printf("get deadllock info : l2c  idle   is %d\n",status);
	status=(becrctl>>18)&0x1;
	printf("get deadllock info : ahb_hready  is %d\n",status);
	status=(becrctl>>19)&0x1;
	printf("get deadllock info : hbusreq_dbg is %d\n",status);
	status=(becrctl>>20)&0x1;
	printf("get deadllock info : rp_alive    is %d\n",status);
	status=(becrctl>>21)&0x1;
	printf("get deadllock info : ahb_wr_alive is %d\n",status);
	status=(becrctl>>22)&0x1;
	printf("get deadllock info : axi_wr_alive is %d\n",status);
	status=(becrctl>>23)&0x1;
	printf("get deadllock info : rap_alive    is %d\n",status);
	status=(becrctl>>24)&0x1;
	printf("get deadllock info : axi_rd_alive is %d\n",status);
	status=(becrctl>>25)&0x1;
	printf("get deadllock info : ahb_rd_alive is %d\n",status);
	status=(becrctl>>26)&0x3;
	printf("get deadllock info : ahb bus htrans is %d\n",status);
	status=(becrctl>>28)&0xf;
	printf("get deadllock info : ahb bus hburst is %d\n",status);
	printf("I counter is 0x%X,I ADDR is 0x%X\n",becrcnti,becradri);
	printf("D counter is 0x%X,D ADDR is 0x%X\n",becrcntd,becradrd);
#if 0
	*(volatile u32 *)(0x127000f0)=0x0;
	*(volatile u32 *)(0x127000f0)=0x80000000;
	riscv_errorepc=*(volatile u32 *)(0x12700030);
	printf("TNPU RISCV ERROR EPC is 0x%X\n",riscv_errorepc);
	riscv_errorepc=*(volatile u32 *)(0x12700080);
	printf("TNPU RISCV STATUS is%x\n",riscv_errorepc);
	status=(riscv_errorepc>>0)&0x1;
	printf("TNPU RISCV NNA PCE STATUS is%x\n",status);
	status=(riscv_errorepc>>1)&0x1;
	printf("TNPU RISCV NNA QOCB IDLE STATUS is%x\n",status);
	status=(riscv_errorepc>>2)&0x1;
	printf("TNPU RISCV NNA RD PIPE IDLE STATUS is%x\n",status);
	status=(riscv_errorepc>>3)&0x1;
	printf("TNPU RISCV NNA BE CTRL BLK  IDLE STATUS is%x\n",status);
	status=(riscv_errorepc>>4)&0x1;
	printf("TNPU RISCV NNA FE CTRL BLK  IDLE STATUS is%x\n",status);
	status=(riscv_errorepc>>5)&0x1;
	printf("TNPU RISCV NNA DMA CTRL IDLE STATUS is%x\n",status);
	status=(riscv_errorepc>>6)&0x1;
	printf("TNPU RISCV NNA BUF IDLE STATUS is%x\n",status);
	status=(riscv_errorepc>>7)&0x1;
	printf("TNPU RISCV NNA MAC IDLE STATUS is%x\n",status);
	status=(riscv_errorepc>>8)&0x1;
	printf("TNPU RISCV NNA I0 PIPE IDLE STATUS is%x\n",status);
    status=(riscv_errorepc>>9)&0x1;
	printf("TNPU RISCV NNA WAIT VALID %x\n",status);
	status=(riscv_errorepc>>10)&0x1;
	printf("TNPU RISCV ORAM CVA DATA READY is%x\n",status);
	status=(riscv_errorepc>>11)&0x1;
	printf("TNPU RISCV ORAM NNA DATA READY is%x\n",status);
	status=(riscv_errorepc>>12)&0x7;
	printf("TNPU RISCV ORAM WBTIN STATUS is%x\n",status);
	status=(riscv_errorepc>>15)&0x7;
	printf("TNPU RISCV ORAM FOUT STATUS is%x\n",status);
	status=(riscv_errorepc>>18)&0x7;
	printf("TNPU RISCV ORAM FIN STATUS is%x\n",status);
	status=(riscv_errorepc>>21)&0x1;
	printf("TNPU RISCV CVA IDLE  is%x\n",status);
	status=(riscv_errorepc>>22)&0x1;
	printf("TNPU RISCV NNA IDLE  is%x\n",status);
	status=(riscv_errorepc>>23)&0x1;
	printf("TNPU RISCV ORAM IDLE  is%x\n",status);
	status=(riscv_errorepc>>24)&0x1;
	printf("TNPU RISCV CORE IDLE  is%x\n",status);
#endif
	WRITE_CP0(9,4,0x1);
	WRITE_CP0(22,4,0x1111111);
	WRITE_CP0(22,5,0x2222222);
	WRITE_CP0(22,6,0x3333333);
	WRITE_CP0(22,7,0x4444444);
	becrctl  =READ_CP0(9,4);
	becrcnti =READ_CP0(22,4);
	becrcntd =READ_CP0(22,5);
	becradri =READ_CP0(22,6);
	becradrd =READ_CP0(22,7);
	printf("After init : becrctl data is %x\n",becrctl);
	printf("I counter is 0x%X,I ADDR is 0x%X\n",becrcnti,becradri);
	printf("D counter is 0x%X,D ADDR is 0x%X\n",becrcntd,becradrd);
}

static void busmonitor_debug(void)
{
	unsigned int monitor_gate = 0;
	unsigned int trans_cnt_en = 0;
	unsigned int tcu_1c0 = 0;
	unsigned int tcu_1c4 = 0;
	/* open monitor_gate */
	monitor_gate = *(volatile unsigned int*)0xb0000020;
	monitor_gate &= ~(0x1 << 16);
	*(volatile unsigned int*)0xb0000020 = monitor_gate;
	/* set TRANS_CNT_EN, monitor ch1 */
	trans_cnt_en = *(volatile unsigned int*)0xb30906f0;
	printf("trans_cnt_en is %d\n", trans_cnt_en);
	*(volatile unsigned int*)0xb30906f0 = 1;
	trans_cnt_en = *(volatile unsigned int*)0xb30906f0;
	printf("trans_cnt_en is %d\n", trans_cnt_en);
	/* get tcu 1c0/1c4 */
	tcu_1c0 = *(volatile unsigned int*)0xb00021c0;
	tcu_1c4 = *(volatile unsigned int*)0xb00021c4;
	printf("tcu_1c0 is %x\n", tcu_1c0);
	printf("tcu_1c4 is %x\n", tcu_1c4);
}

int do_bus_debug (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	printf("******get_deadlock_info******\n");
	/*
	 * pipe_rdy->0 表示CPU流水线卡死，具体是哪个模块卡死，可以看对应的rdy
	 * core sleep->表示当前cpu+l1 cache是否处于sleep状态
	 * l2c idle->表示当前l2c是否处于idle状态
	 * ahb hready->复位前cpu和外部ahb总线的hready状态
	 * hbusreq_dbg->复位前cpu和外部ahb总线的hbusreq的状态
	 * rp_alive->为1标识复位前l2c 和外部总线存在写数据未响应
	 * ahb_wr_alive->为1表示复位前ahb总线上有未完成的写操作
	 * axi_wr_alive->为1表示复位前axi总线上有未完成的写操作
	 * rap_alive->不清楚，和总线的地址请求相关
	 * axi_rd_alive->为1表示复位前ahb总线上有未完成的读操作
	 * ahb_rd_alive->为1表示复位前axi总线上有未完成的读操作
	 * ahb_bus_htrans->复位前cpu和外部ahb总线的htrans状态
	 * ahb_bus_hburst->复位前cpu和外部ahb总线的hburst状态
	 * */
	get_deadlock_info();
	printf("******busmonitor_debug******\n");
	/*
	 * reset后对应pass和fail情况如下：
	 * pass:1c0全0且(1c4是0x11aaaaa或者0x51aaaaa)
	 * fail:除pass情况外其余均为fail
	 * */
	busmonitor_debug();
	return 0;
}

U_BOOT_CMD(
	bus_debug,   1,  1,  do_bus_debug,
	"open or colse the bus debug",
	"<interface>\n"
);

#endif
