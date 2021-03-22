/*
 * Delay.c
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */

/**
 * @file    Delay.c
 * @brief   Delay funkcia
 * @details jednoduchá funkcia pre vyvolanie oneskorenia
 */

/*!

 */
#include <Delay.h>

void delay(int Milisekundy) {
	int i = 0;
	while (i <= Milisekundy * 10000) {
		i++;
		__asm("nop");
	}
}

