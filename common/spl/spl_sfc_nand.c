#include <common.h>
#include <config.h>
#include <spl.h>
#include <asm/io.h>
#include <nand.h>
#include <asm/arch/sfc.h>

#define UBOOT_AUTO_MAX_SIZE (0x100000)
#define SPL_SIZE	(16 * 1024)

void sfc_read_page(unsigned int page,unsigned int dst_addr)
{

	/*** page_read_to_cache ***/
	writel(1<<THRESHOLD  | 1 <<PHASE_NUM,SFC_GLB) ;
	writel(1<<CE_DL | 1<<HOLD_DL | 1<<WP_DL,SFC_DEV_CONF);
	writel(3<<ADDR_WIDTH  | 1 <<CMD_EN |  0x13 <<TRAN_CMD,SFC_TRAN_CONF0);
	writel(0x0,SFC_TRAN_LEN);

	writel(page ,SFC_DEV_ADR0);
	writel(1 <<26 | 1 <<25 | 1<<24 | 1<<16 |0xf <<TRAN_CMD,SFC_TRAN_CONF1);

	writel(0xc0,SFC_DEV_ADR1);
	writel(0x1,SFC_DEV_STA_MSK);
	writel(0x0,SFC_STA_EXP);
	__sfc_start();
	while(!(__sfc_is_end()))
	{
		udelay(100);
	}
	writel(0x1f,SFC_SCR);
	/*** page_read_from_cache ***/
	writel(1<<THRESHOLD  | 1 <<PHASE_NUM,SFC_GLB);
	writel(1<<CE_DL | 1<<HOLD_DL | 1<<WP_DL,SFC_DEV_CONF);
	writel(3 <<ADDR_WIDTH | 1 <<CMD_EN |1<< DATA_EN |  0x3 <<TRAN_CMD,SFC_TRAN_CONF0);
	writel(0x800,SFC_TRAN_LEN);
	writel(0,SFC_DEV_ADR0);

	/*** read_buf ***/
	int i,j;
	unsigned char *data;
	unsigned int temp;
	__sfc_start();
	for(i=0;i<0x200;i++)
	{
		while(!(__sfc_rece_req()))
		{
			udelay(100);
		}
		writel(0x1f,SFC_SCR);
		temp = readl(SFC_DR);
		for(j=0;j<4;j++)
		{
			*((unsigned char *)(dst_addr) +j+i*4)=*((unsigned char*)(&temp) +j);
		}
	}
	while(!(__sfc_is_end()))
	{
		udelay(100);
	}
	writel(0x1f,SFC_SCR);
	return ;
}

void sfc_nand_load(long offs,long size,void *dst)
{
	int pagesize = 2048;
	int blocksize =2048 *64;


	int page=8;
	int pagecopy_cnt = 0;
	int page_per_blk;
	int blk;
	int ret;
	long totalsize = UBOOT_AUTO_MAX_SIZE;

	while (pagecopy_cnt * pagesize < totalsize) {
	sfc_read_page(page,(unsigned char *)dst);

	dst += pagesize;
	page++;
	pagecopy_cnt++;
	}
	return ;
}

void spl_sfc_nand_load_image(void)
{


	struct image_header *header;

	header = (struct image_header *)(CONFIG_SYS_TEXT_BASE);

	spl_parse_image_header(header);

	sfc_nand_load(-1, -1,(void *)CONFIG_SYS_TEXT_BASE);


}






