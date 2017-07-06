#pragma once

#include <motor.h>
#include <MPU6050.h>

struct cube {
    uint8 name[8];  // 名称

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
