#ifndef _JZ_ADC_H_
#define _JZ_ADC_H_

#define ADC_CHN_NUM 1

#define ADC_CHN0 0
#define ADC_CHN1 1


void adc_init(void);
void adc_deinit(void);
void adc_enable(unsigned char chn);
void adc_disable(unsigned char chn);
unsigned int get_adc_voltage(unsigned char chn);
unsigned int get_adc_value(unsigned char chn);

#endif /* _JZ_ADC_H_ */
