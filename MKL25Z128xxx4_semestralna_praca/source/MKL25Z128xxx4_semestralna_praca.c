
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
volatile uint8_t LED_P_OUT, LED_1_OUT, LED_2_OUT, LED_3_OUT, LED_4_OUT = 0;
volatile uint8_t LED_P_IN, LED_1_IN, LED_2_IN, LED_3_IN, LED_4_IN = 0;
volatile uint8_t Switch_e0, Switch_e1, Switch_e2, Switch_e3, Switch_e4 = 0;
volatile uint8_t Destination, Destination_Display = 0;
volatile uint8_t Poloha_Actual, Poloha_Actual_Display = 0;
volatile uint8_t Last_Known_Movement = 10; // 1 hore, 0 dole, 2 stop
volatile uint8_t Door_Status = 5; // 1=open, 0=closed
uint8_t Motor_Stop_Control, Motor_Up_Control, Motor_Down_Control = 0; // korekcia aby sa neposielali spravy nonstop

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

char crc8(const unsigned char *data, const unsigned int length) {
	char crc = 0x00;
	char extract;
	char sum;
	for (int i = 0; i < length; i++) {
		extract = *data;
		for (char tempI = 8; tempI; tempI--) {
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

void Send_ACK_Demo(uint8_t Adresa_Prijatej_spravy) {
	uint8_t CRC8_Data[] = { Adresa_Prijatej_spravy, Adresa_MCU };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t CRC8_ACK[] = { Sprava_Ack, Adresa_Prijatej_spravy, Adresa_MCU, 0x00,
			crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, CRC8_ACK, sizeof(CRC8_ACK));
}

void Rozsviet_LED_OUT() {
	if (Sprava[2] == 0xc0) { //LED PRIZEMIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x10, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x10, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_P_OUT = 1;
	}
	if (Sprava[2] == 0xc1) { //LED 1. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x11, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x11, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_1_OUT = 1;
	}
	if (Sprava[2] == 0xc2) { //LED 2. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x12, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x12, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_2_OUT = 1;
	}
	if (Sprava[2] == 0xc3) { //LED 3. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x13, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x13, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_3_OUT = 1;
	}
	if (Sprava[2] == 0xc4) { //LED 4. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x14, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x14, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_4_OUT = 1;
	}
}

void Rozsviet_LED_IN() {
	if (Sprava[2] == 0xb0) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x20, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x20, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_P_IN = 1;
	}
	if (Sprava[2] == 0xb1) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x21, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x21, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_1_IN = 1;
	}
	if (Sprava[2] == 0xb2) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x22, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x22, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_2_IN = 1;
	}
	if (Sprava[2] == 0xb3) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x23, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x23, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_3_IN = 1;
	}
	if (Sprava[2] == 0xb4) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x24, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x24, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_4_IN = 1;
	}
}

void Door_Open() {
	uint8_t CRC8_Data[] = { 0xf0, 0x00, 0x00 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Open[] = { Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x00,
			crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Open, sizeof(Sprava_Door_Open));
}

void Door_Close() { //JUST TRYIN PREROBIT
	uint8_t CRC8_Data[] = { 0xf0, 0x00, 0x01 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Close[] = { Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x01,
			crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Close,
			sizeof(Sprava_Door_Close));
}

void delay(int Milisekundy) {
	int i = 0;
	while (i <= Milisekundy * 10000) {
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
void Motor_Up_Movement() {
	uint8_t CRC8_Data[] = { 0xf1, 0x00, 0x02, 0x64, 0x00, 0x00, 0x00 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Move_Up[] = { Sprava_Data, 0xf1, Adresa_MCU, 0x05, 0x02, 0x64, 0x00,
			0x00, 0x00, crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Move_Up, sizeof(Move_Up));
}

void Motor_Down_Movement() {
	uint8_t CRC8_Data[] = { 0xf1, 0x00, 0x02, 0x9C, 0xFF, 0xFF, 0xFF };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Move_Down[] = { Sprava_Data, 0xf1, Adresa_MCU, 0x05, 0x02, 0x9C,
			0xFF, 0xFF, 0xFF, crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Move_Down, sizeof(Move_Down));
}

void Motor_Stop() {
	uint8_t CRC8_Data[] = { 0xf1, 0x00, 0x01 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Stop[] = { Sprava_Data, 0xf1, Adresa_MCU, 0x01, 0x01, crc8(
			CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Stop, sizeof(Stop));
}

void LED_SWITCH_SETTER() {
	if (LED_P_OUT || LED_P_IN == 1) {
		Switch_e0 = 1;
	}
	if (LED_1_OUT || LED_1_IN == 1) {
		Switch_e1 = 1;
	}
	if (LED_2_OUT || LED_2_IN == 1) {
		Switch_e2 = 1;
	}
	if (LED_3_OUT || LED_3_IN == 1) {
		Switch_e3 = 1;
	}
	if (LED_4_OUT || LED_4_IN == 1) {
		Switch_e4 = 1;
	}
}

void Zhasni_LED_OUT() {
	if (Sprava[2] == 0xe0) {
		uint8_t CRC8_Data[] = { 0x10, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x10, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_P_OUT = 0;
	}
	if (Sprava[2] == 0xe1) {
		uint8_t CRC8_Data[] = { 0x11, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x11, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_1_OUT = 0;
	}

	if (Sprava[2] == 0xe2) {
		uint8_t CRC8_Data[] = { 0x12, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x12, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_2_OUT = 0;
	}
	if (Sprava[2] == 0xe3) {
		uint8_t CRC8_Data[] = { 0x13, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x13, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_3_OUT = 0;
	}
	if (Sprava[2] == 0xe4) {
		uint8_t CRC8_Data[] = { 0x14, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x14, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_4_OUT = 0;
	}
}

void Zhasni_LED_IN() {
	if (Sprava[2] == 0xe0) {
		uint8_t CRC8_Data[] = { 0x20, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x20, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_P_IN = 0;
	}
	if (Sprava[2] == 0xe1) {
		uint8_t CRC8_Data[] = { 0x21, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x21, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_1_IN = 0;
	}

	if (Sprava[2] == 0xe2) {
		uint8_t CRC8_Data[] = { 0x22, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x22, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_2_IN = 0;
	}
	if (Sprava[2] == 0xe3) {
		uint8_t CRC8_Data[] = { 0x23, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x23, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_3_IN = 0;
	}
	if (Sprava[2] == 0xe4) {
		uint8_t CRC8_Data[] = { 0x24, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x24, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
		LED_4_IN = 0;
	}
}

void Motor_Go_To_0() {
	if (Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe3) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe2) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe1) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}

}
void Motor_Go_To_1() {
	if (Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe3) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe2) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
}
void Motor_Go_To_2() {
	if (Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe3) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe1) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
	if (Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
}
void Motor_Go_To_3() {
	if (Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		Last_Known_Movement = 0;
	}
	if (Poloha_Actual == 0xe2) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
	if (Poloha_Actual == 0xe1) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
	if (Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}

}
void Motor_Go_To_4() {
	if (Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
	if (Poloha_Actual == 0xe3) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
	if (Poloha_Actual == 0xe2) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
	if (Poloha_Actual == 0xe1) {
		Motor_Up_Movement();
		Last_Known_Movement = 1;
	}
}

void Set_Destination() {
	Last_Known_Movement = 2;
	if (Switch_e4 == 1) {
		Destination = 0xe4;
		Motor_Go_To_4();
	}
	if (Switch_e3 == 1) {
		Destination = 0xe3;
		Motor_Go_To_3();
	}
	if (Switch_e2 == 1) {
		Destination = 0xe2;
		Motor_Go_To_2();
	}
	if (Switch_e1 == 1) {
		Destination = 0xe1;
		Motor_Go_To_1();
	}
	if (Switch_e0 == 1) {
		Destination = 0xe0;
		Motor_Go_To_0();
	}
}

void Display(uint8_t Direction_Display, uint8_t Floor_Display) {
	uint8_t CRC8_Data[] = { 0x30, 0x00, Direction_Display, Floor_Display };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Info[] = { Sprava_Data, 0x30, Adresa_MCU, 0x02, Direction_Display,
			Floor_Display, crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Info, sizeof(Info));
}




void Last_Know_Movement_Setter() {
	if (Last_Known_Movement == 1) {
		Destination_Display = 0x01;
		Motor_Down_Control = 0;
		Motor_Stop_Control = 0;
		if (Motor_Up_Control == 0) {
			if (Door_Status == 1) {
				if (Switch_e0 == 1 || Switch_e1 == 1 || Switch_e2 == 1
						|| Switch_e3 == 1 || Switch_e4 == 1) {
					Door_Close();
					Door_Status = 0;
					delay(200);
				}
			}
			Motor_Up_Movement();
			Motor_Up_Control = 1;
		}
	}

	if (Last_Known_Movement == 0) {
		Destination_Display = 0x02;
		Motor_Up_Control = 0;
		Motor_Stop_Control = 0;
		if (Motor_Down_Control == 0) {
			if (Door_Status == 1) {
				if (Switch_e0 == 1 || Switch_e1 == 1 || Switch_e2 == 1
						|| Switch_e3 == 1 || Switch_e4 == 1) {
					Door_Close();
					Door_Status = 0;
					delay(200);
				}
			}
			Motor_Down_Movement();
			Motor_Down_Control = 1;
		}
	}

	if (Last_Known_Movement == 2) {
		Destination_Display = 0x00;
		Motor_Down_Control=0;
		Motor_Up_Control=0;
		if(Motor_Stop_Control == 0){
		Motor_Stop();
		Motor_Stop_Control = 1;
		Display(Destination_Display, Poloha_Actual_Display);
		}
	}
}

void Stop_Floor() {
	if (Sprava[2] == 0xe0) {
		Poloha_Actual = 0xe0;
		Poloha_Actual_Display = 0x50;
		Display(Destination_Display, Poloha_Actual_Display);
		if (Switch_e0 == 1) {
			Motor_Stop();
			Motor_Down_Control = 0;
			Motor_Up_Control = 0;
			Switch_e0 = 0;
			delay(200);
			Door_Open();
			Door_Status = 1;
			if (Switch_e0 == 1 || Switch_e1 == 1 || Switch_e2 == 1
					|| Switch_e3 == 1 || Switch_e4 == 1) {
				delay(1000);
				Door_Close();
				Door_Status = 0;
				delay(200);
			}

			Zhasni_LED_OUT();
			Zhasni_LED_IN();
		}
	}
	if (Sprava[2] == 0xe1) {
		Poloha_Actual = 0xe1;
		Poloha_Actual_Display = 0x31;
		Display(Destination_Display, Poloha_Actual_Display);
		if (Switch_e1 == 1) {
			Motor_Stop();
			Motor_Down_Control = 0;
			Motor_Up_Control = 0;
			Switch_e1 = 0;
			delay(200);
			Door_Open();
			Door_Status = 1;
			if (Switch_e0 == 1 || Switch_e1 == 1 || Switch_e2 == 1
					|| Switch_e3 == 1 || Switch_e4 == 1) {
				delay(1000);
				Door_Close();
				Door_Status = 0;
				delay(200);
			}
			Zhasni_LED_OUT();
			Zhasni_LED_IN();
		}
	}
	if (Sprava[2] == 0xe2) {
		Poloha_Actual = 0xe2;
		Poloha_Actual_Display = 0x32;
		Display(Destination_Display, Poloha_Actual_Display);
		if (Switch_e2 == 1) {
			Motor_Stop();
			Motor_Down_Control = 0;
			Motor_Up_Control = 0;
			Switch_e2 = 0;
			delay(200);
			Door_Open();
			Door_Status = 1;
			if (Switch_e0 == 1 || Switch_e1 == 1 || Switch_e2 == 1
					|| Switch_e3 == 1 || Switch_e4 == 1) {
				delay(1000);
				Door_Close();
				Door_Status = 0;
				delay(200);
			}

			Zhasni_LED_OUT();
			Zhasni_LED_IN();
		}
	}
	if (Sprava[2] == 0xe3) {
		Poloha_Actual = 0xe3;
		Poloha_Actual_Display = 0x33;
		Display(Destination_Display, Poloha_Actual_Display);
		if (Switch_e3 == 1) {
			Motor_Stop();
			Motor_Down_Control = 0;
			Motor_Up_Control = 0;
			Switch_e3 = 0;
			delay(200);
			Door_Open();
			Door_Status = 1;
			if (Switch_e0 == 1 || Switch_e1 == 1 || Switch_e2 == 1
					|| Switch_e3 == 1 || Switch_e4 == 1) {
				delay(1000);
				Door_Close();
				Door_Status = 0;
				delay(200);
			}

			Zhasni_LED_OUT();
			Zhasni_LED_IN();
		}
	}
	if (Sprava[2] == 0xe4) {
		Poloha_Actual = 0xe4;
		Poloha_Actual_Display = 0x34;
		Display(Destination_Display, Poloha_Actual_Display);
		if (Switch_e4 == 1) {
			Motor_Stop();
			Motor_Down_Control = 0;
			Motor_Up_Control = 0;
			Switch_e4 = 0;
			delay(200);
			Door_Open();
			Door_Status = 1;
			if (Switch_e0 == 1 || Switch_e1 == 1 || Switch_e2 == 1
					|| Switch_e3 == 1 || Switch_e4 == 1) {
				delay(1000);
				Door_Close();
				Door_Status = 0;
				delay(200);
			}
			Zhasni_LED_OUT();
			Zhasni_LED_IN();
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
			Send_ACK_Demo(Sprava[2]);
			delay(10);
			Rozsviet_LED_OUT();
			Rozsviet_LED_IN();
			LED_SWITCH_SETTER();
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

