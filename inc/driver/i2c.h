#pragma once

#include <stm32f4xx_i2c.h>

/* ����/����ģʽ */
#define I2C_DIRECTION_TX    ((uint8)0x00)
#define I2C_DIRECTION_RX    ((uint8)0x01)

/*
 * i2c_init - ��ʼ��i2c�˿�
 *
 * @i2c: I2C�˿�
 */
void i2c2_init(void);
/*
 * i2c_read_byte - ��ȡһ���ֽ�
 *
 * @i2c: I2C�˿�
 * @addr: ���豸��ַ
 * @reg: ���豸�Ĵ���
 */
uint8 i2c_read_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg);
/*
 * i2c_read_bytes - ��ȡ����ֽ�
 *
 * @i2c: I2C�˿�
 * @addr: ���豸��ַ
 * @reg: ���豸�Ĵ���
 * @len: ���ݳ���
 * @buf: ����
 */
void i2c_read_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, uint8 *buf);
/*
 * i2c_write_byte - ����һ���ֽ�
 *
 * @i2c: I2C�˿�
 * @addr: ���豸��ַ
 * @reg: ���豸�Ĵ���
 * @data: ����
 */
void i2c_write_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 data);
/*
* i2c_write_bytes - ���Ͷ���ֽ�
*
* @i2c: I2C�˿�
* @addr: ���豸��ַ
* @reg: ���豸�Ĵ���
* @len: ���ݳ���
* @buf: ����
*/
void i2c_write_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, const uint8 *buf);

