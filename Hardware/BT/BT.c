#include "BT.h"
#include "usart.h"

void BT_PowerInit(void)
{
    RCC->APB2ENR|=1<<3;      //??PORTB??	
    GPIOB->CRL&=0XFFFFF0FF;  //PB2????
    GPIOB->CRL|=0X00000300;
    GPIOB->ODR|=1<<2;        //PB2??
    BT_ON
	
}

char Cmdreturn[CmdreturnLength];//ָ��ķ��ؽ������
/********************************************
              ������д��һ��ָ���
********************************************/
void Uart1SendaBTCmd(const char* p)
{
    char i;
    for (i=0; i<CmdreturnLength; i++)
        Cmdreturn[i] = 0;//�ͷ�ָ����ջ���
    delay_ms(100);//д��һ��ָ���ʱ500ms�ٶȽ��ջ���
    for (i = 0; i < strlen(p); i++)
        UART1_Put_Char(*(p + i));
    delay_ms(100);//д��һ��ָ���ʱ500ms�ٶȽ��ջ���

    i = 0;
    while (UartBuf_Cnt(&UartRxbuf) != 0)     //�����ڻ��岻Ϊ��ʱ�������ڻ��帳ֵ��ָ��������
        Cmdreturn[i++] = UartBuf_RD(&UartRxbuf);
}

void BT_init(void)
{
    USART1_Init(9600);//����ģ��Ĭ�ϲ�����
	delay_ms(5);
	Uart1SendaBTCmd("TTM:REN-Crazepony");
	Uart1SendaBTCmd("TTM:BPS-115200");
	delay_ms(2000);//�޸Ĳ�����2s����Ч

    USART1_Init(115200);//MCU���ڲ���������Ϊ115200
	delay_ms(5);
	Uart1SendaBTCmd("TTM:REN-Crazepony");
	Uart1SendaBTCmd("TTM:BPS-115200");
}