

/*
//�������ҳ�Ʒ
//����ϵ�п�����Ӧ�ó���
//��ע΢�Ź��ںţ����ҵ���
//���ҿ������������� www.DoYoung.net/YT 
//������ѿ����н�ѧ��Ƶ�����ؼ������ϣ�������������
//�������ݾ��� ����������ҳ www.doyoung.net
*/

/*
���޸���־��
1-201708202309 ������


*/



#include "MPU6050.h"
#include "usart.h"
#include "i2c_software.h"
#include <math.h>

#define PRINT_ACCEL     (0x01)
#define PRINT_GYRO      (0x02)
#define PRINT_QUAT      (0x04)
#define ACCEL_ON        (0x01)
#define GYRO_ON         (0x02)
#define MOTION          (0)
#define NO_MOTION       (1)
#define DEFAULT_MPU_HZ  (200)
#define FLASH_SIZE      (512)
#define FLASH_MEM_START ((void*)0x1800)
#define q30  1073741824.0f


void MPU6050_Init(void){  //��ʼ��MPU6050
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_PWR_MGMT_1,0x80);//�������״̬
	delay_ms(1000); //�ȴ���������
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_PWR_MGMT_1,0x00);//�������״̬
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_SMPLRT_DIV,0x07);//�����ǲ�����
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_CONFIG,0x06);	 
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_USER_CTRL, 0x00);
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 0x01);
	
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_GYRO_CONFIG,0x18);//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_ACCEL_CONFIG,0x08);//���ü��ٶȴ�����������4Gģʽ (��������2g��4g��8g��16g����Ӧ��ֵ�ֱ�Ϊ0x00��0x08��0x10��0x18)
}

void MPU6050_Init2(void) {
	IICwriteByte(MPU6050_ADD, MPU6050_RA_PWR_MGMT_1, 0x80);//�������״̬
	delay_ms(1000);
	IICwriteByte(MPU6050_ADD, MPU6050_RA_PWR_MGMT_1, 0x00);//�������״̬
	IICwriteByte(MPU6050_ADD, MPU6050_RA_SMPLRT_DIV, 0x07);//�����ǲ�����
	IICwriteByte(MPU6050_ADD, MPU6050_RA_CONFIG, 0x06);	 
	IICwriteByte(MPU6050_ADD, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 0x01);
	
	IICwriteByte(MPU6050_ADD, MPU6050_RA_GYRO_CONFIG, 0x18);//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
	IICwriteByte(MPU6050_ADD, MPU6050_RA_ACCEL_CONFIG,0x08);//���ü��ٶȴ�����������4Gģʽ (��������2g��4g��8g��16g����Ӧ��ֵ�ֱ�Ϊ0x00��0x08��0x10��0x18)	
}

void MPU6050_READ(u16* n){ //����X��Y��Z������ٶ�/������ԭʼ���� //n[0]��AX��n[1]��AY��n[2]��AZ��n[3]��GX��n[4]��GY��n[5]��GZ 
	u8 i;
    u8 t[14]; 
    I2C_READ_BUFFER(MPU6050_ADD, MPU6050_RA_ACCEL_XOUT_H, t, 14); //�������������ݵ�ַ�������˼��ٶȺ������ǹ�12�ֽ�
    for(i=0; i<3; i++) 	//���ϼ��ٶ�
      n[i]=((t[2*i] << 8) + t[2*i+1]);
    for(i=4; i<7; i++)	//����������
      n[i-1]=((t[2*i] << 8) + t[2*i+1]);        
}

void MPU6050_READ3(u16* n) {
	u8 i;
	u8 t[14];
	IICReadBytes(MPU6050_ADD, MPU6050_RA_ACCEL_XOUT_H, 14, t);
	for (i = 0; i < 3; i++)
		n[i] = ((t[2*i] << 8) + t[2*i+1]);
	for (i = 4; i < 7; i++)	//����������
		n[i-1] = ((t[2*i] << 8) + t[2*i+1]);    
}

void MPU6050_READ2(int16_t* n) {
	u8 i;
    u8 t[14]; 
    I2C_READ_BUFFER(MPU6050_ADD, MPU6050_RA_ACCEL_XOUT_H, t, 14); //�������������ݵ�ַ�������˼��ٶȺ������ǹ�12�ֽ�
    for(i=0; i<3; i++) 	//���ϼ��ٶ�
      n[i]=((t[2*i] << 8) + t[2*i+1]);
    for(i=4; i<7; i++)	//����������
      n[i-1]=((t[2*i] << 8) + t[2*i+1]);	
}


 
/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
