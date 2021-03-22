/*
 * CRC8.c
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */

/**
 * @file    CRC8.c
 * @brief   CRC8 kod
 * @details funkcia pre výpočet CRC8
 *
 */
#include <CRC8.h>
/*!
 * zdroj: https://www.devcoons.com/crc8/
 */

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
