/*
 * Stop_Floor_Function.c
 *
 *  Created on: 17. 3. 2021
 *      Author: Ado
 */

/**
 * @file    Stop_Floor_Function.c
 * @brief   zastavenie vytahu na poschodi
 * @details //funkcia vytvorena pre zastavenie výťahu v prípade, že prechádza cez poschodie
 * ktorého switch je v stave "Active" aj keď daný switch nieje zapísaný ako "Destination"
 *
 */
#include <Stop_Floor_Function.h>




 /*!
  *  tvar funkcie ak sme prijalu správu od switchu
  *   pošlú sa údaje o aktuálnej polohe výťahu na displej výťahu
  *   ak je daný switch v stave "Active" tak sa pošle správa o zastavení
  *   prestavia sa Motor_Control premenné na 0
  *   po delayi sa pošle správa otvorenia dverí a prestaví sa Door_Status na 1
  *   ak sú iné switche v stave active tak po krátkom delayi sa pošle správa o zatvorení dverí
  *   ako posledné sa pošle správa o vypnúti LED v prípade, že výťah vykonal svoju funkciu na príslušno poschodí
  */

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

