/*
 * Premenne.c
 *
 *  Created on: 17. 3. 2021
 *      Author: Ado
 */

/**
 * @file    Premenne.c
 * @brief   globalne premenne
 *
 */

#include <Premenne.h>

volatile uint8_t Sprava[10], Index = 0;
volatile uint8_t Sprava_Complete = 0;
uint8_t LED_P_OUT, LED_1_OUT, LED_2_OUT, LED_3_OUT, LED_4_OUT = 0;
uint8_t LED_P_IN, LED_1_IN, LED_2_IN, LED_3_IN, LED_4_IN = 0;
uint8_t Switch_e0, Switch_e1, Switch_e2, Switch_e3, Switch_e4 = 0;
uint8_t Destination, Destination_Display = 0;
uint8_t Poloha_Actual, Poloha_Actual_Display = 0;
uint8_t Last_Known_Movement = 10; // 1 hore, 0 dole, 2 stop
uint8_t Door_Status = 5; // 1=open, 0=closed
uint8_t Motor_Stop_Control, Motor_Up_Control, Motor_Down_Control = 0; // korekcia aby sa neposielali spravy nonstop

