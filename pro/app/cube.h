#pragma once

#include <motor.h>
#include <MPU6050.h>


struct cube {
    uint8 name[8];

    double ctrl_x;                          // x,y,z����Ŀ��λ��
    double ctrl_y;
    double ctrl_z;
    // ֻ���ֶ�
    //struct sys_time gTime;                  // ����ʱ��
    //struct flight_parameters gFlightParam;  // ���в���
    //struct eular_angle gEularAngle;         // ŷ����

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
