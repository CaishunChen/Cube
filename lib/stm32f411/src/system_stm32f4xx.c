#include <stm32f411.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_flash.h>

#define PLL_M 8
#define PLL_N 200
#define PLL_P 4
#define PLL_Q 4

void SystemInit(void) {
    // 设置FPU
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));

    RCC->CR |= RCC_CR_HSION;
    // 重新配置、控制、中断寄存器
    RCC->CFGR = 0x00000000;
    RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_HSEBYP);
    RCC->PLLCFGR = 0x24003010;
    RCC->CIR = 0x00000000;

    // 打开外部高速时钟
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    /* Configure the main PLL */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (PLL_Q << 24);
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

    /* Enable the main PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till the main PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0) {}

    /* HCLK = SYSCLK / 1*/
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    /* PCLK2 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    /* PCLK1 = HCLK / 2*/
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN |FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS;

    /* Select the main PLL as system clock source */
    RCC->CFGR &= (uint32)((uint32)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;
}

