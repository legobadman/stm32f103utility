#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define LCK_PORT		GPIOB
#define LED1234_PORT	GPIOA
#define LED5678_PORT	GPIOB

#define LCK_PIN		GPIO_Pin_2
#define LED56		GPIO_Pin_1
#define LED34		GPIO_Pin_8
#define LED12		GPIO_Pin_11
#define LED78		GPIO_Pin_3

void LED_Init(void);

#define LCKLED_ON      {GPIO_SetBits(LCK_PORT, LCK_PIN);}
#define LCKLED_OFF     {GPIO_ResetBits(LCK_PORT, LCK_PIN);}

#endif