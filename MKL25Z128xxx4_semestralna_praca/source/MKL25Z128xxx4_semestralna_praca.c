
#include "board.h"
#include <Premenne.h>
#include <Send_Funkcie.h>
#include <CRC8.h>
#include <Delay.h>
#include <Motor_Go.h>
#include <Stop_Floor_Function.h>
#include <Led_And_Movement.h>

void DEMO_LPSCI_IRQHandler(void) {
	if ((kLPSCI_RxDataRegFullFlag) & LPSCI_GetStatusFlags(DEMO_LPSCI)) {
		Sprava[Index] = LPSCI_ReadByte(DEMO_LPSCI);
		if (Index != 4) {
			Index++;
		}
		if (Index == 4) {
			Sprava_Complete = 1;
		}
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
	Destination = 0xe4;
	Switch_e4 = 1;
	while (1) {
		if (Sprava_Complete == 1) {
			Send_ACK(Sprava[2]);
			delay(10);
			LED_and_Switch_Setter_On();
			Rozsviet_LED_OUT(Sprava[2]);
			Rozsviet_LED_IN(Sprava[2]);
			Last_Know_Movement_Setter();
			Stop_Floor();
			if (Poloha_Actual == Destination) {
				Set_Destination();
			}
			Index = 0;
			Sprava_Complete = 0;
		}
	}
}
