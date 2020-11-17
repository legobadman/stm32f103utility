#ifndef __USART_H
#define __USART_H
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#include "sys.h" 


#define USART_n		USART1  //����ʹ��printf�����Ĵ��ڣ���������Ҫʹ��USART_printfר�ú�������

#define USART1_REC_LEN  			200  	//����USART1�������ֽ���
#define USART2_REC_LEN  			200  	//����USART2�������ֽ���
#define USART3_REC_LEN  			200  	//����USART3�������ֽ���

//��ʹ��ĳ������ʱҪ��ֹ�˴��ڣ��Լ��ٱ�����
#define EN_USART1 			1		//ʹ�ܣ�1��/��ֹ��0������1
#define EN_USART2 			0		//ʹ�ܣ�1��/��ֹ��0������2
#define EN_USART3 			1		//ʹ�ܣ�1��/��ֹ��0������3
	  	
extern u8  USART1_RX_BUF[USART1_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
 
extern u16 USART1_RX_STA;         		//����״̬���	
extern u16 USART2_RX_STA;         		//����״̬���	
extern u16 USART3_RX_STA;         		//����״̬���

typedef struct
{
	u16 volatile Wd_Indx;
	u16 volatile Rd_Indx;
	u16 Mask;
	u8* pbuf;
} UartBuf;

extern UartBuf UartTxbuf;	//���η��ͽṹ��
extern UartBuf UartRxbuf;	//���ν��սṹ��

//��������
void USART1_Init(u32 bound);//����1��ʼ��������
void USART2_Init(u32 bound);//����2��ʼ��������
void USART3_Init(u32 bound);//����3��ʼ��������
void USART1_printf(char* fmt,...); //����1��ר��printf����
void USART2_printf(char* fmt,...); //����2��ר��printf����
void USART3_printf(char* fmt,...); //����3��ר��printf����

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

void UART1_Put_Char(unsigned char DataToSend);
u8 Uart1_Put_Char(unsigned char DataToSend);
void UartBuf_WD(UartBuf* Ringbuf, u8 DataIn);
u8 UartBuf_RD(UartBuf* Ringbuf);
u16 UartBuf_Cnt(UartBuf* Ringbuf);

#endif


