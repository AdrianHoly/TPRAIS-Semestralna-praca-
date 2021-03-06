
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
volatile uint8_t Sprava_Complete = 0;



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

void Rozsviet_LED(){
	if(Sprava[2]==0xc0){ //prizemna LED
	uint8_t CRC8_Data[] = { 0x10,0x00, 0x01 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t LED_P[] = {Sprava_Data, 0x10, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, LED_P, sizeof(LED_P));

	}
}

void Door_Open(){
	uint8_t CRC8_Data[] = { 0xf0,0x00, 0x00 };
    uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Open[] = {Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x00, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Open, sizeof(Sprava_Door_Open));
}

void Door_Close(){ //JUST TRYIN PREROBIT
	 //prizemna LED
	uint8_t CRC8_Data[] = { 0xf0,0x00, 0x01 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Close[] = {Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x01, crc8(CRC8_Data,CRC8_Data_Length)};
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Close, sizeof(Sprava_Door_Close));

}

void delay(int Milisekundy) {
	int i = 0;
	while(i == Milisekundy * 10000){
		i++;
	}
	__asm("nop");
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

    while(1) {
    	if (Sprava_Complete == 1) {
    		Send_ACK_Demo(Sprava[2]);
    		delay(10);
    		Rozsviet_LED();
            if(Sprava[2]==0xc0){
    		Door_Close();
            }
            if(Sprava[2]==0xc4){
            Door_Open();
            }
    		Index=0;
    		Sprava_Complete = 0;
    	}
    }
}

