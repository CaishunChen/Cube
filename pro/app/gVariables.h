#pragma once

#include <config.h>

#include <stm32f411.h>

#include <queue.h>


extern Queue_T gU1RxQ;
extern uint8 gU1RxQ_Buf[CONFIG_USART_BUF_SIZE + 1];

