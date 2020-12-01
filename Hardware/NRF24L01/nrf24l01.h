#ifndef __NRF24L01_H__
#define __NRF24L01_H__
	 
#include "sys.h"


#define NRF_CE	GPIO_Pin_12
#define NRF_CE_HIGH	GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define NRF_CE_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define NRF_CS	GPIO_Pin_4 
#define NRF_CS_HIGH	GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define NRF_CS_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define NRF_IRQ	GPIO_Pin_15 
#define NRF_IRQ_STATUS	(GPIO_ReadInputDataBit(GPIOA, NRF_IRQ)) //PAin(1)

#define FLASH_CS	GPIO_Pin_2
#define FLASH_CS_HIGH	GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define FLASH_CS_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define SD_CARD_CS	GPIO_Pin_3 
#define SD_CARD_CS_HIGH	GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define SD_CARD_CS_LOW	GPIO_ResetBits(GPIOA,GPIO_Pin_3)



#define NRF_CE2	GPIO_Pin_0
#define NRF_CE2_HIGH	GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define NRF_CE2_LOW		GPIO_ResetBits(GPIOC,GPIO_Pin_0)

#define NRF_CS2	GPIO_Pin_1 
#define NRF_CS2_HIGH	GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define NRF_CS2_LOW		GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define NRF_IRQ2	GPIO_Pin_2
#define NRF_IRQ2_STATUS		PCin(2)//GPIO_ReadInputDataBit(GPIOC, NRF_IRQ2)

//����궨��
#define R_REGISTER			0x00
#define W_REGISTER			0x20
#define R_RX_PAYLOAD		0x61
#define W_TX_PAYLOAD		0XA0
#define FLUSH_TX			0xE1
#define FLUSH_RX			0xE2
#define RESUE_TX_PL			0xE3
#define ACTIVATE			0x50
#define R_RX_PL_WID			0x60
#define W_AC_PAYLOAD		0xA8
#define W_TX_PAYLOAD_NOACK	0xB0
#define NRF24L01_NOP		0xFF

//NRF24L01�Ĵ���
#define	NRF24L01_CONFIG		0x00	//���üĴ���:bit0:0-TX;1-RX;bit1:�ϵ�ʹ��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��
									//bit4:����ط������ж�(MAX_RT)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA				0x01
#define EN_RXADDR			0x02
#define SETUP_AW			0x03
#define SETUP_PETR			0x04
#define RF_CH				0x05
#define RF_SETUP			0x06
#define NRF24L01_STATUS		0x07
#define OBSERVE_TX			0x08
#define RPD					0X09			
#define RX_ADDR_P0			0x0A
#define RX_ADDR_P1			0x0B
#define RX_ADDR_P2			0x0C
#define RX_ADDR_P3			0x0D
#define RX_ADDR_P4			0x0E
#define RX_ADDR_P5			0x0F
#define TX_ADDR				0x10
#define RX_PW_P0			0x11
#define RX_PW_P1			0x12
#define RX_PW_P2			0x13
#define RX_PW_P3			0x14
#define RX_PW_P4			0x15
#define RX_PW_P5			0x16
#define FIFO_STATUS			0x17
#define DYNPD				0x1C
#define NRF24L01_FEATURE	0x1D

//nrf24l01�������ݿ�ȶ���
#define TX_ADR_WIDTH		5	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH		5	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH		5//32	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH		10//32	//32�ֽڵ��û����ݿ��

#define MAX_TX				0x10	//�ﵽ����ʹ����ж�
#define TX_OK				0x20	//��������ж�
#define RX_OK				0x40	//���������ж�

void NRF24L01_GPIO_Init(void);
uint8_t NRF24L01_Write_Reg(uint8_t res,uint8_t value);
uint8_t NRF24L01_Read_Reg(uint8_t res);
uint8_t NRF24L01_Read_Buf(uint8_t res,uint8_t len,uint8_t *pBuf);
uint8_t NRF24L01_Check(void);
void NRF24L01_Power_Down_Mode(void);
void NRF24L01_TX_Mode(void);
void NRF24L01_RX_Mode(void);
uint8_t NRF24L01_TX_Packet(uint8_t *txbuf);
uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf);
void NRF24L01_Init(void);//��ʼ��

/*********�ڶ�ģ��********************/
uint8_t NRF24L01_Write_Reg2(uint8_t res,uint8_t value);
uint8_t NRF24L01_Read_Reg2(uint8_t res);
uint8_t NRF24L01_Read_Buf2(uint8_t res,uint8_t len,uint8_t *pBuf);
uint8_t NRF24L01_Check2(void);
void NRF24L01_Power_Down_Mode2(void);
void NRF24L01_TX_Mode2(void);
void NRF24L01_RX_Mode2(void);
uint8_t NRF24L01_TX_Packet2(uint8_t *txbuf);
uint8_t NRF24L01_RX_Packet2(uint8_t *rxbuf);
void NRF24L01_Init2(void);//��ʼ��
		 				    
#endif







