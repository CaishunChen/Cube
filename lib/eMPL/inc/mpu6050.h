#pragma once
#include <types.h>
#include <fakei2c.h>

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
    float m[9];     // 旋转矩阵

    float roll;
    float pitch;
    float yaw;

    float roll_rate;
    float pitch_rate;
    float yaw_rate;

    float xb_acc;    // 机体坐标系加速度
    float yb_acc;
    float zb_acc;

    float xn_acc;    // 世界坐标系加速度
    float yn_acc;
    float zn_acc;

    float xn_acc_bias;  // 世界坐标系下加速度计重力零偏
    float yn_acc_bias;
    float zn_acc_bias;

    float vx;           // 速度
    float vy;
    float vz;

    float rx;           // 位置
    float ry;
    float rz;
};

uint8 mpu6050_init(void);
void mpu6050_calibrate(void);
void mpu6050_pose(void);
uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);

