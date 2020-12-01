#ifndef __PWM_H__
#define __PWM_H__

void TIM2_PWM_Init(u16 arr, u16 psc);
u16 AcceleratorCurve(u16 adc_value);

#endif
