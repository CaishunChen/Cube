#include <stm32f411.h>
#include <stm32f4xx_rcc.h>
#include <led.h>
#include <motor.h>

void config_interruts(void);


#define DelayTime 5000

void delay(uint16 n) {
    for (uint16 i = 0; i < n; i++)
        for (uint16 j = 0; j < DelayTime; j++)
            ;
}

int main(void)
{
    led_init();
    motors_init();

    LED_1 = LED_ON;
    LED_2 = LED_ON;

    //config_interruts();

    motor_set_pwm_duty(&motor1, 0.1);
    motor_set_pwm_duty(&motor2, -0.1);
    motor_set_pwm_duty(&motor3, -0.1);
    motor_set_pwm_duty(&motor4, 0.1);
    motor_set_pwm_duty(&motor5, 0.1);
    motor_set_pwm_duty(&motor6, 0.1);

    motor_enable(&motor1);
    delay(DelayTime);
    motor_disable(&motor1);

    motor_enable(&motor2);
    delay(DelayTime);
    motor_disable(&motor2);

    motor_enable(&motor3);
    delay(DelayTime);
    motor_disable(&motor3);

    motor_enable(&motor4);
    delay(DelayTime);
    motor_disable(&motor4);

    motor_enable(&motor5);
    delay(DelayTime);
    motor_disable(&motor5);

    motor_enable(&motor6);
    delay(DelayTime);
    motor_disable(&motor6);

    delay(DelayTime);
    delay(DelayTime);
    motor_set_pwm_duty(&motor4, 0.1);
    motor_enable(&motor4);
    delay(DelayTime);
    delay(DelayTime);
    motor_set_pwm_duty(&motor4, -0.1);
    delay(DelayTime);
    delay(DelayTime);
    motor_disable(&motor4);


    while (1) {

    }
}


