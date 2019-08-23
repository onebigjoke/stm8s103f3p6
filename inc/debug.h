#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "main.h"



void DebugCmdOpen(void);
void Help_Menu(void);
void Debug_Init(void);
void Debug_Process(void);
static void Debug_Msg_Show(void);
static void Debug_Rx_Data(void);
static void Debug_Tx_Data(void);
static void Debug_Interrupt_Proc(void);

#endif /* _DEBUG_H_ */