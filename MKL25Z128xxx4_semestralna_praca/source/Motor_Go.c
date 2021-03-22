/*
 * Motor_Go.c
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */

/**
 * @file    Motor_Go.c
 * @brief   funkcie pre smerovanie vytahu
 * @details následujúce funkcie riadia pohyb výťahu
 *
 */
#include <Motor_Go.h>
/*!
 * Príklad:
 * Ak je aktuálna poloha SwitchE4, a bola zavolaná funkcia Motor_Go_To_0
 * tak podľa tejto podmienky vieme určiť ktorým smerom sa má vydať
 * v tomto prípade smerom dole.
 * každá funkcia vracia hodnotu podľa podmienky a hodnoty predstavujú Last_Known_Movement
 * (t.j. 1 smer hore, 0 smer dole)
 */


int Motor_Go_To_0(uint8_t Par_Poloha_Actual) {
	if (Par_Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe3) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe2) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe1) {
		Motor_Down_Movement();
		return 0;
	}

}
int Motor_Go_To_1(uint8_t Par_Poloha_Actual) {
	if (Par_Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe3) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe2) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		return 1;
	}
}
int Motor_Go_To_2(uint8_t Par_Poloha_Actual) {
	if (Par_Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe3) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe1) {
		Motor_Up_Movement();
		return 1;
	}
	if (Par_Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		return 1;
	}
}
int Motor_Go_To_3(uint8_t Par_Poloha_Actual) {
	if (Par_Poloha_Actual == 0xe4) {
		Motor_Down_Movement();
		return 0;
	}
	if (Par_Poloha_Actual == 0xe2) {
		Motor_Up_Movement();
		return 1;
	}
	if (Par_Poloha_Actual == 0xe1) {
		Motor_Up_Movement();
		return 1;
	}
	if (Par_Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		return 1;
	}

}
int Motor_Go_To_4(uint8_t Par_Poloha_Actual) {
	if (Par_Poloha_Actual == 0xe0) {
		Motor_Up_Movement();
		return 1;
	}
	if (Par_Poloha_Actual == 0xe3) {
		Motor_Up_Movement();
		return 1;
	}
	if (Par_Poloha_Actual == 0xe2) {
		Motor_Up_Movement();
		return 1;
	}
	if (Par_Poloha_Actual == 0xe1) {
		Motor_Up_Movement();
		return 1;
	}
}
