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
    // ����ѡ��
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
 * i2c2_init - ��ʼ��i2c�˿�
 *
 * @i2c: I2C�˿�
 */
void i2c2_init() {
    i2c2_init_gpio();

    I2C2->CR1.bits.PE = 0;
    I2C2->CR1.bits.SMBUS = 0;
    // i2c����Ƶ��50MHz
    I2C2->CR2.bits.FREQ = 50;
    // 100KHz�ı�׼ͨ��, 250 / 50 * 10^-6
    I2C2->CCR.bits.F_S = 0;
    I2C2->CCR.bits.CCR = 250;
    I2C2->TRISE.bits.TRISE = 51;
    // ����
    I2C2->CR1.bits.PE = 1;
    I2C2->CR1.bits.ACK = 0;
}
/*
 * i2c_read_byte - ��ȡһ���ֽ�
 *
 * @i2c: I2C�˿�
 * @addr: ���豸��ַ
 * @reg: ���豸�Ĵ���
 */
uint8 i2c_read_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg) {
    // ���߿���
    while (1 == i2c->SR2.bits.BUSY);
    // ����START,����Masterģʽ
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // ����7λ��ַ, ����ģʽ
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // ���ͼĴ�����ַ
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // ���²�����ʼλ, ����Master Receiverģʽ
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    i2c->DR = (addr << 1) | I2C_DIRECTION_RX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR));
    // ֻ��һ���ֽ�,���Է���NACK
    i2c->CR1.bits.ACK = 0;
    i2c->CR1.bits.STOP = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_RXNE));
    uint8 data = i2c->DR;

    i2c->CR1.bits.ACK = 1;
    return data;
}
/*
 * i2c_read_bytes - ��ȡ����ֽ�
 *
 * @i2c: I2C�˿�
 * @addr: ���豸��ַ
 * @reg: ���豸�Ĵ���
 * @len: ���ݳ���
 * @buf: ����
 */
void i2c_read_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, uint8 *buf) {
    // ���߿���
    while (1 == i2c->SR2.bits.BUSY);
    // ����START,����Masterģʽ
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // ����7λ��ַ, ����ģʽ
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // ���ͼĴ�����ַ
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // ���²�����ʼλ, ����Master Receiverģʽ
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    i2c->DR = (addr << 1) | I2C_DIRECTION_RX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR));
    // ���ν����ֽ�,ÿ�ν��ն���Ҫ����һ��ACK
    i2c->CR1.bits.ACK = 1;
    while (len > 1) {
        while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_RXNE));
        buf[0] = i2c->DR;
        buf++; len--;
    }
    // �����һ���ֽ�,���Է���NACK
    i2c->CR1.bits.ACK = 0;
    i2c->CR1.bits.STOP = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_RXNE));
    buf[0] = i2c->DR;

    i2c->CR1.bits.ACK = 1;
}
/*
 * i2c_write_byte - ����һ���ֽ�
 *
 * @i2c: I2C�˿�
 * @addr: ���豸��ַ
 * @reg: ���豸�Ĵ���
 * @data: ����
 */
void i2c_write_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 data) {
    // ���߿���
    while (1 == i2c->SR2.bits.BUSY);
    // ����START,����Masterģʽ
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // ����7λ��ַ, ����ģʽ
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // ���ͼĴ�����ַ
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // ��������
    i2c->DR = data;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    
    i2c->CR1.bits.STOP = 1;
}
/*
 * i2c_write_bytes - ���Ͷ���ֽ�
 *
 * @i2c: I2C�˿�
 * @addr: ���豸��ַ
 * @reg: ���豸�Ĵ���
 * @len: ���ݳ���
 * @buf: ����
 */
void i2c_write_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, const uint8 *buf) {
    // ���߿���
    while (1 == i2c->SR2.bits.BUSY);
    // ����START,����Masterģʽ
    i2c->CR1.bits.START = 1;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_SB));
    // ����7λ��ַ, ����ģʽ
    i2c->DR = (addr << 1) | I2C_DIRECTION_TX;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_ADDR_TXE_TRA));
    // ���ͼĴ�����ַ
    i2c->DR = reg;
    while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
    // ��������
    while (len--) {
        i2c->DR = buf[0];
        while (TRUE != i2c_check_status(i2c, I2C_STA_BUSY_MSL_TXE_TRA_BTF));
        buf++;
    }
    
    i2c->CR1.bits.STOP = 1;
}
