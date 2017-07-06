#pragma once

#include <motor.h>
#include <MPU6050.h>

struct cube {
    uint8 name[8];  // ����

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


struct CubeDirCtrl {
    struct MotorCtrl *fmotor;   // ������ǰ�˶��ĵ��
    struct MotorCtrl *bmotor;   // ��������˶��ĵ��
    bool en;                    // ʹ��
    double ctrl;                // ������[-1.0,1.0]
};

void cube_enable_dir(struct CubeDirCtrl *dir);
void cube_disable_dir(struct CubeDirCtrl *dir);
void cube_set_dir_ctrl(struct CubeDirCtrl *dir, double ctrl);
void cube_init(void);
