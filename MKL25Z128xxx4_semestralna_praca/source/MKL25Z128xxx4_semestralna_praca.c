
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include <fsl_lpsci.h>
#include <stdbool.h>


#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_LPSCI_IRQn UART0_IRQn
#define DEMO_LPSCI_IRQHandler UART0_IRQHandler

#define Sprava_Ack 0xa1
#define Sprava_Data 0xa0
#define Adresa_MCU 0x00

volatile uint8_t Sprava[10], Index = 0;
volatile uint8_t Index_Hranica = 4;
volatile uint8_t Sprava_Complete = 0;
volatile uint8_t LED_P_OUT, LED_1_OUT, LED_2_OUT, LED_3_OUT, LED_4_OUT = 0;
volatile uint8_t LED_P_IN, LED_1_IN, LED_2_IN, LED_3_IN, LED_4_IN = 0;
volatile uint8_t Switch_e0, Switch_e1, Switch_e2, Switch_e3, Switch_e4 = 0;
volatile uint8_t Motor_Down_Movement_Status,Motor_Up_Movement_Status = 0;
volatile uint8_t Destination = 0xe4;

void DEMO_LPSCI_IRQHandler(void) {
	if ((kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(DEMO_LPSCI)) {
		Sprava[Index] = LPSCI_ReadByte(DEMO_LPSCI);
		if(Index != 4){
			Index++;
		}
		if (Index == 4){
			Sprava_Complete = 1;

		}
	}
}

char crc8(const unsigned char *data,const unsigned int length)
{
   char crc = 0x00;
   char extract;
   char sum;
   for(int i=0;i<length;i++)
   {
      extract = *data;
      for (char tempI = 8; tempI; tempI--)
      {
         sum = (crc ^ extract) & 0x01;
         crc >>= 1;
         if (sum)
            crc ^= 0x8C;
         extract >>= 1;
      }
      data++;
   }
   return crc;
}

void Send_ACK_Demo(uint8_t Adresa_Prijatej_spravy){
	uint8_t CRC8_Data[] = { Adresa_Prijatej_spravy, Adresa_MCU };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t CRC8_ACK[] = { Sprava_Ack, Adresa_Prijatej_spravy, Adresa_MCU, 0x00, crc8(CRC8_Data,CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, CRC8_ACK, sizeof(CRC8_ACK));
}



void Rozsviet_LED_OUT(){
	if(Sprava[2]==0xc0){ //LED PRIZEMIE VONKAJSI TERMINAL
	    uint8_t CRC8_Data[] = { 0x10,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x10, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_P_OUT = 1;
	}
	if(Sprava[2]==0xc1){ //LED 1. POSCHODIE VONKAJSI TERMINAL
	    uint8_t CRC8_Data[] = { 0x11,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x11, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_1_OUT = 1;
	}
	if(Sprava[2]==0xc2){ //LED 2. POSCHODIE VONKAJSI TERMINAL
	    uint8_t CRC8_Data[] = { 0x12,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x12, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_2_OUT = 1;
	}
	if(Sprava[2]==0xc3){ //LED 3. POSCHODIE VONKAJSI TERMINAL
	    uint8_t CRC8_Data[] = { 0x13,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x13, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_3_OUT = 1;
	}
	if(Sprava[2]==0xc4){ //LED 4. POSCHODIE VONKAJSI TERMINAL
	    uint8_t CRC8_Data[] = { 0x14,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x14, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_4_OUT = 1;
	}
}

void Rozsviet_LED_IN(){
	if(Sprava[2]==0xb0){ //prizemna LED VNUTORNY TERMINAL
	    uint8_t CRC8_Data[] = { 0x20,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x20, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_P_IN = 1;
	}
	if(Sprava[2]==0xb1){ //prizemna LED VNUTORNY TERMINAL
	    uint8_t CRC8_Data[] = { 0x21,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x21, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_1_IN = 1;
	}
	if(Sprava[2]==0xb2){ //prizemna LED VNUTORNY TERMINAL
	    uint8_t CRC8_Data[] = { 0x22,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x22, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_2_IN = 1;
	}
	if(Sprava[2]==0xb3){ //prizemna LED VNUTORNY TERMINAL
	    uint8_t CRC8_Data[] = { 0x23,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x23, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_3_IN = 1;
	}
	if(Sprava[2]==0xb4){ //prizemna LED VNUTORNY TERMINAL
	    uint8_t CRC8_Data[] = { 0x24,0x00, 0x01 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x24, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_4_IN = 1;
	}
}




void Door_Open(){
	uint8_t CRC8_Data[] = { 0xf0,0x00, 0x00 };
    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Open[] = {Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Open, sizeof(Sprava_Door_Open));
}

void Door_Close(){ //JUST TRYIN PREROBIT
	uint8_t CRC8_Data[] = { 0xf0,0x00, 0x01 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Close[] = {Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Close, sizeof(Sprava_Door_Close));
}

void delay(int Milisekundy) {
	int i = 0;
	while(i <= Milisekundy * 10000){
		i++;
		__asm("nop");
	}


}


//https://www.cs.cornell.edu/~tomf/notes/cps104/twoscomp.html
//pouzity dvojkovy doplnok
//na jednotlive prevody som pouzil prog kalk.
//0000 0000 0000 0000 0000 0000 0110 0100     //dec 100 // up
// 0 0 0 0 0 0 6 4
//1111 1111 1111 1111 1111 1111 1001 1011     //spravena inverzia
//1111 1111 1111 1111 1111 1111 1001 1100     // +1
// F F F F F F 9 C  // FF FF FF 9C            // -100   // down
// MSB
void Motor_Up_Movement(){
	uint8_t CRC8_Data[] = { 0xf1,0x00, 0x02, 0x64, 0x00, 0x00, 0x00 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Move_Up[] = {Sprava_Data, 0xf1, Adresa_MCU, 0x05, 0x02, 0x64, 0x00, 0x00, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, Move_Up, sizeof(Move_Up));
}

void Motor_Down_Movement(){
	uint8_t CRC8_Data[] = { 0xf1,0x00, 0x02, 0x9C, 0xFF, 0xFF, 0xFF };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Move_Down[] = {Sprava_Data, 0xf1, Adresa_MCU, 0x05, 0x02, 0x9C, 0xFF, 0xFF, 0xFF, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, Move_Down, sizeof(Move_Down));
}

void Motor_Stop(){
	uint8_t CRC8_Data[] = { 0xf1,0x00, 0x01};
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Stop[] = {Sprava_Data, 0xf1, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, Stop, sizeof(Stop));
}

void LED_SWITCH_SETTER(){
	if(LED_P_OUT || LED_P_IN == 1){
		Switch_e0 = 1;
	}
	if(LED_1_OUT || LED_1_IN == 1){
		Switch_e1 = 1;
	}
	if(LED_2_OUT || LED_2_IN == 1){
		Switch_e2 = 1;
	}
	if(LED_3_OUT || LED_3_IN == 1){
		Switch_e3 = 1;
	}
	if(LED_4_OUT || LED_4_IN == 1){
		Switch_e4 = 1;
	}
}

void Stop_Floor (){
	if(Sprava[2]== 0xe1){
		if( Switch_e1 == 1){
		Motor_Stop();
		delay(200);
		Door_Open();
		delay(1000);
		Door_Close();
		if(Motor_Down_Movement_Status==1){
			Motor_Down_Movement();
		}
		if(Motor_Up_Movement_Status==1){
			Motor_Up_Movement();
		}
		Switch_e1 = 0;
		Zhasni_LED_OUT();
		}
	}
	if(Sprava[2]== 0xe2){
		if( Switch_e2 == 1){
		Motor_Stop();
		delay(200);
		Door_Open();
		delay(1000);
		Door_Close();
		if(Motor_Down_Movement_Status==1){
			Motor_Down_Movement();
		}
		if(Motor_Up_Movement_Status==1){
			Motor_Up_Movement();
		}
		Switch_e2 = 0;
		Zhasni_LED_OUT();
		}
	}
	if(Sprava[2]== 0xe3){
		if( Switch_e3 == 1){
		Motor_Stop();
		delay(200);
		Door_Open();
		delay(1000);
		Door_Close();
		if(Motor_Down_Movement_Status==1){
			Motor_Down_Movement();
		}
		if(Motor_Up_Movement_Status==1){
			Motor_Up_Movement();
		}
		Switch_e3 = 0;
		Zhasni_LED_OUT();
		}
	}
}

void Zhasni_LED_OUT(){
	if(Sprava[2]==0xe0){
		uint8_t CRC8_Data[] = { 0x10,0x00, 0x00 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x10, Adresa_MCU, 0x01, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_P_OUT = 0;
		}
	if(Sprava[2]==0xe1){
		uint8_t CRC8_Data[] = { 0x11,0x00, 0x00 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x11, Adresa_MCU, 0x01, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_1_OUT = 0;
		}

	if(Sprava[2]==0xe2){
		uint8_t CRC8_Data[] = { 0x12,0x00, 0x00 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x12, Adresa_MCU, 0x01, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_2_OUT = 0;
		}
	if(Sprava[2]==0xe3){
		uint8_t CRC8_Data[] = { 0x13,0x00, 0x00 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x13, Adresa_MCU, 0x01, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_3_OUT = 0;
		}
	if(Sprava[2]==0xe4){
		uint8_t CRC8_Data[] = { 0x14,0x00, 0x00 };
	    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	    uint8_t LED[] = {Sprava_Data, 0x14, Adresa_MCU, 0x01, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	    LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	    LED_4_OUT = 0;
		}
}

void Movement_Up_Down(){
	if(Sprava[2]==Destination){
		Destination = 0xe4;
		Motor_Stop();
		delay(200);
		Door_Open();
		Zhasni_LED_OUT();
	    delay(1000);
	    Door_Close();
	    Motor_Up_Movement();
	    Motor_Up_Movement_Status=1;
		Motor_Down_Movement_Status=0;

	}

	if(Sprava[2]==Destination){
		Destination = 0xe0;
		Motor_Stop();
		delay(200);
		Door_Open();
		Zhasni_LED_OUT();
		delay(1000);
		Door_Close();
		Motor_Down_Movement();
		Motor_Down_Movement_Status=1;
		Motor_Up_Movement_Status=0;

	}

}

int main(void) {

	lpsci_config_t config;

		BOARD_InitPins();
		BOARD_BootClockRUN();
		CLOCK_SetLpsci0Clock(0x1U);

		LPSCI_GetDefaultConfig(&config);
		config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
		config.enableTx = true;
		config.enableRx = true;

		LPSCI_Init(DEMO_LPSCI, &config, DEMO_LPSCI_CLK_FREQ);

		/* Enable RX interrupt. */
		LPSCI_EnableInterrupts(DEMO_LPSCI, kLPSCI_RxDataRegFullInterruptEnable);
		EnableIRQ(DEMO_LPSCI_IRQn);
        Door_Close();
        delay(100);
        Motor_Up_Movement();
		while(1) {
		    	if (Sprava_Complete == 1) {
		    		Send_ACK_Demo(Sprava[2]);
		    		delay(10);
		    		Rozsviet_LED_OUT();
		    		Rozsviet_LED_IN();
		    		LED_SWITCH_SETTER();
		    		delay(10);
		    		Movement_Up_Down();
		    		Stop_Floor();
		    		Index=0;
		    		Sprava_Complete = 0;
		    	}
		    }
		}

