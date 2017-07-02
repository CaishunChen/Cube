#pragma once

#include <motor.h>

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
