/*
 * problem7.c
 * Communication with MCU from PC and utilize 
 *      bidirectional data transmission and parsing.
 */

#include "bsp.h"
#define LEDDELAY 1600000U

int main(void){
    BSP_system_init();

    //trap
    for(;;){
    	uart_tx(uart_rx());
        print("Adem YILDIZ was here!\n\r");
        delay(LEDDELAY);
    }

    return 0;
}
