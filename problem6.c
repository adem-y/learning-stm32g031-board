/*
 * problem6.c
 *
 * author: Adem Yildiz
 * A general purpose timer is enabled and speed of the toggling led is changed by using timers.
 * Button is the external interrupt that causes this action.
 */

#include "stm32g0xx.h"

uint32_t sca = 999; // timer counts 1 second

void TIM1_BRK_UP_TRG_COM_IRQHandler(void){
    GPIOC->ODR ^= (1U << 6); // toggle the led
    TIM1->SR &= ~(1U << 0); // clear update status register
}

void set_tim1(void){

    RCC->APBENR2 |= (1U << 11); // enable TIM1 module clock

    TIM1->CR1  = 0; // control register = 0, just in case
    TIM1->CR1 |= (1U << 7);
    TIM1->CNT  = 0; // counter register = 0

    // sca seconds interrupt
    TIM1->PSC = sca; // prescaler varies with sca
    TIM1->ARR = 16000;

    TIM1->DIER |= (1U << 0); // interrupt enable
    TIM1->CR1 |= (1U << 0); // TIM1 enabe

    NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 3); // setting lowest priority
    NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

void EXTI2_3_IRQHandler(void){
    EXTI->RPR1 |= (1U << 3); //pending check for button
}

int main(void) {

    RCC->IOPENR |= (1U << 2); // enable GPIOC
    RCC->IOPENR |= (1U << 1); // enable GPIOB

    // pc6 is output
    GPIOC->MODER &= ~(3U << 2*6);
    GPIOC->MODER |= (1U << 2*6);

    // pb3 is input
    GPIOB->MODER &= ~(3U << 2*3);

    turnLed:
    // turn the led on
    GPIOC->ODR |= (1U << 6);

    EXTI->RTSR1 |= (1U << 3); // rising edge triggered
    EXTI->EXTICR[0] |= (1U << 8*3); //interrupt select
    EXTI->IMR1 |= (1U << 3); //interrupt mask

    // for button at pb3
    NVIC_SetPriority(EXTI2_3_IRQn, 1); //setting priority
    NVIC_EnableIRQ(EXTI2_3_IRQn);

    set_tim1();
    //updating prescaler value
    if (sca < 9999){
    	sca = sca + 999; // increase timer counts +1 second
    	goto turnLed; //turning the led on with new sca value
    	} else {
    		sca = 999; // makes timer 1 second again
    	}

    while(1) {
    }

    return 0;
}
