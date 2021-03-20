/*
 * Send_Funkcie.h
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */
#include <Definicie.h>

#ifndef SEND_FUNKCIE_H_
#define SEND_FUNKCIE_H_

void Send_ACK(uint8_t Adresa_Prijatej_spravy);
void Motor_Up_Movement();
void Motor_Down_Movement();
void Motor_Stop();
void Door_Open();
void Door_Close();
void Display(uint8_t Direction_Display, uint8_t Floor_Display);
void Rozsviet_LED_IN(uint8_t Stlaceny_Button);
void Rozsviet_LED_OUT(uint8_t Stlaceny_Button);
void Zhasni_LED_OUT(uint8_t Prislusne_Poschodie);
void Zhasni_LED_IN(uint8_t Prislusne_Poschodie);




#endif /* SEND_FUNKCIE_H_ */
