#include "usart.h"

void usart_Tx(uint8_t data){

    while( (UART1->SR & (1<<7)) == 0 );
    UART1->DR = data;

}