#include "stm32f10x.h" //STM32头文件
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "i2c_hardware.h"
#include "i2c_software.h"
#include "spi.h"
#include "MPU6050.h"
#include "math.h"
#include "bmp280.h"
#include "HMC5883L.h"
#include "nrf24l01.h"
#include "BT.h"
#include "adc.h"
#include "Motor.h"
#include "pwm.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

//#define I2c_Hardware
#define IMU_SOFTWARE	//软件姿态解算

extern vu16 ADC_DMA_IN[4];	//摇杆数值存放点

bool bLocked = true;
uint16_t leftX = 0, leftY = 0, RightX = 0, RightY = 0;

#define DEBUG_MPU6050
#define IMU_SOFTWARE
#define USE_CRAZEPONY_DMP
#ifdef USE_CRAZEPONY_DMP
	#include "dmp.h"
#endif
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

int main (void){//主程序
	int8_t err = -1;
	uint8_t ret,i;
	float yaw, yaw_acc_error;
	bool wtf = false;
	unsigned char c = 0;
	float pressure = 0, temperature = 0, asl = 0;

	delay_ms(500); //上电时等待其他器件就绪
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	RCC_Configuration(); //系统时钟初始化 
	USART1_Init(115200);
	SPI_GPIO_Init();
	SPI1_Init();
	printf("core init\r\n");

#ifdef I2c_Hardware
	I2C_Configuration();//I2C初始化
#else
	IIC_Init();
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

#ifdef IMU_SOFTWARE
	MPU6050_Init(); //MPU6050初始化
	zero_padding(5000);
	while(1) {
		//to_ground();
		Accel_GetAngle();
		//Gyro_GetAngle();
		//to_angle();
		//check_data();
		//display_data();
		//printf("Hello, World.\r\n");
		//to_ground();
		//check_angle();
		//printf("Read_DMP Return is %d\n",Read_DMP(&Pitch,&Roll,&Yaw));
		//printf("Pitch is:%f,Roll is:%f,Yaw is:%f\n",Pitch,Roll,Yaw);
	}
#else
#ifdef USE_CRAZEPONY_DMP
	MPU6050_DMP_Initialize();     //初始化DMP引擎
	printf("DMP Inited!!!");
	while (1) {
		DMP_Routing();
		//DMP_getYawPitchRoll();
		//printf("Roll, Pitch, Yaw = %f, %f, %f\r\n", DMP_DATA.dmp_roll, DMP_DATA.dmp_pitch, DMP_DATA.dmp_yaw);
		printf("%f,%f,%f\r\n", DMP_DATA.dmp_roll, DMP_DATA.dmp_pitch, DMP_DATA.dmp_yaw);
		//delay_ms(20);
	}
#else
	MPU6050_Initialize();           //=====MPU6050初始化   
	DMP_Init();                     //=====初始化DMP 
	MBOT_EXTI_Init();               //=====MPU6050 5ms定时中断初始化
	printf("DMP Inited!!!");
	while (1)
	{
		getAngle(&yaw, &yaw_acc_error);
		printf("Roll, Pitch = %lf, %lf\r\n", Roll, Pitch);
	}
#endif
#endif
#endif
	
#ifdef DEBUG_HMC5883
	//HMC5883L_I2C_Init();
	HMC5883L_Init();
	//HMC5883L_Initialize();
	//wtf = HMC5883L_TestConnection();
	while (1) {
		IIC_Start();
		IIC_Send_Byte(0x3c); // HMC5883L,0x3c,0x03读取这两个地址的寄存器数据，分别为LSB和MSB
		IIC_Wait_Ack();
		IIC_Send_Byte(0x03); //
		IIC_Wait_Ack();

		IIC_Start();
		IIC_Send_Byte(0x3d); //读数据
		IIC_Wait_Ack();

		for (i = 0; i < 5; i++)        //第六次不应答
		{
			XYZ_Data[i] = IIC_Read_Byte(1);
		}
		XYZ_Data[5] = IIC_Read_Byte(0);
		IIC_Stop();
		delay_ms(5);

		//if (wtf) {
		//	printf("succeed!\r\n");
		//} else {
		//	printf("failed!\r\n");
		//}
	}
#endif
	
#ifdef DEBUG_BMP	
	c = bmp280_init();
	//while (1)
	//{
	//	printf("bmp280-id: %d\r\n", c);
	//}
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
	TIM2_PWM_Init(59999, 23);	//设置频率为50Hz，公式为：溢出时间Tout（单位秒）=(arr+1) (psc+1) / Tclk
								//20MS = (59999+1)*(23+1) / 72000000
								//确定了PWM周期为20ms (50Hz)，设置自动装载

	TIM_SetCompare1(TIM2, 1500);	//1指的是通道
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
	TIM2_PWM_Init(59999, 23);	//设置频率为50Hz，公式为：溢出时间Tout（单位秒）=(arr+1) (psc+1) / Tclk
								//20MS = (59999+1)*(23+1) / 72000000
								//确定了PWM周期为20ms (50Hz)，设置自动装载
	TIM_SetCompare1(TIM2, leftX);	//1指的是通道
	TIM_SetCompare2(TIM2, leftY);
	TIM_SetCompare3(TIM2, RightX);
	TIM_SetCompare4(TIM2, RightY);

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

			//左边的X轴是向下递增的，因此为了方便日常理解，这里调转过来成为Y轴。
			leftY = MAX(4000 - ADC_DMA_IN[0], 0);
			leftX = ADC_DMA_IN[1];
			RightX = MAX(4000 - ADC_DMA_IN[3], 0);
			RightY = MAX(4000 - ADC_DMA_IN[2], 0);

			printf("Left X:%d, Y:%d\r\n", leftX, leftY);
			printf("Right X:%d, Y:%d\r\n", RightX, RightY);

			if (rxbuf[8] == 1)
			{
				bLocked = !bLocked;
			}

			if (bLocked == false)
			{
				TIM_SetCompare1(TIM2, AcceleratorCurve(leftX));	//1指的是通道
				TIM_SetCompare2(TIM2, AcceleratorCurve(leftY));
				TIM_SetCompare3(TIM2, AcceleratorCurve(RightX));
				TIM_SetCompare4(TIM2, AcceleratorCurve(RightY));
			}
			else
			{
				TIM_SetCompare1(TIM2, 0);
				TIM_SetCompare2(TIM2, 0);
				TIM_SetCompare3(TIM2, 0);
				TIM_SetCompare4(TIM2, 0);
			}
		}
		else
		{
			printf("something wrong\r\n");
		}
		delay_ms(200);	 //延时300ms
	}
}