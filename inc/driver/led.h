/***********************************************************
 *
 * led.h - ��ɫ������
 *
 ************************************** ���ҳ�.2017.0201 ***/
#ifndef CUBE_LED_H
#define CUBE_LED_H

#include <stm32f4xx_gpio.h>

#define LED_1 PBout(15)
#define LED_2 PAout(8)

#define LED_ON 0
#define LED_OFF 1

/*
 * led_init - ��ʼ��LED
 */
void led_init(void);

#endif
