#pragma once
#include <types.h>
#include <fakei2c.h>


extern float Pitch,Roll,Yaw;

struct mpu6050 {
    uint8 addr;                 // 设备地址0xD0或者0xD2
    uint8 error;                // MPU6050状态
    i2c_dev *i2c;               // I2C端口
};

struct mpu6050_measure_value {
    short gyro[3];
    short accel[3];
    long quat[4];

    float q[4];

    float roll;
    float pitch;
    float yaw;

    float roll_rate;
    float pitch_rate;
    float yaw_rate;

    float xaccb;    // 机体坐标系加速度
    float yaccb;
    float zaccb;
};

bool i2c_write(uint8 addr, uint8 reg, uint8 len, uint8* data);
bool i2c_read(uint8 addr, uint8 reg, uint8 len, uint8 *buf);

uint8 MPU6050_Init(void);
void MPU6050_Pose(void);
uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);

