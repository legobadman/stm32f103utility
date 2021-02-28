#include "qmc5883l.h"
#include "i2c_software.h"

static void QMC5883L_IOInit(void)
{

}

u8 QMC5883L_Read_Reg(u8 reg)
{
	u8 wtf = I2C_ReadOneByte(QMC5883L_ADDR_READ, reg);
	return wtf;
}

void QMC5883L_Write_Reg(uint8_t reg, uint8_t data)
{
	IICWriteByte(QMC5883L_ADDR_WRITE, reg, data);
}

bool QMC5883L_Init(void)
{
	u8 uErrTime = 0;
	QMC5883L_IOInit();
	//QMC5883L_POWER_ON();

	/* 读取器件ID */
	u8 wtf = I2C_ReadOneByte(QMC5883L_ADDR_READ, QMC5883L_ADDR_CHIPID);
	while (wtf != QMC5883L_CHIPID_VALUE)
	{
		uErrTime++;
		if (uErrTime > 250)
		{
			return false;
		}
	}
	//I2C_
	IICWriteByte(QMC5883L_ADDR_WRITE, QMC5883L_ADDR_CFGA, QMC5883L_CFGA_VALUE_STANDBY);
	IICWriteByte(QMC5883L_ADDR_WRITE, QMC5883L_ADDR_CFGC, QMC5883L_CFGC_VALUE);
	IICWriteByte(QMC5883L_ADDR_WRITE, QMC5883L_ADDR_CFGD, QMC5883L_CFGD_VALUE);
	IICWriteByte(QMC5883L_ADDR_WRITE, QMC5883L_ADDR_PERIORC, QMC5883L_PERIORC_VALUE);

	wtf = I2C_ReadOneByte(QMC5883L_ADDR_READ, QMC5883L_ADDR_CFGA);
	if (wtf != QMC5883L_CFGA_VALUE_STANDBY)
	{
		return false;
	}

	//QMC5883L_Register_Write(QMC5883L_ADDR_CFGA, QMC5883L_CFGA_VALUE_STANDBY);   /* OSR = 512;RNG = 8G(0x1d RNG=8G);ODR=200Hz;MODE:待机模式*/
	//QMC5883L_Register_Write(QMC5883L_ADDR_CFGC, QMC5883L_CFGC_VALUE);
	//QMC5883L_Register_Write(QMC5883L_ADDR_CFGD, QMC5883L_CFGD_VALUE);
	//QMC5883L_Register_Write(QMC5883L_ADDR_PERIORC, QMC5883L_PERIORC_VALUE);

	//if (QMC5883L_Register_Read(QMC5883L_ADDR_CFGA) != QMC5883L_CFGA_VALUE_STANDBY)
	//{
	//	return false;
	//}
	return true;
}