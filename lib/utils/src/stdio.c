
#include <stdio.h>
#include <rt_misc.h>

#include <stm32f411.h>

#pragma import(__use_no_semihosting_swi)

extern usart_regs_t *gUart;

int SendChar(int ch) {
    uart_send_byte(gUart, ch);
    return (ch);
}

int GetKey(void) {
    while (0 == gUart->SR.bits.RXNE);
    return ((int)gUart->DR.bits.byte);
}


struct __FILE {
    int handle; // Add whatever you need here
};

FILE __stdout;

FILE __stdin;

int fputc(int ch, FILE *f) {
    return SendChar(ch);
}

void _ttywrch(int ch) {
    SendChar(ch);
}

int ferror(FILE *f) { // Your implementation of ferror
    return EOF;
}

void _sys_exit(int return_code) {
label: goto label; // endless loop
}

