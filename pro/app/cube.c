#include <cube.h>
#include <gVariables.h>
#include <stdio.h>


/*
 * cube_init - 初始化Cube
 */
void cube_init(void) {
    motors_init();
    for (int i = 0; i < 6; i++) {
        motor_set_pwm_duty(&gMotor[i], -0.1);
        motor_disable(&gMotor[i]);
    }

    
}
/*
 * cube_enable_dir - 使能运动方向
 *
 * @dir: 运动方向
 */
void cube_enable_dir(struct CubeDirCtrl *dir) {
    dir->en = TRUE;

    motor_set_pwm_duty(dir->fmotor, -0.1);
    motor_set_pwm_duty(dir->bmotor, -0.1);
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



