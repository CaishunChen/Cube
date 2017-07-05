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
double fdt = 0;
struct sys_time gTime1;
uint8 data = 0;

void ctrl_routine(void) {
    if (!isStarted)
        return;

    uint8 data = mpu6050_read_uint8(&gMpu6050, 0x3A);
    if (data & 0x01) {
        MPU6050_Pose();
    }
}


int main(void) {
    isStarted = FALSE;
    sys_init();
    led_init();
    usart1_init(115200);

    timer_init(1000, 5000);
    control_func = ctrl_routine;
    /*
    cube_init();
    cmd_init(&gU1RxQ, usart1_send_bytes);
    */
    LED_2 = LED_OFF;
    config_interruts();

    printf("=======%f==========\r\n", 12.4409);
    LED_2 = LED_ON;

    uint8 err = MPU6050_Init();
    if (err) {
        printf("≥ı ºªØMPU6050¥ÌŒÛ: 0x%x", err);
        while (1);
    }

    printf("=======%f==========\r\n", 0.031);
    isStarted = TRUE;
    while (1) {
        //data = mpu6050_read_uint8(&gMpu6050, 0x3A);
        //if (data & 0x01) {
        //    MPU6050_Pose();
        //}
    }
}


