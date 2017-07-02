#include <system.h>
#include <led.h>
#include <cube.h>
#include <pose.h>

void config_interruts(void);

uint8 data = 0;
int main(void) {
    sys_init();

    led_init();
    cube_init();
    LED_2 = LED_OFF;

    config_interruts();

    sys_delay_ms(1000);
    while (MPU6050_ERROR_NONE != mpu6050_init(&gMpu6050));

    LED_2 = LED_ON;

    while (1) {
        data = mpu6050_read_uint8(&gMpu6050, 0x3A);
        if (data & 0x01) {
            mpu6050_update(&gMpu6050);
        }
        //xtos_delay_ms(1);
    }
}


