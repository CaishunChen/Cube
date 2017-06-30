#include <system.h>
#include <stm32f411.h>
#include <stm32f4xx_rcc.h>
#include <led.h>
#include <motor.h>
#include <fakei2c.h>
#include <mpu6050.h>

void config_interruts(void);

static void i2c_scl_h(void) {
    PBout(10) = 1;
}

static void i2c_scl_l(void) {
    PBout(10) = 0;
}

static void i2c_sda_h(void) {
    PBout(3) = 1;
}

static void i2c_sda_l(void) {
    PBout(3) = 0;
}

static bool i2c_sda(void) {
    return (1 == PBin(3)) ? TRUE : FALSE;
}

static void i2c_set_sda_out(void) {
    GPIOB->MODER.bits.pin3 = GPIO_Mode_Out;
}

static void i2c_set_sda_in(void) {
    GPIOB->MODER.bits.pin3 = GPIO_Mode_In;
}
/*
* PB10 -> SCL, PB3 -> SDA
*/
static void i2c_init(void) {
    RCC->AHB1ENR.bits.gpiob = 1;

    GPIOB->MODER.bits.pin3 = GPIO_Mode_Out;
    GPIOB->MODER.bits.pin10 = GPIO_Mode_Out;

    GPIOB->OTYPER.bits.pin3 = GPIO_OType_PP;
    GPIOB->OTYPER.bits.pin10 = GPIO_OType_PP;

    GPIOB->PUPDR.bits.pin3 = GPIO_Pull_Up;
    GPIOB->PUPDR.bits.pin10 = GPIO_Pull_Up;

    GPIOB->OSPEEDR.bits.pin3 = GPIO_OSpeed_High;
    GPIOB->OSPEEDR.bits.pin10 = GPIO_OSpeed_High;

    i2c_sda_h();
    i2c_scl_h();
}

static i2c_dev gI2C = {
    .scl_h = i2c_scl_h,
    .scl_l = i2c_scl_l,
    .sda_h = i2c_sda_h,
    .sda_l = i2c_sda_l,
    .sda = i2c_sda,
    .set_sda_out = i2c_set_sda_out,
    .set_sda_in = i2c_set_sda_in,
    .init = i2c_init,
    .delayus = 20
};

static struct mpu6050 gMpu6050 = {
    .addr = 0xD0,
    .i2c = &gI2C
};
uint8 data = 0;
int main(void) {
    sys_init();

    led_init();

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


