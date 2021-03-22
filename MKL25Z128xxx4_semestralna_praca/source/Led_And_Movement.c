/*
 * Led_And_Movement.c
 *
 *  Created on: 17. 3. 2021
 *      Author: Ado
 */

/**
 * @file    Led_And_Movement.c
 * @brief   funkcie s globalnymi premennymi
 * @details v tejto knižnici sú umiestnené hlavne funckie kde operujem s globálnymi premennými
 *  funkcie sú zamerané na LED a pohybové globálne premenné
 *
 */
/*!
 * ak prijmem správu z tlačidla vonkajšieho alebo vnútorného terminálu
 * tak sa prestavia hodnoty LED na príslušnom poschodí na 1 a tiež sa nastaví
 * prislúchajúca premenná switch_e% na hodnotu 1
 */

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

/*!
 * na začiatku sa nastaví hodnota Last_Known_Movement na hodnotu 2 (t.j. stop pohyb)
 * tento krok som spravil v dôsledku korekcie funkcie, ku ktorej dochádzalo hlavne
 * ked bolo stlačených viacej tlačidiel naraz (hlavne v iných smeroch)
 *
 * (Switch_e4 == 1 && Last_Known_Movement == 2 )
 * ak je aktívny switch a zároveň Last_Known_Movement je na hodnote 2
 * ak by podmienka nezahrnovala LKM podmienku, tak pri dalsej podmienke by
 * bolo možné prebiť aktuálny smer výťahu, čo by malo za následok zastavenie výťahu
 */

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

/*!
 * funkcia vznikla za účelom zavretia dverí po tom čo výťah zastaví na príslušnom poschodí
 * LKM values = 1 smer hore, 0 smer dole, 2 stop
 * Motor_Control premenne su pouzite ako korekcia aby sa neposielali spravy viackrát
 * kedže táto funkcia nieje v maine ohraničená žiadnou podmienkou tak sa vykoná stále
 *
 * príklad:
 * ak je LKM 1 (t.j. výťah išiel smerom hore), tak sa nastavia control premenné ostatných smerov na hodnotu 0
 * aby sme ich mohli do budúcnosti použiť ak je Motor_Control príslušného smeru nastavená na 0 a dvere sú v
 * stave "OPEN" (t.j. Door_Status == 1) a ak je akýkoľvek iný switch v stave "Active" (myslené Switch_e% == 1)
 * tak sa dvere zavrú aby mohol výťah korektne pokračovať ďalej v jazde. pokiaľ nieje žiaden iný switch "Active"
 *  tak dvere zostanú otvorené a výťah čaká na "privolanie" následne sa pošle správa pre vykonávanie príslušneho
 *   smeru a nastavenie príslušnej Motor_Control premennej na hodnotu 1.
 */


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
