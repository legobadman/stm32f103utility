#ifndef __BT_H__
#define __BT_H__

#include "stm32f10x.h"
#include <stdio.h>

#define BT_BAUD_Set 115200
#define BT_NAME		"Crazepony"
#define BT_PIN		"1234"
//AT �ַ���
#define BT_BAUD_AT	"OK+NAME:" BT_NAME

#define BT_ON      {GPIO_SetBits(GPIOB, GPIO_Pin_2);printf("BT power init success...\r\n");}
#define BT_OFF     {GPIO_ResetBits(GPIOB, GPIO_Pin_2);printf("BT power close success...\r\n");}//�궨����������

#define CmdreturnLength 20

#define BThavewrote        0xa5
#define BTneedwrite        0x5a

#define BT_NAMEmax  10
#define BT_PINmax   10

typedef struct 
{
	u8 Name[BT_NAMEmax];
	u32 Baud;
	u8 PinCode[BT_PINmax];
} BTtype;

void BT_PowerInit(void);   //����͸����Դ��ʼ��
void BT_ATcmdWrite(void);	//����д����
void BT_init(void);//rf-bm-s02a

extern float BTstate;

#endif
