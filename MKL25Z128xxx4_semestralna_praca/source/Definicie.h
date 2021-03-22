/*
 * Definicie.h
 *
 *  Created on: 15. 3. 2021
 *      Author: Ado
 */

/**
 * @file    Definicie.h
 * @brief   kniznica definicii
 * @details V tejto knižnici sú umiestnené základné includy a definície pre prerušenie a UART
 */

#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include <fsl_lpsci.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef DEFINICIE_H_
#define DEFINICIE_H_



#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)
#define DEMO_LPSCI_IRQn UART0_IRQn
#define DEMO_LPSCI_IRQHandler UART0_IRQHandler

#define Sprava_Ack 0xa1
#define Sprava_Data 0xa0
#define Adresa_MCU 0x00


#endif /* DEFINICIE_H_ */
