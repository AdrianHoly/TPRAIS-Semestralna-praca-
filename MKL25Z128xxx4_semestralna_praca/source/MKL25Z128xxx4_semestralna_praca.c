
#include "board.h"
#include <Premenne.h>
#include <Send_Funkcie.h>
#include <CRC8.h>
#include <Delay.h>
#include <Motor_Go.h>

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

void LED_and_Switch_Setter_On() {
	if (Sprava[2] == 0xc0 || Sprava[2] == 0xb0) {
		LED_P_OUT = 1;
		LED_P_IN = 1;
		Switch_e0 = 1;
	}
	if (Sprava[2] == 0xc1 || Sprava[2] == 0xb1) {
		LED_1_OUT = 1;
		LED_1_IN = 1;
		Switch_e1 = 1;
	}
	if (Sprava[2] == 0xc2 || Sprava[2] == 0xb2) {
		LED_2_OUT = 1;
		LED_2_IN = 1;
		Switch_e2 = 1;
	}
	if (Sprava[2] == 0xc3 || Sprava[2] == 0xb3) {
		LED_3_OUT = 1;
		LED_3_IN = 1;
		Switch_e3 = 1;
	}
	if (Sprava[2] == 0xc4 || Sprava[2] == 0xb4) {
		LED_4_OUT = 1;
		LED_4_IN = 1;
		Switch_e4 = 1;
	}
}

void Set_Destination() {
	Last_Known_Movement = 2;
	if (Switch_e4 == 1 && Last_Known_Movement == 2 ) {
		Destination = 0xe4;
		Motor_Go_To_4(Poloha_Actual);
		Last_Known_Movement = Motor_Go_To_4(Poloha_Actual);
	}
	if (Switch_e3 == 1 && Last_Known_Movement == 2) {
		Destination = 0xe3;
		Motor_Go_To_3(Poloha_Actual);
		Last_Known_Movement = Motor_Go_To_3(Poloha_Actual);
	}
	if (Switch_e2 == 1 && Last_Known_Movement == 2) {
		Destination = 0xe2;
		Motor_Go_To_2(Poloha_Actual);
		Last_Known_Movement = Motor_Go_To_2(Poloha_Actual);
	}
	if (Switch_e1 == 1 && Last_Known_Movement == 2) {
		Destination = 0xe1;
		Motor_Go_To_1(Poloha_Actual);
		Last_Known_Movement = Motor_Go_To_1(Poloha_Actual);
	}
	if (Switch_e0 == 1 && Last_Known_Movement == 2) {
		Destination = 0xe0;
		Motor_Go_To_0(Poloha_Actual);
		Last_Known_Movement = Motor_Go_To_0(Poloha_Actual);
	}
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
		Motor_Down_Control = 0;
		Motor_Up_Control = 0;
		if (Motor_Stop_Control == 0) {
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

			Zhasni_LED_OUT(Sprava[2]);
			LED_P_OUT = 0;
			Zhasni_LED_IN(Sprava[2]);
			LED_P_IN = 0;
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
			Zhasni_LED_OUT(Sprava[2]);
			LED_1_OUT = 0;
			Zhasni_LED_IN(Sprava[2]);
			LED_1_IN = 0;
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

			Zhasni_LED_OUT(Sprava[2]);
			LED_2_OUT = 0;
			Zhasni_LED_IN(Sprava[2]);
			LED_2_IN = 0;
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

			Zhasni_LED_OUT(Sprava[2]);
			LED_3_OUT = 0;
			Zhasni_LED_IN(Sprava[2]);
			LED_3_IN = 0;
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
			Zhasni_LED_OUT(Sprava[2]);
			LED_4_OUT = 0;
			Zhasni_LED_IN(Sprava[2]);
			LED_4_IN = 0;
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
