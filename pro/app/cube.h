#pragma once

#include <motor.h>
#include <MPU6050.h>

struct cube_ctrl_bits {
    uint8 motor_ctrl : 1;   // 控制电机，最低优先级
    uint8 dir_ctrl : 1;     // 控制xyz, 次优先级
    uint8 pos_ctrl : 1;     // 控制姿态和位置, 最高优先级
    uint8 rsv : 5;          // 保留，待扩展
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
    float m[9];     // 44 旋转矩阵

    float roll;     // 80
    float pitch;    // 84
    float yaw;      // 88

    float roll_rate;    // 92
    float pitch_rate;   // 96
    float yaw_rate;     // 100

    float xb_acc;       // 104 机体坐标系加速度
    float yb_acc;       // 108
    float zb_acc;       // 112

    float xn_acc;       // 116 世界坐标系加速度
    float yn_acc;       // 120
    float zn_acc;       // 124

    float xn_acc_bias;  // 128 世界坐标系下加速度计重力零偏
    float yn_acc_bias;  // 132
    float zn_acc_bias;  // 136

    float vx;           // 140 速度
    float vy;           // 144
    float vz;           // 148

    float rx;           // 152 位置
    float ry;           // 156
    float rz;           // 160

    // 可写寄存器
    uint8 name[8];          // 164 名称
    union cube_ctrl ctrl;   // 172 控制字
    union cube_ens ens;     // 173 电机使能,方向使能
    float pwms[6];          // 174 电机占空比,方向占空比
                            // 198
};
#pragma pack()

#define CUBE_WRITABLE_ADDR  164


struct CubeDirCtrl {
    struct MotorCtrl *fmotor;   // 控制向前运动的电机
    struct MotorCtrl *bmotor;   // 控制向后运动的电机
    bool en;                    // 使能
    double ctrl;                // 控制量[-1.0,1.0]
};

void cube_enable_dir(struct CubeDirCtrl *dir);
void cube_disable_dir(struct CubeDirCtrl *dir);
void cube_set_dir_ctrl(struct CubeDirCtrl *dir, double ctrl);
void cube_init(void);
