#ifndef CUBE_MOTOR_H
#define CUBE_MOTOR_H

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_timer.h>

#define Motor1_Switch PAout(6)
#define Motor2_Switch PAout(5)
#define Motor3_Switch PAout(4)
#define Motor4_Switch PBout(13)
#define Motor5_Switch PBout(14)
#define Motor6_Switch PBout(12)

struct MotorCtrl {
    uint32 *pwm1;
    uint32 *pwm2;
    volatile unsigned long *en;
};

/*
 * motors_init - 初始化电机
 */
void motors_init(void);
/*
 * motors_enable - 开启电机
 *
 * @m: 目标电机
 */
void motor_enable(struct MotorCtrl *m);
/*
 * motor_disable - 关闭电机
 *
 * @m: 目标电机
 */
void motor_disable(struct MotorCtrl *m);
/*
 * motor_set_pwm_duty - 设置占空比
 *
 * @m: 目标电机
 * @duty: 目标占空比[-1,1]
 */
void motor_set_pwm_duty(struct MotorCtrl *m, double duty);

#endif
