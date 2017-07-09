#pragma once

#include <motor.h>
#include <MPU6050.h>

struct cube_ctrl_bits {
    uint8 motor_ctrl : 1;   // ���Ƶ����������ȼ�
    uint8 dir_ctrl : 1;     // ����xyz, �����ȼ�
    uint8 pos_ctrl : 1;     // ������̬��λ��, ������ȼ�
    uint8 rsv : 5;          // ����������չ
};

union cube_ctrl {
    struct cube_ctrl_bits bits;
    uint8 all;
};

struct cube_motoren_bits {
    uint8 motor1 : 1;
    uint8 motor2 : 1;
    uint8 motor3 : 1;
    uint8 motor4 : 1;
    uint8 motor5 : 1;
    uint8 motor6 : 1;
    uint8 rsv : 2;
};

struct cube_diren_bits {
    uint8 x : 1;
    uint8 rsv0 : 1;
    uint8 y : 1;
    uint8 rsv1 : 1;
    uint8 z : 1;
    uint8 rsv2 : 3;
};

union cube_ens {
    struct cube_motoren_bits motor_en;
    struct cube_diren_bits dir_en;
    uint8 all;
};


#pragma pack(1)
struct cube {
    short gyro[3];  // 0
    short accel[3]; // 6
    long quat[4];   // 12

    float q[4];     // 28
    float m[9];     // 44 ��ת����

    float roll;     // 80
    float pitch;    // 84
    float yaw;      // 88

    float roll_rate;    // 92
    float pitch_rate;   // 96
    float yaw_rate;     // 100

    float xb_acc;       // 104 ��������ϵ���ٶ�
    float yb_acc;       // 108
    float zb_acc;       // 112

    float xn_acc;       // 116 ��������ϵ���ٶ�
    float yn_acc;       // 120
    float zn_acc;       // 124

    float xn_acc_bias;  // 128 ��������ϵ�¼��ٶȼ�������ƫ
    float yn_acc_bias;  // 132
    float zn_acc_bias;  // 136

    float vx;           // 140 �ٶ�
    float vy;           // 144
    float vz;           // 148

    float rx;           // 152 λ��
    float ry;           // 156
    float rz;           // 160

    // ��д�Ĵ���
    uint8 name[8];          // 164 ����
    union cube_ctrl ctrl;   // 172 ������
    union cube_ens ens;     // 173 ���ʹ��,����ʹ��
    float pwms[6];          // 174 ���ռ�ձ�,����ռ�ձ�
                            // 198
};
#pragma pack()

#define CUBE_WRITABLE_ADDR  164


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
