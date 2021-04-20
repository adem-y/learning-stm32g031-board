/*
 * problem4.c
 *
 * author: Adem Yildiz
 * In this problem two buttons are connected
 * and for both of them external interrupts are enabled.
 * Two leds are also connected for each button.
 */

#include "stm32g0xx.h"
//									PB1->button1 (for blue)
//									PB2->button2 (for yellow)
//									PA0->blue led
//									PA1->yellow led
#define LEDDELAY    16000

void delay(volatile uint32_t);

void EXTI2_3_IRQHandler(void){

   GPIOA->ODR ^= (1U << 1); // for yellow led
   for (volatile long long int a = 0; a < 5000000; ++a);
   GPIOA->BRR ^= (1U << 1);
   EXTI->RPR1 |= (1U << 2); //control pending: 2 ? 3
}

void EXTI0_1_IRQHandler(void){

	GPIOA->ODR ^= (1U << 0); //for blue led
    for (volatile long long int a = 0; a < 10000000; ++a);
    GPIOA->BRR ^= (1U << 0);

    EXTI->RPR1 |= (1U << 1); //control pending: 0 ? 1
}

int main(void) {

    /* Enable GPIOA and GPIOB clock */
    RCC->IOPENR |= (1U << 1); //for PBs
    RCC->IOPENR |= (1U << 0); //for PAs
    
    /* for PA0 */
    GPIOA->MODER &= ~(3U << 2*0);
    GPIOA->MODER |= (1U << 2*0);

    /* for PA1 */
    GPIOA->MODER &= ~(3U << 2*1);
    GPIOA->MODER |= (1U << 2*1);

   /* lighting the leds up! */
    GPIOA->ODR |= (1U << 0);  //for pa0 (blue led)
    GPIOA->ODR |= (1U << 1);  //for pa1 (yellow led)

    /* setting PB1 as input (button 1) */
    GPIOB->MODER &= ~(3U << 2*1);
    /* setting PB2 as input (button 2) */
    GPIOB->MODER &= ~(3U << 2*2);


    /* defining button1 (PB1) external interrupt */
    EXTI->RTSR1 |= (1U << 1);
    EXTI->EXTICR[0] |= (1U << 8*1);
    EXTI->IMR1 |= (1U << 1);
    /* enable NVIC and seting priority for button1 */
    NVIC_SetPriority(EXTI0_1_IRQn, 0); //priority number is 0
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    /* defining button2 (PB2) external interrupt */
    EXTI->RTSR1 |= (1U << 2);
    EXTI->EXTICR[0] |= (1U << 8*2);
    EXTI->IMR1 |= (1U << 2);
    /* enable NVIC and seting priority for button2 */
    NVIC_SetPriority(EXTI2_3_IRQn, 1); //priority number is 1
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    while(1) {
    }
    return 0;
}

void delay(volatile uint32_t s) {
    for(; s>0; s--);
}