#pragma once

#include <stm32f4xx_i2c.h>

/* 发送/接收模式 */
#define I2C_DIRECTION_TX    ((uint8)0x00)
#define I2C_DIRECTION_RX    ((uint8)0x01)

/*
 * i2c_init - 初始化i2c端口
 *
 * @i2c: I2C端口
 */
void i2c2_init(void);
/*
 * i2c_read_byte - 读取一个字节
 *
 * @i2c: I2C端口
 * @addr: 从设备地址
 * @reg: 从设备寄存器
 */
uint8 i2c_read_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg);
/*
 * i2c_read_bytes - 读取多个字节
 *
 * @i2c: I2C端口
 * @addr: 从设备地址
 * @reg: 从设备寄存器
 * @len: 数据长度
 * @buf: 缓存
 */
void i2c_read_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, uint8 *buf);
/*
 * i2c_write_byte - 发送一个字节
 *
 * @i2c: I2C端口
 * @addr: 从设备地址
 * @reg: 从设备寄存器
 * @data: 数据
 */
void i2c_write_byte(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 data);
/*
* i2c_write_bytes - 发送多个字节
*
* @i2c: I2C端口
* @addr: 从设备地址
* @reg: 从设备寄存器
* @len: 数据长度
* @buf: 数据
*/
void i2c_write_bytes(i2c_regs_t *i2c, uint8 addr, uint8 reg, uint8 len, const uint8 *buf);

