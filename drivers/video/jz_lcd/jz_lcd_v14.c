/*
 * x1830/X2000 LCDC DRIVER
 *
 * Copyright (c) 2014 Ingenic Semiconductor Co.,Ltd
 * Author: Huddy <hyli@ingenic.cn>
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

#include <asm/io.h>
#include <config.h>
#include <serial.h>
#include <common.h>
#include <lcd.h>
/*#include <asm/arch/lcdc.h>*/
#include <asm/arch/gpio.h>
#include <asm/arch/clk.h>
#include <jz_lcd/jz_lcd_v14.h>
#include "dpu_reg.h"

#include <asm/gpio.h>

#define DEBUG

struct jzfb_config_info lcd_config_info;
static int lcd_enable_state = 0;
void board_set_lcd_power_on(void);
void flush_cache_all(void);
/* void lcd_close_backlight(void); */
/* void lcd_set_backlight_level(int num); */
#define fb_write(addr,config)				\
	writel(config,DPU_BASE+addr)
#define fb_read(addr)				\
	readl(DPU_BASE+addr)

#ifdef DEBUG



static void dump_lcdc_registers(void)
{
    printf("-----------------dc_reg------------------\n");
    printf("LCDC_TINY_CHAIN_ADDR:   %lx\n",fb_read(LCDC_TINY_CHAIN_ADDR));
    printf("LCDC_TINY_GLB_CFG:      %lx\n",fb_read(LCDC_TINY_GLB_CFG));
    printf("LCDC_TINY_CSC_MULT_YRV: %lx\n",fb_read(LCDC_TINY_CSC_MULT_YRV));
    printf("LCDC_TINY_CSC_MULT_GUGV:%lx\n",fb_read(LCDC_TINY_CSC_MULT_GUGV));
    printf("LCDC_TINY_CSC_MULT_BU:  %lx\n",fb_read(LCDC_TINY_CSC_MULT_BU));
    printf("LCDC_TINY_CSC_SUB_YUV:  %lx\n",fb_read(LCDC_TINY_CSC_SUB_YUV));
    printf("LCDC_TINY_CTRL:         %lx\n",fb_read(LCDC_TINY_CTRL));
    printf("LCDC_TINY_ST:           %lx\n",fb_read(LCDC_TINY_ST));
    printf("LCDC_TINY_CSR:          %lx\n",fb_read(LCDC_TINY_CSR));
    printf("LCDC_TINY_INTC:         %lx\n",fb_read(LCDC_TINY_INTC));
    printf("LCDC_TINY_PCFG:         %lx\n",fb_read(LCDC_TINY_PCFG));
    printf("LCDC_TINY_INT_FLAG:     %lx\n",fb_read(LCDC_TINY_INT_FLAG));
    printf("LCDC_TINY_CHAIN_SITE:   %lx\n",fb_read(LCDC_TINY_CHAIN_SITE));
    printf("LCDC_TINY_DMA_SITE:     %lx\n",fb_read(LCDC_TINY_DMA_SITE));
    printf("LCDC_TINY_Y_DMA_SITE:     %lx\n",fb_read(LCDC_TINY_Y_DMA_SITE));
    printf("LCDC_TINY_UV_DMA_SITE:     %lx\n",fb_read(LCDC_TINY_UV_DMA_SITE));
    printf("LCDC_TINY_DES_READ:       %lx\n",fb_read(LCDC_TINY_DES_READ));
    printf("LCDC_DISP_COMMON:       %lx\n",fb_read(LCDC_DISP_COMMON));
    printf("-----------------dc_reg------------------\n");

    printf("----------------tft_reg------------------\n");
    printf("LCDC_TFT_TIMING_HSYNC:       %lx\n",fb_read(LCDC_TFT_TIMING_HSYNC));
    printf("LCDC_TFT_TIMING_VSYNC:       %lx\n",fb_read(LCDC_TFT_TIMING_VSYNC));
    printf("LCDC_TFT_TIMING_HDE:         %lx\n",fb_read(LCDC_TFT_TIMING_HDE));
    printf("LCDC_TFT_TIMING_VDE:         %lx\n",fb_read(LCDC_TFT_TIMING_VDE));
    printf("LCDC_TFT_TRAN_CFG:           %lx\n",fb_read(LCDC_TFT_TRAN_CFG));
    printf("LCDC_TFT_ST:                 %lx\n",fb_read(LCDC_TFT_ST));
    printf("----------------tft_reg------------------\n");

    printf("---------------slcd_reg------------------\n");
    printf("LCDC_SLCD_PANEL_CFG     %lx\n",fb_read(LCDC_SLCD_PANEL_CFG));
    printf("LCDC_SLCD_WR_DUTY       %lx\n",fb_read(LCDC_SLCD_WR_DUTY));
    printf("LCDC_SLCD_TIMING        %lx\n",fb_read(LCDC_SLCD_TIMING));
    printf("LCDC_SLCD_FRM_SIZE      %lx\n",fb_read(LCDC_SLCD_FRM_SIZE));
    printf("LCDC_SLCD_SLOW_TIME     %lx\n",fb_read(LCDC_SLCD_SLOW_TIME));
    printf("LCDC_SLCD_CMD           %lx\n",fb_read(LCDC_SLCD_CMD));
    printf("LCDC_SLCD_ST            %lx\n",fb_read(LCDC_SLCD_ST));
    printf("---------------slcd_reg------------------\n");

		fb_write(LCDC_TINY_CTRL, (1 << 2));
		printf("====================Frame Descriptor register======================\n");
		printf("NextDesAddr             %lx\n",fb_read(LCDC_TINY_DES_READ));
		printf("BufferAddr_RGB_Y        %lx\n",fb_read(LCDC_TINY_DES_READ));
		printf("Stride_RGB_Y            %lx\n",fb_read(LCDC_TINY_DES_READ));
		printf("ChainCfg                %lx\n",fb_read(LCDC_TINY_DES_READ));
		printf("InterruptCtrl           %lx\n",fb_read(LCDC_TINY_DES_READ));
		printf("BufferAddr_UV           %lx\n",fb_read(LCDC_TINY_DES_READ));
		printf("Stride_UV               %lx\n",fb_read(LCDC_TINY_DES_READ));
		printf("=================Frame Descriptor register end======================\n");
		return;
}

#endif


struct jzfb_colormode {
	uint32_t mode;
	uint32_t color;
	uint32_t bits_per_pixel;
	uint32_t nonstd;
	struct fb_bitfield red;
	struct fb_bitfield green;
	struct fb_bitfield blue;
	struct fb_bitfield transp;
};

static struct jzfb_colormode jzfb_colormodes[] = {
	{
		.mode = LAYER_CFG_FORMAT_RGB888,
		.color = LAYER_CFG_COLOR_RGB,
		.bits_per_pixel = 32,
		.nonstd = 0,
#ifdef CONFIG_ANDROID
		.color = LAYER_CFG_COLOR_BGR,
		.red	= { .length = 8, .offset = 0, .msb_right = 0 },
		.green	= { .length = 8, .offset = 8, .msb_right = 0 },
		.blue	= { .length = 8, .offset = 16, .msb_right = 0 },
#else
		.color = LAYER_CFG_COLOR_RGB,
		.red	= { .length = 8, .offset = 16, .msb_right = 0 },
		.green	= { .length = 8, .offset = 8, .msb_right = 0 },
		.blue	= { .length = 8, .offset = 0, .msb_right = 0 },
#endif
		.transp	= { .length = 0, .offset = 0, .msb_right = 0 },
	}, {
		.mode = LAYER_CFG_FORMAT_ARGB8888,
		.bits_per_pixel = 32,
		.nonstd = 0,
#ifdef CONFIG_ANDROID
		.color = LAYER_CFG_COLOR_BGR,
		.red	= { .length = 8, .offset = 0, .msb_right = 0 },
		.green	= { .length = 8, .offset = 8, .msb_right = 0 },
		.blue	= { .length = 8, .offset = 16, .msb_right = 0 },
#else
		.color = LAYER_CFG_COLOR_RGB,
		.red	= { .length = 8, .offset = 16, .msb_right = 0 },
		.green	= { .length = 8, .offset = 8, .msb_right = 0 },
		.blue	= { .length = 8, .offset = 0, .msb_right = 0 },
#endif
		.transp	= { .length = 8, .offset = 24, .msb_right = 0 },
	}, {
		.mode = LAYER_CFG_FORMAT_RGB555,
		.color = LAYER_CFG_COLOR_RGB,
		.bits_per_pixel = 16,
		.nonstd = 0,
		.red	= { .length = 5, .offset = 10, .msb_right = 0 },
		.green	= { .length = 5, .offset = 5, .msb_right = 0 },
		.blue	= { .length = 5, .offset = 0, .msb_right = 0 },
		.transp	= { .length = 0, .offset = 0, .msb_right = 0 },
	}, {
		.mode = LAYER_CFG_FORMAT_ARGB1555,
		.color = LAYER_CFG_COLOR_RGB,
		.bits_per_pixel = 16,
		.nonstd = 0,
		.red	= { .length = 5, .offset = 10, .msb_right = 0 },
		.green	= { .length = 5, .offset = 5, .msb_right = 0 },
		.blue	= { .length = 5, .offset = 0, .msb_right = 0 },
		.transp	= { .length = 1, .offset = 15, .msb_right = 0 },
	}, {
		.mode = LAYER_CFG_FORMAT_RGB565,
		.color = LAYER_CFG_COLOR_RGB,
		.bits_per_pixel = 16,
		.nonstd = 0,
		.red	= { .length = 5, .offset = 11, .msb_right = 0 },
		.green	= { .length = 6, .offset = 5, .msb_right = 0 },
		.blue	= { .length = 5, .offset = 0, .msb_right = 0 },
		.transp	= { .length = 0, .offset = 0, .msb_right = 0 },
	}, {
		.mode = LAYER_CFG_FORMAT_YUV422,
		.bits_per_pixel = 16,
		.nonstd = LAYER_CFG_FORMAT_YUV422,
	},
};

static void jzfb_colormode_to_var(struct fb_var_screeninfo *var,
		struct jzfb_colormode *color)
{
	var->bits_per_pixel = color->bits_per_pixel;
	var->nonstd = color->nonstd;
	var->red = color->red;
	var->green = color->green;
	var->blue = color->blue;
	var->transp = color->transp;
}

static bool cmp_var_to_colormode(struct fb_var_screeninfo *var,
		struct jzfb_colormode *color)
{
	bool cmp_component(struct fb_bitfield *f1, struct fb_bitfield *f2)
	{
		return f1->length == f2->length &&
			f1->offset == f2->offset &&
			f1->msb_right == f2->msb_right;
	}

	if (var->bits_per_pixel == 0 ||
			var->red.length == 0 ||
			var->blue.length == 0 ||
			var->green.length == 0)
		return 0;

	return var->bits_per_pixel == color->bits_per_pixel &&
		cmp_component(&var->red, &color->red) &&
		cmp_component(&var->green, &color->green) &&
		cmp_component(&var->blue, &color->blue) &&
		cmp_component(&var->transp, &color->transp);
}

struct jzfb_framedesc g_framedesc[MAX_DESC_NUM] __attribute__((aligned(256)));
struct jzfb_layerdesc g_layerdesc[MAX_DESC_NUM][MAX_LAYER_NUM] __attribute__((aligned(256)));

static int jzfb_alloc_devmem(struct jzfb_config_info *info)
{
	int i;
	for(i = 0; i < MAX_DESC_NUM; i++) {
		/* uncached addr*/
		info->framedesc[i] = ((unsigned int)&g_framedesc[i]) | 0xa0000000;
		/* phys addr */
		info->framedesc_phys[i] = virt_to_phys((unsigned int)info->framedesc[i]);

			info->vidmem[i] = info->screen;
			info->vidmem_rgb_y_phys[i] = virt_to_phys((unsigned int)info->vidmem[i]);

	}
}


	/*unsigned int wb_buf[1024];*/

static int jzfb_prepare_dma_desc(struct jzfb_config_info *info)
{
    int i;
    struct fb_videomode *mode = info->modes;
		struct jzfb_framedesc **framedesc;

framedesc = info->framedesc;
    /* use for double buffer technology  */
    info->frm_size = mode->xres * mode->yres * 2;
    for(i = 0; i < MAX_DESC_NUM; i++) {
#ifdef CONFIG_SLCDC_USE_TE
        if(i >= (MAX_DESC_NUM-1))
            framedesc[i]->NextDesAddr = info->framedesc_phys[0];
        else
            framedesc[i]->NextDesAddr = info->framedesc_phys[i+1];
#else	/* CONFIG_SLCDC_USE_TE */
        framedesc[i]->NextDesAddr = info->framedesc_phys[i];
#endif	/* CONFIG_SLCDC_USE_TE */
        framedesc[i]->BufferAddr_RGB_Y = info->vidmem_rgb_y_phys[i];
        framedesc[i]->Stride_RGB_Y =  mode->xres;
        framedesc[i]->ChainCfg = 0x1;
        framedesc[i]->InterruptCtrl = fb_read(LCDC_TINY_INTC);
//        framedesc[i]->BufferAddr_UV = info->vidmem_uv_phys[i];
        framedesc[i]->Stride_UV = mode->xres;
    }
    fb_write(LCDC_TINY_CHAIN_ADDR, info->framedesc_phys[0]);
    return 0;
}



#if defined(CONFIG_LCD_LOGO)
static void fbmem_set(void *_ptr, unsigned short val, unsigned count)
{
	int bpp = NBITS(panel_info.vl_bpix);
	if(bpp == 16){
		unsigned short *ptr = _ptr;

		while (count--)
			*ptr++ = val;
	} else if (bpp == 32){
		int val_32;
		int alpha, rdata, gdata, bdata;
		unsigned int *ptr = (unsigned int *)_ptr;

		alpha = 0xff;

		rdata = val >> 11;
		rdata = (rdata << 3) | 0x7;

		gdata = (val >> 5) & 0x003F;
		gdata = (gdata << 2) | 0x3;

		bdata = val & 0x001F;
		bdata = (bdata << 3) | 0x7;

		if (lcd_config_info.fmt_order == FORMAT_X8B8G8R8) {
			val_32 =
				(alpha << 24) | (bdata << 16) | (gdata << 8) | rdata;
			/*fixed */
		} else if (lcd_config_info.fmt_order == FORMAT_X8R8G8B8) {
			val_32 =
				(alpha << 24) | (rdata << 16) | (gdata << 8) | bdata;
		}

		while (count--){
			*ptr++ = val_32;
		}
	}
}
 /*
  *565RLE image format: [count(2 bytes), rle(2 bytes)]
	*/
void rle_plot_biger(unsigned short *src_buf, unsigned short *dst_buf, int bpp)
{
	int vm_width, vm_height;
	int photo_width, photo_height, photo_size;
	int dis_width, dis_height;
	unsigned int write_count;
	int flag_bit = (bpp == 16) ? 1 : 2;
	unsigned short *photo_ptr = (unsigned short *)src_buf;
	unsigned short *lcd_fb = (unsigned short *)dst_buf;

	vm_width = panel_info.vl_col;
	vm_height = panel_info.vl_row;

	photo_width = photo_ptr[0];
	photo_height = photo_ptr[1];
	photo_size = photo_ptr[3] << 16 | photo_ptr[2]; 	//photo size
	debug("photo_size =%d photo_width = %d, photo_height = %d\n", photo_size,
				photo_width, photo_height);
	photo_ptr += 4;

	dis_height = photo_height < vm_height ? photo_height : vm_height;

	write_count = photo_ptr[0];
	while (photo_size > 0) {
		while (dis_height > 0) {
			dis_width = photo_width < vm_width ? photo_width : vm_width;
			while (dis_width > 0) {
				if (photo_size < 0)
					break;
				fbmem_set(lcd_fb, photo_ptr[1], write_count);
				lcd_fb += write_count * flag_bit;
				photo_ptr += 2;
				photo_size -= 2;
				write_count = photo_ptr[0];
				dis_width -= write_count;
			}
			if (dis_width < 0) {
				photo_ptr -= 2;
				photo_size += 2;
				lcd_fb += dis_width * flag_bit;
				write_count = - dis_width;
			}
			int extra_width = photo_width - vm_width;
			while (extra_width > 0) {
				photo_ptr += 2;
				photo_size -= 2;
				write_count = photo_ptr[0];
				extra_width -= write_count;
			}
			if (extra_width < 0) {
				photo_ptr -= 2;
				photo_size += 2;
				write_count = -extra_width;
			}
			dis_height -= 1;
		}
		if (dis_height <= 0)
			break;
	}
	return;
}

#ifdef CONFIG_LOGO_EXTEND
#ifndef  CONFIG_PANEL_DIV_LOGO
#define  CONFIG_PANEL_DIV_LOGO   (3)
#endif
void rle_plot_extend(unsigned short *src_buf, unsigned short *dst_buf, int bpp)
{
	int vm_width, vm_height;
	int photo_width, photo_height, photo_size;
	int dis_width, dis_height, ewidth, eheight;
	unsigned short compress_count, compress_val, write_count;
	unsigned short compress_tmp_count, compress_tmp_val;
	unsigned short *photo_tmp_ptr;
	unsigned short *photo_ptr;
	unsigned short *lcd_fb;
	int rate0, rate1, extend_rate;
	int i, 	test;

	int flag_bit = 1;
	if(bpp == 16){
		flag_bit = 1;
	}else if(bpp == 32){
		flag_bit = 2;
	}

	vm_width = panel_info.vl_col;
	vm_height = panel_info.vl_row;
	photo_ptr = (unsigned short *)src_buf;
	lcd_fb = (unsigned short *)dst_buf;

	rate0 =  vm_width/photo_ptr[0] > CONFIG_PANEL_DIV_LOGO ? (vm_width/photo_ptr[0])/CONFIG_PANEL_DIV_LOGO : 1;
	rate1 =  vm_height/photo_ptr[1] > CONFIG_PANEL_DIV_LOGO ? (vm_width/photo_ptr[1])/CONFIG_PANEL_DIV_LOGO : 1;
	extend_rate = rate0 < rate1 ?  rate0 : rate1;
	debug("logo extend rate = %d\n", extend_rate);

	photo_width = photo_ptr[0] * extend_rate;
	photo_height = photo_ptr[1] * extend_rate;
	photo_size = ( photo_ptr[3] << 16 | photo_ptr[2]) * extend_rate; 	//photo size
	debug("photo_size =%d photo_width = %d, photo_height = %d\n", photo_size,
				photo_width, photo_height);
	photo_ptr += 4;

	dis_height = photo_height < vm_height ? photo_height : vm_height;
	ewidth = (vm_width - photo_width)/2;
	eheight = (vm_height - photo_height)/2;
	compress_count = compress_tmp_count= photo_ptr[0] * extend_rate;
	compress_val = compress_tmp_val= photo_ptr[1];
	photo_tmp_ptr = photo_ptr;
	write_count = 0;
	debug("0> photo_ptr = %08x, photo_tmp_ptr = %08x\n", photo_ptr, photo_tmp_ptr);
	while (photo_size > 0) {
		if (eheight > 0) {
			lcd_fb += eheight * vm_width * flag_bit;
		}
		while (dis_height > 0) {
			for(i = 0; i < extend_rate && dis_height > 0; i++){
				debug("I am  here\n");
				photo_ptr = photo_tmp_ptr;
				debug("1> photo_ptr = %08x, photo_tmp_ptr = %08x\n", photo_ptr, photo_tmp_ptr);
				compress_count = compress_tmp_count;
				compress_val = compress_tmp_val;

				dis_width = photo_width < vm_width ? photo_width : vm_width;
				if (ewidth > 0) {
					lcd_fb += ewidth*flag_bit;
				}
				while (dis_width > 0) {
					if (photo_size < 0)
						break;
					write_count = compress_count;
					if (write_count > dis_width)
						write_count = dis_width;

					fbmem_set(lcd_fb, compress_val, write_count);
					lcd_fb += write_count * flag_bit;
					if (compress_count > write_count) {
						compress_count = compress_count - write_count;
					} else {
						photo_ptr += 2;
						photo_size -= 2;
						compress_count = photo_ptr[0] * extend_rate;
						compress_val = photo_ptr[1];
					}

					dis_width -= write_count;
				}

				if (ewidth > 0) {
					lcd_fb += ewidth * flag_bit;
				} else {
					int xwidth = -ewidth;
					while (xwidth > 0) {
						unsigned write_count = compress_count;

						if (write_count > xwidth)
							write_count = xwidth;

						if (compress_count > write_count) {
							compress_count = compress_count - write_count;
						} else {
							photo_ptr += 2;
							photo_size -= 2;
							compress_count = photo_ptr[0];
							compress_val = photo_ptr[1];
						}
						xwidth -= write_count;
					}

				}
				dis_height -= 1;
			}
			photo_tmp_ptr = photo_ptr;
			debug("2> photo_ptr = %08x, photo_tmp_ptr = %08x\n", photo_ptr, photo_tmp_ptr);
			compress_tmp_count = compress_count;
			compress_tmp_val = compress_val;
		}

		if (eheight > 0) {
			lcd_fb += eheight * vm_width *flag_bit;
		}
		if (dis_height <= 0)
			return;
	}
	return;
}
#endif

void rle_plot_smaller(unsigned short *src_buf, unsigned short *dst_buf, int bpp)
{
	int vm_width, vm_height;
	int photo_width, photo_height, photo_size;
	int dis_width, dis_height, ewidth, eheight;
	unsigned short compress_count, compress_val, write_count;
	unsigned short *photo_ptr;
	unsigned short *lcd_fb;
	int flag_bit = 1;
	if(bpp == 16){
		flag_bit = 1;
	}else if(bpp == 32){
		flag_bit = 2;
	}

	vm_width = panel_info.vl_col;
	vm_height = panel_info.vl_row;
	photo_ptr = (unsigned short *)src_buf;
	lcd_fb = (unsigned short *)dst_buf;

	photo_width = photo_ptr[0];
	photo_height = photo_ptr[1];
	photo_size = photo_ptr[3] << 16 | photo_ptr[2]; 	//photo size
	debug("photo_size =%d photo_width = %d, photo_height = %d\n", photo_size,
				photo_width, photo_height);
	photo_ptr += 4;

	dis_height = photo_height < vm_height ? photo_height : vm_height;
	ewidth = (vm_width - photo_width)/2;
	eheight = (vm_height - photo_height)/2;
	compress_count = photo_ptr[0];
	compress_val = photo_ptr[1];
	write_count = 0;
	while (photo_size > 0) {
		if (eheight > 0) {
			lcd_fb += eheight * vm_width * flag_bit;
		}
		while (dis_height > 0) {
			dis_width = photo_width < vm_width ? photo_width : vm_width;
			if (ewidth > 0) {
				lcd_fb += ewidth*flag_bit;
			}
			while (dis_width > 0) {
				if (photo_size < 0)
					break;
				write_count = compress_count;
				if (write_count > dis_width)
					write_count = dis_width;
				fbmem_set(lcd_fb, compress_val, write_count);
				lcd_fb += write_count * flag_bit;
				if (compress_count > write_count) {
					compress_count = compress_count - write_count;
				} else {
					photo_ptr += 2;
					photo_size -= 2;
					compress_count = photo_ptr[0];
					compress_val = photo_ptr[1];
				}

				dis_width -= write_count;
			}

			if (ewidth > 0) {
				lcd_fb += ewidth * flag_bit;
			} else {
				int xwidth = -ewidth;
				while (xwidth > 0) {
					unsigned write_count = compress_count;

					if (write_count > xwidth)
						write_count = xwidth;

					if (compress_count > write_count) {
						compress_count = compress_count - write_count;
					} else {
						photo_ptr += 2;
						photo_size -= 2;
						compress_count = photo_ptr[0];
						compress_val = photo_ptr[1];
					}
					xwidth -= write_count;
				}

			}
			dis_height -= 1;
		}

		if (eheight > 0) {
			lcd_fb += eheight * vm_width *flag_bit;
		}
		if (dis_height <= 0)
			return;
	}
	return;
}

void rle_plot(unsigned short *buf, unsigned char *dst_buf)
{
	int vm_width, vm_height;
	int photo_width, photo_height, photo_size;
	int flag;
	int bpp;


	unsigned short *photo_ptr = (unsigned short *)buf;
	unsigned short *lcd_fb = (unsigned short *)dst_buf;
	bpp = NBITS(panel_info.vl_bpix);
	vm_width = panel_info.vl_col;
	vm_height = panel_info.vl_row;

	flag =  photo_ptr[0] * photo_ptr[1] - vm_width * vm_height;
	if(flag <= 0){
#ifdef CONFIG_LOGO_EXTEND
		rle_plot_extend(photo_ptr, lcd_fb, bpp);
#else
		rle_plot_smaller(photo_ptr, lcd_fb, bpp);
#endif
	}else if(flag > 0){
		rle_plot_biger(photo_ptr, lcd_fb, bpp);
	}
	return;
}

#else
void rle_plot(unsigned short *buf, unsigned char *dst_buf)
{
}
#endif
void fb_fill(void *logo_addr, void *fb_addr, int count)
{
	//memcpy(logo_buf, fb_addr, count);
	int i;
	int *dest_addr = (int *)fb_addr;
	int *src_addr = (int *)logo_addr;

	for(i = 0; i < count; i = i + 4){
		*dest_addr =  *src_addr;
		src_addr++;
		dest_addr++;
	}

}

static void jzfb_slcd_start(struct jzfb_config_info *info)
{

    fb_write(LCDC_TINY_CTRL, SLCD_START);
}

static void jzfb_chain_start(struct jzfb_config_info *info)
{

    int count = 10000;
    while((fb_read(LCDC_TINY_ST) & WORKING) && count--) {
		udelay(10);
    }

    if(count > 0) {
		fb_write(LCDC_TINY_CTRL, CHAIN_START);
    } else {
		serial_puts("SLCDC Dma chain is busy \n");
    }

#if 0
	if(!(reg_read(jzfb, LCDC_TINY_ST) & WORKING))
        reg_write(jzfb, LCDC_TINY_CTRL, CHAIN_START);
    else
        printk("Dma chain has started!\n");
#endif
}


static void jzfb_cmp_start()
{
		uint32_t smart_cfg;
		smart_cfg = fb_read(LCDC_SLCD_PANEL_CFG);
		smart_cfg |= SLCD_FMT_EN;
    fb_write(LCDC_SLCD_PANEL_CFG, smart_cfg);
    /* when CHAINcfg is sigle mode, CHAIN_START must be setted */
    jzfb_chain_start(&lcd_config_info);
    jzfb_slcd_start(&lcd_config_info);
		//reg_write(jzfb, LCDC_TINY_CTRL, CHAIN_START);

}




static void slcd_send_mcu_command_data(struct jzfb_config_info *info, unsigned long data, unsigned int flag)
{
    int count = 10000;
    while ((fb_read(LCDC_SLCD_ST) & SLCD_ST_BUSY) && count--) {
        udelay(10);
    }
    if (count < 0) {
        serial_puts("SLCDC wait busy state wrong");
    }
    fb_write(LCDC_SLCD_CMD, flag | data);
		udelay(10);

}


static void wait_busy()
{
    int count = 10000;
    while ((fb_read(LCDC_SLCD_ST) & SLCD_ST_BUSY)
           && count--) {
        udelay(10);
    }
    if (count < 0) {
        serial_puts("SLCDC wait busy state wrong");
    }
}

void lcd_enable(void)
{
		slcd_send_mcu_command_data(&lcd_config_info, 0x2c, FLAG_CMD);
		wait_busy();
		jzfb_cmp_start();

//	dump_lcdc_registers();
		lcd_enable_state = 1;

		int count = 10000;
		while(!(fb_read(LCDC_TINY_ST) & FRM_END) && count--) {
				udelay(10);
		}

		printf("LCD_TINY_ST =%x\n", fb_read(LCDC_TINY_ST));
		return;
}

static void lcd_disable(struct jzfb_config_info *info, stop_mode_t stop_md)
{

    if(!(fb_read(LCDC_TINY_INTC) & FRM_END)) {
        while(!(fb_read(LCDC_TINY_ST) & FRM_END))
            printf("wait frame end.\n");
        fb_write(LCDC_TINY_CTRL, QCK_STOP);
        fb_write(LCDC_TINY_CSR, FRM_END);
    } else if((fb_read(LCDC_DISP_COMMON) & DISP_COM_SLCD)
              && !(fb_read(LCDC_SLCD_PANEL_CFG) & SLCD_CONTINUA_MODE)
              && stop_md){
        fb_write(LCDC_TINY_CTRL, QCK_STOP);
    }

	lcd_enable_state = 0;
	return;
}


static void jzfb_slcd_mcu_init(struct jzfb_config_info *info)
{

	unsigned int is_enabled, i,j;
	unsigned long tmp;
	struct jzfb_smart_config *smart_config;
	struct smart_lcd_data_table *data_table;
	uint32_t length_data_table;
	stop_mode_t stop_md;
	int count = 10000;

	if (info->lcd_type != LCD_TYPE_SLCD)
		return;

	is_enabled = lcd_enable_state;
	 /*
		*if (!is_enabled) {
		*  lcd_enable();
		*}
		*/
	smart_config = info->smart_config;
	data_table = smart_config->data_table;
	length_data_table = smart_config->length_data_table;


	if (length_data_table &&data_table) {
		for (i = 0; i < length_data_table; i++) {
 fb_write(LCDC_SLCD_PANEL_CFG,
                      fb_read(LCDC_SLCD_PANEL_CFG) & ~(SLCD_FMT_EN));
			switch (data_table[i].type) {
				case SMART_CONFIG_DATA:
					slcd_send_mcu_command_data(info,data_table[i].value,FLAG_PAR);
					break;
				case SMART_CONFIG_CMD:
					slcd_send_mcu_command_data(info,data_table[i].value,FLAG_CMD);
					break;
				case SMART_CONFIG_UDELAY:
					udelay(data_table[i].value);
					break;
				default:
					serial_puts("Unknow SLCD data type\n");
					break;
			}
		}

 while ((fb_read(LCDC_SLCD_ST) & SLCD_ST_BUSY)
               && count--) {
            udelay(10);
        }
        if (count < 0) {
            serial_puts("SLCDC wait busy state wrong");
        }

	}
}



void slcd_set_mcu_register(struct jzfb_config_info *info,unsigned long cmd, unsigned long data)
{
	slcd_send_mcu_command(info, cmd);
	slcd_send_mcu_data(info, data);
}



static int jzfb_slcd_set_par(struct jzfb_config_info *info)
{
	int i,j;
	struct fb_videomode *mode = info->modes;
  uint32_t cfg;
  uint32_t smart_cfg;
	uint32_t smart_frm_size;
	uint32_t smart_wtime;
	uint32_t smart_timing;
	uint32_t smart_slowtime;
	int bpp = 16;

  cfg = 0;
	smart_cfg = 0;
	smart_wtime = 0;
	smart_timing = 0;
	smart_slowtime = 0;
	smart_frm_size = 0;

   switch(info->smart_config->bus_width){
    case 8:
        smart_cfg |= SMART_LCD_CWIDTH_8_BIT;
        smart_cfg |= SMART_LCD_DWIDTH_8_BIT;
        break;
    case 9:
        smart_cfg |= SMART_LCD_CWIDTH_9_BIT;
        smart_cfg |= SMART_LCD_DWIDTH_9_BIT;
        break;
    case 16:
        smart_cfg |= SMART_LCD_CWIDTH_16_BIT;
        smart_cfg |= SMART_LCD_DWIDTH_16_BIT;
        break;
    case 18:
        smart_cfg |= SMART_LCD_CWIDTH_18_BIT;
        smart_cfg |= SMART_LCD_DWIDTH_18_BIT;
        break;
    case 24:
        smart_cfg |= SMART_LCD_CWIDTH_24_BIT;
        smart_cfg |= SMART_LCD_DWIDTH_24_BIT;
        break;
    default:
        printf("ERR: please check out your bus width config\n");
        break;
    }

 if (info->smart_config->clkply_active_rising)
        smart_cfg |= SLCD_CLK_PLY;
    if (info->smart_config->rsply_cmd_high)
        smart_cfg |= SLCD_DC_MD;
    if (info->smart_config->csply_active_high)
        smart_cfg |= (1 << 11);

    smart_cfg |= SLCD_CS_DP;
    smart_cfg |= SLCD_RDY_DP;
    /* debug WR active status  */
    smart_cfg |= SLCD_WR_DP;
    /* smart_cfg &= ~SLCD_WR_DP; */
    smart_cfg &= ~SLCD_SWITCH_WAIT_TE;
    smart_cfg |= SLCD_CONTINUA_MODE;

#ifdef CONFIG_SLCDC_CONTINUA
    smart_cfg |= SLCD_CONTINUA_MODE;
#endif
#if 1
    switch (bpp) {
    case 16:
        smart_cfg |= SMART_LCD_FORMAT_565;
        break;
    case 18:
        smart_cfg |= SMART_LCD_FORMAT_666;
        break;
    case 24:
        smart_cfg |= SMART_LCD_FORMAT_888;
        break;
    default:
        printf("ERR: please check out your bpp config\n");
        break;
    }

#endif
    if (info->smart_config->newcfg_6800_md)
        smart_cfg |= SLCD_DBI_TYPE_6800;
    else
        smart_cfg |= SLCD_DBI_TYPE_8080;

    smart_frm_size |= mode->yres << 16;
    smart_frm_size |= mode->xres;

    jzfb_prepare_dma_desc(info);

#ifdef LCDC_TINY_NV12
    cfg |= LCD_GLB_FORMAT_NV12;
    cfg |= DMA_SEL_NV12;
#else /* LCDC_TINY_NV12 */
    cfg |= LCD_GLB_COLOR_RGB;
    switch (bpp) {
    case 15:
        cfg |= LCD_GLB_FORMAT_555;
        break;
    case 16:
        cfg |= LCD_GLB_FORMAT_565;
        break;
    case 24:
        cfg |= LCD_GLB_FORMAT_888;
        break;
    default:
        printf("ERR: please check out your bpp config\n");
        break;
    }
    /*cfg |= CLKGATE_CLS;*/
    cfg |= BURST_LEN;
    cfg |= DMA_SEL_RGB;
#endif /* LCDC_TINY_NV12 */

		fb_read(LCDC_SLCD_PANEL_CFG);
    fb_write(LCDC_SLCD_PANEL_CFG, smart_cfg);
    fb_write(LCDC_SLCD_FRM_SIZE, smart_frm_size);
    fb_write(LCDC_SLCD_WR_DUTY, smart_wtime);
    fb_write(LCDC_SLCD_TIMING, smart_timing);
    fb_write(LCDC_SLCD_SLOW_TIME, smart_slowtime);
    fb_write(LCDC_TINY_GLB_CFG, cfg);

    jzfb_slcd_mcu_init(info);

//	dump_lcdc_registers();
#ifdef CONFIG_SLCDC_USE_TE
    smart_cfg &= ~SLCD_TE_ANTI_JIT;
    smart_cfg |= SLCD_TE_MD;
    smart_cfg |= SLCD_SWITCH_WAIT_TE;
    fb_write(LCDC_SLCD_PANEL_CFG, smart_cfg);

#endif

	return 0;
}


static struct fb_videomode *jzfb_get_mode(struct fb_var_screeninfo *var,
                                          struct jzfb_config_info *info)
{
    size_t i;
    struct fb_videomode *mode = info->modes;

    for (i = 0; i < info->num_modes; ++i, ++mode) {
        if (mode->flag & FB_MODE_IS_VGA) {
            if (mode->xres == var->xres &&
                mode->yres == var->yres
                && mode->pixclock == var->pixclock)
                return mode;
        } else {
            if (mode->xres == var->xres && mode->yres == var->yres
                && mode->vmode == var->vmode
                && mode->right_margin == var->right_margin) {
                if (lcd_config_info.lcd_type != LCD_TYPE_SLCD) {
                    if (mode->pixclock == var->pixclock)
                        return mode;
                } else {
                    return mode;
                }
            }
        }
    }

    return NULL;
}


static void jzfb_videomode_to_var(struct fb_var_screeninfo *var,
		const struct fb_videomode *mode)
{
	var->xres = mode->xres;
	var->yres = mode->yres;
	var->xres_virtual = mode->xres;
	var->yres_virtual = mode->yres * MAX_DESC_NUM * MAX_LAYER_NUM;
	var->xoffset = 0;
	var->yoffset = 0;
	var->left_margin = mode->left_margin;
	var->right_margin = mode->right_margin;
	var->upper_margin = mode->upper_margin;
	var->lower_margin = mode->lower_margin;
	var->hsync_len = mode->hsync_len;
	var->vsync_len = mode->vsync_len;
	var->sync = mode->sync;
	var->vmode = mode->vmode & FB_VMODE_MASK;
	var->pixclock = mode->pixclock;
}

static int jzfb_check_colormode(struct fb_var_screeninfo *var, uint32_t *mode)
{
	int i;

	if (var->nonstd) {
		for (i = 0; i < ARRAY_SIZE(jzfb_colormodes); ++i) {
			struct jzfb_colormode *m = &jzfb_colormodes[i];
			if (var->nonstd == m->nonstd) {
				jzfb_colormode_to_var(var, m);
				*mode = m->mode;
				return 0;
			}
		}

		return -22;
	}

	for (i = 0; i < ARRAY_SIZE(jzfb_colormodes); ++i) {
		struct jzfb_colormode *m = &jzfb_colormodes[i];
		if (cmp_var_to_colormode(var, m)) {
			jzfb_colormode_to_var(var, m);
			*mode = m->mode;
			return 0;
		}
	}

	return -22;
}


static int jz_lcd_init_mem(void *lcdbase, struct jzfb_config_info *info)
{
	unsigned long palette_mem_size;

	info->screen = (unsigned long)lcdbase;
	jzfb_alloc_devmem(info);

	return 0;
}


static int calc_refresh_ratio(struct jzfb_config_info *info)
{
	struct jzfb_smart_config *smart_config;
	smart_config = info->smart_config;

	switch(smart_config->smart_type){
	case SMART_LCD_TYPE_8080:
	case SMART_LCD_TYPE_6800:
		break;
	case SMART_LCD_TYPE_SPI_3:
		return 9;
	case SMART_LCD_TYPE_SPI_4:
		return 8;
	default:
		printf("%s %d err!\n",__func__,__LINE__);
		break;
	}

	switch(smart_config->pix_fmt) {
	case SMART_LCD_FORMAT_888:
		if(smart_config->dwidth == SMART_LCD_DWIDTH_8_BIT)
			return 3;
		if(smart_config->dwidth == SMART_LCD_DWIDTH_24_BIT)
			return 1;
	case SMART_LCD_FORMAT_565:
		if(smart_config->dwidth == SMART_LCD_DWIDTH_8_BIT)
			return 2;
		if(smart_config->dwidth == SMART_LCD_DWIDTH_16_BIT)
			return 1;
	default:
		printf("%s %d err!\n",__func__,__LINE__);
		break;
	}

	return 1;
}

static void refresh_pixclock_auto_adapt(struct jzfb_config_info *info)
{
	struct fb_videomode *mode;
	uint16_t hds, vds;
	uint16_t hde, vde;
	uint16_t ht, vt;
	struct fb_var_screeninfo *var = &info->var;
	unsigned long rate;
	unsigned int refresh_ratio = 1;

	mode = info->modes;
	if (mode == NULL) {
		printf("%s error: get video mode failed\n", __func__);
	}

	hds = mode->hsync_len + mode->left_margin;
	hde = hds + mode->xres;
	ht = hde + mode->right_margin;

	vds = mode->vsync_len + mode->upper_margin;
	vde = vds + mode->yres;
	vt = vde + mode->lower_margin;

	if (info->lcd_type == LCD_TYPE_SLCD) {
		refresh_ratio = calc_refresh_ratio(info);
	}

	hds = mode->hsync_len + mode->left_margin;
	hde = hds + mode->xres;
	ht = hde + mode->right_margin;

	vds = mode->vsync_len + mode->upper_margin;
	vde = vds + mode->yres;
	vt = vde + mode->lower_margin;

	if(mode->refresh){
		rate = mode->refresh * vt * ht * refresh_ratio;

		mode->pixclock = KHZ2PICOS(round_up(rate, 1000)/1000);
		var->pixclock = mode->pixclock;
	}else if(mode->pixclock){
		rate = PICOS2KHZ(mode->pixclock) * 1000;
		mode->refresh = rate / vt / ht / refresh_ratio;
	}else{
		printf("%s error:lcd important config info is absenced\n",__func__);
	}

}


static int jzfb_set_par(struct jzfb_config_info *info)
{
	struct fb_videomode *mode = info->modes;
//	struct fb_videomode *mode;
	struct fb_var_screeninfo *var = &info->var;
	struct jzfb_frmdesc_msg *desc_msg;
	int ret;
	int i;
	unsigned int intc = 0;

   fb_write(LCDC_TINY_CSR, 0x7e);	//mask of LCDC_TINY_CSR

    /* reg_write(jzfb, LCDC_TINY_INTC, FRM_END | DMA_END | GEN_STOPM | QCK_STOPM); */
    fb_write(LCDC_TINY_INTC, DMA_END);
    fb_write(LCDC_TINY_INTC, FRM_END);
    fb_write(LCDC_TINY_INTC, QCK_STOPM);
    /* reg_write(jzfb, LCDC_TINY_INTC, GEN_STOPM | QCK_STOPM); */
    if (lcd_config_info.lcd_type == LCD_TYPE_SLCD) {
        fb_write(LCDC_DISP_COMMON, DISP_COM_SLCD);
        jzfb_slcd_set_par(info);
    } else {
        fb_write(LCDC_DISP_COMMON, DISP_COM_TFT);
      //  jzfb_tft_set_par(info);
    }

	return 0;
}

void lcd_ctrl_init(void *lcd_base)
{
	unsigned long pixel_clock_rate;
	uint32_t colormode;
	int ret;

	/* init registers base address */
	memcpy(&lcd_config_info , &jzfb1_init_data , sizeof(jzfb1_init_data));
	lcd_config_info.lcdbaseoff = 0;


	lcd_set_flush_dcache(1);

	refresh_pixclock_auto_adapt(&lcd_config_info);
	pixel_clock_rate = PICOS2KHZ(lcd_config_info.modes->pixclock) * 1000;

	/* smart lcd WR freq = (lcd pixel clock)/2 */
	if (lcd_config_info.lcd_type == LCD_TYPE_SLCD) {
		pixel_clock_rate *= 2;
	}

	clk_set_rate(LCD, pixel_clock_rate);
	*((volatile unsigned int *)(0xb0000064)) = (0xa0000027);
	udelay(200);
	*((volatile unsigned int *)(0xb0000020)) = (0xf1a9efe0);

	/*lcd_close_backlight();*/
	panel_pin_init();

//	struct fb_videomode *mode = lcd_config_info.modes;
	jzfb_videomode_to_var(&lcd_config_info.var, lcd_config_info.modes);

	jzfb_colormode_to_var(&lcd_config_info.var, &jzfb_colormodes[4]);


#ifdef CONFIG_LCD_FORMAT_X8B8G8R8
	lcd_config_info.fmt_order = FORMAT_X8B8G8R8;
#else
	lcd_config_info.fmt_order = FORMAT_X8R8G8B8;
#endif

	ret = jzfb_check_colormode(&lcd_config_info.var, &colormode);
	if(ret) {
		printf("Check colormode failed!\n");
		return  ret;
	}

	jz_lcd_init_mem(lcd_base, &lcd_config_info);

	panel_power_on();

	jzfb_set_par(&lcd_config_info);
	return;
}

void lcd_show_board_info(void)
{
	return;
}

void lcd_setcolreg(ushort regno, ushort red, ushort green, ushort blue)
{
	return;
}

