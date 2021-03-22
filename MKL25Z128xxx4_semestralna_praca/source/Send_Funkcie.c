/*
 * Send_Funkcie.c
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */

/**
 * @file    Send_Funkcie.c
 * @brief   odosielacie funkcie
 * @details funkcie pre odosielanie správ do aplikácie výťahu
 *
 */
#include <Send_Funkcie.h>
/*!
 * každá funkcia je v tvare
 * CRC8_Data = dáta pre CRC8 funkciu v tvare podla zadania (Adresa prijímača, Adresa odosielateľa a dáta)
 * veľkosť správy ukladám do premennej CRC8_Data_Length
 * Packet posielanej správy = Start_Byte/ Adresa prijimatela/ Adresa odosielateľa/ veľkosť dát / dáta / CRC8
 * LPSCI_WriteBlocking vpíše dáta do Tx Buffera.
 */



/*!
 * posielanie Acknowligmentu do aplikácie
 */
void Send_ACK(uint8_t Adresa_Prijatej_spravy) {
	uint8_t CRC8_Data[] = { Adresa_Prijatej_spravy, Adresa_MCU };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t CRC8_ACK[] = { Sprava_Ack, Adresa_Prijatej_spravy, Adresa_MCU, 0x00,
			crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, CRC8_ACK, sizeof(CRC8_ACK));
}

/*!
 * https://www.cs.cornell.edu/~tomf/notes/cps104/twoscomp.html
 * pouzity dvojkovy doplnok
 * na jednotlive prevody som pouzil prog kalk.
 * 0000 0000 0000 0000 0000 0000 0110 0100     //dec 100 // up
 * 0 0 0 0 0 0 6 4
 * 1111 1111 1111 1111 1111 1111 1001 1011     //spravena inverzia
 * 1111 1111 1111 1111 1111 1111 1001 1100     // +1
 * F F F F F F 9 C  // FF FF FF 9C            // -100   // down
 */

/*!
 * posielanie správy na pohyb smerom na vrch
 */
void Motor_Up_Movement() {
	uint8_t CRC8_Data[] = { 0xf1, 0x00, 0x02, 0x64, 0x00, 0x00, 0x00 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Move_Up[] = { Sprava_Data, 0xf1, Adresa_MCU, 0x05, 0x02, 0x64, 0x00,
			0x00, 0x00, crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Move_Up, sizeof(Move_Up));
}

/*!
 * smerom na dol
 */
void Motor_Down_Movement() {
	uint8_t CRC8_Data[] = { 0xf1, 0x00, 0x02, 0x9C, 0xFF, 0xFF, 0xFF };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Move_Down[] = { Sprava_Data, 0xf1, Adresa_MCU, 0x05, 0x02, 0x9C,
			0xFF, 0xFF, 0xFF, crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Move_Down, sizeof(Move_Down));
}

/*!
 * zastavenie motora
 */
void Motor_Stop() {
	uint8_t CRC8_Data[] = { 0xf1, 0x00, 0x01 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Stop[] = { Sprava_Data, 0xf1, Adresa_MCU, 0x01, 0x01, crc8(
			CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Stop, sizeof(Stop));
}

/*!
 * otvorenie dverí
 */
void Door_Open() {
	uint8_t CRC8_Data[] = { 0xf0, 0x00, 0x00 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Open[] = { Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x00,
			crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Open, sizeof(Sprava_Door_Open));
}

/*!
 * zatvorenie dverí
 */
void Door_Close() {
	uint8_t CRC8_Data[] = { 0xf0, 0x00, 0x01 };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Sprava_Door_Close[] = { Sprava_Data, 0xf0, Adresa_MCU, 0x01, 0x01,
			crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Sprava_Door_Close,
			sizeof(Sprava_Door_Close));
}


 /*!
  *  parametre predstavujú aktuálny smer (Hore/Dole) a aktuálne poschodie
  *  posielanie údajov na displej
  */
void Display(uint8_t Direction_Display, uint8_t Floor_Display) {
	uint8_t CRC8_Data[] = { 0x30, 0x00, Direction_Display, Floor_Display };
	uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
	uint8_t Info[] = { Sprava_Data, 0x30, Adresa_MCU, 0x02, Direction_Display,
			Floor_Display, crc8(CRC8_Data, CRC8_Data_Length) };
	LPSCI_WriteBlocking(DEMO_LPSCI, Info, sizeof(Info));
}


/*!
 *  rozsvietenie vonkajších LED
 */
void Rozsviet_LED_OUT(uint8_t Stlaceny_Button) {
	if (Stlaceny_Button == 0xc0) { //LED PRIZEMIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x10, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x10, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xc1) { //LED 1. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x11, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x11, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xc2) { //LED 2. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x12, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x12, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xc3) { //LED 3. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x13, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x13, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xc4) { //LED 4. POSCHODIE VONKAJSI TERMINAL
		uint8_t CRC8_Data[] = { 0x14, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x14, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
}

/*!
 * rozsvietenie vnútorných LED
 */
void Rozsviet_LED_IN(uint8_t Stlaceny_Button) {
	if (Stlaceny_Button == 0xb0) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x20, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x20, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xb1) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x21, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x21, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xb2) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x22, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x22, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xb3) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x23, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x23, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Stlaceny_Button == 0xb4) { //prizemna LED VNUTORNY TERMINAL
		uint8_t CRC8_Data[] = { 0x24, 0x00, 0x01 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x24, Adresa_MCU, 0x01, 0x01, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
}

/*!
 * zhasnutie vonkajších LED
 */
void Zhasni_LED_OUT(uint8_t Prislusne_Poschodie) {
	if (Prislusne_Poschodie == 0xe0) {
		uint8_t CRC8_Data[] = { 0x10, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x10, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Prislusne_Poschodie == 0xe1) {
		uint8_t CRC8_Data[] = { 0x11, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x11, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}

	if (Prislusne_Poschodie == 0xe2) {
		uint8_t CRC8_Data[] = { 0x12, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x12, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Prislusne_Poschodie == 0xe3) {
		uint8_t CRC8_Data[] = { 0x13, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x13, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Prislusne_Poschodie == 0xe4) {
		uint8_t CRC8_Data[] = { 0x14, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x14, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
}

/*!
 * zhasnutie vnútorných LED
 */
void Zhasni_LED_IN(uint8_t Prislusne_Poschodie) {
	if (Prislusne_Poschodie == 0xe0) {
		uint8_t CRC8_Data[] = { 0x20, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x20, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Prislusne_Poschodie == 0xe1) {
		uint8_t CRC8_Data[] = { 0x21, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x21, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}

	if (Prislusne_Poschodie == 0xe2) {
		uint8_t CRC8_Data[] = { 0x22, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x22, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Prislusne_Poschodie == 0xe3) {
		uint8_t CRC8_Data[] = { 0x23, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x23, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
	if (Prislusne_Poschodie == 0xe4) {
		uint8_t CRC8_Data[] = { 0x24, 0x00, 0x00 };
		uint8_t CRC8_Data_Length = sizeof(CRC8_Data);
		uint8_t LED[] = { Sprava_Data, 0x24, Adresa_MCU, 0x01, 0x00, crc8(
				CRC8_Data, CRC8_Data_Length) };
		LPSCI_WriteBlocking(DEMO_LPSCI, LED, sizeof(LED));
	}
}
