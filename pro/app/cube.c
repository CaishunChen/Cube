#include <cube.h>

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
 * cube_init - ��ʼ��Cube
 */
void cube_init(void) {
    motors_init();

    cube_disable_dir(&gXDir);
    cube_disable_dir(&gYDir);
    cube_disable_dir(&gZDir);
}
/*
 * cube_enable_dir - ʹ���˶�����
 *
 * @dir: �˶�����
 */
void cube_enable_dir(struct CubeDirCtrl *dir) {
    dir->en = TRUE;

    motor_set_pwm_duty(dir->fmotor, 0.5);
    motor_set_pwm_duty(dir->bmotor, 0.5);
    motor_enable(dir->fmotor);
    motor_enable(dir->bmotor);
}
/*
 * cube_disable_dir - �ر��˶�����
 *
 * @dir: �˶�����
 */
void cube_disable_dir(struct CubeDirCtrl *dir) {
    dir->en = FALSE;

    motor_set_pwm_duty(dir->fmotor, 0.0);
    motor_set_pwm_duty(dir->bmotor, 0.0);
    motor_disable(dir->fmotor);
    motor_disable(dir->bmotor);
}
/*
 * cube_set_dir_ctrl - ���÷��������
 *
 * @dir: �˶�����
 * @ctrl: ������
 */
void cube_set_dir_ctrl(struct CubeDirCtrl *dir, double ctrl) {
    ctrl = (ctrl < -1.0) ? -1.0 : ctrl;
    ctrl = (ctrl > 1.0) ? 1.0 : ctrl;

    dir->ctrl = ctrl;
    double tmp = 0.5 * (ctrl + 1.0);
    motor_set_pwm_duty(dir->fmotor, tmp);
    motor_set_pwm_duty(dir->bmotor, 1.0 - tmp);
}



