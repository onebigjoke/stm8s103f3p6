#ifndef _MAIN_H_
#define _MAIN_H_
#include "stm8s.h"
#include "usart.h"


extern volatile uint32_t systick;
extern uint32_t GetCurrentTime(void);

void Init_Tim1(void);


#endif /* _MAIN_H_ */