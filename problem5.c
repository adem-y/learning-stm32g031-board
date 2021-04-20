/*
 * problem5.c
 *
 * author: Adem Yildiz
 * This problem aims to create an accurate delay function using SysTick exception.
 * Systick is a counter to create time delays and periodic interrupts.
 */

#include "stm32g0xx.h"

#define SEC 5000000

void delay_ms(uint32_t);

int main(void) {

   SystemCoreClockUpdate();

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);

    while(1) {

    	GPIOC->ODR |= (1U << 6);
    	delay_ms(SEC);

    	GPIOC->ODR &= ~(1U << 6);
    	delay_ms(SEC);
    }

    return 0;
}

void delay_ms(uint32_t second){
    SysTick_Config(SystemCoreClock / second);

}
