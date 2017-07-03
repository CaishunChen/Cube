#include <system.h>
#include <gVariables.h>
#include <led.h>
#include <usart.h>
#include <command.h>
#include <cube.h>

#include <stdio.h>

void config_interruts(void);

uint8 data = 0;
double fdt = 0;
struct sys_time gTime1;

int main(void) {
    sys_init();

    led_init();
    usart1_init(115200);

    cube_init();
    cmd_init(&gU1RxQ, usart1_send_bytes);
    LED_2 = LED_OFF;

    config_interruts();

    printf("===================\r\n");

    sys_delay_ms(1000);
    sys_get_time(&gTime);
    LED_2 = LED_ON;

    while (1) {
        data = mpu6050_read_uint8(&gMpu6050, 0x3A);
        if (data & 0x01) {
            fdt = sys_time_dis_fms(&gTime);
            sys_get_time(&gTime);

            mpu6050_update(&gMpu6050);
            imu_update_quat(&(gMpu6050.fvalue), &gFlightParam, 0.001 * fdt);
            imu_update_xyz(&(gMpu6050.fvalue), &gFlightParam, 0.001 * fdt);
            imu_get_euler_angle(&gFlightParam, &gEularAngle);
        }

        Parse_Command();
        Exec_Command();
        Clear_Command();
        //sys_delay_ms(1);
    }
}


