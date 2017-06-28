#include <system.h>
#include <stm32f411.h>
#include <stm32f4xx_rcc.h>
#include <led.h>
#include <motor.h>

void config_interruts(void);


int main(void) {
    sys_init();

    led_init();

    config_interruts();


    while (1) {
        LED_1 = LED_ON;
        sys_delay_ms(100);
        LED_1 = LED_OFF;
        sys_delay_ms(1000);
    }
}


