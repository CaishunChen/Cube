#include <stm32f411.h>
#include <gVariables.h>
#include <usart.h>
#include <queue.h>
#include <stdio.h>
#include <cube.h>
/*
 * usart1_init - 初始化串口1
 *
 * 1停止位,无校验,PA9->TX, PA10->RX
 *
 * @baudrate: 波特率
 */
void usart1_init(uint32 baudrate) {
    RCC->AHB1ENR.bits.gpioa = 1;
    RCC->APB2ENR.bits.usart1 = 1;// 串口时钟使能
    
    // 功能选择
    GPIOA->AFR.bits.pin9 = GPIO_AF_USART1;
    GPIOA->AFR.bits.pin10 = GPIO_AF_USART1;
    // PA9 : tx
    GPIOA->MODER.bits.pin9 = GPIO_Mode_Af;
    GPIOA->OTYPER.bits.pin9 = GPIO_OType_PP;
    GPIOA->PUPDR.bits.pin9 = GPIO_Pull_Up;
    GPIOA->OSPEEDR.bits.pin9 = GPIO_OSpeed_High;
    // PA10 : rx
    GPIOA->MODER.bits.pin10 = GPIO_Mode_Af;
    GPIOA->OTYPER.bits.pin10 = GPIO_OType_OD;
    GPIOA->PUPDR.bits.pin10 = GPIO_Pull_No;

    uart_init(USART1, baudrate);

    init_queue(&gU1RxQ, gU1RxQ_Buf, CONFIG_USART_BUF_SIZE);
}

uint8 usart1_send_bytes(const uint8 *buf, uint32 len) {
    uart_send_bytes(USART1, buf, len);
    return 0;
}


void USART1_IRQHandler(void) {
    if (0 != USART1->SR.bits.RXNE) {
        enqueue(&gU1RxQ, USART1->DR.bits.byte);
    }
}
