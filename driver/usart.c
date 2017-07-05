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

void _enable_motor(uint8 id) {
    for (int i = 0; i < 6; i++) {
        motor_disable(&gMotor[i]);
    }
    motor_enable(&gMotor[id]);
}



extern float fdt;
extern float gvx;
extern float gvy;
void USART1_IRQHandler(void) {
    if (0 != USART1->SR.bits.RXNE) {
        //enqueue(&gU1RxQ, USART1->DR.bits.byte);
        uint8 data = USART1->DR.bits.byte;
        uart_send_byte(USART1, data);

        if ('b' == data) {
            gImuValue.xn_acc_bias = gImuValue.xn_acc;
            gImuValue.yn_acc_bias = gImuValue.yn_acc;
            gImuValue.zn_acc_bias = gImuValue.zn_acc;

            gCtrolStarted = TRUE;
            printf("fdt = %f\tgvx = %f\tgvy = %f\r\n", fdt, gImuValue.vx, gImuValue.vy);
            //printf("fdt = %f\tgrx = %f\tgry = %f\r\n", fdt, gImuValue.rx, gImuValue.ry);
        }

        if ('a' == data) {
            printf("fdt = %f\tvx = %f\tvy = %f\r\n", fdt, gvx, gvy);
        }

        if ('c' == data) {
            gImuValue.vx = 0.0f;
            gImuValue.vy = 0.0f;
        }

        if (('1' <= data) && (data <= '6'))
            _enable_motor(data - '1');
        if ('0' == data) {
            for (int i = 0; i < 6; i++) {
                motor_disable(&gMotor[i]);
            }
        }

    }
}
