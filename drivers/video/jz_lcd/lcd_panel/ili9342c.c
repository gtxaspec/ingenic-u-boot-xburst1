#include <config.h>
#include <serial.h>
#include <common.h>
#include <lcd.h>
#include <linux/list.h>
#include <linux/fb.h>
#include <asm/types.h>
#include <asm/arch/tcu.h>
#include <asm/arch/lcdc.h>
#include <asm/arch/gpio.h>
#include <regulator.h>

#include <jz_lcd/ili_9342c.h>
#include <jz_lcd/jz_lcd_v14.h>


//#define TEST_MODE 1
#define X_RES       320
#define Y_ERS       240
vidinfo_t panel_info = { X_RES , Y_ERS, LCD_BPP, };

struct lcd_ili9342c_data lcd_ili9342c_pdata = {
		.gpio_lcd_cs = CONFIG_GPIO_LCD_CS,
		.gpio_lcd_rd = NULL,
		.gpio_lcd_rst = CONFIG_GPIO_LCD_RST,
		.gpio_bl_pwr_en = CONFIG_GPIO_BL_PWR_EN,
};


static struct smart_lcd_data_table ili9342c_data_table[] = {

	{SMART_CONFIG_CMD,0xC8},
	{SMART_CONFIG_DATA,0xFF},
	{SMART_CONFIG_DATA,0x93},
	{SMART_CONFIG_DATA,0x42},

	{SMART_CONFIG_CMD,0xC0},
	{SMART_CONFIG_DATA,0x0E},
	{SMART_CONFIG_DATA,0x0E},

	{SMART_CONFIG_CMD,0xC1},
	{SMART_CONFIG_DATA,0x03},

	{SMART_CONFIG_CMD,0x36},
	{SMART_CONFIG_DATA,0x08},

	{SMART_CONFIG_CMD,0x3A},
	{SMART_CONFIG_DATA,0x05},

	{SMART_CONFIG_CMD,0x21},

	{SMART_CONFIG_CMD,0xC5},
	{SMART_CONFIG_DATA,0xF0},

	{SMART_CONFIG_CMD,0xB4},
	{SMART_CONFIG_DATA,0x02},

	{SMART_CONFIG_CMD,0xE0},
	{SMART_CONFIG_DATA,0x00},
	{SMART_CONFIG_DATA,0x0C},
	{SMART_CONFIG_DATA,0x11},
	{SMART_CONFIG_DATA,0x04},
	{SMART_CONFIG_DATA,0x11},
	{SMART_CONFIG_DATA,0x08},
	{SMART_CONFIG_DATA,0x37},
	{SMART_CONFIG_DATA,0x89},
	{SMART_CONFIG_DATA,0x4C},
	{SMART_CONFIG_DATA,0x06},
	{SMART_CONFIG_DATA,0x0C},
	{SMART_CONFIG_DATA,0x0A},
	{SMART_CONFIG_DATA,0x2E},
	{SMART_CONFIG_DATA,0x34},
	{SMART_CONFIG_DATA,0x0F},

	{SMART_CONFIG_CMD,0xE1},
	{SMART_CONFIG_DATA,0x00},
	{SMART_CONFIG_DATA,0x0B},
	{SMART_CONFIG_DATA,0x11},
	{SMART_CONFIG_DATA,0x05},
	{SMART_CONFIG_DATA,0x13},
	{SMART_CONFIG_DATA,0x09},
	{SMART_CONFIG_DATA,0x33},
	{SMART_CONFIG_DATA,0x67},
	{SMART_CONFIG_DATA,0x48},
	{SMART_CONFIG_DATA,0x07},
	{SMART_CONFIG_DATA,0x0E},
	{SMART_CONFIG_DATA,0x0B},
	{SMART_CONFIG_DATA,0x2E},
	{SMART_CONFIG_DATA,0x33},
	{SMART_CONFIG_DATA,0x0F},


	{SMART_CONFIG_CMD,0x2A},
	{SMART_CONFIG_DATA,0x00},
	{SMART_CONFIG_DATA,0x00},
	{SMART_CONFIG_DATA,0x01},
	{SMART_CONFIG_DATA,0x3F},

	{SMART_CONFIG_CMD,0x2B},
	{SMART_CONFIG_DATA,0x00},
	{SMART_CONFIG_DATA,0x00},
	{SMART_CONFIG_DATA,0x00},
	{SMART_CONFIG_DATA,0xEF},

	{SMART_CONFIG_CMD,0x11},

	{SMART_CONFIG_UDELAY,120000},

	{SMART_CONFIG_CMD,0x29},

	{SMART_CONFIG_UDELAY,120000},

	{SMART_CONFIG_CMD,0x2C},

};

unsigned long truly_cmd_buf[]= {
		0x2C2C2C2C,
};

struct fb_videomode jzfb1_videomode = {
	.name = "ili9342c-lcd",
	.refresh = 60,
	.xres = X_RES,
	.yres = Y_ERS,
	.left_margin = 0, //148,     //hbp
	.right_margin = 0, //88,             //hfp
	.upper_margin = 0, //36,             ///vbp
	.lower_margin = 0, //5,                  //vfp
	.hsync_len = 0, //44,
	.vsync_len = 0, //5,
	.sync = FB_SYNC_HOR_HIGH_ACT & FB_SYNC_VERT_HIGH_ACT,
	.vmode = FB_VMODE_NONINTERLACED,
	.flag = 0,
	.pixclock = 6728,
};

struct jzfb_smart_config ili9342c_cfg = {
//	.te_anti_jit = 1,
//	.te_md = 0,
	//.te_switch = 1,
//	.te_switch = 0,
	.dc_md = 0,
	.wr_md = 1,
//	.te_dp = 1,
  .rsply_cmd_high       = 0,
	.csply_active_high    = 0,
	.newcfg_fmt_conv =  1,
	.smart_type = SMART_LCD_TYPE_8080,
	.pix_fmt = SMART_LCD_FORMAT_565,
	.dwidth = SMART_LCD_DWIDTH_8_BIT,
	.cwidth = SMART_LCD_CWIDTH_8_BIT,
	.bus_width = 8,

	.write_gram_cmd = truly_cmd_buf,
	.length_cmd = ARRAY_SIZE(truly_cmd_buf),
	.data_table = ili9342c_data_table,
	.length_data_table = ARRAY_SIZE(ili9342c_data_table),
};


struct jzfb_config_info jzfb1_init_data = {
		.modes = &jzfb1_videomode,
		.lcd_type = LCD_TYPE_SLCD,
		.bpp = 16,

		.smart_config = &ili9342c_cfg,
		.pixclk_falling_edge = 0,
    .date_enable_active_low = 0,

	  .dither_enable = 0,
};



void panel_pin_init(void)
{

 int ret = 0;
		ret = gpio_request(lcd_ili9342c_pdata.gpio_lcd_rst, "lcd_rst");
		if(ret){
			/*
			 *printf("cannot request gpio lcd_rst\n");
			 */
		}


		ret = gpio_request(lcd_ili9342c_pdata.gpio_lcd_cs, "lcd_cs");
    if(ret){
			/*
       *printf("cannot request gpio lcd_cs\n");
			 */
		}
		ret = gpio_request(lcd_ili9342c_pdata.gpio_bl_pwr_en, "bl_en");
   if(ret){
			/*
       *printf("cannot request gpio bl_en\n");
			 */
		}


   serial_puts("lcd_ili9342c panel display pin init\n");

}


void panel_power_on(void)
{
	//	gpio_direction_output(lcd_ili9342c_pdata.gpio_bl_pwr_en, 1);
		gpio_direction_output(lcd_ili9342c_pdata.gpio_lcd_cs, 1);

		gpio_direction_output(lcd_ili9342c_pdata.gpio_lcd_rst, 1);
		mdelay(120);
		gpio_direction_output(lcd_ili9342c_pdata.gpio_lcd_rst, 0);
		mdelay(100);
		gpio_direction_output(lcd_ili9342c_pdata.gpio_lcd_rst, 1);
		mdelay(120);

		mdelay(5);
		gpio_direction_output(lcd_ili9342c_pdata.gpio_lcd_cs, 0);
		gpio_direction_output(lcd_ili9342c_pdata.gpio_bl_pwr_en, 1);

	serial_puts("lcd_ili9342c power on\n");
}

void panel_power_off(void)
{
    gpio_direction_output(lcd_ili9342c_pdata.gpio_bl_pwr_en, 0);
    mdelay(5);
    gpio_direction_output(lcd_ili9342c_pdata.gpio_lcd_cs, 0); /* cpo cno disable */

		gpio_direction_output(lcd_ili9342c_pdata.gpio_lcd_rst, 0); /* vdd :1.8v 1:enable, 0:disable */

	serial_puts("lcd_ili9342c power off\n");

}

