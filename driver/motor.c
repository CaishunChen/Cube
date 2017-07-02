#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <motor.h>

/* TODO: 以参数的形式存在 */
#define Motor_Pwm_Pres 500
#define Motor_Pwm_Period 100

struct MotorCtrl motor1 = {
    .pwm1 = (uint32*)&(TIM2->CCR1),
    .pwm2 = (uint32*)&(TIM2->CCR2),
    .en = (volatile unsigned long *)&Motor1_Switch
};
struct MotorCtrl motor2 = {
    .pwm1 = (uint32*)&(TIM9->CCR1),
    .pwm2 = (uint32*)&(TIM9->CCR2),
    .en = (volatile unsigned long *)&Motor2_Switch
};
struct MotorCtrl motor3 = {
    .pwm1 = (uint32*)&(TIM3->CCR3),
    .pwm2 = (uint32*)&(TIM3->CCR4),
    .en = (volatile unsigned long *)&Motor3_Switch
};
struct MotorCtrl motor4 = {
    .pwm1 = (uint32*)&(TIM3->CCR1),
    .pwm2 = (uint32*)&(TIM3->CCR2),
    .en = (volatile unsigned long *)&Motor4_Switch
};
struct MotorCtrl motor5 = {
    .pwm1 = (uint32*)&(TIM4->CCR1),
    .pwm2 = (uint32*)&(TIM4->CCR2),
    .en = (volatile unsigned long *)&Motor5_Switch
};
struct MotorCtrl motor6 = {
    .pwm1 = (uint32*)&(TIM4->CCR3),
    .pwm2 = (uint32*)&(TIM4->CCR4),
    .en = (volatile unsigned long *)&Motor6_Switch
};

static void motor_timpwm_init(timer_regs_t * tim, uint8 channel, uint16 pres, uint16 period) {
    tim->CR1.bits.DIR = TIM_COUNT_DIR_UP;
    tim->PSC = pres - 1;
    tim->ARR = period;
    tim->EGR.bits.UG = 1;
    tim->DIER.bits.UIE = 1;
    tim->CR1.bits.ARPE = 1;

    union timer_chanel_mode cfg;
    cfg.oc.OCxM = TIM_OCMode_PWM1;
    cfg.oc.OCxPE = 1;
    timer_set_ccmr(tim, channel, cfg);

    union timer_chanel_en cen;
    cen.bits.CCxE = 1;
    cen.bits.CCxNE = 0;
    cen.bits.CCxP = 0;
    cen.bits.CCxNP = 0;
    timer_set_ccer(tim, channel, cen);
}

static void motor_timers_init(void) {
    RCC->APB1ENR.bits.tim2 = 1;
    RCC->APB1ENR.bits.tim3 = 1;
    RCC->APB1ENR.bits.tim4 = 1;
    RCC->APB2ENR.bits.tim9 = 1;

    motor_timpwm_init(TIM2, 1, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM2, 2, Motor_Pwm_Pres, Motor_Pwm_Period);

    motor_timpwm_init(TIM3, 1, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM3, 2, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM3, 3, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM3, 4, Motor_Pwm_Pres, Motor_Pwm_Period);

    motor_timpwm_init(TIM4, 1, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM4, 2, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM4, 3, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM4, 4, Motor_Pwm_Pres, Motor_Pwm_Period);

    motor_timpwm_init(TIM9, 1, Motor_Pwm_Pres, Motor_Pwm_Period);
    motor_timpwm_init(TIM9, 2, Motor_Pwm_Pres, Motor_Pwm_Period);

    TIM2->CCR1 = 0; TIM2->CCR2 = 0;

    TIM3->CCR1 = 0; TIM3->CCR2 = 0;
    TIM3->CCR3 = 0; TIM3->CCR4 = 0;

    TIM4->CCR1 = 0; TIM4->CCR2 = 0;
    TIM4->CCR3 = 0; TIM4->CCR4 = 0;

    TIM9->CCR1 = 0; TIM9->CCR2 = 0;

    TIM2->CR1.bits.CEN = 1;
    TIM3->CR1.bits.CEN = 1;
    TIM4->CR1.bits.CEN = 1;
    TIM9->CR1.bits.CEN = 1;
}

static void motor_pwmios_init(void) {
    RCC->AHB1ENR.bits.gpioa = 1;
    RCC->AHB1ENR.bits.gpiob = 1;
    // 功能选择
    GPIOA->AFR.bits.pin0 = GPIO_AF_TIM2;
    GPIOA->AFR.bits.pin1 = GPIO_AF_TIM2;
    GPIOA->AFR.bits.pin2 = GPIO_AF_TIM9;
    GPIOA->AFR.bits.pin3 = GPIO_AF_TIM9;
    GPIOB->AFR.bits.pin0 = GPIO_AF_TIM3;
    GPIOB->AFR.bits.pin1 = GPIO_AF_TIM3;
    GPIOB->AFR.bits.pin4 = GPIO_AF_TIM3;
    GPIOB->AFR.bits.pin5 = GPIO_AF_TIM3;
    GPIOB->AFR.bits.pin6 = GPIO_AF_TIM4;
    GPIOB->AFR.bits.pin7 = GPIO_AF_TIM4;
    GPIOB->AFR.bits.pin8 = GPIO_AF_TIM4;
    GPIOB->AFR.bits.pin9 = GPIO_AF_TIM4;
    // 配置管脚
    struct gpio_pin_conf pincof;
    pincof.mode = GPIO_Mode_Af;
    pincof.otype = GPIO_OType_PP;
    pincof.pull = GPIO_Pull_Up;
    pincof.speed = GPIO_OSpeed_High;
    gpio_init(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3, &pincof);
    gpio_init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                     GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9, &pincof);
}

static void motor_gpios_init(void) {
    RCC->AHB1ENR.bits.gpioa = 1;
    RCC->AHB1ENR.bits.gpiob = 1;
    // 配置管脚
    struct gpio_pin_conf pincof;
    pincof.mode = GPIO_Mode_Out;
    pincof.otype = GPIO_OType_PP;
    pincof.pull = GPIO_Pull_Down;
    pincof.speed = GPIO_OSpeed_High;
    gpio_init(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14, &pincof);
    gpio_init(GPIOA, GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4, &pincof);

    Motor1_Switch = 0;
    Motor2_Switch = 0;
    Motor3_Switch = 0;
    Motor4_Switch = 0;
    Motor5_Switch = 0;
    Motor6_Switch = 0;
}
/*
 * motors_init - 初始化电机
 */
void motors_init(void) {
    motor_gpios_init();
    motor_pwmios_init();
    motor_timers_init();
}
/*
 * motors_enable - 开启电机
 *
 * @m: 目标电机
 */
void motor_enable(struct MotorCtrl *m) {
    m->en[0] = 1;
}
/*
 * motor_disable - 关闭电机
 *
 * @m: 目标电机
 */
void motor_disable(struct MotorCtrl *m) {
    m->en[0] = 0;
}
/*
 * motor_set_pwm_duty - 设置占空比
 *
 * @m: 目标电机
 * @duty: 目标占空比[-1,1]
 */
void motor_set_pwm_duty(struct MotorCtrl *m, double duty) {
    if (duty > 0) {
        duty = (duty > 1.0) ? 1.0 : duty;
        m->pwm1[0] = (uint32)(duty * Motor_Pwm_Period);
        m->pwm2[0] = 0;
    } else {
        duty = (duty < -1.0) ? 1.0 : -duty;
        m->pwm1[0] = 0;
        m->pwm2[0] = (uint32)(duty * Motor_Pwm_Period);
    }
}


