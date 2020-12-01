#include "sys.h"
#include "pwm.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

void TIM2_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_OCInitTypeDef oc;
	TIM_TimeBaseInitTypeDef timebase;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ��TIM3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //��ʱ����Ӧ�����IO�˿���PA�顣
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//TIM2���ù�����ӳ��TIM2_CH1_ETR��ӳ����PA0��TIM2_CH2��ӳ����PA1��TIM2_CH3��ӳ����PA2��TIM2_CH4��ӳ����PA3��
	//PA0~PA3��ӦPWMA~PWMD
	gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);
	
	timebase.TIM_Period = arr;	//�����Զ���װ��ֵ
	timebase.TIM_Prescaler = psc;	//Ԥ��Ƶϵ��
	timebase.TIM_CounterMode = TIM_CounterMode_Up;	//�������������
	timebase.TIM_ClockDivision = TIM_CKD_DIV1;	//ʱ�ӵķ�Ƶ����
	TIM_TimeBaseInit(TIM2, &timebase);
	
	oc.TIM_OCMode = TIM_OCMode_PWM1;	// PWMģʽ1: CNT < CCR ʱ�����Ч��ƽ
	oc.TIM_OCPolarity = TIM_OCPolarity_High;//���ü���-��Ч��ƽΪ���ߵ�ƽ
	oc.TIM_OutputState = TIM_OutputState_Enable;	//���ʹ��
	TIM_OC1Init(TIM2, &oc);	//OC3ָ����ͨ����3
	TIM_OC2Init(TIM2, &oc);
	TIM_OC3Init(TIM2, &oc);
	TIM_OC4Init(TIM2, &oc);
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}

u16 AcceleratorCurve(u16 adc_value)
{
	//��������Ϊ f(x) = 2*x - 4400
	return MAX(0, 2 * adc_value - 4400);
}