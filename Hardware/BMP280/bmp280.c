#include "bmp280.h"
#include "i2c_software.h"
#include "delay.h"

unsigned char bmp280_init()
{
	unsigned char bmp280ID;
	//ֱ�Ӷ��豸��ַ����work��work
	delay_ms(20);
	bmp280ID = I2C_ReadOneByte(BMP280_ADDR, BMP280_CHIP_ID);
	return bmp280ID;
}