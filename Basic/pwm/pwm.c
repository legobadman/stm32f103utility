#include "sys.h"
#include "pwm.h"

void TIM2_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef gpio;
	TIM_OCInitTypeDef oc;
	TIM_TimeBaseInitTypeDef timebase;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//ʹ��TIM3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //��ʱ����Ӧ�����IO�˿���PA�顣
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_0;	//TIM_CH3, �ǹ̶��ģ��û����������޸ģ�����޺궨��
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
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);
}