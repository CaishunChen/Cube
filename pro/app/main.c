#include <system.h>
#include <gVariables.h>
#include <led.h>
#include <usart.h>
#include <command.h>
#include <cube.h>
#include <timer.h>
#include <mpu6050.h>

#include <stdio.h>

void config_interruts(void);

bool isStarted = FALSE;


float fdt = 0;
struct sys_time gTime1;
uint8 data = 0;


float gvx = 0;
float gvy = 0;

void ctrl_routine(void) {
    if (!isStarted)
        return;

    struct sys_time time;
    sys_get_time(&time);
    fdt = sys_time_diff_fms(&gTime1, &time);
    gTime1.ms = time.ms;
    gTime1.us = time.us;

    gvx = (gImuValue.xn_acc - gImuValue.xn_acc_bias) * fdt * 0.001f;
    gvy = (gImuValue.yn_acc - gImuValue.yn_acc_bias) * fdt * 0.001f;

    if (!gCtrolStarted) {
        return;
    }

    gImuValue.rx += gImuValue.vx * fdt * 0.001f;
    gImuValue.ry += gImuValue.vy * fdt * 0.001f;

    gImuValue.vx += gvx;
    gImuValue.vy += gvy;
}


int main(void) {
    isStarted = FALSE;
    sys_init();
    led_init();
    usart1_init(115200);

    timer_init(1000, 5000);
    control_func = ctrl_routine;
    sys_get_time(&gTime1);

    /*
    motors_init();
    for (int i = 0; i < 6; i++) {
        motor_set_pwm_duty(&gMotor[i], -0.1);
        motor_enable(&gMotor[i]);
    }

    cube_init();
    */
    cmd_init(&gU1RxQ, usart1_send_bytes);

    LED_2 = LED_OFF;
    config_interruts();

    //sys_delay_ms(10000);

    printf("初始化MPU6050\r\n");
    LED_2 = LED_ON;

    uint8 err = mpu6050_init();
    if (err) {
        printf("初始化MPU6050错误: 0x%x\r\n", err);
        while (1);
    }

    printf("初始化MPU6050结束\r\n");
    isStarted = TRUE;

    while (1) {
        data = mpu6050_read_uint8(&gMpu6050, 0x3A);
        if (data & 0x01) {
            mpu6050_pose();
        }

    }
}


