

/*
//杜洋工作室出品
//洋桃系列开发板应用程序
//关注微信公众号：洋桃电子
//洋桃开发板资料下载 www.DoYoung.net/YT 
//即可免费看所有教学视频，下载技术资料，技术疑难提问
//更多内容尽在 杜洋工作室主页 www.doyoung.net
*/

/*
《修改日志》
1-201708202309 创建。


*/



#include "MPU6050.h"
#include "usart.h"
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


void MPU6050_Init(void){  //初始化MPU6050
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_PWR_MGMT_1,0x80);//解除休眠状态
	delay_ms(1000); //等待器件就绪
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_PWR_MGMT_1,0x00);//解除休眠状态
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_SMPLRT_DIV,0x07);//陀螺仪采样率
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_CONFIG,0x06);	 
	//I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_ACCEL_CONFIG,0x00);//配置加速度传感器工作在16G模式
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 0x01);
	
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_GYRO_CONFIG,0x18);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_ACCEL_CONFIG,0x08);//配置加速度传感器工作在4G模式 (四种量程2g，4g，8g，16g，对应的值分别为0x00，0x08，0x10，0x18)
}

void MPU6050_READ(u16* n){ //读出X、Y、Z三轴加速度/陀螺仪原始数据 //n[0]是AX，n[1]是AY，n[2]是AZ，n[3]是GX，n[4]是GY，n[5]是GZ 
	u8 i;
    u8 t[14]; 
    I2C_READ_BUFFER(MPU6050_ADD, MPU6050_RA_ACCEL_XOUT_H, t, 14); //读出连续的数据地址，包括了加速度和陀螺仪共12字节
    for(i=0; i<3; i++) 	//整合加速度
      n[i]=((t[2*i] << 8) + t[2*i+1]);
    for(i=4; i<7; i++)	//整合陀螺仪
      n[i-1]=((t[2*i] << 8) + t[2*i+1]);        
}

void MPU6050_READ2(int16_t* n) {
	u8 i;
    u8 t[14]; 
    I2C_READ_BUFFER(MPU6050_ADD, MPU6050_RA_ACCEL_XOUT_H, t, 14); //读出连续的数据地址，包括了加速度和陀螺仪共12字节
    for(i=0; i<3; i++) 	//整合加速度
      n[i]=((t[2*i] << 8) + t[2*i+1]);
    for(i=4; i<7; i++)	//整合陀螺仪
      n[i-1]=((t[2*i] << 8) + t[2*i+1]);	
}


 
/*********************************************************************************************
 * 杜洋工作室 www.DoYoung.net
 * 洋桃电子 www.DoYoung.net/YT 
*********************************************************************************************/
