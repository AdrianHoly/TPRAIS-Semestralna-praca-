/*
 * Premenne.h
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */
#include <Definicie.h>

#ifndef PREMENNE_H_
#define PREMENNE_H_


extern volatile uint8_t Sprava[10], Index;
extern volatile uint8_t Sprava_Complete ;
extern uint8_t LED_P_OUT, LED_1_OUT, LED_2_OUT, LED_3_OUT, LED_4_OUT;
extern uint8_t LED_P_IN, LED_1_IN, LED_2_IN, LED_3_IN, LED_4_IN;
extern uint8_t Switch_e0, Switch_e1, Switch_e2, Switch_e3, Switch_e4;
extern uint8_t Destination, Destination_Display; //premenne použité na posielanie údajov na displej
extern uint8_t Poloha_Actual, Poloha_Actual_Display; // premenné pre aktuálnu polohu a aktuálnu polohu displeja
extern uint8_t Last_Known_Movement; // 1 hore, 0 dole, 2 stop
extern uint8_t Door_Status; // 1=open, 0=closed
extern uint8_t Motor_Stop_Control, Motor_Up_Control, Motor_Down_Control; // korekcia aby sa neposielali spravy nonstop


#endif /* PREMENNE_H_ */
