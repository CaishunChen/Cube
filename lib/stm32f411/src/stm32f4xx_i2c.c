#include <stm32f4xx_i2c.h>

/*
 * i2c_check_status - 检查I2C总线状态
 *
 * @i2c: 目标总线
 * @sr: 待测状态, sr[31:16] <==> SR2, sr[15:0] <==> SR1
 */
bool i2c_check_status(i2c_regs_t *i2c, uint32 sr) {
    union Data32 tmp;
    tmp.half_word[0] = i2c->SR1.all;
    tmp.half_word[1] = i2c->SR2.all;
    return (sr & tmp.word) ? TRUE : FALSE;
}

