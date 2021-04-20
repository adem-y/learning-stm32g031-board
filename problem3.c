/*
 * problem3.c
 * 
 * author: Adem Yildiz
 * In this problem an external interrupt routine is created.
 * Highest possible priority is given to that interrupt and interrupts are created by pressing button.
 * Inserted led is on when button is pressed.
 */

#include "stm32g0xx.h"

#define LEDDELAY    1600000

void delay(volatile uint32_t);

void EXTI2_3_IRQHandler(void){

    GPIOC->ODR ^= (1U << 6);
    for (volatile long long int a = 0; a < 1000000000000; ++a);
    GPIOC->BRR ^= (1U << 6);

    EXTI->RPR1 |= (1U << 3);
}

int main(void) {

    /* Enable GPIOC clock */
    RCC->IOPENR |= (1U << 2);

    /* Setup PC6 as output */
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    /* Turn on LED */
    GPIOC->ODR |= (1U << 6);

    /* Enable GPIOB clock */
    RCC->IOPENR |= (1U << 1);

    /* Set PB3 as input */
    GPIOB->MODER &= ~(3U << 2*3);

    EXTI->RTSR1 |= (1U << 3);
    EXTI->EXTICR[0] |= (1U << 8*3);
    EXTI->IMR1 |= (1U << 3);

    NVIC_SetPriority(EXTI2_3_IRQn, 0);
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    while(1) {
        
    }
    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}