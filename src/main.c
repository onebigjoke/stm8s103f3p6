#include "main.h"

// PB5 : LED
// PC4 : CCO : 16MHz/8 = 2MHz
// systick TIM1
// usart - PD6 : RX (BLUETOOTH : COM16 - DESKTOP)
//         PD5 : TX
volatile uint32_t systick = 0;
uint32_t timeTest = 0;

int main( void )
{

    //CCOR PC4 Check
    // CLK->CCOR &= ~(0xF); // CCOR Clear
    // CLK->CCOR |= (4<<1); // Fcpu 2Mhz
    // CLK->CCOR |= (1<<0);
    Init_Led();
    Init_Tim1();

    UART1->BRR2 = 1;
    UART1->BRR1 = 1; // 2,000,000 / 115200 = 0d17 = 0x11

    UART1->CR2 |= (1<<5); // Receiver interrupt Enable
    UART1->CR2 |= (1<<3); // Transmitter Enable
    UART1->CR2 |= (1<<2); // Receive Enable

    // disableInterrupts();
    enableInterrupts();

    while(1){

        if(timeTest == 0){
            timeTest = GetCurrentTime();
        }

        if(GetCurrentTime() - timeTest > 1000){
            timeTest = 0;
            GPIOB->ODR ^= (1<<5);
            usart_Tx('A');

        }

    }
  return 0;
}


void Init_Led(void){
    GPIOB->DDR |= (1<<5);
    GPIOB->CR1 |= (1<<5);
    GPIOB->ODR &= ~(1<<5);
}

void Init_Tim1(void){
    TIM1->IER |= (1<<0); // Update Interrupt Enable
    TIM1->PSCRH = 0;
    TIM1->PSCRL = 1; // 2MHZ / 2 = 1,000,000Hz?
    TIM1->ARRL = (1000);
    TIM1->CR1 |= (1<<0); // TIM1 Counter Enables
}

uint32_t GetCurrentTime(void){
    uint32_t time;

    disableInterrupts();
    time = systick;
    enableInterrupts();

    return time;
}
