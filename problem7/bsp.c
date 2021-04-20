#include "bsp.h"

static volatile uint32_t tick = 0;


void BSP_system_init(){

    //__disable_irq();
    SysTick_Config(SystemCoreClock / 1000);
    BSP_UART_init(9600);
    //__enable_irq();

}

void SysTick_Handler(void){
	if(tick > 0){
		--tick;
	}
}

void delay_ms(uint32_t s){
	tick = s;
	while(tick);
}

void delay(volatile unsigned int s){
	for(; s > 0; s--);
}

/* all about interrupt
void USART2_IRQHandler(void){
    uint8_t data = (uint8_t)USART2 -> RDR;
    // RXNE is automatically cleared when read
    printChar(data);
}
*/

void uart_tx(uint8_t c){
    USART2 -> TDR = (uint16_t) c;
    while(!(USART2 -> ISR & (1 << 6))); // transmission complete?
}

uint8_t uart_rx(void){
    uint8_t data = (uint8_t)USART2 -> RDR;
    // RXNE is automatically cleared when read
    //printChar(data);

}


void printChar(uint8_t c){
    USART2 -> TDR = (uint16_t)c;
    while(!(USART2 -> ISR & (1 << 6))); // transmission complete?
}

void _print(int fd, char *buf, int len){
    (void)fd;
    for(int i = 0; i < len; ++i){
        printChar(buf[i]);
    }
}

void print(char *buf){
    int len = 0;
    while(buf[len++] != '\0');
    _print(0, buf, len);
}


/* PA2 PA3 are connected to the Virtual COM port
*  USART2 module
*/
void BSP_UART_init(uint32_t baud){
    RCC -> IOPENR |= (1U << 0);
    RCC -> APBENR1 |= (1U << 17);

    /* setup PA2 as AF1 */
    GPIOA -> MODER &= ~(3U << 2*2); // clear
    GPIOA -> MODER |= (2U << 2*2);
    /* choose AF1 from mux */
    GPIOA -> AFR[0] &= ~(0xFU << 4*2); // clear alternate func
    GPIOA -> AFR[0] |= (1U << 4*2);

    /* setup PA3 as AF1 */
    GPIOA -> MODER &= ~(3U << 2*3); // clear
    GPIOA -> MODER |= (2U << 2*3);
    /* choose AF1 from mux */
    GPIOA -> AFR[0] &= ~(0xFU << 4*3); // clear alternate func
    GPIOA -> AFR[0] |= (1U << 4*3);

    /* setup UART2
        reset UART CR1 */
    USART2 -> CR1 = 0;
    USART2 -> CR1 |= (1 << 3);  // transmitter enable
    USART2 -> CR1 |= (1 << 2);  // receiver enable
    USART2 -> CR1 |= (1 << 5);  // rxneie, interrupt falan

    USART2 -> BRR = (uint16_t)(SystemCoreClock / baud); // baud rate
    USART2 -> CR1 |= (1 << 0);  // USART enable
}
