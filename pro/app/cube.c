#include <cube.h>
#include <gVariables.h>
#include <stdio.h>

struct CubeDirCtrl gXDir = {
    &motor1,
    &motor2,
    0.0
};
struct CubeDirCtrl gYDir = {
    &motor3,
    &motor4,
    0.0
};
struct CubeDirCtrl gZDir = {
    &motor5,
    &motor6,
    0.0
};
/*
 * cube_init - 初始化Cube
 */
void cube_init(void) {
    //imu_init(&gFlightParam);
    uint8 err = MPU6050_Init();
    if (err) {
        printf("初始化MPU6050错误: 0x%x", err);
        while (1);
    }

    motors_init();

    cube_disable_dir(&gXDir);
    cube_disable_dir(&gYDir);
    cube_disable_dir(&gZDir);
}
/*
 * cube_enable_dir - 使能运动方向
 *
 * @dir: 运动方向
 */
void cube_enable_dir(struct CubeDirCtrl *dir) {
    dir->en = TRUE;

    motor_set_pwm_duty(dir->fmotor, 0.5);
    motor_set_pwm_duty(dir->bmotor, 0.5);
    motor_enable(dir->fmotor);
    motor_enable(dir->bmotor);
}
/*
 * cube_disable_dir - 关闭运动方向
 *
 * @dir: 运动方向
 */
void cube_disable_dir(struct CubeDirCtrl *dir) {
    dir->en = FALSE;

    motor_set_pwm_duty(dir->fmotor, 0.0);
    motor_set_pwm_duty(dir->bmotor, 0.0);
    motor_disable(dir->fmotor);
    motor_disable(dir->bmotor);
}
/*
 * cube_set_dir_ctrl - 设置方向控制量
 *
 * @dir: 运动方向
 * @ctrl: 控制量
 */
void cube_set_dir_ctrl(struct CubeDirCtrl *dir, double ctrl) {
    ctrl = (ctrl < -1.0) ? -1.0 : ctrl;
    ctrl = (ctrl > 1.0) ? 1.0 : ctrl;

    dir->ctrl = ctrl;
    double tmp = 0.5 * (ctrl + 1.0);
    motor_set_pwm_duty(dir->fmotor, tmp);
    motor_set_pwm_duty(dir->bmotor, 1.0 - tmp);
}



