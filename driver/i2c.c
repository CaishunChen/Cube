#include <i2c.h>
#include <stm32f411.h>

void i2c2_init_gpio(void) {
    RCC->AHB1ENR.bits.gpiob = 1;
    RCC->APB1ENR.bits.i2c2 = 1;
    I2C2->CR1.bits.SWRST = 1;
    I2C2->CR1.bits.SWRST = 0;

    GPIOB->MODER.bits.pin3 = GPIO_Mode_Out;
    GPIOB->OTYPER.bits.pin3 = GPIO_OType_PP;
    GPIOB->PUPDR.bits.pin3 = GPIO_Pull_Up;
    GPIOB->OSPEEDR.bits.pin3 = GPIO_OSpeed_High;

    GPIOB->MODER.bits.pin10 = GPIO_Mode_Out;
    GPIOB->OTYPER.bits.pin10 = GPIO_OType_PP;
    GPIOB->PUPDR.bits.pin10 = GPIO_Pull_Up;
    GPIOB->OSPEEDR.bits.pin10 = GPIO_OSpeed_High;

    PBout(3) = 1;
    PBout(10) = 1;
    // 功能选择
    GPIOB->AFR.bits.pin3 = GPIO_AF_I2C2;
    GPIOB->AFR.bits.pin10 = GPIO_AF_I2C2;
    //
    GPIOB->MODER.bits.pin3 = GPIO_Mode_Af;
    GPIOB->OTYPER.bits.pin3 = GPIO_OType_OD;
    GPIOB->PUPDR.bits.pin3 = GPIO_Pull_No;
    //
    GPIOB->MODER.bits.pin10 = GPIO_Mode_Af;
    GPIOB->OTYPER.bits.pin10 = GPIO_OType_OD;
    GPIOB->PUPDR.bits.pin10 = GPIO_Pull_No;
}

/*
 * i2c2_init - 初始化i2c端口
 *
 * @i2c: I2C端口
 */
void i2c2_init() {
    i2c2_init_gpio();

    I2C2->CR1.bits.PE = 0;
    I2C2->CR1.bits.SMBUS = 0;
    // i2c工作频率50MHz
    I2C2->CR2.bits.FREQ = 50;
    // 100KHz的标准通信, 250 / 50 * 10^-6
    I2C2->CCR.bits.F_S = 0;
    I2C2->CCR.bits.CCR = 250;
    I2C2->TRISE.bits.TRISE = 51;
    // 开启
    I2C2->CR1.bits.PE = 1;
    I2C2->CR1.bits.ACK = 0;
}
/*
 * i2c_read_byte - 读取一个字节
 *
 * @i2c: I2C端口
 * @addr: 从设备地址
 * @reg: 从设备寄存器
 */
uint8 i2c_read_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg) {
    // 总线空闲
    while (1 == i2c->SR2.bits.BUSY);
    // 产生START,进入Master模式
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // 发送7位地址, 发送模式
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // 发送寄存器地址
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // 重新产生起始位, 进入Master Receiver模式
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    i2c->DR = (addr << 1) | I2C_DIRECTION_RX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR));
    // 只读一个字节,所以返回NACK
    i2c->CR1.bits.ACK = 0;
    i2c->CR1.bits.STOP = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_RXNE));
    uint8 data = i2c->DR;

    i2c->CR1.bits.ACK = 1;
    return data;
}
/*
 * i2c_read_bytes - 读取多个字节
 *
 * @i2c: I2C端口
 * @addr: 从设备地址
 * @reg: 从设备寄存器
 * @len: 数据长度
 * @buf: 缓存
 */
void i2c_read_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, uint8 *buf) {
    // 总线空闲
    while (1 == i2c->SR2.bits.BUSY);
    // 产生START,进入Master模式
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // 发送7位地址, 发送模式
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // 发送寄存器地址
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // 重新产生起始位, 进入Master Receiver模式
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    i2c->DR = (addr << 1) | I2C_DIRECTION_RX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR));
    // 依次接收字节,每次接收都需要返回一个ACK
    i2c->CR1.bits.ACK = 1;
    while (len > 1) {
        while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_RXNE));
        buf[0] = i2c->DR;
        buf++; len--;
    }
    // 读最后一个字节,所以返回NACK
    i2c->CR1.bits.ACK = 0;
    i2c->CR1.bits.STOP = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_RXNE));
    buf[0] = i2c->DR;

    i2c->CR1.bits.ACK = 1;
}
/*
 * i2c_write_byte - 发送一个字节
 *
 * @i2c: I2C端口
 * @addr: 从设备地址
 * @reg: 从设备寄存器
 * @data: 数据
 */
void i2c_write_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 data) {
    // 总线空闲
    while (1 == i2c->SR2.bits.BUSY);
    // 产生START,进入Master模式
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // 发送7位地址, 发送模式
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // 发送寄存器地址
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // 发送数据
    i2c->DR = data;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    
    i2c->CR1.bits.STOP = 1;
}
/*
 * i2c_write_bytes - 发送多个字节
 *
 * @i2c: I2C端口
 * @addr: 从设备地址
 * @reg: 从设备寄存器
 * @len: 数据长度
 * @buf: 数据
 */
void i2c_write_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, const uint8 *buf) {
    // 总线空闲
    while (1 == i2c->SR2.bits.BUSY);
    // 产生START,进入Master模式
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // 发送7位地址, 发送模式
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // 发送寄存器地址
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // 发送数据
    while (len--) {
        i2c->DR = buf[0];
        while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
        buf++;
    }
    
    i2c->CR1.bits.STOP = 1;
}
