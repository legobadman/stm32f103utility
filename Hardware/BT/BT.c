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

char Cmdreturn[CmdreturnLength];//指令的返回结果缓存
/********************************************
              往蓝牙写入一个指令包
********************************************/
void Uart1SendaBTCmd(const char* p)
{
    char i;
    for (i=0; i<CmdreturnLength; i++)
        Cmdreturn[i] = 0;//释放指令接收缓存
    delay_ms(100);//写完一条指令，延时500ms再度接收缓存
    for (i = 0; i < strlen(p); i++)
        UART1_Put_Char(*(p + i));
    delay_ms(100);//写完一条指令，延时500ms再度接收缓存

    i = 0;
    while (UartBuf_Cnt(&UartRxbuf) != 0)     //当串口缓冲不为空时，将串口缓冲赋值给指令结果缓冲
        Cmdreturn[i++] = UartBuf_RD(&UartRxbuf);
}

void BT_init(void)
{
    USART1_Init(9600);//蓝牙模块默认波特率
	delay_ms(5);
	Uart1SendaBTCmd("TTM:REN-Crazepony");
	Uart1SendaBTCmd("TTM:BPS-115200");
	delay_ms(2000);//修改波特率2s后生效

    USART1_Init(115200);//MCU串口波特率设置为115200
	delay_ms(5);
	Uart1SendaBTCmd("TTM:REN-Crazepony");
	Uart1SendaBTCmd("TTM:BPS-115200");
}