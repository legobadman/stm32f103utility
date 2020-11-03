#include "nrf24l01.h"
#include "delay.h"
#include "sys.h"
#include "spi.h"
#include "usart.h"

const uint8_t TX_ADDRESS_X[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};//���Ͷ˵�ַ
const uint8_t RX_ADDRESS_X[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};//���ն˵�ַ


/*****************************************************************************
* ��  ����void NRF24L01_GPIO_Init(void)
* ��  �ܣ�NRF24L01���GPIO��ʼ��
* ��  ����void
* ����ֵ����
* ��  ע����
*****************************************************************************/
void NRF24L01_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC,ENABLE);

	//SPI1�ϻ�����flash��sd��,һֱƬѡ�ø�,�ų�����
	
	GPIO_InitStructure.GPIO_Pin = NRF_CE;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NRF_IRQ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//�ж���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NRF_CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,NRF_IRQ|NRF_CE);
	GPIO_SetBits(GPIOA,NRF_CS);

//�ڶ�ģ����ص�GPIO
	/*	
	GPIO_InitStructure.GPIO_Pin = NRF_CE2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NRF_IRQ2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = NRF_CS2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,NRF_IRQ2|NRF_CE2);
	GPIO_SetBits(GPIOC,NRF_CS2);
	*/
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Write_Reg(uint8_t res,uint8_t value)
* ��  �ܣ��ӼĴ���д��һ�ֽ�����
* ��  ����res:�Ĵ�����ַ;value:д���ֵ
* ����ֵ��status:��ȡ�ļĴ���״ֵ̬
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Write_Reg(uint8_t res,uint8_t value)
{
	uint8_t status;
	
	NRF_CS_LOW;
	status = SPI1_ReadWrite_Byte(res);
	SPI1_ReadWrite_Byte(value);
	NRF_CS_HIGH;

	return status;	//���ؼĴ���״ֵ̬
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Read_Reg(uint8_t res)
* ��  �ܣ��ӼĴ�����ȡһ�ֽ�����
* ��  ����res:�Ĵ�����ַ
* ����ֵ��ret:��ȡ��ֵ
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Read_Reg(uint8_t res)
{
	uint8_t ret;
	
	NRF_CS_LOW;
	SPI1_ReadWrite_Byte(res);
	ret = SPI1_ReadWrite_Byte(0xFF);	//�ڶ�ȡ����ǰ�ȶ�MISO�ĵ�ƽ,��ֹ����slave�豸
	NRF_CS_HIGH;

	return ret;
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Read_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
* ��  �ܣ���ָ��λ�ö�ȡһ�����ȵ�����
* ��  ����res:ָ��λ��;len:���ݳ���;*pBuf:ָ�����ݵ��׵�ַ
* ����ֵ��status:�Ĵ�����״̬
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Read_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
{
	uint8_t status,i;

	NRF_CS_LOW;
	status = SPI1_ReadWrite_Byte(res);
	for(i=0;i<len;i++)
		{
			pBuf[i] = SPI1_ReadWrite_Byte(0xFF);
	}
	NRF_CS_HIGH;

	return status;
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Write_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
* ��  �ܣ���ָ��λ��д��һ�����ȵ�����
* ��  ����res:ָ��λ��;len:���ݳ���;*pBuf:ָ�����ݵ��׵�ַ
* ����ֵ��status:�Ĵ�����״̬
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Write_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
{
	uint8_t status,i;

	NRF_CS_LOW;
	status = SPI1_ReadWrite_Byte(res);
	for (i = 0; i < len; i++)
		{
			SPI1_ReadWrite_Byte(*pBuf++);
			//SPI1_ReadWrite_Byte(pBuf[i]);
		}
	NRF_CS_HIGH;
	
	return status;
}

void NRF24L01_Power_Down_Mode(void)
{
	NRF_CS_LOW;
	NRF24L01_Write_Reg(NRF24L01_CONFIG, 0x00);
	NRF_CS_HIGH;
	
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Check(void)
* ��  �ܣ����NRF24L01�Ƿ����
* ��  ������
* ����ֵ��0:�ɹ���1:ʧ��
* ��  ע����NRF24L01�ķ��͵�ַ�Ĵ���д��5�ֽ������ٶ�����,�ж�24L01�Ƿ�����������
*****************************************************************************/
uint8_t NRF24L01_Check(void)
{
	uint8_t buf[5]={0xa5,0xa5,0xa5,0xa5,0xa5};
	uint8_t i;

	NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,5,buf);//д��5�ֽڵĵ�ַ
	NRF24L01_Read_Buf(TX_ADDR,5,buf);
	for(i=0;i<5;i++)
		{
			if(buf[i]!=0xa5)
				break;
	}
	if(i!=5)
		return 1;
	return 0;
}

/*****************************************************************************
* ��  ����void NRF24L01_TX_Mode(void)
* ��  �ܣ�NRF24L01����ģʽ����
* ��  ������
* ����ֵ����
* ��  ע����
*****************************************************************************/
void NRF24L01_TX_Mode(void)
{
	NRF_CE_LOW;
	NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,TX_ADR_WIDTH,(uint8_t *)TX_ADDRESS_X);//дTX�ڵ��ַ
	NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0,RX_ADR_WIDTH, (uint8_t *) RX_ADDRESS_X);//дRX�ڵ��ַ,Ϊ���Զ�ʹ��ACK
	NRF24L01_Write_Reg(W_REGISTER+EN_AA, 0x01);//ʹ��ͨ��0�Զ�Ӧ��
	NRF24L01_Write_Reg(W_REGISTER+EN_RXADDR, 0x01);//ʹ��ͨ��0���յ�ַ
	NRF24L01_Write_Reg(W_REGISTER+SETUP_PETR, 0x1a);//�����Զ��ط����ʱ��:500us+86us,����ش����:10��
	NRF24L01_Write_Reg(W_REGISTER+RF_CH, 40);//����ͨ��Ϊ40
	NRF24L01_Write_Reg(W_REGISTER+RF_SETUP, 0x0f);//���÷������:0dB����;2Mnps;���������濪��
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_CONFIG,0x0e);//��������:PWR_UP;EN_CRC;16BIT_CRC;����ģʽ;���������ж�
	NRF_CE_HIGH;//NRF_CEΪ��,10us��������������
	delay_us(12);
}

/*****************************************************************************
* ��  ����void NRF24L01_TX_Mode(void)
* ��  �ܣ�NRF24L01����ģʽ����
* ��  ������
* ����ֵ����
* ��  ע����
*****************************************************************************/
void NRF24L01_RX_Mode(void)
{
	NRF_CE_LOW;
	NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0, RX_ADR_WIDTH, (uint8_t *) RX_ADDRESS_X);//дRX��ַ�ڵ�
	NRF24L01_Write_Reg(W_REGISTER+EN_AA,0x01);//ʹ��ͨ��0�Զ�Ӧ��
	NRF24L01_Write_Reg(W_REGISTER+EN_RXADDR,0x01);//ʹ��ͨ��0���յ�ַ
	NRF24L01_Write_Reg(W_REGISTER+RF_CH, 40);//����RFͨ��Ƶ��
	NRF24L01_Write_Reg(W_REGISTER+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0��Ч���ݿ��
	NRF24L01_Write_Reg(W_REGISTER+RF_SETUP, 0x0f);//����TX�������:0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_CONFIG, 0x0f);//��������:PWR_UP;EN_CRC;16BIT_CRC;����ģʽ;���������ж�
	NRF_CE_HIGH;//CEΪ��,�������ģʽ
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_TX_Packet(uint8_t *txbuf)
* ��  �ܣ�NRF24L01����һ������
* ��  ����*txbuf���ȴ��������ݵ��׵�ַ
* ����ֵ��MAX_TX:����ط�����;TX_OK:�������;0xFF:����ʧ��
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_TX_Packet(uint8_t *txbuf)
{
	uint8_t ret;

	NRF_CE_LOW;
	NRF24L01_Write_Buf(W_TX_PAYLOAD, TX_PLOAD_WIDTH, txbuf);//д���ݵ�txbuf,32�ֽ�
	NRF_CE_HIGH;//��������
	while (NRF_IRQ_STATUS);//�ȴ��������
	while(1) {printf("send finish\r\n");}
	ret = NRF24L01_Read_Reg(NRF24L01_STATUS);//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_STATUS, ret);//���TX_DS or MAX_RT���жϱ�־
	if(ret&MAX_TX)//�ﵽ����ط�����
		{
			NRF24L01_Write_Reg(FLUSH_TX, 0XFF);//���TX FIFO	�Ĵ���
			return MAX_TX;
	}
	if(ret&TX_OK)	//�������
		{
			return TX_OK;
	}
	return 0xFF;//����ʧ��
	
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf)
* ��  �ܣ�NRF24L01����һ������
* ��  ����*rxbuf���ȴ��������ݵ��׵�ַ
* ����ֵ��0:���ճɹ�;1:��������ʧ��
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf)
{
	uint8_t ret;


	NRF_CE_HIGH;
	while(NRF_IRQ_STATUS);
	NRF_CE_LOW;

	ret = NRF24L01_Read_Reg(NRF24L01_STATUS);//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg(W_REGISTER+NRF24L01_STATUS, ret);//���TX_DS or MAX_RT���жϱ�־
	if(ret&RX_OK)//���յ�����
		{
			NRF24L01_Read_Buf(R_RX_PAYLOAD,RX_PLOAD_WIDTH,rxbuf);//��ȡ����
			NRF24L01_Write_Reg(FLUSH_RX,0xFF);//���RX FIFO�Ĵ���
			return 0;
	}
	return 1;//û�н��յ�����
}

/*****************************************************************************
* ��  ����void NRF24L01_Init(void)
* ��  �ܣ�NRF24L01��ʼ��
* ��  ������
* ����ֵ����
* ��  ע����ӦGPIO����
*****************************************************************************/
void NRF24L01_Init(void)
{
	NRF24L01_GPIO_Init();
	
	NRF_CS_HIGH;
	NRF_CE_LOW;
}


/****************�ڶ�������ģ��*********************/


uint8_t NRF24L01_Write_Reg2(uint8_t res,uint8_t value)
{
	uint8_t status;
	
	NRF_CS2_LOW;
	status = SPI2_ReadWrite_Byte(res);
	SPI2_ReadWrite_Byte(value);
	NRF_CS2_HIGH;

	return status;	//���ؼĴ���״ֵ̬
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Read_Reg(uint8_t res)
* ��  �ܣ��ӼĴ�����ȡһ�ֽ�����
* ��  ����res:�Ĵ�����ַ
* ����ֵ��ret:��ȡ��ֵ
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Read_Reg2(uint8_t res)
{
	uint8_t ret;
	
	NRF_CS2_LOW;
	SPI2_ReadWrite_Byte(res);
	ret = SPI2_ReadWrite_Byte(0xFF);	//�ڶ�ȡ����ǰ�ȶ�MISO�ĵ�ƽ,��ֹ����slave�豸
	NRF_CS2_HIGH;

	return ret;
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Read_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
* ��  �ܣ���ָ��λ�ö�ȡһ�����ȵ�����
* ��  ����res:ָ��λ��;len:���ݳ���;*pBuf:ָ�����ݵ��׵�ַ
* ����ֵ��status:�Ĵ�����״̬
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Read_Buf2(uint8_t res,uint8_t len,uint8_t *pBuf)
{
	uint8_t status,i;

	NRF_CS2_LOW;
	status = SPI2_ReadWrite_Byte(res);
	for(i=0;i<len;i++)
		{
			pBuf[i] = SPI2_ReadWrite_Byte(0xFF);
	}
	NRF_CS2_HIGH;

	return status;
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Write_Buf(uint8_t res,uint8_t len,uint8_t *pBuf)
* ��  �ܣ���ָ��λ��д��һ�����ȵ�����
* ��  ����res:ָ��λ��;len:���ݳ���;*pBuf:ָ�����ݵ��׵�ַ
* ����ֵ��status:�Ĵ�����״̬
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Write_Buf2(uint8_t res,uint8_t len,uint8_t *pBuf)
{
	uint8_t status,i;

	NRF_CS2_LOW;
	status = SPI2_ReadWrite_Byte(res);
	for (i = 0; i < len; i++)
		{
			SPI2_ReadWrite_Byte(*pBuf++);
			//SPI1_ReadWrite_Byte(pBuf[i]);
		}
	NRF_CS2_HIGH;
	
	return status;
}

void NRF24L01_Power_Down_Mode2(void)
{
	NRF_CS2_LOW;
	NRF24L01_Write_Reg2(NRF24L01_CONFIG, 0x00);
	NRF_CS2_HIGH;
	
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_Check(void)
* ��  �ܣ����NRF24L01�Ƿ����
* ��  ������
* ����ֵ��0:�ɹ���1:ʧ��
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_Check2(void)
{
	uint8_t buf[5]={0xa5,0xa5,0xa5,0xa5,0xa5};
	uint8_t i;

	NRF24L01_Write_Buf2(W_REGISTER+TX_ADDR,5, buf);//д��5�ֽڵĵ�ַ
	NRF24L01_Read_Buf2(TX_ADDR, 5,buf);
	//printf("nrf2 check data is %s \r\n",buf);
	for(i=0;i<5;i++)
		{
			if(buf[i]!=0xa5)
				break;
	}
	if(i!=5)
		return 1;
	return 0;
}

/*****************************************************************************
* ��  ����void NRF24L01_TX_Mode(void)
* ��  �ܣ�NRF24L01����ģʽ����
* ��  ������
* ����ֵ����
* ��  ע����
*****************************************************************************/
void NRF24L01_TX_Mode2(void)
{
	NRF_CE2_LOW;
	NRF24L01_Write_Buf2(W_REGISTER+TX_ADDR,TX_ADR_WIDTH,(uint8_t *)TX_ADDRESS_X);//дTX�ڵ��ַ
	NRF24L01_Write_Buf2(W_REGISTER+RX_ADDR_P0,RX_ADR_WIDTH, (uint8_t *) RX_ADDRESS_X);//дRX�ڵ��ַ,Ϊ���Զ�ʹ��ACK
	NRF24L01_Write_Reg2(W_REGISTER+EN_AA, 0x01);//ʹ��ͨ��0�Զ�Ӧ��
	NRF24L01_Write_Reg2(W_REGISTER+EN_RXADDR, 0x01);//ʹ��ͨ��0���յ�ַ
	NRF24L01_Write_Reg2(W_REGISTER+SETUP_PETR, 0x1a);//�����Զ��ط����ʱ��:500us+86us,����ش����:10��
	NRF24L01_Write_Reg2(W_REGISTER+RF_CH, 40);//����ͨ��Ϊ40
	NRF24L01_Write_Reg2(W_REGISTER+RF_SETUP, 0x0f);//���÷������:0dB����;2Mnps;���������濪��
	NRF24L01_Write_Reg2(W_REGISTER+NRF24L01_CONFIG,0x0e);//��������:PWR_UP;EN_CRC;16BIT_CRC;����ģʽ;���������ж�
	NRF_CE2_HIGH;//NRF_CEΪ��,10us��������������
}

/*****************************************************************************
* ��  ����void NRF24L01_TX_Mode(void)
* ��  �ܣ�NRF24L01����ģʽ����
* ��  ������
* ����ֵ����
* ��  ע����
*****************************************************************************/
void NRF24L01_RX_Mode2(void)
{
	NRF_CE2_LOW;
	NRF24L01_Write_Buf2(W_REGISTER+RX_ADDR_P0, RX_ADR_WIDTH, (uint8_t *) RX_ADDRESS_X);//дRX��ַ�ڵ�
	NRF24L01_Write_Reg2(W_REGISTER+EN_AA,0x01);//ʹ��ͨ��0�Զ�Ӧ��
	NRF24L01_Write_Reg2(W_REGISTER+EN_RXADDR,0x01);//ʹ��ͨ��0���յ�ַ
	NRF24L01_Write_Reg2(W_REGISTER+RF_CH, 40);//����RFͨ��Ƶ��
	NRF24L01_Write_Reg2(W_REGISTER+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0��Ч���ݿ��
	NRF24L01_Write_Reg2(W_REGISTER+RF_SETUP, 0x0f);//����TX�������:0db����,2Mbps,���������濪��
	NRF24L01_Write_Reg2(W_REGISTER+NRF24L01_CONFIG, 0x0f);//��������:PWR_UP;EN_CRC;16BIT_CRC;����ģʽ;���������ж�
	NRF_CE2_HIGH;//CEΪ��,�������ģʽ
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_TX_Packet(uint8_t *txbuf)
* ��  �ܣ�NRF24L01����һ������
* ��  ����*txbuf���ȴ��������ݵ��׵�ַ
* ����ֵ��MAX_TX:����ط�����;TX_OK:�������;0xFF:����ʧ��
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_TX_Packet2(uint8_t *txbuf)
{
	uint8_t ret;

	NRF_CE2_LOW;
	NRF24L01_Write_Buf2(W_TX_PAYLOAD, TX_PLOAD_WIDTH, txbuf);//д���ݵ�txbuf,32�ֽ�
	NRF_CE2_HIGH;//��������
	while (NRF_IRQ2_STATUS!=0);//�ȴ��������
	ret = NRF24L01_Read_Reg2(NRF24L01_STATUS);//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg2(W_REGISTER+NRF24L01_STATUS, ret);//���TX_DS or MAX_RT���жϱ�־
	if(ret&MAX_TX)//�ﵽ����ط�����
		{
			NRF24L01_Write_Reg2(FLUSH_TX, 0XFF);//���TX FIFO	�Ĵ���
			return MAX_TX;
	}
	if(ret&TX_OK)	//�������
		{
			return TX_OK;
	}
	return 0xFF;//����ʧ��
	
}

/*****************************************************************************
* ��  ����uint8_t NRF24L01_RX_Packet(uint8_t *rxbuf)
* ��  �ܣ�NRF24L01����һ������
* ��  ����*rxbuf���ȴ��������ݵ��׵�ַ
* ����ֵ��0:���ճɹ�;1:��������ʧ��
* ��  ע����
*****************************************************************************/
uint8_t NRF24L01_RX_Packet2(uint8_t *rxbuf)
{
	uint8_t ret;

	//NRF_CE2_HIGH;
	//while(NRF_IRQ2_STATUS);
	//NRF_CE2_LOW;

	ret = NRF24L01_Read_Reg2(NRF24L01_STATUS);//��ȡ״̬�Ĵ�����ֵ
	NRF24L01_Write_Reg2(W_REGISTER+NRF24L01_STATUS, ret);//���TX_DS or MAX_RT���жϱ�־
	if(ret&RX_OK)//���յ�����
		{
			NRF24L01_Read_Buf2(R_RX_PAYLOAD,RX_PLOAD_WIDTH,rxbuf);//��ȡ����
			/*for(i=0;i<5;i++)
			{
				printf("***rxbuf is %d\r\n",rxbuf[i]);
			}*/
			NRF24L01_Write_Reg2(FLUSH_RX,0xFF);//���RX FIFO�Ĵ���
			return 0;
	}
	return 1;//û�н��յ�����
}

/*****************************************************************************
* ��  ����void NRF24L01_Init(void)
* ��  �ܣ�NRF24L01��ʼ��
* ��  ������
* ����ֵ����
* ��  ע����ӦGPIO����
*****************************************************************************/
void NRF24L01_Init2(void)
{
	//NRF24L01_GPIO_Init();
	NRF_CS2_HIGH;
	NRF_CE2_LOW;
}


