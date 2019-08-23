#include "debug.h"


signed char   scCmdModeCheck=0;
unsigned char ucCmdModeCount=0;
unsigned char ucCmdModeState=0;
signed char   scCmdJudgement = FALSE;

Uart_Protocol_Str gStrDebug;
Uart_Protocol_Str* gpStrDebug;

Uart_Protocol_Str gStrRemote;
Uart_Protocol_Str* gpStrRemote;
unsigned char gRecTxTempBuff[MAX_PROTOCOL_SIZE];

char	chRx_buf[BUFFER_SIZE][20];
char	chCmd_msg[BUFFER_SIZE][20]={
    "help",	//0
    "debug",	//1,
    "mt",	//2,
    "pwr",	//3,
    "led",	//4,
    "valve",	//5,
};


void DebugCmdOpen(void){


    if(gpStrDebug->usRxTail != gpStrDebug->usRxHead){

        if(gpStrDebug->ucRxBuff[gpStrDebug->usRxTail] == BACK_SPACE_KEY)
        {
            if(gpStrDebug->ucParseusCnt != 0)
            {
                gpStrDebug->usRxHead-= 2; // Head = Head - 2 ;(remove BACK_SPACE_KEY + Char)
                gpStrDebug->usRxTail--;
                gpStrDebug->ucParseusCnt--;


                //uart_print("%c%c%c",BACK_SPACE_KEY, SPACE_KEY, BACK_SPACE_KEY); // 190709_by KES commented original code
                uart_char(BACK_SPACE_KEY); // 190716_by KES : modified
                uart_char(SPACE_KEY);
                uart_char(BACK_SPACE_KEY);

            }
            else
            {
                gpStrDebug->usRxHead--;

                //uart_print("%c%c", SPACE_KEY, BACK_SPACE_KEY); // 190709_by KES commented original code
                uart_char(SPACE_KEY);      // 190716_by KES : modified
                uart_char(BACK_SPACE_KEY);
            }
            return;
        }


        uart_char(gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]);
        if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'd') && (ucCmdModeCount == 0)){
            ucCmdModeCount = 1;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'e') && (ucCmdModeCount == 1)){
            ucCmdModeCount = 2;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'b') && (ucCmdModeCount == 2)){
            ucCmdModeCount = 3;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'u') && (ucCmdModeCount == 3)){
            ucCmdModeCount = 4;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'g') && (ucCmdModeCount == 4)){
            ucCmdModeCount = 5;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'c') && (ucCmdModeCount == 5)){
            ucCmdModeCount = 6;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'm') && (ucCmdModeCount == 6)){
            ucCmdModeCount = 7;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 'd') && (ucCmdModeCount == 7)){
            ucCmdModeCount = 8;
        }else if((gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1]   == 0x0d) && (ucCmdModeCount == 8)){
            //nChairRunState = CHAIR_STATE_WAIT_COMMAND;
            ucCmdModeCount = 0;
            scCmdJudgement = TRUE;
        }else{
            gpStrDebug->ucRxBuff[(gpStrDebug->usRxHead)-1] = 0;
            ucCmdModeCount = 0;
            scCmdJudgement = FALSE;
        }

        gpStrDebug->usRxTail++;
        gpStrDebug->usRxTail %= MAX_ARRAY_SIZE;

        gpStrDebug->ucParseusCnt++;

        if(gpStrDebug->ucParseusCnt >= MAX_DEBUG_PACKET){
            gpStrDebug->ucParseusCnt = 0;
        }

    }


    if(scCmdJudgement == TRUE){
        uart_print("\r\nPass");
        scCmdJudgement = FALSE;
        gpStrDebug->ucParseusCnt = 0;
        return;

    }else if(scCmdJudgement == FALSE){
        uart_print("\r\nFail\r\n");
        scCmdJudgement = FALSE;
    }else{ // scCmdJudgement == FALSE

    }

    scCmdJudgement = FALSE;
}



void Debug_Init(void)
{
    gpStrDebug = &gStrDebug;
    gpStrDebug->ucpWriteBuff 	= gpStrDebug->ucRxBuff;
    // gpStrDebug->huart 		= &huart3; // 190709_by KES : commented original code
    // HAL_UART_Receive_IT(gStrDebug.huart, &gStrDebug.ucRxData, 1); // 190709_by KES : commented original code
}



static void Debug_Rx_Process(void){



}




void Debug_Process(void)
{
    // uart_print("\r\nDebugProcess>>"); // 190709_by KES : commented original code
    Debug_Rx_Data();
    // Debug_Rx_Process(); // 190729_by KES : Commented
    // Debug_Tx_Data();
    Debug_Interrupt_Proc();
}

static void Debug_Tx_Data(void)
{
    Debug_Msg_Show();
}



static void Debug_Rx_Data(void)
{
    if(gpStrDebug->usRxTail != gpStrDebug->usRxHead){

        if(gpStrDebug->ucRxBuff[gpStrDebug->usRxTail] == BACK_SPACE_KEY){
            if(gpStrDebug->ucParseusCnt != 0){
                gpStrDebug->usRxHead-= 2; // Head = Head - 2 ;(remove BACK_SPACE_KEY + Char)
                gpStrDebug->usRxTail--;
                gpStrDebug->ucParseusCnt--;

                //uart_print("%c%c%c",BACK_SPACE_KEY, SPACE_KEY, BACK_SPACE_KEY); // 190709_by KES commented original code
                uart_char(BACK_SPACE_KEY); // 190716_by KES : modified
                uart_char(SPACE_KEY);
                uart_char(BACK_SPACE_KEY);
            }else{
                gpStrDebug->usRxHead--;

                //uart_print("%c%c", SPACE_KEY, BACK_SPACE_KEY); // 190709_by KES commented original code
                uart_char(SPACE_KEY);      // 190716_by KES : modified
                uart_char(BACK_SPACE_KEY);
            }
            return;
        }


        if(gpStrDebug->usRxTail == gpStrDebug->usRxHead){ // 190801_by KES : Write new line when Tail and Head has become same value.
            uart_print("\r\n");
        }

        gpStrDebug->ucParsedata[gStrDebug.ucParseusCnt] = gpStrDebug->ucRxBuff[gpStrDebug->usRxTail];
        gpStrDebug->usRxTail++;
        gpStrDebug->usRxTail %= MAX_ARRAY_SIZE;




        // echo
        //Uart Echo
        // HAL_UART_Transmit(gStrDebug.huart, &gStrDebug.ucParsedata[gStrDebug.ucParseusCnt], 1, TIME_200MS); // 190709_by KES  : commented original code

        // 190715_by KES : Insert "\r\n" to the head of echo(User Command)
        if(gpStrDebug->ucParseusCnt == 0){
            uart_print("\r\n");
        }

        // 190715 BY_KES : Uart ehco
        uart_char(gStrDebug.ucParsedata[gStrDebug.ucParseusCnt]);


        if(gpStrDebug->ucParsedata[gpStrDebug->ucParseusCnt] == ENTER_KEY )
        {
            // HAL_UART_STATE_READY : 0x20
            // while(gStrDebug.huart->gState != HAL_UART_STATE_READY) // 190709_by KES : commented original code

            // USART_STATUS_RXDATAV : Set when data is available in the receive buffer. Cleared when the receive buffer is empty.

#if 0 // 수정해야 함.
            while ((USART2->STATUS & USART_STATUS_RXDATAV))// 190709_by KES : Modified
            {
                gpStrDebug->usTxWaitTime++;
                if(gpStrDebug->usTxWaitTime >= 10000){
                    gpStrDebug->usTxWaitTime = 0;
                    gpStrDebug->ucTxWaitFlag = TRUE;
                    break;
                }
            }
#endif



            if(gpStrDebug->ucTxWaitFlag){
                gpStrDebug->ucTxWaitFlag = FALSE;
                gpStrDebug->usTxWaitTime = 0;
                return;
            }



            gpStrDebug->usRxMidusCnt = 0;
            gpStrDebug->usRxLitusCnt = 0;
            gpStrDebug->usRxMsgusCnt = 0;
            memset(chRx_buf, 0x0, sizeof(chRx_buf));

            for(unsigned int tusCnt = 0; tusCnt < gpStrDebug->ucParseusCnt; tusCnt++)
            {
                if(gpStrDebug->ucParsedata[tusCnt] == SPACE_KEY)
                {
                    tusCnt++;
                    gpStrDebug->usRxMidusCnt++;
                    gpStrDebug->usRxLitusCnt = 0;
                }
                chRx_buf[gpStrDebug->usRxMidusCnt][gpStrDebug->usRxLitusCnt] = gpStrDebug->ucParsedata[tusCnt];
                gpStrDebug->usRxLitusCnt++;
            }

            gpStrDebug->usRxMidusCnt= 0;
            gpStrDebug->usRxLitusCnt = 0;
            gpStrDebug->ucParseusCnt = MAX_DEBUG_PACKET-1; // 39

            unsigned char Array[MAX_PROTOCOL_SIZE];
            unsigned char CheckSum = 0;

            /*
            char chCmd_msg[30][20]={
            "help",		//0
            "debug",	    //1,
            "mt",		    //2,
            "pwr",		    //3,
            "led",		    //4,
            "valve",	    //5,
        };
            */

            while(1){   // If there are same then return 0
                if(strcmp(chRx_buf[gStrDebug.usRxMidusCnt], chCmd_msg[gStrDebug.usRxMsgusCnt])){
                    if(gpStrDebug->usRxMsgusCnt++ >= 30){
                        break;

                    }

                }else{
                    switch(gpStrDebug->usRxMsgusCnt){ // 190718_By KES : Original code
                        case DEBUG_HELP:
                        Help_Menu();

                        break;

                        case DEBUG_MSG:

                        break;

                        case DEBUG_MOTOR:
                        if(strcmp(chRx_buf[1],"test") == SAME){
                            if(strcmp(chRx_buf[2],"leg") == SAME){
                                uart_print("test leg. Sensor detect\n\r");


                            }else if(strcmp(chRx_buf[2],"back") == SAME){
                                uart_print("test back. Sensor detect\n\r");

                            }else{
                                uart_print("\n\rmt test [ERROR]. \n\rThe command you typed is not correct");
                            }

                        }else if(strcmp(chRx_buf[1],"body") == SAME){
                            if(strcmp(chRx_buf[2],"up") == SAME){
                                uart_print("\n\r Body UP");

                            }else if(strcmp(chRx_buf[2],"down") == SAME){
                                uart_print("\n\r Body Down");


                            }
                    }else if(strcmp(chRx_buf[1],"leg") == SAME){
                        if(strcmp(chRx_buf[2],"up") == SAME){
                            uart_print("\n\r Leg Up");


                        }else if(strcmp(chRx_buf[2],"down") == SAME){
                            uart_print("\n\r Leg Down");


                        }
                    }else if(strcmp(chRx_buf[1],"walk") == SAME){
                        if(strcmp(chRx_buf[2],"dir") == SAME){
                            uart_print("\n\r Walk Direction : ");

                        }else if(strcmp(chRx_buf[2],"loc") == SAME){
                            uart_print("\n\r Walk Location : ");

                            uart_print("\n\r");

                        }
                    }else if(strcmp(chRx_buf[1],"xd") == SAME){

                        if(strcmp(chRx_buf[2],"dir") == SAME){
                            uart_print("\n\r XD Direction : ");

                        }else if(strcmp(chRx_buf[2],"loc") == SAME){
                            uart_print("\n\r XD Location : ");

                        }else if(strcmp(chRx_buf[2],"set") == SAME){
                            uart_print("\n\r XD set : ");

                        }else{
                            uart_print("mt xd [ERROR]. \n\r The command you typed is not correct \n\r");

                        }

                    }else if(strcmp(chRx_buf[1],"mode") == SAME){
                        if(strcmp(chRx_buf[2],"press") == SAME){
                            uart_print("\n\rManual Press");


                        }else if(strcmp(chRx_buf[2],"fix") == SAME){
                            uart_print("\n\rManual Fix");


                        }else{
                            uart_print("\n\rmt mode [ERROR]. \n\rThe command you typed is not correct");

                        }

                    }else if(strcmp(chRx_buf[1],"roll") == SAME){

                        if(strcmp(chRx_buf[2],"on") == SAME){
                            uart_print("\n\rFoot rolling motor on");

                        }else if(strcmp(chRx_buf[2],"off") == SAME){


                        }else if(strcmp(chRx_buf[2],"speed") == SAME){


                        }

                    }else{
                        uart_print("\n\rmt [ERROR]. \n\rThe command you typed is not correct");

                    }

                    break;

                    case DEBUG_POWER_CTL:

                    break;

                    case DEBUG_LED_CTL:

                    break;


                    case DEBUG_VALVE_CTL:

                    if(strcmp(chRx_buf[1],"auto") == SAME){
                        uart_print("\n\rAuto air on\n\rAir Strength : ");

                    }else if(strcmp(chRx_buf[1],"arm") == SAME){
                        unsigned char airStrength= 0;
                        uart_print("\n\rArm air on\n\rAir Strength : ");

                    }else if(strcmp(chRx_buf[1],"leg") == SAME){
                        unsigned char airStrength= 0;
                        uart_print("\n\rLeg air on\n\rAir Strength : ");

                    }else if(strcmp(chRx_buf[1],"butt") == SAME){
                        uart_print("\n\rButtocks air on\n\rAir Strength : ");


                    }else if(strcmp(chRx_buf[1],"off") == SAME){
                        uart_print("\n\rAll valve off");

                    }else{
                        uart_print("\n\rvalve [ERROR]. \n\rThe command you typed is not correct");
                    }

                    break;

                    default :

                    break;
                }

                break;
            }
        }

        if(gpStrDebug->usRxMsgusCnt == EQUAL_NOTHING){
            uart_print("\n\r[ERROR]. \n\rThe command you typed is not correct");
        }

    }



    gpStrDebug->ucParseusCnt++;

    if(gpStrDebug->ucParseusCnt >= MAX_DEBUG_PACKET){
        gpStrDebug->ucParseusCnt = 0;
    }
}
// HAL_UART_Receive_IT(gStrDebug.huart, &gStrDebug.ucRxData, 1);
}





static void Debug_Interrupt_Proc(void)
{
    if(gpStrDebug->ucTickIrq == TRUE){
        gpStrDebug->ucTickIrq = FALSE;
    }
}

static void Debug_Msg_Show(void)
{

}






// 190709_by KES commented
#if 0
void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
    if(gStrRemote.huart->Instance  == huart->Instance)
    {
	/*
        if(gpStrRemote->ucTxSendFlag == TRUE)
        {
        gpStrRemote->ucTxSendFlag = FALSE;
        gpStrRemote->usTxTail++;
        if(gpStrRemote->usTxTail >= MAX_PROTOCOL_NUM){
        gpStrRemote->usTxTail = 0;
    }
    }
	*/
    }

    if(gStrRs485.huart->Instance  == huart->Instance)
    {
        DWT_Delay_us(100);
        RX485_DATACTL(RX_MODE);
        Rs485_Tx_Complete();

    }
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
    if( gStrDebug.huart->Instance == huart->Instance)
    {
        gpStrDebug->ucRxBuff[gpStrDebug->usRxHead] = gpStrDebug->ucRxData;

        gpStrDebug->usRxHead++;
        if(gpStrDebug->usRxHead >= MAX_ARRAY_SIZE)
        {
            gpStrDebug->usRxHead = 0;
        }
        HAL_UART_Receive_IT(gStrDebug.huart, &gStrDebug.ucRxData, 1);
    }

    if(gStrRemote.huart->Instance  == huart->Instance)
    {
        unsigned char uctmp;

        uctmp = gStrRemote.ucRxData;

        //if received char is equal to below char, timer(MainCom_rx.TimeOut) run.
        //If the time exceeds 100 ms, MCU clear received packet.
        if(uctmp == REMOTE_CHARACTER_1){
            if(gStrRemote.ucReadState== FALSE){
                gStrRemote.usReadusCnt= 0;
            }
            gStrRemote.ucReadState = TRUE;
        }

        gStrRemote.ucpWriteBuff[gStrRemote.usReadusCnt] = uctmp;
        gStrRemote.usReadusCnt++;

        //If received data is over 5, start parse received data
        if((gStrRemote.ucReadState == TRUE) && (gStrRemote.usReadusCnt > 5) && (gStrRemote.usReadusCnt == gStrRemote.ucpWriteBuff[UART_LENGTH] + 6)){
            gStrRemote.ucpReadBuff= (unsigned char*)(gStrRemote.ucRxBuff + gStrRemote.usReadOffset);

            gStrRemote.usReadOffset += MAX_PROTOCOL_SIZE;
            if(gStrRemote.usReadOffset >= MAX_ARRAY_SIZE){
                gStrRemote.usReadOffset = 0;
            }

            gStrRemote.ucpWriteBuff= (unsigned char*)(gStrRemote.ucRxBuff + gStrRemote.usReadOffset);

            for(unsigned char usCnt =0; usCnt < MAX_PROTOCOL_SIZE; usCnt++) {
                gStrRemote.ucpWriteBuff[usCnt] = 0;
            }

            gStrRemote.ucCmdStart = TRUE;
            gStrRemote.ucReadState = FALSE;
            gStrRemote.usReadusCnt= 0;
            gStrRemote.ucCalCheckSum = 0;
            gStrRemote.ucReadTimeOut = 0;
            //HAL_StatusTypeDef ret = HAL_UART_Receive_IT(gStrRemote.huart, &gStrRemote.ucRxData, 1);
            //return;
        }
        //end parser

        if(gStrRemote.usReadusCnt >= MAX_ARRAY_SIZE) {
            gStrRemote.usReadusCnt = 0;
        }
        HAL_UART_Receive_IT(gStrRemote.huart, &gStrRemote.ucRxData, 1);
    }

    if(gStrRs485.huart->Instance  == huart->Instance)
    {
        unsigned char uctmp;

        uctmp = gStrRs485.ucRxData;

        //if received char is equal to below char, timer(MainCom_rx.TimeOut) run.
        //If the time exceeds 100 ms, MCU clear received packet.
        if(uctmp == REMOTE_CHARACTER_1){
            if(gStrRs485.ucReadState== FALSE){
                gStrRs485.usReadusCnt= 0;
            }
            gStrRs485.ucReadState = TRUE;
        }
        gStrRs485.ucpWriteBuff[gStrRs485.usReadusCnt] = uctmp;
        gStrRs485.usReadusCnt++;

        //If received data is over 5, start parse received data
        if((gStrRs485.ucReadState == TRUE) && (gStrRs485.usReadusCnt > 5) && (gStrRs485.usReadusCnt == gStrRs485.ucpWriteBuff[UART_LENGTH] + 6)){
            gStrRs485.ucpReadBuff = (unsigned char*)(gStrRs485.ucRxBuff + gStrRs485.usReadOffset);

            gStrRs485.usReadOffset += MAX_PROTOCOL_SIZE;
            if(gStrRs485.usReadOffset >= MAX_ARRAY_SIZE){
                gStrRs485.usReadOffset = 0;
            }

            gStrRs485.ucpWriteBuff= (unsigned char*)(gStrRs485.ucRxBuff + gStrRs485.usReadOffset);

            for(unsigned char usCnt =0; usCnt < MAX_PROTOCOL_SIZE; usCnt++) {
                gStrRs485.ucpWriteBuff[usCnt] = 0;
            }

            gStrRs485.ucCmdStart = TRUE;
            gStrRs485.ucReadState = FALSE;
            gStrRs485.usReadusCnt= 0;
            gStrRs485.ucCalCheckSum = 0;
            gStrRs485.ucReadTimeOut = 0;
        }
        //end parser

        if(gStrRs485.usReadusCnt >= MAX_ARRAY_SIZE) {
            gStrRs485.usReadusCnt = 0;
        }

        HAL_UART_Receive_IT(gStrRs485.huart, &gStrRs485.ucRxData, 1);
    }
}

#endif

void Help_Menu(void)
{

    uart_print("\r\n********************************************************************************************\r\n");

    uart_print("[mt : Motor]\r\n");
    uart_print(" mt mode press - Manual Press mode.\r\n");
    uart_print(" mt mode fix   - Massage range : Fix.\r\n\r\n");

    uart_print(" mt walk dir   - Walk Motor direction.\r\n");
    uart_print("         loc   - Walk Motor location.\r\n");
    uart_print("         up    - Move walk motor up.\r\n");
    uart_print("         down  - Move walk motor down.\r\n");
    uart_print("         stop  - Stop walk motor.\r\n\r\n");

    uart_print(" mt xd loc     - XD motor location.\r\n");
    uart_print("       set 0~5 - XD motor set STEP 0~5\r\n");
    uart_print(" ex)\r\n");
    uart_print("  ***************************************\r\n");
    uart_print("  * mt xd set 1   - XD motor set STEP 1 *\r\n");
    uart_print("  ***************************************\r\n\r\n");

    uart_print(" mt body up     - Body angle motor up.\r\n");
    uart_print("         down   - Body angle motor down.\r\n");
    uart_print("         stop   - Body angle motor stop.\r\n\r\n\r\n");

    uart_print(" mt leg up     - Leg motor up.\r\n");
    uart_print("        down   - Leg motor down.\r\n");
    uart_print("        stop   - Leg motor stop.\r\n\r\n\r\n");

    uart_print("[valve : Air valve]\r\n");
    uart_print(" valve auto 1~5 - Auto     airbags on. Strength 1~5\r\n");
    uart_print(" valve arm  1~5 - Arm      airbags on. Strength 1~5\r\n");
    uart_print(" valve leg  1~5 - Leg      airbags on. Strength 1~5\r\n");
    uart_print(" valve butt 1~5 - Buttocks airbags on. Strength 1~5\r\n");
    uart_print(" ex)\r\n");
    uart_print("  ****************************************************\r\n");
    uart_print("  * valve butt 3   - Buttocks airbags on. Strength 3 *\r\n");
    uart_print("  * valve off      - All airbags off                 *\r\n");
    uart_print("  ****************************************************\r\n\r\n");
    uart_print("\r\n********************************************************************************************\r\n");

}