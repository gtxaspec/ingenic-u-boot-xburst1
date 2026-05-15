/* drivers/video/jz_fb_v14/jz_fb.h
 *
 * Copyright (C) 2016 Ingenic Semiconductor Inc.
 *
 * Author:clwang<chunlei.wang@ingenic.com>
 *
 * This program is free software, you can redistribute it and/or modify it
 *
 * under the terms of the GNU General Public License version 2 as published by
 *
 * the Free Software Foundation.
 */
#ifndef __JZ_LCD_V14_H__
#define __JZ_LCD_V14_H__

#include <linux/list.h>
#include <linux/fb.h>
#include <common.h>
#include <linux/types.h>

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#ifdef CONFIG_ONE_FRAME_BUFFERS
#define MAX_DESC_NUM 1
#endif

#ifdef CONFIG_TWO_FRAME_BUFFERS
#define MAX_DESC_NUM 2
#endif

#ifdef CONFIG_THREE_FRAME_BUFFERS
#define MAX_DESC_NUM 3
#endif

#define PICOS2KHZ(a) (1000000000/(a))
#define KHZ2PICOS(a) (1000000000/(a))
#define FB_SYNC_HOR_HIGH_ACT    1	/* horizontal sync high active  */
#define FB_SYNC_VERT_HIGH_ACT   2	/* vertical sync high active    */
#define FB_SYNC_EXT		4	/* external sync        */
#define FB_SYNC_COMP_HIGH_ACT   8	/* composite sync high active   */
#define FB_SYNC_BROADCAST	16	/* broadcast video timings      */
/* vtotal = 144d/288n/576i => PAL  */
/* vtotal = 121d/242n/484i => NTSC */
#define FB_SYNC_ON_GREEN	32	/* sync on green */

#define FB_VMODE_NONINTERLACED  0	/* non interlaced */
#define FB_VMODE_INTERLACED	1	/* interlaced       */
#define FB_VMODE_DOUBLE		2	/* double scan */
#define FB_VMODE_ODD_FLD_FIRST	4	/* interlaced: top line first */

#define PIXEL_ALIGN 4
#define DESC_ALIGN 8
#define MAX_DESC_NUM 1
#define MAX_LAYER_NUM 2
#define MAX_BITS_PER_PIX (32)

#define MAX_STRIDE_VALUE (4096)

#define FRAME_CTRL_DEFAULT_SET  (0x04)

#define FRAME_CFG_ALL_UPDATE (0xFF)

#define FB_MODE_IS_VGA    (1 << 30)

void panel_pin_init(void);
void panel_power_on(void);
void panel_power_off(void);
void panel_init_set_sequence(struct dsi_device *dsi);
void board_set_lcd_power_on(void);

#if PWM_BACKLIGHT_CHIP
void lcd_set_backlight_level(int num);
void lcd_close_backlight(void);
#else
void lcd_init_backlight(int num);
void send_low_pulse(int num);
void lcd_set_backlight_level(int num);
void lcd_close_backlight(void);
#endif

struct jz_fb_dma_descriptor {
        u_long fdadr;           /* Frame descriptor address register */
        u_long fsadr;           /* Frame source address register */
        u_long fidr;            /* Frame ID register */
        u_long ldcmd;           /* Command register */
        u_long offsize;         /* Stride Offsize(in word) */
        u_long page_width;      /* Stride Pagewidth(in word) */
        u_long cmd_num;         /* Command Number(for SLCD) */
        u_long desc_size;       /* Foreground Size */
};

/* smart lcd interface_type */
enum smart_lcd_type {
        SMART_LCD_TYPE_6800,
        SMART_LCD_TYPE_8080,
        SMART_LCD_TYPE_SPI_3,
        SMART_LCD_TYPE_SPI_4,
};

enum jzfb_lcd_type {
        LCD_TYPE_TFT = 0,
        LCD_TYPE_SLCD =1,
	LCD_TYPE_MIPI_SLCD =2,
	LCD_TYPE_MIPI_TFT =3,
};


/* smart lcd format */
enum smart_lcd_format {
        SMART_LCD_FORMAT_565,
        SMART_LCD_FORMAT_666,
        SMART_LCD_FORMAT_888,
};

enum smart_lcd_dwidth {
	SMART_LCD_DWIDTH_8_BIT,
	SMART_LCD_DWIDTH_9_BIT,
	SMART_LCD_DWIDTH_16_BIT,
	SMART_LCD_DWIDTH_18_BIT,
	SMART_LCD_DWIDTH_24_BIT,
};

enum smart_lcd_cwidth {
	SMART_LCD_CWIDTH_8_BIT,
	SMART_LCD_CWIDTH_9_BIT,
	SMART_LCD_CWIDTH_16_BIT,
	SMART_LCD_CWIDTH_18_BIT,
	SMART_LCD_CWIDTH_24_BIT,
};

enum {
	LAYER_CFG_FORMAT_RGB555 = 0,
	LAYER_CFG_FORMAT_ARGB1555 = 1,
	LAYER_CFG_FORMAT_RGB565	= 2,
	LAYER_CFG_FORMAT_RGB888	= 4,
	LAYER_CFG_FORMAT_ARGB8888 = 5,
	LAYER_CFG_FORMAT_NV12 = 8,
	LAYER_CFG_FORMAT_NV21 = 9,
	LAYER_CFG_FORMAT_YUV422 = 10,
	LAYER_CFG_FORMAT_TILE_H264 = 12,
};

enum {
		LCD_GLB_FORMAT_555 = (0 << 16),
		LCD_GLB_FORMAT_565 = (1 << 16),
		LCD_GLB_FORMAT_888 = (2 << 16),
		LCD_GLB_FORMAT_NV12 = (4 << 16),
		LCD_GLB_FORMAT_NV21 = (5 << 16),

};


enum jzfb_format_order {
        FORMAT_X8R8G8B8 = 1,
        FORMAT_X8B8G8R8,
};

enum {
	LAYER_CFG_COLOR_RGB = 0,
	LAYER_CFG_COLOR_RBG = 1,
	LAYER_CFG_COLOR_GRB = 2,
	LAYER_CFG_COLOR_GBR = 3,
	LAYER_CFG_COLOR_BRG = 4,
	LAYER_CFG_COLOR_BGR = 5,
};
enum {
  LCD_GLB_COLOR_RGB = (0 << 19),
  LCD_GLB_COLOR_RBG = (1 << 19),
  LCD_GLB_COLOR_GRB = (2 << 19),
  LCD_GLB_COLOR_BRG = (3 << 19),
  LCD_GLB_COLOR_GBR = (4 << 19),
  LCD_GLB_COLOR_BGR = (5 << 19),
};

typedef union frm_size {
	uint32_t d32;
	struct {
		uint32_t width:12;
	        uint32_t reserve12_15:4;
	        uint32_t height:12;
	        uint32_t reserve28_31:4;
	}b;
} frm_size_t;

typedef union frm_ctrl {
	uint32_t d32;
	struct {
		uint32_t stop:1;
		uint32_t bit1_keep0:1;
		uint32_t bit2_keep1:1;
		uint32_t bit3_4_keep0:2;
		uint32_t reserve5_31:27;
	}b;
} frm_ctrl_t;

typedef union lay_cfg_en {
	uint32_t d32;
	struct {
		uint32_t bit0_3_keep0:4;
		uint32_t lay0_en:1;
		uint32_t lay1_en:1;
		uint32_t bit6_7_keep0:2;
		uint32_t lay0_z_order:2;
		uint32_t lay1_z_order:2;
		uint32_t bit12_15_keep0:4;
		uint32_t reserve16_31:16;
	}b;
} lay_cfg_en_t;

typedef union cmp_irq_ctrl {
	uint32_t d32;
	struct {
		uint32_t reserve_0:1;
		uint32_t eof_msk:1;
		uint32_t sof_msk:1;
		uint32_t reserve3_14:12;
		uint32_t bit15_keep0:1;
		uint32_t reserve_16:1;
		uint32_t eod_msk:1;
		uint32_t reserve16_31:14;
	}b;
} cmp_irq_ctrl_t;

typedef union lay_size {
	uint32_t d32;
	struct {
		uint32_t width:12;
	        uint32_t reserve12_15:4;
	        uint32_t height:12;
	        uint32_t reserve28_31:4;
	}b;
} lay_size_t;

typedef union lay_cfg {
	uint32_t d32;
	struct {
		uint32_t g_alpha:8;
		uint32_t reserve8_9:2;
		uint32_t color:3;
		uint32_t g_alpha_en:1;
		uint32_t domain_multi:1;
		uint32_t reserve14_15:1;
		uint32_t format:4;
		uint32_t reserve22_31:12;
	}b;
} lay_cfg_t;

typedef union lay_scale {
	uint32_t d32;
	struct {
		uint32_t target_width:12;
	        uint32_t reserve12_15:4;
	        uint32_t target_height:12;
	        uint32_t reserve28_31:4;
	}b;
} lay_scale_t;

typedef union lay_pos {
	uint32_t d32;
	struct {
		uint32_t x_pos:12;
	        uint32_t reserve12_15:4;
	        uint32_t y_pos:12;
	        uint32_t reserve28_31:4;
	}b;
} lay_pos_t;

/*
 *struct jzfb_framedesc {
 *  uint32_t	   FrameNextCfgAddr;
 *  frm_size_t	   FrameSize;
 *  frm_ctrl_t	   FrameCtrl;
 *  uint32_t	   WritebackAddr;
 *  uint32_t	   WritebackStride;
 *  uint32_t	   Layer0CfgAddr;
 *  uint32_t	   Layer1CfgAddr;
 *  uint32_t 	   Layer2CfgAddr;
 *  uint32_t	   Layer3CfgAddr;
 *  lay_cfg_en_t       LayCfgEn;
 *  cmp_irq_ctrl_t	   InterruptControl;
 *};
 */

struct jzfb_framedesc {
	unsigned int NextDesAddr;
	unsigned int BufferAddr_RGB_Y;
	unsigned int Stride_RGB_Y;
	unsigned int ChainCfg;
	unsigned int InterruptCtrl;
	unsigned int BufferAddr_UV;
	unsigned int Stride_UV;
};

struct jzfb_layerdesc {
	lay_size_t	LayerSize;
	lay_cfg_t	LayerCfg;
	uint32_t	LayerBufferAddr;
	lay_scale_t	LayerScale;
	uint32_t	layer_reserve0;
	uint32_t	LayerScratch;
	lay_pos_t	LayerPos;
	uint32_t	layer_reserve1;
	uint32_t	layer_reserve2;
	uint32_t	LayerStride;
	uint32_t	BufferAddr_UV;
	uint32_t	stride_UV;
};

struct jzfb_lay_cfg {
	unsigned int lay_en;
	unsigned int tlb_en;
	unsigned int lay_z_order;
	unsigned int pic_width;
	unsigned int pic_height;
	unsigned int disp_pos_x;
	unsigned int disp_pos_y;
	unsigned int g_alpha_en;
	unsigned int g_alpha_val;
	unsigned int color;
	unsigned int domain_multi;
	unsigned int format;
	unsigned int stride;
};

struct jzfb_frm_cfg {
	struct jzfb_lay_cfg lay_cfg[MAX_LAYER_NUM];
};

typedef enum stop_mode {
	QCK_STOP,
	GEN_STOP,
} stop_mode_t;

typedef struct layer_csc_mode {
	enum csc_mode {
		CSC_MODE_0,
		CSC_MODE_1,
		CSC_MODE_2,
		CSC_MODE_3,
	}csc_mode;
	uint8_t layer;
} jzfb_layer_csc_t;


typedef enum framedesc_st {
	FRAME_DESC_AVAILABLE = 1,
	FRAME_DESC_SET_OVER = 2,
	FRAME_DESC_USING = 3,
}framedesc_st_t;

typedef enum frm_cfg_st {
	FRAME_CFG_NO_UPDATE,
	FRAME_CFG_UPDATE,
}frm_cfg_st_t;

struct jzfb_frm_mode {
	struct jzfb_frm_cfg frm_cfg;
	frm_cfg_st_t update_st[MAX_DESC_NUM];
};

typedef enum {
	DESC_ST_FREE,
	DESC_ST_AVAILABLE,
	DESC_ST_RUNING,
} frm_st_t;

struct jzfb_frmdesc_msg {
	struct jzfb_framedesc *addr_virt;
	dma_addr_t addr_phy;
	struct list_head list;
	int state;
	int index;
};

enum tft_lcd_color_even {
        TFT_LCD_COLOR_EVEN_RGB,
        TFT_LCD_COLOR_EVEN_RBG,
        TFT_LCD_COLOR_EVEN_BGR,
        TFT_LCD_COLOR_EVEN_BRG,
        TFT_LCD_COLOR_EVEN_GBR,
        TFT_LCD_COLOR_EVEN_GRB,
};

enum tft_lcd_color_odd {
        TFT_LCD_COLOR_ODD_RGB,
        TFT_LCD_COLOR_ODD_RBG,
        TFT_LCD_COLOR_ODD_BGR,
        TFT_LCD_COLOR_ODD_BRG,
        TFT_LCD_COLOR_ODD_GBR,
        TFT_LCD_COLOR_ODD_GRB,
};

enum tft_lcd_mode {
        TFT_LCD_MODE_PARALLEL_24B,
        TFT_LCD_MODE_SERIAL_RGB,
        TFT_LCD_MODE_SERIAL_RGBD,
	TFT_LCD_MODE_PARALLEL_888,
	TFT_LCD_MODE_PARALLEL_666,
	TFT_LCD_MODE_PARALLEL_565,
};

enum jzfb_copy_type {
    FB_COPY_TYPE_NONE,
    FB_COPY_TYPE_ROTATE_0,
    FB_COPY_TYPE_ROTATE_180,
    FB_COPY_TYPE_ROTATE_90,
    FB_COPY_TYPE_ROTATE_270,
    FB_COPY_TYPE_ERR,
};

enum smart_config_type {
	SMART_CONFIG_CMD =  0,
	SMART_CONFIG_DATA =  1,
	SMART_CONFIG_UDELAY =  2,
	SMART_CONFIG_PRM = 3,
};

struct smart_lcd_data_table {
	enum smart_config_type type;
	uint32_t value;
};

struct jzfb_tft_config {
        unsigned int pix_clk_inv:1;
        unsigned int de_dl:1;
        unsigned int sync_dl:1;
        unsigned int vsync_dl:1;
        enum tft_lcd_color_even color_even;
        enum tft_lcd_color_odd color_odd;
        enum tft_lcd_mode mode;
        enum jzfb_copy_type fb_copy_type;
};
struct jzfb_smart_config{
		enum smart_lcd_type smart_type;	/* smart lcd transfer type, 0: parrallel, 1: serial */
		enum smart_lcd_format pix_fmt;

		unsigned clkply_active_rising:1;	/* smart lcd clock polarity:
0: Active edge is Falling,1: Active edge is Rasing */
		unsigned rsply_cmd_high:1;	/* smart lcd RS polarity.
0: Command_RS=0, Data_RS=1; 1: Command_RS=1, Data_RS=0 */
		unsigned csply_active_high:1;	/* smart lcd CS Polarity.
0: Active level is low, 1: Active level is high */

		unsigned newcfg_6800_md:1;
		unsigned newcfg_fmt_conv:1;
		unsigned newcfg_datatx_type:1;
		unsigned newcfg_cmdtx_type:1;
		unsigned newcfg_cmd_9bit:1;

		size_t length_cmd;
		unsigned long *write_gram_cmd;	/* write graphic ram command */
		unsigned bus_width;	/* bus width in bit */
		unsigned int length_data_table;	/* array size of data_table */
		struct smart_lcd_data_table *data_table;	/* init data table */
		int (*init) (void);
		int (*gpio_for_slcd) (void);
		unsigned int dc_md:1;
		unsigned int wr_md:1;
		unsigned int te_mipi_switch:1;
		unsigned int te_switch:1;
		enum smart_lcd_dwidth dwidth;
		enum smart_lcd_cwidth cwidth;
	} ;

struct jzfb_config_info {
	int num_modes;
	struct fb_videomode *modes;	 /* valid video modes */
	enum jzfb_format_order fmt_order;	/* frame buffer pixel format order */
	struct fb_var_screeninfo var;   /* Current var */
	struct jzfb_frm_mode current_frm_mode;
	struct jzfb_tft_config *tft_config;
	struct jzfb_smart_config *smart_config;
	int lcdbaseoff;		/* lcd register base offset from LCD_BASE */
	int tlb_disable_ch;

	int current_frm_desc;
	enum jzfb_lcd_type lcd_type;	/* lcd type */
	unsigned int bpp;	/* bits per pixel for the lcd */
	unsigned pinmd:1;	/* 16bpp lcd data pin mapping. 0: LCD_D[15:0],1: LCD_D[17:10] LCD_D[8:1] */

	unsigned pixclk_falling_edge:1;	/* pixclk_falling_edge: pixel clock at falling edge */
	unsigned date_enable_active_low:1;	/* data enable active low */

	size_t vidmem_size;
	void *vidmem[MAX_DESC_NUM];
//	dma_addr_t vidmem_phys[MAX_DESC_NUM];
  dma_addr_t vidmem_rgb_y_phys[MAX_DESC_NUM];
	dma_addr_t vidmem_uv_phys[MAX_DESC_NUM];


	struct fb_fix_screeninfo fix;
	int frm_size;
	struct jzfb_framedesc *framedesc[MAX_DESC_NUM];
	dma_addr_t framedesc_phys[MAX_DESC_NUM];
	struct jzfb_layerdesc *layerdesc[MAX_DESC_NUM][MAX_LAYER_NUM];
	dma_addr_t layerdesc_phys[MAX_DESC_NUM][MAX_LAYER_NUM];

	unsigned dither_enable:1;	/* enable dither function: 1, disable dither function: 0 */
	/* when LCD'bpp is lower than 24, suggest to enable it */
	struct {
		unsigned dither_red;	/* 0:8bit dither, 1:6bit dither, 2: 5bit dither, 3: 4bit dither */
		unsigned dither_green;	/* 0:8bit dither, 1:6bit dither, 2: 5bit dither, 3: 4bit dither */
		unsigned dither_blue;	/* 0:8bit dither, 1:6bit dither, 2: 5bit dither, 3: 4bit dither */
	} dither;

	struct {
		unsigned spl;	/* special_tft SPL signal register setting */
		unsigned cls;	/* special_tft CLS signal register setting */
		unsigned ps;	/* special_tft PS signal register setting */
		unsigned rev;	/* special_tft REV signal register setting */
	} special_tft_config;

	unsigned long fdadr0;	/* physical address of frame/palette descriptor */
	unsigned long fdadr1;	/* physical address of frame descriptor */

	/* DMA descriptors */
	struct jz_fb_dma_descriptor *dmadesc_fblow;
	struct jz_fb_dma_descriptor *dmadesc_fbhigh;
	struct jz_fb_dma_descriptor *dmadesc_palette;
	struct jz_fb_dma_descriptor *dmadesc_cmd;
	struct jz_fb_dma_descriptor *dmadesc_cmd_tmp;

	unsigned long screen;	/* address of frame buffer */
	unsigned int screen_size;
	unsigned long palette;	/* address of palette memory */
	unsigned int palette_size;
	unsigned long dma_cmd_buf;	/* address of dma command buffer */

	void *par;
};

void jzfb_clk_enable(struct jzfb *jzfb);
void jzfb_clk_disable(struct jzfb *jzfb);

extern struct jzfb_config_info lcd_config_info;
extern struct jzfb_config_info jzfb1_init_data;
extern struct fb_videomode jzfb1_videomode;
extern struct dsi_device jz_dsi;
#endif /*__JZ_LCD_H__*/


