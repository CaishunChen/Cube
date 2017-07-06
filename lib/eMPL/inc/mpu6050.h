#pragma once
#include <types.h>
#include <fakei2c.h>

struct mpu6050 {
    uint8 addr;                 // 设备地址0xD0或者0xD2
    uint8 error;                // MPU6050状态
    i2c_dev *i2c;               // I2C端口
};

uint8 mpu6050_init(void);
void mpu6050_calibrate(void);
void mpu6050_pose(void);
uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);

