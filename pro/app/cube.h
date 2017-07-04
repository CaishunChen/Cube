#pragma once

#include <motor.h>
#include <MPU6050.h>


struct cube {
    uint8 name[8];

    double ctrl_x;                          // x,y,z方向目标位置
    double ctrl_y;
    double ctrl_z;
    // 只读字段
    //struct sys_time gTime;                  // 运行时间
    //struct flight_parameters gFlightParam;  // 飞行参数
    //struct eular_angle gEularAngle;         // 欧拉角

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
