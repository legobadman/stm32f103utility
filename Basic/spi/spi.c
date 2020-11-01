#include "spi.h"
#include "delay.h"
#include "sys.h"

/*****************************************************************************
* ��  ����void SPI_GPIO_Init(void)
* ��  �ܣ�SPI1���GPIO��ʼ������
* ��  ������
* ����ֵ����
* ��  ע����
*****************************************************************************/
void SPI_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_CLK|SPI1_MISO|SPI1_MOSI;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,SPI1_CLK|SPI1_MISO|SPI1_MOSI);
}

/*****************************************************************************
* ��  ����void SPI1_Init(void)
* ��  �ܣ�SPI1��ʼ������
* ��  ������
* ����ֵ����
* ��  ע����
*****************************************************************************/
void SPI1_Init(void)
{
	SPI_InitTypeDef SPI1_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);//spi1��APB2������
	
	SPI1_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI1_InitStructure.SPI_Mode = SPI_Mode_Master;//����SPI����ģʽ:����Ϊ��SPI
	SPI1_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI1_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//ѡ���˴���ʱ�ӵ���̬:ʱ�����յ͵�ƽ
	SPI1_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//���ݲ����ڵ�һ��ʱ����
	SPI1_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI1_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16:72/16=4.5MHz
	SPI1_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI1_InitStructure.SPI_CRCPolynomial = 7;//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1,&SPI1_InitStructure);
	SPI_Cmd(SPI1,ENABLE);

	SPI1_ReadWrite_Byte(0xFF);//��������
}

/*****************************************************************************
* ��  ����uint8_t SPI1_ReadWrite_Byte(uint8_t tx_dat)
* ��  �ܣ�SPI1�շ�����
* ��  ����tx_dat:���͵�����
* ����ֵ�����յ�����
* ��  ע����
*****************************************************************************/
uint8_t SPI1_ReadWrite_Byte(uint8_t tx_dat)
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
		SPI_I2S_SendData(SPI1,tx_dat);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
		return SPI_I2S_ReceiveData(SPI1);
}

/*****************************************************************************
* ��  ����void SPI1_Speed_Set(uint8_t speed)
* ��  �ܣ�SPI1ͨ���ٶ�����
* ��  ����speed:�ٶ�����
* ����ֵ����
* ��  ע����
*****************************************************************************/

void SPI1_Speed_Set(uint8_t speed)
{
	SPI1->CR1 &= 0xFFC7;
	SPI1->CR1 |= speed;
	SPI1->CR1 |= 1<<6;
}

/*****************************************************************************
* ��  ����void SPI2_Init(void)
* ��  �ܣ�SPI2��ʼ������
* ��  ������
* ����ֵ����
* ��  ע����
*****************************************************************************/
void SPI2_Init(void)
{
	SPI_InitTypeDef SPI2_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	SPI2_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI2_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI2_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI2_InitStructure.SPI_CPOL = SPI_CPOL_Low;	//����״̬
	SPI2_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI2_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI2_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI2_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI2_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI2_InitStructure);
	SPI_Cmd(SPI2,ENABLE);

	SPI2_ReadWrite_Byte(0xFF);//��������
}

/*****************************************************************************
* ��  ����uint8_t SPI2_ReadWrite_Byte(uint8_t tx_dat)
* ��  �ܣ�SPI2�շ�����
* ��  ����tx_dat:���͵�����
* ����ֵ�����յ�����
* ��  ע����
*****************************************************************************/
uint8_t SPI2_ReadWrite_Byte(uint8_t tx_dat)
{
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
		SPI_I2S_SendData(SPI2,tx_dat);
	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
		return SPI_I2S_ReceiveData(SPI2);
}






