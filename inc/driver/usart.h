#pragma once

#include <stm32f4xx_usart.h>

void usart1_init(uint32 baudrate);
uint8 usart1_send_bytes(const uint8 *buf, uint32 len);
