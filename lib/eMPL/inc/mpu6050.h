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

    float roll_rate;
    float pitch_rate;
    float yaw_rate;

    float xb_acc;    // ��������ϵ���ٶ�
    float yb_acc;
    float zb_acc;

    float xn_acc;    // ��������ϵ���ٶ�
    float yn_acc;
    float zn_acc;

    float xn_acc_bias;  // ��������ϵ�¼��ٶȼ�������ƫ
    float yn_acc_bias;
    float zn_acc_bias;

    float vx;           // �ٶ�
    float vy;
    float vz;

    float rx;           // λ��
    float ry;
    float rz;
};

uint8 mpu6050_init(void);
void mpu6050_calibrate(void);
void mpu6050_pose(void);
uint8 mpu6050_read_uint8(struct mpu6050 *mpu, uint8 reg);

