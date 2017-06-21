/***********************************************************
 *
 * led.c - 三色灯驱动
 *
 ************************************** 高乙超.2017.0201 ***/
#include <stm32f411.h>
#include <stm32f4xx_rcc.h>
#include <led.h>
/*
 * led_init - 初始化三色灯
 */
void led_init(void) {
    RCC->AHB1ENR.bits.gpioa = 1;
    RCC->AHB1ENR.bits.gpiob = 1;

    GPIOA->MODER.bits.pin8 = GPIO_Mode_Out;
    GPIOB->MODER.bits.pin15 = GPIO_Mode_Out;

    GPIOA->OTYPER.bits.pin8 = GPIO_OType_PP;
    GPIOB->OTYPER.bits.pin15 = GPIO_OType_PP;

    GPIOA->PUPDR.bits.pin8 = GPIO_Pull_Up;
    GPIOB->PUPDR.bits.pin15 = GPIO_Pull_Up;

    GPIOA->OSPEEDR.bits.pin8 = GPIO_OSpeed_Very_High;
    GPIOB->OSPEEDR.bits.pin15 = GPIO_OSpeed_Very_High;
}

