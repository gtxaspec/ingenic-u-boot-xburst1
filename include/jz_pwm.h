#ifndef _JZ_PWM_H_
#define _JZ_PWM_H_

#define PWM_CHN_NUM 4

#define PWM_CHN0 0
#define PWM_CHN1 1
#define PWM_CHN2 2
#define PWM_CHN3 3


int pwm_init(void);
int pwm_config(unsigned int channel, int duty_ns, int period_ns);
int pwm_enable(unsigned int channel);
int pwm_disable(unsigned int channel);

#endif /* _JZ_PWM_H_ */
