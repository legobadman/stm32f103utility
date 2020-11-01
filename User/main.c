/*********************************************************************************************
ģ��������  ��������/���ҵ���
��������	MPU6050ԭʼ������ʾ����
��д�ˣ�	����	
��дʱ�䣺	2018��7��6��
Ӳ��֧�֣�	����1�ſ����� STM32F103C8 �ⲿ����8MHz RCC����������Ƶ72MHz��  

�޸���־������
1-	
	
							
˵����
 # ��������������1�ſ������Ӳ�������ϱ�д�ģ���ֲ���˽�Ӳ���ӿڲ��졣
 # ��ģ�������STM32F103�ڲ���RCCʱ�����ã������������õδ�ʱ������ʱ������
 # �ɸ����Լ�����Ҫ���ӻ�ɾ����

*********************************************************************************************/
#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "relay.h"
#include "oled0561.h"
#include "usart.h"
#include "ioi2c.h"
#include "MPU6050.h"
#include "math.h"
#include "fbm320.h"
#include "bmp280.h"

#define X_ACCEL_OFFSET -15000 
#define Y_ACCEL_OFFSET -7400 
#define Z_ACCEL_OFFSET (-27000 + 16384)
#define X_GYRO_OFFSET -53 
#define Y_GYRO_OFFSET 270
#define Z_GYRO_OFFSET 144


typedef struct Angle
{
    double X_Angle;
    double Y_Angle;
    double Z_Angle;   
} MPU6050_Angle;


void check_data(void) {
	int16_t	i, t[6]={0}, n_avg = 3000;
	int avg[6] = {0};
	for (i = 0; i < n_avg; i++) {
		MPU6050_READ2(t);	//���ٶ�
		avg[0] += t[0] + X_ACCEL_OFFSET;
		avg[1] += t[1] + Y_ACCEL_OFFSET;
		avg[2] += t[2] + Z_ACCEL_OFFSET;
		avg[3] += t[3] + X_GYRO_OFFSET;
		avg[4] += t[4] + Y_GYRO_OFFSET;
		avg[5] += t[5] + Z_GYRO_OFFSET;
		//printf("ACCEL_X,T,Z: %d\t,%d\t,%d\t\r\n", t[0], t[1], t[2]);
		//printf("GYRO_X,T,Z: %d\t,%d\t,%d\t\r\n", t[3], t[4], t[5]);
		delay_ms(10); //��ʱ������ˢ���ٶȣ�
	}
	avg[0] /= n_avg;
	avg[1] /= n_avg;
	avg[2] /= n_avg;
	avg[3] /= n_avg;
	avg[4] /= n_avg;
	avg[5] /= n_avg;
	
	printf("Average GYRO_X,Y,Z: %d, %d, %d\r\n", avg[3], avg[4], avg[5]);
	printf("Average ACCEL_X,Y,Z: %d, %d, %d\r\n", avg[0], avg[1], avg[2]);
	
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET); //��鷢���жϱ�־λ

	delay_ms(1000); //��ʱ������ˢ���ٶȣ�    
}

void zero_correct(int16_t* n) {
 	n[0] += X_ACCEL_OFFSET;
	n[1] += Y_ACCEL_OFFSET;
	n[2] += Z_ACCEL_OFFSET;
	n[3] += X_GYRO_OFFSET;
	n[4] += Y_GYRO_OFFSET;
	n[5] += Z_GYRO_OFFSET;
}

void display_data(void) {
  	int16_t	t[6]={0};
	MPU6050_READ2(t);
	//zero_correct(t);
	//printf("ACCEL_X,Y,Z: %d, %d, %d\r\n", t[0], t[1], t[2]);
	//printf("GYRO_X,Y,Z: %d, %d, %d\r\n", t[3], t[4], t[5]);
	//printf("ACCEL_X,Y,Z: %lfg, %lfg, %lfg\r\n", t[0]/16384.0, t[1]/16384.0, t[2]/16384.0);
	printf("GYRO_X,Y,Z: %lf,%lf,%lf\r\n", t[3]/16.384, t[4]/16.384, t[5]/16.384);
	delay_ms(10); //��ʱ������ˢ���ٶȣ�
}

void to_ground(void) {
	u16	t[6]={0};
	MPU6050_READ(t);
	printf("%d,%d,%d,%d,%d,%d\r\n", t[0], t[1], t[2], t[3], t[4], t[5]);
	delay_ms(10);
}

void MPU6050_Get_Angle(MPU6050_Angle* data) {
	int16_t	t[6]={0};
	MPU6050_READ2(t);
	zero_correct(t);
	data->X_Angle = acos(t[0] / 16384.0);
	data->Y_Angle = acos(t[1] / 16384.0);
	data->Z_Angle = acos(t[2] / 16384.0);

	data->X_Angle = data->X_Angle * 57.29577;
    data->Y_Angle = data->Y_Angle * 57.29577;
    data->Z_Angle = data->Z_Angle * 57.29577;
}

void check_angle(void) {
 	MPU6050_Angle data;
	MPU6050_Get_Angle(&data);
	printf("X_Angle = %lf'", data.X_Angle);
    printf("Y_Angle = %lf'", data.Y_Angle);
    printf("Z_Angle = %lf'", data.Z_Angle);
    printf("\r\n");
	delay_ms(100);
}

void print_rslt(const char api_name[], int8_t rslt)
{
    if (rslt != BMP280_OK)
    {
        printf("%s\t", api_name);
        if (rslt == BMP280_E_NULL_PTR)
        {
            printf("Error [%d] : Null pointer error\r\n", rslt);
        }
        else if (rslt == BMP280_E_COMM_FAIL)
        {
            printf("Error [%d] : Bus communication failed\r\n", rslt);
        }
        else if (rslt == BMP280_E_IMPLAUS_TEMP)
        {
            printf("Error [%d] : Invalid Temperature\r\n", rslt);
        }
        else if (rslt == BMP280_E_DEV_NOT_FOUND)
        {
            printf("Error [%d] : Device not found\r\n", rslt);
        }
        else
        {
            /* For more error codes refer "*_defs.h" */
            printf("Error [%d] : Unknown error code\r\n", rslt);
        }
    }
}


int main (void){//������
	int8_t err = -1, rslt = 0;
	struct bmp280_dev bmp;
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	USART1_Init(115200);
	I2C_Configuration();//I2C��ʼ��
	
	bmp.delay_ms = delay_ms;
	bmp.dev_id = BMP280_I2C_ADDR_PRIM;
	bmp.intf = BMP280_I2C_INTF;
	bmp.read = i2c_reg_read;
    bmp.write = i2c_reg_write;
	
	//rslt = bmp280_init(&bmp);
	//while (1)
	//{
	//	printf("%d\r\n", rslt);
	//	//print_rslt(" bmp280_init status", rslt);
	//}
	
	
	
	MPU6050_Init(); //MPU6050��ʼ��
	//SPI_GPIO_Init();
	//SPI1_Init();
	//err = fbm320_init();

	//NRF24L01_Init();

	//printf("nrf24l01 start!\r\n");
	//while (NRF24L01_Check())
	//{
	//		printf("nrf24l01 01 failed!\r\n");
	//}

	while(1){
		//printf("nrf24l01 01 succeed!!!\r\n");
		//printf("%d\r\n", err);
		//check_data();
		//display_data();
		//printf("Hello, World.\r\n");
		to_ground();
		//check_angle();
		//printf("Read_DMP Return is %d\n",Read_DMP(&Pitch,&Roll,&Yaw));
		//printf("Pitch is:%f,Roll is:%f,Yaw is:%f\n",Pitch,Roll,Yaw);
	}
}

/*********************************************************************************************
 * �������� www.DoYoung.net
 * ���ҵ��� www.DoYoung.net/YT 
*********************************************************************************************/
/*

���������塿
u32     a; //����32λ�޷��ű���a
u16     a; //����16λ�޷��ű���a
u8     a; //����8λ�޷��ű���a
vu32     a; //�����ױ��32λ�޷��ű���a
vu16     a; //�����ױ�� 16λ�޷��ű���a
vu8     a; //�����ױ�� 8λ�޷��ű���a
uc32     a; //����ֻ����32λ�޷��ű���a
uc16     a; //����ֻ�� ��16λ�޷��ű���a
uc8     a; //����ֻ�� ��8λ�޷��ű���a

#define ONE  1   //�궨��

delay_us(1); //��ʱ1΢��
delay_ms(1); //��ʱ1����
delay_s(1); //��ʱ1��

GPIO_WriteBit(LEDPORT,LED1,(BitAction)(1)); //LED����

*/



