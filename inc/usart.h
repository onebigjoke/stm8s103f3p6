#ifndef _USART_H_
#define _USART_H_
#include "main.h"

#define BACK_SPACE_KEY                          0x08
#define SPACE_KEY				0x20
#define ENTER_KEY				0x0d

#define BUFFER_SIZE                             30
#define EQUAL_NOTHING  			        (BUFFER_SIZE+1)

#define MAX_BUFF_LENGTH				40
#define MAX_DEBUG_PACKET			MAX_BUFF_LENGTH

#define	MAX_PROTOCOL_SIZE			MAX_BUFF_LENGTH
#define MAX_PROTOCOL_NUM			20
#define MAX_ARRAY_SIZE				MAX_PROTOCOL_NUM * MAX_PROTOCOL_SIZE

#define CMD_RETRY_MAX_COUNT			3
#define CMD_RETRY_INTERVAL			TIME_50MS


typedef enum DEBUG_COMMAND{
	DEBUG_HELP,
	DEBUG_MSG,
	DEBUG_MOTOR,
	DEBUG_POWER_CTL,
	DEBUG_LED_CTL,
	DEBUG_VALVE_CTL,

	DEBUG_COMMAND_MAX,
}Debug_Cmd;



typedef struct UART_PROTOCOL_STRUCTURE
{
    unsigned char	ucInit;
    unsigned char	ucInitusCnt;
    unsigned char 	ucRxData;			//using receive data from uart

    unsigned short	usTxHead;
    unsigned short	usTxTail;
    unsigned short	usTxCnt;
    unsigned char	ucTxSendEnable;

    unsigned char	ucRxBuff[MAX_ARRAY_SIZE];
    unsigned short	usRxHead;
    unsigned short	usRxTail;
    unsigned short	usRxusCnt;

    /*Protocol parsing variable*/
    unsigned char* 	ucpReadBuff;
    unsigned char*	ucpWriteBuff;
    unsigned char	ucCmdStart;
    unsigned short	usReadOffset;
    unsigned short	usReadusCnt;
    unsigned char	ucReadState;
    unsigned short	usReadErrusCnt;			// checksum err

    unsigned char 	ucReadTimeOut;
    unsigned short 	ucReadTimeOutusCnt;		// can't receive all packet in time

    unsigned short 	usTxOffset;
    unsigned char	ucCalCheckSum;

    unsigned char 	ucTxSendFlag;
    unsigned char	ucTxSendFlagTimer;
    unsigned short 	usTxWaitTime;
    unsigned char 	ucTxWaitFlag;
    unsigned short	usTxSendTimer;
    unsigned short 	usTxTimeOut;
    unsigned short 	usTxTimeOutusCnt;
    unsigned char 	ucTxSendInterval;

    unsigned char	ucTickusCnt;
    unsigned char 	ucTickIrq;

    /*ucDebug parsing variable*/
    unsigned short	usRxMidusCnt;
    unsigned short	usRxLitusCnt;
    unsigned short	usRxMsgusCnt;

    unsigned char	ucParseusCnt;
    unsigned char	ucParsedata[MAX_BUFF_LENGTH];
}Uart_Protocol_Str;



extern void usart_Tx(uint8_t data);


#endif /* _USART_H_ */