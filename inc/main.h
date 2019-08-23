#ifndef _MAIN_H_
#define _MAIN_H_
#include <stdio.h>
#include <string.h>
#include "stm8s.h"
#include "usart.h"
#include "debug.h"

#define SAME            0
#define TRUE		1
#define FALSE		0


extern volatile uint32_t systick;
extern uint32_t GetCurrentTime(void);

void Init_Led(void);
void Init_Tim1(void);


#endif /* _MAIN_H_ */