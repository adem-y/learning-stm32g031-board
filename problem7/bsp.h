#ifndef BSP_H_
#define BSP_H_

#include "stm32g0xx.h"

void SysTick_Handler(void);

void BSP_system_init();

void uart_tx(uint8_t);
uint8_t uart_rx(void);

void BSP_UART_init(uint32_t);
void printChar(uint8_t);
void _print(int, char * ,int);
void print(char *);


void delay_ms(uint32_t);
void delay(volatile unsigned int);

#endif
