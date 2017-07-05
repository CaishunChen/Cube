#pragma once
#include <types.h>
#include <fakei2c.h>

struct mpu6050 {
    uint8 addr;                 // �豸��ַ0xD0����0xD2
    uint8 error;                // MPU6050״̬
    i2c_dev *i2c;               // I2C�˿�
};

struct mpu6050_measure_value {
    short gyro[3];
    short accel[3];
    long quat[4];

    float q[4];
    float m[9];     // ��ת����

    float roll;
    float pitch;
    float yaw;

    float roll2;
    float pitch2;
    float yaw2;

    float roll_rate;
    float pitch_rate;
    float yaw_rate;

    float xaccb;    // ��������ϵ���ٶ�
    float yaccb;
    float zaccb;
};

uint8 MPU6050_Init(void);
void MPU6050_Pose(void);
uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);

