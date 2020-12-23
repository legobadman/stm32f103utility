

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
#include "i2c_software.h"
#include <math.h>
#include <stdbool.h>

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
short gyro[3], accel[3], sensors;
static signed char gyro_orientation[9] = { -1, 0, 0,
										   0,-1, 0,
										   0, 0, 1 };
float Pitch, Roll, Yaw;
float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;

void MPU6050_Init(void){  //初始化MPU6050
#ifdef I2c_Hardware
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_PWR_MGMT_1,0x80);//解除休眠状态
	delay_ms(1000); //等待器件就绪
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_PWR_MGMT_1,0x00);//解除休眠状态
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_SMPLRT_DIV,0x07);//陀螺仪采样率
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_CONFIG,0x06);	 
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_USER_CTRL, 0x00);
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 0x01);
	
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_GYRO_CONFIG,0x18);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C_SAND_BYTE(MPU6050_ADD,MPU6050_RA_ACCEL_CONFIG,0x08);//配置加速度传感器工作在4G模式 (四种量程2g，4g，8g，16g，对应的值分别为0x00，0x08，0x10，0x18)
#else
	IICwriteByte(MPU6050_ADD, MPU6050_RA_PWR_MGMT_1, 0x80);//解除休眠状态
	delay_ms(1000);
	IICwriteByte(MPU6050_ADD, MPU6050_RA_PWR_MGMT_1, 0x00);//解除休眠状态
	IICwriteByte(MPU6050_ADD, MPU6050_RA_SMPLRT_DIV, 0x07);//陀螺仪采样率
	IICwriteByte(MPU6050_ADD, MPU6050_RA_CONFIG, 0x06);
	IICwriteByte(MPU6050_ADD, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, 0x01);

	IICwriteByte(MPU6050_ADD, MPU6050_RA_GYRO_CONFIG, 0x18);//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	IICwriteByte(MPU6050_ADD, MPU6050_RA_ACCEL_CONFIG, 0x08);//配置加速度传感器工作在4G模式 (四种量程2g，4g，8g，16g，对应的值分别为0x00，0x08，0x10，0x18)
#endif
}

void MPU6050_READ(u16* n){ //读出X、Y、Z三轴加速度/陀螺仪原始数据 //n[0]是AX，n[1]是AY，n[2]是AZ，n[3]是GX，n[4]是GY，n[5]是GZ 
	u8 i;
    u8 t[14];
#ifdef I2c_Hardware
    I2C_READ_BUFFER(MPU6050_ADD, MPU6050_RA_ACCEL_XOUT_H, t, 14); //读出连续的数据地址，包括了加速度和陀螺仪共12字节
#else
    IICReadBytes(MPU6050_ADD, MPU6050_RA_ACCEL_XOUT_H, 14, t);
#endif
    for(i=0; i<3; i++) 	//整合加速度
      n[i]=((t[2*i] << 8) + t[2*i+1]);
    for(i=4; i<7; i++)	//整合陀螺仪
      n[i-1]=((t[2*i] << 8) + t[2*i+1]);        
}

uint8_t buffer[14];
int16_t  MPU6050_FIFO[6][11];
int16_t Gx_offset = 0, Gy_offset = 0, Gz_offset = 0;

/**************************实现函数********************************************
*函数原型:		unsigned char MPU6050_is_DRY(void)
*功　　能:	    检查 MPU6050的中断引脚，测试是否完成转换
返回 1  转换完成
0 数据寄存器还没有更新
*******************************************************************************/
unsigned char MPU6050_is_DRY(void) {
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == Bit_SET) {
		return 1;
	}
	else return 0;
}

void MPU6050_setClockSource(uint8_t source) {
	IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

void MPU6050_reset(void) {
	IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_DEVICE_RESET_BIT, 1);
}

/**************************实现函数********************************************
*函数原型:		uint8_tMPU6050_getDeviceID(void)
*功　　能:	    读取  MPU6050 WHO_AM_I 标识	 将返回 0x68
*******************************************************************************/
uint8_t MPU6050_getDeviceID(void) {

	IICReadBytes(devAddr, MPU6050_RA_WHO_AM_I, 1, buffer);
	return buffer[0];
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setSleepEnabled(uint8_tenabled)
*功　　能:	    设置  MPU6050 是否进入睡眠模式
                enabled =1   睡觉
                enabled =0   工作
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		uint8_tMPU6050_testConnection(void)
*功　　能:	    检测MPU6050 是否已经连接
*******************************************************************************/
uint8_t MPU6050_testConnection(void) {
	if (MPU6050_getDeviceID() == 0x68)  //0b01101000;
		return 1;
	else return 0;
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_tenabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_tenabled)
*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*功　　能:	    设置  MPU6050 加速度计的最大量程
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
	IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

void MPU6050_setFullScaleGyroRange(uint8_t range) {
	IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

void MPU6050_Check(void)
{
	switch (MPU6050_testConnection())
	{
	case 0:printf("MPU6050 not found...\r\n");
		break;
	case 1:printf("MPU6050 check success...\r\n");
		break;
	}
}


//读acc
void MPU6050AccRead(int16_t* accData)
{
    uint8_t buf[6];

    IICReadBytes(devAddr, MPU6050_RA_ACCEL_XOUT_H, 6, buf);
    accData[0] = (int16_t)((buf[0] << 8) | buf[1]);
    accData[1] = (int16_t)((buf[2] << 8) | buf[3]);
    accData[2] = (int16_t)((buf[4] << 8) | buf[5]);


}
//读gyro
void MPU6050GyroRead(int16_t* gyroData)
{
    uint8_t buf[6];

    IICReadBytes(devAddr, MPU6050_RA_GYRO_XOUT_H, 6, buf);
    gyroData[0] = (int16_t)((buf[0] << 8) | buf[1]);
    gyroData[1] = (int16_t)((buf[2] << 8) | buf[3]);
    gyroData[2] = (int16_t)((buf[4] << 8) | buf[5]);
}


//用于校准 DMP的偏置值
void MPU6050_setAccOffset(int16_t offset[3])
{
    uint8_t buf[2], i = 0;
    for (i = 0; i < 3; i++)
    {
        buf[0] = offset[i] >> 8;
        buf[1] = offset[i];
        IICwriteBytes(devAddr, MPU6050_RA_XA_OFFS_H + i * 2, 2, buf);
    }
}

void MPU6050_setGyroOffset(int16_t offset[3])
{
    uint8_t buf[2], i = 0;
    for (i = 0; i < 3; i++)
    {
        buf[0] = offset[i] >> 8;
        buf[1] = offset[i];
        IICwriteBytes(devAddr, MPU6050_RA_XG_OFFS_USRH + i * 2, 2, buf);
    }
}



// BANK_SEL register
void MPU6050_setMemoryBank(uint8_t bank, uint8_t prefetchEnabled, uint8_t userBank) {
    bank &= 0x1F;
    if (userBank) bank |= 0x20;
    if (prefetchEnabled) bank |= 0x40;
    IICwriteByte(devAddr, MPU6050_RA_BANK_SEL, bank);
}

// MEM_START_ADDR register
void MPU6050_setMemoryStartAddress(uint8_t address) {
    IICwriteByte(devAddr, MPU6050_RA_MEM_START_ADDR, address);
}

// MEM_R_W register
uint8_t MPU6050_readMemoryByte(void) {
    IICReadBytes(devAddr, MPU6050_RA_MEM_R_W, 1, buffer);
    return buffer[0];
}

// XG_OFFS_USR* registers
int16_t MPU6050_getXGyroOffset(void) {
    IICReadBytes(devAddr, MPU6050_RA_XG_OFFS_USRH, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}
int16_t MPU6050_getYGyroOffset(void) {
    IICReadBytes(devAddr, MPU6050_RA_YG_OFFS_USRH, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}
int16_t MPU6050_getZGyroOffset(void) {
    IICReadBytes(devAddr, MPU6050_RA_ZG_OFFS_USRH, 2, buffer);
    return (((int16_t)buffer[0]) << 8) | buffer[1];
}

uint8_t verifyBuffer[MPU6050_DMP_MEMORY_CHUNK_SIZE];
uint8_t progBuffer[MPU6050_DMP_MEMORY_CHUNK_SIZE];
uint8_t MPU6050_writeMemoryBlock(const uint8_t* data, uint16_t dataSize, uint8_t bank, uint8_t address, uint8_t verify, uint8_t useProgMem) {
    uint8_t chunkSize;
    //uint8_t *verifyBuffer;
    uint8_t* tprogBuffer;
    uint16_t i;
    uint8_t j;
    MPU6050_setMemoryBank(bank, 0, 0);
    MPU6050_setMemoryStartAddress(address);
    for (i = 0; i < dataSize;) {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) chunkSize = 256 - address;

        // write the chunk of data as specified
        tprogBuffer = (uint8_t*)data + i;


        IICwriteBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, tprogBuffer);

        // verify data if needed
        if (verify) {
            MPU6050_setMemoryBank(bank, 0, 0);
            MPU6050_setMemoryStartAddress(address);
            IICReadBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, verifyBuffer);

            for (j = 0; j < chunkSize; j++) {
                if (tprogBuffer[j] != verifyBuffer[j])
                    return 0; // uh oh.
            }
        }

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_tautomatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) {
            if (address == 0) bank++;
            MPU6050_setMemoryBank(bank, 0, 0);
            MPU6050_setMemoryStartAddress(address);
        }
    }
    return 1;
}

uint8_t MPU6050_writeDMPConfigurationSet(const uint8_t* data, uint16_t dataSize, uint8_t useProgMem) {
    uint8_t* progBuffer, success, special;
    uint16_t i;

    // config set data is a long string of blocks with the following structure:
    // [bank] [offset] [length] [byte[0], byte[1], ..., byte[length]]
    uint8_t bank, offset, length;
    for (i = 0; i < dataSize;) {
        bank = data[i++];
        offset = data[i++];
        length = data[i++];

        // write data or perform special action
        if (length > 0) {
            // regular block of data to write
            progBuffer = (uint8_t*)data + i;

            success = MPU6050_writeMemoryBlock(progBuffer, length, bank, offset, 1, 0);
            i += length;
        }
        else {
            // special instruction
            // NOTE: this kind of behavior (what and when to do certain things)
            // is totally undocumented. This code is in here based on observed
            // behavior only, and exactly why (or even whether) it has to be here
            // is anybody's guess for now.

            special = data[i++];
            if (special == 0x01) {
                // enable DMP-related interrupts

                IICwriteByte(devAddr, MPU6050_RA_INT_ENABLE, 0x32);  // single operation

                success = 1;
            }
            else {
                // unknown special command
                success = 0;
            }
        }

        if (!success) {
            return 0; // uh oh
        }
    }
    return 1;
}

uint8_t MPU6050_writeProgMemoryBlock(const uint8_t* data, uint16_t dataSize, uint8_t bank, uint8_t address, uint8_t verify) {
    return MPU6050_writeMemoryBlock(data, dataSize, bank, address, verify, 0);
}

uint8_t MPU6050_writeProgDMPConfigurationSet(const uint8_t* data, uint16_t dataSize) {
    return MPU6050_writeDMPConfigurationSet(data, dataSize, 0);
}

/** Set full interrupt enabled status.
 * Full register byte for all interrupts, for quick reading. Each bit should be
 * set 0 for disabled, 1 for enabled.
 * @param enabled New interrupt enabled status
 * @see getIntFreefallEnabled()
 * @see MPU6050_RA_INT_ENABLE
 * @see MPU6050_INTERRUPT_FF_BIT
 **/
void MPU6050_setIntEnabled(uint8_t enabled) {
    IICwriteByte(devAddr, MPU6050_RA_INT_ENABLE, enabled);
}

/** Set gyroscope sample rate divider.
 * @param rate New sample rate divider
 * @see getRate()
 * @see MPU6050_RA_SMPLRT_DIV
 */
void MPU6050_setRate(uint8_t rate) {
    IICwriteByte(devAddr, MPU6050_RA_SMPLRT_DIV, rate);
}

/** Set digital low-pass filter configuration.
 * @param mode New DLFP configuration setting
 * @see getDLPFBandwidth()
 * @see MPU6050_DLPF_BW_256
 * @see MPU6050_RA_CONFIG
 * @see MPU6050_CFG_DLPF_CFG_BIT
 * @see MPU6050_CFG_DLPF_CFG_LENGTH
 */
void MPU6050_setDLPFMode(uint8_t mode) {
    IICwriteBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}

/** Set external FSYNC configuration.
 * @see getExternalFrameSync()
 * @see MPU6050_RA_CONFIG
 * @param sync New FSYNC configuration value
 */
void MPU6050_setExternalFrameSync(uint8_t sync) {
    IICwriteBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_EXT_SYNC_SET_BIT, MPU6050_CFG_EXT_SYNC_SET_LENGTH, sync);
}

void MPU6050_setDMPConfig1(uint8_t config) {
    IICwriteByte(devAddr, MPU6050_RA_DMP_CFG_1, config);
}

void MPU6050_setDMPConfig2(uint8_t config) {
    IICwriteByte(devAddr, MPU6050_RA_DMP_CFG_2, config);
}

void MPU6050_setOTPBankValid(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OTP_BNK_VLD_BIT, enabled);
}


void MPU6050_setXGyroOffset(int16_t offset) {
    buffer[0] = offset >> 8;
    buffer[1] = offset & 0x00ff;
    IICwriteBytes(devAddr, MPU6050_RA_XG_OFFS_USRH, 2, buffer);
}
void MPU6050_setYGyroOffset(int16_t offset) {
    buffer[0] = offset >> 8;
    buffer[1] = offset & 0x00ff;
    IICwriteBytes(devAddr, MPU6050_RA_YG_OFFS_USRH, 2, buffer);
}

void MPU6050_setZGyroOffset(int16_t offset) {
    buffer[0] = offset >> 8;
    buffer[1] = offset & 0x00ff;
    IICwriteBytes(devAddr, MPU6050_RA_ZG_OFFS_USRH, 2, buffer);
}

/** Reset the FIFO.
 * This bit resets the FIFO buffer when set to 1 while FIFO_EN equals 0. This
 * bit automatically clears to 0 after the reset has been triggered.
 * @see MPU6050_RA_USER_CTRL
 * @see MPU6050_USERCTRL_FIFO_RESET_BIT
 */
void MPU6050_resetFIFO(void) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_RESET_BIT, 1);
}

/** Get current FIFO buffer size.
 * This value indicates the number of bytes stored in the FIFO buffer. This
 * number is in turn the number of bytes that can be read from the FIFO buffer
 * and it is directly proportional to the number of samples available given the
 * set of sensor data bound to be stored in the FIFO (register 35 and 36).
 * @return Current FIFO buffer size
 */
uint16_t MPU6050_getFIFOCount(void) {
    IICReadBytes(devAddr, MPU6050_RA_FIFO_COUNTH, 2, buffer);
    return (((uint16_t)buffer[0]) << 8) | buffer[1];
}

/** Set free-fall event acceleration threshold.
 * @param threshold New motion detection acceleration threshold value (LSB = 2mg)
 * @see getMotionDetectionThreshold()
 * @see MPU6050_RA_MOT_THR
 */
void MPU6050_setMotionDetectionThreshold(uint8_t threshold) {
    IICwriteByte(devAddr, MPU6050_RA_MOT_THR, threshold);
}

/** Set zero motion detection event acceleration threshold.
 * @param threshold New zero motion detection acceleration threshold value (LSB = 2mg)
 * @see getZeroMotionDetectionThreshold()
 * @see MPU6050_RA_ZRMOT_THR
 */
void MPU6050_setZeroMotionDetectionThreshold(uint8_t threshold) {
    IICwriteByte(devAddr, MPU6050_RA_ZRMOT_THR, threshold);
}

/** Set motion detection event duration threshold.
 * @param duration New motion detection duration threshold value (LSB = 1ms)
 * @see getMotionDetectionDuration()
 * @see MPU6050_RA_MOT_DUR
 */
void MPU6050_setMotionDetectionDuration(uint8_t duration) {
    IICwriteByte(devAddr, MPU6050_RA_MOT_DUR, duration);
}

/** Set zero motion detection event duration threshold.
 * @param duration New zero motion detection duration threshold value (LSB = 1ms)
 * @see getZeroMotionDetectionDuration()
 * @see MPU6050_RA_ZRMOT_DUR
 */
void MPU6050_setZeroMotionDetectionDuration(uint8_t duration) {
    IICwriteByte(devAddr, MPU6050_RA_ZRMOT_DUR, duration);
}

void MPU6050_readMemoryBlock(uint8_t* data, uint16_t dataSize, uint8_t bank, uint8_t address) {
    uint8_t chunkSize;
    uint16_t i;
    MPU6050_setMemoryBank(bank, 0, 0);
    MPU6050_setMemoryStartAddress(address);

    for (i = 0; i < dataSize;) {
        // determine correct chunk size according to bank position and data size
        chunkSize = MPU6050_DMP_MEMORY_CHUNK_SIZE;

        // make sure we don't go past the data size
        if (i + chunkSize > dataSize) chunkSize = dataSize - i;

        // make sure this chunk doesn't go past the bank boundary (256 bytes)
        if (chunkSize > 256 - address) chunkSize = 256 - address;

        // read the chunk of data as specified
        IICwriteBytes(devAddr, MPU6050_RA_MEM_R_W, chunkSize, data + i);

        // increase byte index by [chunkSize]
        i += chunkSize;

        // uint8_tautomatically wraps to 0 at 256
        address += chunkSize;

        // if we aren't done, update bank (if necessary) and address
        if (i < dataSize) {
            if (address == 0) bank++;
            MPU6050_setMemoryBank(bank, 0, 0);
            MPU6050_setMemoryStartAddress(address);
        }
    }
}

void MPU6050_getFIFOBytes(uint8_t* data, uint8_t length) {
    IICReadBytes(devAddr, MPU6050_RA_FIFO_R_W, length, data);
}

/** Get full set of interrupt status bits.
 * These bits clear to 0 after the register has been read. Very useful
 * for getting multiple INT statuses, since each single bit read clears
 * all of them because it has to read the whole byte.
 * @return Current interrupt status
 * @see MPU6050_RA_INT_STATUS
 */
uint8_t MPU6050_getIntStatus(void) {
    return I2C_ReadOneByte(devAddr, MPU6050_RA_INT_STATUS);
}

void MPU6050_setDMPEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_EN_BIT, enabled);
}

uint8_t MPU6050_getOTPBankValid(void) {
    uint8_t  temp = I2C_ReadOneByte(devAddr, MPU6050_RA_XG_OFFS_TC);
    return temp & (1 << MPU6050_TC_OTP_BNK_VLD_BIT);
}

int8_t MPU6050_getXGyroOffsetTC(void) {
    uint8_t  temp = I2C_ReadOneByte(devAddr, MPU6050_RA_XG_OFFS_TC);
    temp &= 0x3F;
    return temp;
}
void MPU6050_setXGyroOffsetTC(int8_t offset) {
    IICwriteBits(devAddr, MPU6050_RA_XG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, offset);
}


// YG_OFFS_TC register
int8_t MPU6050_getYGyroOffsetTC(void) {
    uint8_t  temp = I2C_ReadOneByte(devAddr, MPU6050_RA_YG_OFFS_TC);
    temp &= 0x3F;
    return temp;
}
void MPU6050_setYGyroOffsetTC(int8_t offset) {
    IICwriteBits(devAddr, MPU6050_RA_YG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, offset);
}

// ZG_OFFS_TC register
int8_t MPU6050_getZGyroOffsetTC(void) {
    uint8_t  temp = I2C_ReadOneByte(devAddr, MPU6050_RA_ZG_OFFS_TC);
    temp &= 0x3F;
    return temp;
}
void MPU6050_setZGyroOffsetTC(int8_t offset) {
    IICwriteBits(devAddr, MPU6050_RA_ZG_OFFS_TC, MPU6050_TC_OFFSET_BIT, MPU6050_TC_OFFSET_LENGTH, offset);
}

/** Set the I2C address of the specified slave (0-3).
 * @param num Slave number (0-3)
 * @param address New address for specified slave
 * @see getSlaveAddress()
 * @see MPU6050_RA_I2C_SLV0_ADDR
 */
void MPU6050_setSlaveAddress(uint8_t num, uint8_t address) {
    if (num > 3) return;
    IICwriteByte(devAddr, MPU6050_RA_I2C_SLV0_ADDR + num * 3, address);
}

/** Reset the I2C Master.
 * This bit resets the I2C Master when set to 1 while I2C_MST_EN equals 0.
 * This bit automatically clears to 0 after the reset has been triggered.
 * @see MPU6050_RA_USER_CTRL
 * @see MPU6050_USERCTRL_I2C_MST_RESET_BIT
 */
void MPU6050_resetI2CMaster(void) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_RESET_BIT, 1);
}

/** Set FIFO enabled status.
 * @param enabled New FIFO enabled status
 * @see getFIFOEnabled()
 * @see MPU6050_RA_USER_CTRL
 * @see MPU6050_USERCTRL_FIFO_EN_BIT
 */
void MPU6050_setFIFOEnabled(uint8_t enabled) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_EN_BIT, enabled);
}

void MPU6050_resetDMP(void) {
    IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_DMP_RESET_BIT, 1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

#define X_ACCEL_OFFSET 250 
#define Y_ACCEL_OFFSET 160 
#define Z_ACCEL_OFFSET (-6885 + 8192 + 60)
#define X_GYRO_OFFSET 36 
#define Y_GYRO_OFFSET -40
#define Z_GYRO_OFFSET 51
#define deg2rad(deg) (0.01745329252*(deg))

float Q[4] = { 1, 0, 0, 0 };

void check_data(void) {
	int16_t	i, n_avg = 3000;
	u16 t[6] = { 0 };
	int avg[6] = { 0 };
	for (i = 0; i < n_avg; i++) {
		MPU6050_READ(t);	//加速度
		avg[0] += (s16)t[0] + X_ACCEL_OFFSET;
		avg[1] += (s16)t[1] + Y_ACCEL_OFFSET;
		avg[2] += (s16)t[2] + Z_ACCEL_OFFSET;
		avg[3] += (s16)t[3] + X_GYRO_OFFSET;
		avg[4] += (s16)t[4] + Y_GYRO_OFFSET;
		avg[5] += (s16)t[5] + Z_GYRO_OFFSET;
		//printf("ACCEL_X,T,Z: %d\t,%d\t,%d\t\r\n", t[0], t[1], t[2]);
		//printf("GYRO_X,T,Z: %d\t,%d\t,%d\t\r\n", t[3], t[4], t[5]);
		delay_ms(10); //延时（决定刷新速度）
	}
	avg[0] /= n_avg;
	avg[1] /= n_avg;
	avg[2] /= n_avg;
	avg[3] /= n_avg;
	avg[4] /= n_avg;
	avg[5] /= n_avg;

	printf("Average GYRO_X,Y,Z: %d, %d, %d\r\n", avg[3], avg[4], avg[5]);
	printf("Average ACCEL_X,Y,Z: %d, %d, %d\r\n", avg[0], avg[1], avg[2]);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); //检查发送中断标志位

	delay_ms(1000); //延时（决定刷新速度）    
}

void zero_correct(int16_t* n) {
	n[0] += X_ACCEL_OFFSET;
	n[1] += Y_ACCEL_OFFSET;
	n[2] += Z_ACCEL_OFFSET;
	n[3] += X_GYRO_OFFSET;
	n[4] += Y_GYRO_OFFSET;
	n[5] += Z_GYRO_OFFSET;
}

void unit_transfer(int16_t* n) {

}

enum PROCESS_TYPE
{
	ORIGNAL,	//原始数据
	SCALE,		//除以量程
	ZERO_PADDING,
	ZERO_PADDING2, //减去偏移并且除以量程
};

static void message(int type, u16 t[])
{
	if (type == ORIGNAL) {
		printf("%d,%d,%d,%d,%d,%d\r\n", (vs16)t[0], (vs16)t[1], (vs16)t[2], (vs16)t[3], (vs16)t[4], (vs16)t[5]);
	} else if (type == SCALE) {
		printf("%lf,%lf,%lf,%lf,%lf,%lf\r\n", (vs16)t[0]/8192.0f, (vs16)t[1]/8192.0f, (vs16)t[2]/8192.0f, (vs16)t[3]/16.384f, (vs16)t[4]/16.384f, (vs16)t[5]/16.384f);
	} else if (type == ZERO_PADDING) {
		printf("%d,%d,%d,%d,%d,%d\r\n", (vs16)t[0] + X_ACCEL_OFFSET, (vs16)t[1] + Y_ACCEL_OFFSET, (vs16)t[2]+Z_ACCEL_OFFSET, 
			(vs16)t[3] + X_GYRO_OFFSET, (vs16)t[4] + Y_GYRO_OFFSET, (vs16)t[5] + Z_GYRO_OFFSET);
	} else if (type == ZERO_PADDING2) {
		printf("%lf,%lf,%lf,%lf,%lf,%lf\r\n", 
				((vs16)t[0] + X_ACCEL_OFFSET) / 8192.0f,
				((vs16)t[1] + Y_ACCEL_OFFSET) / 8192.0f,
				((vs16)t[2] + Z_ACCEL_OFFSET) / 8192.0f,
				((vs16)t[3] + X_GYRO_OFFSET) / 16.384f,
				((vs16)t[4] + Y_GYRO_OFFSET) / 16.384f,
				((vs16)t[5] + Z_GYRO_OFFSET) / 16.384f);
	}
}

void display_data(void) {
	u16 t[6] = { 0 };
	float wx = 0, wy = 0, wz = 0;
	float halt_T = 20.0;
	float yaw = 0.0, pitch = 0.0, roll = 0.0;
	MPU6050_READ(t);
	wx = deg2rad((s16)t[3] / 16.384);
	wy = deg2rad((s16)t[4] / 16.384);
	wz = deg2rad((s16)t[5] / 16.384);

	Q[0] = Q[0] + (-wx * Q[1] - wy * Q[2] - wz * Q[3]) * 0.02;
	Q[1] = Q[1] + (wx * Q[0] + wz * Q[2] - wy * Q[3]) * 0.02;
	Q[2] = Q[2] + (wy * Q[0] - wz * Q[1] + wx * Q[3]) * 0.02;
	Q[3] = Q[3] + (wz * Q[0] + wy * Q[1] - wx * Q[2]) * 0.02;

	yaw = atan2(2 * Q[1] * Q[2] + 2 * Q[0] * Q[3], -2 * Q[2] * Q[2] - 2 * Q[3] * Q[3] + 1) * 57.3;
	pitch = asin(-2 * Q[1] * Q[3] + 2 * Q[0] * Q[2]) * 57.3;
	roll = atan2(2 * Q[2] * Q[3] + 2 * Q[0] * Q[1], -2 * Q[1] * Q[1] - 2 * Q[2] * Q[2] + 1) * 57.3;

	//zero_correct(t);
	//printf("ACCEL_X,Y,Z: %d, %d, %d\r\n", t[0], t[1], t[2]);
	//printf("GYRO_X,Y,Z: %d, %d, %d\r\n", t[3], t[4], t[5]);
	//printf("ACCEL_X,Y,Z: %lfg, %lfg, %lfg\r\n", t[0]/16384.0, t[1]/16384.0, t[2]/16384.0);
	//printf("GYRO_X,Y,Z: %lf,%lf,%lf\r\n", t[3]/16.384, t[4]/16.384, t[5]/16.384);
	//printf("q0,q1,q2,q3: %lf,%lf,%lf,%lf\r\n", Q[0], Q[1], Q[2], Q[3]);
	printf("yaw,pitch,roll: %lf, %lf, %lf\r\n", yaw, pitch, roll);
	//printf("GYRO_rad_X,Y,Z: %lf,%lf,%lf\r\n", wx, wy, wz);
	delay_ms(halt_T); //延时（决定刷新速度）
}

void IMU(void) {
	u16	t[6] = { 0 };
	MPU6050_READ(t);
}

void to_ground(void) {
	u16	t[6] = { 0 };
	MPU6050_READ(t);
	message(ZERO_PADDING, t);
	delay_ms(10);
}

void Accel_GetAngle(void) {
	u16	t[6] = { 0 };
    float roll = 0, pitch = 0;
	MPU6050_READ(t);
    zero_correct(t);
    roll = atan((float)t[1] / t[2]) * 57.2957;
    pitch = asin(-(s16)t[0] / 8192.0f) * 57.2957;
	printf("%.2f,%.2f\n", roll, pitch);
    //delay_ms(20);
}

static float roll_t = 0, pitch_t = 0;

void Gyro_GetAngle(void) {
    static bool inited = false;
    static float haltT = 0.001, wx = 0, wy = 0;
    int i = 0;
    u16	t[6] = { 0 };
    if (inited == false) {
        for (i = 0; i < 100; i++)
        {
			MPU6050_READ(t);
			zero_correct(t);
			roll_t = atan((float)t[1] / t[2]) * 57.2957;
			pitch_t = asin(-(s16)t[0] / 8192.0f) * 57.2957;
			//printf("%d\t%d\r\n", t[0], t[1]);
            //printf("%lf,%lf\r\n", roll_t, pitch_t);
            delay_ms(20);
        }
        roll_t = 0;
        pitch_t = 0;
        //printf("roll = %lf\tpitch = %lf\r\n", roll_t, pitch_t);
        inited = true;
        return;
    }
	MPU6050_READ(t);
	zero_correct(t);
    wx = (s16)t[3] / 16.384f;
    wy = (s16)t[4] / 16.384f;
    //printf("%lf, %lf\r\n", wx, wy);
    roll_t = roll_t + (wx * haltT) * 57.2957;
    pitch_t = pitch_t + (wy * haltT) * 57.2957;
    printf("%.2f,%.2f\n", roll_t, pitch_t);
    delay_ms(20);
}



void MPU6050_Get_Angle(MPU6050_Angle* data) {
	u16	t[6] = { 0 };
	MPU6050_READ(t);
	zero_correct(t);
	data->X_Angle = acos((s16)t[0] / 8192.0f);
	data->Y_Angle = acos((s16)t[1] / 8192.0f);
	data->Z_Angle = acos((s16)t[2] / 8192.0f);

	data->X_Angle = data->X_Angle * 57.29577;
	data->Y_Angle = data->Y_Angle * 57.29577;
	data->Z_Angle = data->Z_Angle * 57.29577;
}

void to_angle(void)
{
	MPU6050_Angle data;
	MPU6050_Get_Angle(&data);
	printf("X_Angle = %lf\t", data.X_Angle);
    printf("Y_Angle = %lf\t", data.Y_Angle);
    printf("Z_Angle = %lf", data.Z_Angle);
    printf("\r\n");
	delay_ms(20);
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

static  unsigned short inv_row_2_scale(const signed char* row)
{
	unsigned short b;

	if (row[0] > 0)
		b = 0;
	else if (row[0] < 0)
		b = 4;
	else if (row[1] > 0)
		b = 1;
	else if (row[1] < 0)
		b = 5;
	else if (row[2] > 0)
		b = 2;
	else if (row[2] < 0)
		b = 6;
	else
		b = 7;            // error
	return b;
}


static  unsigned short inv_orientation_matrix_to_scalar(
	const signed char* mtx)
{
	unsigned short scalar;
	scalar = inv_row_2_scale(mtx);
	scalar |= inv_row_2_scale(mtx + 3) << 3;
	scalar |= inv_row_2_scale(mtx + 6) << 6;

	return scalar;
}


///////////其他mpu和dmp的实现版本
static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result == 0x03) { //返回0x03为MPU6050
        /* Test passed. We can trust the gyro data here, so let's push it down to the DMP*/
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);   //读取当前陀螺仪的状态
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);    //根据读取的状态进行较准
    
        mpu_get_accel_sens(&accel_sens);    //读取当前加速度计的状态
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        //printf("setting bias succesfully ......\r\n");
    }
}


void MPU6050_Initialize(void)
{
    MPU6050_setClockSource(MPU6050_CLOCK_PLL_YGYRO);
	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//陀螺仪最大量程 +-2000度每秒
	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	//加速度度最大量程 +-2G
	MPU6050_setSleepEnabled(0); //进入工作状态
    MPU6050_setFIFOEnabled(1);
	MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
	MPU6050_setI2CBypassEnabled(0);	 //主控制器的I2C与	MPU6050的AUXI2C
}

void DMP_Init(void)
{
    u8 temp[1] = { 0 };
    IICReadBytes(MPU6050_ADD, MPU6050_RA_WHO_AM_I, 1, temp);

    if (temp[0] != 0x68)
    {
        NVIC_SystemReset();
    }
    if (!mpu_init())
    {
		if (!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))
		{
			//printf("mpu_set_sensor complete ......\r\n");
		}
		if (!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
		{
			//printf("mpu_configure_fifo complete ......\r\n");
		}
		if (!mpu_set_sample_rate(DEFAULT_MPU_HZ))
		{
			//printf("mpu_set_sample_rate complete ......\r\n");
		}
		if (!dmp_load_motion_driver_firmware())
		{
			//printf("dmp_load_motion_driver_firmware complete ......\r\n");
		}
		if (!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation)))
		{
			//printf("dmp_set_orientation complete ......\r\n");
		}
		if (!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
			DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
			DMP_FEATURE_GYRO_CAL))
		{
			//printf("dmp_enable_feature complete ......\r\n");
		}
		if (!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
		{
			//printf("dmp_set_fifo_rate complete ......\r\n");
		}
		run_self_test();
		if (!mpu_set_dmp_state(1))
		{
			//printf("mpu_set_dmp_state complete ......\r\n");
		}
    }
}

void Read_DMP(void)
{
    unsigned long sensor_timestamp;
    unsigned char more;
    long quat[4];
    int ret = 0;

    ret = dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more);
    printf("ret: %d\r\n", ret);
	if (sensors & INV_WXYZ_QUAT)
	{
        printf("start updating...\r\n");
		q0 = quat[0] / q30;
		q1 = quat[1] / q30;
		q2 = quat[2] / q30;
		q3 = quat[3] / q30;
		Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; 	
		Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3; // roll
		Yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;//yaw
	}
}

int Read_Temperature(void)
{
	float Temp;
	Temp = (I2C_ReadOneByte(devAddr, MPU6050_RA_TEMP_OUT_H) << 8) + I2C_ReadOneByte(devAddr, MPU6050_RA_TEMP_OUT_L);
	if (Temp > 32768) Temp -= 65536;
	Temp = (36.53 + Temp / 340) * 10;
	return (int)Temp;
}

void getAngle(float* yaw, float* yaw_acc_error)
{
	Read_DMP();                   //===读取Yaw(-180 - 179)

	if (Yaw < 0)
		Yaw = Yaw + 360;
	*yaw = Yaw;                   //===转换yaw(   0 - 359)

	*yaw = *yaw - *yaw_acc_error; //===减去yaw随时间的向上漂移

	if (*yaw < 0)
		*yaw = *yaw + 360;
}
//------------------End of File----------------------------
