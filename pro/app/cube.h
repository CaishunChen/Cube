#pragma once

#include <motor.h>

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
