#pragma once

#include <stm32f4xx_timer.h>

extern void(*control_func)(void);

void timer_init(uint16 pres, uint16 period);

