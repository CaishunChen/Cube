#pragma once

#include <stm32f411.h>
#include <system.h>
#include <imu.h>
#include <config.h>
#include <queue.h>
#include <mpu6050.h>


extern usart_regs_t *gUart;
extern Queue_T gU1RxQ;
extern uint8 gU1RxQ_Buf[CONFIG_USART_BUF_SIZE + 1];

extern struct eular_angle gEularAngle;
extern struct flight_parameters gFlightParam;
extern struct sys_time gTime;

extern struct mpu6050 gMpu6050;
