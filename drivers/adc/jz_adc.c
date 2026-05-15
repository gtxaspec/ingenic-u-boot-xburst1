#include <common.h>
#include <asm/io.h>
#include <jz_adc.h>

#define ADC_BASE 0xb0070000

#define ADC_ENA   0x00 //
#define ADC_CFG   0x04 //
#define ADC_CTRL  0x08 //
#define ADC_STATE 0x0C //
#define ADC_DATA0 0x10 //
#define ADC_DATA1 0x14 //
#define ADC_CLK   0x20 //
#define ADC_STB   0x24 //
#define ADC_RETM  0x28 //

#define REFER_VOL 1800 // mV
#define AUXCONST  4096

void adc_init(void)
{
	unsigned int adc_reg;
	writel(0, ADC_BASE + ADC_ENA);
	mdelay(3);
	writel(1 << 8 | 1 << 9 | 1 << 10, ADC_BASE + ADC_ENA);
	writel(5, ADC_BASE + ADC_STB);
	writel(2, ADC_BASE + ADC_RETM);

	adc_reg = (50 << 24) | (25 << 16) | (2 << 8) | 10;
	writel(adc_reg, ADC_BASE + ADC_CLK);
	// writel(0xff, ADC_BASE + ADC_CTRL);
}

void adc_deinit(void)
{
	writel(1 << 15, ADC_BASE + ADC_ENA);
}

void adc_enable(unsigned char chn)
{
	unsigned int adc_reg;
	if (chn > (ADC_CHN_NUM - 1))
		return;
	adc_reg = readl(ADC_BASE + ADC_ENA);
	adc_reg |= (1 << chn);
	writel(adc_reg, ADC_BASE + ADC_ENA);
	udelay(50);
}

void adc_disable(unsigned char chn)
{
	unsigned int adc_reg;
	if (chn > (ADC_CHN_NUM - 1))
		return;
	adc_reg = readl(ADC_BASE + ADC_ENA);
	adc_reg = adc_reg & ~(1 << chn);
	writel(adc_reg, ADC_BASE + ADC_ENA);
}

unsigned int get_adc_value(unsigned char chn)
{
	unsigned int value;
	if (chn > (ADC_CHN_NUM - 1))
		return 0;

	do
	{
		value = readl(ADC_BASE + ADC_STATE);
	} while (!(value & (1 << chn)));
	writel(1 << chn, ADC_BASE + ADC_STATE);

	switch (chn)
	{
	case 0:
		value = 0xFFF & readl(ADC_BASE + ADC_DATA0);
		break;
	case 1:
		value = 0xFFF & (readl(ADC_BASE + ADC_DATA0) >> 16);
		break;
	case 2:
		value = 0xFFF & readl(ADC_BASE + ADC_DATA1);
		break;
	case 3:
		value = 0xFFF & (readl(ADC_BASE + ADC_DATA1) >> 16);
		break;
	}

	return value;
}

// mV
unsigned int get_adc_voltage(unsigned char chn)
{
	unsigned int voltage, value;
	if (chn > (ADC_CHN_NUM - 1))
		return 0;

	voltage = get_adc_value(chn) * REFER_VOL / AUXCONST;

	return voltage;
}