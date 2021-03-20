/*
 * Delay.c
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */
#include <Delay.h>
// jednoduchá funkcia pre vyvolanie prerušenia
void delay(int Milisekundy) {
	int i = 0;
	while (i <= Milisekundy * 10000) {
		i++;
		__asm("nop");
	}
}

