#include <timer.h>
#include <gVariables.h>
#include <stdio.h>

/*
* tim3_init - 初始化TIM3, 挂在慢速总线APB1上,驱动频率最高为42MHz
*
* @pres: 预分频系数
* @period: 计数周期
*/
void timer_init(uint16 pres, uint16 period) {
    RCC->APB2ENR.bits.tim10 = 1;
    gTimer->CR1.bits.DIR = TIM_COUNT_DIR_UP;
    gTimer->PSC = pres;
    gTimer->ARR = period;
    gTimer->EGR.bits.UG = 1;

    gTimer->DIER.bits.UIE = 1;
    gTimer->CR1.bits.CEN = 1;
}

void TIM1_UP_TIM10_IRQHandler(void) {
    if (1 == gTimer->SR.bits.UIF) {
        printf("T\r\n");
    }
    gTimer->SR.bits.UIF = 0;
}

