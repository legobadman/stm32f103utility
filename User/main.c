#include "stm32f10x.h" //STM32ͷ�ļ�
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "i2c_hardware.h"
#include "i2c_software.h"
#include "spi.h"
#include "MPU6050.h"
#include "math.h"
#include "bmp280.h"
#include "fbm320.h"
#include "HMC5883L.h"
#include "nrf24l01.h"
#include "BT.h"
#include "adc.h"
#include "Motor.h"
#include "pwm.h"


#define X_ACCEL_OFFSET -15000 
#define Y_ACCEL_OFFSET -7400 
#define Z_ACCEL_OFFSET (-27000 + 16384)
#define X_GYRO_OFFSET -53 
#define Y_GYRO_OFFSET 270
#define Z_GYRO_OFFSET 144

//#define ENABLE_I2C
//#define I2c_Hardware
extern vu16 ADC_DMA_IN[4];	//ҡ����ֵ��ŵ�

bool bLocked = true;

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
#ifdef I2c_Hardware
	MPU6050_READ(t);
#else
	MPU6050_READ3(t);
#endif
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


//#define DEBUG_MPU6050
//#define DEBUG_HMC5883
//#define DEBUG_BT
//#define DEBUG_BMP
//#define DEBUG_NRF24L01
//#define DEBUG_FBM320
//#define DEBUG_MOTOR
//#define DEBUG_REMOTE
//#define DEBUG_PWM

uint8_t txbuf[5] = {0};
uint8_t rxbuf[RX_PLOAD_WIDTH] = { 0 };

//#define DEBUG_BMP

int main (void){//������
	int8_t err = -1;
	uint8_t ret,i;
	bool wtf = false;
	unsigned char c = 0;
	float pressure = 0, temperature = 0, asl = 0;
	delay_ms(500); //�ϵ�ʱ�ȴ�������������
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration(); //ϵͳʱ�ӳ�ʼ�� 
	USART1_Init(115200);
	SPI_GPIO_Init();
	SPI1_Init();
	printf("core init\r\n");

#ifdef ENABLE_I2C
#ifdef I2c_Hardware
	I2C_Configuration();//I2C��ʼ��
#else
	IIC_Init();
#endif
#endif

#ifdef DEBUG_BT	
	BT_PowerInit();
	BT_init();
	while(1) {
	}
#endif

#ifdef DEBUG_NRF24L01


	
#endif

#ifdef DEBUG_MPU6050	
#ifdef I2c_Hardware
	MPU6050_Init(); //MPU6050��ʼ��
#else
	MPU6050_Init2(); //MPU6050��ʼ��
#endif
	while(1) {
		to_ground();
		//check_data();
		//display_data();
		//printf("Hello, World.\r\n");
		//to_ground();
		//check_angle();
		//printf("Read_DMP Return is %d\n",Read_DMP(&Pitch,&Roll,&Yaw));
		//printf("Pitch is:%f,Roll is:%f,Yaw is:%f\n",Pitch,Roll,Yaw);
	}
#endif
	
#ifdef DEBUG_HMC5883
	HMC5883L_I2C_Init();
	//HMC5883L_Initialize();
	wtf = HMC5883L_TestConnection();
	while (1) {
		if (wtf) {
			printf("succeed!\r\n");
		} else {
			printf("failed!\r\n");
		}
	}
#endif
	
#ifdef DEBUG_BMP	
	c = bmp280_init();
	//printf("bmp280-id: 0x%x\r\n", c);
	while (1) {
		bmp280GetData(&pressure, &temperature, &asl);
		printf("pressure is %f\r\n", pressure);
		printf("temperature is %f\r\n", temperature);
		delay_ms(200);
		//printf("id: %4x\r\n", c);
	}
#endif

#ifdef DEBUG_FBM320
	fbm320_init();
#endif

#ifdef DEBUG_MOTOR
	MotorInit();
	while(1) {
		//GPIO_SetBits(GPIOA, GPIO_Pin_0);
		//delay_ms(4);
		//GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		//delay_ms(20);
	}
#endif
	
#ifdef DEBUG_PWM

	//M1
	TIM2_PWM_Init(59999, 23);	//����Ƶ��Ϊ50Hz����ʽΪ�����ʱ��Tout����λ�룩=(arr+1) (psc+1) / Tclk
								//20MS = (59999+1)*(23+1) / 72000000
								//ȷ����PWM����Ϊ20ms (50Hz)�������Զ�װ��

	TIM_SetCompare1(TIM2, 0);	//1ָ����ͨ��
	TIM_SetCompare2(TIM2, 1500);
	while(1) {
		
	}
#endif

	NRF24L01_Init();
	printf("nrf24l01 start!\r\n");
	while (NRF24L01_Check())
	{
		printf("nrf24l01 01 failed!\r\n");
	}
	NRF24L01_RX_Mode();

	//Main
	TIM2_PWM_Init(59999, 23);	//����Ƶ��Ϊ50Hz����ʽΪ�����ʱ��Tout����λ�룩=(arr+1) (psc+1) / Tclk
								//20MS = (59999+1)*(23+1) / 72000000
								//ȷ����PWM����Ϊ20ms (50Hz)�������Զ�װ��
	TIM_SetCompare1(TIM2, 0);	//1ָ����ͨ��
	TIM_SetCompare2(TIM2, 0);

	while (1)
	{
		ret = NRF24L01_RX_Packet(rxbuf);
		if (ret == 0)
		{
			ADC_DMA_IN[0] = rxbuf[1];
			ADC_DMA_IN[0] = (ADC_DMA_IN[0] << 8) | rxbuf[0];

			ADC_DMA_IN[1] = rxbuf[3];
			ADC_DMA_IN[1] = (ADC_DMA_IN[1] << 8) | rxbuf[2];

			ADC_DMA_IN[2] = rxbuf[5];
			ADC_DMA_IN[2] = (ADC_DMA_IN[2] << 8) | rxbuf[4];

			ADC_DMA_IN[3] = rxbuf[7];
			ADC_DMA_IN[3] = (ADC_DMA_IN[3] << 8) | rxbuf[6];

			printf("Left X:%d, Y:%d\r\n", ADC_DMA_IN[0], ADC_DMA_IN[1]);
			printf("Right X:%d, Y:%d\r\n", ADC_DMA_IN[2], ADC_DMA_IN[3]);

			if (rxbuf[8] == 1)
			{
				bLocked = !bLocked;	
			}
			if (bLocked == false)
			{
				TIM_SetCompare2(TIM2, 1500);
			}
			else
			{
				TIM_SetCompare2(TIM2, 0);
			}
		}
		else
		{
			printf("something wrong\r\n");
		}
		delay_ms(200);	 //��ʱ300ms
	}


	
}