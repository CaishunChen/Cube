#include <gVariables.h>
#include <fakei2c.h>


timer_regs_t *gTimer = TIM10;
usart_regs_t *gUart = USART1;
Queue_T gU1RxQ;
uint8 gU1RxQ_Buf[CONFIG_USART_BUF_SIZE + 1];

struct sys_time gTime;;

bool gCtrolStarted = FALSE;

/***********************************************************/
/*                 i2c 函数和对象定义                       */
/* PB10 -> SCL, PB3 -> SDA                                 */
/***********************************************************/
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

i2c_dev gI2C = {
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

struct mpu6050 gMpu6050 = {
    .addr = 0xD0,
    .i2c = &gI2C
};

struct mpu6050_measure_value gImuValue;

