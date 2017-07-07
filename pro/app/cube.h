#pragma once

#include <motor.h>
#include <MPU6050.h>

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
    uint8 name[8];      // 164 名称
                        // 172
};

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
