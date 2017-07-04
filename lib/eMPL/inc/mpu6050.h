#pragma once
#include <types.h>
#include <fakei2c.h>


extern float Pitch,Roll,Yaw;

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

    float roll;
    float pitch;
    float yaw;

    float roll_rate;
    float pitch_rate;
    float yaw_rate;

    float xaccb;    // ��������ϵ���ٶ�
    float yaccb;
    float zaccb;
};

bool i2c_write(uint8 addr, uint8 reg, uint8 len, uint8* data);
bool i2c_read(uint8 addr, uint8 reg, uint8 len, uint8 *buf);

uint8 MPU6050_Init(void);
void MPU6050_Pose(void);
uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);

